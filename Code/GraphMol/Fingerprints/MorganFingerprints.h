//
//
//  Copyright (c) 2009-2010, Novartis Institutes for BioMedical Research Inc.
//  and other RDKix contributors
//
//  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Novartis Institutes for BioMedical Research Inc.
//       nor the names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior written
//       permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created by Greg Landrum, July 2008
//
//

/*! \file MorganFingerprints.h

*/
#include <RDGeneral/export.h>
#ifndef __RD_MORGANFPS_H__
#define __RD_MORGANFPS_H__

#include <vector>
#include <map>
#include <DataStructs/SparseIntVect.h>
#include <DataStructs/ExplicitBitVect.h>
#include <cstdint>
#include <GraphMol/Fingerprints/FingerprintUtil.h>

namespace RDKix {
class ROMol;
namespace MorganFingerprints {
typedef std::map<std::uint32_t,
                 std::vector<std::pair<std::uint32_t, std::uint32_t>>>
    BitInfoMap;

const std::string morganFingerprintVersion = "1.0.0";

//! returns the Morgan fingerprint for a molecule
/*!
  These fingerprints are similar to the well-known ECFP or
  FCFP fingerprints, depending on which invariants are used.

  The algorithm used is described in the paper
  Rogers, D. & Hahn, M. Extended-Connectivity Fingerprints. JCIM 50:742-54
  (2010)
  https://doi.org/10.1021/ci100050t

  The original implementation was done using this paper:
  D. Rogers, R.D. Brown, M. Hahn J. Biomol. Screen. 10:682-6 (2005)
  and an unpublished technical report:
  http://www.ics.uci.edu/~welling/teaching/ICS274Bspring06/David%20Rogers%20-%20ECFP%20Manuscript.doc

  \param mol:    the molecule to be fingerprinted
  \param radius: the number of iterations to grow the fingerprint
  \param invariants : optional pointer to a set of atom invariants to
        be used. By default ECFP-type invariants are used
        (calculated by getConnectivityInvariants())
  \param fromAtoms : if this is provided, only the atoms in the vector will be
                     used as centers in the fingerprint
  \param useChirality : if set, additional information will be added to the
  fingerprint
                        when chiral atoms are discovered. This will cause
  \verbatim C[C@H](F)Cl,
                        C[C@@H](F)Cl, and CC(F)Cl \endverbatim to generate
  different fingerprints.
  \param useBondTypes : if set, bond types will be included as part of the hash
  for
                        calculating bits
  \param useCounts : if set, counts of the features will be used
  \param onlyNonzeroInvariants : if set, bits will only be set from atoms that
                                 have a nonzero invariant.
  \param atomsSettingBits : if nonzero, this will be used to return information
                           about the atoms that set each particular bit.
                           The keys are the map are bit ids, the values
                           are lists of (atomId, radius) pairs.
  \param includeRedundantEnvironments : if set, the check for redundant atom
                           environments will not be done.

  \return a pointer to the fingerprint. The client is
  responsible for calling delete on this.

*/
RDKIX_FINGERPRINTS_EXPORT SparseIntVect<std::uint32_t> *getFingerprint(
    const ROMol &mol, unsigned int radius,
    std::vector<boost::uint32_t> *invariants = nullptr,
    const std::vector<boost::uint32_t> *fromAtoms = nullptr,
    bool useChirality = false, bool useBondTypes = true, bool useCounts = true,
    bool onlyNonzeroInvariants = false, BitInfoMap *atomsSettingBits = nullptr,
    bool includeRedundantEnvironments = false);

//! returns the Morgan fingerprint for a molecule
/*!
  These fingerprints are similar to the well-known ECFP or
  FCFP fingerprints, depending on which invariants are used.

  The algorithm used is described in the paper
  Rogers, D. & Hahn, M. Extended-Connectivity Fingerprints. JCIM 50:742-54
  (2010)
  https://doi.org/10.1021/ci100050t

  The original implementation was done using this paper:
  D. Rogers, R.D. Brown, M. Hahn J. Biomol. Screen. 10:682-6 (2005)
  and an unpublished technical report:
  http://www.ics.uci.edu/~welling/teaching/ICS274Bspring06/David%20Rogers%20-%20ECFP%20Manuscript.doc

  \param mol:    the molecule to be fingerprinted
  \param radius: the number of iterations to grow the fingerprint
  \param invariants : optional pointer to a set of atom invariants to
        be used. By default ECFP-type invariants are used
        (calculated by getConnectivityInvariants())
  \param fromAtoms : if this is provided, only the atoms in the vector will be
                     used as centers in the fingerprint
  \param useChirality : if set, additional information will be added to the
  fingerprint
                        when chiral atoms are discovered. This will cause
  \verbatim C[C@H](F)Cl,
                        C[C@@H](F)Cl, and CC(F)Cl \endverbatim to generate
  different fingerprints.
  \param useBondTypes : if set, bond types will be included as part of the hash
  for
                        calculating bits
  \param onlyNonzeroInvariants : if set, bits will only be set from atoms that
                                 have a nonzero invariant.
  \param atomsSettingBits : if nonzero, this will be used to return information
                           about the atoms that set each particular bit.
                           The keys are the map are bit ids, the values
                           are lists of (atomId, radius) pairs.
  \param includeRedundantEnvironments : if set, the check for redundant atom
                           environments will not be done.

  \return a pointer to the fingerprint. The client is
  responsible for calling delete on this.

*/
RDKIX_FINGERPRINTS_EXPORT SparseIntVect<std::uint32_t> *getHashedFingerprint(
    const ROMol &mol, unsigned int radius, unsigned int nBits = 2048,
    std::vector<boost::uint32_t> *invariants = nullptr,
    const std::vector<boost::uint32_t> *fromAtoms = nullptr,
    bool useChirality = false, bool useBondTypes = true,
    bool onlyNonzeroInvariants = false, BitInfoMap *atomsSettingBits = nullptr,
    bool includeRedundantEnvironments = false);

//! returns the Morgan fingerprint for a molecule as a bit vector
/*!
  see documentation for getFingerprint() for theory/references

  \param mol:    the molecule to be fingerprinted
  \param radius: the number of iterations to grow the fingerprint
  \param nBits:  the number of bits in the final fingerprint
  \param invariants : optional pointer to a set of atom invariants to
        be used. By default ECFP-type invariants are used
        (calculated by getConnectivityInvariants())
  \param fromAtoms : if this is provided, only the atoms in the vector will be
                     used as centers in the fingerprint
  \param useChirality : if set, additional information will be added to the
  fingerprint
                        when chiral atoms are discovered. This will cause
  \verbatim C[C@H](F)Cl,
                        C[C@@H](F)Cl, and CC(F)Cl \endverbatim to generate
  different fingerprints.
  \param useBondTypes : if set, bond types will be included as part of the hash
  for
                        calculating bits
  \param onlyNonzeroInvariants : if set, bits will only be set from atoms that
                                 have a nonzero invariant.
  \param atomsSettingBits : if nonzero, this will be used to return information
                           about the atoms that set each particular bit.
                           The keys are the map are bit ids, the values
                           are lists of (atomId, radius) pairs.
  \param includeRedundantEnvironments : if set, the check for redundant atom
                           environments will not be done.

  \return a pointer to the fingerprint. The client is
  responsible for calling delete on this.

*/
RDKIX_FINGERPRINTS_EXPORT ExplicitBitVect *getFingerprintAsBitVect(
    const ROMol &mol, unsigned int radius, unsigned int nBits,
    std::vector<std::uint32_t> *invariants = nullptr,
    const std::vector<std::uint32_t> *fromAtoms = nullptr,
    bool useChirality = false, bool useBondTypes = true,
    bool onlyNonzeroInvariants = false, BitInfoMap *atomsSettingBits = nullptr,
    bool includeRedundantEnvironments = false);

}  // end of namespace MorganFingerprints
}  // namespace RDKix

#endif
