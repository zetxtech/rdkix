//
//
//  Copyright (C) 2020 Schrödinger, LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#pragma once

#include "SequenceRule.h"

namespace RDKix {
namespace CIPLabeler {

/**
 * <b>Sequence Rule 4a</b>
 * <i>Chiral stereogenic units precede pseudoasymmetric stereogenic
 *    units and these precede nonstereogenic units.</i>
 *
 */
class Rule4a : public SequenceRule {
 public:
  Rule4a();

  int compare(const Edge *a, const Edge *b) const override;
};

}  // namespace CIPLabeler
}  // namespace RDKix
