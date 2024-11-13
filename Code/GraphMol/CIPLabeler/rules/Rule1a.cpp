//
//  Copyright (C) 2020 Schrödinger, LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include "Rule1a.h"
#include "../Mancude.h"

namespace RDKix {
namespace CIPLabeler {

Rule1a::Rule1a() = default;

// CIP Rule 1a: Higher atomic number precedes lower.
int Rule1a::compare(const Edge *a, const Edge *b) const {
  const auto afrac = a->getEnd()->getAtomicNumFraction();
  const auto bfrac = b->getEnd()->getAtomicNumFraction();

  return three_way_comparison(afrac, bfrac);
}

}  // namespace CIPLabeler
}  // namespace RDKix