#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include <G4SystemOfUnits.hh>
#include <G4PVReplica.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <G4Tubs.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>
#include <G4Cons.hh>
#include <G4Orb.hh>
#include <G4EllipticalTube.hh>
#include <G4Transform3D.hh>
#include <G4Trd.hh>
#include <G4UnionSolid.hh>
#include <G4PropagatorInField.hh>
#include <G4UniformMagField.hh>
#include <G4Mag_UsualEqRhs.hh>
#include <G4HelixHeum.hh>
#include <G4MagIntegratorDriver.hh>
#include <G4ChordFinder.hh>
#include <G4FieldManager.hh>
#include <G4QuadrupoleMagField.hh>
#include <G4RotationMatrix.hh>
#include <G4GlobalMagFieldMessenger.hh>
#include <G4MagIntegratorStepper.hh>
#include <G4ClassicalRK4.hh>
#include <globals.hh>
#include <G4SubtractionSolid.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SubtractionSolid.hh>
#include <G4UniformElectricField.hh>
#include <G4EqMagElectricField.hh>
#include <G4TransportationManager.hh>
#include "G4Sphere.hh"
#include <G4VSolid.hh>
#include "CADMesh.hh"
#include <iostream>
#include <vector>
#include <string>
#include "G4AutoDelete.hh"
#include "G4LogicalVolumeStore.hh"

#include "MyMagField.hh"


using namespace std;

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{

    G4double size = 30*m;

    nist = G4NistManager::Instance();
    world_mat = nist->FindOrBuildMaterial("G4_Galactic");

    world = new G4Box("world", size/2, size/2, size/2);
    log_world = new G4LogicalVolume(world, world_mat, "world_log");
    phys_world = new G4PVPlacement(0, G4ThreeVector(), log_world, "1001", 0, false, 0);

}

DetectorConstruction::~DetectorConstruction()
{
    
    delete world;
    delete log_world;
    delete phys_world;

}


G4VPhysicalVolume* DetectorConstruction::Construct()
{



    //--------- Material definition ---------
  
    G4double a, z, density;

    //Liquid Hydrogen
    G4Material* LH2 = new G4Material("Hydrogen", z=2., a=2.02*g/mole,  density=0.07*g/cm3, kStateGas,3*kelvin,1.7e5*pascal);


    G4Material *vac = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material *concrete = nist->FindOrBuildMaterial("G4_CONCRETE");
    G4Material *Pb = nist->FindOrBuildMaterial("G4_Pb");



    G4Tubs* target = new G4Tubs("target", 0*mm, 5*mm, 25*mm, 0*deg, 360*deg);
    G4LogicalVolume *log_target = new G4LogicalVolume(target, LH2, "log_target");
    new G4PVPlacement(0, G4ThreeVector(0.0*m, 0.0*m, 3.5*m), log_target,
                                                "target", log_world, true, 0);






    log_world->SetVisAttributes (G4VisAttributes::GetInvisible());

    return phys_world;

}


void DetectorConstruction::ConstructSDandField()
{


    // MAGNETIC FIELD SETTING
    G4MagneticField *magF = new MyMagField();

    G4FieldManager* fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(magF);
    fieldMgr->CreateChordFinder(magF);

    //OPTIONAL (magnetic field to logic volume)
    // G4FieldManager *fieldMgr = new G4FieldManager(magF);
    // log_tube->SetFieldManager(fieldMgr, true);


    // G4SDManager* SDman = G4SDManager::GetSDMpointer();
    // SensitiveDetector *detector = new SensitiveDetector("detector");
    // SDman->AddNewDetector(detector);
    // SetSensitiveDetector("log_sample", detector);

}



