//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
/*! \file MolStandardize.h

        \brief Defines the CleanupParameters and some convenience functions.

*/
#include <RDGeneral/export.h>
#ifndef RD_MOLSTANDARDIZE_H
#define RD_MOLSTANDARDIZE_H

#include <string>
#include <GraphMol/RDKixBase.h>
#include <GraphMol/MolStandardize/Metal.h>

namespace RDKix {
class RWMol;
class ROMol;

namespace MolStandardize {

//! The CleanupParameters structure defines the default parameters for the
/// cleanup process and also allows the user to customize the process by
/// changing the parameters.
/*!

  <b>Notes:</b>
    - To customize the parameters, the structure must be initialized first.
                  (Another on the TODO list)
                - For this project, not all the parameters have been revealed.
  (TODO)

*/
struct RDKIX_MOLSTANDARDIZE_EXPORT CleanupParameters {
  // TODO reveal all parameters
 private:
  const char *rdbase_cstr = std::getenv("RDBASE");

 public:
  std::string rdbase = rdbase_cstr != nullptr ? rdbase_cstr : "";
  std::string normalizations;
  std::string acidbaseFile;
  std::string fragmentFile;
  std::string tautomerTransforms;
  int maxRestarts{200};  //!< The maximum number of times to attempt to apply
                         //!< the series of normalizations (default 200).
  bool preferOrganic{false};  //!< Whether to prioritize organic fragments when
                              //!< choosing fragment parent (default False).
  bool doCanonical{true};     //!< Whether to apply normalizations in a
                              //!< canonical order
  int maxTautomers{1000};     //!< The maximum number of tautomers to enumerate
                              //!< (default 1000).
  int maxTransforms{1000};    //!< The maximum number of tautomer
                              //!< transformations to apply (default 1000).
  bool tautomerRemoveSp3Stereo{
      true};  //!< Whether to remove stereochemistry from sp3 centers involved
              //!< in tautomerism (defaults to true)
  bool tautomerRemoveBondStereo{
      true};  //!< Whether to remove stereochemistry from double bonds involved
              //!< in tautomerism (defaults to true)
  bool tautomerRemoveIsotopicHs{
      true};  //!< Whether to remove isotopic Hs from centers involved in
              //!< tautomerism (defaults to true)
  bool tautomerReassignStereo{
      true};  //!< Whether enumerate() should call assignStereochemistry on all
              //!< generated tautomers (defaults to true)
  bool largestFragmentChooserUseAtomCount{
      true};  //!< Whether LargestFragmentChooser should use atom count as main
              //!< criterion before MW (defaults to true)
  bool largestFragmentChooserCountHeavyAtomsOnly{
      false};  //!< Whether LargestFragmentChooser should only count heavy atoms
               //!< (defaults to false)
  std::vector<std::pair<std::string, std::string>> normalizationData;
  std::vector<std::pair<std::string, std::string>> fragmentData;
  std::vector<std::tuple<std::string, std::string, std::string>> acidbaseData;
  std::vector<std::tuple<std::string, std::string, std::string, std::string>>
      tautomerTransformData;
  CleanupParameters() {}
};

RDKIX_MOLSTANDARDIZE_EXPORT extern const CleanupParameters
    defaultCleanupParameters;

RDKIX_MOLSTANDARDIZE_EXPORT void updateCleanupParamsFromJSON(
    CleanupParameters &params, const std::string &json);

//! The cleanup function is equivalent to the
/// molvs.Standardizer().standardize(mol) function. It calls the same steps,
/// namely: RemoveHs, RDKix SanitizeMol, MetalDisconnector, Normalizer,
/// Reionizer, RDKix AssignStereochemistry.
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *cleanup(
    const RWMol *mol,
    const CleanupParameters &params = defaultCleanupParameters);
//! \overload
inline RWMol *cleanup(const RWMol &mol, const CleanupParameters &params =
                                            defaultCleanupParameters) {
  return cleanup(&mol, params);
};

//! Works the same as Normalizer().normalize(mol)
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *normalize(
    const RWMol *mol,
    const CleanupParameters &params = defaultCleanupParameters);

//! Works the same as Reionizer().reionize(mol)
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *reionize(
    const RWMol *mol,
    const CleanupParameters &params = defaultCleanupParameters);

//! Works the same as FragmentRemover().remove(mol)
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *removeFragments(
    const RWMol *mol,
    const CleanupParameters &params = defaultCleanupParameters);

//! Works the same as TautomerEnumerator().canonicalize(mol)
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *canonicalTautomer(
    const RWMol *mol,
    const CleanupParameters &params = defaultCleanupParameters);

//! Returns the tautomer parent of a given molecule. The fragment parent is the
/// standardized canonical tautomer of the molecule
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *tautomerParent(
    const RWMol &mol,
    const CleanupParameters &params = defaultCleanupParameters,
    bool skipStandardize = false);

//! Returns the fragment parent of a given molecule. The fragment parent is the
/// largest organic covalent unit in the molecule.
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *fragmentParent(
    const RWMol &mol,
    const CleanupParameters &params = defaultCleanupParameters,
    bool skip_standardize = false);

//! calls removeStereochemistry() on the given molecule
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *stereoParent(
    const RWMol &mol,
    const CleanupParameters &params = defaultCleanupParameters,
    bool skip_standardize = false);

//! removes all isotopes specifications from the given molecule
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *isotopeParent(
    const RWMol &mol,
    const CleanupParameters &params = defaultCleanupParameters,
    bool skip_standardize = false);

//! Returns the charge parent of a given molecule. The charge parent is the
//! uncharged version of the fragment parent.
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *chargeParent(
    const RWMol &mol,
    const CleanupParameters &params = defaultCleanupParameters,
    bool skip_standardize = false);

//! Returns the super parent. The super parent is the fragment, charge, isotope,
//! stereo, and tautomer parent of the molecule.
RDKIX_MOLSTANDARDIZE_EXPORT RWMol *superParent(
    const RWMol &mol,
    const CleanupParameters &params = defaultCleanupParameters,
    bool skip_standardize = false);

//! Convenience function for quickly standardizing a single SMILES string.
/// Returns a standardized canonical SMILES string given a SMILES string.
/// This is the equivalent of calling cleanup() on each of the molecules
RDKIX_MOLSTANDARDIZE_EXPORT std::string standardizeSmiles(
    const std::string &smiles);

//! Do a disconnection of an organometallic complex according to rules
//! preferred by Syngenta.  All bonds to metals are broken, including
//! covalent bonds to Group I/II metals (so including Grignards, lithium
//! complexes etc.).  The ligands are left in the charge states they came
//! in with.  If there are haptic bonds defined by a dummy atom bonded to
//! a metal by a bond that has a _MolFileBondEndPts (which will contain the
//! indices of the atoms involved in the haptic bond) then the dummy atom
//! is removed also.
//! Do the disconnection in place.
//! The options are splitGrignards, splitAromaticC, adjustCharges and
//! removeHapticDummies.  Roll on C++20 and designated initializers!
RDKIX_MOLSTANDARDIZE_EXPORT void disconnectOrganometallics(
    RWMol &mol, RDKix::MolStandardize::MetalDisconnectorOptions mdo = {
                    true, true, false, true});
//! As above, but returns new disconnected molecule.
RDKIX_MOLSTANDARDIZE_EXPORT ROMol *disconnectOrganometallics(
    const ROMol &mol, RDKix::MolStandardize::MetalDisconnectorOptions mdo = {
                          true, true, false, true});

//! TODO
RDKIX_MOLSTANDARDIZE_EXPORT std::vector<std::string> enumerateTautomerSmiles(
    const std::string &smiles,
    const CleanupParameters &params = defaultCleanupParameters);
};  // namespace MolStandardize
}  // namespace RDKix
#endif
