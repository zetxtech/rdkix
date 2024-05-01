//
//  Copyright (C) 2003-2021 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDBoost/python.h>
#include <string>

#include <DataStructs/BitVects.h>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/FragCatalog/FragCatalogEntry.h>
#include <GraphMol/FragCatalog/FragCatGenerator.h>
#include <GraphMol/FragCatalog/FragCatParams.h>
#include <Catalogs/Catalog.h>
#include <Catalogs/CatalogParams.h>

namespace python = boost::python;
namespace RDKix {
struct fragparams_wrapper {
  static void wrap() {
    // this exposed to be read only
    // i.e. one constructed there can be no changes done to the parameter object
    python::class_<FragCatParams>(
        "FragCatParams",
        python::init<int, int, std::string, double>(
            (python::arg("self"), python::arg("lLen"), python::arg("uLen"),
             python::arg("fgroupFilename"), python::arg("tol") = 1e-8)))
        .def("GetTypeString", &FragCatParams::getTypeStr, python::args("self"))
        .def("GetUpperFragLength", &FragCatParams::getUpperFragLength,
             python::args("self"))
        .def("GetLowerFragLength", &FragCatParams::getLowerFragLength,
             python::args("self"))
        .def("GetTolerance", &FragCatParams::getTolerance, python::args("self"))
        .def("GetNumFuncGroups", &FragCatParams::getNumFuncGroups,
             python::args("self"))
        .def("GetFuncGroup",
             (const ROMol* (FragCatParams::*)(int) const) &
                 FragCatParams::getFuncGroup,
             python::return_value_policy<python::reference_existing_object>(),
             python::args("self", "fid"))
        .def("Serialize", &FragCatParams::Serialize, python::args("self"));
  };
};
}  // namespace RDKix

void wrap_fragparams() { RDKix::fragparams_wrapper::wrap(); }
