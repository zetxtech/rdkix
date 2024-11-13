/*
 *  Copyright (c) 2015, Novartis Institutes for BioMedical Research Inc.
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

//%import "ROMol.i"
%include "std_vector.i"


%{
#include <GraphMol/SubstructLibrary/SubstructLibrary.h>
#include <GraphMol/TautomerQuery/TautomerQuery.h>
#include <GraphMol/GeneralizedSubstruct/XQMol.h>
using RDKix::GeneralizedSubstruct::ExtendedQueryMol;
%}
%shared_ptr(RDKix::TautomerQuery)
%shared_ptr(RDKix::MolHolderBase)
%shared_ptr(RDKix::MolHolder)
%shared_ptr(RDKix::CachedMolHolder)
%shared_ptr(RDKix::CachedSmilesMolHolder)
%shared_ptr(RDKix::CachedTrustedSmilesMolHolder)
%shared_ptr(RDKix::FPHolderBase)
%shared_ptr(RDKix::PatternHolder)
%shared_ptr(RDKix::TautomerPatternHolder)
%shared_ptr(RDKix::KeyHolderBase)
%shared_ptr(RDKix::KeyFromPropHolder)

%template(UChar_Vect) std::vector<unsigned char>;

%typemap(javacode) RDKix::SubstructLibrary %{
  public static SubstructLibrary Deserialize(byte[] b) {
    UChar_Vect vec = null;
    try {
      vec = new UChar_Vect();
      vec.reserve(b.length);
      for (int size=0;size<b.length;++size) {
        vec.add((short)b[size]);
      }
      return new SubstructLibrary(vec);
    } finally {
      if (vec != null) {
        vec.delete();
      }
    }
  }
%}

%extend RDKix::SubstructLibrary {
  SubstructLibrary(const std::vector<unsigned char> & data ) {
    std::string str(data.begin(), data.end());
    return new RDKix::SubstructLibrary(str);
  }

  static bool canSerialize() {
    return RDKix::SubstructLibraryCanSerialize();
  }
}

 


#ifdef SWIGJAVA
%typemap(jni) std::string RDKix::SubstructLibrary::Serialize "jbyteArray"
%typemap(jtype) std::string RDKix::SubstructLibrary::Serialize "byte[]"
%typemap(jstype) std::string RDKix::SubstructLibrary::Serialize "byte[]"
%typemap(javaout) std::string RDKix::SubstructLibrary::Serialize {
  return $jnicall;
}
%typemap(out) std::string RDKix::SubstructLibrary::Serialize {
  $result = JCALL1(NewByteArray, jenv, $1.size());
  JCALL4(SetByteArrayRegion, jenv, $result, 0, $1.size(), (const jbyte*)$1.c_str());
}
#endif

%include <GraphMol/TautomerQuery/TautomerQuery.h>
%include <GraphMol/SubstructLibrary/SubstructLibrary.h>

%extend RDKix::SubstructLibrary {
 %template(getMatches) getMatches<ROMol>;
 %template(getMatches) getMatches<TautomerQuery>;
 %template(getMatches) getMatches<RDKix::GeneralizedSubstruct::ExtendedQueryMol>;
 %template(countMatches) countMatches<ROMol>;
 %template(countMatches) countMatches<TautomerQuery>;
 %template(countMatches) countMatches<ExtendedQueryMol>;
 %template(hasMatch) hasMatch<ROMol>;
 %template(hasMatch) hasMatch<TautomerQuery>;
 %template(hasMatch) hasMatch<ExtendedQueryMol>;
}


%pragma(java) modulecode=%{
   public static SubstructLibrary SubstructLibraryDeserialize(byte[] b) {
     UChar_Vect vec = new UChar_Vect();
     vec.reserve(b.length);
     for (int size=0;size<b.length;++size) {
       vec.add((short)b[size]);
     }
     return new SubstructLibrary(vec);
   }
%}
