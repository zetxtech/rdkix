//
//  Copyright (C) 2018-2021 Boran Adas and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <boost/python.hpp>
#include <GraphMol/Fingerprints/FingerprintGenerator.h>
#include <GraphMol/Fingerprints/RDKixFPGenerator.h>
#include <RDBoost/Wrap.h>

using namespace RDKix;
namespace python = boost::python;

namespace RDKix {
namespace RDKixFPWrapper {
template <typename OutputType>
FingerprintGenerator<OutputType> *getRDKixFPGenerator(
    unsigned int minPath, unsigned int maxPath, bool useHs, bool branchedPaths,
    bool useBondOrder, bool countSimulation, python::object &py_countBounds,
    std::uint32_t fpSize, std::uint32_t numBitsPerFeature,
    python::object &py_atomInvGen) {
  AtomInvariantsGenerator *atomInvariantsGenerator = nullptr;

  python::extract<AtomInvariantsGenerator *> atomInvGen(py_atomInvGen);
  if (atomInvGen.check() && atomInvGen()) {
    atomInvariantsGenerator = atomInvGen()->clone();
  }

  std::vector<std::uint32_t> countBounds = {1, 2, 4, 8};

  if (py_countBounds) {
    auto tmp = pythonObjectToVect<std::uint32_t>(py_countBounds);
    countBounds = *tmp;
  }

  return RDKixFP::getRDKixFPGenerator<OutputType>(
      minPath, maxPath, useHs, branchedPaths, useBondOrder,
      atomInvariantsGenerator, countSimulation, countBounds, fpSize,
      numBitsPerFeature, true);
}

AtomInvariantsGenerator *getRDKixAtomInvGen() {
  return new RDKixFP::RDKixFPAtomInvGenerator();
}

void exportRDKix() {
  python::class_<RDKixFP::RDKixFPArguments, python::bases<FingerprintArguments>,
                 boost::noncopyable>("RDKixFingerprintOptions", python::no_init)
      .def_readwrite("minPath", &RDKixFP::RDKixFPArguments::d_minPath,
                     "minimum path length (in bonds) to be included")
      .def_readwrite("maxPath", &RDKixFP::RDKixFPArguments::d_maxPath,
                     "maximum path length (in bonds) to be included")
      .def_readwrite(
          "useHs", &RDKixFP::RDKixFPArguments::df_useHs,
          "use explicit Hs in the paths (if molecule has explicit Hs)")
      .def_readwrite("branchedPaths",
                     &RDKixFP::RDKixFPArguments::df_branchedPaths,
                     "generate branched subgraphs, not just linear ones")
      .def_readwrite("useBondOrder",
                     &RDKixFP::RDKixFPArguments::df_useBondOrder,
                     "include bond orders in the path hashes");
  python::def(
      "GetRDKixFPGenerator", &getRDKixFPGenerator<std::uint64_t>,
      (python::arg("minPath") = 1, python::arg("maxPath") = 7,
       python::arg("useHs") = true, python::arg("branchedPaths") = true,
       python::arg("useBondOrder") = true,
       python::arg("countSimulation") = false,
       python::arg("countBounds") = python::object(),
       python::arg("fpSize") = 2048, python::arg("numBitsPerFeature") = 2,
       python::arg("atomInvariantsGenerator") = python::object()),
      "Get an RDKix fingerprint generator\n\n"
      "  ARGUMENTS:\n"
      "    - minPath: the minimum path length (in bonds) to be included\n"
      "    - maxPath: the maximum path length (in bonds) to be included\n"
      "    - useHs: toggles inclusion of Hs in paths (if the molecule has "
      "explicit Hs)\n"
      "    - branchedPaths: toggles generation of branched subgraphs, not just "
      "linear paths\n"
      "    - useBondOrder: toggles inclusion of bond orders in the path "
      "hashes\n"
      "    - countSimulation:  if set, use count simulation while  "
      "generating the fingerprint\n"
      "    - countBounds: boundaries for count simulation, corresponding bit "
      "will be  set if the count is higher than the number provided for that "
      "spot\n"
      "    - fpSize: size of the generated fingerprint, does not affect the "
      "sparse versions\n"
      "    - numBitsPerFeature: the number of bits set per path/subgraph "
      "found\n"
      "    - atomInvariantsGenerator: atom invariants to be used during "
      "fingerprint generation\n\n"
      "This generator supports the following AdditionalOutput types:\n"
      "    - atomToBits: which bits each atom is involved in\n"
      "    - atomCounts: how many bits each atom sets\n"
      "    - bitPaths: map from bitId to vectors of bond indices for the "
      "individual subgraphs\n\n"
      "  RETURNS: FingerprintGenerator\n\n",
      python::return_value_policy<python::manage_new_object>());

  python::def("GetRDKixAtomInvGen", &getRDKixAtomInvGen,
              "Get an RDKix atom invariants generator\n\n"
              "  RETURNS: AtomInvariantsGenerator\n\n",
              python::return_value_policy<python::manage_new_object>());

  return;
}
}  // namespace RDKixFPWrapper

}  // namespace RDKix
