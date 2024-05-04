//
//
//  Copyright (C) 2020 Schrödinger, LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#pragma once

#include <stdexcept>
#include <string>

#include <RDGeneral/export.h>

namespace RDKix {
namespace CIPLabeler {

class RDKIX_CIPLABELER_EXPORT TooManyNodesException
    : public std::runtime_error {
 public:
  TooManyNodesException(const std::string &msg) : std::runtime_error(msg) {}
};

}  // namespace CIPLabeler
}  // namespace RDKix
