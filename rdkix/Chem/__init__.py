#
#  Copyright (C) 2000-2017  greg Landrum and other RDKix contributors
#
#   @@ All Rights Reserved @@
#  This file is part of the RDKix.
#  The contents are covered by the terms of the BSD license
#  which is included in the file license.txt, found at the root
#  of the RDKix source tree.
#
""" A module for molecules and stuff

 see Chem/index.html in the doc tree for documentation

"""
from rdkix import DataStructs, RDConfig, rdBase
from rdkix.Chem import rdchem
from rdkix.Geometry import rdGeometry

_HasSubstructMatchStr = rdchem._HasSubstructMatchStr
from rdkix.Chem.inchi import *
from rdkix.Chem.rdchem import *
from rdkix.Chem.rdCIPLabeler import *
from rdkix.Chem.rdmolfiles import *
from rdkix.Chem.rdmolops import *

try:
  # This is an optional component of the build
  from rdkix.Chem.rdMolInterchange import *
except ImportError:
  pass

# Coordgen needs to know where its template file is.
# The default install puts it in RDDataDir
try:
  from rdkix.Chem import rdCoordGen
except ImportError:
  pass
else:
  templDir = RDConfig.RDDataDir
  if templDir[-1] != '/':
    templDir += '/'
  rdCoordGen.SetDefaultTemplateFileDir(templDir)


def QuickSmartsMatch(smi, sma, unique=True, display=False):
  m = MolFromSmiles(smi)
  p = MolFromSmarts(sma)
  res = m.GetSubstructMatches(p, unique)
  if display:
    pass
  return res


def CanonSmiles(smi, useChiral=1):
  m = MolFromSmiles(smi)
  return MolToSmiles(m, useChiral)


def SupplierFromFilename(fileN, delim='', **kwargs):
  ext = fileN.split('.')[-1].lower()
  if ext == 'sdf':
    suppl = SDMolSupplier(fileN, **kwargs)
  elif ext == 'csv':
    if not delim:
      delim = ','
    suppl = SmilesMolSupplier(fileN, delimiter=delim, **kwargs)
  elif ext == 'txt':
    if not delim:
      delim = '\t'
    suppl = SmilesMolSupplier(fileN, delimiter=delim, **kwargs)
  elif ext == 'tdt':
    suppl = TDTMolSupplier(fileN, delimiter=delim, **kwargs)
  else:
    raise ValueError("unrecognized extension: %s" % ext)

  return suppl


def FindMolChiralCenters(mol, force=True, includeUnassigned=False, includeCIP=True,
                         useLegacyImplementation=True):
  """
    >>> from rdkix import Chem
    >>> mol = Chem.MolFromSmiles('[C@H](Cl)(F)Br')
    >>> Chem.FindMolChiralCenters(mol)
    [(0, 'R')]
    >>> mol = Chem.MolFromSmiles('[C@@H](Cl)(F)Br')
    >>> Chem.FindMolChiralCenters(mol)
    [(0, 'S')]

    >>> Chem.FindMolChiralCenters(Chem.MolFromSmiles('CCC'))
    []

    By default unassigned stereo centers are not reported:

    >>> mol = Chem.MolFromSmiles('C[C@H](F)C(F)(Cl)Br')
    >>> Chem.FindMolChiralCenters(mol,force=True)
    [(1, 'S')]

    but this can be changed:

    >>> Chem.FindMolChiralCenters(mol,force=True,includeUnassigned=True)
    [(1, 'S'), (3, '?')]

    The handling of unassigned stereocenters for dependent stereochemistry is not correct 
    using the legacy implementation:

    >>> Chem.FindMolChiralCenters(Chem.MolFromSmiles('C1CC(C)C(C)C(C)C1'),includeUnassigned=True)
    [(2, '?'), (6, '?')]
    >>> Chem.FindMolChiralCenters(Chem.MolFromSmiles('C1C[C@H](C)C(C)[C@H](C)C1'),includeUnassigned=True)
    [(2, 'S'), (4, '?'), (6, 'R')]

    But works with the new implementation:

    >>> Chem.FindMolChiralCenters(Chem.MolFromSmiles('C1CC(C)C(C)C(C)C1'),includeUnassigned=True, useLegacyImplementation=False)
    [(2, '?'), (4, '?'), (6, '?')]

    Note that the new implementation also gets the correct descriptors for para-stereochemistry:

    >>> Chem.FindMolChiralCenters(Chem.MolFromSmiles('C1C[C@H](C)[C@H](C)[C@H](C)C1'),useLegacyImplementation=False)
    [(2, 'S'), (4, 's'), (6, 'R')]

    With the new implementation, if you don't care about the CIP labels of stereocenters, you can save
    some time by disabling those:

    >>> Chem.FindMolChiralCenters(Chem.MolFromSmiles('C1C[C@H](C)[C@H](C)[C@H](C)C1'), includeCIP=False, useLegacyImplementation=False)
    [(2, 'Tet_CCW'), (4, 'Tet_CCW'), (6, 'Tet_CCW')]

  """
  if useLegacyImplementation:
    AssignStereochemistry(mol, force=force, flagPossibleStereoCenters=includeUnassigned)
    centers = []
    for atom in mol.GetAtoms():
      if atom.HasProp('_CIPCode'):
        centers.append((atom.GetIdx(), atom.GetProp('_CIPCode')))
      elif includeUnassigned and atom.HasProp('_ChiralityPossible'):
        centers.append((atom.GetIdx(), '?'))
  else:
    centers = []
    itms = FindPotentialStereo(mol)
    if includeCIP:
      atomsToLabel = []
      bondsToLabel = []
      for si in itms:
        if si.type == StereoType.Atom_Tetrahedral:
          atomsToLabel.append(si.centeredOn)
        elif si.type == StereoType.Bond_Double:
          bondsToLabel.append(si.centeredOn)
      AssignCIPLabels(mol, atomsToLabel=atomsToLabel, bondsToLabel=bondsToLabel)
    for si in itms:
      if si.type == StereoType.Atom_Tetrahedral and (includeUnassigned
                                                     or si.specified == StereoSpecified.Specified):
        idx = si.centeredOn
        atm = mol.GetAtomWithIdx(idx)
        if includeCIP and atm.HasProp("_CIPCode"):
          code = atm.GetProp("_CIPCode")
        else:
          if si.specified:
            code = str(si.descriptor)
          else:
            code = '?'
            atm.SetIntProp('_ChiralityPossible', 1)
        centers.append((idx, code))
  return centers


#------------------------------------
#
#  doctest boilerplate
#
def _test():
  import doctest
  import sys
  return doctest.testmod(sys.modules["__main__"])


#------------------------------------
#
#  patcher
#

class ConversionError(Exception):
  pass

def _patch():
  def to_rdkit(self):
    try:
      import pickle
      bx = pickle.dumps(self)
      b = bx.replace(b'rdkix.Chem.rdchem', b'rdkit.Chem.rdchem')
      return pickle.loads(b)
    except (pickle.UnpicklingError, ModuleNotFoundError):
      raise ConversionError('Fail to convert rdkix molecule to rdkit (either rdkit is not installed or version difference is not tolerated), please use smiles/smarts instead.')    
  Mol.to_rdkit = to_rdkit
  
_patch()

if __name__ == '__main__':
  import sys
  failed, tried = _test()
  sys.exit(failed)
