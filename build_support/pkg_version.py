from __future__ import print_function

import os
import re
from datetime import datetime

from setuptools import setup

pkg_version = ''
src_dir = os.path.realpath(__file__)
have_src_dir = os.path.isfile(src_dir)
i = 0
while (i < 2 and have_src_dir):
  i += 1
  src_dir = os.path.dirname(src_dir)
  have_src_dir = os.path.isdir(src_dir)
if (not have_src_dir):
  raise OSError('Could not find SRC_DIR, got: ' + str(src_dir))
# parse root CMakeLists.txt and Code/cmake/Modules/RDKixUtils.cmake
root_cmakelists_path = os.path.join(src_dir, 'CMakeLists.txt')
rdkixutils_path = os.path.join(src_dir, 'Code', 'cmake', 'Modules', 'RDKixUtils.cmake')

var_dict = {}
for file in (root_cmakelists_path, rdkixutils_path):
  with open(file, 'rt') as hnd:
    # vars we want to read
    var_set = set(['RDKix_Year', 'RDKix_Month', 'RDKix_Revision', 'RDKix_ABI', 'RDKix_RELEASENAME'])
    line = hnd.readline()
    while (line):
      # is this an uncommented set command?
      m = re.match(r'^\s*set\s*\((\w+)\s*\"(.*)\"\s*\)', line, re.IGNORECASE)
      # if it is
      if (m is not None):
        # extract the var name
        var_name = m.group(1)
        if (var_name in var_set):
          # if the var name is in the vars we want to read
          var_value = m.group(2)
          keepLooping = True
          while (keepLooping):
            # recursively replace variables we already found
            m = re.match(r'^.*\${(\w+)}', var_value)
            keepLooping = (m is not None)
            if (keepLooping):
              v = var_dict.get(m.group(1))
              # if the variable is not defined, remove
              # the preceding dot
              eat_dot = 0
              if (v is None):
                v = ''
                eat_dot = 1
              # replace variable name with its value
              s = m.start(1) - (2 + eat_dot)
              e = m.end(1) + 1
              var_value = var_value[:s] + v + var_value[e:]
          # assign variable value and keep parsing
          var_dict[var_name] = var_value
      line = hnd.readline()
d = datetime.today().strftime('%Y%m%d')
rdkixVersion = var_dict.get('RDKix_RELEASENAME')
if (rdkixVersion is not None):
  if rdkixVersion.endswith('.dev1'):
    pkg_version = rdkixVersion[:-1] + d
  else:
    pkg_version = rdkixVersion
else:
  # if extracting rdkixVersion somehow failed, use the date
  pkg_version = d

print('rdkixVersion:', pkg_version)
setup(rdkixVersion=pkg_version)
