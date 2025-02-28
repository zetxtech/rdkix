//
//  Copyright (c) 2007-2021, Novartis Institutes for BioMedical Research Inc.
//  and other RDKix contributors
//
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
//     * Neither the name of Novartis Institutues for BioMedical Research Inc.
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
#include <GraphMol/MolPickler.h>
#include <GraphMol/Wrap/props.hpp>
#include <RDBoost/python.h>
#include <GraphMol/ChemReactions/Reaction.h>
#include <GraphMol/ChemReactions/ReactionPickler.h>
#include <GraphMol/ChemReactions/ReactionParser.h>
#include <GraphMol/ChemReactions/ReactionRunner.h>
#include <GraphMol/ChemReactions/PreprocessRxn.h>
#include <GraphMol/ChemReactions/SanitizeRxn.h>
#include <GraphMol/Depictor/DepictUtils.h>
#include <GraphMol/FilterCatalog/FunctionalGroupHierarchy.h>

#include <RDBoost/Wrap.h>

#include <RDGeneral/Exceptions.h>
#include <GraphMol/SanitException.h>
#include <RDGeneral/FileParseException.h>
#include <GraphMol/ChemReactions/ReactionFingerprints.h>
#include <GraphMol/ChemReactions/ReactionUtils.h>

namespace python = boost::python;

void rdChemicalReactionParserExceptionTranslator(
    RDKix::ChemicalReactionParserException const &x) {
  std::ostringstream ss;
  ss << "ChemicalReactionParserException: " << x.what();
  PyErr_SetString(PyExc_ValueError, ss.str().c_str());
}
void rdChemicalReactionExceptionTranslator(
    RDKix::ChemicalReactionException const &x) {
  std::ostringstream ss;
  ss << "ChemicalParserException: " << x.what();
  PyErr_SetString(PyExc_ValueError, ss.str().c_str());
}

