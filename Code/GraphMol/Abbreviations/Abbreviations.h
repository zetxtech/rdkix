//
//  Copyright (C) 2020 Greg Landrum and T5 Informatics GmbH
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_ABBREVIATIONS_H
#define RD_ABBREVIATIONS_H
#include <GraphMol/Substruct/SubstructMatch.h>
#include <utility>
#include <vector>
#include <string>
#include <memory>

namespace RDKix {
class ROMol;
class RWMol;

namespace Abbreviations {
struct RDKIX_ABBREVIATIONS_EXPORT AbbreviationDefinition {
  std::string label;
  std::string displayLabel;
  std::string displayLabelW;
  std::string smarts;
  std::shared_ptr<ROMol> mol;                  //!< optional
  std::vector<unsigned int> extraAttachAtoms;  //!< optional
  bool operator==(const AbbreviationDefinition& other) const {
    return label == other.label && displayLabel == other.displayLabel &&
           displayLabelW == other.displayLabelW && smarts == other.smarts;
  }
  bool operator!=(const AbbreviationDefinition& other) const {
    return !(*this == other);
  }
};
struct RDKIX_ABBREVIATIONS_EXPORT AbbreviationMatch {
  MatchVectType match;
  AbbreviationDefinition abbrev;
  AbbreviationMatch(std::vector<std::pair<int, int>> matchArg,
                    AbbreviationDefinition abbrevArg)
      : match(std::move(matchArg)), abbrev(std::move(abbrevArg)) {}
  AbbreviationMatch() : match(), abbrev() {}
  bool operator==(const AbbreviationMatch& other) const {
    return abbrev == other.abbrev && match == other.match;
  }
  bool operator!=(const AbbreviationMatch& other) const {
    return !(*this == other);
  }
};
namespace common_properties {
RDKIX_ABBREVIATIONS_EXPORT extern const std::string numDummies;
RDKIX_ABBREVIATIONS_EXPORT extern const std::string origAtomMapping;
RDKIX_ABBREVIATIONS_EXPORT extern const std::string origBondMapping;
}  // namespace common_properties
namespace Utils {
//! returns the default set of abbreviation definitions
RDKIX_ABBREVIATIONS_EXPORT std::vector<AbbreviationDefinition>
getDefaultAbbreviations();
//! returns the default set of linker definitions
RDKIX_ABBREVIATIONS_EXPORT std::vector<AbbreviationDefinition>
getDefaultLinkers();

//! parses a string describing abbreviation matches and returns the result
/*

\param text the data to be parsed, see below for the format
\param removeExtraDummies controls whether or not dummy atoms beyond atom 0 are
       removed. Set this to true to create abbreviations for linkers
\param allowConnectionToDummies allows abbreviations to directly connect to
       abbreviations. set this to true for linkers

Format of the text data:
  A series of lines, each of which contains:

    label SMARTS displayLabel displayLabelW

  the "displayLabel" and "displayLabelW" fields are optional.
  where label is the label used for the abbreviation,
  SMARTS is the SMARTS definition of the abbreviation.
  displayLabel is used in drawings to render the abbreviations.
  displayLabelW is the display label if a bond comes in from the right

  Use dummies to indicate attachment points. The assumption is that the first
  atom is a dummy (one will be added if this is not true) and that the second
  atom is the surrogate for the rest of the group.

*/
RDKIX_ABBREVIATIONS_EXPORT std::vector<AbbreviationDefinition>
parseAbbreviations(const std::string& text, bool removeExtraDummies = false,
                   bool allowConnectionToDummies = false);
//! \brief equivalent to calling \c parseAbbreviations(text,true,true)
inline std::vector<AbbreviationDefinition> parseLinkers(
    const std::string& text) {
  return parseAbbreviations(text, true, true);
};
}  // namespace Utils

//! returns all matches for the abbreviations across the molecule
/*!

    \param abbrevs the abbreviations to look for. This list is used in order.
    \param maxCoverage any abbreviation that covers than more than this fraction
        of the molecule's atoms (not counting dummies) will not be returned.
*/
RDKIX_ABBREVIATIONS_EXPORT std::vector<AbbreviationMatch>
findApplicableAbbreviationMatches(
    const ROMol& mol, const std::vector<AbbreviationDefinition>& abbrevs,
    double maxCoverage = 0.4);
//! applies the abbreviation matches to a molecule, modifying it in place.
//! the modified molecule is not sanitized
RDKIX_ABBREVIATIONS_EXPORT void applyMatches(
    RWMol& mol, const std::vector<AbbreviationMatch>& matches);
//! creates "SUP" SubstanceGroups on the molecule describing the abbreviation
RDKIX_ABBREVIATIONS_EXPORT void labelMatches(
    RWMol& mol, const std::vector<AbbreviationMatch>& matches);
//! convenience function for finding and applying abbreviations
//! the modified molecule is not sanitized
RDKIX_ABBREVIATIONS_EXPORT void condenseMolAbbreviations(
    RWMol& mol, const std::vector<AbbreviationDefinition>& abbrevs,
    double maxCoverage = 0.4, bool sanitize = true);
//! convenience function for finding and labeling abbreviations as SUP
//! SubstanceGroups
RDKIX_ABBREVIATIONS_EXPORT void labelMolAbbreviations(
    RWMol& mol, const std::vector<AbbreviationDefinition>& abbrevs,
    double maxCoverage = 0.4);
//! collapses abbreviation (i.e. "SUP") substance groups
//! the modified molecule is not sanitized
RDKIX_ABBREVIATIONS_EXPORT void condenseAbbreviationSubstanceGroups(RWMol& mol);

}  // namespace Abbreviations
}  // namespace RDKix
#endif
