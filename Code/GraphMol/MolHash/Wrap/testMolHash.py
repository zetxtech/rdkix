import os
import sys
import unittest

from rdkix import Chem, RDConfig
from rdkix.Chem import rdMolHash


class TestCase(unittest.TestCase):

  def setUp(self):
    pass

  def test1(self):
    m = Chem.MolFromSmiles('C1CCCC(O)C1c1ccnc(OC)c1')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.AnonymousGraph),
                     '***1****(*2*****2*)*1')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.ElementGraph),
                     'COC1CC(C2CCCCC2O)CCN1')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.CanonicalSmiles),
                     'COc1cc(C2CCCCC2O)ccn1')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.MurckoScaffold),
                     'c1cc(C2CCCCC2)ccn1')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.ExtendedMurcko),
                     '*c1cc(C2CCCCC2*)ccn1')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.MolFormula), 'C12H17NO2')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.AtomBondCounts), '15,16')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.DegreeVector), '0,4,9,2')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.Mesomer),
                     'CO[C]1[CH][C](C2CCCCC2O)[CH][CH][N]1_0')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.Regioisomer),
                     '*O.*O*.C.C1CCCCC1.c1ccncc1')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.NetCharge), '0')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.SmallWorldIndexBR), 'B16R2')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.SmallWorldIndexBRL), 'B16R2L9')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.ArthorSubstructureOrder),
                     '000f001001000c000300005f000000')

  def testTautomerV2(self):
    m = Chem.MolFromSmiles('CCC=O')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.HetAtomTautomer), "CC[CH][O]_0_0")
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.HetAtomTautomerv2),
                     "[CH3]-[C]:[C]:[O]_3_0")
    m = Chem.MolFromSmiles('CC=CO')
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.HetAtomTautomer),
                     "C[CH][CH][O]_1_0")
    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.HetAtomTautomerv2),
                     "[CH3]-[C]:[C]:[O]_3_0")

  def testCxSmiles(self):
    m = Chem.MolFromSmiles(
      'C[C@@H](O)[C@@H](C)[C@@H](C)C[C@H](C1=CN=CN1)C1=CNC=N1 |o1:8,5,&1:1,3,r,c:11,18,t:9,15|')

    self.assertEqual(rdMolHash.MolHash(m, rdMolHash.HashFunction.HetAtomTautomer),
                     'C[C@H](CC([C]1[CH][N][CH][N]1)[C]1[CH][N][CH][N]1)[C@@H](C)[C@H](C)[O]_3_0')

    self.assertEqual(
      rdMolHash.MolHash(m, rdMolHash.HashFunction.HetAtomTautomer, True),
      'C[C@H](CC([C]1[CH][N][CH][N]1)[C]1[CH][N][CH][N]1)[C@@H](C)[C@H](C)[O]_3_0 |o1:1,&1:14,16|')


if __name__ == "__main__":
  unittest.main()
