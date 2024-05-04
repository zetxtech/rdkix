/*
 *
 *  Copyright (c) 2020-2021, Greg Landrum and T5 Informatics GmbH
 *  All rights reserved.
 *
 *  This file is part of the RDKix.
 *  The contents are covered by the terms of the BSD license
 *  which is included in the file license.txt, found at the root
 *  of the RDKix source tree.
 *
 */
%{
#include <GraphMol/MolEnumerator/MolEnumerator.h>
%}


%ignore RDKix::MolEnumerator::detail::idxPropName;
%ignore RDKix::MolEnumerator::detail::preserveOrigIndices;
%ignore RDKix::MolEnumerator::detail::removeOrigIndices;

%ignore RDKix::MolEnumerator::MolEnumeratorOp;
%ignore RDKix::MolEnumerator::PositionVariationOp;
%ignore RDKix::MolEnumerator::LinkNodeOp;
%ignore RDKix::MolEnumerator::RepeatUnitOp;

%inline %{

RDKix::MolEnumerator::MolEnumeratorParams getLinkNodeParams(){
    RDKix::MolEnumerator::MolEnumeratorParams res;
    res.dp_operation.reset(new RDKix::MolEnumerator::LinkNodeOp());
    return res;
}
RDKix::MolEnumerator::MolEnumeratorParams getPositionVariationParams(){
    RDKix::MolEnumerator::MolEnumeratorParams res;
    res.dp_operation.reset(new RDKix::MolEnumerator::PositionVariationOp());
    return res;
}
RDKix::MolEnumerator::MolEnumeratorParams getRepeatUnitParams(){
    RDKix::MolEnumerator::MolEnumeratorParams res;
    res.dp_operation.reset(new RDKix::MolEnumerator::RepeatUnitOp());
    return res;
}

%}
%include<GraphMol/MolEnumerator/MolEnumerator.h>
