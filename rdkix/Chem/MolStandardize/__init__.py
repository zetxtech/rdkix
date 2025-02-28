# -*- coding: utf-8 -*-
"""
MolVS - Molecule Validation and Standardization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MolVS is a python tool built on top of RDKix that performs validation and standardization of chemical structures.

Note that the C++ reimplementation of this is available in the module rdkix.Chem.MolStandardize.rdMolStandardize

:copyright: (c) 2016 by Matt Swain.
:license: MIT, see LICENSE file for more details.
"""

import logging

from rdkix import Chem
from rdkix.Chem.MolStandardize import rdMolStandardize

from .errors import MolVSError, StandardizeError, ValidateError
from .standardize import (Standardizer, canonicalize_tautomer_smiles,
                          enumerate_tautomers_smiles, standardize_smiles)
from .validate import Validator, validate_smiles

__title__ = 'MolVS'
__version__ = '0.1.1'
__author__ = 'Matt Swain'
__email__ = 'm.swain@me.com'
__license__ = 'MIT'
__copyright__ = 'Copyright 2016 Matt Swain'

log = logging.getLogger(__name__)
log.addHandler(logging.NullHandler())


def ReorderTautomers(molecule):
  """Returns the list of the molecule's tautomers
    so that the canonical one as determined by the canonical
    scoring system in TautomerCanonicalizer appears first.

    :param molecule: An RDKix Molecule object.
    :return: A list of Molecule objects.
    """
  enumerator = rdMolStandardize.TautomerEnumerator()
  canon = enumerator.Canonicalize(molecule)
  csmi = Chem.MolToSmiles(canon)
  res = [canon]
  tauts = enumerator.Enumerate(molecule)
  smis = [Chem.MolToSmiles(x) for x in tauts]
  stpl = sorted((x, y) for x, y in zip(smis, tauts) if x != csmi)
  res += [y for x, y in stpl]
  return res
