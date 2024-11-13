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

// Defines all of the C++ -> Java exception handling

%{
#include <RDGeneral/Exceptions.h>
#include <JavaWrappers/GenericRDKixException.h>
%}

// ===== ChemicalReactionException =====
%typemap(javabase) RDKix::ChemicalReactionException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.ChemicalReactionException") RDKix::ChemicalReactionException {
  jclass excep = jenv->FindClass("org/RDKix/ChemicalReactionException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::ChemicalReactionException %{
  public String getMessage() {
    return what();
  }
%}


// ===== ChemicalReactionParserException =====
%typemap(javabase) RDKix::ChemicalReactionParserException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.ChemicalReactionParserException") RDKix::ChemicalReactionParserException {
  jclass excep = jenv->FindClass("org/RDKix/ChemicalReactionParserException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::ChemicalReactionParserException %{
  public String getMessage() {
    return what();
  }
%}

// ===== ConformerException =====
%typemap(javabase) RDKix::ConformerException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.ConformerException") RDKix::ConformerException {
  jclass excep = jenv->FindClass("org/RDKix/ConformerException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::ConformerException %{
  public String getMessage() {
    return what();
  }
%}

// ===== MolPicklerException =====
%typemap(javabase) RDKix::MolPicklerException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.MolPicklerException") RDKix::MolPicklerException {
  jclass excep = jenv->FindClass("org/RDKix/MolPicklerException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::MolPicklerException %{
  public String getMessage() {
    return what();
  }
%}

// ===== MolSanitizeException =====
%typemap(javabase) RDKix::MolSanitizeException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.MolSanitizeException") RDKix::MolSanitizeException {
  jclass excep = jenv->FindClass("org/RDKix/MolSanitizeException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::MolSanitizeException %{
  public String getMessage() {
    return what();
  }
%}

// ===== SmilesParseException =====
%typemap(javabase) RDKix::SmilesParse::SmilesParseException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.SmilesParseException") RDKix::SmilesParse::SmilesParseException {
  jclass excep = jenv->FindClass("org/RDKix/SmilesParseException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::SmilesParse::SmilesParseException %{
  public String getMessage() {
    return what();
  }
%}

// ===== KeyErrorException =====
%typemap(javabase) KeyErrorException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.KeyErrorException") KeyErrorException {
  jclass excep = jenv->FindClass("org/RDKix/KeyErrorException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::KeyErrorException %{
  public String getMessage() {
    return what();
  }
%}

// ===== GenericRDKixException =====
%typemap(javabase) RDKix::GenericRDKixException "java.lang.RuntimeException";
%typemap(throws, throws="org.RDKix.GenericRDKixException") RDKix::GenericRDKixException {
  jclass excep = jenv->FindClass("org/RDKix/GenericRDKixException");
  if (excep)
    jenv->ThrowNew(excep, $1.what());
  return $null;
}
%typemap(javacode) RDKix::GenericRDKixException %{
  public String getMessage() {
    return what();
  }
%}

// Note that these files must follow the typemap declarations
%include <RDGeneral/Exceptions.h>
%include <JavaWrappers/GenericRDKixException.h>

%exception {
  try {
     $action
  } catch (RDKix::ChemicalReactionException &e) {
    jclass clazz = jenv->FindClass("org/RDKix/ChemicalReactionException");
    jenv->ThrowNew(clazz, e.what());
    return $null;
  } catch (RDKix::ChemicalReactionParserException &e) {
    jclass clazz = jenv->FindClass("org/RDKix/ChemicalReactionParserException");
    jenv->ThrowNew(clazz, e.what());
    return $null;
  } catch (RDKix::ConformerException &e) {
    jclass clazz = jenv->FindClass("org/RDKix/ConformerException");
    jenv->ThrowNew(clazz, e.what());
    return $null;
  } catch (RDKix::MolPicklerException &e) {
    jclass clazz = jenv->FindClass("org/RDKix/MolPicklerException");
    jenv->ThrowNew(clazz, e.what());
    return $null;
  } catch (RDKix::MolSanitizeException &e) {
    jclass clazz = jenv->FindClass("org/RDKix/MolSanitizeException");
    jenv->ThrowNew(clazz, e.what());
    return $null;
  } catch (RDKix::SmilesParseException &e) {
    jclass clazz = jenv->FindClass("org/RDKix/SmilesParseException");
    jenv->ThrowNew(clazz, e.what());
    return $null;
  } catch (KeyErrorException &e) {
    jclass clazz = jenv->FindClass("org/RDKix/KeyErrorException");
    jenv->ThrowNew(clazz, e.key().c_str());
    return $null;

  // Generic exception -- anything else
  } catch (std::exception &e) {
    jclass clazz = jenv->FindClass("org/RDKix/GenericRDKixException");
    jenv->ThrowNew(clazz, "Unknown exception");
    return $null;

  }

}
