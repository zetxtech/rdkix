//
//  Copyright (C) 2021 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <iostream>
#include "MonomerInfo.h"

using namespace RDKix;

//! allows AtomPDBResidueInfo objects to be dumped to streams
std::ostream &operator<<(std::ostream &target, const AtomPDBResidueInfo &apri) {
  target << apri.getSerialNumber() << " " << apri.getName() << " "
         << apri.getResidueName() << " " << apri.getChainId() << " "
         << apri.getResidueNumber();
  return target;
}
