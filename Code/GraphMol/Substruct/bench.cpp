//
//  Copyright (C) 2001,2003 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#ifdef WIN32
#pragma warning(disable : 4786)  // warning: long & complicated stl warning
#pragma warning(disable : 4788)  // warning: long & complicated stl warning
#endif

// std bits
#include <iostream>

// RD bits
#include <GraphMol/RDKixBase.h>
#include <GraphMol/RDKixQueries.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include "SubstructMatch.h"
#include "SubstructUtils.h"
#include <cstdlib>

using namespace RDKix;

void test1(int stopAfter = 1000) {
  std::cout << " ----------------- Test 1" << std::endl;
  MatchVectType matchV;
  std::vector<MatchVectType> matches;
  std::string fName = getenv("RDBASE");
  fName += "/Data/NCI/first_5K.smi";
  int n;
  SmilesMolSupplier suppl(fName);
  RWMol *q = SmartsToMol("C1[C;X4][D3]1");
  TEST_ASSERT(q);

  for (int i = 0; i < stopAfter; i++) {
    ROMol *mol = suppl[i];
    if (mol) {
      n = SubstructMatch(mol, q, matches, true);
    }
  }
  std::cout << "Done\n" << std::endl;
}

int main(int argc, char *argv[]) {
  test1();
  return 0;
}
