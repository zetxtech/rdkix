//
//  Copyright (C) 2012-2016 Greg Landrum
//   @@ All Rights Reserved @@
//
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDGeneral/test.h>
#include <RDGeneral/Invariant.h>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <RDGeneral/RDLog.h>
#include <vector>
#include <algorithm>
#include <fstream>

#include <GraphMol/Descriptors/PBF.h>

void test1() {
  BOOST_LOG(rdErrorLog) << "-------------------------------------" << std::endl;
  BOOST_LOG(rdErrorLog) << "    Basic PBF tests." << std::endl;

  std::string pathName = getenv("RDBASE");
  std::string sdfName =
      pathName + "/Code/GraphMol/Descriptors/test_data/PBF_egfr.sdf";
  RDKix::SDMolSupplier reader(sdfName, true, false);
  std::string fName =
      pathName + "/Code/GraphMol/Descriptors/test_data/PBF_egfr.out";
  std::ifstream instrm(fName.c_str());
  int nDone = 0;
  while (!reader.atEnd()) {
    RDKix::ROMol *m = reader.next();
    TEST_ASSERT(m);
    std::string nm;
    m->getProp("_Name", nm);
    double dpbf = RDKix::Descriptors::PBF(*m);

    std::string inm;
    double ref;
    instrm >> inm;
    instrm >> ref;
    TEST_ASSERT(inm == nm);
    if (fabs(ref - dpbf) > .001) {
      std::cerr << "value mismatch: " << inm << " " << ref << " " << dpbf
                << std::endl;
    }
    TEST_ASSERT(fabs(ref - dpbf) < 0.001);
    delete m;
    ++nDone;
  }
  BOOST_LOG(rdErrorLog) << "  done" << std::endl;
}

void testPBFEdges() {
  BOOST_LOG(rdErrorLog) << "-------------------------------------" << std::endl;
  BOOST_LOG(rdErrorLog) << "    PBF edge cases." << std::endl;

  {
    std::string pathName = getenv("RDBASE");
    std::string sdfName =
        pathName + "/Code/GraphMol/Descriptors/test_data/linear.mol";

    RDKix::ROMol *m = RDKix::MolFileToMol(sdfName);
    TEST_ASSERT(m);
    double dpbf = RDKix::Descriptors::PBF(*m);
    TEST_ASSERT(dpbf <= 1e-4);
    delete m;
  }
  {
    std::string pathName = getenv("RDBASE");
    std::string sdfName =
        pathName + "/Code/GraphMol/Descriptors/test_data/linear_2atom.mol";

    RDKix::ROMol *m = RDKix::MolFileToMol(sdfName);
    TEST_ASSERT(m);
    double dpbf = RDKix::Descriptors::PBF(*m);
    TEST_ASSERT(dpbf <= 1e-4);
    delete m;
  }
  {
    std::string pathName = getenv("RDBASE");
    std::string sdfName =
        pathName + "/Code/GraphMol/Descriptors/test_data/planar.mol";

    RDKix::ROMol *m = RDKix::MolFileToMol(sdfName);
    TEST_ASSERT(m);
    double dpbf = RDKix::Descriptors::PBF(*m);
    TEST_ASSERT(dpbf <= 1e-4);
    delete m;
  }
  {
    std::string pathName = getenv("RDBASE");
    std::string sdfName =
        pathName + "/Code/GraphMol/Descriptors/test_data/planar_3atom.mol";

    RDKix::ROMol *m = RDKix::MolFileToMol(sdfName);
    TEST_ASSERT(m);
    double dpbf = RDKix::Descriptors::PBF(*m);
    TEST_ASSERT(dpbf <= 1e-4);
    delete m;
  }
  {
    RDKix::RWMol m;
    bool updateLabel = true;
    bool takeOwnership = true;
    m.addAtom(new RDKix::Atom(6), updateLabel, takeOwnership);
    m.addAtom(new RDKix::Atom(6), updateLabel, takeOwnership);
    m.addAtom(new RDKix::Atom(6), updateLabel, takeOwnership);
    m.addAtom(new RDKix::Atom(6), updateLabel, takeOwnership);
    m.addAtom(new RDKix::Atom(6), updateLabel, takeOwnership);
    m.addAtom(new RDKix::Atom(6), updateLabel, takeOwnership);
    m.addConformer(new RDKix::Conformer(m.getNumAtoms()));
    double dpbf = RDKix::Descriptors::PBF(m);
    TEST_ASSERT(dpbf <= 1e-4);
  }

  BOOST_LOG(rdErrorLog) << "  done" << std::endl;
}

int main() {
  RDLog::InitLogs();
  test1();
  testPBFEdges();
}
