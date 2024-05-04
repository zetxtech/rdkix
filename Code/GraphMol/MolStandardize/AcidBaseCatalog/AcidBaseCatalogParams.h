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
#ifndef RD_ACIDBASE_CATALOG_PARAMS_H
#define RD_ACIDBASE_CATALOG_PARAMS_H

#include <Catalogs/CatalogParams.h>
#include "AcidBaseCatalogUtils.h"
#include <GraphMol/RDKixBase.h>
#include <string>
#include <vector>
#include <iostream>

namespace RDKix {
class ROMol;

namespace MolStandardize {
class RDKIX_MOLSTANDARDIZE_EXPORT AcidBaseCatalogParams
    : public RDCatalog::CatalogParams {
 public:
  AcidBaseCatalogParams() {
    d_typeStr = "AcidBase Catalog Parameters";
    d_pairs.clear();
  }

  AcidBaseCatalogParams(const std::string &acidBaseFile);
  AcidBaseCatalogParams(std::istream &acidBaseFile);
  AcidBaseCatalogParams(
      const std::vector<std::tuple<std::string, std::string, std::string>>
          &data);

  // copy constructor
  AcidBaseCatalogParams(const AcidBaseCatalogParams &other);

  ~AcidBaseCatalogParams() override;

  unsigned int getNumPairs() const {
    return static_cast<unsigned int>(d_pairs.size());
  }

  const std::vector<std::pair<ROMOL_SPTR, ROMOL_SPTR>> &getPairs() const;

  const std::pair<ROMOL_SPTR, ROMOL_SPTR> getPair(unsigned int fid) const;

  void toStream(std::ostream &) const override;
  std::string Serialize() const override;
  void initFromStream(std::istream &ss) override;
  void initFromString(const std::string &text) override;

 private:
  std::vector<std::pair<ROMOL_SPTR, ROMOL_SPTR>> d_pairs;

};  // class AcidBaseCatalogParams

}  // namespace MolStandardize
}  // namespace RDKix

#endif
