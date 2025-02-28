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
#include <RDGeneral/test.h>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/RDKixQueries.h>
#include <RDGeneral/RDLog.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>

#include "FragCatGenerator.h"

#include "FragCatalogEntry.h"
#include "FragCatParams.h"
#include "FragCatalogUtils.h"
#include "FragFPGenerator.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <GraphMol/Subgraphs/SubgraphUtils.h>
#include <GraphMol/Subgraphs/Subgraphs.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <RDGeneral/types.h>
#include <RDGeneral/FileParseException.h>
#include <DataStructs/BitVects.h>

using namespace RDKix;

void testMols(std::vector<std::unique_ptr<ROMol>> &mols, FragFPGenerator &fpGen,
              FragCatalog &fcat) {
  int nDone = 0;
  for (auto &&mi : mols) {
    ExplicitBitVect *fp = fpGen.getFPForMol(*mi, fcat);
    switch (nDone) {
      case 0:
        TEST_ASSERT(fp->getNumOnBits() == 3);
        TEST_ASSERT((*fp)[0]);
        TEST_ASSERT((*fp)[1]);
        TEST_ASSERT((*fp)[2]);
        break;
      case 1:
        TEST_ASSERT(fp->getNumOnBits() == 2);
        TEST_ASSERT((*fp)[1]);
        TEST_ASSERT((*fp)[3]);
        break;
      case 2:
        TEST_ASSERT(fp->getNumOnBits() == 3);
        TEST_ASSERT((*fp)[1]);
        TEST_ASSERT((*fp)[4]);
        TEST_ASSERT((*fp)[5]);
        break;
      case 3:
        TEST_ASSERT(fp->getNumOnBits() == 3);
        TEST_ASSERT((*fp)[1]);
        TEST_ASSERT((*fp)[6]);
        TEST_ASSERT((*fp)[7]);
        break;
      case 4:
        TEST_ASSERT(fp->getNumOnBits() == 2);
        TEST_ASSERT((*fp)[0]);
        TEST_ASSERT((*fp)[8]);
        break;
      case 5:
        TEST_ASSERT(fp->getNumOnBits() == 3);
        TEST_ASSERT((*fp)[0]);
        TEST_ASSERT((*fp)[6]);
        TEST_ASSERT((*fp)[9]);
        break;
      case 6:
        TEST_ASSERT(fp->getNumOnBits() == 5);
        TEST_ASSERT((*fp)[0]);
        TEST_ASSERT((*fp)[1]);
        TEST_ASSERT((*fp)[2]);
        TEST_ASSERT((*fp)[3]);
        TEST_ASSERT((*fp)[10]);
        break;
    }
    delete fp;
    nDone += 1;
  }
}

void test1() {
  std::string rdbase = getenv("RDBASE");
  std::string fname = rdbase + "/Code/GraphMol/FragCatalog/test_data/mols.smi";
  std::string fgrpFile =
      rdbase + "/Code/GraphMol/FragCatalog/test_data/funcGroups.txt";
  SmilesMolSupplier suppl(fname, " ", 0, 1, false);

  auto *fparams = new FragCatParams(1, 6, fgrpFile, 1.0e-8);
  TEST_ASSERT(fparams->getNumFuncGroups() == 15);
  FragCatalog fcat(fparams);
  FragCatGenerator catGen;

  std::vector<std::unique_ptr<ROMol>> mols;
  unsigned int nDone = 0;
  ROMol *m = suppl.next();
  while (m) {
    mols.push_back(std::unique_ptr<ROMol>(m));
    nDone += 1;
    catGen.addFragsFromMol(*m, &fcat);
    try {
      m = suppl.next();
    } catch (FileParseException &) {
      m = nullptr;
    }
  }
  TEST_ASSERT(mols.size() == 16);
  TEST_ASSERT(nDone == 16);
  int nents = fcat.getNumEntries();
  std::cerr << " " << nents << std::endl;
  TEST_ASSERT(nents == 21);
  FragFPGenerator fpGen;

  BOOST_LOG(rdInfoLog) << "----- Test 1" << std::endl;
  testMols(mols, fpGen, fcat);
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;

  //----------------------------------------------------------
  //  SERIALIZATION TESTS
  //----------------------------------------------------------

  // make sure we can pickle and unpickle parameter objects:
  FragCatParams newParams;
  std::string pickle = fparams->Serialize();
  newParams.initFromString(pickle);
  TEST_ASSERT(newParams.getLowerFragLength() == fparams->getLowerFragLength());
  TEST_ASSERT(newParams.getUpperFragLength() == fparams->getUpperFragLength());
  TEST_ASSERT(newParams.getTolerance() == fparams->getTolerance());
  TEST_ASSERT(newParams.getNumFuncGroups() == fparams->getNumFuncGroups());

  // make sure we can pickle and unpickle catalog entries:
  const FragCatalogEntry *fpEntry = fcat.getEntryWithIdx(0);
  auto *fpEntry2 = new FragCatalogEntry();
  fpEntry2->initFromString(fpEntry->Serialize());
  TEST_ASSERT(fpEntry->getDescription() == fpEntry2->getDescription());
  TEST_ASSERT(fpEntry->getOrder() == fpEntry2->getOrder());
  TEST_ASSERT(fpEntry->getBitId() == fpEntry2->getBitId());
  TEST_ASSERT(fpEntry2->match(fpEntry, 1e-8));
  TEST_ASSERT(fpEntry->match(fpEntry2, 1e-8));
  delete fpEntry2;

  // test catalogs' initFromString:
  FragCatalog fcat2;
  fcat2.initFromString(fcat.Serialize());
  TEST_ASSERT(fcat2.getNumEntries() == fcat.getNumEntries());
  BOOST_LOG(rdInfoLog) << "----- Test 2" << std::endl;
  testMols(mols, fpGen, fcat2);
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;

  // and the pickle ctor:
  FragCatalog *fcat3 = new FragCatalog(fcat.Serialize());
  TEST_ASSERT(fcat3->getNumEntries() == fcat.getNumEntries());
  BOOST_LOG(rdInfoLog) << "----- Test 3" << std::endl;
  testMols(mols, fpGen, *fcat3);
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;

  //----------------------------------------------------------
  // test issue 115
  //----------------------------------------------------------
  BOOST_LOG(rdInfoLog) << "----- Test Issue 115" << std::endl;
  delete fparams;
  fparams = new FragCatParams(3, 3, fgrpFile, 1.0e-8);
  TEST_ASSERT(fparams->getNumFuncGroups() == 15);
  delete fcat3;
  fcat3 = new FragCatalog(fparams);
  suppl.reset();
  nDone = 0;
  while (!suppl.atEnd()) {
    nDone++;
    ROMol *m = suppl.next();
    catGen.addFragsFromMol(*m, fcat3);
    delete m;
  }
  TEST_ASSERT(nDone == suppl.length());
  TEST_ASSERT(fcat3->getNumEntries() == 21);
  TEST_ASSERT(fcat3->getFPLength() == 10);
  for (unsigned int i = 0; i < fcat3->getFPLength(); i++) {
    TEST_ASSERT(fcat3->getEntryWithBitId(i)->getOrder() == 3);
  }
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;

  //----------------------------------------------------------
  // test issue 117
  //----------------------------------------------------------
  BOOST_LOG(rdInfoLog) << "----- Test Issue 117" << std::endl;
  delete fparams;
  fparams = new FragCatParams(1, 2, fgrpFile, 1.0e-8);
  delete fcat3;
  fcat3 = new FragCatalog(fparams);
  ROMol *tmpMol = SmilesToMol("OC(N)CO");
  TEST_ASSERT(tmpMol);
  catGen.addFragsFromMol(*tmpMol, fcat3);
  TEST_ASSERT(fcat3->getNumEntries() == 1);
  // std::cout << fcat3->getEntryWithBitId(0)->getDescription() << std::endl;
  TEST_ASSERT(fcat3->getEntryWithBitId(0)->getDescription() ==
              "C(<-O>)<-N>C<-O>");
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;

  delete tmpMol;
  delete fparams;
  delete fcat3;
}

