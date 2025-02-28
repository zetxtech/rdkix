//
//  Copyright (C) 2018 Susan H. Leung
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDBoost/Wrap.h>

#include <GraphMol/RDKixBase.h>
#include <GraphMol/MolStandardize/MolStandardize.h>

#include <vector>

namespace python = boost::python;

namespace {
template <typename FUNCTYPE>
RDKix::ROMol *msHelper(const RDKix::ROMol *mol, python::object params,
                       FUNCTYPE func) {
  if (!mol) {
    throw_value_error("Molecule is None");
  }
  const RDKix::MolStandardize::CleanupParameters *ps =
      &RDKix::MolStandardize::defaultCleanupParameters;
  if (params) {
    ps = python::extract<RDKix::MolStandardize::CleanupParameters *>(params);
  }
  return static_cast<RDKix::ROMol *>(
      func(static_cast<const RDKix::RWMol *>(mol), *ps));
}

RDKix::ROMol *cleanupHelper(const RDKix::ROMol *mol, python::object params) {
  return msHelper(
      mol, params,
      static_cast<
          RDKix::RWMol *(*)(const RDKix::RWMol *,
                            const RDKix::MolStandardize::CleanupParameters &)>(
          RDKix::MolStandardize::cleanup));
}

RDKix::ROMol *normalizeHelper(const RDKix::ROMol *mol, python::object params) {
  return msHelper(mol, params, RDKix::MolStandardize::normalize);
}

RDKix::ROMol *reionizeHelper(const RDKix::ROMol *mol, python::object params) {
  return msHelper(mol, params, RDKix::MolStandardize::reionize);
}

RDKix::ROMol *removeFragsHelper(const RDKix::ROMol *mol,
                                python::object params) {
  return msHelper(mol, params, RDKix::MolStandardize::removeFragments);
}

RDKix::ROMol *canonicalTautomerHelper(const RDKix::ROMol *mol,
                                      python::object params) {
  return msHelper(mol, params, RDKix::MolStandardize::canonicalTautomer);
}

template <typename FUNCTYPE>
RDKix::ROMol *parentHelper(const RDKix::ROMol *mol, python::object params,
                           bool skip_standardize, FUNCTYPE func) {
  if (!mol) {
    throw_value_error("Molecule is None");
  }
  const RDKix::MolStandardize::CleanupParameters *ps =
      &RDKix::MolStandardize::defaultCleanupParameters;
  if (params) {
    ps = python::extract<RDKix::MolStandardize::CleanupParameters *>(params);
  }
  return static_cast<RDKix::ROMol *>(
      func(static_cast<const RDKix::RWMol &>(*mol), *ps, skip_standardize));
}

RDKix::ROMol *tautomerParentHelper(const RDKix::ROMol *mol,
                                   python::object params,
                                   bool skip_standardize) {
  return parentHelper(mol, params, skip_standardize,
                      RDKix::MolStandardize::tautomerParent);
}
RDKix::ROMol *fragmentParentHelper(const RDKix::ROMol *mol,
                                   python::object params,
                                   bool skip_standardize) {
  return parentHelper(mol, params, skip_standardize,
                      RDKix::MolStandardize::fragmentParent);
}
RDKix::ROMol *stereoParentHelper(const RDKix::ROMol *mol, python::object params,
                                 bool skip_standardize) {
  return parentHelper(mol, params, skip_standardize,
                      RDKix::MolStandardize::stereoParent);
}
RDKix::ROMol *isotopeParentHelper(const RDKix::ROMol *mol,
                                  python::object params,
                                  bool skip_standardize) {
  return parentHelper(mol, params, skip_standardize,
                      RDKix::MolStandardize::isotopeParent);
}
RDKix::ROMol *chargeParentHelper(const RDKix::ROMol *mol, python::object params,
                                 bool skip_standardize) {
  return parentHelper(mol, params, skip_standardize,
                      RDKix::MolStandardize::chargeParent);
}
RDKix::ROMol *superParentHelper(const RDKix::ROMol *mol, python::object params,
                                bool skip_standardize) {
  return parentHelper(mol, params, skip_standardize,
                      RDKix::MolStandardize::superParent);
}
RDKix::ROMol *disconnectOrganometallicsHelper(RDKix::ROMol &mol,
                                              python::object params) {
  if (params) {
    RDKix::MolStandardize::MetalDisconnectorOptions *mdo =
        python::extract<RDKix::MolStandardize::MetalDisconnectorOptions *>(
            params);
    return RDKix::MolStandardize::disconnectOrganometallics(mol, *mdo);
  } else {
    return RDKix::MolStandardize::disconnectOrganometallics(mol);
  }
}

}  // namespace

void wrap_validate();
void wrap_charge();
void wrap_metal();
void wrap_fragment();
void wrap_normalize();
void wrap_tautomer();

