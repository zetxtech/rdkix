// $Id: GenericRDKixException.h
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

// A generic exception (to cause a corresponding one to be created in Java)

#include <RDGeneral/export.h>
#include <exception>
#include <string>

// RDKIX_JAVAWRAPPERS_EXPORT does not get defined in RDGeneral/export.h,
// and we only use it here for non-windows builds, so just define it based
// on RDKIX_RDGENERAL_EXPORT
#if defined(RDKIX_DYN_LINK) && defined(WIN32) && defined(BOOST_HAS_DECLSPEC)
#define RDKIX_JAVAWRAPPERS_EXPORT
#else
#define RDKIX_JAVAWRAPPERS_EXPORT RDKIX_RDGENERAL_EXPORT
#endif

namespace RDKix {

class RDKIX_JAVAWRAPPERS_EXPORT GenericRDKixException : public std::exception {
 public:
  GenericRDKixException(const std::string &i) : _value(i) {}
  GenericRDKixException(const char *msg) : _value(msg) {}
  const char *what() const noexcept override { return _value.c_str(); }
  ~GenericRDKixException() noexcept = default;

 private:
  std::string _value;
};
}  // namespace RDKix
