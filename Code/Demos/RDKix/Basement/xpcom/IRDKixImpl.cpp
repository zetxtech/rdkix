#include "RDKixImpl.h"
#include "RDMolImpl.h"
#include "RDMolSupplierImpl.h"
#include "nsIGenericFactory.h"
#include "nsISupportsUtils.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(RDKixImpl);
NS_GENERIC_FACTORY_CONSTRUCTOR(RDMolecule);
NS_GENERIC_FACTORY_CONSTRUCTOR(RDMolSupplier);

static const nsModuleComponentInfo components[] = {
    {"RDKix Interface", IRDKIX_IID, "@rationaldiscovery.com/RDKix/base",
     RDKixImplConstructor},
    {"RDKix Molecule Interface", IRDMOLECULE_IID,
     "@rationaldiscovery.com/RDKix/molecule", RDMoleculeConstructor},
    {"RDKix Molecule Supplier Interface", IRDMOLSUPPLIER_IID,
     "@rationaldiscovery.com/RDKix/molsupplier", RDMolSupplierConstructor},

};

NS_IMPL_NSGETMODULE(nsRDKixModule, components);
