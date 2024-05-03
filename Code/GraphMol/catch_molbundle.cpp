//
//  Copyright (C) 2023 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include "catch.hpp"

#include <GraphMol/RDKixBase.h>
#include <GraphMol/MolBundle.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <algorithm>

using namespace RDKix;

#ifdef RDK_USE_BOOST_SERIALIZATION

TEST_CASE("MolBundle serialization") {
  SECTION("basics") {
    MolBundle bundle;
    bundle.addMol(ROMOL_SPTR(SmilesToMol("CCC")));
    bundle.addMol(ROMOL_SPTR(SmilesToMol("CCN")));
    CHECK(!bundle.empty());
    auto pkl = bundle.serialize();
    MolBundle nbundle(pkl);
    REQUIRE(bundle.size() == nbundle.size());
    for (auto i = 0u; i < bundle.size(); ++i) {
      CHECK(MolToSmiles(*bundle[i]) == MolToSmiles(*nbundle[i]));
    }
  }
  SECTION("empty") {
    MolBundle bundle;
    CHECK(bundle.empty());
    auto pkl = bundle.serialize();
    MolBundle nbundle(pkl);
    REQUIRE(bundle.size() == nbundle.size());
  }
}
#endif