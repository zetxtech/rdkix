/* 
* $Id$
*
*  Copyright (c) 2010, Novartis Institutes for BioMedical Research Inc.
*  All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met: 
*
*     * Redistributions of source code must retain the above copyright 
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following 
*       disclaimer in the documentation and/or other materials provided 
*       with the distribution.
*     * Neither the name of Novartis Institutes for BioMedical Research Inc. 
*       nor the names of its contributors may be used to endorse or promote 
*       products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_pair.i"

%{
#include <GraphMol/AtomIterators.h>
#include <GraphMol/ROMol.h>
%}


%include <GraphMol/AtomIterators.h>
/* Equality testing operators have been overloaded, so we need to expose them in a different way.
   These 'rename' lines appear to have to precede the 'template' definitions. */
%rename(eq) RDKix::AtomIterator_<RDKix::Atom,RDKix::ROMol>::operator==;
%rename(ne) RDKix::AtomIterator_<RDKix::Atom,RDKix::ROMol>::operator!=;
%rename(eq) RDKix::HeteroatomIterator_<RDKix::Atom,RDKix::ROMol>::operator==;
%rename(ne) RDKix::HeteroatomIterator_<RDKix::Atom,RDKix::ROMol>::operator!=;
%rename(eq) RDKix::AromaticAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator==;
%rename(ne) RDKix::AromaticAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator!=;
%rename(eq) RDKix::QueryAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator==;
%rename(ne) RDKix::QueryAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator!=;
/* Increment and decrement operators currently necessary */
%rename(next) RDKix::AtomIterator_<RDKix::Atom,RDKix::ROMol>::operator++;
%rename(next) RDKix::HeteroatomIterator_<RDKix::Atom,RDKix::ROMol>::operator++;
%rename(next) RDKix::AromaticAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator++;
%rename(next) RDKix::QueryAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator++;
%rename(prev) RDKix::AtomIterator_<RDKix::Atom,RDKix::ROMol>::operator--;
%rename(prev) RDKix::HeteroatomIterator_<RDKix::Atom,RDKix::ROMol>::operator--;
%rename(prev) RDKix::AromaticAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator--;
%rename(prev) RDKix::QueryAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator--;
/* A better name for the iterator's Atom object */
%rename(getAtom) RDKix::AtomIterator_<RDKix::Atom,RDKix::ROMol>::operator*;
%rename(getAtom) RDKix::HeteroatomIterator_<RDKix::Atom,RDKix::ROMol>::operator*;
%rename(getAtom) RDKix::AromaticAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator*;
%rename(getAtom) RDKix::QueryAtomIterator_<RDKix::Atom,RDKix::ROMol>::operator*;

%template(AtomIterator) RDKix::AtomIterator_<RDKix::Atom,RDKix::ROMol>;
%template(HeteroatomIterator) RDKix::HeteroatomIterator_<RDKix::Atom,RDKix::ROMol>;
%template(AromaticAtomIterator) RDKix::AromaticAtomIterator_<RDKix::Atom,RDKix::ROMol>;
%template(QueryAtomIterator) RDKix::QueryAtomIterator_<RDKix::Atom,RDKix::ROMol>;



