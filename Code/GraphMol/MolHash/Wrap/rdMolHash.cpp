//
//  Copyright (C) 2020-2022 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDBoost/python.h>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/MolHash/MolHash.h>
#include <RDBoost/Wrap.h>

namespace python = boost::python;
using namespace RDKix;

namespace {

std::string MolHashHelper(const ROMol &mol, MolHash::HashFunction func,
                          bool useCXSmiles, unsigned cxFlagsToSkip) {
  RWMol cpy(mol);
  return MolHash::MolHash(&cpy, func, useCXSmiles, cxFlagsToSkip);
}
}  // namespace

BOOST_PYTHON_MODULE(rdMolHash) {
  python::scope().attr("__doc__") =
      "Module containing functions to generate hashes for molecules";

  python::enum_<MolHash::HashFunction>("HashFunction")
      .value("AnonymousGraph", MolHash::HashFunction::AnonymousGraph)
      .value("ElementGraph", MolHash::HashFunction::ElementGraph)
      .value("CanonicalSmiles", MolHash::HashFunction::CanonicalSmiles)
      .value("MurckoScaffold", MolHash::HashFunction::MurckoScaffold)
      .value("ExtendedMurcko", MolHash::HashFunction::ExtendedMurcko)
      .value("MolFormula", MolHash::HashFunction::MolFormula)
      .value("AtomBondCounts", MolHash::HashFunction::AtomBondCounts)
      .value("DegreeVector", MolHash::HashFunction::DegreeVector)
      .value("Mesomer", MolHash::HashFunction::Mesomer)
      .value("HetAtomTautomer", MolHash::HashFunction::HetAtomTautomer)
      .value("HetAtomProtomer", MolHash::HashFunction::HetAtomProtomer)
      .value("RedoxPair", MolHash::HashFunction::RedoxPair)
      .value("Regioisomer", MolHash::HashFunction::Regioisomer)
      .value("NetCharge", MolHash::HashFunction::NetCharge)
      .value("SmallWorldIndexBR", MolHash::HashFunction::SmallWorldIndexBR)
      .value("SmallWorldIndexBRL", MolHash::HashFunction::SmallWorldIndexBRL)
      .value("ArthorSubstructureOrder",
             MolHash::HashFunction::ArthorSubstructureOrder)
      .value("HetAtomTautomerv2", MolHash::HashFunction::HetAtomTautomerv2);

  python::def(
      "MolHash", MolHashHelper,
      (python::arg("mol"), python::arg("func"),
       python::arg("useCxSmiles") = false, python::arg("cxFlagsToSkip") = 0),
      "Generate a hash for a molecule. The func argument determines "
      "which hash is generated.");
}
