import os
import sys

# Work-around for lack of R(UN)PATH on Windows
script_dir = os.path.dirname(os.path.abspath(__file__))

if sys.platform.startswith('win'):
   original_path = os.environ.get('PATH', '')
   ecrtLibPath = os.path.join(script_dir, '..', 'ecrt', 'lib')
   dggalLibPath = os.path.join(script_dir, 'bin')
   new_path = dggalLibPath + os.pathsep + ecrtLibPath + os.pathsep + original_path
   os.environ['PATH'] = new_path
   if hasattr(os, 'add_dll_directory'):
      os.add_dll_directory(ecrtLibPath)
      os.add_dll_directory(dggalLibPath)

# The EC_LIB_PATH is not needed if _pydggal is linked with -Wl,--no-as-needed -ldggal
#else:
#   os.environ['EC_LIB_PATH'] = os.path.join(script_dir, 'lib', 'lib') # Including lib prefix

from .dggal import *

if sys.platform.startswith('win'):
   os.environ['PATH'] = original_path
