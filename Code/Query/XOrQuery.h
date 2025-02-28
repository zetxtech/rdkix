//
// Copyright (c) 2003-2020 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_XORQUERY_H
#define RD_XORQUERY_H

#include "Query.h"

namespace Queries {

//! a Query implementing XOR: requires exactly one child to be \c true
template <class MatchFuncArgType, class DataFuncArgType = MatchFuncArgType,
          bool needsConversion = false>
class RDKIX_QUERY_EXPORT XOrQuery
    : public Query<MatchFuncArgType, DataFuncArgType, needsConversion> {
 public:
  typedef Query<MatchFuncArgType, DataFuncArgType, needsConversion> BASE;
  XOrQuery() { this->df_negate = false; }

  bool Match(const DataFuncArgType what) const override {
    bool res = false;
    typename BASE::CHILD_VECT_CI it1;
    for (it1 = this->beginChildren(); it1 != this->endChildren(); ++it1) {
      bool tmp = (*it1)->Match(what);
      if (tmp) {
        if (res) {
          res = false;
          break;
        } else {
          res = true;
        }
      }
    }
    if (this->getNegation()) {
      res = !res;
    }
    return res;
  }

  Query<MatchFuncArgType, DataFuncArgType, needsConversion> *copy()
      const override {
    XOrQuery<MatchFuncArgType, DataFuncArgType, needsConversion> *res =
        new XOrQuery<MatchFuncArgType, DataFuncArgType, needsConversion>();

    typename BASE::CHILD_VECT_CI i;
    for (i = this->beginChildren(); i != this->endChildren(); ++i) {
      res->addChild(typename BASE::CHILD_TYPE(i->get()->copy()));
    }
    res->setNegation(this->getNegation());
    res->d_description = this->d_description;
    res->d_queryType = this->d_queryType;
    return res;
  }
};
}  // namespace Queries
#endif
