//
//  Copyright (C) 2020 Gareth Jones, Glysade LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

%include "std_vector.i"

%{
#include <GraphMol/RascalMCES/RascalClusterOptions.h>
#include <GraphMol/RascalMCES/RascalOptions.h>
#include <GraphMol/RascalMCES/RascalResult.h>
#include <GraphMol/RascalMCES/RascalMCES.h>
%}



%include <GraphMol/RascalMCES/RascalClusterOptions.h>
%include <GraphMol/RascalMCES/RascalOptions.h>
%ignore RDKix::RascalMCES::RascalResult::getMcesMol;
%include <GraphMol/RascalMCES/RascalResult.h>
%ignore RDKix::RascalMCES::rascalCluster;
%ignore RDKix::RascalMCES::rascalButinaCluster;
%include <GraphMol/RascalMCES/RascalMCES.h>
// The extra functions in extend_std_vector.i do not play well with RascalResult
%ignore std::vector<RDKix::RascalMCES::RascalResult>::equals;
%ignore std::vector<RDKix::RascalMCES::RascalResult>::vector(size_type);
%template(RascalResult_Vect) std::vector<RDKix::RascalMCES::RascalResult>;
%template(Unsigned_Vect_Vect) std::vector<std::vector<unsigned int>>;

// The Rascal code uses std::shared_ptr rather than boost::shared_ptr

%extend RDKix::RascalMCES::RascalResult {
	RDKix::ROMol *getMCESMol() {
		auto shared_ptr = ($self)->getMcesMol();
		return shared_ptr.get();
	}
}

%inline %{
	namespace RDKix {
		namespace RascalMCES {
		   class RascalApp {
		   };
		}
	}
%}


%extend RDKix::RascalMCES::RascalApp {
    static std::vector<std::vector<unsigned int> > RascalCluster(const std::vector<boost::shared_ptr<RDKix::ROMol> >& mols, const RDKix::RascalMCES::RascalClusterOptions& clusterOptions=RascalClusterOptions()) {
		std::vector<std::shared_ptr<RDKix::ROMol> > rascalMolecules;
		for (auto molIn: mols) {
			rascalMolecules.emplace_back(new RDKix::ROMol(*molIn));
		}
		return RDKix::RascalMCES::rascalCluster(rascalMolecules, clusterOptions);
	}
    static std::vector<std::vector<unsigned int> > RascalButinaCluster(const std::vector<boost::shared_ptr<RDKix::ROMol> >& mols, const RDKix::RascalMCES::RascalClusterOptions& clusterOptions=RascalClusterOptions()) {
		std::vector<std::shared_ptr<RDKix::ROMol> > rascalMolecules;
		for (auto molIn: mols) {
			rascalMolecules.emplace_back(new RDKix::ROMol(*molIn));
		}
		return RDKix::RascalMCES::rascalButinaCluster(rascalMolecules, clusterOptions);
	}
}
