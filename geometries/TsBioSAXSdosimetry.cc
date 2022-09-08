// Component for TsBioSAXSdosimetry
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

#include "TsBioSAXSdosimetry.hh"

#include "TsParameterManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

TsBioSAXSdosimetry::TsBioSAXSdosimetry(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM, TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
  
    tmpCoordinates.resize(4);
    TargetCoordinates.resize(0);
    ResolveParameters();

   
}


TsBioSAXSdosimetry::~TsBioSAXSdosimetry()
{;}

void TsBioSAXSdosimetry::ResolveParameters() {
    
    CapillaryOuterRadius = fPm->GetDoubleParameter(GetFullParmName("OuterRadius"), "Length");
    CapillaryInnerRadius  = fPm->GetDoubleParameter(GetFullParmName("InnerRadius"), "Length");
    CapillaryLength  = fPm->GetDoubleParameter(GetFullParmName("Length"), "Length");

    TargetRadius  = fPm->GetDoubleParameter(GetFullParmName("TargetRadius"), "Length");
    NumberOfTargets  = fPm->GetIntegerParameter(GetFullParmName("Targets"));
    
    G4String parameterName = "Ge/Capillary/Sphere/IsParallel"; 
    if(fPm->ParameterExists(parameterName)){
        TargetSpheresAreParallelComponents = fPm->GetBooleanParameter(parameterName);
        G4cout<<"Target spheres are scored in a parallel world."<<G4endl;

    }
    else{
        G4cout<<"Target spheres are scored normally."<<G4endl;
        TargetSpheresAreParallelComponents = false;
    }
}


G4VPhysicalVolume* TsBioSAXSdosimetry::Construct()
{
	BeginConstruction();
    
    //***********************************************************************
    //              Envelope Geometry : Capillary 
    //***********************************************************************
    // parameters name, inner radius, outer radius, z half length, starting phi, segment nagle
    G4Tubs* gCapillary = new G4Tubs ("Capillary", 0.0, CapillaryOuterRadius, CapillaryLength/2.0 , 0.0, CLHEP::twopi);
    position = new G4ThreeVector(0.0,0.0,0.0);
    rotationMatrix = new G4RotationMatrix();

    fEnvelopeLog = CreateLogicalVolume(gCapillary);
    fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);


    //*******************************
    // Subcomponent: CapillaryWall
    //*******************************
    G4Tubs* gWall = new G4Tubs ("Wall", CapillaryInnerRadius, CapillaryOuterRadius, CapillaryLength/2.0 , 0.0, CLHEP::twopi);
    G4LogicalVolume* lWall = CreateLogicalVolume("Wall", gWall);
    G4VPhysicalVolume* pWall = CreatePhysicalVolume("Wall", lWall, rotationMatrix, position, fEnvelopePhys);


    //*******************************
    // Subcomponent: CapillaryLiquid
    //*******************************
    //G4Tubs* gCapillaryLiquid = new G4Tubs ("CapillaryLiquid", 0.0, CapillaryInnerRadius, CapillaryLength/2.0 , 0.0, CLHEP::twopi);
    //G4LogicalVolume* lCapillaryLiquid = CreateLogicalVolume("CapillaryLiquid", gCapillaryLiquid);
    //G4VPhysicalVolume* pCapillaryLiquid = CreatePhysicalVolume("CapillaryLiquid",  lCapillaryLiquid, rotationMatrix, position, fEnvelopePhys);



    //*******************************
    // Subcomponent: TargetSphere
    //*******************************
        
    G4Orb* gTargetSphere = new G4Orb("Sphere", TargetRadius);
    G4LogicalVolume* lTargetSphere = CreateLogicalVolume("Sphere", gTargetSphere);
        
    //Randomly distribute TargetSpheres throughout the capillary volume
    for (int m = 0; m < NumberOfTargets; m++){
            
        G4cout << "** Add Sphere  " << m <<  " **" << G4endl;

            G4VPhysicalVolume* pTargetSphere = CreatePhysicalVolume("Sphere", m, true, lTargetSphere, rotationMatrix, AddTargetSphereToCapillary(), fEnvelopePhys);
        }
    

    G4cout << "*** Total target spheres in capillary  : " << TargetCoordinates.size() <<" ***" <<G4endl;
    InstantiateChildren(fEnvelopePhys);
	return fEnvelopePhys;
}


G4ThreeVector* TsBioSAXSdosimetry::AddTargetSphereToCapillary(){
    
    long unsigned placementAttempts = 0;
    long unsigned placementAttemptsWarning = 10000;
    G4double maxLengthCoordinate = CapillaryLength - 1.001*TargetRadius;
    G4double maxRadiusCoordinate = CapillaryInnerRadius - 1.001*TargetRadius;

    if(TargetSpheresAreParallelComponents == true){

        G4double r = G4UniformRand() * maxRadiusCoordinate;
        G4double length = G4UniformRand() * maxLengthCoordinate;
        G4double phi = G4UniformRand() * 2.0 * pi;
                                    
        G4double x =  r * std::cos(phi);
        G4double y =  r * std::sin(phi);
        G4double z =  length - (maxLengthCoordinate/2.0);

        G4ThreeVector* position = new G4ThreeVector(x,y,z);
        AddCoordinates(TargetCoordinates,TargetRadius,x,y,z);
        return position;

    }

    else{
        while (true){
                    
            G4double r = G4UniformRand() * maxRadiusCoordinate;
            G4double length = G4UniformRand() * maxLengthCoordinate;
            G4double phi = G4UniformRand() * 2.0 * pi;
                                    
            G4double x =  r * std::cos(phi);
            G4double y =  r * std::sin(phi);
            G4double z =  length - (maxLengthCoordinate/2.0);
        
            if (CheckOverlapOfSphereWithComponents(TargetCoordinates, TargetRadius,x,y,z)){
        
                placementAttempts ++;
                if (placementAttempts > placementAttemptsWarning){
                    G4cerr << "Couldn't find a proper placement position for the current object within "<< placementAttempts <<" attempts. Continuing..."<<  G4endl;
                    placementAttemptsWarning = 2*placementAttemptsWarning;
                }
            }
                
            else{
            
                G4ThreeVector* position = new G4ThreeVector(x,y,z);
                AddCoordinates(TargetCoordinates,TargetRadius,x,y,z);
                return position;
            }
        }
   }
}


G4bool TsBioSAXSdosimetry::CheckOverlapOfSphereWithComponents(std::vector<std::vector<G4double> >& Coordinates,G4double r, G4double x, G4double y, G4double z){

    for(int i=0; i<Coordinates.size(); i++){
        
        if ( (r+Coordinates[i][0] ) > sqrt( ((x-Coordinates[i][1])*(x-Coordinates[i][1])) + ((y-Coordinates[i][2])*(y-Coordinates[i][2])) + ((z-Coordinates[i][3])*(z-Coordinates[i][3])) ) ) { 
        return true; 
        }
    }
    return false;    
}


void TsBioSAXSdosimetry::AddCoordinates(std::vector<std::vector<G4double> >& Coordinates, G4double r, G4double x, G4double y, G4double z){
    //G4cout << "*** add coordinates ***" <<G4endl;
    tmpCoordinates[0]=r;
    tmpCoordinates[1]=x;
    tmpCoordinates[2]=y;
    tmpCoordinates[3]=z;
    Coordinates.push_back(tmpCoordinates);
}


