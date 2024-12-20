#include "DetectorConstruction.hh"
#include <FTFP_BERT_HP.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include "Action.hh"
#include "G4UIcommand.hh"
#include "Randomize.hh"
#include <QGSP_BIC.hh>
#include "G4RadioactiveDecayPhysics.hh"
#include <G4EmLivermorePhysics.hh>
#include "G4RunManager.hh"
#include <unistd.h>
#include "time.h"
#include "G4EmLivermorePhysics.hh"
#include "MyParallelWorld.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4VisExecutive.hh"


// #include "/home/alion/simulation/BMN/bmnroot/macro/run/geometry_run/geometry_run8.C"


using namespace std;

int main(int argc,char** argv)
{


    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    G4RunManager *run = new G4RunManager;

    DetectorConstruction *geom = new DetectorConstruction();

    // G4String paraWorldName = "ParallelWorld";
    // geom->RegisterParallelWorld(new MyParallelWorld(paraWorldName));

    run->SetUserInitialization(geom);

    FTFP_BERT_HP *phys = new FTFP_BERT_HP;
    phys->RegisterPhysics(new G4RadioactiveDecayPhysics());
    // phys->RegisterPhysics(new G4ParallelWorldPhysics(paraWorldName));
    run->SetUserInitialization(phys);


    Action *act = new Action;
    run->SetUserInitialization(act);

    G4VisExecutive *vis = new G4VisExecutive;
    vis->Initialize();

    run->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);

    UImanager->ApplyCommand("/control/verbose 2");
    UImanager->ApplyCommand("/run/verbose 2");
    UImanager->ApplyCommand("/run/setCut 10 mm");

    //UImanager->ApplyCommand("/run/setCutForAGivenParticle neutron 0.001 mm");
    //UImanager->ApplyCommand("/cuts/setLowEdge 7 MeV");
    
    bool lowelectro = false;
    
    if (lowelectro == true){

        UImanager->ApplyCommand("/process/em/fluo true");
        UImanager->ApplyCommand("/process/em/auger true");
        UImanager->ApplyCommand("/process/em/augerCascade true");
        UImanager->ApplyCommand("/process/em/pixe true");
        UImanager->ApplyCommand("/process/em/pixeElecXSmodel Penelope");
    
    }
    
    UImanager->ApplyCommand("/run/initialize");

    bool visual = true;
    
    if (visual == true){

        UImanager->ApplyCommand("/vis/verbose 2");
        UImanager->ApplyCommand("/tracking/verbose 0");
        UImanager->ApplyCommand("/vis/scene/create");
        UImanager->ApplyCommand("/vis/open VRML2FILE");
        UImanager->ApplyCommand("/vis/drawVolume");

        UImanager->ApplyCommand("/vis/drawVolume worlds");
        // UImanager->ApplyCommand("/vis/filtering/trajectories/create/particleFilter");
        // UImanager->ApplyCommand("/vis/filtering/trajectories/particleFilter-0/add mu-");
        // UImanager->ApplyCommand("/vis/filtering/trajectories/particleFilter-0/add mu+");

        UImanager->ApplyCommand("/vis/set/extentForField");
        // UImanager->ApplyCommand("/vis/viewer/set/style surface");
        UImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90. 90");
        UImanager->ApplyCommand("/vis/scene/add/trajectories");
        UImanager->ApplyCommand("/vis/scene/add/hits");
        UImanager->ApplyCommand("/vis/modeling/trajectories/create/drawByCharge");
        UImanager->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2");
        UImanager->ApplyCommand("/vis/scene/add/magneticField");
        UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
        UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
    }

    G4int seed1 = clock(  );
    G4double a1 = -(1000000)+2*1000000*G4UniformRand();
    int par1 = G4Threading::G4GetThreadId() + a1;
    G4Random::setTheSeed( seed1, par1);

    int number = 100;
    //int number = 10000+0*G4UniformRand();
    UImanager->ApplyCommand("/run/beamOn " + std::to_string(number));
    
    delete ui;
    delete vis;
    delete run;

}
