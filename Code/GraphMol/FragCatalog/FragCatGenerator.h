//
//  Copyright (C) 2003-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_FRAG_CAT_GENERATOR_H
#define RD_FRAG_CAT_GENERATOR_H

#include <Catalogs/Catalog.h>
#include "FragCatalogEntry.h"
#include "FragCatParams.h"
#include <GraphMol/Subgraphs/Subgraphs.h>

namespace RDKix {
class ROMol;

typedef RDCatalog::HierarchCatalog<FragCatalogEntry, FragCatParams, int>
    FragCatalog;

class RDKIX_FRAGCATALOG_EXPORT FragCatGenerator {
 public:
  FragCatGenerator() {}

  unsigned int addFragsFromMol(const ROMol &mol, FragCatalog *fcat);
};
}  // namespace RDKix

#endif
