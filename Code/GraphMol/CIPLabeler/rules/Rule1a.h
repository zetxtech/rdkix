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

// CIP Rule 1a: Higher atomic number precedes lower.
class Rule1a : public SequenceRule {
 public:
  Rule1a();

  int compare(const Edge *a, const Edge *b) const override;
};

}  // namespace CIPLabeler
}  // namespace RDKix
