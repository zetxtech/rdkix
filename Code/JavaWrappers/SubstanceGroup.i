//
// Created by Gareth Jones on 9/2/2020.
//
// Copyright 2020 Schrodinger, Inc
//  @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.



%{
#include <GraphMol/SubstanceGroup.h>

const RDKix::SubstanceGroup *getSubstanceGroupWithIdx(RDKix::ROMol &mol, unsigned int idx) {
  auto &groups = RDKix::getSubstanceGroups(mol);
  return &(groups[idx]);
}

unsigned int getSubstanceGroupCount(RDKix::ROMol &mol) {
    return RDKix::getSubstanceGroups(mol).size();
}
%}


// Base class RDProps is wrapped with shared_ptr, so SubstanceGroup must be too.
%shared_ptr(RDKix::SubstanceGroup)
%ignore getSubstanceGroups;

RDKix::SubstanceGroup *getSubstanceGroupWithIdx(RDKix::ROMol &mol, unsigned int idx);
unsigned int getSubstanceGroupCount(RDKix::ROMol &mol);

%ignore RDKix::SubstanceGroup::getAtoms;
%rename(getAtoms) RDKix::SubstanceGroup::getSgAtoms;
%ignore RDKix::SubstanceGroup::getBonds;
%rename(getBonds) RDKix::SubstanceGroup::getSgBonds;
%ignore RDKix::SubstanceGroup::getBrackets;
%ignore RDKix::SubstanceGroup::getCStates;
%ignore RDKix::SubstanceGroup::getAttachPoints;

%include <GraphMol/SubstanceGroup.h>

%extend RDKix::SubstanceGroup {
  // Wrap getAtoms, getParentAtoms and getBonds to return vector<int> by value (not reference).
  // int instead of unsigned int as vector<unsigned int> is not wrapped in C#
  // Not using references to avoid a free() error from Java
  const std::vector<int> getSgAtoms() {
    auto atoms = $self->getAtoms();
    std::vector<int> atomIdxs(atoms.begin(), atoms.end());
    return atomIdxs;
  }
  const std::vector<int> getSgBonds() {
    auto bonds = $self->getBonds();
    std::vector<int> bondIdxs(bonds.begin(), bonds.end());
    return bondIdxs;
  }
  const std::vector<int> getSgParentAtoms() {
    auto atoms = $self->getParentAtoms();
    std::vector<int> atomIdxs(atoms.begin(), atoms.end());
    return atomIdxs;
  }

  // SWIG does not do well wrapping vectors of objects, so provide accessors
  const RDKix::SubstanceGroup::Bracket *getBracket(unsigned int idx)  {
    auto &brackets = $self->getBrackets();
    return &(brackets[idx]);
  }
  const RDKix::SubstanceGroup::CState *getCState(unsigned int idx)  {
    auto &cstates = $self->getCStates();
    return &(cstates[idx]);
  }
  const RDKix::SubstanceGroup::AttachPoint *getAttachPoint(unsigned int idx)  {
    auto &attachPoints = $self->getAttachPoints();
    return &(attachPoints[idx]);
  }
  size_t getBracketCount() {
    return $self->getBrackets().size();
  }
  size_t getCStateCount() {
    return $self->getCStates().size();
  }
  size_t getAttachPointCount() {
    return $self->getAttachPoints().size();
  }
}

%template(getStringProp) RDKix::SubstanceGroup::getProp<std::string>;
%template(getUIntProp) RDKix::SubstanceGroup::getProp<unsigned int>;
%template(getStringVectProp) RDKix::SubstanceGroup::getProp<RDKix::STR_VECT>;
%template(getUIntVectProp) RDKix::SubstanceGroup::getProp<RDKix::UINT_VECT>;

