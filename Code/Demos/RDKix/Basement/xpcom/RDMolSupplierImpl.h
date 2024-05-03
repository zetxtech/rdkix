#ifndef _RDMOLSUPPLIERIMPL_H_
#define _RDMOLSUPPLIERIMPL_H_
#include "IRDKix.h"

namespace RDKix {
class ROMol;
class MolSupplier;
}  // namespace RDKix

class RDMolSupplier : public IRDMolSupplier {
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_IRDMOLSUPPLIER

  RDMolSupplier() : dp_suppl(0) {}
  RDMolSupplier(RDKix::MolSupplier *suppl) : dp_suppl(suppl) {}
  RDKix::MolSupplier *dp_suppl;

 private:
  ~RDMolSupplier();

 protected:
  /* additional members */
};

#endif