namespace RDKix {
python::object ReactionToBinaryWithProps(const ChemicalReaction &self,
                                         unsigned int props) {
  std::string res;
  ReactionPickler::pickleReaction(self, res, props);
  python::object retval = python::object(
      python::handle<>(PyBytes_FromStringAndSize(res.c_str(), res.length())));
  return retval;
}
python::object ReactionToBinary(const ChemicalReaction &self) {
  return ReactionToBinaryWithProps(self,
                                   MolPickler::getDefaultPickleProperties());
}
//
// allows reactions to be pickled.
//
struct reaction_pickle_suite : rdkix_pickle_suite {
  static python::tuple getinitargs(const ChemicalReaction &self) {
    return python::make_tuple(ReactionToBinary(self));
  };
};

template <typename T>
PyObject *RunReactants(ChemicalReaction *self, T reactants,
                       unsigned int maxProducts) {
  if (!self->isInitialized()) {
    NOGIL gil;
    self->initReactantMatchers();
  }
  MOL_SPTR_VECT reacts;
  unsigned int len1 =
      python::extract<unsigned int>(reactants.attr("__len__")());
  reacts.resize(len1);
  for (unsigned int i = 0; i < len1; ++i) {
    reacts[i] = python::extract<ROMOL_SPTR>(reactants[i]);
    if (!reacts[i]) {
      throw_value_error("reaction called with None reactants");
    }
  }
  std::vector<MOL_SPTR_VECT> mols;
  {
    NOGIL gil;
    mols = self->runReactants(reacts, maxProducts);
  }
  PyObject *res = PyTuple_New(mols.size());

  for (unsigned int i = 0; i < mols.size(); ++i) {
    PyObject *lTpl = PyTuple_New(mols[i].size());
    for (unsigned int j = 0; j < mols[i].size(); ++j) {
      PyTuple_SetItem(lTpl, j,
                      python::converter::shared_ptr_to_python(mols[i][j]));
    }
    PyTuple_SetItem(res, i, lTpl);
  }
  return res;
}

template <typename T>
PyObject *RunReactant(ChemicalReaction *self, T reactant,
                      unsigned int reactionIdx) {
  ROMOL_SPTR react = python::extract<ROMOL_SPTR>(reactant);

  std::vector<MOL_SPTR_VECT> mols;

  {
    NOGIL gil;
    if (!self->isInitialized()) {
      self->initReactantMatchers();
    }
    mols = self->runReactant(react, reactionIdx);
  }
  PyObject *res = PyTuple_New(mols.size());

  for (unsigned int i = 0; i < mols.size(); ++i) {
    PyObject *lTpl = PyTuple_New(mols[i].size());
    for (unsigned int j = 0; j < mols[i].size(); ++j) {
      PyTuple_SetItem(lTpl, j,
                      python::converter::shared_ptr_to_python(mols[i][j]));
    }
    PyTuple_SetItem(res, i, lTpl);
  }
  return res;
}

bool RunReactantInPlace(ChemicalReaction *self, ROMol *reactant) {
  auto react = static_cast<RWMol *>(reactant);
  bool res = false;
  {
    NOGIL gil;
    if (!self->isInitialized()) {
      self->initReactantMatchers();
    }
    res = self->runReactant(*react);
  }
  return res;
}

python::tuple ValidateReaction(const ChemicalReaction *self,
                               bool silent = false) {
  unsigned int numWarn, numError;
  self->validate(numWarn, numError, silent);
  return python::make_tuple(numWarn, numError);
}

ROMol *GetProductTemplate(const ChemicalReaction *self, unsigned int which) {
  if (which >= self->getNumProductTemplates()) {
    throw_value_error("requested template index too high");
  }
  auto iter = self->beginProductTemplates();
  iter += which;
  auto *res = const_cast<ROMol *>(iter->get());
  return res;
}
ROMol *GetReactantTemplate(const ChemicalReaction *self, unsigned int which) {
  if (which >= self->getNumReactantTemplates()) {
    throw_value_error("requested template index too high");
  }
  auto iter = self->beginReactantTemplates();
  iter += which;
  auto *res = const_cast<ROMol *>(iter->get());
  return res;
}
ROMol *GetAgentTemplate(const ChemicalReaction *self, unsigned int which) {
  if (which >= self->getNumAgentTemplates()) {
    throw_value_error("requested template index too high");
  }
  auto iter = self->beginAgentTemplates();
  iter += which;
  auto *res = const_cast<ROMol *>(iter->get());
  return res;
}

void RemoveUnmappedReactantTemplates(ChemicalReaction *self,
                                     double thresholdUnmappedAtoms,
                                     bool moveToAgentTemplates,
                                     python::object targetList) {
  if (targetList == python::object()) {
    self->removeUnmappedReactantTemplates(thresholdUnmappedAtoms,
                                          moveToAgentTemplates);
  } else {
    MOL_SPTR_VECT tmp;
    self->removeUnmappedReactantTemplates(thresholdUnmappedAtoms,
                                          moveToAgentTemplates, &tmp);
    python::list molList = python::extract<python::list>(targetList);
    if (tmp.size() > 0) {
      for (auto &i : tmp) {
        molList.append(i);
      }
    }
  }
}

void RemoveUnmappedProductTemplates(ChemicalReaction *self,
                                    double thresholdUnmappedAtoms,
                                    bool moveToAgentTemplates,
                                    python::object targetList) {
  if (targetList == python::object()) {
    self->removeUnmappedProductTemplates(thresholdUnmappedAtoms,
                                         moveToAgentTemplates);
  } else {
    MOL_SPTR_VECT tmp;
    self->removeUnmappedProductTemplates(thresholdUnmappedAtoms,
                                         moveToAgentTemplates, &tmp);
    python::list molList = python::extract<python::list>(targetList);
    if (tmp.size() > 0) {
      for (auto &i : tmp) {
        molList.append(i);
      }
    }
  }
}

void RemoveAgentTemplates(ChemicalReaction &self, python::object targetList) {
  if (targetList == python::object()) {
    self.removeAgentTemplates();
  } else {
    MOL_SPTR_VECT tmp;
    self.removeAgentTemplates(&tmp);
    python::list molList = python::extract<python::list>(targetList);
    if (tmp.size() > 0) {
      for (auto &i : tmp) {
        molList.append(i);
      }
    }
  }
}

void Compute2DCoordsForReaction(RDKix::ChemicalReaction &rxn,
                                double spacing = 2.0, bool updateProps = true,
                                bool canonOrient = false,
                                unsigned int nFlipsPerSample = 0,
                                unsigned int nSamples = 0, int sampleSeed = 0,
                                bool permuteDeg4Nodes = false,
                                double bondLength = -1) {
  double oBondLen = RDDepict::BOND_LEN;
  if (bondLength > 0) {
    RDDepict::BOND_LEN = bondLength;
  }
  RDDepict::compute2DCoordsForReaction(rxn, spacing, updateProps, canonOrient,
                                       nFlipsPerSample, nSamples, sampleSeed,
                                       permuteDeg4Nodes);
  if (bondLength > 0) {
    RDDepict::BOND_LEN = oBondLen;
  }
}

bool IsMoleculeReactantOfReaction(const ChemicalReaction &rxn,
                                  const ROMol &mol) {
  unsigned int which;
  return isMoleculeReactantOfReaction(rxn, mol, which);
}
bool IsMoleculeProductOfReaction(const ChemicalReaction &rxn,
                                 const ROMol &mol) {
  unsigned int which;
  return isMoleculeProductOfReaction(rxn, mol, which);
}
bool IsMoleculeAgentOfReaction(const ChemicalReaction &rxn, const ROMol &mol) {
  unsigned int which;
  return isMoleculeAgentOfReaction(rxn, mol, which);
}

ChemicalReaction *ReactionFromSmarts(const char *smarts, python::dict replDict,
                                     bool useSmiles) {
  PRECONDITION(smarts, "null SMARTS string");
  std::map<std::string, std::string> replacements;
  for (unsigned int i = 0;
       i < python::extract<unsigned int>(replDict.keys().attr("__len__")());
       ++i) {
    replacements[python::extract<std::string>(replDict.keys()[i])] =
        python::extract<std::string>(replDict.values()[i]);
  }
  ChemicalReaction *res;
  res = RxnSmartsToChemicalReaction(smarts, &replacements, useSmiles);
  return res;
}

python::object GetReactingAtoms(const ChemicalReaction &self,
                                bool mappedAtomsOnly) {
  python::list res;
  VECT_INT_VECT rAs = getReactingAtoms(self, mappedAtomsOnly);
  for (auto &rA : rAs) {
    res.append(python::tuple(rA));
  }
  return python::tuple(res);
}

python::object AddRecursiveQueriesToReaction(ChemicalReaction &self,
                                             python::dict queryDict,
                                             std::string propName,
                                             bool getLabels = false) {
  // transform dictionary into map
  std::map<std::string, ROMOL_SPTR> queries;
  for (unsigned int i = 0;
       i < python::extract<unsigned int>(queryDict.keys().attr("__len__")());
       ++i) {
    ROMol *m = python::extract<ROMol *>(queryDict.values()[i]);
    ROMOL_SPTR nm(new ROMol(*m));
    std::string k = python::extract<std::string>(queryDict.keys()[i]);
    queries[k] = nm;
  }

  if (getLabels) {
    std::vector<std::vector<std::pair<unsigned int, std::string>>> labels;
    addRecursiveQueriesToReaction(self, queries, propName, &labels);

    // transform labels into python::tuple(python::tuple(python::tuple))
    python::list reactantLabels;
    for (auto &label : labels) {
      python::list tmpLabels;
      for (auto &j : label) {
        python::list tmpPair;
        tmpPair.append(j.first);
        tmpPair.append(j.second);
        tmpLabels.append(python::tuple(tmpPair));
      }
      reactantLabels.append(python::tuple(tmpLabels));
    }
    return python::tuple(reactantLabels);
  } else {
    addRecursiveQueriesToReaction(self, queries, propName);
    return python::object();  // this is None
  }
}

python::object PreprocessReaction(ChemicalReaction &reaction,
                                  python::dict queryDict,
                                  std::string propName) {
  // transform dictionary into map
  std::map<std::string, ROMOL_SPTR> queries;
  unsigned int size =
      python::extract<unsigned int>(queryDict.keys().attr("__len__")());
  if (!size) {
    const bool normalized = true;
    queries = GetFlattenedFunctionalGroupHierarchy(normalized);
  } else {
    for (unsigned int i = 0; i < size; ++i) {
      ROMol *m = python::extract<ROMol *>(queryDict.values()[i]);
      ROMOL_SPTR nm(new ROMol(*m));
      std::string k = python::extract<std::string>(queryDict.keys()[i]);
      queries[k] = nm;
    }
  }

  unsigned int nReactants = reaction.getNumReactantTemplates();
  unsigned int nProducts = reaction.getNumProductTemplates();
  unsigned int nWarn, nError;
  reaction.validate(nWarn, nError);
  std::vector<std::vector<std::pair<unsigned int, std::string>>> labels;

  if (!nError) {
    preprocessReaction(reaction, nWarn, nError, labels, queries, propName);
  }

  // transform labels into python::tuple(python::tuple(python::tuple))
  python::list reactantLabels;
  for (auto &label : labels) {
    python::list tmpLabels;
    for (auto &j : label) {
      python::list tmpPair;
      tmpPair.append(j.first);
      tmpPair.append(j.second);
      tmpLabels.append(python::tuple(tmpPair));
    }
    reactantLabels.append(python::tuple(tmpLabels));
  }
  return python::make_tuple(nWarn, nError, nReactants, nProducts,
                            python::tuple(reactantLabels));
}

typedef boost::uint64_t sanitize_ops;

RxnOps::SanitizeRxnFlags sanitizeReaction(
    ChemicalReaction &rxn, sanitize_ops sanitizeOps,
    const MolOps::AdjustQueryParameters &params, bool catchErrors) {
  unsigned int operationsThatFailed = 0;
  try {
    RxnOps::sanitizeRxn(rxn, operationsThatFailed, sanitizeOps, params);
  } catch (...) {
    if (!catchErrors) {
      throw;
    }
  }
  return static_cast<RxnOps::SanitizeRxnFlags>(operationsThatFailed);
}

python::object addReactionToPNGStringHelper(const ChemicalReaction &rxn,
                                            python::object png, bool includePkl,
                                            bool includeSmiles,
                                            bool includeSmarts,
                                            bool includeRxn) {
  std::string cstr = python::extract<std::string>(png);

  auto res = addChemicalReactionToPNGString(
      rxn, cstr, includePkl, includeSmiles, includeSmarts, includeRxn);

  python::object retval = python::object(
      python::handle<>(PyBytes_FromStringAndSize(res.c_str(), res.length())));
  return retval;
}
python::object addReactionToPNGFileHelper(const ChemicalReaction &rxn,
                                          python::object fname, bool includePkl,
                                          bool includeSmiles,
                                          bool includeSmarts, bool includeRxn) {
  std::string cstr = python::extract<std::string>(fname);

  auto res = addChemicalReactionToPNGFile(rxn, cstr, includePkl, includeSmiles,
                                          includeSmarts, includeRxn);

  python::object retval = python::object(
      python::handle<>(PyBytes_FromStringAndSize(res.c_str(), res.length())));
  return retval;
}

SubstructMatchParameters *getParamsHelper(ChemicalReaction &rxn) {
  return &rxn.getSubstructParams();
}

}  // namespace RDKix

