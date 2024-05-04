//
// Copyright (c) 2010-2018 greg Landrum
//
//   @@ All Rights Reserved  @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

// inspired by:
// https://github.com/openbabel/openbabel/blob/master/src/config.h.cmake
#include <RDGeneral/export.h>
/* Version check macro
   Can be used like #if (RDKIX_VERSION >= RDKIX_VERSION_CHECK(2018, 3, 1)) */
#define RDKIX_VERSION_CHECK(year, month, rev) \
  ((year * 1000) + (month * 10) + (rev))

/* RDKIX_VERSION is (year*1000) + (month*10) + (rev) */
#define RDKIX_VERSION \
  RDKIX_VERSION_CHECK(@RDKix_Year @, @RDKix_intMonth @, @RDKix_Revision @)

namespace RDKix {
RDKIX_RDGENERAL_EXPORT extern const char* rdkixVersion;
RDKIX_RDGENERAL_EXPORT extern const char* boostVersion;
RDKIX_RDGENERAL_EXPORT extern const char* rdkixBuild;
}  // namespace RDKix
