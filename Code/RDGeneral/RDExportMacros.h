//
//  Copyright (C) 2021 Schrödinger, LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#pragma once

#ifndef SWIG
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#endif

#include <boost/config.hpp>

// RDKix export macro definitions
#ifdef RDKIX_DYN_LINK
#if defined(_WIN32) && defined(BOOST_HAS_DECLSPEC)
#define RDKIX_EXPORT_API __declspec(dllexport)
#define RDKIX_IMPORT_API __declspec(dllimport)
#elif __GNUC__ >= 4 || defined(__clang__)
#define RDKIX_EXPORT_API __attribute__((visibility("default")))
#define RDKIX_IMPORT_API __attribute__((visibility("default")))
#endif  // WIN32
#endif  // RDKIX_DYN_LINK
// RDKix end export macro definitions

#endif  // SWIG

#ifndef RDKIX_EXPORT_API
#define RDKIX_EXPORT_API
#endif
#ifndef RDKIX_IMPORT_API
#define RDKIX_IMPORT_API
#endif
