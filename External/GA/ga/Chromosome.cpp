//
//  Copyright (C) 2020 Gareth Jones, Glysade LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include "Chromosome.h"

namespace GapeGa {

int Chromosome::idCounter = 0;

Chromosome::Chromosome() : chromosomeId(idCounter++), fitness(0) {}

Chromosome::~Chromosome() {}

}  // namespace GapeGa
