//
//  Copyright (C) 2019-2022 NextMove Software and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.

#include <GraphMol/RDKixBase.h>
#include <GraphMol/RDKixQueries.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include "nmmolhash.h"

namespace RDKix {
namespace MolHash {
void Strip(RWMol *mol, unsigned int striptype) {
  // The order of these operations is significant to some degree
  // - Hydrogens should be at the end as that is the common
  //   use case

  if (striptype & static_cast<unsigned>(StripType::AtomStereo)) {
    for (auto aptr : mol->atoms()) {
      aptr->setChiralTag(RDKix::Atom::CHI_UNSPECIFIED);
    }
    if (!mol->getStereoGroups().empty()) {
      std::vector<StereoGroup> no_sgs;
      mol->setStereoGroups(std::move(no_sgs));
    }
  }
  if (striptype & static_cast<unsigned>(StripType::BondStereo)) {
    for (auto bptr : mol->bonds()) {
      if (bptr->getStereo() > RDKix::Bond::STEREOANY) {
        bptr->setStereo(RDKix::Bond::STEREOANY);
      }
    }
  }
  if (striptype & static_cast<unsigned>(StripType::Isotope)) {
    for (auto aptr : mol->atoms()) {
      aptr->setIsotope(0);
    }
  }
  if (striptype & static_cast<unsigned>(StripType::AtomMap)) {
    for (auto aptr : mol->atoms()) {
      aptr->setAtomMapNum(0);
    }
  }
  if (striptype & static_cast<unsigned>(StripType::Hydrogen)) {
    MolOps::removeHs(*mol);
  }
}

void SplitMolecule(RWMol *mol, std::vector<RWMol *> &molv) {
  auto mfrags = RDKix::MolOps::getMolFrags(*mol);
  for (const auto &frag : mfrags) {
    const auto *wrappedmol = frag.get();  // reach inside the shared pointer...
    molv.push_back(new RWMol(*wrappedmol));  // ...and make a copy
  }
}
}  // namespace MolHash
}  // namespace RDKix