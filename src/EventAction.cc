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
/// \file B1/src/EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int neutronCount = 0, protonCount = 0, gammaCount = 0, electronCount = 0, positronCount = 0;

void EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
  neutronCount = protonCount = gammaCount = electronCount = positronCount = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
    // 결과 파일에 저장
    std::ofstream outFile("ddd_FullCount.txt", std::ios_base::app);
    outFile << "Neutron count: " << neutronCount << std::endl;
    outFile << "Proton count: " << protonCount << std::endl;
    outFile << "Gamma count: " << gammaCount << std::endl;
    outFile << "Electron count: " << electronCount << std::endl;
    outFile << "Positron count: " << positronCount << std::endl;
    outFile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::CountParticles(G4String particleName) {
    if (particleName == "neutron") {
        neutronCount++;
    } else if (particleName == "proton") {
        protonCount++;
    } else if (particleName == "gamma") {
        gammaCount++;
    } else if (particleName == "e-") {
        electronCount++;
    } else if (particleName == "e+") {
        positronCount++;
    }
}

}
