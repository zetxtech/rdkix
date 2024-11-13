//
// Modifying molecules example8.cpp

#include <iostream>

#include <GraphMol/GraphMol.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/MolOps.h>

int main(int argc, char **argv) {
  std::shared_ptr<RDKix::ROMol> mol1(RDKix::SmilesToMol("CCO"));
  std::cout << "Number of atoms : " << mol1->getNumAtoms() << std::endl;
  std::shared_ptr<RDKix::ROMol> mol2(RDKix::MolOps::addHs(*mol1));
  std::cout << "Number of atoms : " << mol2->getNumAtoms() << std::endl;

  std::shared_ptr<RDKix::RWMol> mol3(new RDKix::RWMol(*mol2));
  RDKix::MolOps::removeHs(*mol3);
  std::cout << "Number of atoms : " << mol3->getNumAtoms() << std::endl;
}
