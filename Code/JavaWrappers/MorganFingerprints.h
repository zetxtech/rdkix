
#include <RDGeneral/export.h>
#include <GraphMol/Atom.h>
#include <GraphMol/GraphMol.h>
#include <GraphMol/Fingerprints/MorganFingerprints.h>
#include <DataStructs/BitVects.h>
#include <vector>

RDKix::SparseIntVect<std::uint32_t> *getFeatureFingerprint(
    const RDKix::ROMol &mol, unsigned int radius, bool useChirality = false,
    bool useBondTypes = true, bool useCounts = true) {
  std::vector<std::uint32_t> *invars =
      new std::vector<std::uint32_t>(mol.getNumAtoms());
  RDKix::MorganFingerprints::getFeatureInvariants(mol, *invars);
  RDKix::SparseIntVect<std::uint32_t> *res =
      RDKix::MorganFingerprints::getFingerprint(
          mol, static_cast<unsigned int>(radius), invars, 0, useChirality,
          useBondTypes, useCounts, false, 0);
  delete invars;
  return res;
}
