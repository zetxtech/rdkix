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
#ifndef RD_MMFFTORSIONANGLE_H
#define RD_MMFFTORSIONANGLE_H

#include <ForceField/Contrib.h>
#include <tuple>

namespace RDGeom {
class Point3D;
}

namespace ForceFields {
namespace MMFF {
class MMFFTor;

//! the torsion term for MMFF
class RDKIX_FORCEFIELD_EXPORT TorsionAngleContrib : public ForceFieldContrib {
 public:
  TorsionAngleContrib() {}
  //! Constructor
  /*!
    The torsion is between atom1 - atom2 - atom3 - atom4
    (i.e the angle between bond atom1-atom2 and bond atom3-atom4
    while looking down bond atom2-atom3)

    \param owner       pointer to the owning ForceField
    \param idx1        index of atom1 in the ForceField's positions
    \param idx2        index of atom2 in the ForceField's positions
    \param idx3        index of atom3 in the ForceField's positions
    \param idx4        index of atom4 in the ForceField's positions
    \param torsionType MMFF type of the torsional bond between atoms 2 and 3
  */
  TorsionAngleContrib(ForceField *owner, unsigned int idx1, unsigned int idx2,
                      unsigned int idx3, unsigned int idx4,
                      const MMFFTor *mmffTorParams);
  double getEnergy(double *pos) const override;
  void getGrad(double *pos, double *grad) const override;
  TorsionAngleContrib *copy() const override {
    return new TorsionAngleContrib(*this);
  }

 private:
  int d_at1Idx{-1}, d_at2Idx{-1}, d_at3Idx{-1}, d_at4Idx{-1};
  double d_V1, d_V2, d_V3;
};

namespace Utils {
//! calculates and returns the cosine of a torsion angle
RDKIX_FORCEFIELD_EXPORT double calcTorsionCosPhi(const RDGeom::Point3D &iPoint,
                                                 const RDGeom::Point3D &jPoint,
                                                 const RDGeom::Point3D &kPoint,
                                                 const RDGeom::Point3D &lPoint);
//! returns the 3-tuple of a torsion angle force constants
RDKIX_FORCEFIELD_EXPORT std::tuple<double, double, double>
calcTorsionForceConstant(const MMFFTor *mmffTorParams);
//! calculates and returns the torsional MMFF energy
RDKIX_FORCEFIELD_EXPORT double calcTorsionEnergy(const double V1,
                                                 const double V2,
                                                 const double V3,
                                                 const double cosPhi);
RDKIX_FORCEFIELD_EXPORT void calcTorsionGrad(RDGeom::Point3D *r,
                                             RDGeom::Point3D *t, double *d,
                                             double **g, double &sinTerm,
                                             double &cosPhi);
}  // namespace Utils
}  // namespace MMFF
}  // namespace ForceFields
#endif