void wrap_enumeration();

BOOST_PYTHON_MODULE(rdChemReactions) {
  python::scope().attr("__doc__") =
      "Module containing classes and functions for working with chemical "
      "reactions.";

  python::register_exception_translator<RDKix::ChemicalReactionParserException>(
      &rdChemicalReactionParserExceptionTranslator);
  python::register_exception_translator<RDKix::ChemicalReactionException>(
      &rdChemicalReactionExceptionTranslator);

  python::enum_<RDKix::FingerprintType>("FingerprintType")
      .value("AtomPairFP", RDKix::AtomPairFP)
      .value("TopologicalTorsion", RDKix::TopologicalTorsion)
      .value("MorganFP", RDKix::MorganFP)
      .value("RDKixFP", RDKix::RDKixFP)
      .value("PatternFP", RDKix::PatternFP);
  std::string docStringReactionFPParams =
      "A class for storing parameters to manipulate the calculation of "
      "fingerprints of chemical reactions.";

  python::class_<RDKix::ReactionFingerprintParams>(
      "ReactionFingerprintParams", docStringReactionFPParams.c_str(),
      python::init<>("Constructor, takes no arguments"))
      .def(python::init<bool, double, unsigned int, int, unsigned int,
                        RDKix::FingerprintType>())
      .def_readwrite("fpSize", &RDKix::ReactionFingerprintParams::fpSize)
      .def_readwrite("fpType", &RDKix::ReactionFingerprintParams::fpType)
      .def_readwrite("bitRatioAgents",
                     &RDKix::ReactionFingerprintParams::bitRatioAgents)
      .def_readwrite("nonAgentWeight",
                     &RDKix::ReactionFingerprintParams::nonAgentWeight)
      .def_readwrite("agentWeight",
                     &RDKix::ReactionFingerprintParams::agentWeight)
      .def_readwrite("includeAgents",
                     &RDKix::ReactionFingerprintParams::includeAgents);

  std::string docString =
      R"DOC(A class for storing and applying chemical reactions.

Sample Usage:
  >>> from rdkix import Chem
  >>> from rdkix.Chem import rdChemReactions
  >>> rxn = rdChemReactions.ReactionFromSmarts('[C:1](=[O:2])O.[N:3]>>[C:1](=[O:2])[N:3]')
  >>> reacts = (Chem.MolFromSmiles('C(=O)O'),Chem.MolFromSmiles('CNC'))
  >>> products = rxn.RunReactants(reacts)
  >>> len(products)
  1
  >>> len(products[0])
  1
  >>> Chem.MolToSmiles(products[0][0])
  'CN(C)C=O'
)DOC";

  bool noproxy = true;
  RegisterVectorConverter<RDKix::ROMOL_SPTR>("MOL_SPTR_VECT", noproxy);

  python::class_<RDKix::ChemicalReaction>(
      "ChemicalReaction", docString.c_str(),
      python::init<>("Constructor, takes no arguments"))
      .def(python::init<const std::string &>())
      .def(python::init<const RDKix::ChemicalReaction &>())
      .def("GetNumReactantTemplates",
           &RDKix::ChemicalReaction::getNumReactantTemplates,
           "returns the number of reactants this reaction expects")
      .def("GetNumProductTemplates",
           &RDKix::ChemicalReaction::getNumProductTemplates,
           "returns the number of products this reaction generates")
      .def("GetNumAgentTemplates",
           &RDKix::ChemicalReaction::getNumAgentTemplates,
           "returns the number of agents this reaction expects")
      .def("AddReactantTemplate", &RDKix::ChemicalReaction::addReactantTemplate,
           "adds a reactant (a Molecule) to the reaction")
      .def("AddProductTemplate", &RDKix::ChemicalReaction::addProductTemplate,
           "adds a product (a Molecule)")
      .def("AddAgentTemplate", &RDKix::ChemicalReaction::addAgentTemplate,
           "adds a agent (a Molecule)")
      .def("RemoveUnmappedReactantTemplates",
           RDKix::RemoveUnmappedReactantTemplates,
           (python::arg("self"), python::arg("thresholdUnmappedAtoms") = 0.2,
            python::arg("moveToAgentTemplates") = true,
            python::arg("targetList") = python::object()),
           "Removes molecules with an atom mapping ratio below "
           "thresholdUnmappedAtoms from reactant templates to the agent "
           "templates or to a given targetList")
      .def("RemoveUnmappedProductTemplates",
           RDKix::RemoveUnmappedProductTemplates,
           (python::arg("self"), python::arg("thresholdUnmappedAtoms") = 0.2,
            python::arg("moveToAgentTemplates") = true,
            python::arg("targetList") = python::object()),
           "Removes molecules with an atom mapping ratio below "
           "thresholdUnmappedAtoms from product templates to the agent "
           "templates or to a given targetList")
      .def("RemoveAgentTemplates", RDKix::RemoveAgentTemplates,
           (python::arg("self"), python::arg("targetList") = python::object()),
           "Removes agents from reaction. If targetList is provide the agents "
           "will be transferred to that list.")
      .def("RunReactants",
           (PyObject * (*)(RDKix::ChemicalReaction *, python::tuple,
                           unsigned int maxProducts)) RDKix::RunReactants,
           (python::arg("self"), python::arg("reactants"),
            python::arg("maxProducts") = 1000),
           "apply the reaction to a sequence of reactant molecules and return "
           "the products as a tuple of tuples.  If maxProducts is not zero,"
           " stop the reaction when maxProducts have been generated "
           "[default=1000]")
      .def("RunReactants",
           (PyObject * (*)(RDKix::ChemicalReaction *, python::list,
                           unsigned int maxProducts)) RDKix::RunReactants,
           (python::arg("self"), python::arg("reactants"),
            python::arg("maxProducts") = 1000),
           "apply the reaction to a sequence of reactant molecules and return "
           "the products as a tuple of tuples.  If maxProducts is not zero,"
           " stop the reaction when maxProducts have been generated "
           "[default=1000]")
      .def("RunReactant",
           (PyObject * (*)(RDKix::ChemicalReaction *, python::object, unsigned))
               RDKix::RunReactant,
           "apply the reaction to a single reactant")
      .def("RunReactantInPlace", RDKix::RunReactantInPlace,
           (python::arg("self"), python::arg("reactant")),
           "apply the reaction to a single reactant in place. The reactant "
           "itself is modified. This can only be used for single reactant - "
           "single product reactions.")
      .def("Initialize", &RDKix::ChemicalReaction::initReactantMatchers,
           (python::arg("self"), python::arg("silent") = false),
           "initializes the reaction so that it can be used")
      .def("IsInitialized", &RDKix::ChemicalReaction::isInitialized,
           "checks if the reaction is ready for use")
      .def("Validate", &RDKix::ValidateReaction,
           (python::arg("self"), python::arg("silent") = false),
           "checks the reaction for potential problems, returns "
           "(numWarnings,numErrors)")
      .def("GetProductTemplate", &RDKix::GetProductTemplate,
           (python::arg("self"), python::arg("which")),
           python::return_value_policy<python::reference_existing_object>(),
           "returns one of our product templates")
      .def("GetReactantTemplate", &RDKix::GetReactantTemplate,
           (python::arg("self"), python::arg("which")),
           python::return_value_policy<python::reference_existing_object>(),
           "returns one of our reactant templates")
      .def("GetAgentTemplate", &RDKix::GetAgentTemplate,
           (python::arg("self"), python::arg("which")),
           python::return_value_policy<python::reference_existing_object>(),
           "returns one of our agent templates")
      .def("_setImplicitPropertiesFlag",
           &RDKix::ChemicalReaction::setImplicitPropertiesFlag,
           (python::arg("self"), python::arg("val")),
           "EXPERT USER: indicates that the reaction can have implicit "
           "properties")
      .def("_getImplicitPropertiesFlag",
           &RDKix::ChemicalReaction::getImplicitPropertiesFlag,
           (python::arg("self")),
           "EXPERT USER: returns whether or not the reaction can have implicit "
           "properties")
      .def("ToBinary", RDKix::ReactionToBinary, (python::arg("self")),
           "Returns a binary string representation of the reaction.")
      .def("ToBinary", RDKix::ReactionToBinaryWithProps,
           (python::arg("self"), python::arg("propertyFlags")),
           "Returns a binary string representation of the reaction.")
      .def("IsMoleculeReactant", RDKix::IsMoleculeReactantOfReaction,
           "returns whether or not the molecule has a substructure match to "
           "one of the reactants.")
      .def("IsMoleculeProduct", RDKix::IsMoleculeProductOfReaction,
           "returns whether or not the molecule has a substructure match to "
           "one of the products.")
      .def("IsMoleculeAgent", RDKix::IsMoleculeAgentOfReaction,
           "returns whether or not the molecule has a substructure match to "
           "one of the agents.")
      .def("GetReactingAtoms", &RDKix::GetReactingAtoms,
           (python::arg("self"), python::arg("mappedAtomsOnly") = false),
           "returns a sequence of sequences with the atoms that change in the "
           "reaction")
      .def("AddRecursiveQueriesToReaction",
           RDKix::AddRecursiveQueriesToReaction,
           (python::arg("reaction"), python::arg("queries") = python::dict(),
            python::arg("propName") = "molFileValue",
            python::arg("getLabels") = false),
           "adds recursive queries and returns reactant labels")

      .def("GetReactants", &RDKix::ChemicalReaction::getReactants,
           python::return_value_policy<python::reference_existing_object>(),
           "get the reactant templates")
      .def("GetProducts", &RDKix::ChemicalReaction::getProducts,
           python::return_value_policy<python::reference_existing_object>(),
           "get the product templates")
      .def("GetAgents", &RDKix::ChemicalReaction::getAgents,
           python::return_value_policy<python::reference_existing_object>(),
           "get the agent templates")
      .def("GetSubstructParams", RDKix::getParamsHelper,
           python::return_value_policy<
               python::reference_existing_object,
               python::with_custodian_and_ward_postcall<0, 1>>(),
           "get the parameter object controlling the substructure matching")

      // properties
      .def("SetProp", RDKix::MolSetProp<RDKix::ChemicalReaction, std::string>,
           (python::arg("self"), python::arg("key"), python::arg("val"),
            python::arg("computed") = false),
           "Sets a molecular property\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to be set (a string).\n"
           "    - value: the property value (a string).\n"
           "    - computed: (optional) marks the property as being "
           "computed.\n"
           "                Defaults to False.\n\n")
      .def("SetDoubleProp", RDKix::MolSetProp<RDKix::ChemicalReaction, double>,
           (python::arg("self"), python::arg("key"), python::arg("val"),
            python::arg("computed") = false),
           "Sets a double valued molecular property\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to be set (a string).\n"
           "    - value: the property value as a double.\n"
           "    - computed: (optional) marks the property as being "
           "computed.\n"
           "                Defaults to 0.\n\n")
      .def("SetIntProp", RDKix::MolSetProp<RDKix::ChemicalReaction, int>,
           (python::arg("self"), python::arg("key"), python::arg("val"),
            python::arg("computed") = false),
           "Sets an integer valued molecular property\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to be set (an unsigned "
           "number).\n"
           "    - value: the property value as an integer.\n"
           "    - computed: (optional) marks the property as being "
           "computed.\n"
           "                Defaults to False.\n\n")
      .def("SetUnsignedProp",
           RDKix::MolSetProp<RDKix::ChemicalReaction, unsigned int>,
           (python::arg("self"), python::arg("key"), python::arg("val"),
            python::arg("computed") = false),
           "Sets an unsigned integer valued molecular property\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to be set (a string).\n"
           "    - value: the property value as an unsigned integer.\n"
           "    - computed: (optional) marks the property as being "
           "computed.\n"
           "                Defaults to False.\n\n")
      .def("SetBoolProp", RDKix::MolSetProp<RDKix::ChemicalReaction, bool>,
           (python::arg("self"), python::arg("key"), python::arg("val"),
            python::arg("computed") = false),
           "Sets a boolean valued molecular property\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to be set (a string).\n"
           "    - value: the property value as a bool.\n"
           "    - computed: (optional) marks the property as being "
           "computed.\n"
           "                Defaults to False.\n\n")
      .def("HasProp", RDKix::MolHasProp<RDKix::ChemicalReaction>,
           "Queries a molecule to see if a particular property has been "
           "assigned.\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to check for (a string).\n")
      .def("GetProp", RDKix::GetProp<RDKix::ChemicalReaction, std::string>,
           "Returns the value of the property.\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to return (a string).\n\n"
           "  RETURNS: a string\n\n"
           "  NOTE:\n"
           "    - If the property has not been set, a KeyError exception "
           "will be raised.\n")
      .def("GetDoubleProp", RDKix::GetProp<RDKix::ChemicalReaction, double>,
           "Returns the double value of the property if possible.\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to return (a string).\n\n"
           "  RETURNS: a double\n\n"
           "  NOTE:\n"
           "    - If the property has not been set, a KeyError exception "
           "will be raised.\n")
      .def("GetIntProp", RDKix::GetProp<RDKix::ChemicalReaction, int>,
           "Returns the integer value of the property if possible.\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to return (a string).\n\n"
           "  RETURNS: an integer\n\n"
           "  NOTE:\n"
           "    - If the property has not been set, a KeyError exception "
           "will be raised.\n")
      .def("GetUnsignedProp",
           RDKix::GetProp<RDKix::ChemicalReaction, unsigned int>,
           "Returns the unsigned int value of the property if possible.\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to return (a string).\n\n"
           "  RETURNS: an unsigned integer\n\n"
           "  NOTE:\n"
           "    - If the property has not been set, a KeyError exception "
           "will be raised.\n")
      .def("GetBoolProp", RDKix::GetProp<RDKix::ChemicalReaction, bool>,
           "Returns the Bool value of the property if possible.\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to return (a string).\n\n"
           "  RETURNS: a bool\n\n"
           "  NOTE:\n"
           "    - If the property has not been set, a KeyError exception "
           "will be raised.\n")
      .def("ClearProp", RDKix::MolClearProp<RDKix::ChemicalReaction>,
           "Removes a property from the reaction.\n\n"
           "  ARGUMENTS:\n"
           "    - key: the name of the property to clear (a string).\n")

      .def("ClearComputedProps",
           RDKix::MolClearComputedProps<RDKix::ChemicalReaction>,
           "Removes all computed properties from the reaction.\n\n")

      .def("GetPropNames", &RDKix::ChemicalReaction::getPropList,
           (python::arg("self"), python::arg("includePrivate") = false,
            python::arg("includeComputed") = false),
           "Returns a tuple with all property names for this reaction.\n\n"
           "  ARGUMENTS:\n"
           "    - includePrivate: (optional) toggles inclusion of private "
           "properties in the result set.\n"
           "                      Defaults to 0.\n"
           "    - includeComputed: (optional) toggles inclusion of computed "
           "properties in the result set.\n"
           "                      Defaults to 0.\n\n"
           "  RETURNS: a tuple of strings\n")

      .def("GetPropsAsDict", RDKix::GetPropsAsDict<RDKix::ChemicalReaction>,
           (python::arg("self"), python::arg("includePrivate") = false,
            python::arg("includeComputed") = false),
           "Returns a dictionary populated with the reaction's properties.\n"
           " n.b. Some properties are not able to be converted to python "
           "types.\n\n"
           "  ARGUMENTS:\n"
           "    - includePrivate: (optional) toggles inclusion of private "
           "properties in the result set.\n"
           "                      Defaults to False.\n"
           "    - includeComputed: (optional) toggles inclusion of computed "
           "properties in the result set.\n"
           "                      Defaults to False.\n\n"
           "  RETURNS: a dictionary\n")

      // enable pickle support
      .def_pickle(RDKix::reaction_pickle_suite());

  python::def(
      "ReactionFromSmarts", RDKix::ReactionFromSmarts,
      (python::arg("SMARTS"), python::arg("replacements") = python::dict(),
       python::arg("useSmiles") = false),
      "construct a ChemicalReaction from a reaction SMARTS string. \n\
see the documentation for rdkix.Chem.MolFromSmiles for an explanation\n\
of the replacements argument.",
      python::return_value_policy<python::manage_new_object>());
  python::def("ReactionToSmarts", RDKix::ChemicalReactionToRxnSmarts,
              (python::arg("reaction")),
              "construct a reaction SMARTS string for a ChemicalReaction");
  python::def("ReactionToSmiles", RDKix::ChemicalReactionToRxnSmiles,
              (python::arg("reaction"), python::arg("canonical") = true),
              "construct a reaction SMILES string for a ChemicalReaction");

  python::def(
      "ReactionFromRxnFile", RDKix::RxnFileToChemicalReaction,
      (python::arg("filename"), python::arg("sanitize") = false,
       python::arg("removeHs") = false, python::arg("strictParsing") = true),
      "construct a ChemicalReaction from an MDL rxn file",
      python::return_value_policy<python::manage_new_object>());
  python::def(
      "ReactionFromRxnBlock", RDKix::RxnBlockToChemicalReaction,
      (python::arg("rxnblock"), python::arg("sanitize") = false,
       python::arg("removeHs") = false, python::arg("strictParsing") = true),
      "construct a ChemicalReaction from a string in MDL rxn format",
      python::return_value_policy<python::manage_new_object>());
  python::def("ReactionToRxnBlock", RDKix::ChemicalReactionToRxnBlock,
              (python::arg("reaction"), python::arg("separateAgents") = false,
               python::arg("forceV3000") = false),
              "construct a string in MDL rxn format for a ChemicalReaction");
  python::def(
      "ReactionToV3KRxnBlock", RDKix::ChemicalReactionToV3KRxnBlock,
      (python::arg("reaction"), python::arg("separateAgents") = false),
      "construct a string in MDL v3000 rxn format for a ChemicalReaction");

  python::def("ReactionFromPNGFile", RDKix::PNGFileToChemicalReaction,
              "construct a ChemicalReaction from metadata in a PNG file",
              python::return_value_policy<python::manage_new_object>());
  python::def("ReactionFromPNGString", RDKix::PNGStringToChemicalReaction,
              "construct a ChemicalReaction from an string with PNG data",
              python::return_value_policy<python::manage_new_object>());
  python::def(
      "ReactionMetadataToPNGFile", RDKix::addReactionToPNGFileHelper,
      (python::arg("mol"), python::arg("filename"),
       python::arg("includePkl") = true, python::arg("includeSmiles") = true,
       python::arg("includeSmarts") = false, python::arg("includeMol") = false),
      "Reads the contents of a PNG file and adds metadata about a reaction to "
      "it. The modified file contents are returned.");
  python::def(
      "ReactionMetadataToPNGString", RDKix::addReactionToPNGStringHelper,
      (python::arg("mol"), python::arg("pngdata"),
       python::arg("includePkl") = true, python::arg("includeSmiles") = true,
       python::arg("includeSmarts") = false, python::arg("includeRxn") = false),
      "Adds metadata about a reaction to the PNG string passed in."
      "The modified string is returned.");

  python::def("ReactionFromMolecule", RDKix::RxnMolToChemicalReaction,
              "construct a ChemicalReaction from an molecule if the RXN role "
              "property of the molecule is set",
              python::return_value_policy<python::manage_new_object>());
  python::def(
      "ReactionToMolecule", RDKix::ChemicalReactionToRxnMol,
      (python::arg("reaction")),
      "construct a molecule for a ChemicalReaction with RXN role property set",
      python::return_value_policy<python::manage_new_object>());

  docString =
      R"DOC(Compute 2D coordinates for a reaction. 
  ARGUMENTS: 
     - reaction - the reaction of interest
     - spacing - the amount of space left between components of the reaction
     - canonOrient - orient the reactants and products in a canonical way
     - updateProps - if set, properties such as conjugation and
        hybridization will be calculated for the reactant and product
        templates before generating coordinates. This should result in
        better depictions, but can lead to errors in some cases.
     - nFlipsPerSample - number of rotatable bonds that are
                flipped at random at a time.
     - nSample - Number of random samplings of rotatable bonds.
     - sampleSeed - seed for the random sampling process.
     - permuteDeg4Nodes - allow permutation of bonds at a degree 4
                 node during the sampling process 
     - bondLength - change the default bond length for depiction
)DOC";
  python::def(
      "Compute2DCoordsForReaction", RDKix::Compute2DCoordsForReaction,
      (python::arg("reaction"), python::arg("spacing") = 2.0,
       python::arg("updateProps") = true, python::arg("canonOrient") = true,
       python::arg("nFlipsPerSample") = 0, python::arg("nSample") = 0,
       python::arg("sampleSeed") = 0, python::arg("permuteDeg4Nodes") = false,
       python::arg("bondLength") = -1.0),
      docString.c_str());

  python::def(
      "CreateDifferenceFingerprintForReaction",
      RDKix::DifferenceFingerprintChemReaction,
      (python::arg("reaction"), python::arg("ReactionFingerPrintParams") =
                                    RDKix::DefaultDifferenceFPParams),
      "construct a difference fingerprint for a ChemicalReaction by "
      "subtracting the reactant "
      "fingerprint from the product fingerprint",
      python::return_value_policy<python::manage_new_object>());

  python::def(
      "CreateStructuralFingerprintForReaction",
      RDKix::StructuralFingerprintChemReaction,
      (python::arg("reaction"), python::arg("ReactionFingerPrintParams") =
                                    RDKix::DefaultStructuralFPParams),
      "construct a structural fingerprint for a ChemicalReaction by "
      "concatenating the reactant "
      "fingerprint and the product fingerprint",
      python::return_value_policy<python::manage_new_object>());

  python::def("IsReactionTemplateMoleculeAgent",
              RDKix::isReactionTemplateMoleculeAgent,
              (python::arg("molecule"), python::arg("agentThreshold")),
              "tests if a molecule can be classified as an agent depending on "
              "the ratio of mapped atoms and a give threshold");
  python::def("HasReactionAtomMapping", RDKix::hasReactionAtomMapping,
              "tests if a reaction obtains any atom mapping");
  python::def("HasReactionSubstructMatch", RDKix::hasReactionSubstructMatch,
              (python::arg("reaction"), python::arg("queryReaction"),
               python::arg("includeAgents") = false),
              "tests if the queryReaction is a substructure of a reaction");
  python::def("HasAgentTemplateSubstructMatch",
              RDKix::hasAgentTemplateSubstructMatch,
              (python::arg("reaction"), python::arg("queryReaction")),
              "tests if the agents of a queryReaction are the same as those of "
              "a reaction");
  python::def("HasProductTemplateSubstructMatch",
              RDKix::hasProductTemplateSubstructMatch,
              (python::arg("reaction"), python::arg("queryReaction")),
              "tests if the products of a queryReaction are substructures of "
              "the products of a reaction");
  python::def("HasReactantTemplateSubstructMatch",
              RDKix::hasReactantTemplateSubstructMatch,
              (python::arg("reaction"), python::arg("queryReaction")),
              "tests if the reactants of a queryReaction are substructures of "
              "the reactants of a reaction");
  python::def(
      "UpdateProductsStereochemistry", RDKix::updateProductsStereochem,
      (python::arg("reaction")),
      "Caution: This is an expert-user function which will change a property (molInversionFlag) of your products.\
          This function is called by default using the RXN or SMARTS parser for reactions and should really only be called if reactions have been constructed some other way.\
          The function updates the stereochemistry of the product by considering 4 different cases: inversion, retention, removal, and introduction");

  python::def(
      "ReduceProductToSideChains", RDKix::reduceProductToSideChains,
      (python::arg("product"), python::arg("addDummyAtoms") = true),
      "reduce the product of a reaction to the side chains added by the reaction.\
              The output is a molecule with attached wildcards indicating where the product was attached.\
              The dummy atom has the same reaction-map number as the product atom (if available).",
      python::return_value_policy<python::manage_new_object>());

  python::def("RemoveMappingNumbersFromReactions",
              RDKix::removeMappingNumbersFromReactions,
              (python::arg("reaction")),
              "Removes the mapping numbers from the molecules of a reaction");

  docString =
      R"DOC(A function for preprocessing reactions with more specific queries.
