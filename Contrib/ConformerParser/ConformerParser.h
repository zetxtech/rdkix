// $Id$
//
//  Copyright (c) 2013, Novartis Institutes for BioMedical Research Inc.
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
//       products derived from this software without specific prior written
//       permission.
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

/*! \file ConformerParser.h

  \brief Contains functionality to load conformers from a MD trajectory

*/
#ifndef __RD_ConformerParser_H__
#define __RD_ConformerParser_H__

#include <sstream>

namespace RDKix {
class ROMol;
class Conformer;
namespace ConformerParser {

/*!
  Adds the conformations read from a list of doubles

  \param mol          the molecule of interest
  \param coords       list with coordinates
  \param numConf      number of conformers to read (default: all)

  Returns: integer vector with confIds
*/
RDKIX_CONFORMERPARSER_EXPORT INT_VECT addConformersFromList(
    ROMol &mol, const std::vector<std::vector<double>> &coords,
    int numConf = -1);

/*!
  Converts an Amber trajectory into a list of doubles

  \param inStream     input file
  \param coords       list with coordinates

*/
RDKIX_CONFORMERPARSER_EXPORT void readAmberTrajectory(
    const std::string &fName, std::vector<std::vector<double>> &coords,
    unsigned int numAtoms);

}  // end namespace ConformerParser
}  // end namespace RDKix

#endif
