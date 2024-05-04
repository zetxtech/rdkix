/*
*
*  Copyright (c) 2019, Greg Landrum
*  All rights reserved.
*
*  This file is part of the RDKix.
*  The contents are covered by the terms of the BSD license
*  which is included in the file license.txt, found at the root
*  of the RDKix source tree.
*
*/
%{
#include <GraphMol/RGroupDecomposition/RGroupDecompParams.h>
#include <GraphMol/RGroupDecomposition/RGroupDecomp.h>
typedef std::vector<std::string> STR_VECT;
%}

%include "std_string.i"


%template(SparseIntVect64) RDKix::SparseIntVect<boost::int64_t>;


%template(ROMol_Vect) std::vector<boost::shared_ptr<RDKix::ROMol>>;
%template(StringMolMap_Vect) std::vector<std::map<std::string, boost::shared_ptr<RDKix::ROMol>>>;
%template(StringROMol_VectMap) std::map<std::string,std::vector<boost::shared_ptr<RDKix::ROMol>>>;


%extend std::map<std::string, boost::shared_ptr<RDKix::ROMol>> {
  std::vector<std::string> keys() {
    std::vector<std::string> _keys;
    for(auto it : *self) {
      std::cerr << "* '" << it.first << "'" << std::endl;
      _keys.push_back(it.first);
    }
    return _keys;
  }
}

%extend std::map<std::string,std::vector<boost::shared_ptr<RDKix::ROMol>>> {
  std::vector<std::string> keys() {
    std::vector<std::string> _keys;
    for(auto it : *self) {
      _keys.push_back(it.first);
    }
    return _keys;
  }
}

%include <GraphMol/RGroupDecomposition/RGroupDecompParams.h>
%include <GraphMol/RGroupDecomposition/RGroupDecomp.h>
