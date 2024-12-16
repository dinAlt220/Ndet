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

#include "G4MagneticField.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class MyMagField : public G4MagneticField
{
  public:

    MyMagField();
   ~MyMagField() override;

    void GetFieldValue(const G4double yTrack[],
                             G4double B[]     ) const override;
    G4Field* Clone() const override;


    int shm_fd;
    void* base_ptr;
    double**** array3D;
    size_t total_size;

    G4RotationMatrix* fpMatrix;
};