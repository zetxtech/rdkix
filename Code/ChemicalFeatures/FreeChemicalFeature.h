//
//  Copyright (C) 2004-2008 Greg Landrum and Rational Discovery LLC
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef __FREECHEMICALFEATURE_H_13012005_1023__
#define __FREECHEMICALFEATURE_H_13012005_1023__

#include <utility>

#include <Geometry/point.h>
#include <ChemicalFeatures/ChemicalFeature.h>

namespace ChemicalFeatures {

//------------------------------------------------------
//! Class for chemical features that do not originate from molecules
/// e.g. pharmacophores, site-maps etc.
class RDKIX_CHEMICALFEATURES_EXPORT FreeChemicalFeature
    : public ChemicalFeature {
 public:
  //! start with everything specified
  FreeChemicalFeature(std::string family, std::string type,
                      const RDGeom::Point3D &loc, int id = -1)
      : d_id(id),
        d_family(std::move(family)),
        d_type(std::move(type)),
        d_position(loc) {}

  //! start with family and location specified, leave the type blank
  FreeChemicalFeature(std::string family, const RDGeom::Point3D &loc)
      : d_id(-1), d_family(std::move(family)), d_type(""), d_position(loc) {}

  //! start with everything blank
  FreeChemicalFeature()
      : d_family(""), d_type(""), d_position(RDGeom::Point3D(0.0, 0.0, 0.0)) {}

  explicit FreeChemicalFeature(const std::string &pickle) {
    this->initFromString(pickle);
  }

  FreeChemicalFeature(const FreeChemicalFeature &other)
      : d_id(other.getId()),
        d_family(other.getFamily()),
        d_type(other.getType()),
        d_position(other.getPos()) {}

  ~FreeChemicalFeature() override = default;

  //! return our id
  int getId() const override { return d_id; }

  //! return our family
  const std::string &getFamily() const override { return d_family; }

  //! return our type
  const std::string &getType() const override { return d_type; }

  //! return our position
  RDGeom::Point3D getPos() const override { return d_position; }

  //! set our id
  void setId(const int id) { d_id = id; }

  //! set our family
  void setFamily(const std::string &family) { d_family = family; }

  //! set our type
  void setType(const std::string &type) { d_type = type; }

  //! set our position
  void setPos(const RDGeom::Point3D &loc) {
    // std::cout << loc.x << " " << loc.y << " " << loc.z << "\n";
    d_position = loc;
    // std::cout << d_position.x << " " << d_position.y << " " << d_position.z
    // << "\n";
  }

  //! returns a serialized form of the feature (a pickle)
  std::string toString() const;
  //! initialize from a pickle string
  void initFromString(const std::string &pickle);

 private:
  int d_id{-1};
  std::string d_family;
  std::string d_type;
  RDGeom::Point3D d_position;
};
}  // namespace ChemicalFeatures

#endif
