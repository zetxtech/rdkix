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
#ifndef RD_LESSEQUALQUERY_H
#define RD_LESSEQUALQUERY_H
#include "Query.h"
#include "EqualityQuery.h"

namespace Queries {
//! \brief a Query implementing <= using a particular
//!  value (and an optional tolerance)
template <class MatchFuncArgType, class DataFuncArgType = MatchFuncArgType,
          bool needsConversion = false>
class RDKIX_QUERY_EXPORT LessEqualQuery
    : public EqualityQuery<MatchFuncArgType, DataFuncArgType, needsConversion> {
 public:
  LessEqualQuery() { this->d_tol = 0; }
  //! constructs with our target value
  explicit LessEqualQuery(DataFuncArgType what) {
    this->d_val = what;
    this->d_tol = 0;
    this->df_negate = false;
  }
  //! constructs with our target value and a tolerance
  LessEqualQuery(DataFuncArgType v, DataFuncArgType t) {
    this->d_val = v;
    this->d_tol = t;
    this->df_negate = false;
  }

  bool Match(const DataFuncArgType what) const override {
    MatchFuncArgType mfArg =
        this->TypeConvert(what, Int2Type<needsConversion>());
    if (queryCmp(this->d_val, mfArg, this->d_tol) <= 0) {
      return !this->getNegation();
    } else {
      return this->getNegation();
    }
  }

  Query<MatchFuncArgType, DataFuncArgType, needsConversion> *copy()
      const override {
    LessEqualQuery<MatchFuncArgType, DataFuncArgType, needsConversion> *res =
        new LessEqualQuery<MatchFuncArgType, DataFuncArgType,
                           needsConversion>();
    res->setNegation(this->getNegation());
    res->setVal(this->d_val);
    res->setTol(this->d_tol);
    res->setDataFunc(this->d_dataFunc);
    res->d_description = this->d_description;
    res->d_queryType = this->d_queryType;
    return res;
  }

  std::string getFullDescription() const override {
    std::ostringstream res;
    res << this->getDescription();
    res << " " << this->d_val;
    if (this->getNegation()) {
      res << " ! <= ";
    } else {
      res << " <= ";
    }
    return res.str();
  }
};
}  // namespace Queries
#endif
