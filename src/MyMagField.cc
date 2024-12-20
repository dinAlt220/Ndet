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
// 19.12.2024, A.V.Skobliakov - Added interpolation to field as parameter (computation time increses)
// -------------------------------------------------------------------


#include "MyMagField.hh"
#include "G4RotationMatrix.hh"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>  
#include <cstring>
#include <iostream>  
#include <G4SystemOfUnits.hh>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

#include "ThreeDLookup.hh"


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


double getF(double**** array3D ,double x, double y, double z, int v)
{

   int xi = int(round(x));
   int yi = int(round(y));
   int zi = int(round(z));

   // std::cout << xi << "\t" << yi << "\t" << zi << std::endl;

   std::vector< std::vector< std::vector<double> > > dataTableB;

   for (auto i = xi-1; i < xi+1; ++i) 
   {
      std::vector< std::vector<double> > tmpiB;
      for (auto j = yi-1; j < yi+1; ++j)
      {
         std::vector<double> tmpjB;
         for (auto k = zi-1; k < zi+1; ++k)
         {
            tmpjB.push_back( array3D[i][j][k][v] );
         }
         tmpiB.push_back(tmpjB);
      }
      dataTableB.push_back(tmpiB);
   }


   std::vector<double> x1, y1, z1;

   for (auto i = xi-1; i < xi+1; ++i)
   {
      x1.push_back(i);
   }

   for (auto i = yi-1; i < yi+1; ++i)
   {
      y1.push_back(i);
   }

   for (auto i = zi-1; i < zi+1; ++i)
   {
      z1.push_back(i);
   }

   ThreeDLookup lut3dB(x1, y1, z1, dataTableB);

   double value = lut3dB.Interp(x, y, z);

   // std::cout << xi << "\t" << yi << "\t" << zi << std::endl;
   // std::cout << "intrt " << value << std::endl;
   // std::cout << array3D[xi][yi][zi][v] << std::endl;

   return value;


}




void MyMagField::GetFieldValue( const G4double y[],         // [7]
                                                G4double B[]  ) const // [3]
{


   // double Bx1, By1, Bz1, Bx2, By2, Bz2;

   int xi = int(round(y[0] / 10));
   int yi = int(round(y[1] / 10));
   int zi = int(round(y[2] / 10));

   double xd = y[0] / 10;
   double yd = y[1] / 10;
   double zd = y[2] / 10;

   // std::cout << xi << "\t" << yi << "\t" << zi << std::endl;
   // std::cout << xd << "\t" << yd << "\t" << zd << std::endl;

   // std::cout << array3D[0][1][2][2] << std::endl;

   // std::cout << xi << "\t" << yi << "\t" << zi << std::endl;

   //x > 0, y > 0
   if (xi <= 599 and xi >= 1 and yi <= 599 and yi >=1 and zi <= 699 and zi >= 1) 
   {


      if (Interpolation == true)
      {
         double Bx1 = getF(array3D, xd, yd, zd, 0) * tesla;
         double By1 = getF(array3D, xd, yd, zd, 1) * tesla;
         double Bz1 = getF(array3D, xd, yd, zd, 2) * tesla;

         B[0] = Bx1;
         B[1] = By1;
         B[2] = Bz1;

         // std::cout << "Inter " << B[0] << "\t" << B[1] << "\t" << B[2] << std::endl;
      }
      else
      {
         double Bx2 = array3D[xi][yi][zi][0] * tesla;
         double By2 = array3D[xi][yi][zi][1] * tesla;
         double Bz2 = array3D[xi][yi][zi][2] * tesla;

         // std::cout << "Inter " << Bx2 << "\t" << By2 << "\t" << Bz2 << std::endl;


         B[0] = Bx2;
         B[1] = By2;
         B[2] = Bz2;
      }

      // std::cout << "Inter " << Bx1 - Bx2 << "\t" << By1 - By2 << "\t" << Bz1 - Bz2 << std::endl;
      // std::cout << xi - xd << "\t" << yi - yd << "\t" << zi - zd << std::endl;
      // std::cout << Bx2 << "\t" << By2 << "\t" << Bz2 << std::endl;

   }
   //x < 0, y > 0
   else
   {
      if (xi >= -599 and xi <= -1 and yi <= 599 and yi >=1 and zi <= 699 and zi >= 1) 
      {

         if (Interpolation == true)
         {
            double Bx1 = getF(array3D, -xd, yd, zd, 0) * tesla;
            double By1 = -getF(array3D, -xd, yd, zd, 1) * tesla;
            double Bz1 = getF(array3D, -xd, yd, zd, 2) * tesla;

            B[0] = Bx1;
            B[1] = By1;
            B[2] = Bz1;
         }
         else
         {
            double Bx2 = array3D[-xi][yi][zi][0] * tesla;
            double By2 = -array3D[-xi][yi][zi][1] * tesla;
            double Bz2 = array3D[-xi][yi][zi][2] * tesla;

            B[0] = Bx2;
            B[1] = By2;
            B[2] = Bz2;
         }


         // std::cout << "Inter " << Bx1 << "\t" << By1 << "\t" << Bz1 << std::endl;
         // std::cout << Bx2 << "\t" << By2 << "\t" << Bz2 << std::endl;
      }
      else
      {
         //x < 0, y < 0
         if (xi >= -599 and xi <= -1 and yi >= -599 and yi <=-1 and zi <= 699 and zi >= 1) 
         {


            if (Interpolation == true)
            {
               double Bx1 = -getF(array3D, -xd, -yd, zd, 0) * tesla;
               double By1 = -getF(array3D, -xd, -yd, zd, 1) * tesla;
               double Bz1 = getF(array3D, -xd, -yd, zd, 2) * tesla;

               B[0] = Bx1;
               B[1] = By1;
               B[2] = Bz1;
            }
            else
            {
               double Bx2 = -array3D[-xi][-yi][zi][0] * tesla;
               double By2 = -array3D[-xi][-yi][zi][1] * tesla;
               double Bz2 = array3D[-xi][-yi][zi][2] * tesla;

               B[0] = Bx2;
               B[1] = By2;
               B[2] = Bz2;
            }

            // std::cout << "Inter " << Bx1 << "\t" << By1 << "\t" << Bz1 << std::endl;
            // std::cout << Bx2 << "\t" << By2 << "\t" << Bz2 << std::endl;

         }
         else
         {
            //x > 0, y < 0
            if (xi <= 599 and xi >= 1 and yi >= -599 and yi <=-1 and zi <= 699 and zi >= 1) 
            {

               if (Interpolation == true)
               {
                  double Bx1 = -getF(array3D, xd, -yd, zd, 0) * tesla;
                  double By1 = getF(array3D, xd, -yd, zd, 1) * tesla;
                  double Bz1 = getF(array3D, xd, -yd, zd, 2) * tesla;

                  B[0] = Bx1;
                  B[1] = By1;
                  B[2] = Bz1;
               }
               else
               {
                  double Bx2 = -array3D[xi][-yi][zi][0] * tesla;
                  double By2 = array3D[xi][-yi][zi][1] * tesla;
                  double Bz2 = array3D[xi][-yi][zi][2] * tesla;

                  B[0] = Bx2;
                  B[1] = By2;
                  B[2] = Bz2;
               }     

               // std::cout << "Inter " << Bx1 << "\t" << By1 << "\t" << Bz1 << std::endl;
               // std::cout << Bx2 << "\t" << By2 << "\t" << Bz2 << std::endl;

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