# $Id$
#
#  Copyright (C) 2003-2006  greg Landrum and Rational Discovery LLC
#
#   @@ All Rights Reserved @@
#  This file is part of the RDKix.
#  The contents are covered by the terms of the BSD license
#  which is included in the file license.txt, found at the root
#  of the RDKix source tree.
#
"""unit testing code for database connection objects

"""

import doctest
import os
import shutil
import tempfile
import traceback
import unittest

from rdkix import RDConfig
from rdkix.Dbase import StorageUtils


class TestCase(unittest.TestCase):

  def testDoctest(self):
    if RDConfig.useSqlLite:
      fd, tempName = tempfile.mkstemp(suffix='sqlt')
      StorageUtils.fd = fd
      StorageUtils.tempDbName = tempName
      shutil.copyfile(RDConfig.RDTestDatabase, StorageUtils.tempDbName)
    else:
      StorageUtils.tempDbName = '::RDTests'
    failed, _ = doctest.testmod(StorageUtils)
    if RDConfig.useSqlLite and os.path.exists(StorageUtils.tempDbName):
      os.close(StorageUtils.fd)
      try:
        os.unlink(StorageUtils.tempDbName)
      except Exception:
        traceback.print_exc()
    self.assertFalse(failed)

  def test_ValidateRDId(self):
    self.assertEqual(StorageUtils.ValidateRDId('RDCmpd-bogus-000-9'), False)


if __name__ == '__main__':  # pragma: nocover
  unittest.main()
