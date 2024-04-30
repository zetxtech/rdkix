//
//  Copyright (C) 2017-2023 Sereina Riniker and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef _RD_TORSIONPREFERENCES_H_
#define _RD_TORSIONPREFERENCES_H_
#include <vector>
#include <string>
#include <memory>

namespace RDKix {
class ROMol;
}  // namespace RDKix

namespace ForceFields {
namespace CrystalFF {

//! A structure used to the experimental torsion patterns
struct RDKIX_FORCEFIELDHELPERS_EXPORT ExpTorsionAngle {
  unsigned int torsionIdx;
  std::string smarts;
  std::vector<double> V;
  std::vector<int> signs;
  std::unique_ptr<const RDKix::ROMol> dp_pattern;
  unsigned int idx[4];
};

struct CrystalFFDetails {
  std::vector<std::vector<int>> expTorsionAtoms;
  std::vector<std::pair<std::vector<int>, std::vector<double>>>
      expTorsionAngles;
  std::vector<std::vector<int>> improperAtoms;
  std::vector<std::pair<int, int>> bonds;
  std::vector<std::vector<int>> angles;
  std::vector<int> atomNums;
  double boundsMatForceScaling;
};

//! Get the experimental torsional angles in a molecule
RDKIX_FORCEFIELDHELPERS_EXPORT void getExperimentalTorsions(
    const RDKix::ROMol &mol, CrystalFFDetails &details,
    bool useExpTorsions = false, bool useSmallRingTorsions = false,
    bool useMacrocycleTorsions = false, bool useBasicKnowledge = false,
    unsigned int version = 1, bool verbose = false);

//! \overload
RDKIX_FORCEFIELDHELPERS_EXPORT void getExperimentalTorsions(
    const RDKix::ROMol &mol, CrystalFFDetails &details,
    std::vector<std::tuple<unsigned int, std::vector<unsigned int>,
                           const ExpTorsionAngle *>> &torsionBonds,
    bool useExpTorsions = false, bool useSmallRingTorsions = false,
    bool useMacrocycleTorsions = false, bool useBasicKnowledge = false,
    unsigned int version = 1, bool verbose = false);

}  // namespace CrystalFF
}  // namespace ForceFields

#endif
