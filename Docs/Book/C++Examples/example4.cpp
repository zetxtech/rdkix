//
// Writing molecules - example4.cpp

#include <fstream>
#include <iostream>
#include <string>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/GraphMol.h>
#include <GraphMol/MolOps.h>
#include <GraphMol/Depictor/RDDepictor.h>
#include <GraphMol/DistGeomHelpers/Embedder.h>
#include <GraphMol/ForceFieldHelpers/MMFF/MMFF.h>

int main(int argc, char **argv) {
  std::shared_ptr<RDKix::ROMol> mol1(RDKix::SmilesToMol("C1CCC1"));
  std::cout << RDKix::MolToMolBlock(*mol1) << std::endl;

  mol1->setProp("_Name", "cyclobutane");
  std::cout << RDKix::MolToMolBlock(*mol1) << std::endl;

  RDDepict::compute2DCoords(*mol1);
  std::cout << RDKix::MolToMolBlock(*mol1) << std::endl;

  std::shared_ptr<RDKix::ROMol> mol2(RDKix::SmilesToMol("C1CCC1"));
  mol2->setProp("_Name", "cyclobutane3D");
  RDKix::DGeomHelpers::EmbedMolecule(*mol2);
  RDKix::MMFF::MMFFOptimizeMolecule(*mol2, 1000, "MMFF94s");
  std::cout << RDKix::MolToMolBlock(*mol2) << std::endl;

  std::shared_ptr<RDKix::ROMol> mol3(RDKix::MolOps::addHs(*mol2));
  RDKix::MMFF::MMFFOptimizeMolecule(*mol3, 1000, "MMFF94s");
  std::cout << RDKix::MolToMolBlock(*mol3) << std::endl;

  std::shared_ptr<RDKix::RWMol> mol4(new RDKix::RWMol(*mol3));
  RDKix::MolOps::addHs(*mol4);

  std::shared_ptr<RDKix::ROMol> mol3sp(RDKix::MolOps::addHs(*mol2));
  mol3sp->setProp("_Name", "cyclobutaneSP");
  RDKix::MMFF::MMFFOptimizeMolecule(*mol3sp, 1000, "MMFF94s");
  std::cout << RDKix::MolToMolBlock(*mol3sp) << std::endl;

  std::shared_ptr<RDKix::ROMol> mol5(RDKix::MolOps::removeHs(*mol3));
  RDKix::MolOps::removeHs(*mol4);

  std::string file_root = getenv("RDBASE");
  file_root += "/Docs/Book";

  std::string mol_file = file_root + "/data/foo.mol";
  std::ofstream ofs(mol_file.c_str());
  ofs << RDKix::MolToMolBlock(*mol5);
}
