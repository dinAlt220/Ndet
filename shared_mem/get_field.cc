#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    const char* shm_name = "/my_3d_shared_mem_with_arrays"; // Name of shared mem

   // Array size for mag. field
    const size_t X = 601;
    const size_t Y = 601;
    const size_t Z = 701;
    const size_t ArraySize = 3;

    // Total size of shared mem
    size_t pointer_size = X * sizeof(double***);    
    pointer_size += X * Y * sizeof(double**);   
    pointer_size += X * Y * Z * sizeof(double*);
    size_t data_size = X * Y * Z * ArraySize * sizeof(double);
    size_t total_size = pointer_size + data_size;

     //Obj. of shared mem
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    //Set mem to shared mem
    void* base_ptr = mmap(0, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (base_ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return 1;
    }

    // Reconstruct pointers
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

    // Read data
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            for (size_t k = 0; k < 3; ++k) {
                std::cout << "array3D[" << i << "][" << j << "][" << k << "] = {";
                for (size_t m = 0; m < ArraySize; ++m) {
                    std::cout << array3D[i][j][k][m];
                    if (m < ArraySize - 1) std::cout << ", ";
                }
                std::cout << "}" << std::endl;
            }
        }
    }

    // Clear resourses
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