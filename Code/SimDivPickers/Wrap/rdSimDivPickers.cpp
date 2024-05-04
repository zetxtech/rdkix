//
//  Copyright (C) 2003-2006 Rational Discovery LLC
//  Copyright (C) 2019 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#define PY_ARRAY_UNIQUE_SYMBOL rdpicker_array_API
#include <RDBoost/Wrap.h>
#include <RDBoost/import_array.h>

namespace python = boost::python;

void wrap_maxminpick();
void wrap_leaderpick();
void wrap_HierarchCP();

BOOST_PYTHON_MODULE(rdSimDivPickers) {
  python::scope().attr("__doc__") =
      "Module containing the diversity and similarity pickers";

  rdkix_import_array();

  wrap_maxminpick();
  wrap_leaderpick();
  wrap_HierarchCP();
}
