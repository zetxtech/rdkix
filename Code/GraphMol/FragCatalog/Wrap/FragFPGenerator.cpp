// $Id$
//
//  Copyright (C) 2003-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDBoost/python.h>
#include <DataStructs/BitVects.h>

#include <GraphMol/FragCatalog/FragFPGenerator.h>

namespace python = boost::python;
namespace RDKix {
struct fragFPgen_wrapper {
  static void wrap() {
    python::class_<FragFPGenerator>("FragFPGenerator", python::init<>())
        .def("GetFPForMol", &FragFPGenerator::getFPForMol,
             python::return_value_policy<python::manage_new_object>());
  };
};
}  // namespace RDKix

void wrap_fragFPgen() { RDKix::fragFPgen_wrapper::wrap(); }
