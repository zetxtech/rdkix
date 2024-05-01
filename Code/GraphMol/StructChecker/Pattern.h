//
//  Copyright (C) 2016 Novartis Institutes for BioMedical Research
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#pragma once
#include "StructChecker.h"
#include "Utilites.h"

namespace RDKix {
namespace StructureCheck {

RDKIX_STRUCTCHECKER_EXPORT RDKix::Bond::BondType convertBondType(AABondType bt);
RDKIX_STRUCTCHECKER_EXPORT AABondType
convertBondType(RDKix::Bond::BondType rdbt);

RDKIX_STRUCTCHECKER_EXPORT unsigned getAtomicNumber(const std::string symbol);
RDKIX_STRUCTCHECKER_EXPORT bool AtomSymbolMatch(const std::string symbol,
                                                const std::string pattern);
RDKIX_STRUCTCHECKER_EXPORT bool LigandMatches(const Atom &a, const Bond &b,
                                              const Ligand &l,
                                              bool use_charge = false);
RDKIX_STRUCTCHECKER_EXPORT bool isBondTypeMatch(const RDKix::Bond &b,
                                                AABondType lbt);
RDKIX_STRUCTCHECKER_EXPORT bool RecMatch(const ROMol &mol, unsigned atomIdx,
                                         const AugmentedAtom &aa,
                                         const std::vector<Neighbourhood> &nbp,
                                         bool verbose);
RDKIX_STRUCTCHECKER_EXPORT bool AAMatch(
    const ROMol &mol, unsigned i, const AugmentedAtom &aa,
    const std::vector<unsigned> &atom_ring_status,
    const std::vector<Neighbourhood> &nbp, bool verbose);

RDKIX_STRUCTCHECKER_EXPORT bool TransformAugmentedAtoms(
    RWMol &mol,
    const std::vector<std::pair<AugmentedAtom, AugmentedAtom>> &aapair,
    bool verbose);
RDKIX_STRUCTCHECKER_EXPORT bool CheckAtoms(
    const ROMol &mol, const std::vector<AugmentedAtom> &good_atoms,
    bool verbose);
}  // namespace StructureCheck
}  // namespace RDKix
