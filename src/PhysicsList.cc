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
/// \file B3/B3a/src/PhysicsList.cc
/// \brief Implementation of the B3::PhysicsList class

#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"



namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList(): G4VModularPhysicsList()
{
  SetVerboseLevel(1);
  decPhysicsList = new G4DecayPhysics();
  emPhysicsList = new G4EmStandardPhysics_option3();
  radiodecPhysicsList = new G4RadioactiveDecayPhysics();
  hadelaPhysicsList = new G4HadronElasticPhysics();
  ioninelaPhysicsList = new G4IonBinaryCascadePhysics();
  hadinelaPhysicsList = new G4HadronPhysicsQGSP_BIC_AllHP();
}
PhysicsList::~PhysicsList()
{ 
  delete emPhysicsList;
  delete decPhysicsList;
  delete radiodecPhysicsList;
  delete hadelaPhysicsList;
  delete ioninelaPhysicsList;
  delete hadinelaPhysicsList;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructParticle()
{
  decPhysicsList->ConstructParticle();
  emPhysicsList->ConstructParticle();
  radiodecPhysicsList->ConstructParticle();
  hadelaPhysicsList->ConstructParticle();
  ioninelaPhysicsList->ConstructParticle();
  hadinelaPhysicsList->ConstructParticle();
}
void PhysicsList::ConstructProcess()
{
  AddTransportation();
  emPhysicsList->ConstructProcess();
  decPhysicsList->ConstructProcess();
  radiodecPhysicsList->ConstructProcess();
  hadelaPhysicsList->ConstructProcess();
  ioninelaPhysicsList->ConstructProcess();
  hadinelaPhysicsList->ConstructProcess();
}

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
