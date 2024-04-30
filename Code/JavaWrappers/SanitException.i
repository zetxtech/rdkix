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

%{
#include <GraphMol/SanitException.h>
%}



%include <GraphMol/SanitException.h>
#ifdef SWIGJAVA
// support upcasting from MolSanitizeException to the other exception types so that
// we can do something with the output of detectChemistryProblems()

// approach from: http://www.swig.org/Doc3.0/Java.html#Java_adding_downcasts
%exception RDKix::AtomSanitizeException::dynamic_cast(RDKix::MolSanitizeException) {
  $action
    if(!result){
      jclass excep = jenv->FindClass("java/lang/ClassCastException");
      if (excep) {
        jenv->ThrowNew(excep, "dynamic_cast exception");
      }
    }
}
%extend RDKix::AtomSanitizeException {
    static RDKix::AtomSanitizeException *dynamic_cast(RDKix::MolSanitizeException *mse){
        return dynamic_cast<RDKix::AtomSanitizeException *>(mse);
    }
};

%exception RDKix::AtomValenceException::dynamic_cast(RDKix::MolSanitizeException) {
  $action
    if(!result){
      jclass excep = jenv->FindClass("java/lang/ClassCastException");
      if (excep) {
        jenv->ThrowNew(excep, "dynamic_cast exception");
      }
    }
}
%extend RDKix::AtomValenceException {
    static RDKix::AtomValenceException *dynamic_cast(RDKix::MolSanitizeException *mse){
        return dynamic_cast<RDKix::AtomValenceException *>(mse);
    }
};

%exception RDKix::AtomKekulizeException::dynamic_cast(RDKix::MolSanitizeException) {
  $action
    if(!result){
      jclass excep = jenv->FindClass("java/lang/ClassCastException");
      if (excep) {
        jenv->ThrowNew(excep, "dynamic_cast exception");
      }
    }
}
%extend RDKix::AtomKekulizeException {
    static RDKix::AtomKekulizeException *dynamic_cast(RDKix::MolSanitizeException *mse){
        return dynamic_cast<RDKix::AtomKekulizeException *>(mse);
    }
};

%exception RDKix::KekulizeException::dynamic_cast(RDKix::MolSanitizeException) {
  $action
    if(!result){
      jclass excep = jenv->FindClass("java/lang/ClassCastException");
      if (excep) {
        jenv->ThrowNew(excep, "dynamic_cast exception");
      }
    }
}
%extend RDKix::KekulizeException {
    static RDKix::KekulizeException *dynamic_cast(RDKix::MolSanitizeException *mse){
        return dynamic_cast<RDKix::KekulizeException *>(mse);
    }
};
#endif