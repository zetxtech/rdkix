//
//  Copyright (C) 2018 Dan Nealschneider
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#define NO_IMPORT_ARRAY
#include <boost/python.hpp>
#include <string>

// ours
#include <RDBoost/Wrap.h>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/StereoGroup.h>

namespace python = boost::python;

namespace RDKix {

namespace {
std::string stereoGroupClassDoc =
    "A collection of atoms with a defined stereochemical relationship.\n\n"
    "Used to help represent a sample with unknown stereochemistry, or that "
    "is a mix\nof diastereomers.\n";

StereoGroup *createStereoGroup(StereoGroupType typ, ROMol &mol,
                               python::object atomIds, unsigned readId) {
  std::vector<Atom *> cppAtoms;
  python::stl_input_iterator<unsigned int> beg(atomIds), end;
  while (beg != end) {
    unsigned int v = *beg;
    if (v >= mol.getNumAtoms()) {
      throw_value_error("atom index exceeds mol.GetNumAtoms()");
    }
    cppAtoms.push_back(mol.getAtomWithIdx(v));
    ++beg;
  }
  auto *sg = new StereoGroup(typ, cppAtoms, readId);
  return sg;
}

python::object getAtomsHelper(StereoGroup &sg) {
  python::list res;
  for (auto at : sg.getAtoms()) {
    res.append(boost::ref(*at));
  }
  return python::tuple(res);
}
}  // namespace

struct stereogroup_wrap {
  static void wrap() {
    python::enum_<RDKix::StereoGroupType>("StereoGroupType")
        .value("STEREO_ABSOLUTE", RDKix::StereoGroupType::STEREO_ABSOLUTE)
        .value("STEREO_OR", RDKix::StereoGroupType::STEREO_OR)
        .value("STEREO_AND", RDKix::StereoGroupType::STEREO_AND)
        .export_values();

    python::class_<StereoGroup, boost::shared_ptr<StereoGroup>>(
        "StereoGroup", stereoGroupClassDoc.c_str(), python::no_init)
        .def("GetGroupType", &StereoGroup::getGroupType,
             "Returns the StereoGroupType.\n")
        .def("GetAtoms", getAtomsHelper,
             "access the atoms in the StereoGroup.\n")
        .def("GetReadId", &StereoGroup::getReadId,
             "return the StereoGroup's original ID.\n"
             "Note that the ID only makes sense for AND/OR groups.\n")
        .def("GetWriteId", &StereoGroup::getWriteId,
             "return the StereoGroup's ID that will be exported.\n"
             "Note that the ID only makes sense for AND/OR groups.\n")
        .def("SetWriteId", &StereoGroup::setWriteId,
             "return the StereoGroup's ID that will be exported.\n"
             "Note that the ID only makes sense for AND/OR groups.\n");

    python::def("CreateStereoGroup", &createStereoGroup,
                "creates a StereoGroup associated with a molecule from a list "
                "of atom Ids",
                (python::arg("stereoGroupType"), python::arg("mol"),
                 python::arg("atomIds"), python::arg("readId") = 0),
                python::return_value_policy<
                    python::manage_new_object,
                    python::with_custodian_and_ward_postcall<0, 2>>());

    python::def(
        "ForwardStereoGroupIds", &RDKix::forwardStereoGroupIds,
        "Forward the original Stereo Group IDs when exporting the Mol.");
  }
};
}  // namespace RDKix

void wrap_stereogroup() { RDKix::stereogroup_wrap::wrap(); }
