

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"


#include <G4Step.hh>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include <G4SystemOfUnits.hh>
#include <thread>
#include <mutex>
#include <G4VProcess.hh>

using namespace std;

SteppingAction::SteppingAction(
                      const DetectorConstruction* detectorConstruction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction)
{}



void SteppingAction::UserSteppingAction(const G4Step* step)
{


}



SteppingAction::~SteppingAction()
{

}


