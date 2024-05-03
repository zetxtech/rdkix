//
//  Copyright (C) 2020-2022 Greg Landrum and T5 Informatics GmbH
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDBoost/python.h>

#include <string>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/Chirality.h>

#include <RDBoost/Wrap.h>

namespace python = boost::python;
namespace RDKix {
struct chiralityops_wrapper {
  static void wrap() {
    RegisterVectorConverter<Chirality::StereoInfo>();

    python::def(
        "FindPotentialStereo",
        (std::vector<Chirality::StereoInfo>(*)(ROMol &, bool, bool)) &
            Chirality::findPotentialStereo,
        (python::arg("mol"), python::arg("cleanIt") = false,
         python::arg("flagPossible") = true),
        "find potential stereo elements in a molecule and returns them as StereoInfo objects\n\
Note that this function is still somewhat experimental and the API\n\
and results may change in a future release.",
        python::with_custodian_and_ward_postcall<0, 1>());
  };
};
}  // namespace RDKix

void wrap_chiralityops() { RDKix::chiralityops_wrapper::wrap(); }
