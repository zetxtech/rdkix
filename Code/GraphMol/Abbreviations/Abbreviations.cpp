//
//  Copyright (C) 2020 Greg Landrum and T5 Informatics GmbH
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include "Abbreviations.h"
#include <GraphMol/RDKixBase.h>
#include <RDGeneral/types.h>
#include <RDGeneral/Invariant.h>

#include <boost/dynamic_bitset.hpp>
#include <iostream>

namespace RDKix {

namespace Abbreviations {

void applyMatches(RWMol& mol, const std::vector<AbbreviationMatch>& matches) {
  boost::dynamic_bitset<> atomsToRemove(mol.getNumAtoms());
  boost::dynamic_bitset<> bondsToRemove(mol.getNumBonds());
  std::vector<unsigned int> prevAtomMapping;
  std::vector<unsigned int> prevBondMapping;
  std::vector<unsigned int> addedBonds;
  addedBonds.reserve(mol.getNumBonds());
  bool hasPrevMapping =
      mol.getPropIfPresent(common_properties::origAtomMapping,
                           prevAtomMapping) &&
      mol.getPropIfPresent(common_properties::origBondMapping, prevBondMapping);
  for (const auto& amatch : matches) {
    // throughout this remember that atom 0 in the match is the dummy

    // convert atom 1 to be the abbreviation so that we don't have to
    // worry about messing up chirality, etc.
    auto connectIdx = amatch.match.at(1).second;
    auto connectingAtom = mol.getAtomWithIdx(connectIdx);
    connectingAtom->setProp(RDKix::common_properties::atomLabel,
                            amatch.abbrev.label);
    if (!amatch.abbrev.displayLabel.empty()) {
      connectingAtom->setProp(RDKix::common_properties::_displayLabel,
                              amatch.abbrev.displayLabel);
    }
    if (!amatch.abbrev.displayLabelW.empty()) {
      connectingAtom->setProp(RDKix::common_properties::_displayLabelW,
                              amatch.abbrev.displayLabelW);
    }

    connectingAtom->setFormalCharge(0);
    connectingAtom->setAtomicNum(0);
    connectingAtom->setIsotope(0);
    connectingAtom->setIsAromatic(false);

    // set the hybridization so these are drawn linearly
    connectingAtom->setHybridization(Atom::HybridizationType::SP);

    for (unsigned int i = 2; i < amatch.match.size(); ++i) {
      const auto& pr = amatch.match.at(i);
      CHECK_INVARIANT(!atomsToRemove[pr.second], "overlapping matches");
      atomsToRemove.set(pr.second);
      for (const auto bond : mol.atomBonds(mol.getAtomWithIdx(pr.second))) {
        bondsToRemove.set(bond->getIdx());
      }
      // if there's a molecule associated with the match, check to see if
      // additional bonds need to be formed
      if (amatch.abbrev.mol &&
          mol.getAtomWithIdx(pr.second)->getDegree() >
              amatch.abbrev.mol->getAtomWithIdx(pr.first)->getDegree()) {
        for (const auto bond : mol.atomBonds(mol.getAtomWithIdx(pr.second))) {
          // if this neighbor isn't in the match:
          auto nbrIdx = bond->getOtherAtomIdx(pr.second);
          if (!std::any_of(amatch.match.begin(), amatch.match.end(),
                           [nbrIdx](const std::pair<int, int>& tpr) {
                             return tpr.second == rdcast<int>(nbrIdx);
                           })) {
            mol.addBond(nbrIdx, connectIdx, Bond::BondType::SINGLE);
            addedBonds.push_back(hasPrevMapping
                                     ? prevBondMapping.at(bond->getIdx())
                                     : bond->getIdx());
          }
        }
      }
    }
    // make connections between any extraAttachAtoms and the connection point
    for (auto oaidx : amatch.abbrev.extraAttachAtoms) {
      int bondIdx = -1;
      for (const auto bond : mol.atomBonds(mol.getAtomWithIdx(oaidx))) {
        if (bondsToRemove.test(bond->getIdx())) {
          CHECK_INVARIANT(bondIdx == -1, "bondIdx must be unique");
          bondIdx = bond->getIdx();
        }
      }
      CHECK_INVARIANT(bondIdx != -1, "bondIdx not found");
      mol.addBond(oaidx, connectIdx, Bond::BondType::SINGLE);
      addedBonds.push_back(hasPrevMapping ? prevBondMapping.at(bondIdx)
                                          : bondIdx);
    }
  }
  std::vector<unsigned int> atomMapping;
  atomMapping.reserve(mol.getNumAtoms());
  std::vector<unsigned int> bondMapping;
  bondMapping.reserve(mol.getNumBonds());
  mol.beginBatchEdit();
  for (unsigned int i = 0; i < atomsToRemove.size(); ++i) {
    if (atomsToRemove[i]) {
      mol.removeAtom(i);
    } else {
      atomMapping.push_back(hasPrevMapping ? prevAtomMapping.at(i) : i);
    }
  }
  for (unsigned int i = 0; i < bondsToRemove.size(); ++i) {
    if (!bondsToRemove[i]) {
      bondMapping.push_back(hasPrevMapping ? prevBondMapping.at(i) : i);
    }
  }
  mol.commitBatchEdit();
  bondMapping.insert(bondMapping.end(), addedBonds.begin(), addedBonds.end());
  CHECK_INVARIANT(atomMapping.size() == mol.getNumAtoms(),
                  "atomMapping mismatch");
  CHECK_INVARIANT(bondMapping.size() == mol.getNumBonds(),
                  "bondMapping mismatch");
  mol.setProp(common_properties::origAtomMapping, atomMapping);
  mol.setProp(common_properties::origBondMapping, bondMapping);
}

void labelMatches(RWMol& mol, const std::vector<AbbreviationMatch>& matches) {
  for (const auto& amatch : matches) {
    // throughout this remember that atom 0 in the match is the dummy
    SubstanceGroup sg(&mol, "SUP");
    sg.setProp("LABEL", amatch.abbrev.label);

    for (unsigned int i = 1; i < amatch.match.size(); ++i) {
      const auto& pr = amatch.match[i];
      sg.addAtomWithIdx(pr.second);
    }
    auto bnd =
        mol.getBondBetweenAtoms(amatch.match[0].second, amatch.match[1].second);
    CHECK_INVARIANT(bnd, "bond to attachment point not found");
    sg.addBondWithIdx(bnd->getIdx());
    sg.addAttachPoint(amatch.match[1].second, amatch.match[0].second, "1");
    addSubstanceGroup(mol, sg);
  }
}

std::vector<AbbreviationMatch> findApplicableAbbreviationMatches(
    const ROMol& mol, const std::vector<AbbreviationDefinition>& abbrevs,
    double maxCoverage) {
  std::vector<AbbreviationMatch> res;
  auto nAtoms = mol.getNumAtoms();
  if (!nAtoms || abbrevs.empty()) {
    return res;
  }

  bool hasRings = mol.getRingInfo()->isInitialized();
  if (!hasRings) {
    MolOps::fastFindRings(mol);
  }

  std::vector<AbbreviationMatch> tres;
  boost::dynamic_bitset<> dummies(mol.getNumAtoms());
  boost::dynamic_bitset<> firstAts(mol.getNumAtoms());
  boost::dynamic_bitset<> covered(mol.getNumAtoms());

  for (const auto& abbrev : abbrevs) {
    CHECK_INVARIANT(abbrev.mol, "molecule is null");
    if (maxCoverage > 0) {
      unsigned int nDummies;
      abbrev.mol->getProp(common_properties::numDummies, nDummies);
      if (double(abbrev.mol->getNumAtoms() - nDummies) / nAtoms >=
          maxCoverage) {
        continue;
      }
    }
    auto matches = SubstructMatch(mol, *abbrev.mol);
    for (const auto& match : matches) {
      CHECK_INVARIANT(match.size() > 1, "bad match size");
      // if we've already covered the first non-dummy atom or used it as a first
      // atom skip this.
      if (firstAts[match[1].second] || covered[match[1].second]) {
        continue;
      }
      bool keepIt = true;
      for (unsigned int i = 2; i < match.size(); ++i) {
        const auto& pr = match[i];
        if (covered[pr.second]) {
          keepIt = false;
          break;
        }
      }
      if (!keepIt) {
        continue;
      }
      for (unsigned int i = 1; i < match.size(); ++i) {
        const auto& pr = match[i];
        covered.set(pr.second);
      }
      dummies.set(match[0].second);
      firstAts.set(match[1].second);
      if (!firstAts[match[0].second]) {
        tres.emplace_back(match, abbrev);
      }
    }
  }
  for (const auto& itm : tres) {
    // if the dummy in this wasn't a first atom anywhere
    if (!firstAts[itm.match[0].second]) {
      res.push_back(std::move(itm));
    }
  }

  // if we added ring info, go ahead and remove it
  if (!hasRings) {
    mol.getRingInfo()->reset();
  }

  return res;
}

void condenseMolAbbreviations(
    RWMol& mol, const std::vector<AbbreviationDefinition>& abbrevs,
    double maxCoverage, bool sanitize) {
  auto applicable =
      findApplicableAbbreviationMatches(mol, abbrevs, maxCoverage);
  applyMatches(mol, applicable);
  if (sanitize) {
    MolOps::symmetrizeSSSR(mol);
  }
};

void labelMolAbbreviations(RWMol& mol,
                           const std::vector<AbbreviationDefinition>& abbrevs,
                           double maxCoverage) {
  auto applicable =
      findApplicableAbbreviationMatches(mol, abbrevs, maxCoverage);
  labelMatches(mol, applicable);
};

RDKIX_ABBREVIATIONS_EXPORT void condenseAbbreviationSubstanceGroups(
    RWMol& mol) {
  auto& molSGroups = getSubstanceGroups(mol);
  std::vector<AbbreviationMatch> abbrevMatches;
  for (const auto& sg : molSGroups) {
    if (sg.getProp<std::string>("TYPE") == "SUP") {
      AbbreviationMatch abbrevMatch;
      std::string label = "abbrev";
      sg.getPropIfPresent("LABEL", label);
      abbrevMatch.abbrev.label = label;
      auto ats = sg.getAtoms();
      auto bnds = sg.getBonds();
      if (bnds.empty()) {
        BOOST_LOG(rdWarningLog) << "SUP group without any bonds" << std::endl;
      } else {
        bool firstAttachFound = false;
        for (unsigned int i = 0; i < bnds.size(); ++i) {
          auto bnd = mol.getBondWithIdx(bnds[i]);
          unsigned int mAt;  // sgroup atom in the match
          unsigned int oAt;  // add the first attachment point to the beginning
                             // of the atom list
          if (std::find(ats.begin(), ats.end(), bnd->getBeginAtomIdx()) !=
              ats.end()) {
            oAt = bnd->getEndAtomIdx();
            mAt = bnd->getBeginAtomIdx();
          } else if (std::find(ats.begin(), ats.end(), bnd->getEndAtomIdx()) !=
                     ats.end()) {
            oAt = bnd->getBeginAtomIdx();
            mAt = bnd->getEndAtomIdx();
          } else {
            BOOST_LOG(rdWarningLog) << "SUP group includes bond not connected "
                                       "to any of the abbreviation atoms"
                                    << std::endl;
            continue;
          }

          if (!firstAttachFound) {
            // make sure the atom connected to the first attachment point
            // is the first one in the match
            if (*ats.begin() != mAt) {
              ats.erase(std::find(ats.begin(), ats.end(), mAt));
              ats.insert(ats.begin(), mAt);
            }
            ats.insert(ats.begin(), oAt);
            firstAttachFound = true;
          } else {
            abbrevMatch.abbrev.extraAttachAtoms.push_back(oAt);
          }
        }
      }

      // create a match record:
      for (unsigned int i = 0; i < ats.size(); ++i) {
        abbrevMatch.match.push_back({i, ats[i]});
      }
      abbrevMatches.push_back(abbrevMatch);
    }
  }
  if (!abbrevMatches.empty()) {
    applyMatches(mol, abbrevMatches);
  } else {
    BOOST_LOG(rdWarningLog) << "no suitable SubstanceGroups found" << std::endl;
  }
};  // namespace Abbreviations

}  // namespace Abbreviations
}  // namespace RDKix
