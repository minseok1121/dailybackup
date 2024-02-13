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
/// \file B1/src/SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  G4Track* track = step->GetTrack();

    if (track->GetTrackStatus() == fStopAndKill && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
        G4ParticleDefinition* particleDef = track->GetDefinition();
        G4String particleName = particleDef->GetParticleName();
            if(!(particleName=="proton"||particleName=="neutron"||particleName=="gamma"||particleName=="e-"||particleName=="e+")) return;
        G4String parentName = "Primary";
        if (track->GetParentID() > 0) {
          G4Track* parentTrack = G4EventManager::GetEventManager()->GetTrackingManager()->GimmeSecondaries()->at(track->GetParentID() - 1);
          G4ParticleDefinition* parentParticleDef = parentTrack->GetDefinition();
          parentName = parentParticleDef->GetParticleName();
        }
        G4String processName = "PPrimary";
        if (track->GetCreatorProcess() != nullptr) {
        processName = track->GetCreatorProcess()->GetProcessName();
        }
        G4double energy = track->GetKineticEnergy();

        // 결과 파일에 저장
        std::ofstream outFile("ddd_"+ particleName + ".txt", std::ios_base::app);
        outFile << eventID << std::endl;
        outFile << "Parent particle: " << parentName << std::endl;
        outFile << "Created by process: " << processName << std::endl;
        outFile << "Particle: " << particleName << ", Energy: " << energy / CLHEP::MeV << " MeV" << std::endl;
        outFile.close();

        // 이벤트 액션으로 입자 종류 전달
        fEventAction->CountParticles(particleName);
    }

  /*
    if (track->GetTrackID() == 1) {
    if (step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "target") {
        G4int stepNumber = track->GetCurrentStepNumber();
        G4double globalTime = step->GetPreStepPoint()->GetGlobalTime();
        G4double kineticEnergy = step->GetPreStepPoint()->GetKineticEnergy();
        //G4double energyDeposition = step->GetTotalEnergyDeposit();
        G4double zPosition = step->GetPreStepPoint()->GetPosition().z();
        G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

        G4String filename = "event_" + std::to_string(eventID) + ".txt";
        std::ofstream outFile(filename, std::ios::app);
        outFile << globalTime << "   " << stepNumber << "      " << zPosition/10 + 620.0 << "      " << kineticEnergy << "      " << processName << G4endl;
        outFile.close();
    }
    }
*/

  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
