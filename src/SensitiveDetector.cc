#include "SensitiveDetector.hh"
#include <G4Step.hh>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <G4SystemOfUnits.hh>
#include <thread>
#include <mutex>
#include "Randomize.hh"
#include "G4RunManager.hh"


using namespace std;



SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory*)
{


	// G4StepPoint* point1 = step->GetPreStepPoint();
	// G4ThreeVector pos1 = point1->GetPosition();

	// const G4AffineTransform transformation1 = point1->GetTouchable()->GetHistory()->GetTopTransform();
	// G4ThreeVector localPosition1 = transformation1.TransformPoint(pos1);

	// double x = localPosition1.x()/cm;
	// double y = localPosition1.y()/cm;
	// double z = localPosition1.z()/cm;


	// G4String p_name = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
	// evID = G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID();

	// G4StepPoint* prePoint = step->GetPreStepPoint();
	// G4TouchableHandle touch1 = prePoint->GetTouchableHandle();
	// G4VPhysicalVolume* volume = touch1->GetVolume();
	// G4String vname = volume->GetName();

    
    return true;

}



SensitiveDetector::~SensitiveDetector()
{

}
