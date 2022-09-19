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
// Tuple scorer for scoring energy deposited in the cell organelles

#ifndef TargetScorer_hh
#define TargetScorer_hh

#include "TsVNtupleScorer.hh"
#include "G4VProcess.hh"

class TargetScorer : public TsVNtupleScorer
{
public:
    TargetScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, TsExtensionManager* eM,
                G4String scorerName, G4String quantity, G4String outFileName, G4bool isSubScorer);
    
    virtual ~TargetScorer();

    G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    
    

    
protected:
    // Output variables
    G4float fEnergyDep;
    G4String fVolName;
};
#endif
