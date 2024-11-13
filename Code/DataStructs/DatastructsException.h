//
//  Copyright (C) 2005-2006 Rational Discovery LLC
//
//  @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//

#include <utility>

#include <RDGeneral/export.h>
#ifndef _DATASTRUCTS_EXCEPTION_H_20050126
#define _DATASTRUCTS_EXCEPTION_H_20050126

class RDKIX_DATASTRUCTS_EXPORT DatastructsException : public std::exception {
 public:
  //! construct with an error message
  DatastructsException(const char *msg) : _msg(msg) {}
  //! construct with an error message
  DatastructsException(std::string msg) : _msg(std::move(msg)) {}
  //! get the error message
  const char *what() const noexcept override { return _msg.c_str(); }
  ~DatastructsException() noexcept override = default;

 private:
  std::string _msg;
};

#endif
