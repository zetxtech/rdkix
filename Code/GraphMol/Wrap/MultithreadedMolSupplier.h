//
//  Copyright (C) 2020 Shrey Aryan
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#ifndef RD_WRAP_MTMOLSUPPLIER_H
#define RD_WRAP_MTMOLSUPPLIER_H

#include <GraphMol/RDKixBase.h>
#include <RDBoost/python.h>
#include <RDBoost/python_streambuf.h>
#include <RDGeneral/FileParseException.h>
#include <RDGeneral/export.h>

namespace RDKix {
//! Note that this returns a pointer to the supplier itself, so be careful
//! that it doesn't get deleted by python!
template <typename T>
T *MTMolSupplIter(T *suppl) {
  return suppl;
}

template <typename T>
std::string MTMolSupplLastItem(T *supp) {
  return supp->getLastItemText();
}

template <typename T>
unsigned int MTMolSupplLastId(T *supp) {
  return supp->getLastRecordId();
}

}  // namespace RDKix
#endif
