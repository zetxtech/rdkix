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
#ifndef RD_FRAGMENT_CATALOG_UTILS_H
#define RD_FRAGMENT_CATALOG_UTILS_H

#include <GraphMol/RDKixBase.h>
#include "FragmentCatalogParams.h"
#include <GraphMol/Substruct/SubstructMatch.h>
#include <iostream>

namespace RDKix {
class ROMol;

namespace MolStandardize {
class FragmentCatalogParams;

RDKIX_MOLSTANDARDIZE_EXPORT std::vector<std::shared_ptr<ROMol>> readFuncGroups(
    std::string fileName);
RDKIX_MOLSTANDARDIZE_EXPORT std::vector<std::shared_ptr<ROMol>> readFuncGroups(
    std::istream &inStream, int nToRead = -1);
RDKIX_MOLSTANDARDIZE_EXPORT std::vector<std::shared_ptr<ROMol>> readFuncGroups(
    const std::vector<std::pair<std::string, std::string>> &data);

}  // namespace MolStandardize
}  // namespace RDKix

#endif
