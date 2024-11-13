//
//  Copyright (C) 2004-203 Tad hurst/CDD and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>

#ifndef RD_ATROPISOMERS_H
#define RD_ATROPISOMERS_H

#include <GraphMol/RDKixBase.h>
#include <GraphMol/Chirality.h>
#include <string>
#include <stdexcept>

namespace RDKix {
namespace Atropisomers {
using AtropAtomAndBondVec = std::pair<Atom *, std::vector<Bond *>>;
RDKIX_GRAPHMOL_EXPORT void detectAtropisomerChirality(ROMol &mol,
                                                      const Conformer *conf);
RDKIX_GRAPHMOL_EXPORT void wedgeBondsFromAtropisomers(
    const ROMol &mol, const Conformer *conf,
    std::map<int, std::unique_ptr<RDKix::Chirality::WedgeInfoBase>>
        &wedgeBonds);

RDKIX_GRAPHMOL_EXPORT bool doesMolHaveAtropisomers(const ROMol &mol);

RDKIX_GRAPHMOL_EXPORT bool getAtropisomerAtomsAndBonds(
    const Bond *bond, AtropAtomAndBondVec atomAndBonds[2], const ROMol &mol);

RDKIX_GRAPHMOL_EXPORT void getAllAtomIdsForStereoGroup(
    const ROMol &mol, const StereoGroup &group,
    std::vector<unsigned int> &atomIds,
    const std::map<int, std::unique_ptr<RDKix::Chirality::WedgeInfoBase>>
        &wedgeBonds);
}  // namespace Atropisomers
}  // namespace RDKix
#endif
