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
#ifndef __RD_BONDSTRETCH_H__
#define __RD_BONDSTRETCH_H__
#include <ForceField/Contrib.h>

namespace ForceFields {
namespace UFF {
class AtomicParams;

//! The bond-stretch term for the Universal Force Field
class RDKIX_FORCEFIELD_EXPORT BondStretchContrib : public ForceFieldContrib {
 public:
  BondStretchContrib() {}
  //! Constructor
  /*!
    \param owner       pointer to the owning ForceField
    \param idx1        index of end1 in the ForceField's positions
    \param idx2        index of end2 in the ForceField's positions
    \param bondOrder   order of the bond (as a double)
    \param end1Params  pointer to the parameters for end1
    \param end2Params  pointer to the parameters for end2

  */
  BondStretchContrib(ForceField *owner, unsigned int idx1, unsigned int idx2,
                     double bondOrder, const AtomicParams *end1Params,
                     const AtomicParams *end2Params);

  double getEnergy(double *pos) const override;

  void getGrad(double *pos, double *grad) const override;

  BondStretchContrib *copy() const override {
    return new BondStretchContrib(*this);
  }

 private:
  int d_end1Idx{-1};       //!< indices of end points
  int d_end2Idx{-1};       //!< indices of end points
  double d_restLen;        //!< rest length of the bond
  double d_forceConstant;  //!< force constant of the bond
};

namespace Utils {
//! calculates and returns the UFF rest length for a bond
/*!

  \param bondOrder the order of the bond (as a double)
  \param end1Params  pointer to the parameters for end1
  \param end2Params  pointer to the parameters for end2

  \return the rest length

*/
RDKIX_FORCEFIELD_EXPORT double calcBondRestLength(
    double bondOrder, const AtomicParams *end1Params,
    const AtomicParams *end2Params);

//! calculates and returns the UFF force constant for a bond
/*!

  \param restLength  the rest length of the bond
  \param end1Params  pointer to the parameters for end1
  \param end2Params  pointer to the parameters for end2

  \return the force constant

*/
RDKIX_FORCEFIELD_EXPORT double calcBondForceConstant(
    double restLength, const AtomicParams *end1Params,
    const AtomicParams *end2Params);
}  // namespace Utils
}  // namespace UFF
}  // namespace ForceFields
#endif
