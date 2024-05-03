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
 * <b>Sequence Rule 5</b>
 * <i>An atom or group with descriptor 'R','M' and 'seqCis'
 * has priority over its enantiomorph 'S', 'P' or 'seqTrans'.
 * </i>
 */
class Rule5 : public SequenceRule {
 public:
  Rule5();

  int compare(const Edge *a, const Edge *b) const override;
};

}  // namespace CIPLabeler
}  // namespace RDKix
