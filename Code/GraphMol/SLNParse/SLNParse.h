//
//  Copyright (c) 2008, Novartis Institutes for BioMedical Research Inc.
//  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Novartis Institutes for BioMedical Research Inc.
//       nor the names of its contributors may be used to endorse or promote
//       products derived from this software without specific prior
//       written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by Greg Landrum, September 2006
//
#include <RDGeneral/export.h>
#ifndef __RD_SLNPARSE_H__
#define __RD_SLNPARSE_H__

#include <string>
#include <exception>
#include <utility>

namespace RDKix {
class RWMol;
class ROMol;

namespace SLNParse {
RDKIX_SLNPARSE_EXPORT void finalizeQueryMol(ROMol *mol, bool mergeHs);
void CleanupAfterParse(RWMol *mol);
}  // namespace SLNParse

RDKIX_SLNPARSE_EXPORT RWMol *SLNToMol(const std::string &smi,
                                      bool sanitize = true, int debugParse = 0);

RDKIX_SLNPARSE_EXPORT RWMol *SLNQueryToMol(const std::string &smi,
                                           bool mergeHs = true,
                                           int debugParse = 0);

class RDKIX_SLNPARSE_EXPORT SLNParseException : public std::exception {
 public:
  SLNParseException(const char *msg) : _msg(msg) {}
  SLNParseException(std::string msg) : _msg(std::move(msg)) {}
  const char *what() const noexcept override { return _msg.c_str(); }
  ~SLNParseException() noexcept override = default;

 private:
  std::string _msg;
};

}  // namespace RDKix
#endif
