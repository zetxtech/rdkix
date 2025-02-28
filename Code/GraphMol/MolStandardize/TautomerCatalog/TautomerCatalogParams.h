//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_TAUTOMER_CATALOG_PARAMS_H
#define RD_TAUTOMER_CATALOG_PARAMS_H

#include <Catalogs/CatalogParams.h>
#include "TautomerCatalogUtils.h"
#include <GraphMol/RDKixBase.h>
#include <string>
#include <vector>
#include <iostream>

namespace RDKix {
class ROMol;

namespace MolStandardize {
class TautomerTransform;

using TautomerTransformDefs =
    std::vector<std::tuple<std::string, std::string, std::string, std::string>>;

namespace defaults {
RDKIX_MOLSTANDARDIZE_EXPORT extern const TautomerTransformDefs
    defaultTautomerTransforms;
RDKIX_MOLSTANDARDIZE_EXPORT extern const TautomerTransformDefs
    defaultTautomerTransformsv1;
}  // namespace defaults
class RDKIX_MOLSTANDARDIZE_EXPORT TautomerCatalogParams
    : public RDCatalog::CatalogParams {
 public:
  TautomerCatalogParams() {
    d_typeStr = "Tautomer Catalog Parameters";
    d_transforms.clear();
  }

  TautomerCatalogParams(const std::string &tautomerFile);
  TautomerCatalogParams(const TautomerTransformDefs &data);
  // copy constructor
  TautomerCatalogParams(const TautomerCatalogParams &other);

  ~TautomerCatalogParams() override;

  const std::vector<TautomerTransform> &getTransforms() const;

  const TautomerTransform getTransform(unsigned int fid) const;

  void toStream(std::ostream &) const override;
  std::string Serialize() const override;
  void initFromStream(std::istream &ss) override;
  void initFromString(const std::string &text) override;

 private:
  //		std::vector<std::pair<ROMol*, ROMol*>> d_pairs;
  std::vector<TautomerTransform> d_transforms;

};  // class TautomerCatalogParams

}  // namespace MolStandardize
}  // namespace RDKix

#endif
