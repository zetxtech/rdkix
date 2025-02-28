//
//  Copyright (C) 2003-2020 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_FINGERPRINTS_H
#define RD_FINGERPRINTS_H

#include <vector>
#include <cstdint>
#include <DataStructs/SparseIntVect.h>

class ExplicitBitVect;
namespace RDKix {
class ROMol;
class MolBundle;

//! \brief Generates a topological (Daylight like) fingerprint for a molecule
//!        using an alternate (faster) hashing algorithm
/*!

  \param mol:          the molecule to be fingerprinted
  \param minPath:      the minimum path length (in bonds) to be included
  \param maxPath:      the minimum path length (in bonds) to be included
  \param fpSize:       the size of the fingerprint
  \param nBitsPerHash: the number of bits to be set by each path
  \param useHs:        toggles inclusion of Hs in paths (if the molecule has
  explicit Hs)
  \param tgtDensity:   if the generated fingerprint is below this density, it
  will
                       be folded until the density is reached.
  \param minSize:      the minimum size to which the fingerprint will be
                       folded
  \param branchedPaths: toggles generation of branched subgraphs, not just
  linear paths
  \param useBondOrders: toggles inclusion of bond orders in the path hashes
  \param atomInvariants: a vector of atom invariants to use while hashing the
  paths
  \param fromAtoms:    only paths starting at these atoms will be included
  \param atomBits:     used to return the bits that each atom is involved in
                       (should be at least \c mol.numAtoms long)

  \return the molecular fingerprint, as an ExplicitBitVect

  <b>Notes:</b>
    - the caller is responsible for <tt>delete</tt>ing the result

*/
RDKIX_FINGERPRINTS_EXPORT ExplicitBitVect *RDKFingerprintMol(
    const ROMol &mol, unsigned int minPath = 1, unsigned int maxPath = 7,
    unsigned int fpSize = 2048, unsigned int nBitsPerHash = 2,
    bool useHs = true, double tgtDensity = 0.0, unsigned int minSize = 128,
    bool branchedPaths = true, bool useBondOrder = true,
    std::vector<std::uint32_t> *atomInvariants = nullptr,
    const std::vector<std::uint32_t> *fromAtoms = nullptr,
    std::vector<std::vector<std::uint32_t>> *atomBits = nullptr,
    std::map<std::uint32_t, std::vector<std::vector<int>>> *bitInfo = nullptr);
const std::string RDKFingerprintMolVersion = "2.0.0";

//! \brief Generates a topological (Daylight like) fingerprint for a molecule
//!        using a layer-based hashing algorithm
/*!

  <b>Experimental:</b> This function is experimental. The API or results may
  change from
  release to release.

  \param mol:          the molecule to be fingerprinted
  \param layerFlags:   the layers to be included (see below)
  \param minPath:      the minimum path length (in bonds) to be included
  \param maxPath:      the minimum path length (in bonds) to be included
  \param fpSize:       the size of the fingerprint
  \param atomCounts:   if provided, this will be used to provide the count of
  the number
                       of paths that set bits each atom is involved in. The
  vector should
                       have at least as many entries as the molecule has atoms
  and is not
                       zeroed out here.
  \param setOnlyBits:  if provided, only bits that are set in this bit vector
  will be set
                       in the result. This is essentially the same as doing:
                          (*res) &= (*setOnlyBits);
                       but also has an impact on the atomCounts (if being used)
  \param branchedPaths: toggles generation of branched subgraphs, not just
  linear paths

  \return the molecular fingerprint, as an ExplicitBitVect

  <b>Notes:</b>
    - the caller is responsible for <tt>delete</tt>ing the result

  <b>Layer definitions:</b>
     - 0x01: pure topology
     - 0x02: bond order
     - 0x04: atom types
     - 0x08: presence of rings
     - 0x10: ring sizes
     - 0x20: aromaticity
*/
RDKIX_FINGERPRINTS_EXPORT ExplicitBitVect *LayeredFingerprintMol(
    const ROMol &mol, unsigned int layerFlags = 0xFFFFFFFF,
    unsigned int minPath = 1, unsigned int maxPath = 7,
    unsigned int fpSize = 2048, std::vector<unsigned int> *atomCounts = nullptr,
    ExplicitBitVect *setOnlyBits = nullptr, bool branchedPaths = true,
    const std::vector<std::uint32_t> *fromAtoms = nullptr);
const unsigned int maxFingerprintLayers = 10;
const std::string LayeredFingerprintMolVersion = "0.7.0";
const unsigned int substructLayers = 0x07;

//! \brief Generates a topological fingerprint for a molecule
//!        using a series of pre-defined structural patterns
/*!

  <b>Experimental:</b> This function is experimental. The API or results may
  change from
  release to release.

  \param mol:          the molecule to be fingerprinted
  \param fpSize:       the size of the fingerprint
  \param atomCounts:   if provided, this will be used to provide the count of
  the number
                       of paths that set bits each atom is involved in. The
  vector should
                       have at least as many entries as the molecule has atoms
  and is not
                       zeroed out here.
  \param setOnlyBits:  if provided, only bits that are set in this bit vector
  will be set
                       in the result. This is essentially the same as doing:
                          (*res) &= (*setOnlyBits);
                       but also has an impact on the atomCounts (if being used)

  \return the molecular fingerprint, as an ExplicitBitVect

  <b>Notes:</b>
    - the caller is responsible for <tt>delete</tt>ing the result

*/
RDKIX_FINGERPRINTS_EXPORT ExplicitBitVect *PatternFingerprintMol(
    const ROMol &mol, unsigned int fpSize = 2048,
    std::vector<unsigned int> *atomCounts = nullptr,
    ExplicitBitVect *setOnlyBits = nullptr, bool tautomericFingerprint = false);
const std::string PatternFingerprintMolVersion = "1.0.0";
//! \overload
RDKIX_FINGERPRINTS_EXPORT ExplicitBitVect *PatternFingerprintMol(
    const MolBundle &bundle, unsigned int fpSize = 2048,
    ExplicitBitVect *setOnlyBits = nullptr, bool tautomericFingerprint = false);

RDKIX_FINGERPRINTS_EXPORT SparseIntVect<boost::uint64_t>
    *getUnfoldedRDKFingerprintMol(
        const ROMol &mol, unsigned int minPath = 1, unsigned int maxPath = 7,
        bool useHs = true, bool branchedPaths = true, bool useBondOrder = true,
        std::vector<std::uint32_t> *atomInvariants = nullptr,
        const std::vector<std::uint32_t> *fromAtoms = nullptr,
        std::vector<std::vector<boost::uint64_t>> *atomBits = nullptr,
        std::map<boost::uint64_t, std::vector<std::vector<int>>> *bitInfo =
            nullptr);

}  // namespace RDKix

#endif
