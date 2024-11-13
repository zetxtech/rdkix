#include "RDKixImpl.h"
#include "RDMolImpl.h"
#include "RDMolSupplierImpl.h"
#include <GraphMol/RDKixBase.h>
#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/FileParsers/MolSupplier.h>
#include <string>

/* Implementation file */
NS_IMPL_ISUPPORTS1(RDKixImpl, IRDKix)

RDKixImpl::RDKixImpl() { /* member initializers and constructor code */ }

RDKixImpl::~RDKixImpl() { /* destructor code */ }

/* unsigned long strlen (in string arg); */
NS_IMETHODIMP RDKixImpl::Strlen(const char *arg, PRUint32 *_retval) {
  std::string text(arg);
  *_retval = text.size();
  return NS_OK;
}

/* IRDMolecule MolFromSmiles (in string smiles); */
NS_IMETHODIMP RDKixImpl::MolFromSmiles(const char *smiles,
                                       IRDMolecule **_retval) {
  std::string smi(smiles);
  RDKix::ROMol *roMol = RDKix::SmilesToMol(smiles);
  if (!roMol) return NS_ERROR_FAILURE;

  RDMolecule *mol = new RDMolecule(roMol);
  if (!mol) return NS_ERROR_OUT_OF_MEMORY;
  *_retval = static_cast<IRDMolecule *>(mol);

  // FIX: does this leak?
  NS_ADDREF(*_retval);

  return NS_OK;
}

/* IRDMolecule MolFromMolBlock (in string molBlock); */
NS_IMETHODIMP RDKixImpl::MolFromMolBlock(const char *molBlock,
                                         IRDMolecule **_retval) {
  RDKix::ROMol *roMol = RDKix::MolBlockToMol(std::string(molBlock));
  if (!roMol) return NS_ERROR_FAILURE;

  RDMolecule *mol = new RDMolecule(roMol);
  if (!mol) return NS_ERROR_OUT_OF_MEMORY;
  *_retval = static_cast<IRDMolecule *>(mol);

  // FIX: does this leak?
  NS_ADDREF(*_retval);

  return NS_OK;
}

/* IRDMolSupplier SupplierFromSDFile (in string fileName); */
NS_IMETHODIMP RDKixImpl::SupplierFromSDFile(const char *fileName,
                                            IRDMolSupplier **_retval) {
  RDKix::MolSupplier *sdSuppl = new RDKix::SDMolSupplier(std::string(fileName));
  if (!sdSuppl) return NS_ERROR_FAILURE;

  RDMolSupplier *suppl = new RDMolSupplier(sdSuppl);
  if (!suppl) return NS_ERROR_OUT_OF_MEMORY;
  *_retval = static_cast<IRDMolSupplier *>(suppl);

  // FIX: does this leak?
  NS_ADDREF(*_retval);

  return NS_OK;
}
