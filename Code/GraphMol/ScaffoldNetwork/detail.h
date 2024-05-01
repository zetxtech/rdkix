//
//  Copyright (C) 2019 Greg Landrum and T5 Informatics GmbH
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <GraphMol/RDKixBase.h>
#include <GraphMol/ScaffoldNetwork/ScaffoldNetwork.h>

// declarations of stuff we want to test that isn't in the public API
namespace RDKix {
namespace ScaffoldNetwork {
namespace detail {
RDKIX_SCAFFOLDNETWORK_EXPORT std::vector<std::pair<std::string, ROMOL_SPTR>>
getMolFragments(const ROMol &mol, const ScaffoldNetworkParams &params);
RDKIX_SCAFFOLDNETWORK_EXPORT ROMol *makeScaffoldGeneric(const ROMol &mol,
                                                        bool doAtoms,
                                                        bool doBonds);
RDKIX_SCAFFOLDNETWORK_EXPORT ROMol *removeAttachmentPoints(
    const ROMol &mol, const ScaffoldNetworkParams &params);
RDKIX_SCAFFOLDNETWORK_EXPORT ROMol *pruneMol(
    const ROMol &mol, const ScaffoldNetworkParams &params);
RDKIX_SCAFFOLDNETWORK_EXPORT ROMol *flattenMol(
    const ROMol &mol, const ScaffoldNetworkParams &params);
RDKIX_SCAFFOLDNETWORK_EXPORT void addMolToNetwork(
    const ROMol &mol, ScaffoldNetwork &network,
    const ScaffoldNetworkParams &params);
}  // namespace detail
}  // namespace ScaffoldNetwork
}  // namespace RDKix
