//
//
// ********************************************************************
// * This file is intended to be used with the TOPAS-nBio extensions to the TOPAS Simulation Toolkit.  *
// * The TOPAS-nBio extensions are freely available under the license *
// * agreement set forth at: https://topas-nbio.readthedocs.io/       *
// *                                                                  *              
// *  Extended by Marc B. Hahn (2022)                                 *
// *  Please report bugs to marc-benjamin.hahn@fu-berlin.de                  *
// *  or on https://github.com/MarcBHahn/TOPAS-bioSAXS-dosimetry           *    
// ********************************************************************
//
// A simple model to generate capillarties and target spheres to simulate the microscopic energy deposit during bioSAXS experiments
// If you use this extension please cite the following literature:
// Hahn et al. Phys. Rev. E 95, 052419 https://journals.aps.org/pre/abstract/10.1103/PhysRevE.95.052419


#ifndef TsBioSAXSdosimetry_hh
#define TsBioSAXSdosimetry_hh

#include "TsVGeometryComponent.hh"
#include <vector>


class TsBioSAXSdosimetry : public TsVGeometryComponent
{    
public:
	TsBioSAXSdosimetry(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
				  TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~TsBioSAXSdosimetry();
	
	G4VPhysicalVolume* Construct();
    
    void ResolveParameters();
    
    
private:
    
    G4double CapillaryOuterRadius;
    G4double CapillaryInnerRadius;
    G4double CapillaryLength;
    G4int NumberOfTargets;
    G4double TargetRadius;

    G4RotationMatrix* rotationMatrix;
    
    std::vector<std::vector<G4double> >  TargetCoordinates;
    std::vector<G4double>  tmpCoordinates;
    
    G4bool CheckOverlapOfSphereWithComponents(std::vector<std::vector<G4double> >& Coordinates, G4double r, G4double x, G4double y, G4double z);
    
    G4ThreeVector* AddTargetSphereToCapillary();
    void AddCoordinates(std::vector<std::vector<G4double> >& Coordinates, G4double r, G4double x, G4double y, G4double z);   

    
};

#endif
