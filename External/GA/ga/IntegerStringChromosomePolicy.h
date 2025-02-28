//
//  Copyright (C) 2020 Gareth Jones, Glysade LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#ifndef INTEGERSTRINGCHROMOSOMEPOLICY_H_
#define INTEGERSTRINGCHROMOSOMEPOLICY_H_

#include "../util/RandomUtil.h"
#include "../util/export.h"
#include "StringChromosomeBase.h"

namespace GapeGa {

class GA_EXPORT IntegerStringChromosomePolicy {
 public:
  IntegerStringChromosomePolicy(GarethUtil::RandomUtil& rng_, int s);
  virtual ~IntegerStringChromosomePolicy();

  int mutate(int pos, int currentValue) const;
  int initialize(int pos) const;
  bool isAllowSwitch() { return false; }

  void setMax(int max);
  void setMax(int pos, int max);
  void setAllowNulls(bool allow);
  void setAllowNulls(int pos, bool allow);

  int getSize() const { return size; }

 private:
  GarethUtil::RandomUtil& rng;
  const int size;
  IntegerStringChromosomePolicy(const IntegerStringChromosomePolicy& orig) =
      delete;
  IntegerStringChromosomePolicy& operator=(
      const IntegerStringChromosomePolicy& other) = delete;

  int* const maxs;
  bool* const allowNulls;
};

}  // namespace GapeGa

#endif /* INTEGERSTRINGCHROMOSOMEPOLICY_H_ */
