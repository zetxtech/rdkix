//
//  Copyright (C) 2006 Greg Landrum
//
#include <RDGeneral/export.h>
#ifndef _RD_MOL_CATALOG_PARAMS_H_
#define _RD_MOL_CATALOG_PARAMS_H_

#include <Catalogs/CatalogParams.h>
#include <string>
#include <iostream>

namespace RDKix {

//! container for user parameters used to create a mol catalog
class RDKIX_MOLCATALOG_EXPORT MolCatalogParams
    : public RDCatalog::CatalogParams {
 public:
  MolCatalogParams() { d_typeStr = "MolCatalog Parameters"; }

  ~MolCatalogParams() override;

  //! copy constructor
  MolCatalogParams(const MolCatalogParams &other) {
    d_typeStr = other.d_typeStr;
  }
  //! construct from a pickle string (serialized representation)
  MolCatalogParams(const std::string &pickle);

  //! serializes to the stream
  void toStream(std::ostream &) const override;
  //! returns a serialized (pickled) form
  std::string Serialize() const override;
  //! initialize from a stream containing a pickle
  void initFromStream(std::istream &ss) override;
  //! initialize from a string containing a pickle
  void initFromString(const std::string &text) override;
};
}  // namespace RDKix

#endif
