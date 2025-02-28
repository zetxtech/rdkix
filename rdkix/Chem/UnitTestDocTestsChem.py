import doctest
import unittest

from rdkix import Chem
from rdkix.Chem import (BRICS, AllChem, Descriptors, EnumerateHeterocycles,
                        EnumerateStereoisomers, FragmentMatcher, MACCSkeys,
                        PropertyMol, Recap, SaltRemover, TemplateAlign,
                        rdDepictor)


def load_tests(loader, tests, ignore):  # pylint: disable=unused-argument
  """ Add the Doctests from the module """
  tests.addTests(doctest.DocTestSuite(EnumerateStereoisomers, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(EnumerateHeterocycles, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(FragmentMatcher, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(MACCSkeys, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(Descriptors, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(Recap, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(BRICS, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(AllChem, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(PropertyMol, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(SaltRemover, optionflags=doctest.ELLIPSIS))
  tests.addTests(doctest.DocTestSuite(Chem, optionflags=doctest.ELLIPSIS))
  # Tests which have a dependency on using the RDKix coordinate generator
  rdDepictor.SetPreferCoordGen(False)
  tests.addTests(doctest.DocTestSuite(TemplateAlign, optionflags=doctest.ELLIPSIS))

  return tests


if __name__ == '__main__':  # pragma: nocover
  unittest.main()
