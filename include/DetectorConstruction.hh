#include <G4VUserDetectorConstruction.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4AssemblyVolume.hh>
#include <G4SubtractionSolid.hh>
#include <string>

using namespace std;

class DetectorConstruction : public G4VUserDetectorConstruction{
   
    public:

        DetectorConstruction();
        ~DetectorConstruction();

    private:

        G4Box* world;
        G4LogicalVolume* log_world;
        G4VPhysicalVolume* phys_world;

        // G4VPhysicalVolume* phys_world1;

        G4NistManager* nist;
        G4Material* world_mat;


        G4LogicalVolume *log_tube;

        G4VPhysicalVolume* Construct() override;
        virtual void ConstructSDandField();


};