Queries are indicated by labels on atoms (molFileAlias property by default)
When these labels are found, more specific queries are placed on the atoms.
By default, the available quieries come from 
  FilterCatalog.GetFlattenedFunctionalGroupHierarchy(True)n
Sample Usage:
  >>> from rdkix import Chem, RDConfig
  >>> from rdkix.Chem import MolFromSmiles, AllChem
  >>> from rdkix.Chem.rdChemReactions import PreprocessReaction
  >>> import os
  >>> testFile = os.path.join(RDConfig.RDCodeDir,'Chem','SimpleEnum','test_data','boronic1.rxn')
  >>> rxn = AllChem.ReactionFromRxnFile(testFile)
  >>> rxn.Initialize()
  >>> nWarn,nError,nReacts,nProds,reactantLabels = PreprocessReaction(rxn)
  >>> nWarn
  0
  >>> nError
  0
  >>> nReacts
  2
  >>> nProds
  1
  >>> reactantLabels
  (((0, 'halogen.bromine.aromatic'),), ((1, 'boronicacid'),))

If there are functional group labels in the input reaction (via atoms with molFileValue properties),
the corresponding atoms will have queries added to them so that they only match such things. We can
see this here:
  >>> rxn = AllChem.ReactionFromRxnFile(testFile)
  >>> rxn.Initialize()
  >>> r1 = rxn.GetReactantTemplate(0)
  >>> m1 = Chem.MolFromSmiles('CCBr')
  >>> m2 = Chem.MolFromSmiles('c1ccccc1Br')
  
