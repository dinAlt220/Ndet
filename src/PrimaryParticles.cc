#include "PrimaryParticles.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "time.h"
#include "G4Geantino.hh"
#include "G4IonTable.hh"
#include "G4ChargedGeantino.hh"
#include "G4NuclideTable.hh"
#include "G4VIsotopeTable.hh"
#include "G4Ions.hh"
#include "vector"
#include "string"
#include <fstream>
#include <iostream>
#include <vector>
#include "Randomize.hh"

using namespace std;


PrimaryParticles::PrimaryParticles(){

    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);

    auto particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("proton");
    fParticleGun->SetParticleDefinition(particleDefinition);

}


void PrimaryParticles::GeneratePrimaries(G4Event* event){


    int seed = clock(  );
    double a = -(1000000)+2*1000000*G4UniformRand();
    int par = G4Threading::G4GetThreadId() + a;
    G4Random::setTheSeed(seed, par);



    if (ac == 10000*n)
    {    
        n++;
       //cout << ac << endl;     
    }

    // double phi =  2 * M_PI * G4UniformRand();
    // double costheta = -1.0 + 2*1.0 * G4UniformRand();
    // double theta =  acos(costheta);

    double r = 2.5 * sqrt(G4UniformRand());
    double theta1 = G4UniformRand() * 2.0 * M_PI;
    double x =  r * cos(theta1);
    double y =  r * sin(theta1);


    // double xv = sin(theta)*cos(phi);
    // double yv = sin(theta)*sin(phi);
    // double zv = cos(theta);

    // double z = 3.0 + 1.0 * G4UniformRand();

    // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xv, yv, zv));
    // fParticleGun->SetParticleEnergy(5000.0*MeV);
    // fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, z*m));


    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    fParticleGun->SetParticleEnergy(5000.0*MeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, -10*m));

    fParticleGun->GeneratePrimaryVertex(event); 


    

}



PrimaryParticles::~PrimaryParticles(){

    delete fParticleGun;

}
