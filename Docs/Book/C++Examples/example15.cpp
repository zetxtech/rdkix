//
// Substructure searching with stereochemistry - example15.cpp

#include <iostream>
#include <GraphMol/GraphMol.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/Substruct/SubstructMatch.h>

int main(int argc, char **argv) {
  std::shared_ptr<RDKix::ROMol> mol1(RDKix::SmilesToMol("CC[C@H](F)Cl"));
  std::shared_ptr<RDKix::RWMol> patt1(RDKix::SmartsToMol("C[C@H](F)Cl"));
  RDKix::MatchVectType res;
  if (RDKix::SubstructMatch(*mol1, *patt1, res)) {
    std::cout << "SMARTS 1 match" << std::endl;
  } else {
    std::cout << "Not SMARTS 1 match" << std::endl;
  }
  std::shared_ptr<RDKix::RWMol> patt2(RDKix::SmartsToMol("C[C@@H](F)Cl"));
  if (RDKix::SubstructMatch(*mol1, *patt2, res)) {
    std::cout << "SMARTS 2 match" << std::endl;
  } else {
    std::cout << "Not SMARTS 2 match" << std::endl;
  }
  std::shared_ptr<RDKix::RWMol> patt3(RDKix::SmartsToMol("CC(F)Cl"));
  if (RDKix::SubstructMatch(*mol1, *patt3, res)) {
    std::cout << "SMARTS 3 match" << std::endl;
  } else {
    std::cout << "Not SMARTS 3 match" << std::endl;
  }

  if (RDKix::SubstructMatch(*mol1, *patt1, res, true, true)) {
    std::cout << "SMARTS 1 chiral match" << std::endl;
  } else {
    std::cout << "Not SMARTS 1 chiral match" << std::endl;
  }
  if (RDKix::SubstructMatch(*mol1, *patt2, res, true, true)) {
    std::cout << "SMARTS 2 chiral match" << std::endl;
  } else {
    std::cout << "Not SMARTS 2 chiral match" << std::endl;
  }
  if (RDKix::SubstructMatch(*mol1, *patt3, res, true, true)) {
    std::cout << "SMARTS 3 chiral match" << std::endl;
  } else {
    std::cout << "Not SMARTS 3 chiral match" << std::endl;
  }

  std::shared_ptr<RDKix::RWMol> mol2(RDKix::SmilesToMol("CC(F)Cl"));
  if (RDKix::SubstructMatch(*mol1, *mol2, res, true, true)) {
    std::cout << "Chiral mol, non-chiral query : match" << std::endl;
  } else {
    std::cout << "Chiral mol, non-chiral query : NO match" << std::endl;
  }

  std::shared_ptr<RDKix::RWMol> patt5(RDKix::SmilesToMol("C[C@H](F)Cl"));
  if (RDKix::SubstructMatch(*mol2, *patt5, res, true, true)) {
    std::cout << "Non-chiral mol, chiral query : match" << std::endl;
  } else {
    std::cout << "Non-chiral mol, chiral query : NO match" << std::endl;
  }
}
