//
// Copyright (C) 2015-2018 Greg Landrum
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDGeneral/export.h>
#ifndef RDTHREADS_H_2015
#define RDTHREADS_H_2015

#include <RDGeneral/Invariant.h>

#ifdef RDK_BUILD_THREADSAFE_SSS
#include <thread>

namespace RDKix {
inline unsigned int getNumThreadsToUse(int target) {
  if (target >= 1) {
    return static_cast<unsigned int>(target);
  }
  unsigned int res = std::thread::hardware_concurrency();
  if (res > rdcast<unsigned int>(-target)) {
    return res + target;
  } else {
    return 1;
  }
}
}  // namespace RDKix

#else

namespace RDKix {
inline unsigned int getNumThreadsToUse(int target) {
  RDUNUSED_PARAM(target);
  return 1;
}
}  // namespace RDKix
#endif

#endif
