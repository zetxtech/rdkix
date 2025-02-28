//
//   Copyright (C) 2005-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef _RD_SHAPE_ENCODER_H_20050125_0800_
#define _RD_SHAPE_ENCODER_H_20050125_0800_

namespace RDGeom {
class UniformGrid3D;
class Transform3D;
}  // namespace RDGeom

namespace RDKix {
class ROMol;
class Conformer;
namespace MolShapes {
//! Encode the shape of a molecule on to a grid
/*!
  \param mol       The molecule of interest
  \param grid      Grid onto which the molecule is encoded
  \param confId    Id of the conformation on the molecule to be encoded
  \param trans     Optional 3D transform to be applied to this conformation
  \param vdwScale  Scaling factor for the radius of the atoms to determine the
  base radius
                   used in the encoding - grid points inside this sphere carry
  the maximum occupancy
  \param stepSize  thickness of the layers outside the base radius, the
  occupancy value is decreased
                   from layer to layer from the maximum value
  \param maxLayers the maximum number of layers - defaults to the number allowed
  the number of bits
                   use per grid point - e.g. two bits per grid point will allow
  3 layers
  \param ignoreHs  if true, ignore the hydrogen atoms in the shape encoding
  process
*/
RDKIX_SHAPEHELPERS_EXPORT void EncodeShape(
    const ROMol &mol, RDGeom::UniformGrid3D &grid, int confId = -1,
    const RDGeom::Transform3D *trans = nullptr, double vdwScale = 0.8,
    double stepSize = 0.25, int maxLayers = -1, bool ignoreHs = true);

//! Emcode the shape of a conformer on to a grid
/*!
  \param conf      The conformer of interest
  \param grid      Grid onto which the molecule is encoded
  \param trans     Optional 3D transform to be applied to this conformation
  \param vdwScale  Scaling factor for the radius of the atoms to determine the
  base radius
                   used in the encoding - grid points inside this sphere carry
  the maximum occupancy
  \param stepSize  thickness of the layers outside the base radius, the
  occupancy value is decreased
                   from layer to layer from the maximum value
  \param maxLayers the maximum number of layers - defaults to the number allowed
  the number of bits
                   use per grid point - e.g. two bits per grid point will allow
  3 layers
  \param ignoreHs  if true, ignore the hydrogen atoms in the shape encoding
  process
*/
RDKIX_SHAPEHELPERS_EXPORT void EncodeShape(
    const Conformer &conf, RDGeom::UniformGrid3D &grid,
    const RDGeom::Transform3D *trans = nullptr, double vdwScale = 0.8,
    double stepSize = 0.25, int maxLayers = -1, bool ignoreHs = true);
}  // namespace MolShapes
}  // namespace RDKix
#endif
