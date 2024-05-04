/*
*  Copyright (C) 2016 Sereina Riniker, Paolo Tosco
* 
*    @@ All Rights Reserved @@
*   This file is part of the RDKix.
*   The contents are covered by the terms of the BSD license
*   which is included in the file license.txt, found at the root
*   of the RDKix source tree.
*/

%{
#include <GraphMol/Trajectory/Snapshot.h>
#include <GraphMol/Trajectory/Trajectory.h>
%}
%include <GraphMol/Trajectory/Snapshot.h>
%include <GraphMol/Trajectory/Trajectory.h>

%extend RDKix::Snapshot {
  Snapshot(std::vector<double> &posVect, double energy = 0.0) {
    double *posArray = new double[posVect.size()];
    for (unsigned int i = 0; i < posVect.size(); ++i)
      posArray[i] = posVect[i];
    boost::shared_array<double> pos(posArray);
    RDKix::Snapshot *s = new RDKix::Snapshot(pos, energy);
    return s;
  }
  Snapshot(const RDKix::Snapshot &other) {
    return new RDKix::Snapshot(other);
  }
  %newobject SnapshotVect;
  static std::vector<RDKix::Snapshot> *SnapshotVect() {
    return new std::vector<RDKix::Snapshot>();
  }
}
