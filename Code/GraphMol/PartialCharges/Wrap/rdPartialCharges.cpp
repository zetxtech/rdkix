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
#include <GraphMol/GraphMol.h>
#include <RDBoost/Wrap.h>

#include <GraphMol/PartialCharges/GasteigerCharges.h>

namespace python = boost::python;

namespace RDKix {
void ComputeGasteigerCharges(const ROMol &mol, int nIter,
                             bool throwOnParamFailure) {
  computeGasteigerCharges(&mol, nIter, throwOnParamFailure);
}
}  // namespace RDKix

BOOST_PYTHON_MODULE(rdPartialCharges) {
  python::scope().attr("__doc__") =
      "Module containing functions to set partial charges - currently "
      "Gasteiger Charges";

  std::string docString =
      "Compute Gasteiger partial charges for molecule\n\n\
 The charges are computed using an iterative procedure presented in \n\
 \n\
 Ref : J.Gasteiger, M. Marseli, Iterative Equalization of Oribital Electronegatiity \n\
 A Rapid Access to Atomic Charges, Tetrahedron Vol 36 p3219 1980\n\
 \n\
 The computed charges are stored on each atom are stored a computed property ( under the name \n\
 _GasteigerCharge). In addition, each atom also stored the total charge for the implicit hydrogens \n\
 on the atom (under the property name _GasteigerHCharge)\n\
 \n\
 ARGUMENTS:\n\n\
    - mol : the molecule of interrest\n\
    - nIter : number of iteration (defaults to 12)\n\
    - throwOnParamFailure : toggles whether or not an exception should be raised if parameters\n\
      for an atom cannot be found.  If this is false (the default), all parameters for unknown\n\
      atoms will be set to zero.  This has the effect of removing that atom from the iteration.\n\n";
  python::def("ComputeGasteigerCharges", RDKix::ComputeGasteigerCharges,
              (python::arg("mol"), python::arg("nIter") = 12,
               python::arg("throwOnParamFailure") = false),
              docString.c_str());
}
