// ********************************************************************
// *                                                                  *
// * This file is based on code from the TOPAS-nBio extensions to the *
// * TOPAS Simulation Toolkit.                                        *
// * The TOPAS-nBio extensions are freely available under the license *
// * agreement set forth at: https://topas-nbio.readthedocs.io/       *
// *                                                                  *
// ********************************************************************
// *  Modifications by Marc B. Hahn (2022)                            *
// *  Please report bugs to hahn@physiks.fu-berlin.de                 *
// *  or on https://github.com/MarcBHahn/TOPAS-bioSAXS-dosimetry           *
// ********************************************************************
//
// Tuple scorer for scoring energy deposited in capillaries

#ifndef TotalEnergyDepositScorer_hh
#define TotalEnergyDepositScorer_hh

#include "TsVNtupleScorer.hh"
#include "G4VProcess.hh"

class TotalEnergyDepositScorer : public TsVNtupleScorer
{
public:
    TotalEnergyDepositScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
                G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer);
    
    virtual ~TotalEnergyDepositScorer();

    G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    void UserHookForEndOfRun();

private:
    static G4float energyCapillaryWall;
    static G4float energyCapillaryLiquid;
    static G4float energyWholeCapillary;
    static G4float energyCombinedTargetSpheres;

    static std::vector<std::vector<G4double> > EnergyDeposit;
    static std::vector<G4double> storedResults;


    // Output variables
    G4float fEnergyDep;
    G4float fDoseDep;
    G4float fEvents;
    G4String fVolName;
    
    
    
    //other vars
    G4Track* aTrack;
    G4String volumeName;
    G4double flagEnergyDep; 
    
};
#endif
