//
//  Copyright (C) 2018-2022 Boran Adas and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <boost/python.hpp>
#include <GraphMol/Fingerprints/FingerprintGenerator.h>
#include <GraphMol/Fingerprints/TopologicalTorsionGenerator.h>
#include <RDBoost/Wrap.h>

using namespace RDKix;
namespace python = boost::python;

namespace RDKix {
namespace TopologicalTorsionWrapper {
template <typename OutputType>
FingerprintGenerator<OutputType> *getTopologicalTorsionFPGenerator(
    const bool includeChirality, const uint32_t torsionAtomCount,
    const bool countSimulation, python::object &py_countBounds,
    const std::uint32_t fpSize, python::object &py_atomInvGen) {
  AtomInvariantsGenerator *atomInvariantsGenerator = nullptr;

  python::extract<AtomInvariantsGenerator *> atomInvGen(py_atomInvGen);
  if (atomInvGen.check() && atomInvGen()) {
    atomInvariantsGenerator = atomInvGen();
    atomInvariantsGenerator = atomInvariantsGenerator->clone();
  }

  std::vector<std::uint32_t> countBounds = {1, 2, 4, 8};
  if (py_countBounds) {
    auto tmp = pythonObjectToVect<std::uint32_t>(py_countBounds);
    countBounds = *tmp;
  }

  return TopologicalTorsion::getTopologicalTorsionGenerator<OutputType>(
      includeChirality, torsionAtomCount, atomInvariantsGenerator,
      countSimulation, fpSize, countBounds, false);
}

void exportTopologicalTorsion() {
  // Topological torsion fingerprint does not support 32 bit output yet
  python::class_<TopologicalTorsion::TopologicalTorsionArguments,
                 python::bases<FingerprintArguments>, boost::noncopyable>(
      "TopologicalTorsionFingerprintOptions", python::no_init)
      .def_readwrite(
          "torsionAtomCount",
          &TopologicalTorsion::TopologicalTorsionArguments::d_torsionAtomCount,
          "number of atoms to be included in the paths")
      .def_readwrite(
          "onlyShortestPaths",
          &TopologicalTorsion::TopologicalTorsionArguments::
              df_onlyShortestPaths,
          "whether or not to only include paths which are the shortest path between the start and end atoms");

  python::def(
      "GetTopologicalTorsionGenerator",
      &getTopologicalTorsionFPGenerator<std::uint64_t>,
      (python::arg("includeChirality") = false,
       python::arg("torsionAtomCount") = 4,
       python::arg("countSimulation") = true,
       python::arg("countBounds") = python::object(),
       python::arg("fpSize") = 2048,
       python::arg("atomInvariantsGenerator") = python::object()),
      "Get an atom pair fingerprint generator\n\n"
      "  ARGUMENTS:\n"
      "    - includeChirality: includeChirality argument for both the default "
      "atom invariants generator and the fingerprint arguments\n"
      "    - torsionAtomCount: the number of atoms to include in the "
      "\"torsions\"\n"
      "    - countSimulation:  if set, use count simulation while  "
      "generating the fingerprint\n"
      "    - countBounds: boundaries for count simulation, corresponding bit "
      "will be  set if the count is higher than the number provided for that "
      "spot\n"
      "    - fpSize: size of the generated fingerprint, does not affect the "
      "sparse versions\n"
      "    - atomInvariantsGenerator: atom invariants to be used during "
      "fingerprint generation\n\n"
      "This generator supports the following AdditionalOutput types:\n"
      "    - atomToBits: which bits each atom is involved in\n"
      "    - atomCounts: how many bits each atom sets\n"
      "    - bitPaths: map from bitId to vectors of atom indices\n\n"
      "  RETURNS: FingerprintGenerator\n\n",
      python::return_value_policy<python::manage_new_object>());

  return;
}
}  // namespace TopologicalTorsionWrapper

}  // namespace RDKix
