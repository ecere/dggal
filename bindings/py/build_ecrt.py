import sys
import os
import platform
from distutils.util import get_platform;
from os import path

dggalDir = path.join(os.getcwd(), '../../')
esdkDir = path.join(dggalDir, '../eC/')

if dggalDir.find('pip-req-build-') == -1 or dggalDir.find('pip-install-of') == -1:
   inPipInstallBuild = False #True
else:
   inPipInstallBuild = False

# print(' -- before ecrt extension build -- ')
pver = platform.python_version()
# print('arg zero: ', sys.argv[0])
# print('count: ', len(sys.argv))
# print('args: ', str(sys.argv))
if inPipInstallBuild == True:
   blddir = 'build/lib.%s-%s' % (get_platform(), pver[0:pver.rfind('.')])
else:
   blddir = 'bindings/py'
from cffi import FFI

dnf = path.dirname(__file__)
dir = path.abspath(path.dirname(__file__))
owd = os.getcwd()
cpath = path.join('..', 'c')
if os.path.isdir(cpath) != True:
   cpath = path.join(dggalDir, 'bindings', 'c')
   if os.path.isdir(cpath) != True:
      print('error: unable to find path to C bindings!')
rel = '' if os.path.isfile(os.path.join(owd, 'build_ecrt.py')) == True else path.join('bindings', 'py')
sysdir = 'win32' if sys.platform == 'win32' else 'linux'
syslibdir = 'bin' if sys.platform == 'win32' else 'lib'
incdir = path.join(dggalDir, 'bindings', 'c')
if rel == '':
   libdir = path.join('..', '..', 'obj', sysdir, syslibdir)
else:
   libdir = path.join('obj', sysdir, syslibdir)

if os.path.isfile(path.join(rel, 'cffi-ecrt.h')) != True:
   print('problem! -- owd:', owd, ' rel:', rel)

if dnf != '':
   os.chdir(dir)

_CF_DIR = str(os.getenv('_CF_DIR'))
_cf_dir = False if _CF_DIR == '' else True
if _cf_dir == True:
   _c_bindings_path, _ = os.path.split(dir)
   _c_bindings_path, _ = os.path.split(_c_bindings_path)
   sys.path.append(path.realpath(path.join(dir, '../../', _CF_DIR, 'bindings/py')))
if rel != '':
   sys.path.append(dir)

def cdefpath(filename):
   fullpath = path.realpath(path.join(owd, rel, filename))
   if path.isfile(fullpath):
      return fullpath
   if _cf_dir == True:
      fullpath = path.join(dir, _CF_DIR, 'bindings/py', filename)
      fullpath = path.realpath(fullpath)
      if path.isfile(fullpath):
         return fullpath
   return 'badpath'

ffi_ecrt = FFI()
ffi_ecrt.cdef(open(cdefpath('cffi-ecrt.h')).read())
PY_BINDINGS_EMBEDDED_C_DISABLE = os.getenv('PY_BINDINGS_EMBEDDED_C_DISABLE')
_embedded_c = True # False if PY_BINDINGS_EMBEDDED_C_DISABLE == '' else True

srcs = []
if _embedded_c == True:
   srcs.append(path.join(cpath, 'ecrt.c'))

libs = []

libs.append('ecrt')
if _embedded_c == False:
   libs.append('ecrt_c')
ffi_ecrt.set_source('_pyecrt',
               '#include "ecrt.h"',
               sources=srcs,
               define_macros=[('BINDINGS_SHARED', None), ('ECRT_EXPORT', None)],
               extra_compile_args=['-DECPRFX=eC_', '-DMS_WIN64', '-Wl,--export-dynamic', '-O2'],
               include_dirs=[path.join(owd, rel), incdir],
               libraries=libs,
               extra_link_args=["-Wl,-rpath,$ORIGIN/lib,-rpath,$ORIGIN/ecrt/lib",],
               library_dirs=[path.join(owd, libdir), path.join(esdkDir, 'obj', sysdir, syslibdir)],
               py_limited_api=False)
if __name__ == '__main__':
   V = os.getenv('V')
   v = True if V == '1' or V == 'y' else False
   ffi_ecrt.compile(verbose=v,tmpdir='.',debug=False)

if dnf != '':
   os.chdir(owd)
