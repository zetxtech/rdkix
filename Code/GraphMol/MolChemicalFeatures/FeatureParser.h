//
//  Copyright (C) 2004-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef __FEATUREPARSER_H_02122004_1810__
#define __FEATUREPARSER_H_02122004_1810__

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include "MolChemicalFeatureDef.h"

namespace RDKix {
//! \brief class used to indicate errors in parsing feature definition
//!   files.
class RDKIX_MOLCHEMICALFEATURES_EXPORT FeatureFileParseException
    : public std::exception {
 public:
  FeatureFileParseException(unsigned int lineNo, std::string line,
                            std::string msg)
      : d_lineNo(lineNo), d_line(std::move(line)), d_msg(std::move(msg)) {}
  unsigned int lineNo() const { return d_lineNo; }
  std::string line() const { return d_line; }
  const char *what() const noexcept override { return d_msg.c_str(); }
  ~FeatureFileParseException() noexcept override = default;

 private:
  unsigned int d_lineNo;
  std::string d_line, d_msg;
};

RDKIX_MOLCHEMICALFEATURES_EXPORT int parseFeatureData(
    const std::string &defnText,
    MolChemicalFeatureDef::CollectionType &featDefs);
RDKIX_MOLCHEMICALFEATURES_EXPORT int parseFeatureData(
    std::istream &istream, MolChemicalFeatureDef::CollectionType &featDefs);
RDKIX_MOLCHEMICALFEATURES_EXPORT int parseFeatureFile(
    const std::string &fileName,
    MolChemicalFeatureDef::CollectionType &featDefs);

namespace Local {
// these functions are exposed only so they can be tested
RDKIX_MOLCHEMICALFEATURES_EXPORT void parseAtomType(
    const std::string &inLine, std::map<std::string, std::string> &atomTypeDefs,
    const unsigned int &lineNo);
RDKIX_MOLCHEMICALFEATURES_EXPORT MolChemicalFeatureDef *parseFeatureDef(
    std::istream &inStream, const std::string &inLine, unsigned int &lineNo,
    const std::map<std::string, std::string> &atomTypeDefs);
}  // namespace Local
}  // end of namespace RDKix
#endif
