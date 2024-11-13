//
//  Copyright (C) 2020 Brian P Kelley
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include "rdDeprotect.h"
#include <RDBoost/python.h>

namespace python = boost::python;

void wrap_deprotect();

BOOST_PYTHON_MODULE(rdDeprotect) { wrap_deprotect(); }
