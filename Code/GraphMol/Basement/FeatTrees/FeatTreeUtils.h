//
//  Copyright (C) 2005-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
/*! \file FeatTreeUtils.h

  \brief No user-serviceable parts inside

  This functionality is used in the construction of feature trees
  from molecules. It's exposed here so that it can be tested.

*/
#include <RDGeneral/export.h>
#ifndef _RD_FEATTREEUTILS_H_
#define _RD_FEATTREEUTILS_H_

#include "FeatTree.h"
#include <vector>

namespace RDKix {
typedef std::vector<int> INT_VECT;
class ROMol;
namespace FeatTrees {
void addRingsAndConnectors(const ROMol &mol, FeatTreeGraph &featGraph);
void addRingRingBonds(const ROMol &mol, FeatTreeGraph &featGraph);
std::vector<unsigned int> addNonringAtoms(const ROMol &mol,
                                          FeatTreeGraph &featGraph);
void addBondsFromNonringAtoms(const ROMol &mol, FeatTreeGraph &featGraph,
                              std::vector<unsigned int> &atomIndices);
void addZeroNodes(FeatTreeGraph &featGraph);
void replaceCycles(FeatTreeGraph &featGraph);
}  // namespace FeatTrees
}  // namespace RDKix
#endif
