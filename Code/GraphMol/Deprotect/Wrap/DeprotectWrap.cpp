//
//  Copyright (C) 2020-2021 Brian P Kelley and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDBoost/python.h>
#include <RDBoost/Wrap.h>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/Deprotect/Deprotect.h>

namespace python = boost::python;
namespace RDKix {

boost::shared_ptr<ROMol> DeprotectWrap(const ROMol &mol,
                                       const python::object &iterable) {
  if (iterable != python::object()) {
    std::vector<Deprotect::DeprotectData> deprotections;
    pythonObjectToVect<Deprotect::DeprotectData>(iterable, deprotections);
    return Deprotect::deprotect(mol, deprotections);
  } else {
    return Deprotect::deprotect(mol, Deprotect::getDeprotections());
  }
}

bool DeprotectInPlaceWrap(ROMol &mol, const python::object &iterable) {
  RWMol &rwmol = static_cast<RWMol &>(mol);
  if (iterable != python::object()) {
    std::vector<Deprotect::DeprotectData> deprotections;
    pythonObjectToVect<Deprotect::DeprotectData>(iterable, deprotections);
    return Deprotect::deprotectInPlace(rwmol, deprotections);
  } else {
    return Deprotect::deprotectInPlace(rwmol, Deprotect::getDeprotections());
  }
}

//! Make a copy so we don't try and change a const vector
std::vector<Deprotect::DeprotectData> GetDeprotectionsWrap() {
  return Deprotect::getDeprotections();
}
}  // namespace RDKix

struct deprotect_wrap {
  static void wrap() {
    const char *constructor_doc =
        "Construct a new DeprotectData instance.\n"
        "  >>> reaction_class = \"amine\"\n"
        "  >>> reaction_smarts = "
        "\"[C;R0][C;R0]([C;R0])([O;R0][C;R0](=[O;R0])[NX3;H0,H1:1])C>>[N:1]\"\n"
        "  >>> abbreviation = \"Boc\"\n"
        "  >>> full_name = \"tert-butyloxycarbonyl\"\n"
        "  >>> data = DeprotectData(reaction_class, reaction_smarts, "
        "abbreviation, full_name)\n"
        "  >>> assert data.isValid()\n"
        "\n";

    const char *deprotect_doc_string =
        "DeprotectData class, contains a single deprotection reaction and "
        "information\n"
        "\n"
        " deprotectdata.deprotection_class - functional group being protected\n"
        " deprotectdata.reaction_smarts - reaction smarts used for "
        "deprotection\n"
        " deprotectdata.abbreviation - common abbreviation for the protecting "
        "group\n"
        " deprotectdata.full_name - full name for the protecting group\n"
        "\n"
        "\n";

    RegisterVectorConverter<RDKix::Deprotect::DeprotectData>(
        "DeprotectDataVect");

    python::class_<RDKix::Deprotect::DeprotectData>(
        "DeprotectData", deprotect_doc_string,
        python::init<std::string, std::string, std::string, std::string>(
            constructor_doc,
            python::args("deprotection_class", "reaction_smarts",
                         "abbreviation", "full_name")))
        .def_readonly("deprotection_class",
                      &RDKix::Deprotect::DeprotectData::deprotection_class)
        .def_readonly("full_name", &RDKix::Deprotect::DeprotectData::full_name)
        .def_readonly("abbreviation",
                      &RDKix::Deprotect::DeprotectData::abbreviation)
        .def_readonly("reaction_smarts",
                      &RDKix::Deprotect::DeprotectData::reaction_smarts)
        .def_readonly("example", &RDKix::Deprotect::DeprotectData::example)
        .def("isValid", &RDKix::Deprotect::DeprotectData::isValid,
             "Returns True if the DeprotectData has a valid reaction");

    python::def("GetDeprotections", &RDKix::GetDeprotectionsWrap,
                "Return the default list of deprotections");

    python::def(
        "Deprotect", &RDKix::DeprotectWrap,
        (python::arg("mol"), python::arg("deprotections") = python::object()),
        "Return the deprotected version of the molecule.");
    python::def(
        "DeprotectInPlace", &RDKix::DeprotectInPlaceWrap,
        (python::arg("mol"), python::arg("deprotections") = python::object()),
        "Deprotects the molecule in place.");
  }
};

void wrap_deprotect() { deprotect_wrap::wrap(); }
