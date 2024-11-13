//
// Copyright (C) David Cosgrove 2023
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDGeneral/export.h>
#ifndef RDKIX_RASCAL_DETAILS_H
#define RDKIX_RASCAL_DETAILS_H

#include <map>

#include <GraphMol/RascalMCES/RascalOptions.h>
#include <GraphMol/RascalMCES/RascalResult.h>
namespace RDKix {
class ROMol;

namespace RascalMCES {

struct RascalClusterOptions;

namespace details {

struct ClusNode {
  std::shared_ptr<RascalResult> d_res;
  double d_sim;
  unsigned int d_mol1Num, d_mol2Num;
};

RDKIX_RASCALMCES_EXPORT double tier1Sim(
    const RDKix::ROMol &mol1, const RDKix::ROMol &mol2,
    std::map<int, std::vector<std::pair<int, int>>> &degSeqs1,
    std::map<int, std::vector<std::pair<int, int>>> &degSeqs2);

RDKIX_RASCALMCES_EXPORT double tier2Sim(
    const ROMol &mol1, const ROMol &mol2,
    const std::map<int, std::vector<std::pair<int, int>>> &degSeqs1,
    const std::map<int, std::vector<std::pair<int, int>>> &degSeqs2,
    const std::vector<unsigned int> &bondLabels1,
    const std::vector<unsigned int> &bondLabels2);

RDKIX_RASCALMCES_EXPORT void getBondLabels(
    const RDKix::ROMol &mol1, const RDKix::ROMol &mol2,
    const RascalOptions &opts, std::vector<unsigned int> &bondLabels1,
    std::vector<unsigned int> &bondLabels2);

std::vector<std::vector<ClusNode>> buildProximityGraph(
    const std::vector<std::shared_ptr<ROMol>> &mols,
    const RascalClusterOptions &clusOpts);

RDKIX_RASCALMCES_EXPORT bool resultCompare(const RascalResult &res1,
                                           const RascalResult &res2);

RDKIX_RASCALMCES_EXPORT void extractClique(
    const std::vector<unsigned int> &clique,
    const std::vector<std::pair<int, int>> &vtxPairs, bool swapped,
    std::vector<std::pair<int, int>> &bondMatches);

// do some simple cleaning of the SMARTS, to make it more user-friendly.
RDKIX_RASCALMCES_EXPORT void cleanSmarts(std::string &smarts,
                                         const std::string &equivalentAtoms);

// Primarily for debugging, these write out the corresponding bonds/atoms
// in Python list format, for ease of cut/paste into a highlighted image
// creation.
RDKIX_RASCALMCES_EXPORT void printBondMatches(const RascalResult &res,
                                              std::ostream &os);

RDKIX_RASCALMCES_EXPORT void printAtomMatches(const RascalResult &res,
                                              std::ostream &os);

// This prints out the scores in the order they are used in resultCompare.
RDKIX_RASCALMCES_EXPORT void printScores(const RascalResult &res,
                                         std::ostream &os);

// Calculate the Johnson similarity between the two molecules using the given
// bondMatches.  It's the fraction of the 2 molecules that are in common,
// somewhat akin to the tanimoto - the square of the number of atoms plus
// number of bonds in the MCES divided by the product of the sums of the number
// of atoms and bonds in the 2 molecules.
// It has nothing to do with lying UK politicians.
RDKIX_RASCALMCES_EXPORT double johnsonSimilarity(
    const std::vector<std::pair<int, int>> &bondMatches,
    const std::vector<std::pair<int, int>> &atomMatches,
    const RDKix::ROMol &mol1, const RDKix::ROMol &mol2);

}  // namespace details

}  // namespace RascalMCES
}  // namespace RDKix
#endif  // RDKIX_RASCAL_MCES_H
