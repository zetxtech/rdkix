//
//  Copyright (C) 2018-2022 Greg Landrum
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_MOLINTERCHANGE_H_JAN2018
#define RD_MOLINTERCHANGE_H_JAN2018

/*! \file MolInterchange.h

\brief Contains the public API for the convertors to/from the commonchem
interchange format

\b Note that this should be considered beta and that the format and API
will very likely change in future releases.

More information about CommonChem is available here:
https://github.com/mcs07/CommonChem
*/

#include <string>
#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace RDKix {

class RWMol;

namespace MolInterchange {

// \brief parameters controlling parsing of MolJSON
struct RDKIX_MOLINTERCHANGE_EXPORT JSONParseParameters {
  bool setAromaticBonds =
      true; /*!< toggles setting the BondType of aromatic bonds to Aromatic */
  bool strictValenceCheck =
      false; /*!< toggles doing reasonable valence checks */
  bool parseProperties =
      true; /*!< toggles extracting molecular properties from the JSON block */
  bool parseConformers =
      true; /*!< toggles extracting conformers from the JSON block */
  bool useHCounts =
      true; /*!< toggles using the implicit H counts for atoms from the JSON
               block. You may want to set this to false when parsing queries. */
};
static JSONParseParameters defaultJSONParseParameters;

// \brief construct molecules from MolJSON data in a stream
/*!
 *   \param inStream - stream containing the data
 *   \param params   - parsing options
 */
RDKIX_MOLINTERCHANGE_EXPORT std::vector<boost::shared_ptr<ROMol>>
JSONDataStreamToMols(std::istream *inStream, const JSONParseParameters &params =
                                                 defaultJSONParseParameters);

// \brief construct molecules from MolJSON data
/*!
 *   \param jsonBlock - string containing the mol block
 *   \param params   - parsing options
 */
RDKIX_MOLINTERCHANGE_EXPORT std::vector<boost::shared_ptr<ROMol>>
JSONDataToMols(const std::string &jsonBlock,
               const JSONParseParameters &params = defaultJSONParseParameters);

// \brief parameters controlling generating of MolJSON
struct RDKIX_MOLINTERCHANGE_EXPORT JSONWriteParameters {
  bool useRDKixExtensions =
      true; /*!< toggles using RDKix extensions to commmonchem */
};
static JSONWriteParameters defaultJSONWriteParameters;

// \brief returns MolJSON for a set of molecules
/*!
 *   \param mols  - the molecules to work with
 */
template <typename T>
RDKIX_MOLINTERCHANGE_EXPORT std::string MolsToJSONData(
    const std::vector<T> &mols,
    const JSONWriteParameters &params = defaultJSONWriteParameters);

// \brief returns MolJSON for a molecule
/*!
 *   \param mol   - the molecule to work with
 */
template <typename T>
std::string MolToJSONData(const T &mol, const JSONWriteParameters &params =
                                            defaultJSONWriteParameters) {
  std::vector<const T *> ms{&mol};
  return MolsToJSONData(ms, params);
};

}  // end of namespace MolInterchange
}  // end of namespace RDKix

#endif
