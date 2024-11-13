# additional pytest configuration

collect_ignore = []
collect_ignore_glob = []
try:
  import win32com
except ImportError:
  collect_ignore.extend(['Chem/DSViewer.py'])

try:
  from rdkix.Avalon import pyAvalonTools
except ImportError:
  collect_ignore_glob.extend(['**Chem/MolKey/*.py'])

from rdkix.Chem import inchi
if not inchi.INCHI_AVAILABLE:
  collect_ignore_glob.extend(['**Chem/MolKey/*.py'])

try:
  import IPython
except ImportError:
  collect_ignore.extend([
    'Chem/Draw/IPythonConsole.py', 'Chem/Draw/InteractiveRenderer.py',
    'Chem/Draw/UnitTestIPython.py'
  ])
