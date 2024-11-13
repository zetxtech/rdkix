//
//  Copyright (C) 2014 Novartis Institutes for BioMedical Research
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#pragma once
#include <vector>
#include <stdexcept>
#include "../RDKixBase.h"
#include "Graph.h"
#include "MatchTable.h"
#include "DebugTrace.h"  // algorithm filter definitions

namespace RDKix {
namespace FMCS {
struct Target {
  const ROMol *Molecule;
  Graph Topology;
  MatchTable AtomMatchTable;
  MatchTable BondMatchTable;
};
}  // namespace FMCS
}  // namespace RDKix