These both match because the reaction file itself just has R1-Br:
  >>> m1.HasSubstructMatch(r1)
  True
  >>> m2.HasSubstructMatch(r1)
  True

After preprocessing, we only match the aromatic Br:
  >>> d = PreprocessReaction(rxn)
  >>> m1.HasSubstructMatch(r1)
  False
  >>> m2.HasSubstructMatch(r1)
  True

We also support or queries in the values field (separated by commas):
  >>> testFile = os.path.join(RDConfig.RDCodeDir,'Chem','SimpleEnum','test_data','azide_reaction.rxn')
  >>> rxn = AllChem.ReactionFromRxnFile(testFile)
  >>> rxn.Initialize()
  >>> reactantLabels = PreprocessReaction(rxn)[-1]
  >>> reactantLabels
  (((1, 'azide'),), ((1, 'carboxylicacid,acidchloride'),))
  >>> m1 = Chem.MolFromSmiles('CC(=O)O')
  >>> m2 = Chem.MolFromSmiles('CC(=O)Cl')
  >>> m3 = Chem.MolFromSmiles('CC(=O)N')
  >>> r2 = rxn.GetReactantTemplate(1)
  >>> m1.HasSubstructMatch(r2)
  True
  >>> m2.HasSubstructMatch(r2)
  True
  >>> m3.HasSubstructMatch(r2)
  False