void testIssue294() {
  BOOST_LOG(rdInfoLog) << "---- Test Issue294\n  Watch memory usage."
                       << std::endl;
  std::string rdbase = getenv("RDBASE");
  std::string fname = rdbase + "/Code/GraphMol/FragCatalog/test_data/mols.smi";
  std::string fgrpFile =
      rdbase + "/Code/GraphMol/FragCatalog/test_data/funcGroups.txt";
  SmilesMolSupplier suppl(fname, " ", 0, 1, false);

  auto *fparams = new FragCatParams(1, 6, fgrpFile, 1.0e-8);
  FragCatalog fcat(fparams);
  FragCatGenerator catGen;

  std::vector<std::unique_ptr<ROMol>> mols;
  ROMol *m = suppl.next();
  while (m) {
    mols.push_back(std::unique_ptr<ROMol>(m));
    catGen.addFragsFromMol(*m, &fcat);
    try {
      m = suppl.next();
    } catch (FileParseException &) {
      m = nullptr;
    }
  }
  int nents = fcat.getNumEntries();
  TEST_ASSERT(nents == 21);
  FragFPGenerator fpGen;

  for (unsigned int i = 0; i < 200; i++) {
    for (auto &&mi : mols) {
      ExplicitBitVect *fp = fpGen.getFPForMol(*mi, fcat);
      delete fp;
    }
  }
  delete fparams;
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;
}

void testWhiteSpaceInSmarts() {
  BOOST_LOG(rdInfoLog) << "----- Test Whitespace in SMARTS Fragment string."
                       << std::endl;

  std::vector<std::string> data(
      {"          ",    // whitespace only
       "          \n",  // whitespace plus new line
       " //   initial space plus comment\nfluorine\t[F]\n"});

  std::vector<size_t> reference_sizes({0, 0, 1});

  auto reference = reference_sizes.cbegin();
  for (const auto &smarts : data) {
    std::istringstream input(smarts);
    auto groups = readFuncGroups(input);
    TEST_ASSERT(groups.size() == *reference);
    ++reference;
  }
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;
}

void testFragmentWithoutSmarts() {
  BOOST_LOG(rdInfoLog) << "----- Test Fragment string without SMARTS."
                       << std::endl;

  std::vector<std::string> data(
      {"//   Name	SMARTS\nnonsense\n",
       "//   Name	SMARTS\nnonsense no new line",
       "//   Name	SMARTS\nnonsense with tab\t\n"});

  for (const auto &smarts : data) {
    bool ok = false;
    std::istringstream input(smarts);
    RDKix::MOL_SPTR_VECT groups;
    try {
      groups = readFuncGroups(input);
    } catch (const Invar::Invariant &) {
      ok = true;
    }
    TEST_ASSERT(ok);
    TEST_ASSERT(groups.empty());
  }
  BOOST_LOG(rdInfoLog) << "---- Done" << std::endl;
}

int main() {
  RDLog::InitLogs();
  test1();
  testIssue294();
  testWhiteSpaceInSmarts();
  testFragmentWithoutSmarts();
  return 0;
}
