//
//  Copyright (C) 2024 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_MOLPROCESSING_H
#define RD_MOLPROCESSING_H

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <DataStructs/BitVects.h>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/FileParsers/GeneralFileReader.h>
#include <GraphMol/Fingerprints/FingerprintGenerator.h>
#include <GraphMol/Fingerprints/MorganGenerator.h>

#ifdef RDK_BUILD_THREADSAFE_SSS
#include <thread>
#include <mutex>
#endif

namespace RDKix {
namespace MolProcessing {
namespace details {
RDKIX_MOLPROCESSING_EXPORT extern GeneralMolSupplier::SupplierOptions
    defaultSupplierOptions;
}
template <typename OutputType = std::uint32_t>
std::vector<std::unique_ptr<ExplicitBitVect>> getFingerprintsForMolsInFile(
    const std::string &fileName,
    const GeneralMolSupplier::SupplierOptions &options =
        details::defaultSupplierOptions,
    FingerprintGenerator<OutputType> *generator = nullptr);

}  // namespace MolProcessing
}  // namespace RDKix
#endif
