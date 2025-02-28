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
#ifndef RD_TAUTOMER_CATALOG_ENTRY_H
#define RD_TAUTOMER_CATALOG_ENTRY_H

#include <Catalogs/CatalogEntry.h>
#include <GraphMol/Subgraphs/Subgraphs.h>
#include <GraphMol/Substruct/SubstructMatch.h>
#include <GraphMol/ChemReactions/Reaction.h>
#include "TautomerCatalogParams.h"
#include <GraphMol/RDKixBase.h>

namespace RDKix {
namespace MolStandardize {

class RDKIX_MOLSTANDARDIZE_EXPORT TautomerCatalogEntry
    : public RDCatalog::CatalogEntry {
 public:
  TautomerCatalogEntry() {
    dp_transform = nullptr;
    d_descrip = "";
    dp_props = new Dict();
    setBitId(-1);
  }

  ~TautomerCatalogEntry() override {
    delete dp_transform;
    dp_transform = nullptr;
    delete dp_props;
    dp_props = nullptr;
  }

  // TODO Catalog.h requires a getOrder function
  unsigned int getOrder() const { return 0; }  // dp_mol->getNumBonds(); }

  void toStream(std::ostream &ss) const override;
  std::string Serialize() const override;
  void initFromStream(std::istream &ss) override;
  void initFromString(const std::string &text) override;

 private:
  TautomerTransform *dp_transform;
  Dict *dp_props;
  std::string d_descrip;

};  // class TautomerCatalogEntry

}  // namespace MolStandardize
}  // namespace RDKix

#endif
