//
//  Copyright (C) 2018 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/utils.h>
#include <RDGeneral/Invariant.h>
#include <RDGeneral/RDLog.h>
#include <GraphMol/RDKixBase.h>
#include <Geometry/point.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/FileParsers/FileParsers.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <sstream>
#include <vector>
#include <string>
namespace pt = boost::property_tree;

namespace RDKix {
namespace {
void ptreeToMol(RWMol *mol, const pt::ptree &molE) {
  PRECONDITION(mol, "no molecule");
  std::vector<RDGeom::Point3D> pts;
  bool is3D = false;
  for (const auto &atE : molE) {
    if (atE.first == "rdkix:atom") {
      std::string asmi = atE.second.get<std::string>("<xmlattr>.atom-smiles");
      Atom *atom = SmilesToAtom(asmi);
      if (!atom) {
        BOOST_LOG(rdWarningLog) << " Could not convert SMILES '" << asmi
                                << "' to a atom. Ignoring it." << std::endl;
        continue;
      }

      bool updateLabel = false, takeOwnership = true;
      mol->addAtom(atom, updateLabel, takeOwnership);
      RDGeom::Point3D pt(atE.second.get<double>("<xmlattr>.x", 0.0),
                         atE.second.get<double>("<xmlattr>.y", 0.0),
                         atE.second.get<double>("<xmlattr>.z", 0.0));
      pts.push_back(pt);
      if (atE.second.get<std::string>("<xmlattr>.z", "0") != "0") {
        is3D = true;
      }
    }
  }
  for (const auto &atE : molE) {
    if (atE.first == "rdkix:bond") {
      std::string asmi = atE.second.get<std::string>("<xmlattr>.bond-smiles");
      Bond *bond = SmilesToBond(asmi);
      if (!bond) {
        BOOST_LOG(rdWarningLog) << " Could not convert SMILES '" << asmi
                                << "' to a bond. Ignoring it." << std::endl;
        continue;
      }
      bond->setBeginAtomIdx(atE.second.get<int>("<xmlattr>.begin-atom-idx") -
                            1);
      bond->setEndAtomIdx(atE.second.get<int>("<xmlattr>.end-atom-idx") - 1);
      bool takeOwnership = true;
      mol->addBond(bond, takeOwnership);
    }
  }
  auto *conf = new Conformer(mol->getNumAtoms());
  for (unsigned int i = 0; i < mol->getNumAtoms(); ++i) {
    conf->setAtomPos(i, pts[i]);
  }
  conf->set3D(is3D);
  mol->addConformer(conf, true);
}
}  // namespace

RWMol *RDKixSVGToMol(std::istream *instream, bool sanitize, bool removeHs) {
  PRECONDITION(instream, "bad stream");
  pt::ptree tree;
  pt::read_xml(*instream, tree);
  RWMol *res = nullptr;
  pt::ptree empty_ptree;
  // const pt::ptree &childE = tree.get_child("svg", empty_ptree);
  const pt::ptree &molsE = tree.get_child("svg.metadata", empty_ptree);
  for (const auto &molE : molsE) {
    if (molE.first == "rdkix:mol") {
      res = new RWMol();
      ptreeToMol(res, molE.second);
      if (res->getNumAtoms()) {
        if (removeHs) {
          bool implicitOnly = false, updateExplicitCount = false;
          MolOps::removeHs(*res, implicitOnly, updateExplicitCount, sanitize);
        } else if (sanitize) {
          MolOps::sanitizeMol(*res);
        }
      }
    }
  }
  return res;
}
RWMol *RDKixSVGToMol(const std::string &svg, bool sanitize, bool removeHs) {
  std::stringstream iss(svg);
  return RDKixSVGToMol(&iss, sanitize, removeHs);
}
}  // namespace RDKix
