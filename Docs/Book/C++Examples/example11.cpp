//
// Working with 3D molecules - example11.cpp

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <Geometry/point.h>
#include <GraphMol/GraphMol.h>
#include <GraphMol/MolOps.h>
#include <GraphMol/MolAlign/AlignMolecules.h>
#include <GraphMol/DistGeomHelpers/Embedder.h>
#include <GraphMol/ForceFieldHelpers/MMFF/MMFF.h>
#include <GraphMol/ForceFieldHelpers/UFF/UFF.h>
#include <GraphMol/SmilesParse/SmilesParse.h>

int main(int main, char **argv) {
  std::shared_ptr<RDKix::ROMol> mol(RDKix::SmilesToMol("C1CCC1OC"));
  std::shared_ptr<RDKix::ROMol> mol1(RDKix::MolOps::addHs(*mol));
  std::shared_ptr<RDKix::ROMol> mol2(RDKix::MolOps::addHs(*mol));
  // Original distance geometry embedding
  RDKix::DGeomHelpers::EmbedMolecule(*mol1, 0, 1234);
  RDKix::UFF::UFFOptimizeMolecule(*mol1);

  // new Riniker and Landrum CSD-based method
  // using the parameters class
  RDKix::DGeomHelpers::EmbedParameters params(RDKix::DGeomHelpers::ETKDG);
  params.randomSeed = 1234;
  RDKix::DGeomHelpers::EmbedMolecule(*mol2, params);

  // Multiple conformations
  RDKix::INT_VECT mol1_cids =
      RDKix::DGeomHelpers::EmbedMultipleConfs(*mol1, 10);
  std::cout << "Number of conformations : " << mol1_cids.size() << std::endl;

  RDKix::INT_VECT mol2_cids;
  int numConfs = 20;
  RDKix::DGeomHelpers::EmbedMultipleConfs(*mol2, mol2_cids, numConfs, params);
  std::cout << "Number of conformations : " << mol2_cids.size() << std::endl;

  std::vector<double> rms_list;
  std::vector<unsigned int> m2cids(mol2_cids.begin(), mol2_cids.end());
  RDKix::MolAlign::alignMolConformers(*mol2, nullptr, &m2cids, nullptr, false,
                                      50, &rms_list);
}
