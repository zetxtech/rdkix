//
//  Copyright (C) 2004-2006 Rational Discovery LLC
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef __RD_DISCRETEDISTMAT_H__
#define __RD_DISCRETEDISTMAT_H__
#include "DiscreteValueVect.h"

namespace RDKix {
class RDKIX_DATASTRUCTS_EXPORT DiscreteDistMat {
 public:
  DiscreteDistMat();
  ~DiscreteDistMat() = default;
  unsigned int getDist(unsigned char v1, unsigned char v2,
                       DiscreteValueVect::DiscreteValueType type);

 private:
  unsigned int d_oneBitTab[256 * 256];
  unsigned int d_twoBitTab[256 * 256];
  unsigned int d_fourBitTab[256 * 256];
};
extern DiscreteDistMat *getDiscreteDistMat();
}  // namespace RDKix
#endif
