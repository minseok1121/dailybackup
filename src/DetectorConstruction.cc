//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4Material* DefinePolyurethane();
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* tar_mat = nist->FindOrBuildMaterial("G4_W");
  G4Material* modulator_mat = DefinePolyurethane();
  G4Material* chamber_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4double targetRadius = 65.56*cm, targetLength = 100.0*cm;
  G4double                          modulatorWidth = 20.0*cm;
  G4double chamberRadius  = 612*cm, chamberSize = 1000.0*cm;
  
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1500*cm;
  G4double world_sizeZ  = 1500*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

  //
  // Envelope
  //
  G4ThreeVector pos0 = G4ThreeVector(0,0,-(targetLength/2+modulatorWidth+chamberSize/2+0.6*cm));

  auto targetS = new G4Tubs("target", 0., targetRadius, targetLength/2, 0. * deg, 360. * deg);
  auto logictarget = new G4LogicalVolume(targetS,  // its solid
    tar_mat,                                     // its material
    "target");                                 // its name
  new G4PVPlacement(nullptr,  // no rotation
    pos0,          // at (0,0,0)
    logictarget,                 // its logical volume
    "target",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  //
  // Shape 1
  //
  G4ThreeVector pos1 = G4ThreeVector(0,0,-(modulatorWidth/2+chamberSize/2+0.6*cm));

  auto modulatorS = new G4Tubs("modulator", 0., targetRadius, modulatorWidth/2, 0. * deg, 360. * deg);
  auto logicmodulator = new G4LogicalVolume(modulatorS, modulator_mat, "modulator", nullptr, nullptr, nullptr);
  new G4PVPlacement(nullptr,  // no rotation
    pos1,           // at (x,y,z)
    logicmodulator,             // its logical volume
    "modulator",                 // its name
    logicWorld,                  // its mother volume
    false,                    // no boolean operations
    0,                        // copy number
    checkOverlaps);          // checking overlaps

  //
  // Shape 2
  //
  G4ThreeVector pos2 = G4ThreeVector(0, 0, 0);

  auto steelchamS = new G4Tubs("steelchamber", 0, chamberRadius+0.6*cm, chamberSize/2+0.6*cm, 0. * deg, 360. * deg);
  auto chamberS = new G4Tubs("chamber", 0, chamberRadius, chamberSize/2, 0. * deg, 360. * deg);
  auto logicchamber = new G4LogicalVolume(chamberS, chamber_mat, "chamber", nullptr, nullptr, nullptr);
  G4SubtractionSolid* solidDecayChamber = new G4SubtractionSolid("DecayChamber", steelchamS, chamberS);
  G4LogicalVolume* logicDecayChamber = new G4LogicalVolume(solidDecayChamber, G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe"), "DecayChamber");
  new G4PVPlacement(nullptr,  // no rotation
    pos2,          // at (x,y,z)
    logicchamber,                // its logical volume
    "chamber",                // its name
    logicWorld,                  // its mother  volume
    false,                    // no boolean operations
    0,                        // copy number
    checkOverlaps);          // checking overlaps
  new G4PVPlacement(nullptr,  // no rotation
    pos2,          // at (x,y,z)
    logicDecayChamber,                // its logical volume
    "steel",                // its name
    logicWorld,                  // its mother  volume
    false,                    // no boolean operations
    0,                        // copy number
    checkOverlaps);          // checking overlaps

  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicchamber;

  //
  //always return the physical World
  //
  return physWorld;
}
G4Material* DefinePolyurethane() {
    G4NistManager* nistManager = G4NistManager::Instance();
    G4double density = 1.2 * g/cm3;
    G4Element* elC = nistManager->FindOrBuildElement("C");
    G4Element* elH = nistManager->FindOrBuildElement("H");
    G4Element* elO = nistManager->FindOrBuildElement("O");
    G4Material* polyurethane = new G4Material("Polyurethane", density, 3);
    polyurethane->AddElement(elC, 15);
    polyurethane->AddElement(elH, 22);
    polyurethane->AddElement(elO, 1);

    return polyurethane;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
