//
//  Copyright (C) 2016 Novartis Institutes for BioMedical Research
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#pragma once
#include "StructChecker.h"

namespace RDKix {
namespace StructureCheck {

class RDKIX_STRUCTCHECKER_EXPORT StructCheckTautomer {
  RWMol &Mol;
  const StructCheckerOptions &Options;

 public:
  StructCheckTautomer(RWMol &mol, const StructCheckerOptions &options)
      : Mol(mol), Options(options) {}
  bool applyTautomer(unsigned it);
};
};  // namespace StructureCheck
}  // namespace RDKix
