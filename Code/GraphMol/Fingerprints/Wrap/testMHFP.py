"""
2019, Daniel Probst, Reymond Group @ University of Bern
 @@ All Rights Reserved @@
This file is part of the RDKix.
The contents are covered by the terms of the BSD license
which is included in the file license.txt, found at the root
of the RDKix source tree.
"""

import unittest

from rdkix import Chem
from rdkix.Chem import rdMHFPFingerprint


class TestCase(unittest.TestCase):

  def setUp(self):
    pass

  def testMHFPDistance(self):
    s = "CN1C=NC2=C1C(=O)N(C(=O)N2C)C"
    enc = rdMHFPFingerprint.MHFPEncoder(128, 42)
    fp = enc.EncodeSmiles(s)
    dist = enc.Distance(fp, fp)
    self.assertEqual(dist, 0.0)

  def testMHFPFingerprint(self):
    s = "CN1C=NC2=C1C(=O)N(C(=O)N2C)C"
    t = "Cn1cnc2c1c(=O)[nH]c(=O)n2C"

    m = Chem.MolFromSmiles(s)
    enc = rdMHFPFingerprint.MHFPEncoder(128, 42)

    self.assertEqual(len(enc.CreateShinglingFromSmiles(s, rings=False)), 42)
    self.assertEqual(len(enc.CreateShinglingFromSmiles(s, min_radius=0)), 58)

    sh_a = enc.CreateShinglingFromSmiles(s)
    sh_b = enc.CreateShinglingFromMol(m)

    self.assertEqual(len(sh_a), 44)
    self.assertEqual(list(sh_a), list(sh_b))

    fp_a = enc.EncodeSmiles(s)
    fp_b = enc.EncodeMol(m)

    self.assertEqual(list(fp_a), list(fp_b))

    fp_c = enc.EncodeSmiles(t)
    dist = rdMHFPFingerprint.MHFPEncoder.Distance(fp_a, fp_c)
    self.assertEqual(dist, 0.5390625)


if __name__ == "__main__":
  unittest.main()
