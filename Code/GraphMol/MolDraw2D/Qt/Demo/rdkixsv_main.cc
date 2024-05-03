//
// file rdkixsv_main.cc
// David Cosgrove
// AstraZeneca
// 20th June 2014
//
// This is the main function for the program rdkixsv, a simple program
// demonstrating the use of the DrawMol2D classes for drawing into a Qt
// widget.

#include "RDKixSVMainWindow.H"

#include <RDGeneral/versions.h>

#include <iostream>

#include <QApplication>

using namespace std;

// ****************************************************************************

int main(int argc, char **argv) {
  QApplication a(argc, argv);
  cout << "Built with Qt version " << QT_VERSION_STR << endl
       << "Running with Qt version " << qVersion() << endl
       << "Using RDKix version " << RDKix::rdkixVersion << endl
       << endl;

  RDKixSV::RDKixSVMainWindow *mw = new RDKixSV::RDKixSVMainWindow(argc, argv);
  mw->setWindowTitle(QObject::tr("RDKix SV"));
  mw->setGeometry(0, 0, 1000, 1000);
  mw->show();

  return a.exec();
}
