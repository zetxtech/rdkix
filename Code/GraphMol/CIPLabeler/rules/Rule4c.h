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
 * <b>Sequence Rule 4c</b>
 * <i>‘r’ precedes ‘s’ and ‘m’ precedes ‘p’</i>
 *
 */
class Rule4c : public SequenceRule {
 public:
  Rule4c();

  int compare(const Edge *a, const Edge *b) const override;
};

}  // namespace CIPLabeler
}  // namespace RDKix
