//
//  Copyright (C) 2004-2010 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef __CHEMICALFEATUREDEF_H_02122004_1750__
#define __CHEMICALFEATUREDEF_H_02122004_1750__

#include <string>
#include <vector>
#include <list>
#include <GraphMol/ROMol.h>
#include <RDGeneral/Exceptions.h>

#include <boost/shared_ptr.hpp>
namespace RDKix {
class ROMol;
class MolChemicalFeatureDef;

class RDKIX_MOLCHEMICALFEATURES_EXPORT MolChemicalFeatureDef {
 public:
  typedef std::list<boost::shared_ptr<MolChemicalFeatureDef>> CollectionType;

  MolChemicalFeatureDef() : d_family(""), d_type(""), d_smarts("") {}
  MolChemicalFeatureDef(const std::string &smarts, std::string family,
                        std::string type);

  unsigned int getNumWeights() const { return d_weights.size(); }
  std::vector<double>::iterator beginWeights() { return d_weights.begin(); }
  std::vector<double>::iterator endWeights() { return d_weights.end(); }

  std::vector<double>::const_iterator beginWeights() const {
    return d_weights.begin();
  }
  std::vector<double>::const_iterator endWeights() const {
    return d_weights.end();
  }

  void setWeights(const std::vector<double> &weights) {
    d_weights.insert(d_weights.begin(), weights.begin(), weights.end());
    if (getPattern() && d_weights.size() != getPattern()->getNumAtoms()) {
      throw ValueErrorException(
          " pattern->getNumAtoms() != len(feature weight vector)");
    }
  }

  const std::string &getFamily() const { return d_family; }
  const std::string &getType() const { return d_type; }
  const std::string &getSmarts() const { return d_smarts; }

  void normalizeWeights();

  const ROMol *getPattern() const { return dp_pattern.get(); }

 private:
  std::string d_family;
  std::string d_type;
  std::string d_smarts;
  boost::shared_ptr<ROMol> dp_pattern;
  std::vector<double> d_weights;
};
}  // namespace RDKix
#endif
