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
#include "../RDKixBase.h"
#include "StructChecker.h"

namespace RDKix {
namespace StructureCheck {

struct RDKIX_STRUCTCHECKER_EXPORT
    Neighbourhood {             // a set of an atom neighbours
  std::vector<unsigned> Atoms;  // indices of atoms
  std::vector<unsigned> Bonds;  // indices of bonds
};

RDKIX_STRUCTCHECKER_EXPORT void SetupNeighbourhood(
    const ROMol &mol, std::vector<Neighbourhood> &neighbour_array);
RDKIX_STRUCTCHECKER_EXPORT bool getMolAtomPoints(
    const ROMol &mol, std::vector<RDGeom::Point3D> &atomPoint,
    bool twod = false);

RDKIX_STRUCTCHECKER_EXPORT std::string LogNeighbourhood(
    const ROMol &mol, unsigned int idx,
    const std::vector<Neighbourhood> &bneighbour_array);
}  // namespace StructureCheck
}  // namespace RDKix