BOOST_PYTHON_MODULE(rdMolStandardize) {
  python::scope().attr("__doc__") =
      "Module containing functions for molecular standardization";

  std::string docString = "";

  python::class_<RDKix::MolStandardize::CleanupParameters, boost::noncopyable>(
      "CleanupParameters", "Parameters controlling molecular standardization")
      .def_readwrite("normalizationsFile",
                     &RDKix::MolStandardize::CleanupParameters::normalizations,
                     "file containing the normalization transformations")
      .def_readwrite("acidbaseFile",
                     &RDKix::MolStandardize::CleanupParameters::acidbaseFile,
                     "file containing the acid and base definitions")
      .def_readwrite("fragmentFile",
                     &RDKix::MolStandardize::CleanupParameters::fragmentFile,
                     "file containing the acid and base definitions")
      .def_readwrite(
          "tautomerTransformsFile",
          &RDKix::MolStandardize::CleanupParameters::tautomerTransforms,
          "file containing the tautomer transformations")
      .def_readwrite("maxRestarts",
                     &RDKix::MolStandardize::CleanupParameters::maxRestarts,
                     "maximum number of restarts")
      .def_readwrite("preferOrganic",
                     &RDKix::MolStandardize::CleanupParameters::preferOrganic,
                     "prefer organic fragments to inorganic ones when deciding "
                     "what to keep")
      .def_readwrite("doCanonical",
                     &RDKix::MolStandardize::CleanupParameters::doCanonical,
                     "apply atom-order dependent normalizations (like "
                     "uncharging) in a canonical order")
      .def_readwrite("maxTautomers",
                     &RDKix::MolStandardize::CleanupParameters::maxTautomers,
                     "maximum number of tautomers to generate (defaults to "
                     "1000)")
      .def_readwrite("maxTransforms",
                     &RDKix::MolStandardize::CleanupParameters::maxTransforms,
                     "maximum number of transforms to apply during tautomer "
                     "enumeration (defaults to 1000)")
      .def_readwrite(
          "tautomerRemoveSp3Stereo",
          &RDKix::MolStandardize::CleanupParameters::tautomerRemoveSp3Stereo,
          "remove stereochemistry from sp3 centers involved in "
          "tautomerism (defaults to True)")
      .def_readwrite(
          "tautomerRemoveBondStereo",
          &RDKix::MolStandardize::CleanupParameters::tautomerRemoveBondStereo,
          "remove stereochemistry from double bonds involved in "
          "tautomerism (defaults to True)")
      .def_readwrite(
          "tautomerRemoveIsotopicHs",
          &RDKix::MolStandardize::CleanupParameters::tautomerRemoveIsotopicHs,
          "remove isotopic Hs from centers involved in "
          "tautomerism (defaults to True)")
      .def_readwrite(
          "tautomerReassignStereo",
          &RDKix::MolStandardize::CleanupParameters::tautomerReassignStereo,
          "call AssignStereochemistry on all generated tautomers "
          "(defaults to True)")
      .def_readwrite("largestFragmentChooserUseAtomCount",
                     &RDKix::MolStandardize::CleanupParameters::
                         largestFragmentChooserUseAtomCount,
                     "Whether LargestFragmentChooser should use atom "
                     "count as main criterion before MW (defaults to True)")
      .def_readwrite("largestFragmentChooserCountHeavyAtomsOnly",
                     &RDKix::MolStandardize::CleanupParameters::
                         largestFragmentChooserCountHeavyAtomsOnly,
                     "whether LargestFragmentChooser should only count "
                     "heavy atoms (defaults to False)");

  python::def("UpdateParamsFromJSON",
              &RDKix::MolStandardize::updateCleanupParamsFromJSON,
              "updates the cleanup parameters from the provided JSON string");

  docString = "Standardizes a molecule";
  python::def("Cleanup", cleanupHelper,
              (python::arg("mol"), python::arg("params") = python::object()),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "Convenience function for standardizing a SMILES";
  python::def("StandardizeSmiles", RDKix::MolStandardize::standardizeSmiles,
              (python::arg("smiles")), docString.c_str());
  docString =
      "Returns the tautomer parent of a given molecule. The fragment parent is "
      "the standardized canonical tautomer of the molecule";
  python::def("TautomerParent", tautomerParentHelper,
              (python::arg("mol"), python::arg("params") = python::object(),
               python::arg("skipStandardize") = false),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "Returns the largest fragment after doing a cleanup";
  python::def("FragmentParent", fragmentParentHelper,
              (python::arg("mol"), python::arg("params") = python::object(),
               python::arg("skipStandardize") = false),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "calls removeStereochemistry() on the given molecule";
  python::def("StereoParent", stereoParentHelper,
              (python::arg("mol"), python::arg("params") = python::object(),
               python::arg("skipStandardize") = false),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "removes all isotopes specifications from the given molecule";
  python::def("IsotopeParent", isotopeParentHelper,
              (python::arg("mol"), python::arg("params") = python::object(),
               python::arg("skipStandardize") = false),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "Returns the uncharged version of the largest fragment";
  python::def("ChargeParent", chargeParentHelper,
              (python::arg("mol"), python::arg("params") = python::object(),
               python::arg("skipStandardize") = false),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString =
      "Returns the super parent. The super parent is the fragment, charge, "
      "isotope, stereo, and tautomer parent of the molecule.";
  python::def("SuperParent", superParentHelper,
              (python::arg("mol"), python::arg("params") = python::object(),
               python::arg("skipStandardize") = false),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString =
      "Applies a series of standard transformations to correct functional "
      "groups and recombine charges";
  python::def("Normalize", normalizeHelper,
              (python::arg("mol"), python::arg("params") = python::object()),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "Ensures the strongest acid groups are charged first";
  python::def("Reionize", reionizeHelper,
              (python::arg("mol"), python::arg("params") = python::object()),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "Removes fragments from the molecule";
  python::def("RemoveFragments", removeFragsHelper,
              (python::arg("mol"), python::arg("params") = python::object()),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString = "Returns the canonical tautomer for the molecule";
  python::def("CanonicalTautomer", canonicalTautomerHelper,
              (python::arg("mol"), python::arg("params") = python::object()),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());
  docString =
      "Returns the molecule disconnected using the organometallics"
      " rules.";
  python::def("DisconnectOrganometallics", disconnectOrganometallicsHelper,
              (python::arg("mol"), python::arg("params") = python::object()),
              docString.c_str(),
              python::return_value_policy<python::manage_new_object>());

  wrap_validate();
  wrap_charge();
  wrap_metal();
  wrap_fragment();
  wrap_normalize();
  wrap_tautomer();
}
