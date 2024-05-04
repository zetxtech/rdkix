/*
*
*  Copyright (c) 2020, Greg Landrum and T5 Informatics GmbH
*  All rights reserved.
*
*  This file is part of the RDKix.
*  The contents are covered by the terms of the BSD license
*  which is included in the file license.txt, found at the root
*  of the RDKix source tree.
*
*/
%{
#include <GraphMol/Abbreviations/Abbreviations.h>
%}


%template(AbbreviationDefinition_Vect) std::vector<RDKix::Abbreviations::AbbreviationDefinition>;
%template(AbbreviationMatch_Vect) std::vector<RDKix::Abbreviations::AbbreviationMatch>;
%include <GraphMol/Abbreviations/Abbreviations.h>
