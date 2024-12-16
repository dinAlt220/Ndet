#include <sys/mman.h>
#include <iostream>

int main() {
    const char* shm_name = "/my_3d_shared_mem_with_arrays";

    if (shm_unlink(shm_name) == -1) {
        perror("shm_unlink");
        return 1;
    }

    std::cout << "Shared memory unlinked successfully." << std::endl;
    return 0;
}