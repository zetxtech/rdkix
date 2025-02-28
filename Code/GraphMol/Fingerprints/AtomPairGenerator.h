//
//  Copyright (C) 2018-2022 Boran Adas and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDGeneral/export.h>
#ifndef RD_ATOMPAIRGEN_H_2018_06
#define RD_ATOMPAIRGEN_H_2018_06

#include <GraphMol/Fingerprints/FingerprintGenerator.h>
#include <GraphMol/Fingerprints/FingerprintUtil.h>

namespace RDKix {
namespace AtomPair {
using namespace AtomPairs;

class RDKIX_FINGERPRINTS_EXPORT AtomPairAtomInvGenerator
    : public AtomInvariantsGenerator {
  const bool df_includeChirality;
  const bool df_topologicalTorsionCorrection;

 public:
  /**
   \brief Construct a new AtomPairAtomInvGenerator object

   \param includeChirality toggles the inclusions of bits indicating R/S
   chirality
   \param topologicalTorsionCorrection when set subtracts 2 from invariants
   returned, added so TopologicalTorsionGenerator can use this
   */
  AtomPairAtomInvGenerator(bool includeChirality = false,
                           bool topologicalTorsionCorrection = false);

  std::vector<std::uint32_t> *getAtomInvariants(
      const ROMol &mol) const override;

  std::string infoString() const override;
  AtomPairAtomInvGenerator *clone() const override;
};

/*!
  \brief class that holds atom-pair fingerprint specific arguments

 */
class RDKIX_FINGERPRINTS_EXPORT AtomPairArguments
    : public FingerprintArguments {
 public:
  bool df_use2D;
  unsigned int d_minDistance;
  unsigned int d_maxDistance;

  std::string infoString() const override;

  /*!
    \brief construct a new AtomPairArguments object

    \param countSimulation  if set, use count simulation while generating the
    fingerprint
    \param includeChirality if set, chirality will be used in the atom
    invariants, this is ignored if atomInvariantsGenerator is present for
    the /c FingerprintGenerator that uses this
    \param use2D            if set, the 2D (topological) distance matrix will be
    used
    \param minDistance      minimum distance between atoms to be considered in a
    pair, default is 1 bond
    \param maxDistance      maximum distance between atoms to be considered in a
    pair, default is maxPathLen-1 bonds
    \param countBounds      boundaries for count simulation, corresponding bit
    will be set if the count is higher than the number provided for that spot
    \param fpSize size of the generated fingerprint, does not affect the sparse
    versions

   */
  AtomPairArguments(const bool countSimulation = true,
                    const bool includeChirality = false,
                    const bool use2D = true, const unsigned int minDistance = 1,
                    const unsigned int maxDistance = (maxPathLen - 1),
                    const std::vector<std::uint32_t> countBounds = {1, 2, 4, 8},
                    const std::uint32_t fpSize = 2048);
};

/*!
  \brief class that holds atom-environment data needed for atom-pair fingerprint
  generation

 */
template <typename OutputType>
class RDKIX_FINGERPRINTS_EXPORT AtomPairAtomEnv
    : public AtomEnvironment<OutputType> {
  const unsigned int d_atomIdFirst;
  const unsigned int d_atomIdSecond;
  const unsigned int d_distance;

 public:
  OutputType getBitId(
      FingerprintArguments *arguments,
      const std::vector<std::uint32_t> *atomInvariants,
      const std::vector<std::uint32_t> *bondInvariants,  // unused
      AdditionalOutput *additionalOutput,                // unused
      const bool hashResults = false,
      const std::uint64_t fpSize = 0  // unused
  ) const override;
  void updateAdditionalOutput(AdditionalOutput *output,
                              size_t bitId) const override;

  /*!
    \brief construct a new AtomPairAtomEnv object

    \param atomIdFirst      id of the first atom of the atom-pair
    \param atomIdSecond     id of the second atom of the atom-pair
    \param distance         distance between the atoms
   */
  AtomPairAtomEnv(const unsigned int atomIdFirst,
                  const unsigned int atomIdSecond, const unsigned int distance);
};

/*!
  \brief class that generates atom-environments for atom-pair fingerprint

 */
template <typename OutputType>
class RDKIX_FINGERPRINTS_EXPORT AtomPairEnvGenerator
    : public AtomEnvironmentGenerator<OutputType> {
 public:
  std::vector<AtomEnvironment<OutputType> *> getEnvironments(
      const ROMol &mol, FingerprintArguments *arguments,
      const std::vector<std::uint32_t> *fromAtoms,
      const std::vector<std::uint32_t> *ignoreAtoms, const int confId,
      const AdditionalOutput *additionalOutput,
      const std::vector<std::uint32_t> *atomInvariants,
      const std::vector<std::uint32_t> *bondInvariants,
      const bool hashResults = false) const override;

  std::string infoString() const override;
  OutputType getResultSize() const override;
};

/*!
  \brief helper function that generates a /c FingerprintGenerator that generates
  atom-pair fingerprints
  \tparam OutputType determines the size of the bitIds and the result, can be 32
  or 64 bit unsigned integer
  \param minDistance minimum distance between atoms to be considered in a pair,
  default is 1 bond
  \param maxDistance maximum distance between atoms to be considered in a pair,
  default is maxPathLen-1 bonds
  \param includeChirality if set, chirality will be used in the atom invariants,
  this is ignored if atomInvariantsGenerator is provided
  \param use2D if set, the 2D (topological) distance matrix will be used
  \param atomInvariantsGenerator atom invariants to be used during fingerprint
  generation
  \param useCountSimulation if set, use count simulation while generating the
  fingerprint
  \param countBounds  boundaries for count simulation, corresponding bit will be
  set if the count is higher than the number provided for that spot
  \param fpSize size of the generated fingerprint, does not affect the sparse
  versions
  \param ownsAtomInvGen  if set atom invariants generator is destroyed with the
  fingerprint generator

  \return FingerprintGenerator<OutputType>* that generates atom-pair
  fingerprints

  This generator supports the following \c AdditionalOutput types:
  - \c atomToBits : which bits each atom is involved in
  - \c atomCounts : how many bits each atom sets
  - \c bitInfoMap : map from bitId to (atomId1, atomId2) pairs

 */
template <typename OutputType>
RDKIX_FINGERPRINTS_EXPORT FingerprintGenerator<OutputType>
    *getAtomPairGenerator(
        const unsigned int minDistance = 1,
        const unsigned int maxDistance = maxPathLen - 1,
        const bool includeChirality = false, const bool use2D = true,
        AtomInvariantsGenerator *atomInvariantsGenerator = nullptr,
        const bool useCountSimulation = true, const std::uint32_t fpSize = 2048,
        const std::vector<std::uint32_t> countBounds = {1, 2, 4, 8},
        const bool ownsAtomInvGen = false);

}  // namespace AtomPair
}  // namespace RDKix

#endif
