//
//  Copyright (C) 2015 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
//

// std bits
#include <RDGeneral/test.h>
#include <iostream>

// RD bits
#include <GraphMol/RDKixBase.h>
#include "GasteigerCharges.h"
#include "GasteigerParams.h"

#include <GraphMol/SmilesParse/SmilesParse.h>

using namespace RDKix;

void testGitHubIssue485() {
  BOOST_LOG(rdErrorLog) << "-------------------------------------" << std::endl;
  BOOST_LOG(rdErrorLog) << "    Test GitHub issue 485: Gasteiger charge "
                           "calculation fails with hexavalent sulfur"
                        << std::endl;

  {
    std::string smi = "CC.S(F)(F)(F)(F)(F)F";
    ROMol *mol = SmilesToMol(smi);
    std::vector<double> charges(mol->getNumAtoms(), 0);
    computeGasteigerCharges(*mol, charges, 12, true);
    TEST_ASSERT(charges[0] == charges[0]);  // test for nan
    TEST_ASSERT(charges[2] == charges[2]);  // test for nan

    delete mol;
  }
  {
    std::string smi = "CCS(F)(F)(F)(F)F";
    ROMol *mol = SmilesToMol(smi);
    std::vector<double> charges(mol->getNumAtoms(), 0);
    computeGasteigerCharges(*mol, charges, 12, true);
    TEST_ASSERT(charges[0] == charges[0]);  // test for nan
    TEST_ASSERT(charges[2] == charges[2]);  // test for nan

    delete mol;
  }
  BOOST_LOG(rdErrorLog) << "  done" << std::endl;
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  testGitHubIssue485();
  return 0;
}
