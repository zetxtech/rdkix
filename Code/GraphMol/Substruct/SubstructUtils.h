//
//  Copyright (C) 2003-2019 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_SUBSTRUCT_UTILS_H
#define RD_SUBSTRUCT_UTILS_H

#include "SubstructMatch.h"

namespace RDKix {
class ROMol;
class Atom;
class Bond;

RDKIX_SUBSTRUCTMATCH_EXPORT double toPrime(const MatchVectType& v);
RDKIX_SUBSTRUCTMATCH_EXPORT void removeDuplicates(std::vector<MatchVectType>& v,
                                                  unsigned int nAtoms);
RDKIX_SUBSTRUCTMATCH_EXPORT bool atomCompat(const Atom* a1, const Atom* a2,
                                            const SubstructMatchParameters& ps);
RDKIX_SUBSTRUCTMATCH_EXPORT bool chiralAtomCompat(const Atom* a1,
                                                  const Atom* a2);
RDKIX_SUBSTRUCTMATCH_EXPORT bool bondCompat(const Bond* b1, const Bond* b2,
                                            const SubstructMatchParameters& ps);
//! This postprocesses the passed substruct matches and returns
//! the match that has the largest number of non-hydrogen atoms
//! in correspondence of terminal dummy atoms
RDKIX_SUBSTRUCTMATCH_EXPORT const MatchVectType& getMostSubstitutedCoreMatch(
    const ROMol& mol, const ROMol& core,
    const std::vector<MatchVectType>& matches);
//! This returns a copy of the passed substruct matches sorted by decreasing
//! number of non-hydrogen atoms in correspondence of terminal dummy atoms
RDKIX_SUBSTRUCTMATCH_EXPORT std::vector<MatchVectType>
sortMatchesByDegreeOfCoreSubstitution(
    const ROMol& mol, const ROMol& core,
    const std::vector<MatchVectType>& matches);
RDKIX_SUBSTRUCTMATCH_EXPORT bool isAtomTerminalRGroupOrQueryHydrogen(
    const Atom* atom);

}  // namespace RDKix

#endif
