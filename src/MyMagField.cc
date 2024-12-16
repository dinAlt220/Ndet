// -------------------------------------------------------------------
// MyMagField
//
// Class description:
//
// Class for creation of magnetic field for data file 
// shm_fd - obj. of shared mem
// base_ptr - pointer to shared mem
// array3D - array [601, 601, 701, 3] {x,y,z, [Bx, By,Bz]}
// total_size - total size of shared mem
// Then the magnetic field components are:
//   Bx = array3D[x][y][z][0] ,
//   By = array3D[x][y][z][1] ,
//   Bz = array3D[x][y][z][2] .
// Here x,y,z are the coordinates of a space point of interest.

// 16.12.2024, A.V.Skobliakov - Created
// -------------------------------------------------------------------


#include "MyMagField.hh"
#include "G4RotationMatrix.hh"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>  
#include <cstring>
#include <iostream>  
#include <G4SystemOfUnits.hh>


// -------------------------------------------------------------------

MyMagField::MyMagField()
{

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
   total_size = pointer_size + data_size;

   //Obj. of shared mem
   shm_fd = shm_open(shm_name, O_RDWR, 0666);

   //Set mem to shared mem
   base_ptr = mmap(0, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

   // Reconstruct pointers
   array3D = reinterpret_cast<double****>(base_ptr);
   double*** y_pointers = reinterpret_cast<double***>(array3D + X);
   double** z_pointers = reinterpret_cast<double**>(y_pointers + X * Y);
   double* data = reinterpret_cast<double*>(z_pointers + X * Y * Z);

   // Set data field to array "array3D"
   for (size_t i = 0; i < X; ++i)
   {
     array3D[i] = &y_pointers[i * Y];
     for (size_t j = 0; j < Y; ++j)
     {
         array3D[i][j] = &z_pointers[(i * Y + j) * Z];
         for (size_t k = 0; k < Z; ++k)
         {
             array3D[i][j][k] = &data[((i * Y * Z + j * Z + k) * ArraySize)];
         }
     }
   }

}

// -------------------------------------------------------------------

G4Field* MyMagField::Clone() const
{
   return new MyMagField();
}

// -------------------------------------------------------------------

MyMagField::~MyMagField()
{
   munmap(base_ptr, total_size);
   close(shm_fd);
}

// -------------------------------------------------------------------

void MyMagField::GetFieldValue( const G4double y[],         // [7]
                                                G4double B[]  ) const // [3]
{

   int xi = int(y[0] / 10);
   int yi = int(y[1] / 10);
   int zi = int(y[2] / 10);

   // std::cout << array3D[0][1][2][2] << std::endl;

   // std::cout << xi << "\t" << yi << "\t" << zi << std::endl;

   //x > 0, y > 0
   if (xi <= 600 and xi >= 0 and yi <= 600 and yi >=0 and zi <= 700 and zi >= 0) 
   {
      B[0] = array3D[xi][yi][zi][0] * tesla;
      B[1] = array3D[xi][yi][zi][1] * tesla;
      B[2] = array3D[xi][yi][zi][2] * tesla;
   }
   //x < 0, y > 0
   else
   {
      if (xi >= -600 and xi <= 0 and yi <= 600 and yi >=0 and zi <= 700 and zi >= 0) 
      {

         B[0] = array3D[-xi][yi][zi][0] * tesla;
         B[1] = -array3D[-xi][yi][zi][1] * tesla;
         B[2] = array3D[-xi][yi][zi][2] * tesla;
      }
      else
      {
         //x < 0, y < 0
         if (xi >= -600 and xi <= 0 and yi >= -600 and yi <=0 and zi <= 700 and zi >= 0) 
         {

            B[0] = -array3D[-xi][-yi][zi][0] * tesla;
            B[1] = -array3D[-xi][-yi][zi][1] * tesla;
            B[2] = array3D[-xi][-yi][zi][2] * tesla;

         }
         else
         {
            //x > 0, y < 0
            if (xi <= 600 and xi >= 0 and yi >= -600 and yi <=0 and zi <= 700 and zi >= 0) 
            {

               B[0] = -array3D[xi][-yi][zi][0] * tesla;
               B[1] = array3D[xi][-yi][zi][1] * tesla;
               B[2] = array3D[xi][-yi][zi][2] * tesla;
            }
            else
            {
               B[0] = 0;
               B[1] = 0;
               B[2] = 0;
            }
         }
      }
   }



}