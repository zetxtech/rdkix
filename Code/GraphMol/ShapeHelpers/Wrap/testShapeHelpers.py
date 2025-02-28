import math
import os
import sys
import unittest

from rdkix import Chem, DataStructs, RDConfig
from rdkix.Chem import rdMolAlign
from rdkix.Chem import rdMolTransforms as rdmt
from rdkix.Chem import rdShapeHelpers as rdshp
from rdkix.Geometry import rdGeometry as geom


class TestCase(unittest.TestCase):

  def setUp(self):
    pass

  def test1Shape(self):
    fileN = os.path.join(RDConfig.RDBaseDir, 'Code', 'GraphMol', 'ShapeHelpers', 'test_data',
                         '1oir.mol')
    m = Chem.MolFromMolFile(fileN)
    rdmt.CanonicalizeMol(m)
    dims1, offset1 = rdshp.ComputeConfDimsAndOffset(m.GetConformer())
    grd = geom.UniformGrid3D(30.0, 16.0, 10.0)
    rdshp.EncodeShape(m, grd, 0)
    ovect = grd.GetOccupancyVect()
    self.assertEqual(ovect.GetTotalVal(), 7405)

    m = Chem.MolFromMolFile(fileN)
    trans = rdmt.ComputeCanonicalTransform(m.GetConformer())
    dims, offset = rdshp.ComputeConfDimsAndOffset(m.GetConformer(), trans=trans)
    dims -= dims1
    offset -= offset1
    self.assertAlmostEqual(dims.Length(), 0.0, 4)
    self.assertAlmostEqual(offset.Length(), 0.0, 4)

    grd1 = geom.UniformGrid3D(30.0, 16.0, 10.0)
    rdshp.EncodeShape(m, grd1, 0, trans)
    ovect = grd1.GetOccupancyVect()

    self.assertEqual(ovect.GetTotalVal(), 7405)

    grd2 = geom.UniformGrid3D(30.0, 16.0, 10.0)
    rdshp.EncodeShape(m, grd2, 0)

    fileN2 = os.path.join(RDConfig.RDBaseDir, 'Code', 'GraphMol', 'ShapeHelpers', 'test_data',
                          '1oir_conf.mol')
    m2 = Chem.MolFromMolFile(fileN2)

    self.assertAlmostEqual(rdshp.ShapeTanimotoDist(m, m), 0.0, 4)
    self.assertAlmostEqual(rdshp.ShapeTverskyIndex(m, m, 1.0, 1.0), 1.0, 4)

    rmsd = rdMolAlign.AlignMol(m, m2)
    self.assertAlmostEqual(rdshp.ShapeTanimotoDist(m, m2), 0.31, 2)
    self.assertAlmostEqual(rdshp.ShapeTverskyIndex(m, m2, 1.0, 1.0), 0.686, 2)

    dist = rdshp.ShapeTanimotoDist(mol1=m, mol2=m2, confId1=0, confId2=0, gridSpacing=0.25,
                                   stepSize=0.125)
    self.assertAlmostEqual(dist, 0.339, 2)

    m = Chem.MolFromMolFile(fileN)
    cpt = rdmt.ComputeCentroid(m.GetConformer())
    dims, offset = rdshp.ComputeConfDimsAndOffset(m.GetConformer())

    grd = geom.UniformGrid3D(dims.x, dims.y, dims.z, 0.5, DataStructs.DiscreteValueType.TWOBITVALUE,
                             offset)
    dims -= geom.Point3D(13.927, 16.97, 9.775)
    offset -= geom.Point3D(-4.353, 16.829, 2.782)
    self.assertAlmostEqual(dims.Length(), 0.0, 4)
    self.assertAlmostEqual(offset.Length(), 0.0, 4)
    rdshp.EncodeShape(m, grd, 0)

    ovect = grd.GetOccupancyVect()

    self.assertEqual(ovect.GetTotalVal(), 7417)
    geom.WriteGridToFile(grd, '1oir_shape.grd')

    m = Chem.MolFromMolFile(fileN)
    lc, uc = rdshp.ComputeConfBox(m.GetConformer())
    rdmt.CanonicalizeMol(m)
    lc1, uc1 = rdshp.ComputeConfBox(m.GetConformer())

    lc2, uc2 = rdshp.ComputeUnionBox((lc, uc), (lc1, uc1))
    lc -= geom.Point3D(-4.353, 16.829, 2.782)
    uc -= geom.Point3D(9.574, 33.799, 12.557)
    self.assertAlmostEqual(lc.Length(), 0.0, 4)
    self.assertAlmostEqual(uc.Length(), 0.0, 4)

    lc1 -= geom.Point3D(-10.7519, -6.0778, -3.0123)
    uc1 -= geom.Point3D(8.7163, 5.3279, 3.1621)
    self.assertAlmostEqual(lc1.Length(), 0.0, 4)
    self.assertAlmostEqual(uc1.Length(), 0.0, 3)

    lc2 -= geom.Point3D(-10.7519, -6.0778, -3.01226)
    uc2 -= geom.Point3D(9.574, 33.799, 12.557)
    self.assertAlmostEqual(lc2.Length(), 0.0, 4)
    self.assertAlmostEqual(uc2.Length(), 0.0, 4)


if __name__ == '__main__':
  print("Testing Shape Helpers wrapper")
  unittest.main()
