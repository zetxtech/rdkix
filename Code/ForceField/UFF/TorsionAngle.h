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
#ifndef __RD_TORSIONANGLE_H__
#define __RD_TORSIONANGLE_H__

#include <ForceField/Contrib.h>
#include <Geometry/point.h>

// we need this so that we get the hybridizations:
#include <GraphMol/Atom.h>

namespace RDGeom {
class Point3D;
}

namespace ForceFields {
namespace UFF {
class AtomicParams;

//! the torsion term for the Universal Force Field
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
    \param bondOrder23 order of the torsional bond between atoms 2 and 3 (as a
    double)
    \param atNum2      atomic number of atom2
    \param atNum3      atomic number of atom3
    \param hyb2        hybridization of atom2
    \param hyb3        hybridization of atom3
    \param at2Params   pointer to the parameters for atom 2
    \param at3Params   pointer to the parameters for atom 3
    \param endAtomIsSP2 (optional)
      This boolean is used to signal whether either atom1 or atom4 are
      RDKix::Atom::SP2 hybridized.
      This triggers a special case when either of these cases holds:
       - atom1 is RDKix::Atom::SP2, atom2 is RDKix::Atom::SP2
         and atom3 is RDKix::Atom::SP3
       - atom4 is RDKix::Atom::SP2, atom3 is RDKix::Atom::SP2
         and atom2 is RDKix::Atom::SP3
  */
  TorsionAngleContrib(ForceField *owner, unsigned int idx1, unsigned int idx2,
                      unsigned int idx3, unsigned int idx4, double bondOrder23,
                      int atNum2, int atNum3,
                      RDKix::Atom::HybridizationType hyb2,
                      RDKix::Atom::HybridizationType hyb3,
                      const AtomicParams *at2Params,
                      const AtomicParams *at3Params, bool endAtomIsSP2 = false);
  double getEnergy(double *pos) const override;
  void getGrad(double *pos, double *grad) const override;
  void scaleForceConstant(unsigned int count) {
    this->d_forceConstant /= static_cast<double>(count);
  }
  TorsionAngleContrib *copy() const override {
    return new TorsionAngleContrib(*this);
  }

 private:
  int d_at1Idx{-1};
  int d_at2Idx{-1};
  int d_at3Idx{-1};
  int d_at4Idx{-1};
  unsigned int d_order{0};
  double d_forceConstant, d_cosTerm;

  //! returns dE/dTheta
  double getThetaDeriv(double cosTheta, double sinTheta) const;

  //! calculate default values of the torsion parameters.
  /*!
     see the constructor for an explanation of the arguments
  */
  void calcTorsionParams(double bondOrder23, int atNum2, int atNum3,
                         RDKix::Atom::HybridizationType hyb2,
                         RDKix::Atom::HybridizationType hyb3,
                         const AtomicParams *at2Params,
                         const AtomicParams *at3Params, bool endAtomIsSP2);
};

namespace Utils {
//! calculates and returns the cosine of a torsion angle
RDKIX_FORCEFIELD_EXPORT double calculateCosTorsion(const RDGeom::Point3D &p1,
                                                   const RDGeom::Point3D &p2,
                                                   const RDGeom::Point3D &p3,
                                                   const RDGeom::Point3D &p4);
RDKIX_FORCEFIELD_EXPORT void calcTorsionGrad(RDGeom::Point3D *r,
                                             RDGeom::Point3D *t, double *d,
                                             double **g, double &sinTerm,
                                             double &cosPhi);
RDKIX_FORCEFIELD_EXPORT double equation17(double bondOrder23,
                                          const AtomicParams *at2Params,
                                          const AtomicParams *at3Params);
RDKIX_FORCEFIELD_EXPORT bool isInGroup6(int num);
}  // namespace Utils
}  // namespace UFF
}  // namespace ForceFields
#endif
