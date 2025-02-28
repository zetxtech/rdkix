//
//  Copyright (C) 2004-2019 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_BOUNDS_MATRIX_BUILDER_H
#define RD_BOUNDS_MATRIX_BUILDER_H

#include <DistGeom/BoundsMatrix.h>

namespace RDKix {
class ROMol;
namespace DGeomHelpers {
//! Set default upper and lower distance bounds in a distance matrix
/*!
  \param mmat        pointer to the bounds matrix to be altered
  \param defaultMin  default value for the lower distance bounds
  \param defaultMax  default value for the upper distance bounds
*/
RDKIX_DISTGEOMHELPERS_EXPORT void initBoundsMat(DistGeom::BoundsMatrix *mmat,
                                                double defaultMin = 0.0,
                                                double defaultMax = 1000.0);
/*! \overload
 */
RDKIX_DISTGEOMHELPERS_EXPORT void initBoundsMat(DistGeom::BoundsMatPtr mmat,
                                                double defaultMin = 0.0,
                                                double defaultMax = 1000.0);

//! Set upper and lower distance bounds between atoms in a molecule based on
/// topology
/*!
  This consists of setting 1-2, 1-3 and 1-4 distance based on bond lengths,
  bond angles and torsion angle ranges. Optionally 1-5 bounds can also be set,
  in particular, for path that contain rigid 1-4 paths.
  The final step involves setting lower bound to the sum of the vdW radii for
  the remaining atom pairs.
  \param mol          The molecule of interest
  \param mmat         Bounds matrix to the bounds are written
  \param set15bounds  If true try to set 1-5 bounds also based on topology
  \param scaleVDW     If true scale the sum of the vdW radii while setting lower
  bounds
                      so that a smaller value (0.7*(vdw1 + vdw2) ) is used for
  paths
                      that are less five bonds apart.
  \param useMacrocycle14config  If 1-4 distances bound heuristics for
  macrocycles is used <b>Note</b> For some strained systems the bounds matrix
  resulting from setting 1-5 bounds may fail triangle smoothing. In these cases
  it is recommended to back out and recompute the bounds matrix with no 1-5
  bounds and with vdW scaling.
*/
RDKIX_DISTGEOMHELPERS_EXPORT void setTopolBounds(
    const ROMol &mol, DistGeom::BoundsMatPtr mmat, bool set15bounds = true,
    bool scaleVDW = false, bool useMacrocycle14config = false,
    bool forceTransAmides = true);

/*! \overload for experimental torsion angle preferences
 */
RDKIX_DISTGEOMHELPERS_EXPORT void setTopolBounds(
    const ROMol &mol, DistGeom::BoundsMatPtr mmat,
    std::vector<std::pair<int, int>> &bonds,
    std::vector<std::vector<int>> &angles, bool set15bounds = true,
    bool scaleVDW = false, bool useMacrocycle14config = false,
    bool forceTransAmides = true);

//! generate the vectors of bonds and angles used by (ET)KDG
RDKIX_DISTGEOMHELPERS_EXPORT void collectBondsAndAngles(
    const ROMol &mol, std::vector<std::pair<int, int>> &bonds,
    std::vector<std::vector<int>> &angles);

}  // namespace DGeomHelpers
}  // namespace RDKix
#endif
