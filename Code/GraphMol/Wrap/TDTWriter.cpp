//
//  Copyright (C) 2005-2021 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#define NO_IMPORT_ARRAY
#include <RDBoost/python.h>
#include <string>

// ours
#include <GraphMol/FileParsers/MolWriters.h>
#include <GraphMol/RDKixBase.h>
#include "rdchem.h"
#include "ContextManagers.h"
#include <RDBoost/PySequenceHolder.h>
#include <RDBoost/python_streambuf.h>

namespace python = boost::python;

namespace RDKix {
using boost_adaptbx::python::streambuf;
TDTWriter *getTDTWriter(python::object &fileobj) {
  // FIX: minor leak here
  auto *sb = new streambuf(fileobj, 't');
  auto *ost = new streambuf::ostream(*sb);
  return new TDTWriter(ost, true);
}

void SetTDTWriterProps(TDTWriter &writer, python::object props) {
  // convert the python list to a STR_VECT
  STR_VECT propNames;
  PySequenceHolder<std::string> seq(props);
  for (unsigned int i = 0; i < seq.size(); i++) {
    propNames.push_back(seq[i]);
  }
  writer.setProps(propNames);
}
struct tdtwriter_wrap {
  static void wrap() {
    std::string docStr =
        "Constructor.\n\n"
        "   If a string argument is provided, it will be treated as the name "
        "of the output file.\n"
        "   If a file-like object is provided, output will be sent there.\n\n";
    python::class_<TDTWriter, boost::noncopyable>(
        "TDTWriter", "A class for writing molecules to TDT files.\n",
        python::no_init)
        .def("__init__", python::make_constructor(&getTDTWriter))
        .def(
            python::init<std::string>(python::args("fileName"), docStr.c_str()))
        .def("__enter__", &MolIOEnter<TDTWriter>,
             python::return_internal_reference<>())
        .def("__exit__", &MolIOExit<TDTWriter>)
        .def("SetProps", SetTDTWriterProps,
             "Sets the properties to be written to the output file\n\n"
             "  ARGUMENTS:\n\n"
             "    - props: a list or tuple of property names\n\n")
        .def("write", &TDTWriter::write,
             (python::arg("self"), python::arg("mol"),
              python::arg("confId") = defaultConfId),
             "Writes a molecule to the output file.\n\n"
             "  ARGUMENTS:\n\n"
             "    - mol: the Mol to be written\n"
             "    - confId: (optional) ID of the conformation to write\n\n")
        .def("flush", &TDTWriter::flush,
             "Flushes the output file (forces the disk file to be "
             "updated).\n\n")
        .def("close", &TDTWriter::close,
             "Flushes the output file and closes it. The Writer cannot be used "
             "after this.\n\n")
        .def("NumMols", &TDTWriter::numMols,
             "Returns the number of molecules written so far.\n\n")
        .def("SetWrite2D", &TDTWriter::setWrite2D,
             (python::arg("self"), python::arg("state") = true),
             "causes 2D conformations to be written (default is 3D "
             "conformations)")
        .def("GetWrite2D", &TDTWriter::getWrite2D)
        .def("SetWriteNames", &TDTWriter::setWriteNames,
             (python::arg("self"), python::arg("state") = true),
             "causes names to be written to the output file as NAME records")
        .def("GetWriteNames", &TDTWriter::getWriteNames)
        .def("SetNumDigits", &TDTWriter::setNumDigits,
             "sets the number of digits to be written for coordinates")
        .def("GetNumDigits", &TDTWriter::getNumDigits);
  };
};
}  // namespace RDKix

void wrap_tdtwriter() { RDKix::tdtwriter_wrap::wrap(); }
