//
//  Copyright (C) 2004-2022 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_CANON_H
#define RD_CANON_H

#include <RDGeneral/BoostStartInclude.h>
#include <tuple>
#include <boost/dynamic_bitset.hpp>
#include <RDGeneral/BoostEndInclude.h>

namespace RDKix {
class ROMol;
class Atom;
class Bond;

namespace Canon {
const int MAX_NATOMS = 5000;  //!< used in the canonical traversal code
const int MAX_CYCLES = 1000;  //!< used in the canonical traversal code
const int MAX_BONDTYPE = 32;  //!< used in the canonical traversal code

//! used in traversals of the molecule
typedef enum {
  WHITE_NODE = 0,  //!< not visited
  GREY_NODE,       //!< visited, but not finished
  BLACK_NODE,      //!< visited and finished
} AtomColors;

//! used to indicate types of entries in the molecular stack:
typedef enum {
  MOL_STACK_ATOM = 0,      //!< an Atom
  MOL_STACK_BOND,          //!< a Bond
  MOL_STACK_RING,          //!< a ring closure
  MOL_STACK_BRANCH_OPEN,   //!< beginning of a branch
  MOL_STACK_BRANCH_CLOSE,  //!< end of a branch
} MolStackTypes;

//! used to store components in the molecular stack
typedef union {
  Atom *atom;
  Bond *bond;
} MolStackUnion;

//! these are the actual elements in the molecular stack
class RDKIX_GRAPHMOL_EXPORT MolStackElem {
 public:
  //! construct an Atom node
  explicit MolStackElem(Atom *at) {
    type = MOL_STACK_ATOM;
    obj.atom = at;
  }
  //! construct a bond node
  /*!

     \param bond  pointer to the Bond being added
     \param idx   index of the Atom traversed before this Bond
       (beginAtom in the canonical traversal order)
  */
  explicit MolStackElem(Bond *bond, int idx) {
    type = MOL_STACK_BOND;
    obj.bond = bond;
    number = idx;
  }
  //! construct for a ring closure
  explicit MolStackElem(int idx) {
    type = MOL_STACK_RING;
    number = idx;
  }
  //! construct for a branch opening or closing
  explicit MolStackElem(const char *chr, int idx) {
    switch (chr[0]) {
      case '(':
        type = MOL_STACK_BRANCH_OPEN;
        break;
      case ')':
        type = MOL_STACK_BRANCH_CLOSE;
        break;
      default:
        break;
    }
    number = idx;
  }
  MolStackTypes type;  //!< stores the type of node
  MolStackUnion obj;   //!< holds our pointer (if appropriate)
  int number =
      -1;  //!< stores our number (relevant for bonds and ring closures)
};
typedef std::vector<MolStackElem> MolStack;

//! used to represent possible branches from an atom
typedef std::tuple<int, int, Bond *> PossibleType;

//! constructs the canonical traversal order for a molecular fragment
/*!

  \param mol       the ROMol we're working on
  \param atomIdx   the index of the atom to start the traversal from
  \param colors    the traversal status of each atom in \c mol
  \param ranks     the assigned rank of each atom in \c mol
  \param molStack  the current traversal stack (used to return the results)

  <b>Notes</b>
    - \c mol will, in general, be modified by this operation as bond directions
      and the like are changed to fit the canonical traversal order

 */
RDKIX_GRAPHMOL_EXPORT void canonicalizeFragment(
    ROMol &mol, int atomIdx, std::vector<AtomColors> &colors,
    const std::vector<unsigned int> &ranks, MolStack &molStack,
    const boost::dynamic_bitset<> *bondsInPlay = nullptr,
    const std::vector<std::string> *bondSymbols = nullptr,
    bool doIsomericSmiles = false, bool doRandom = false);

//! Check if a chiral atom needs to have its tag flipped after reading or before
//! writing SMILES
RDKIX_GRAPHMOL_EXPORT bool chiralAtomNeedsTagInversion(const RDKix::ROMol &mol,
                                                       const RDKix::Atom *atom,
                                                       bool isAtomFirst,
                                                       size_t numClosures);

//! Canonicalizes the atom stereo labels in enhanced stereo groups
/*!

  For example, after calling this function the chiral centers in the
  molecules `C[C@H](F)Cl |&1:1|` and `C[C@@H](F)Cl |&1:1|` will have the same
  chiral tags.

*/
RDKIX_GRAPHMOL_EXPORT void canonicalizeEnhancedStereo(
    ROMol &mol, const std::vector<unsigned int> *atomRanks = nullptr);

}  // end of namespace Canon
}  // end of namespace RDKix
#endif
