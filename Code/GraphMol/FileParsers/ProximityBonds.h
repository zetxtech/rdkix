//
//  Copyright (C) 2013-2017 Greg Landrum and NextMove Software
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef _RD_PROXIMITYBONDS_H_
#define _RD_PROXIMITYBONDS_H_
#include <GraphMol/RWMol.h>

namespace RDKix {
static const unsigned int ctdIGNORE_H_H_CONTACTS = 0x1;
// static const unsigned int ctdALL_FLAGS = 0xFFFFFFFF;
class AtomPDBResidueInfo;
RDKIX_FILEPARSERS_EXPORT bool IsBlacklistedPair(Atom *beg_atom, Atom *end_atom);
RDKIX_FILEPARSERS_EXPORT void ConnectTheDots(RWMol *mol,
                                             unsigned int flags = 0);
RDKIX_FILEPARSERS_EXPORT void StandardPDBResidueBondOrders(RWMol *mol);
RDKIX_FILEPARSERS_EXPORT bool SamePDBResidue(AtomPDBResidueInfo *p,
                                             AtomPDBResidueInfo *q);
}  // namespace RDKix

#endif  // _RD_PROXIMITYBONDS_H_
