import sys
import os
import platform
from distutils.util import get_platform;
from os import path
from cffi import FFI
from distutils.sysconfig import get_config_var
import pkg_resources

owd = os.getcwd()

if path.isfile('cffi-dggal.h'):
   bindings_py_dir = '.'
else:
   bindings_py_dir = path.join('bindings', 'py')
   if not path.isfile(bindings_py_dir):
      bindings_py_dir = path.join(owd, 'dggal', 'bindings', 'py')

dnf = path.dirname(__file__)
dir = path.abspath(path.dirname(__file__))

cpath = os.path.normpath(path.join(dnf, '..', 'c'))

incdir = cpath

if path.isdir(cpath) != True:
   print('error: unable to find path to C bindings!')
if path.isfile(path.join(bindings_py_dir, 'cffi-dggal.h')) != True:
   print('Cannot find cffi-dggal.h in', bindings_py_dir)

sysdir = 'win32' if sys.platform == 'win32' else 'linux'
syslibdir = 'bin' if sys.platform == 'win32' else 'lib'
libdir = path.join(bindings_py_dir, '..', '..', 'obj', sysdir, syslibdir)

if dnf != '':
   os.chdir(dir)

sys.path.append(bindings_py_dir)

ext = '.so' if get_config_var('EXT_SUFFIX') is None else get_config_var('EXT_SUFFIX')

try:
   ecdev_location = os.path.join(pkg_resources.get_distribution("ecdev").location, 'ecdev')
   incdir_ecrt = os.path.join(ecdev_location, 'include')
   ecrt_location = os.path.join(ecdev_location, syslibdir)
except:
   incdir_ecrt = os.path.join(bindings_py_dir, '..', '..', '..', 'eC', 'bindings', 'py')
   ecrt_location = os.path.join(bindings_py_dir, '..', '..', '..', 'eC', 'obj', sysdir, syslibdir)
# ecrt_location = os.path.join(pkg_resources.get_distribution("ecrt").location, 'ecrt', '.lib')

ffi_ecrt = FFI()
ffi_ecrt.cdef(open(path.join(incdir_ecrt, 'cffi-ecrt.h')).read())

extra_link_args = ["-Wl,-rpath,$ORIGIN/lib:$ORIGIN/ecrt/lib"]
if sys.platform == 'win32':
   extra_link_args.append('-Wl,--export-all-symbols')
else:
   extra_link_args.append('-Wl,--export-dynamic')

ffi_ecrt.set_source('_pyecrt',
               '#include "ecrt.h"',
               sources=[],
               define_macros=[('BINDINGS_SHARED', None), ('ECRT_EXPORT', None)],
               extra_compile_args=['-DECPRFX=eC_', '-DMS_WIN64', '-Wl,--export-dynamic', '-O2'],
               include_dirs=[incdir_ecrt],
               libraries=[],
               # _py* CFFI packages are currently being packaged outside of the main extension directory
               extra_link_args=extra_link_args,
               library_dirs=[libdir],
               py_limited_api=False)

ffi_dggal = FFI()
ffi_dggal.include(ffi_ecrt)
ffi_dggal.cdef(open(path.join(bindings_py_dir, 'cffi-dggal.h')).read())
PY_BINDINGS_EMBEDDED_C_DISABLE = os.getenv('PY_BINDINGS_EMBEDDED_C_DISABLE')
_embedded_c = True # False if PY_BINDINGS_EMBEDDED_C_DISABLE == '' else True

srcs = []
if _embedded_c == True:
   srcs.append(path.join(cpath, 'dggal.c'))

libs = []

libs.append('ecrt')
#libs.append('dggal') # Adding dggal here doesn't seem to work with -Wl,--no-as-needed to force dependency ensuring dlopen() will find DGGAL using RPATH
if _embedded_c == False:
   libs.append('dggal_c')

# _py* CFFI packages are currently being packaged outside of the main extension directory
extra_link_args = ['-Wl,--no-as-needed','-ldggal',"-Wl,-rpath,$ORIGIN/lib:$ORIGIN/../../ecrt/lib:$ORIGIN/dggal/lib:$ORIGIN/ecrt/lib", '-DMS_WIN64', '-O2']
if sys.platform == 'win32':
   extra_link_args.append('-Wl,--export-all-symbols')
else:
   extra_link_args.append('-Wl,--export-dynamic')

ffi_dggal.set_source('_pydggal',
               '#include "dggal.h"',
               sources=srcs,
               define_macros=[('BINDINGS_SHARED', None), ('DGGAL_EXPORT', None)],
               extra_compile_args=['-DECPRFX=eC_', '-DMS_WIN64', '-O2'], #--export-dynamic' ]
               include_dirs=[bindings_py_dir, incdir, incdir_ecrt],
               libraries=libs,
               extra_link_args=extra_link_args,
               library_dirs=[libdir, ecrt_location],
               py_limited_api=False)
if __name__ == '__main__':
   V = os.getenv('V')
   v = True if V == '1' or V == 'y' else False

   ffi_dggal.compile(verbose=v,tmpdir='.',debug=False) # True)

if dnf != '':
   os.chdir(owd)
