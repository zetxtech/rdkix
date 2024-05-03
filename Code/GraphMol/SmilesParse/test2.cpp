//
//  Copyright (C) 2018 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/test.h>
#include <iostream>
#include <string>
#include <GraphMol/RDKixBase.h>
#include "SmilesParse.h"
#include "SmilesWrite.h"
#include "SmartsWrite.h"
#include <RDGeneral/RDLog.h>
//#include <boost/log/functions.hpp>
using namespace RDKix;
using namespace std;

void testParseAtomSmiles() {
  BOOST_LOG(rdInfoLog) << "Testing ParseAtomSmiles" << std::endl;
  {  // these should pass
    std::vector<std::string> smiles = {"C", "[NH4+]", "[13C@H]"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Atom> a1(SmilesToAtom(pr));
      TEST_ASSERT(a1);
      TEST_ASSERT(a1->getAtomicNum() > 0);
    }
  }

  {  // these should fail
    std::vector<std::string> smiles = {"CO", "", "C-O", "-", "[Bg]"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Atom> a1(SmilesToAtom(pr));
      TEST_ASSERT(!a1);
    }
  }
  BOOST_LOG(rdInfoLog) << "done" << std::endl;
}

void testParseBondSmiles() {
  BOOST_LOG(rdInfoLog) << "Testing ParseBondSmiles" << std::endl;
  {  // these should pass
    std::vector<std::string> smiles = {":", "=", "#", "-", "/", "\\"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Bond> a1(SmilesToBond(pr));
      TEST_ASSERT(a1);
    }
  }

  {  // these should fail
    std::vector<std::string> smiles = {"C", "", "C-O", "*"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Bond> a1(SmilesToBond(pr));
      TEST_ASSERT(!a1);
    }
  }
  BOOST_LOG(rdInfoLog) << "done" << std::endl;
}

void testParseAtomSmarts() {
  BOOST_LOG(rdInfoLog) << "Testing ParseAtomSmarts" << std::endl;
  {  // these should pass
    std::vector<std::string> smiles = {"C", "[NH4+]", "[13C@H]",
                                       "[C,N,$(C=CC)]"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Atom> a1(SmartsToAtom(pr));
      TEST_ASSERT(a1);
    }
  }

  {  // these should fail
    std::vector<std::string> smiles = {"CO", "", "C-O", "-", "[Bg]"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Atom> a1(SmartsToAtom(pr));
      TEST_ASSERT(!a1);
    }
  }
  BOOST_LOG(rdInfoLog) << "done" << std::endl;
}

void testParseBondSmarts() {
  BOOST_LOG(rdInfoLog) << "Testing ParseBondSmarts" << std::endl;
  {  // these should pass
    std::vector<std::string> smiles = {":",  "=", "#",  "-",  "/",
                                       "\\", "@", "!-", "=,#"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Bond> a1(SmartsToBond(pr));
      TEST_ASSERT(a1);
    }
  }

  {  // these should fail
    std::vector<std::string> smiles = {"C", "", "-O", "*"};
    for (const auto &pr : smiles) {
      std::unique_ptr<Bond> a1(SmartsToBond(pr));
      TEST_ASSERT(!a1);
    }
  }
  BOOST_LOG(rdInfoLog) << "done" << std::endl;
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  RDLog::InitLogs();
// boost::logging::enable_logs("rdApp.debug");
#if 1
  testParseAtomSmiles();
  testParseBondSmiles();
  testParseAtomSmarts();
  testParseBondSmarts();
#endif
}
