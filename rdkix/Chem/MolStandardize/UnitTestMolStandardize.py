"""
unit tests for the MolStandardize module
tests include:
reorder_tautomers
"""

import unittest

from rdkix import Chem
from rdkix.Chem import MolStandardize
from rdkix.Chem.MolStandardize import rdMolStandardize


class TestCase(unittest.TestCase):

  def testBasic(self):
    m = Chem.MolFromSmiles('Oc1c(cccc3)c3nc2ccncc12')
    enumerator = rdMolStandardize.TautomerEnumerator()
    canon = enumerator.Canonicalize(m)
    reord = MolStandardize.ReorderTautomers(m)[0]
    canonSmile = Chem.MolToSmiles(canon)
    reordSmile = Chem.MolToSmiles(reord)
    self.assertEqual(canonSmile, reordSmile)

  def testLength(self):
    m = Chem.MolFromSmiles('Oc1c(cccc3)c3nc2ccncc12')
    enumerator = rdMolStandardize.TautomerEnumerator()
    tauts = enumerator.Enumerate(m)
    reordtauts = MolStandardize.ReorderTautomers(m)
    self.assertEqual(len(reordtauts), len(tauts))
