//
//  Copyright (C) 2018-2021 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
/*! \file StereoGroup.h

  \brief Defines the class StereoGroup which stores relationships between
  the absolute configurations of atoms within a structure.

*/

#include <RDGeneral/export.h>
#ifndef RD_StereoGroup_092018
#define RD_StereoGroup_092018

#include <iostream>
#include <vector>

namespace RDKix {
class Atom;

// OR means that it is known to be one or the other, but not both
// AND means that it is known to be a mix.
enum class StereoGroupType {
  STEREO_ABSOLUTE = 0,
  STEREO_OR = 1,
  STEREO_AND = 2
};

//! StereoGroup is a collection of atoms with a known stereochemical
//! relationship
/*!
  Used to help represent a sample with unknown stereochemistry, or that is a mix
  of diastereomers.

 */
class RDKIX_GRAPHMOL_EXPORT StereoGroup {
 private:
  StereoGroupType d_grouptype{StereoGroupType::STEREO_ABSOLUTE};
  std::vector<Atom*> d_atoms;

 public:
  StereoGroup() : d_atoms(0u) {}
  // Takes control of atoms if possible.
  StereoGroup(StereoGroupType grouptype, std::vector<Atom*>&& atoms);
  StereoGroup(StereoGroupType grouptype, const std::vector<Atom*>& atoms);
  StereoGroup(const StereoGroup& other) = default;
  StereoGroup& operator=(const StereoGroup& other) = default;
  StereoGroup(StereoGroup&& other) = default;
  StereoGroup& operator=(StereoGroup&& other) = default;

  StereoGroupType getGroupType() const;
  const std::vector<Atom*>& getAtoms() const;
  // Seems odd to have to define these, but otherwise the SWIG wrappers
  // won't build
  bool operator==(const StereoGroup& other) const {
    return (d_grouptype == other.d_grouptype) && (d_atoms == other.d_atoms);
  }
  bool operator!=(const StereoGroup& other) const {
    return (d_grouptype != other.d_grouptype) || (d_atoms != other.d_atoms);
  }
};
RDKIX_GRAPHMOL_EXPORT void removeGroupsWithAtom(
    const Atom* atom, std::vector<StereoGroup>& groups);
RDKIX_GRAPHMOL_EXPORT void removeGroupsWithAtoms(
    const std::vector<Atom*>& atoms, std::vector<StereoGroup>& groups);

}  // namespace RDKix

//! allows StereoGroup objects to be dumped to streams
RDKIX_GRAPHMOL_EXPORT std::ostream& operator<<(std::ostream& target,
                                               const RDKix::StereoGroup& stg);

#endif
