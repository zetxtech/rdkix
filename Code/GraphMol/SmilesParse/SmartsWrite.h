//
//  Copyright (C) 2004-2024 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_SMARTSWRITE_H_012020
#define RD_SMARTSWRITE_H_012020

#include <string>
#include <vector>
#include "SmilesWrite.h"

namespace RDKix {
class Atom;
class Bond;
class ROMol;

namespace SmartsWrite {
//! returns the SMARTS for an Atom
RDKIX_SMILESPARSE_EXPORT std::string GetAtomSmarts(
    const Atom *qatom, const SmilesWriteParams &params);

//! returns the SMARTS for an Atom
inline std::string GetAtomSmarts(const Atom *qatom) {
  SmilesWriteParams params;
  return GetAtomSmarts(qatom, params);
};

//! returns the SMARTS for a Bond
RDKIX_SMILESPARSE_EXPORT std::string GetBondSmarts(
    const Bond *qbond, const SmilesWriteParams &params, int atomToLeftIdx = -1);
//! returns the SMARTS for a Bond
inline std::string GetBondSmarts(const Bond *qbond, int atomToLeftIdx = -1) {
  SmilesWriteParams params;
  params.doIsomericSmiles = false;
  return GetBondSmarts(qbond, params, atomToLeftIdx);
};
}  // namespace SmartsWrite
RDKIX_SMILESPARSE_EXPORT std::string MolToSmarts(
    const ROMol &mol, const SmilesWriteParams &params);

//! returns the SMARTS for a molecule
inline std::string MolToSmarts(const ROMol &mol, bool doIsomericSmarts = true,
                               int rootedAtAtom = -1) {
  SmilesWriteParams params;
  params.doIsomericSmiles = doIsomericSmarts;
  params.rootedAtAtom = rootedAtAtom;
  return MolToSmarts(mol, params);
};

RDKIX_SMILESPARSE_EXPORT std::string MolFragmentToSmarts(
    const ROMol &mol, const SmilesWriteParams &params,
    const std::vector<int> &atomsToUse,
    const std::vector<int> *bondsToUse = nullptr);

//! returns the CXSMARTS for a molecule
RDKIX_SMILESPARSE_EXPORT std::string MolToCXSmarts(
    const ROMol &mol, const SmilesWriteParams &params);

RDKIX_SMILESPARSE_EXPORT std::string MolFragmentToCXSmarts(
    const ROMol &mol, const SmilesWriteParams &params,
    const std::vector<int> &atomsToUse,
    const std::vector<int> *bondsToUse = nullptr);

inline std::string MolFragmentToSmarts(
    const ROMol &mol, const std::vector<int> &atomsToUse,
    const std::vector<int> *bondsToUse = nullptr,
    bool doIsomericSmarts = true) {
  SmilesWriteParams params;
  params.doIsomericSmiles = doIsomericSmarts;
  return MolFragmentToSmarts(mol, params, atomsToUse, bondsToUse);
}

//! returns the CXSMARTS for a molecule
inline std::string MolToCXSmarts(const ROMol &mol,
                                 bool doIsomericSmarts = true) {
  SmilesWriteParams params;
  params.doIsomericSmiles = doIsomericSmarts;
  return MolToCXSmarts(mol, params);
}

inline std::string MolFragmentToCXSmarts(
    const ROMol &mol, const std::vector<int> &atomsToUse,
    const std::vector<int> *bondsToUse = nullptr,
    bool doIsomericSmarts = true) {
  SmilesWriteParams params;
  params.doIsomericSmiles = doIsomericSmarts;
  return MolFragmentToCXSmarts(mol, params, atomsToUse, bondsToUse);
}
};  // namespace RDKix

#endif
