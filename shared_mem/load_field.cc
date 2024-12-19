#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;



std::vector<double> stringToDoubleArray(const std::string& str) {
    std::vector<double> result;
    std::istringstream iss(str); // Create a string stream
    double number;
    
    // Read numbers from the stream into the vector
    while (iss >> number) {
        result.push_back(number);
        // Skip any delimiter if necessary (e.g., comma)
        if (iss.peek() == '\t') {
            iss.ignore();
        }
    }
    
    return result;
}


int main() {
    const char* shm_name = "/my_3d_shared_mem_with_arrays"; // Name of shared mem

    // Array size for mag. field
    const size_t X = 601;
    const size_t Y = 601;
    const size_t Z = 701;
    const size_t ArraySize = 3;

    double full_size = X * Y * Z;

    // Total size of shared mem
    size_t pointer_size = X * sizeof(double***);
    pointer_size += X * Y * sizeof(double**);
    pointer_size += X * Y * Z * sizeof(double*);
    size_t data_size = X * Y * Z * ArraySize * sizeof(double);
    size_t total_size = pointer_size + data_size;

    //Obj. of shared mem
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    //Set mem to shared mem
    if (ftruncate(shm_fd, total_size) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return 1;
    }

    //Mem mapping
    void* base_ptr = mmap(0, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (base_ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return 1;
    }

    //Init field array
    double**** array3D = reinterpret_cast<double****>(base_ptr);
    double*** y_pointers = reinterpret_cast<double***>(array3D + X);
    double** z_pointers = reinterpret_cast<double**>(y_pointers + X * Y);
    double* data = reinterpret_cast<double*>(z_pointers + X * Y * Z);

    for (size_t i = 0; i < X; ++i) {
        array3D[i] = &y_pointers[i * Y];
        for (size_t j = 0; j < Y; ++j) {
            array3D[i][j] = &z_pointers[(i * Y + j) * Z];
            for (size_t k = 0; k < Z; ++k) {
                array3D[i][j][k] = &data[((i * Y * Z + j * Z + k) * ArraySize)];
            }
        }
    }

    //Set field data to array

    ifstream file("/home/alion/simulation/BMN/Ndet/shared_mem/Data.txt"); //!!! Data field

    if (!file.is_open())
    {
        cout << "File open error" << endl;
    }
    else
    {
        cout << "File is open" << endl;
        string line;

        int i = 0;
        int p1 = 1;

        while (getline(file, line)) 
        {
            double percent = (i / full_size) * 100.0;

            if (percent > p1)
            {
                cout << percent << " " << "%" << endl;
                p1++;
            }

            if (i > 0)
            {
                std::vector<double> numbers = stringToDoubleArray(line);

                if (!line.empty())
                {
                    int xi = int(numbers[0] * 100);
                    int yi = int(numbers[1] * 100);
                    int zi = int(numbers[2] * 100);

                    array3D[xi][yi][zi][0] = static_cast<double>(numbers[3]);
                    array3D[xi][yi][zi][1] = static_cast<double>(numbers[4]);
                    array3D[xi][yi][zi][2] = static_cast<double>(numbers[5]);
                }           
            }
            i++;
        }
    }


    std::cout << "3D array with arrays initialized in shared memory." << std::endl;

    // Освобождаем ресурсы
    if (munmap(base_ptr, total_size) == -1) {
        perror("munmap");
        return 1;
    }

    if (close(shm_fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}