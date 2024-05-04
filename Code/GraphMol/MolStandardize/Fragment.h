//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef RD_FRAGMENT_REMOVER_H
#define RD_FRAGMENT_REMOVER_H

#include <Catalogs/Catalog.h>
#include <GraphMol/MolStandardize/FragmentCatalog/FragmentCatalogEntry.h>
#include <GraphMol/MolStandardize/FragmentCatalog/FragmentCatalogParams.h>
#include <GraphMol/MolStandardize/MolStandardize.h>

namespace RDKix {
class ROMol;

namespace MolStandardize {

RDKIX_MOLSTANDARDIZE_EXPORT extern const CleanupParameters
    defaultCleanupParameters;

typedef RDCatalog::HierarchCatalog<FragmentCatalogEntry, FragmentCatalogParams,
                                   int>
    FragmentCatalog;

class RDKIX_MOLSTANDARDIZE_EXPORT FragmentRemover {
 public:
  FragmentRemover();
  FragmentRemover(const std::string fragmentFile, bool leave_last,
                  bool skip_if_all_match = false);
  FragmentRemover(std::istream &fragmentStream, bool leave_last,
                  bool skip_if_all_match = false);
  FragmentRemover(const std::vector<std::pair<std::string, std::string>> &data,
                  bool leave_last, bool skip_if_all_match = false);
  ~FragmentRemover();

  //! making FragmentRemover objects non-copyable
  FragmentRemover(const FragmentRemover &other) = delete;
  FragmentRemover &operator=(FragmentRemover const &) = delete;

  ROMol *remove(const ROMol &mol);

 private:
  // Setting leave_last to True will ensure at least one fragment
  //  is left in the molecule, even if it is matched by a
  //  FragmentPattern
  bool LEAVE_LAST;
  // If set, this causes the original molecule to be returned
  // if every fragment in it matches the salt list
  bool SKIP_IF_ALL_MATCH;
  FragmentCatalog *d_fcat;

};  // class FragmentRemover

// caller owns the returned pointer
inline FragmentRemover *fragmentRemoverFromParams(
    const CleanupParameters &params, bool leave_last = true,
    bool skip_if_all_match = false) {
  if (params.fragmentData.empty()) {
    return new FragmentRemover(params.fragmentFile, leave_last,
                               skip_if_all_match);
  } else {
    return new FragmentRemover(params.fragmentData, leave_last,
                               skip_if_all_match);
  }
}

class RDKIX_MOLSTANDARDIZE_EXPORT LargestFragmentChooser {
 public:
  //  LargestFragmentChooser() {}
  LargestFragmentChooser(bool preferOrganic = false)
      : preferOrganic(preferOrganic) {}
  LargestFragmentChooser(const CleanupParameters &params)
      : preferOrganic(params.preferOrganic),
        useAtomCount(params.largestFragmentChooserUseAtomCount),
        countHeavyAtomsOnly(params.largestFragmentChooserCountHeavyAtomsOnly) {}
  LargestFragmentChooser(const LargestFragmentChooser &other);
  ~LargestFragmentChooser() = default;

  ROMol *choose(const ROMol &mol);
  struct Largest {
    Largest();
    Largest(std::string &smiles, boost::shared_ptr<ROMol> fragment,
            unsigned int &numatoms, double &weight, bool &organic);
    std::string Smiles;
    boost::shared_ptr<ROMol> Fragment;
    unsigned int NumAtoms{0};
    double Weight{0};
    bool Organic{false};
  };

 private:
  bool preferOrganic;
  bool useAtomCount{true};
  bool countHeavyAtomsOnly{false};
};  // class LargestFragmentChooser
}  // namespace MolStandardize
}  // namespace RDKix

#endif