unrecognized final group types are returned as None:
  >>> testFile = os.path.join(RDConfig.RDCodeDir,'Chem','SimpleEnum','test_data','bad_value1.rxn')
  >>> rxn = AllChem.ReactionFromRxnFile(testFile)
  >>> rxn.Initialize()
  >>> nWarn,nError,nReacts,nProds,reactantLabels = PreprocessReaction(rxn)
  Traceback (most recent call last):
    ...
  KeyError: 'boromicacid'

One unrecognized group type in a comma-separated list makes the whole thing fail:
  >>> testFile = os.path.join(RDConfig.RDCodeDir,'Chem','SimpleEnum','test_data','bad_value2.rxn')
  >>> rxn = AllChem.ReactionFromRxnFile(testFile)
  >>> rxn.Initialize()
  >>> nWarn,nError,nReacts,nProds,reactantLabels = PreprocessReaction(rxn)
  Traceback (most recent call last):
    ...
  KeyError: 'carboxylicacid,acidchlroide'
  >>> testFile = os.path.join(RDConfig.RDCodeDir,'Chem','SimpleEnum','test_data','bad_value3.rxn')
  >>> rxn = AllChem.ReactionFromRxnFile(testFile)
  >>> rxn.Initialize()
  >>> nWarn,nError,nReacts,nProds,reactantLabels = PreprocessReaction(rxn)
  Traceback (most recent call last):
    ...
  KeyError: 'carboxyliccaid,acidchloride'
  >>> rxn = rdChemReactions.ChemicalReaction()
  >>> rxn.Initialize()
  >>> nWarn,nError,nReacts,nProds,reactantLabels = PreprocessReaction(rxn)
  >>> reactantLabels
  ()
  >>> reactantLabels == ()
  True
)DOC";

  python::def(
      "PreprocessReaction", RDKix::PreprocessReaction,
      (python::arg("reaction"), python::arg("queries") = python::dict(),
       python::arg("propName") = RDKix::common_properties::molFileValue),
      docString.c_str());

  python::enum_<RDKix::RxnOps::SanitizeRxnFlags>("SanitizeFlags")
      .value("SANITIZE_NONE", RDKix::RxnOps::SANITIZE_NONE)
      .value("SANITIZE_ATOM_MAPS", RDKix::RxnOps::SANITIZE_ATOM_MAPS)
      .value("SANITIZE_RGROUP_NAMES", RDKix::RxnOps::SANITIZE_RGROUP_NAMES)
      .value("SANITIZE_ADJUST_REACTANTS",
             RDKix::RxnOps::SANITIZE_ADJUST_REACTANTS)
      .value("SANITIZE_MERGEHS", RDKix::RxnOps::SANITIZE_MERGEHS)
      .value("SANITIZE_ALL", RDKix::RxnOps::SANITIZE_ALL)
      .export_values();
  ;

  python::def(
      "GetDefaultAdjustParams", RDKix::RxnOps::DefaultRxnAdjustParams,
      "Returns the default adjustment parameters for reactant templates");

  python::def("GetChemDrawRxnAdjustParams",
              RDKix::RxnOps::ChemDrawRxnAdjustParams,
              "(deprecated, see MatchOnlyAtRgroupsAdjustParams)\n\tReturns the "
              "chemdraw style adjustment parameters for reactant templates");

  python::def(
      "MatchOnlyAtRgroupsAdjustParams",
      RDKix::RxnOps::MatchOnlyAtRgroupsAdjustParams,
      "Only match at the specified rgroup locations in the reactant templates");

  docString =
      R"DOC(Does some sanitization of the reactant and product templates of a reaction.

    - The reaction is modified in place.
    - If sanitization fails, an exception will be thrown unless catchErrors is set

  ARGUMENTS:

    - rxn: the reaction to be modified
    - sanitizeOps: (optional) reaction sanitization operations to be carried out
      these should be constructed by or'ing together the
      operations in rdkix.Chem.rdChemReactions.SanitizeFlags
    - optional adjustment parameters for changing the meaning of the substructure
      matching done in the templates.  The default is 
      rdkix.Chem.rdChemReactions.DefaultRxnAdjustParams which aromatizes
      kekule structures if possible.
    - catchErrors: (optional) if provided, instead of raising an exception
      when sanitization fails (the default behavior), the 
      first operation that failed (as defined in rdkix.Chem.rdChemReactions.SanitizeFlags)
      is returned. Zero is returned on success.

  The operations carried out by default are:
    1) fixRGroups(): sets R group labels on mapped dummy atoms when possible
    2) fixAtomMaps(): attempts to set atom maps on unmapped R groups
    3) adjustTemplate(): calls adjustQueryProperties() on all reactant templates
    4) fixHs(): merges explicit Hs in the reactant templates that don't map to heavy atoms
)DOC";
  python::def("SanitizeRxn", RDKix::sanitizeReaction,
              (python::arg("rxn"),
               python::arg("sanitizeOps") =
                   rdcast<unsigned int>(RDKix::RxnOps::SANITIZE_ALL),
               python::arg("params") = RDKix::RxnOps::DefaultRxnAdjustParams(),
               python::arg("catchErrors") = false),
              docString.c_str());

  wrap_enumeration();
}
