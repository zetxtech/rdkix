/*
 *  Copyright (c) 2019 Greg Landrum
 *  All rights reserved.
 *
 *  This file is part of the RDKix.
 *  The contents are covered by the terms of the BSD license
 *  which is included in the file license.txt, found at the root
 *  of the RDKix source tree.
 */

%include "std_string.i"

%{
#include <GraphMol/MolHash/MolHash.h>
%}

%ignore RDKix::MolHash::generateMoleculeHashCode;
%ignore RDKix::MolHash::CodeFlags;
%ignore RDKix::MolHash::fillAtomBondCodes;
%ignore RDKix::MolHash::HashSet;
%ignore RDKix::MolHash::generateMoleculeHashSet;
%ignore RDKix::MolHash::encode;

%include<GraphMol/MolHash/nmmolhash.h>

