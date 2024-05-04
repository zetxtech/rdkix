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
#include <GraphMol/RDKixBase.h>
#include <GraphMol/FragCatalog/FragCatGenerator.h>

namespace python = boost::python;
namespace RDKix {
struct fragcatgen_wrapper {
  static void wrap() {
    python::class_<FragCatGenerator>("FragCatGenerator", python::init<>())
        .def("AddFragsFromMol", &FragCatGenerator::addFragsFromMol);
  };
};  // end of struct
}  // namespace RDKix

void wrap_fragcatgen() { RDKix::fragcatgen_wrapper::wrap(); }
