//  Copyright (c) 2016, Novartis Institutes for BioMedical Research Inc.
//  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Novartis Institutes for BioMedical Research Inc.
//       nor the names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <RDBoost/python.h>
#include <RDBoost/Wrap.h>

#include <GraphMol/StructChecker/StructChecker.h>
#include <GraphMol/RDKixBase.h>

namespace python = boost::python;

namespace RDKix {
namespace StructureCheck {
unsigned int checkMolStructureHelper(const StructChecker &checker, ROMol &m) {
  RWMol &fixer = static_cast<RWMol &>(m);
  return checker.checkMolStructure(fixer);
}
}  // namespace StructureCheck
}  // namespace RDKix

struct struct_wrapper {
  static void wrap() {
    python::enum_<RDKix::StructureCheck::StructChecker::StructureFlags>(
        "StructureFlags")
        .value("NO_CHANGE", RDKix::StructureCheck::StructChecker::NO_CHANGE)
        .value("BAD_MOLECULE",
               RDKix::StructureCheck::StructChecker::BAD_MOLECULE)
        .value("ALIAS_CONVERSION_FAILED",
               RDKix::StructureCheck::StructChecker::ALIAS_CONVERSION_FAILED)
        .value("STEREO_ERROR",
               RDKix::StructureCheck::StructChecker::STEREO_ERROR)
        .value("STEREO_FORCED_BAD",
               RDKix::StructureCheck::StructChecker::STEREO_FORCED_BAD)
        .value("ATOM_CLASH", RDKix::StructureCheck::StructChecker::ATOM_CLASH)
        .value("ATOM_CHECK_FAILED",
               RDKix::StructureCheck::StructChecker::ATOM_CHECK_FAILED)
        .value("SIZE_CHECK_FAILED",
               RDKix::StructureCheck::StructChecker::SIZE_CHECK_FAILED)
        .value("TRANSFORMED", RDKix::StructureCheck::StructChecker::TRANSFORMED)
        .value("FRAGMENTS_FOUND",
               RDKix::StructureCheck::StructChecker::FRAGMENTS_FOUND)
        .value("EITHER_WARNING",
               RDKix::StructureCheck::StructChecker::EITHER_WARNING)
        .value("DUBIOUS_STEREO_REMOVED",
               RDKix::StructureCheck::StructChecker::DUBIOUS_STEREO_REMOVED)
        .value("RECHARGED", RDKix::StructureCheck::StructChecker::RECHARGED)
        .value("STEREO_TRANSFORMED",
               RDKix::StructureCheck::StructChecker::STEREO_TRANSFORMED)
        .value("TEMPLATE_TRANSFORMED",
               RDKix::StructureCheck::StructChecker::TEMPLATE_TRANSFORMED)
        .value("TAUTOMER_TRANSFORMED",
               RDKix::StructureCheck::StructChecker::TAUTOMER_TRANSFORMED);

    python::class_<RDKix::StructureCheck::StructCheckerOptions,
                   RDKix::StructureCheck::StructCheckerOptions *>(
        "StructCheckerOptions", python::init<>())
        .def_readwrite(
            "AcidityLimit",
            &RDKix::StructureCheck::StructCheckerOptions::AcidityLimit)
        .def_readwrite(
            "RemoveMinorFragments",
            &RDKix::StructureCheck::StructCheckerOptions::RemoveMinorFragments)
        .def_readwrite(
            "DesiredCharge",
            &RDKix::StructureCheck::StructCheckerOptions::DesiredCharge)
        .def_readwrite(
            "CheckCollisions",
            &RDKix::StructureCheck::StructCheckerOptions::CheckCollisions)
        .def_readwrite(
            "CollisionLimitPercent",
            &RDKix::StructureCheck::StructCheckerOptions::CollisionLimitPercent)
        .def_readwrite("MaxMolSize",
                       &RDKix::StructureCheck::StructCheckerOptions::MaxMolSize)
        .def_readwrite(
            "ConvertSText",
            &RDKix::StructureCheck::StructCheckerOptions::ConvertSText)
        .def_readwrite("StripZeros",
                       &RDKix::StructureCheck::StructCheckerOptions::StripZeros)
        .def_readwrite(
            "CheckStereo",
            &RDKix::StructureCheck::StructCheckerOptions::CheckStereo)
        .def_readwrite(
            "ConvertAtomTexts",
            &RDKix::StructureCheck::StructCheckerOptions::ConvertAtomTexts)
        .def_readwrite(
            "GroupsToSGroups",
            &RDKix::StructureCheck::StructCheckerOptions::GroupsToSGroups)
        .def_readwrite("Verbose",
                       &RDKix::StructureCheck::StructCheckerOptions::Verbose)
        .def(
            "LoadGoodAugmentedAtoms",
            &RDKix::StructureCheck::StructCheckerOptions::
                loadGoodAugmentedAtoms,
            (python::arg("path")),
            "Load the set of good augmented atoms from the specified file path")
        .def("LoadAcidicAugmentedAtoms",
             &RDKix::StructureCheck::StructCheckerOptions::
                 loadAcidicAugmentedAtoms,
             (python::arg("path")),
             "Load the set of acidic augmented atoms from the specified file "
             "path")
        .def("LoadAugmentedAtomTranslations",
             &RDKix::StructureCheck::StructCheckerOptions::
                 loadAugmentedAtomTranslations,
             (python::arg("path")),
             "Load the set of acidic augmented atoms from the specified file "
             "path");

    python::class_<RDKix::StructureCheck::StructChecker>("StructChecker",
                                                         python::init<>())
        .def(
            python::init<const RDKix::StructureCheck::StructCheckerOptions &>())
        .def("CheckMolStructure",
             &RDKix::StructureCheck::checkMolStructureHelper,
             (python::arg("mol")),
             "Check the structure and return a set of structure flags")
        .def("StructureFlagsToString",
             &RDKix::StructureCheck::StructChecker::StructureFlagsToString,
             (python::arg("flags")),
             "Return the structure flags as a human readable string")
        .staticmethod("StructureFlagsToString")
        .def("StringToStructureFlags",
             &RDKix::StructureCheck::StructChecker::StringToStructureFlags,
             (python::arg("str")),
             "Convert a comma separated string to the appropriate structure "
             "flags")
        .staticmethod("StringToStructureFlags");
  }
};

BOOST_PYTHON_MODULE(rdStructChecker) { struct_wrapper::wrap(); }
