//
//  Copyright (C) 2015 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#define PY_ARRAY_UNIQUE_SYMBOL rdmmpa_array_API
#include <boost/python.hpp>
#include <GraphMol/ROMol.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <RDBoost/Wrap.h>
#include <GraphMol/MMPA/MMPA.h>

namespace python = boost::python;

namespace {
python::tuple fragmentMolHelper(const RDKix::ROMol &mol, unsigned int maxCuts,
                                unsigned int maxCutBonds,
                                const std::string &pattern,
                                bool resultsAsMols) {
  std::vector<std::pair<RDKix::ROMOL_SPTR, RDKix::ROMOL_SPTR>> tres;
  bool ok = RDKix::MMPA::fragmentMol(mol, tres, maxCuts, maxCutBonds, pattern);
  python::list pyres;
  if (ok) {
    for (std::vector<std::pair<RDKix::ROMOL_SPTR, RDKix::ROMOL_SPTR>>::
             const_iterator pr = tres.begin();
         pr != tres.end(); ++pr) {
      python::list lres;
      if (resultsAsMols) {
        lres.append(pr->first);
        lres.append(pr->second);
      } else {
        if (pr->first) {
          lres.append(RDKix::MolToSmiles(*(pr->first), true));
        } else {
          lres.append("");
        }
        lres.append(RDKix::MolToSmiles(*(pr->second), true));
      }
      pyres.append(python::tuple(lres));
    }
  }
  return python::tuple(pyres);
}

python::tuple fragmentMolHelper2(const RDKix::ROMol &mol, unsigned int minCuts,
                                 unsigned int maxCuts, unsigned int maxCutBonds,
                                 const std::string &pattern,
                                 bool resultsAsMols) {
  std::vector<std::pair<RDKix::ROMOL_SPTR, RDKix::ROMOL_SPTR>> tres;
  bool ok = RDKix::MMPA::fragmentMol(mol, tres, minCuts, maxCuts, maxCutBonds,
                                     pattern);
  python::list pyres;
  if (ok) {
    for (std::vector<std::pair<RDKix::ROMOL_SPTR, RDKix::ROMOL_SPTR>>::
             const_iterator pr = tres.begin();
         pr != tres.end(); ++pr) {
      python::list lres;
      if (resultsAsMols) {
        lres.append(pr->first);
        lres.append(pr->second);
      } else {
        if (pr->first) {
          lres.append(RDKix::MolToSmiles(*(pr->first), true));
        } else {
          lres.append("");
        }
        lres.append(RDKix::MolToSmiles(*(pr->second), true));
      }
      pyres.append(python::tuple(lres));
    }
  }
  return python::tuple(pyres);
}

python::tuple fragmentMolHelper3(const RDKix::ROMol &mol, python::object ob,
                                 unsigned int minCuts, unsigned int maxCuts,
                                 bool resultsAsMols) {
  std::vector<std::pair<RDKix::ROMOL_SPTR, RDKix::ROMOL_SPTR>> tres;
  std::unique_ptr<std::vector<unsigned int>> v =
      pythonObjectToVect<unsigned int>(ob);
  if (!v) {
    throw_value_error("bondsToCut must be non-empty");
  }
  bool ok = RDKix::MMPA::fragmentMol(mol, tres, *v, minCuts, maxCuts);
  python::list pyres;
  if (ok) {
    for (std::vector<std::pair<RDKix::ROMOL_SPTR, RDKix::ROMOL_SPTR>>::
             const_iterator pr = tres.begin();
         pr != tres.end(); ++pr) {
      python::list lres;
      if (resultsAsMols) {
        lres.append(pr->first);
        lres.append(pr->second);
      } else {
        if (pr->first) {
          lres.append(RDKix::MolToSmiles(*(pr->first), true));
        } else {
          lres.append("");
        }
        lres.append(RDKix::MolToSmiles(*(pr->second), true));
      }
      pyres.append(python::tuple(lres));
    }
  }
  return python::tuple(pyres);
}

}  // namespace

BOOST_PYTHON_MODULE(rdMMPA) {
  python::scope().attr("__doc__") =
      "Module containing a C++ implementation of code for doing MMPA";

  std::string docString =
      "Does the fragmentation necessary for an MMPA analysis";
  python::def("FragmentMol", fragmentMolHelper,
              (python::arg("mol"), python::arg("maxCuts") = 3,
               python::arg("maxCutBonds") = 20,
               python::arg("pattern") = "[#6+0;!$(*=,#[!#6])]!@!=!#[*]",
               python::arg("resultsAsMols") = true),
              docString.c_str());

  python::def("FragmentMol", fragmentMolHelper2,
              (python::arg("mol"), python::arg("minCuts"),
               python::arg("maxCuts"), python::arg("maxCutBonds"),
               python::arg("pattern") = "[#6+0;!$(*=,#[!#6])]!@!=!#[*]",
               python::arg("resultsAsMols") = true),
              docString.c_str());

  python::def("FragmentMol", fragmentMolHelper3,
              (python::arg("mol"), python::arg("bondsToCut"),
               python::arg("minCuts") = 1, python::arg("maxCuts") = 3,
               python::arg("resultsAsMols") = true),
              docString.c_str());
}
