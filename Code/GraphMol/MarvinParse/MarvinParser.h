//
//  Copyright (C) 2022-2023 Tad Hurst, Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <RDGeneral/export.h>

#ifndef RD_MARVINPARSER_H
#define RD_MARVINPARSER_H

#include <GraphMol/RDKixBase.h>
#include <GraphMol/ChemReactions/Reaction.h>

#include <string>
#include <iostream>

namespace RDKix {
namespace v2 {
namespace MarvinParser {
RDKIX_MARVINPARSER_EXPORT bool MrvFileIsReaction(const std::string &fname);
RDKIX_MARVINPARSER_EXPORT bool MrvDataStreamIsReaction(std::istream &inStream);
RDKIX_MARVINPARSER_EXPORT bool MrvBlockIsReaction(
    const std::string &molmrvText);

struct RDKIX_MARVINPARSER_EXPORT MrvParserParams {
  bool sanitize = true; /**< sanitize the molecule after building it */
  bool removeHs = true; /**< remove Hs after constructing the molecule */
};

RDKIX_MARVINPARSER_EXPORT std::unique_ptr<RWMol> MolFromMrvDataStream(
    std::istream &inStream, const MrvParserParams &params = MrvParserParams());
RDKIX_MARVINPARSER_EXPORT std::unique_ptr<RWMol> MolFromMrvBlock(
    const std::string &molmrvText,
    const MrvParserParams &params = MrvParserParams());
RDKIX_MARVINPARSER_EXPORT std::unique_ptr<RWMol> MolFromMrvFile(
    const std::string &fName,
    const MrvParserParams &params = MrvParserParams());

RDKIX_MARVINPARSER_EXPORT std::unique_ptr<ChemicalReaction>
ReactionFromMrvDataStream(std::istream &inStream,
                          const MrvParserParams &params = MrvParserParams());
RDKIX_MARVINPARSER_EXPORT std::unique_ptr<ChemicalReaction>
ReactionFromMrvBlock(const std::string &molmrvText,
                     const MrvParserParams &params = MrvParserParams());
RDKIX_MARVINPARSER_EXPORT std::unique_ptr<ChemicalReaction> ReactionFromMrvFile(
    const std::string &fName,
    const MrvParserParams &params = MrvParserParams());
}  // namespace MarvinParser
}  // namespace v2

inline namespace v1 {
inline bool MrvFileIsReaction(const std::string &fname) {
  return v2::MarvinParser::MrvFileIsReaction(fname);
}
inline bool MrvDataStreamIsReaction(std::istream *inStream) {
  return v2::MarvinParser::MrvDataStreamIsReaction(*inStream);
}

inline bool MrvDataStreamIsReaction(std::istream &inStream) {
  return v2::MarvinParser::MrvDataStreamIsReaction(inStream);
}
inline bool MrvBlockIsReaction(const std::string &molmrvText) {
  return v2::MarvinParser::MrvBlockIsReaction(molmrvText);
}

inline RWMol *MrvDataStreamToMol(std::istream *inStream, bool sanitize = false,
                                 bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::MolFromMrvDataStream(*inStream, params).release();
}
inline RWMol *MrvDataStreamToMol(std::istream &inStream, bool sanitize = false,
                                 bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::MolFromMrvDataStream(inStream, params).release();
}
inline RWMol *MrvBlockToMol(const std::string &molmrvText,
                            bool sanitize = false, bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::MolFromMrvBlock(molmrvText, params).release();
}
inline RWMol *MrvFileToMol(const std::string &fName, bool sanitize = false,
                           bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::MolFromMrvFile(fName, params).release();
}

inline ChemicalReaction *MrvDataStreamToChemicalReaction(
    std::istream *inStream, bool sanitize = false, bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::ReactionFromMrvDataStream(*inStream, params)
      .release();
}
inline ChemicalReaction *MrvDataStreamToChemicalReaction(
    std::istream &inStream, bool sanitize = false, bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::ReactionFromMrvDataStream(inStream, params)
      .release();
}
inline ChemicalReaction *MrvBlockToChemicalReaction(
    const std::string &molmrvText, bool sanitize = false,
    bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::ReactionFromMrvBlock(molmrvText, params).release();
}
inline ChemicalReaction *MrvFileToChemicalReaction(const std::string &fName,
                                                   bool sanitize = false,
                                                   bool removeHs = false) {
  v2::MarvinParser::MrvParserParams params;
  params.sanitize = sanitize;
  params.removeHs = removeHs;
  return v2::MarvinParser::ReactionFromMrvFile(fName, params).release();
}
}  // namespace v1

struct RDKIX_MARVINPARSER_EXPORT MrvWriterParams {
  bool includeStereo = true; /**< toggles inclusion of stereochemical */
  bool kekulize =
      true; /**< triggers kekulization of the molecule before it's written */
  bool prettyPrint = false;   /**< makes the output more human readable */
  unsigned int precision = 6; /**< precision of the coordinates */
};

// \brief Writes a Marvin (MRV) file for a molecule
/*!
 *     \param mol           - the molecule
 *     \param fName         - the file to write to
 *     \param params        - marvin write params
 *     \param confId        - selects which conformation to output
 */
RDKIX_MARVINPARSER_EXPORT void MolToMrvFile(const ROMol &mol,
                                            const std::string &fName,
                                            const MrvWriterParams &params,
                                            int confId = -1);

// \brief Writes a Marvin (MRV) file for a molecule
/*!
 *     \param mol           - the molecule
 *     \param fName         - the file to write to
 *     \param includeStereo - toggles inclusion of stereochemical information in
 * the output \param confId        - selects which conformation to output \param
 * kekulize      - triggers kekulization of the molecule before it's written
 *     \param prettyPrint   - makes the output more human readable
 */
inline void MolToMrvFile(const ROMol &mol, const std::string &fName,
                         bool includeStereo = true, int confId = -1,
                         bool kekulize = true, bool prettyPrint = false) {
  MrvWriterParams params{includeStereo, kekulize, prettyPrint};
  MolToMrvFile(mol, fName, params, confId);
}

// \brief Returns a Marvin (Mrv) Mol block for a molecule
/*!
 *     \param mol           - the molecule
 *     \param params        - marvin write params
 *     \param confId        - selects which conformation to output
 */
RDKIX_MARVINPARSER_EXPORT std::string MolToMrvBlock(
    const ROMol &mol, const MrvWriterParams &params, int confId = -1);

// \brief Returns a Marvin (Mrv) Mol block for a molecule
/*!
 *     \param mol           - the molecule
 *     \param includeStereo - toggles inclusion of stereochemical information in
 * the output \param confId        - selects which conformation to output \param
 * kekulize      - triggers kekulization of the molecule before it's written
 *     \param prettyPrint   - makes the output more human readable
 */
inline std::string MolToMrvBlock(const ROMol &mol, bool includeStereo = true,
                                 int confId = -1, bool kekulize = true,
                                 bool prettyPrint = false) {
  MrvWriterParams params{includeStereo, kekulize, prettyPrint};
  return MolToMrvBlock(mol, params, confId);
}

RDKIX_MARVINPARSER_EXPORT std::string ChemicalReactionToMrvBlock(
    const ChemicalReaction &rxn, bool prettyPrint = false);

RDKIX_MARVINPARSER_EXPORT void ChemicalReactionToMrvFile(
    const ChemicalReaction &rxn, const std::string &fName,
    bool prettyPrint = false);
}  // namespace RDKix

#endif
