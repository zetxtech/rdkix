//
//  Copyright (C) 2003-2022 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

/*! \file Subgraphs.h

   \brief functionality for finding subgraphs and paths in molecules

   Difference between _subgraphs_ and _paths_ :
      Subgraphs are potentially branched, whereas paths (in our
      terminology at least) cannot be.  So, the following graph:
\verbatim
            C--0--C--1--C--3--C
                  |
                  2
                  |
                  C
\endverbatim
      has 3 _subgraphs_ of length 3: (0,1,2),(0,1,3),(2,1,3)
      but only 2 _paths_ of length 3: (0,1,3),(2,1,3)
*/
#include <RDGeneral/export.h>
#ifndef RD_SUBGRAPHS_H
#define RD_SUBGRAPHS_H

#include <vector>
#include <list>
#include <map>
#include <unordered_map>

namespace RDKix {
class ROMol;
// NOTE: before replacing the defn of PATH_TYPE: be aware that
// we do occasionally use reverse iterators on these things, so
// replacing with a slist would probably be a bad idea.
typedef std::vector<int> PATH_TYPE;
typedef std::list<PATH_TYPE> PATH_LIST;
typedef PATH_LIST::const_iterator PATH_LIST_CI;

typedef std::map<int, PATH_LIST> INT_PATH_LIST_MAP;
typedef INT_PATH_LIST_MAP::const_iterator INT_PATH_LIST_MAP_CI;
typedef INT_PATH_LIST_MAP::iterator INT_PATH_LIST_MAP_I;

// --- --- --- --- --- --- --- --- --- --- --- --- ---
//
//
// --- --- --- --- --- --- --- --- --- --- --- --- ---

//! \brief find all bond subgraphs in a range of sizes
/*!
 *   \param mol - the molecule to be considered
 *   \param lowerLen - the minimum subgraph size to find
 *   \param upperLen - the maximum subgraph size to find
 *   \param useHs     - if set, hydrogens in the graph will be considered
 *                      eligible to be in paths. NOTE: this will not add
 *                      Hs to the graph.
 *   \param rootedAtAtom - if non-negative, only subgraphs that start at
 *                         this atom will be returned.
 *
 *   The result is a map from subgraph size -> list of paths
 *               (i.e. list of list of bond indices)
 */
RDKIX_SUBGRAPHS_EXPORT INT_PATH_LIST_MAP findAllSubgraphsOfLengthsMtoN(
    const ROMol &mol, unsigned int lowerLen, unsigned int upperLen,
    bool useHs = false, int rootedAtAtom = -1);

//! \brief find all bond subgraphs of a particular size
/*!
 *   \param mol - the molecule to be considered
 *   \param targetLen - the length of the subgraphs to be returned
 *   \param useHs     - if set, hydrogens in the graph will be considered
 *                      eligible to be in paths. NOTE: this will not add
 *                      Hs to the graph.
 *   \param rootedAtAtom - if non-negative, only subgraphs that start at
 *                         this atom will be returned.
 *
 *
 *   The result is a list of paths (i.e. list of list of bond indices)
 */
RDKIX_SUBGRAPHS_EXPORT PATH_LIST
findAllSubgraphsOfLengthN(const ROMol &mol, unsigned int targetLen,
                          bool useHs = false, int rootedAtAtom = -1);

//! \brief find unique bond subgraphs of a particular size
/*!
 *   \param mol - the molecule to be considered
 *   \param targetLen - the length of the subgraphs to be returned
 *   \param useHs     - if set, hydrogens in the graph will be considered
 *                      eligible to be in paths. NOTE: this will not add
 *                      Hs to the graph.
 *   \param useBO     - if set, bond orders will be considered when uniquifying
 *                      the paths
 *   \param rootedAtAtom - if non-negative, only subgraphs that start at
 *                         this atom will be returned.
 *
 *   The result is a list of paths (i.e. list of list of bond indices)
 */
RDKIX_SUBGRAPHS_EXPORT PATH_LIST findUniqueSubgraphsOfLengthN(
    const ROMol &mol, unsigned int targetLen, bool useHs = false,
    bool useBO = true, int rootedAtAtom = -1);
//! \brief find all paths of a particular size
/*!
 *   \param mol - the molecule to be considered
 *   \param targetLen - the length of the paths to be returned
 *   \param useBonds  - if set, the path indices will be bond indices,
 *                      not atom indices
 *   \param useHs     - if set, hydrogens in the graph will be considered
 *                      eligible to be in paths. NOTE: this will not add
 *                      Hs to the graph.
 *   \param rootedAtAtom - if non-negative, only subgraphs that start at
 *                         this atom will be returned.
 *   \param onlyShortestPaths - if set then only paths which are <= the shortest
 *                              path between the begin and end atoms will be
 *                              included in the results
 *
 *   The result is a list of paths (i.e. list of list of bond indices)
 */
RDKIX_SUBGRAPHS_EXPORT PATH_LIST findAllPathsOfLengthN(
    const ROMol &mol, unsigned int targetLen, bool useBonds = true,
    bool useHs = false, int rootedAtAtom = -1, bool onlyShortestPaths = false);
RDKIX_SUBGRAPHS_EXPORT INT_PATH_LIST_MAP findAllPathsOfLengthsMtoN(
    const ROMol &mol, unsigned int lowerLen, unsigned int upperLen,
    bool useBonds = true, bool useHs = false, int rootedAtAtom = -1,
    bool onlyShortestPaths = false);

//! \brief Find bond subgraphs of a particular radius around an atom.
//!        Return empty result if there is no bond at the requested radius.
/*!
 *   \param mol - the molecule to be considered
 *   \param radius - the radius of the subgraphs to be considered
 *   \param rootedAtAtom - the atom to consider
 *   \param useHs     - if set, hydrogens in the graph will be considered
 *                      eligible to be in paths. NOTE: this will not add
 *                      Hs to the graph.
 *   \param enforceSize - If false, all the bonds within the requested radius
 *                        (<= radius) is collected. Otherwise, at least one bond
 *                        located at the requested radius must be found and
 * added. \param atomMap - Optional: If provided, it will measure the minimum
 * distance of the atom from the rooted atom (start with 0 from the rooted
 * atom). The result is a pair of the atom ID and the distance. The result is a
 * path (a vector of bond indices)
 */
RDKIX_SUBGRAPHS_EXPORT PATH_TYPE findAtomEnvironmentOfRadiusN(
    const ROMol &mol, unsigned int radius, unsigned int rootedAtAtom,
    bool useHs = false, bool enforceSize = true,
    std::unordered_map<unsigned int, unsigned int> *atomMap = nullptr);

}  // namespace RDKix

#endif
