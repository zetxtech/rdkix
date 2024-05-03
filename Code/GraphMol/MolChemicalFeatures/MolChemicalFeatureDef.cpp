// $Id$
//
//  Copyright (C) 2004-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <GraphMol/RDKixBase.h>
#include <GraphMol/SmilesParse/SmilesParse.h>

#include <utility>
#include "MolChemicalFeature.h"
#include "MolChemicalFeatureDef.h"

namespace RDKix {

MolChemicalFeatureDef::MolChemicalFeatureDef(const std::string &smarts,
                                             std::string family,
                                             std::string type)
    : d_family(std::move(family)), d_type(std::move(type)), d_smarts(smarts) {
  auto *mol = static_cast<ROMol *>(SmartsToMol(smarts));
  dp_pattern.reset(mol);
}
void MolChemicalFeatureDef::normalizeWeights() {
  double accum = 0.0;
  for (auto i = this->beginWeights(); i != this->endWeights(); i++) {
    accum += *i;
  }
  for (auto i = this->beginWeights(); i != this->endWeights(); i++) {
    *i /= accum;
  }
}
}  // namespace RDKix
