#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "vector"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"


using namespace std;

class PrimaryParticles : public G4VUserPrimaryGeneratorAction{
    public:

        PrimaryParticles();
        ~PrimaryParticles();

        G4ParticleTable* particleTable;

        virtual void GeneratePrimaries(G4Event* event);
        
        int ac = 0;
        int n = 1;



    private:

        G4ParticleGun* fParticleGun;
        
};
