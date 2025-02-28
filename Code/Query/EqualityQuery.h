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
#ifndef RD_EQUALITYQUERY_H
#define RD_EQUALITYQUERY_H
#include "Query.h"
#include <sstream>

namespace Queries {

//! \brief a Query implementing ==: arguments must match a particular
//!  value (within an optional tolerance)
template <typename MatchFuncArgType,
          typename DataFuncArgType = MatchFuncArgType,
          bool needsConversion = false>
class RDKIX_QUERY_EXPORT EqualityQuery
    : public Query<MatchFuncArgType, DataFuncArgType, needsConversion> {
 public:
  EqualityQuery() { this->df_negate = false; }

  //! constructs with our target value
  explicit EqualityQuery(MatchFuncArgType v) {
    this->d_val = v;
    this->df_negate = false;
  }

  //! constructs with our target value and a tolerance
  EqualityQuery(MatchFuncArgType v, MatchFuncArgType t) {
    this->d_val = v;
    this->d_tol = t;
    this->df_negate = false;
  }

  //! sets our target value
  void setVal(MatchFuncArgType what) { this->d_val = what; }
  //! returns our target value
  const MatchFuncArgType getVal() const { return this->d_val; }

  //! sets our tolerance
  void setTol(MatchFuncArgType what) { this->d_tol = what; }
  //! returns out tolerance
  const MatchFuncArgType getTol() const { return this->d_tol; }

  bool Match(const DataFuncArgType what) const override {
    MatchFuncArgType mfArg =
        this->TypeConvert(what, Int2Type<needsConversion>());
    if (queryCmp(this->d_val, mfArg, this->d_tol) == 0) {
      return !this->getNegation();
    } else {
      return this->getNegation();
    }
  }

  Query<MatchFuncArgType, DataFuncArgType, needsConversion> *copy()
      const override {
    EqualityQuery<MatchFuncArgType, DataFuncArgType, needsConversion> *res =
        new EqualityQuery<MatchFuncArgType, DataFuncArgType, needsConversion>();
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
      res << " != ";
    } else {
      res << " = ";
    }
    res << "val";
    return res.str();
  }
};
}  // namespace Queries
#endif
