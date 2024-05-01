#ifndef _RDKIXIMPL_H_
#define _RDKIXIMPL_H_
#include "IRDKix.h"

/* Header file */
class RDKixImpl : public IRDKix {
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_IRDKIX

  RDKixImpl();

 private:
  ~RDKixImpl();

 protected:
  /* additional members */
};
#endif
