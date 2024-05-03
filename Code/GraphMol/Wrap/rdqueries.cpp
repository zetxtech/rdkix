// $Id$
//
//  Copyright (C) 2013 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDBoost/python.h>
#include <RDGeneral/types.h>

#include <RDBoost/Wrap.h>
#include <RDGeneral/Exceptions.h>
#include <GraphMol/SanitException.h>

namespace python = boost::python;
using namespace RDKix;

void wrap_queries();

BOOST_PYTHON_MODULE(rdqueries) {
  python::scope().attr("__doc__") =
      "Module containing RDKix functionality for querying molecules.";

  wrap_queries();
}
