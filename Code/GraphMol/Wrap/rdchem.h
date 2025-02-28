//
//  Copyright (C) 2003-2006 Rational Discovery LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include <RDGeneral/export.h>
#ifndef _RDCHEM_INCL_
#define _RDCHEM_INCL_

#define PY_ARRAY_UNIQUE_SYMBOL rdchem_array_API

namespace RDKix {
class ConformerException;
}
void rdExceptionTranslator(RDKix::ConformerException const& x);

#endif
