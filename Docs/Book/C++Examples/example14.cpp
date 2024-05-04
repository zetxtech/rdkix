//
// Substructure searching - example14.cpp

#include <iostream>
#include <vector>
#include <GraphMol/GraphMol.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/Substruct/SubstructMatch.h>

int main(int argc, char **argv) {
  std::shared_ptr<RDKix::ROMol> mol1(RDKix::SmilesToMol("c1ccccc1O"));
  std::shared_ptr<RDKix::RWMol> patt(RDKix::SmartsToMol("ccO"));
  RDKix::MatchVectType res;
  if (RDKix::SubstructMatch(*mol1, *patt, res)) {
    std::cout << "Pattern matched molecule : " << std::endl;
    for (size_t i = 0; i < res.size(); ++i) {
      std::cout << "(" << res[i].first << "," << res[i].second << ")";
    }
    std::cout << std::endl;
  }

  std::vector<RDKix::MatchVectType> hits_vect;
  if (RDKix::SubstructMatch(*mol1, *patt, hits_vect)) {
    for (size_t i = 0; i < hits_vect.size(); ++i) {
      std::cout << "Match " << i + 1 << " : ";
      for (size_t j = 0; j < hits_vect[i].size(); ++j) {
        std::cout << "(" << hits_vect[i][j].first << ","
                  << hits_vect[i][j].second << ")";
      }
      std::cout << std::endl;
    }
  }

  std::string file_root = getenv("RDBASE");
  file_root += "/Docs/Book";

  std::string sdf_file = file_root + "/data/actives_5ht3.sdf";
  RDKix::SDMolSupplier mol_supplier(sdf_file, true);
  std::shared_ptr<RDKix::RWMol> patt1(RDKix::SmartsToMol("c[NH1]"));
  std::vector<std::shared_ptr<RDKix::ROMol>> matches;
  while (!mol_supplier.atEnd()) {
    std::shared_ptr<RDKix::ROMol> mol3(mol_supplier.next());
    if (mol3 && RDKix::SubstructMatch(*mol3, *patt1, res)) {
      matches.push_back(mol3);
    }
  }
  std::cout << "There were " << matches.size() << " hits in the file."
            << std::endl;

  std::shared_ptr<RDKix::ROMol> mol4(RDKix::SmilesToMol("C1=CC=CC=C1OC"));
  std::shared_ptr<RDKix::RWMol> smi_mol1(RDKix::SmilesToMol("CO"));
  if (RDKix::SubstructMatch(*mol4, *smi_mol1, res)) {
    std::cout << "SMILES match" << std::endl;
  } else {
    std::cout << "Not SMILES match" << std::endl;
  }
  std::shared_ptr<RDKix::RWMol> smt_mol1(RDKix::SmartsToMol("CO"));
  if (RDKix::SubstructMatch(*mol4, *smt_mol1, res)) {
    std::cout << "SMARTS match" << std::endl;
  } else {
    std::cout << "Not SMARTS match" << std::endl;
  }

  std::shared_ptr<RDKix::RWMol> smi_mol2(RDKix::SmilesToMol("COC"));
  if (RDKix::SubstructMatch(*mol4, *smi_mol2, res)) {
    std::cout << "SMILES match" << std::endl;
  } else {
    std::cout << "Not SMILES match" << std::endl;
  }
  std::shared_ptr<RDKix::RWMol> smt_mol2(RDKix::SmartsToMol("COC"));
  if (RDKix::SubstructMatch(*mol4, *smt_mol2, res)) {
    std::cout << "SMARTS match" << std::endl;
  } else {
    std::cout << "Not SMARTS match" << std::endl;
  }
  // Needs aromatic C
  std::shared_ptr<RDKix::RWMol> smt_mol3(RDKix::SmartsToMol("COc"));
  if (RDKix::SubstructMatch(*mol4, *smt_mol3, res)) {
    std::cout << "SMARTS match" << std::endl;
  } else {
    std::cout << "Not SMARTS match" << std::endl;
  }
}
