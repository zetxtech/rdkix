//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_TAUTOMER_CATALOG_UTILS_H
#define RD_TAUTOMER_CATALOG_UTILS_H

#include <GraphMol/RDKixBase.h>
#include "TautomerCatalogParams.h"
#include <GraphMol/Substruct/SubstructMatch.h>
#include <GraphMol/ChemReactions/Reaction.h>
#include <GraphMol/Bond.h>
#include <iostream>
#include <utility>

namespace RDKix {
class ROMol;

namespace MolStandardize {
class TautomerCatalogParams;

// typedef enum {
//	SINGLE,
//	DOUBLE,
//	TRIPLE,
//	AROMATIC,
//} BondType;

// typedef std::vector<ROMol*, std::string, std::string> tautomerTransform;
class RDKIX_MOLSTANDARDIZE_EXPORT TautomerTransform {
 public:
  ROMol* Mol = nullptr;
  std::vector<Bond::BondType> BondTypes;
  std::vector<int> Charges;

  TautomerTransform(ROMol* mol, std::vector<Bond::BondType> bondtypes,
                    std::vector<int> charges)
      : Mol(mol),
        BondTypes(std::move(bondtypes)),
        Charges(std::move(charges)) {}

  TautomerTransform(const TautomerTransform& other)
      : BondTypes(other.BondTypes), Charges(other.Charges) {
    Mol = new ROMol(*other.Mol);
  }

  TautomerTransform& operator=(const TautomerTransform& other) {
    if (this != &other) {
      delete Mol;
      Mol = new ROMol(*other.Mol);
      BondTypes = other.BondTypes;
      Charges = other.Charges;
    }
    return *this;
  }

  ~TautomerTransform() { delete Mol; }
};

RDKIX_MOLSTANDARDIZE_EXPORT std::vector<Bond::BondType> stringToBondType(
    std::string bond_str);
RDKIX_MOLSTANDARDIZE_EXPORT std::vector<int> stringToCharge(
    std::string charge_str);

RDKIX_MOLSTANDARDIZE_EXPORT std::vector<TautomerTransform> readTautomers(
    std::string fileName);
RDKIX_MOLSTANDARDIZE_EXPORT std::vector<TautomerTransform> readTautomers(
    std::istream& inStream, int nToRead = -1);
RDKIX_MOLSTANDARDIZE_EXPORT std::vector<TautomerTransform> readTautomers(
    const std::vector<
        std::tuple<std::string, std::string, std::string, std::string>>& data);
}  // namespace MolStandardize
}  // namespace RDKix

#endif
