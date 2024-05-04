//
// Writing molecules - example3.cpp

#include <iostream>
#include <string>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/GraphMol.h>
#include <GraphMol/MolOps.h>

int main(int argc, char **argv) {
  std::string file_root = getenv("RDBASE");
  file_root += "/Docs/Book";
  std::string mol_file = file_root + "/data/chiral.mol";
  std::shared_ptr<RDKix::ROMol> mol(RDKix::MolFileToMol(mol_file));
  std::cout << RDKix::MolToSmiles(*mol, true) << std::endl;
  // 2nd parameter doIsomericSmiles defaults to true
  std::cout << RDKix::MolToSmiles(*mol, false) << std::endl;

  std::shared_ptr<RDKix::ROMol> mol1(RDKix::SmilesToMol("C1=CC=CN=C1"));
  std::cout << RDKix::MolToSmiles(*mol1) << std::endl;

  std::shared_ptr<RDKix::ROMol> mol2(RDKix::SmilesToMol("c1cccnc1"));
  std::cout << RDKix::MolToSmiles(*mol2) << std::endl;

  std::shared_ptr<RDKix::ROMol> mol3(RDKix::SmilesToMol("n1ccccc1"));
  std::cout << RDKix::MolToSmiles(*mol3) << std::endl;

  std::shared_ptr<RDKix::RWMol> mol4(new RDKix::RWMol(*mol));
  RDKix::MolOps::Kekulize(*mol4);
  std::cout << RDKix::MolToSmiles(*mol4) << std::endl;

  mol1.reset(RDKix::SmilesToMol("C1CCC1"));
  std::cout << RDKix::MolToMolBlock(*mol1) << std::endl;

  mol1->setProp("_Name", "cyclobutane");
  std::cout << RDKix::MolToMolBlock(*mol1) << std::endl;
}
