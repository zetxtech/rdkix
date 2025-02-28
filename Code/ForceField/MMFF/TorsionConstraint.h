//
//  Copyright (C) 2013 Paolo Tosco
//
//  Copyright (C) 2004-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef __RD_MMFFTORSIONCONSTRAINT_H__
#define __RD_MMFFTORSIONCONSTRAINT_H__
#include <iostream>
#include <ForceField/Contrib.h>

namespace ForceFields {
namespace MMFF {

//! A dihedral angle range constraint modelled after a TorsionContrib
class RDKIX_FORCEFIELD_EXPORT TorsionConstraintContrib
    : public ForceFieldContrib {
 public:
  TorsionConstraintContrib() {}
  //! Constructor
  /*!
  \param owner          pointer to the owning ForceField
  \param idx1           index of atom1 in the ForceField's positions
  \param idx2           index of atom2 in the ForceField's positions
  \param idx3           index of atom3 in the ForceField's positions
  \param idx4           index of atom4 in the ForceField's positions
  \param minDihedralDeg minimum dihedral angle
  \param maxDihedralDeg maximum dihedral angle
  \param forceConst     force Constant

  */
  TorsionConstraintContrib(ForceField *owner, unsigned int idx1,
                           unsigned int idx2, unsigned int idx3,
                           unsigned int idx4, double minDihedralDeg,
                           double maxDihedralDeg, double forceConst);
  TorsionConstraintContrib(ForceField *owner, unsigned int idx1,
                           unsigned int idx2, unsigned int idx3,
                           unsigned int idx4, bool relative,
                           double minDihedralDeg, double maxDihedralDeg,
                           double forceConst);

  ~TorsionConstraintContrib() override = default;
  double getEnergy(double *pos) const override;

  void getGrad(double *pos, double *grad) const override;
  TorsionConstraintContrib *copy() const override {
    return new TorsionConstraintContrib(*this);
  }

 private:
  void setParameters(ForceField *owner, unsigned int idx1, unsigned int idx2,
                     unsigned int idx3, unsigned int idx4,
                     double minDihedralDeg, double maxDihedralDeg,
                     double forceConst);
  double computeDihedralTerm(double dihedral) const;
  int d_at1Idx{-1}, d_at2Idx{-1}, d_at3Idx{-1},
      d_at4Idx{-1};  //!< indices of atoms forming the dihedral angle
  double d_minDihedralDeg,
      d_maxDihedralDeg;    //!< rest amplitudes of the dihedral angle
  double d_forceConstant;  //!< force constant of the angle constraint
};
}  // namespace MMFF
}  // namespace ForceFields
#endif
