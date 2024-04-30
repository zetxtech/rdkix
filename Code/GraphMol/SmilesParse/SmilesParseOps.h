//
//  Copyright (C) 2001-2022 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_SMILESPARSEOPS_H
#define RD_SMILESPARSEOPS_H
#include <GraphMol/Bond.h>

namespace RDKix {
class RWMol;
class Atom;
class QueryBond;
}  // namespace RDKix
namespace SmilesParseOps {
RDKIX_SMILESPARSE_EXPORT void CheckRingClosureBranchStatus(RDKix::Atom *atom,
                                                           RDKix::RWMol *mp);
RDKIX_SMILESPARSE_EXPORT void ReportParseError(const char *message,
                                               bool throwIt = true);
RDKIX_SMILESPARSE_EXPORT void CleanupAfterParseError(RDKix::RWMol *mol);
// This uses SMARTS semantics: unspecified bonds are treated as
// aromatic or single.
RDKIX_SMILESPARSE_EXPORT void AddFragToMol(
    RDKix::RWMol *mol, RDKix::RWMol *frag,
    RDKix::Bond::BondType bondOrder = RDKix::Bond::UNSPECIFIED,
    RDKix::Bond::BondDir bondDir = RDKix::Bond::NONE);
RDKIX_SMILESPARSE_EXPORT RDKix::Bond::BondType GetUnspecifiedBondType(
    const RDKix::RWMol *mol, const RDKix::Atom *atom1,
    const RDKix::Atom *atom2);
RDKIX_SMILESPARSE_EXPORT void CheckChiralitySpecifications(RDKix::RWMol *mol,
                                                           bool strict);
RDKIX_SMILESPARSE_EXPORT void CloseMolRings(RDKix::RWMol *mol,
                                            bool toleratePartials);
RDKIX_SMILESPARSE_EXPORT void SetUnspecifiedBondTypes(RDKix::RWMol *mol);
RDKIX_SMILESPARSE_EXPORT void AdjustAtomChiralityFlags(RDKix::RWMol *mol);
RDKIX_SMILESPARSE_EXPORT void CleanupAfterParsing(RDKix::RWMol *mol);
RDKIX_SMILESPARSE_EXPORT void parseCXExtensions(
    RDKix::RWMol &mol, const std::string &extText,
    std::string::const_iterator &pos, unsigned int startAtomIdx = 0,
    unsigned int startBondIdx = 0);
inline void parseCXExtensions(RDKix::RWMol &mol, const std::string &extText,
                              unsigned int startAtomIdx,
                              unsigned int startBondIdx) {
  auto iter = extText.begin();
  parseCXExtensions(mol, extText, iter, startAtomIdx, startBondIdx);
};
//! removes formal charge, isotope, etc. Primarily useful for QueryAtoms
RDKIX_SMILESPARSE_EXPORT void ClearAtomChemicalProps(RDKix::Atom *atom);

//! returns whether or not the combination of tag and permutation provided are
//! legal
RDKIX_SMILESPARSE_EXPORT bool checkChiralPermutation(int chiralTag,
                                                     int permutation);

//! this is a bit of a hack to try and get nicer "SMILES" from
//! a SMARTS molecule
RDKIX_SMILESPARSE_EXPORT RDKix::QueryBond *getUnspecifiedQueryBond(
    const RDKix::Atom *a1, const RDKix::Atom *a2);

namespace detail {
constexpr auto _needsDetectBondStereo = "_needsDetectBondStereo";
constexpr auto _needsDetectAtomStereo = "_needsDetectAtomStereo";
}  // namespace detail
}  // namespace SmilesParseOps

#endif
