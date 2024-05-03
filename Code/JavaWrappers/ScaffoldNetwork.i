/*
*
*  Copyright (c) 2019, Greg Landrum and T5 Informatics GmbH
*  All rights reserved.
*
*  This file is part of the RDKix.
*  The contents are covered by the terms of the BSD license
*  which is included in the file license.txt, found at the root
*  of the RDKix source tree.
*
*/
%{
#include <GraphMol/ScaffoldNetwork/ScaffoldNetwork.h>
typedef std::vector<std::string> STR_VECT;
typedef std::vector<unsigned> UINT_VECT;
%}


%template(ROMol_Vect) std::vector<boost::shared_ptr<RDKix::ROMol>>;
%template(NetworkEdge_Vect) std::vector<RDKix::ScaffoldNetwork::NetworkEdge>;
%include <GraphMol/ScaffoldNetwork/ScaffoldNetwork.h>
%template(createScaffoldNetwork) RDKix::ScaffoldNetwork::createScaffoldNetwork<std::vector<boost::shared_ptr<RDKix::ROMol>>>;

// this is needed for the csharp wrappers to access count values as it does
// not seem to be possible to wrap std::vector<unsigned>
%extend RDKix::ScaffoldNetwork::ScaffoldNetwork {
    unsigned int nodeCount(unsigned int nodeNumber) {
        return $self->counts.at(nodeNumber);
    }
}