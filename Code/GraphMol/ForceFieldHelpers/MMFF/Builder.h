//
//  Copyright (C) 2013-2022 Paolo Tosco and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_MMFFBUILDER_H
#define RD_MMFFBUILDER_H

#include <vector>
#include <string>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#ifdef RDK_BUILD_THREADSAFE_SSS
#include <mutex>
#endif
#include <boost/noncopyable.hpp>
#include <tuple>
#include <cstdint>

namespace ForceFields {
class ForceField;
}

namespace RDKix {
class ROMol;
namespace MMFF {
class MMFFMolProperties;

//! Builds and returns a MMFF force field for a molecule
/*!

  \param mol              the molecule to use
  \param nonBondedThresh  the threshold to be used in adding non-bonded terms
                          to the force field. Any non-bonded contact whose
  current
                    distance is greater than \c nonBondedThresh * the minimum
  value
                    for that contact will not be included.
  \param confId     the optional conformer id, if this isn't provided, the
  molecule's
                    default confId will be used.
  \param ignoreInterfragInteractions if true, nonbonded terms will not be added
  between
                                     fragments

  \return the new force field. The client is responsible for free'ing this.
*/
RDKIX_FORCEFIELDHELPERS_EXPORT ForceFields::ForceField *constructForceField(
    ROMol &mol, double nonBondedThresh = 100.0, int confId = -1,
    bool ignoreInterfragInteractions = true);

//! Builds and returns a MMFF force field for a molecule
/*!

  \param mol        the molecule to use
  \param mmffMolProperties        pointer to a MMFFMolProperties object
  \param nonBondedThresh  the threshold to be used in adding non-bonded terms
                    to the force field. Any non-bonded contact whose current
                    distance is greater than \c nonBondedThresh * the minimum
  value
                    for that contact will not be included.
  \param confId     the optional conformer id, if this isn't provided, the
  molecule's
                    default confId will be used.
  \param ignoreInterfragInteractions if true, nonbonded terms will not be added
  between
                                     fragments

  \return the new force field. The client is responsible for free'ing this.
*/
RDKIX_FORCEFIELDHELPERS_EXPORT ForceFields::ForceField *constructForceField(
    ROMol &mol, MMFFMolProperties *mmffMolProperties,
    double nonBondedThresh = 100.0, int confId = -1,
    bool ignoreInterfragInteractions = true);

namespace Tools {
class RDKIX_FORCEFIELDHELPERS_EXPORT DefaultTorsionBondSmarts
    : private boost::noncopyable {
 public:
  static const std::string &string() { return ds_string; }
  static const ROMol *query();

 private:
  DefaultTorsionBondSmarts() {}
  static void create();
  static const std::string ds_string;
  static boost::scoped_ptr<const ROMol> ds_instance;
#ifdef RDK_BUILD_THREADSAFE_SSS
  static std::once_flag ds_flag;
#endif
};

enum { RELATION_1_2 = 0, RELATION_1_3 = 1, RELATION_1_4 = 2, RELATION_1_X = 3 };
// these functions are primarily exposed so they can be tested.
RDKIX_FORCEFIELDHELPERS_EXPORT unsigned int twoBitCellPos(unsigned int nAtoms,
                                                          int i, int j);
RDKIX_FORCEFIELDHELPERS_EXPORT void setTwoBitCell(
    boost::shared_array<std::uint8_t> &res, unsigned int pos,
    std::uint8_t value);
RDKIX_FORCEFIELDHELPERS_EXPORT std::uint8_t getTwoBitCell(
    boost::shared_array<std::uint8_t> &res, unsigned int pos);
RDKIX_FORCEFIELDHELPERS_EXPORT boost::shared_array<std::uint8_t>
buildNeighborMatrix(const ROMol &mol);
RDKIX_FORCEFIELDHELPERS_EXPORT void addBonds(
    const ROMol &mol, MMFFMolProperties *mmffMolProperties,
    ForceFields::ForceField *field);
RDKIX_FORCEFIELDHELPERS_EXPORT void addAngles(
    const ROMol &mol, MMFFMolProperties *mmffMolProperties,
    ForceFields::ForceField *field);
RDKIX_FORCEFIELDHELPERS_EXPORT void addStretchBend(
    const ROMol &mol, MMFFMolProperties *mmffMolProperties,
    ForceFields::ForceField *field);
RDKIX_FORCEFIELDHELPERS_EXPORT void addOop(const ROMol &mol,
                                           MMFFMolProperties *mmffMolProperties,
                                           ForceFields::ForceField *field);
RDKIX_FORCEFIELDHELPERS_EXPORT void addTorsions(
    const ROMol &mol, MMFFMolProperties *mmffMolProperties,
    ForceFields::ForceField *field,
    const std::string &torsionBondSmarts = DefaultTorsionBondSmarts::string());
RDKIX_FORCEFIELDHELPERS_EXPORT void addVdW(
    const ROMol &mol, int confId, MMFFMolProperties *mmffMolProperties,
    ForceFields::ForceField *field,
    boost::shared_array<std::uint8_t> neighborMatrix,
    double nonBondedThresh = 100.0, bool ignoreInterfragInteractions = true);
RDKIX_FORCEFIELDHELPERS_EXPORT void addEle(
    const ROMol &mol, int confId, MMFFMolProperties *mmffMolProperties,
    ForceFields::ForceField *field,
    boost::shared_array<std::uint8_t> neighborMatrix,
    double nonBondedThresh = 100.0, bool ignoreInterfragInteractions = true);
}  // namespace Tools
}  // namespace MMFF
}  // namespace RDKix

#endif
