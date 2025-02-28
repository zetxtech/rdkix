//
//  Copyright (C) 2020 Gareth Jones, Glysade LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include "BinaryStringChromosomePolicy.h"

namespace GapeGa {

BinaryStringChromosomePolicy::BinaryStringChromosomePolicy(
    GarethUtil::RandomUtil& rng_)
    : rng(rng_) {}

BinaryStringChromosomePolicy::~BinaryStringChromosomePolicy() {}

bool BinaryStringChromosomePolicy::mutate(int pos, bool currentValue) const {
  (void)pos;  // not used
  return !currentValue;
}

bool BinaryStringChromosomePolicy::initialize(int pos) const {
  (void)pos;  // not used
  return rng.randomBoolean();
}

}  // namespace GapeGa
