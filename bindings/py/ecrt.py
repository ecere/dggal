from _pyecrt import *
import sys
import inspect
import os

app = None

# hardcoded content

# fix me, conflict in class Class with 'type' arg
pyType = type

def pyFFI():
   return app.ffi

def pyTypeByName(n):
   return app.appGlobals[-1].get(n, None)

def pyObject(eCObject):
   if eCObject._class.bindingsClass != ffi.NULL:
      handle = ffi.cast("void **", ffi.cast("char *", eCObject) + eCObject._class.offset)[0]
      if handle != ffi.NULL: return ffi.from_handle(handle)
   return None

def pyOrNewObject(c, eCObject, templateParams=None):
   if eCObject == ffi.NULL: return None
   o = pyObject(eCObject)
   if o is None:
      if templateParams is not None: o = c(templateParams=templateParams, impl=eCObject)
      else: o = c(impl=eCObject)
   return o

def pyTypedObject(a):
   cargs = ()
   if type(a) == str:            cargs += (lib.class_String, ffi.new("char[]", a.encode('u8')))
   elif type(a) == int:          cargs += (lib.class_int,    ffi.new("int *", a))
   elif type(a) == float:        cargs += (lib.class_double, ffi.new("double *", a))
   elif isinstance(a, Instance): cargs += (a.impl._class, a.impl)
   else:
      # TODO: Improve on this
      c = lib.eC_findClass(app.impl, type(a).__name__.encode('u8'))
      if c.type == ClassType.structClass:
         cargs += (c, a.impl)
      elif c.type == ClassType.noHeadClass:
         cargs += (c, a.impl)
      else:
         cargs += (c, ffi.new("int *", a.impl))
   return cargs

def pyAddrTypedObject(t):
   cargs = ()
   if t == str:            cargs += (lib.class_String, ffi.new("char[]"))
   elif t == int:          cargs += (lib.class_int,    ffi.new("int *"))
   elif t == float:        cargs += (lib.class_double, ffi.new("double *"))
   elif issubclass(t, Instance): cargs += (lib.eC_findClass(app.impl, t.__name__.encode('u8')), ffi.new("eC_Instance *"))
   else:
      # TODO: Improve on this
      c = lib.eC_findClass(app.impl, t.__name__.encode('u8'))
      if c != ffi.NULL:
         if c.type == ClassType.structClass:
            cargs += (c, ffi.new(t.__name__))
         elif c.type == ClassType.noHeadClass:
            cargs += (c, ffi.new(t.__name__ + " * "))
         else:
            cargs += (c, ffi.new(t.__name__)) # ffi.new("int *"))
   return cargs

def pyRetAddrTypedObject(t, a):
   if t == str:            return ffi.string(a[0]).decode('u8')
   elif t == int:          return a[0]
   elif t == float:        return a[0]
   elif issubclass(t, Instance) and a[0] != ffi.NULL:
      t = pyTypeByName(ffi.string(a[0]._class.name).decode('u8'))
   return t(impl = a[0])

def IPTR(lib, ffi, self, c):
   if self is None or self.impl == ffi.NULL: return None
   cn = c.__name__
   co = getattr(lib, 'class_' + cn)
   offset = co.offset
   bp = ffi.cast("char *", self.impl) + offset
   s = ffi.cast("struct class_members_" + cn + " *", bp)
   return s

@ffi.callback("void(eC_Instance)")
def cb_Instance_destructor(i):
   instance = pyObject(i)
   if instance is not None:
      Instance.instances.remove(instance)
      instance.handle = 0
   else:
      _print("No matching instance! for ", ffi.string(i._class.name).decode('u8'))

@ffi.callback("eC_bool(eC_Instance, eC_bool)")
def cb_Instance_constructor(i, a):
   s = (ffi.string(i._class.name).decode('u8'))[2:]
   if a:
      for ag in app.appGlobals:
         g = ag.get(s, None)
         if g is not None:
            g(impl=i)
            break
   return True

def I18N(s):
   return ffi.string(lib.getTranslatedString(os.path.splitext(os.path.basename(inspect.getfile(sys._getframe(1))))[0].encode('u8'), s.encode('u8'), ffi.NULL        )).decode('u8')
def I18NC(s, c):
   return ffi.string(lib.getTranslatedString(os.path.splitext(os.path.basename(inspect.getfile(sys._getframe(1))))[0].encode('u8'), s.encode('u8'), c.encode('u8'))).decode('u8')

def init_args(c, self, args, kwArgs):
   super(c, self).init_args([] if hasattr(c, 'private_inheritance') else args, kwArgs)
   for k, v in zip(c.class_members, args[:]):
      if v is not None:
         setattr(self, k, v)
         del args[0]
   for k, v in {k:v for k,v in kwArgs.items() if k in c.class_members and v is not None}.items():
      setattr(self, k, v)
      del kwArgs[k]

def convertTypedArgs(args):
   cargs = ()
   ag_ffi = app.ffi
   for a in args:
      if   type(a) == String:         cargs += (lib.class_String, a.impl)
      elif type(a) == str:            cargs += (lib.class_String, ffi.new("char[]", a.encode('u8')))
      elif type(a) == int:            cargs += (lib.class_int,    ffi.new("int *", a))
      elif type(a) == float:          cargs += (lib.class_double, ffi.new("double *", a))
      elif isinstance(a, Instance):   cargs += (a.impl._class, a.impl)
      else:
         # TODO: Improve on this
         c = lib.eC_findClass(app.impl, type(a).__name__.encode('u8'))
         if c.type == ClassType.structClass:
            cargs += (c, a.impl)
         elif c.type == ClassType.noHeadClass:
            cargs += (c, a.impl)
         elif c.type == ClassType.unitClass:
            tString = ffi.string(c.dataTypeString).decode('u8');
            if tString == "double" or tString == "float":
               cargs += (lib.class_double, ffi.new("double *", float(a)))
            elif tString == "char *":
               cargs += (lib.class_String, ffi.new("char[]", str(a).encode('u8')))
            elif tString == "int":
               cargs += (lib.class_int, ffi.new("int *", int(a)))
            else:
               cargs += (c, ag_ffi.new(type(a).__name__ + " *", a.impl))
         else:
            cargs += (c, ag_ffi.new(type(a).__name__ + " *", a.impl))
   return cargs + (ffi.NULL,)

def ellipsisArgs(args):
   cargs = ()
   for a in args:
      if type(a) == str:               cargs += (ffi.new("char[]", a.encode('u8')),) # tocheck
      elif type(a) == int:             cargs += (ffi.cast("int", a),)
      elif type(a) == float:           cargs += (ffi.cast("double", a),)
      elif isinstance(a, Instance):    cargs += (a.impl,)
      elif hasattr(a, 'impl'):
         if type(a.impl) == str:       cargs += (ffi.new("char[]", a.impl.encode('u8')),) # tocheck
         elif type(a.impl) == int:     cargs += (ffi.cast("int", a.impl),)
         elif type(a.impl) == float:   cargs += (ffi.cast("double", a.impl),)
         else:                         print("ellipsisArgs: warning, unknown argument type")
      else:                            print("ellipsisArgs: warning, unknown argument type")
   return cargs

def ecPtr(_pyObject):
   if _pyObject is None: return ffi.NULL
   return _pyObject.impl

def TA(a):
   u = ffi.new("eC_DataValue *")
   if type(a) == int:
      u.i64 = a
   elif type(a) == float:
      u.f = a
   elif isinstance(a, Instance):
      u.p = a.impl
   return u.ui64

def OTA(c, value):
   ffi = app.ffi

   cn = ffi.string(c.name).decode('u8')
   pc = app.appGlobals[-1].get(cn, None)
   if not pc and c.templateClass:
      cn = ffi.string(c.templateClass.name).decode('u8')
      pc = app.appGlobals[-1].get(cn, None)
   if pc is not None:
      if c.type == lib.ClassType_normalClass:
         if pc == String:
            return String(impl=ffi.cast("char *", lib.pTAvoid(value)))
         else:
            return pc(impl=lib.oTAInstance(value))
      elif c.type == lib.ClassType_noHeadClass:
         return pc(impl=ffi.cast(cn + "*", lib.oTAInstance(value)))
      elif c.type == lib.ClassType_structClass:
         # REVIEW: missing eC_ prefix?
         return pc(impl=ffi.cast("eC_" + cn + "*", lib.oTAInstance(value))[0])
      elif c.type == lib.ClassType_bitClass:
         # REVIEW: new for bit classes
         return pc(impl=ffi.cast("eC_" + cn, value))
   else:
      # Review this new handling
      u = ffi.new("eC_DataValue *")
      u.ui64 = value
      if cn == "int":
         return u.i64
      elif cn == "float":
         return u.f
   # TODO: Fill this up
   printLn("WARNING: OTA() Missing Implementation for ", cn)
   return None

def ffis(s): return ffi.string(s).decode('u8')

def coFromPy(v0):
   t = type(v0)
   co = ffi.NULL
   if t == int:     co = lib.class_int
   elif t == float: co = lib.class_double
   else:
      if co == ffi.NULL and isinstance(v0, Instance):
         c = t
         while True:
            if hasattr(c, 'pyClass_' + c.__name__):
               co = getattr(c, 'pyClass_' + c.__name__)
               break
            if len(c.__bases__) >= 1:
               c = c.__bases__[0]
            break
      if co == ffi.NULL:
         c = t
         while True:
            ag_lib = app.lib
            if hasattr(ag_lib, 'class_' + c.__name__):
               co = getattr(ag_lib, 'class_' + c.__name__)
               break
            if len(c.__bases__) >= 1:
               c = c.__bases__[0]
            break
      if co == ffi.NULL:
         print("Container error: could not match to eC class: ", t.__name__)
   return co

class pyBaseClass:
   buc = None
   def __init__(self, impl):
      self.impl = impl

   def __neg__(self):
      t = type(self)
      return t(impl=-self.impl)

   def __int__(self):
      t = type(self)
      if t.buc is not None and t.buc != t:
         return int(self.value)
      return int(self.impl)
   def __long__(self):
      t = type(self)
      if t.buc is not None and t.buc != t:
         return long(self.value)
      return long(self.impl)
   def __float__(self):
      t = type(self)
      if t.buc is not None and t.buc != t:
         return float(self.value)
      return float(self.impl)

   def __truediv__(self, other):
      t = type(self)
      #buc = t.buc if t.buc is not None else t
      #if not isinstance(other, buc): other = t(impl = other)
      return t(impl=self.impl / float(other))
   def __rtruediv__(self, other):
      t = type(self)
      #buc = t.buc if t.buc is not None else t
      #if not isinstance(other, buc): other = t(other)
      return t(impl=other / float(self.impl)) #self.impl)
   def __mul__(self, other):
      t = type(self)
      #buc = t.buc if t.buc is not None else t
      #if not isinstance(other, buc): other = t(other)
      return t(impl=self.impl * float(other)) #other.impl)
   def __rmul__(self, other):
      t = type(self)
      #buc = t.buc if t.buc is not None else t
      #if not isinstance(other, buc): other = t(other)
      #return t(impl=other.impl * self.impl)
      return t(impl=other * float(self.impl)) #self.impl)
   def __add__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return t(impl=self.impl + other.impl)
   def __radd__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return t(impl=other.impl + self.impl)
   def __sub__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return t(impl=self.impl - other.impl)
   def __rsub__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return t(impl=other.impl - self.impl)

   def __lt__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return self.impl < other.impl
   def __gt__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return self.impl > other.impl
   def __le__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return self.impl <= other.impl
   def __ge__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return self.impl >= other.impl
   def __ne__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return self.impl != other.impl
   def __eq__(self, other):
      t = type(self)
      buc = t.buc if t.buc is not None else t
      if not isinstance(other, buc): other = t(other)
      return self.impl == other.impl

# hardcoded classes

class Bool:
   true  = lib.true
   false = lib.false

class Struct:
   def onCompare(self, other):
       if self is not None or other is not None:
           c = type(self) if self is not None else type(other) if other is not None else None
           cn = c.__name__ if c is not None else None
           co = getattr(app.lib, 'class_' + cn) if cn is not None else None
           first = self.impl if self is not None else ffi.NULL
           second = other.impl if other is not None else ffi.NULL
           return lib._onCompare(co, first, second)
       return 0

class Instance:
   instances = []
   def __init__(self, *args, **kwArgs): self.init_args(list(args), kwArgs)
   def init_args(self, args, kwArgs):
      templateParams = kwArgs.get('templateParams')
      impl = kwArgs.get('impl')
      if hasattr(self, 'impl') and self.impl is not None: impl = self.impl
      if impl is None:
         c = type(self)
         while not hasattr(c, 'pyClass_' + c.__name__):
            c = c.__bases__[0]
         pyClass = getattr(c, 'pyClass_' + c.__name__)
         if templateParams is not None:
            templateName = ffi.string(pyClass.name).decode('u8') + templateParams
            if hasattr(c, templateName):
               template = getattr(c, templateName)
            else:
               template = lib.eC_findClass(pyClass.module, templateName.encode('u8'))
               setattr(c, templateName, template)
            self.impl = ffi.cast("eC_Instance", lib.Instance_newEx(template, False))
         else:
            self.impl = ffi.cast("eC_Instance", lib.Instance_newEx(pyClass, False))
      else:
         self.impl = impl
      self.impl._refCount += 1
      Instance.instances.append(self)
      if not hasattr(self, 'handle'):
         self.handle = ffi.new_handle(self)
      if impl != ffi.NULL and (impl is None or impl._class.bindingsClass != ffi.NULL):
         ffi.cast("void **", ffi.cast("char *", self.impl) + self.impl._class.offset)[0] = self.handle

   def delete(self):
      lib.Instance_delete(self.impl)
      self.impl = ffi.NULL
   '''def onCompare(self, other):
      if self is None or self.impl == ffi.NULL: return None
      c = type(self)
      cn = c.__name__
      co = getattr(lib, 'class_' + cn)
      lib.Instance_onCompare(co, self.impl, other.impl)'''

# end of hardcoded content

@ffi.callback("eC_bool(eC_Module)")
def cb_Module_onLoad(__e):
   module = pyOrNewObject(Module, __e)
   return module.fn_Module_onLoad(module)

@ffi.callback("void(eC_Module)")
def cb_Module_onUnload(__e):
   module = pyOrNewObject(Module, __e)
   module.fn_Module_onUnload(module)
def regclass(c):
   app.registerClass(c)
   return c

class Module(Instance):
   class_members = [
                      'application',
                      'classes',
                      'defines',
                      'functions',
                      'modules',
                      'prev',
                      'next',
                      'name',
                      'library',
                      'Unload',
                      'importType',
                      'origImportType',
                      'privateNameSpace',
                      'publicNameSpace',
                      'onLoad',
                      'onUnload',
                   ]

   def init_args(self, args, kwArgs): init_args(Module, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   # hack: hardcoded content
   def registerClass(self, n, isWrapper = False):
      if isWrapper:
         cn = "Py" + n.__name__
         bn = n.__name__
      else:
         cn = n.__name__
         b = n
         while not hasattr(b, 'pyClass_' + b.__name__):
            b = b.__bases__[0]
         bn = b.__name__
      pyClass = lib.eC_registerClass(lib.ClassType_normalClass, cn.encode('u8'), bn.encode('u8'), 8, 0,
         ffi.cast("eC_bool(*)(void *)", cb_Instance_constructor),
         ffi.cast("void(*)(void *)", cb_Instance_destructor),
         self.impl, lib.AccessMode_publicAccess, lib.AccessMode_publicAccess)
      setattr(n, 'pyClass_' + n.__name__, pyClass)
      pyClass.bindingsClass = ffi.new_handle(n)
   # hack: end of hardcoded content

   @property
   def application(self): return pyOrNewObject(Application, IPTR(lib, ffi, self, Module).application)
   @application.setter
   def application(self, value):
      if not isinstance(value, Application): value = Application(value)
      IPTR(lib, ffi, self, Module).application = value.impl

   @property
   def classes(self): return OldList(impl = IPTR(lib, ffi, self, Module).classes)
   @classes.setter
   def classes(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      IPTR(lib, ffi, self, Module).classes = value.impl

   @property
   def defines(self): return OldList(impl = IPTR(lib, ffi, self, Module).defines)
   @defines.setter
   def defines(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      IPTR(lib, ffi, self, Module).defines = value.impl

   @property
   def functions(self): return OldList(impl = IPTR(lib, ffi, self, Module).functions)
   @functions.setter
   def functions(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      IPTR(lib, ffi, self, Module).functions = value.impl

   @property
   def modules(self): return OldList(impl = IPTR(lib, ffi, self, Module).modules)
   @modules.setter
   def modules(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      IPTR(lib, ffi, self, Module).modules = value.impl

   @property
   def prev(self): return pyOrNewObject(Module, IPTR(lib, ffi, self, Module).prev)
   @prev.setter
   def prev(self, value):
      if not isinstance(value, Module): value = Module(value)
      IPTR(lib, ffi, self, Module).prev = value.impl

   @property
   def next(self): return pyOrNewObject(Module, IPTR(lib, ffi, self, Module).next)
   @next.setter
   def next(self, value):
      if not isinstance(value, Module): value = Module(value)
      IPTR(lib, ffi, self, Module).next = value.impl

   @property
   def name(self): return IPTR(lib, ffi, self, Module).name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, Module).name = value

   @property
   def library(self): return IPTR(lib, ffi, self, Module).library
   @library.setter
   def library(self, value): IPTR(lib, ffi, self, Module).library = value

   @property
   def Unload(self): return IPTR(lib, ffi, self, Module).Unload
   @Unload.setter
   def Unload(self, value): IPTR(lib, ffi, self, Module).Unload = value

   @property
   def importType(self): return ImportType(impl = IPTR(lib, ffi, self, Module).importType)
   @importType.setter
   def importType(self, value): IPTR(lib, ffi, self, Module).importType = value.impl

   @property
   def origImportType(self): return ImportType(impl = IPTR(lib, ffi, self, Module).origImportType)
   @origImportType.setter
   def origImportType(self, value): IPTR(lib, ffi, self, Module).origImportType = value.impl

   @property
   def privateNameSpace(self): return NameSpace(impl = IPTR(lib, ffi, self, Module).privateNameSpace)
   @privateNameSpace.setter
   def privateNameSpace(self, value):
      if not isinstance(value, NameSpace): value = NameSpace(value)
      IPTR(lib, ffi, self, Module).privateNameSpace = value.impl

   @property
   def publicNameSpace(self): return NameSpace(impl = IPTR(lib, ffi, self, Module).publicNameSpace)
   @publicNameSpace.setter
   def publicNameSpace(self, value):
      if not isinstance(value, NameSpace): value = NameSpace(value)
      IPTR(lib, ffi, self, Module).publicNameSpace = value.impl

   def load(self, name, importAccess):
      if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
      elif name is None: name = ffi.NULL
      return pyOrNewObject(Module, lib.Module_load(self.impl, name, importAccess))

   def fn_unset_Module_onLoad(self):
      return lib.Module_onLoad(self.impl)

   @property
   def onLoad(self):
      if hasattr(self, 'fn_Module_onLoad'): return self.fn_Module_onLoad
      else: return self.fn_unset_Module_onLoad
   @onLoad.setter
   def onLoad(self, value):
      self.fn_Module_onLoad = value
      lib.Instance_setMethod(self.impl, "OnLoad".encode('u8'), cb_Module_onLoad)

   def fn_unset_Module_onUnload(self):
      return lib.Module_onUnload(self.impl)

   @property
   def onUnload(self):
      if hasattr(self, 'fn_Module_onUnload'): return self.fn_Module_onUnload
      else: return self.fn_unset_Module_onUnload
   @onUnload.setter
   def onUnload(self, value):
      self.fn_Module_onUnload = value
      lib.Instance_setMethod(self.impl, "OnUnload".encode('u8'), cb_Module_onUnload)

   def unload(self, module = None):
      if module is not None and not isinstance(module, Module): module = Module(module)
      module = ffi.NULL if module is None else module.impl
      lib.Module_unload(self.impl, module)

@ffi.callback("void(eC_Application)")
def cb_Application_main(__e):
   application = pyOrNewObject(Application, __e)
   application.fn_Application_main(application)

class Application(Module):
   # hack: hardcoded content
   appGlobals = []
   def __init__(self, appGlobals = None):
      global app
      app = self
      if appGlobals is not None:
         self.appGlobals.append(appGlobals)
      else:
         self.appGlobals.append(globals())
      impl = lib.ecrt_init(ffi.NULL, True, True, len(sys.argv), [ffi.new("char[]", i.encode('u8')) for i in sys.argv])
      Module.__init__(self, impl = impl)
      self.registerClass(String, True)
      self.registerClass(Application, True)
      self.registerClass(Instance, True)
      self.registerClass(Module, True)
      self.registerClass(AVLTree, True)
      self.registerClass(Array, True)
      self.registerClass(Container, True)
      self.registerClass(CustomAVLTree, True)
      self.registerClass(HashMap, True)
      self.registerClass(HashTable, True)
      self.registerClass(LinkList, True)
      self.registerClass(List, True)
      self.registerClass(Map, True)
      self.registerClass(Archive, True)
      self.registerClass(ArchiveDir, True)
      self.registerClass(BufferedFile, True)
      self.registerClass(ConsoleFile, True)
      self.registerClass(DualPipe, True)
      self.registerClass(File, True)
      self.registerClass(FileMonitor, True)
      self.registerClass(TempFile, True)
      self.registerClass(ECONGlobalSettings, True)
      self.registerClass(ECONParser, True)
      self.registerClass(GlobalAppSettings, True)
      self.registerClass(GlobalSettings, True)
      self.registerClass(GlobalSettingsData, True)
      self.registerClass(GlobalSettingsDriver, True)
      self.registerClass(JSONGlobalSettings, True)
      self.registerClass(JSONParser, True)
      self.registerClass(OptionsMap, True)
      self.registerClass(Thread, True)
      self.registerClass(CIString, True)
      self.registerClass(ClassDesignerBase, True)
      self.registerClass(DesignerBase, True)
      self.registerClass(IOChannel, True)
      self.registerClass(SerialBuffer, True)
      self.registerClass(ZString, True)

   @property
   def lib(self): return self.appGlobals[-1].get("lib", None)
   @property
   def ffi(self): return self.appGlobals[-1].get("ffi", None)
   # hack: end of hardcoded content

   @property
   def argc(self): return IPTR(lib, ffi, self, Application).argc
   @argc.setter
   def argc(self, value): IPTR(lib, ffi, self, Application).argc = value

   @property
   def argv(self): return IPTR(lib, ffi, self, Application).argv
   @argv.setter
   def argv(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, Application).argv = value

   @property
   def exitCode(self): return IPTR(lib, ffi, self, Application).exitCode
   @exitCode.setter
   def exitCode(self, value): IPTR(lib, ffi, self, Application).exitCode = value

   @property
   def isGUIApp(self): return IPTR(lib, ffi, self, Application).isGUIApp
   @isGUIApp.setter
   def isGUIApp(self, value): IPTR(lib, ffi, self, Application).isGUIApp = value

   @property
   def allModules(self): return OldList(impl = IPTR(lib, ffi, self, Application).allModules)
   @allModules.setter
   def allModules(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      IPTR(lib, ffi, self, Application).allModules = value.impl

   @property
   def parsedCommand(self): return IPTR(lib, ffi, self, Application).parsedCommand
   @parsedCommand.setter
   def parsedCommand(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, Application).parsedCommand = value

   @property
   def systemNameSpace(self): return NameSpace(impl = IPTR(lib, ffi, self, Application).systemNameSpace)
   @systemNameSpace.setter
   def systemNameSpace(self, value):
      if not isinstance(value, NameSpace): value = NameSpace(value)
      IPTR(lib, ffi, self, Application).systemNameSpace = value.impl

   def fn_unset_Application_main(self):
      return lib.Application_main(self.impl)

   @property
   def main(self):
      if hasattr(self, 'fn_Application_main'): return self.fn_Application_main
      else: return self.fn_unset_Application_main
   @main.setter
   def main(self, value):
      self.fn_Application_main = value
      lib.Instance_setMethod(self.impl, "Main".encode('u8'), cb_Application_main)

class SecSince1970(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

   @property
   def _global(self): return SecSince1970(impl = lib.SecSince1970_get_global(self.impl))

   @property
   def local(self): return SecSince1970(impl = lib.SecSince1970_get_local(self.impl))

SecSince1970.buc = SecSince1970

class Time(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

Time.buc = Time

class Date(Struct):
   def __init__(self, year = 0, month = 0, day = 0, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_Date *", impl)
      else:
         if isinstance(year, tuple):
            __tuple = year
            year = 0
            if len(__tuple) > 0: year  = __tuple[0]
            if len(__tuple) > 1: month = __tuple[1]
            if len(__tuple) > 2: day   = __tuple[2]
         elif isinstance(year, DateTime):
            self.impl = ffi.new("eC_Date *")
            lib.DateTime_to_Date(year.impl, self.impl)
            return
         self.impl = ffi.new("eC_Date *", { 'year' : year, 'month' : month, 'day' : day })

   @property
   def year(self): return self.impl.year
   @year.setter
   def year(self, value): self.impl.year = value

   @property
   def month(self): return self.impl.month
   @month.setter
   def month(self, value): self.impl.month = value

   @property
   def day(self): return self.impl.day
   @day.setter
   def day(self, value): self.impl.day = value

   @property
   def dayOfTheWeek(self): return lib.Date_get_dayOfTheWeek(self.impl)

   def onGetStringEn(self, stringOutput, fieldData, onType):
      if isinstance(stringOutput, str): stringOutput = ffi.new("char[]", stringOutput.encode('u8'))
      elif stringOutput is None: stringOutput = ffi.NULL
      if hasattr(fieldData, 'impl'): fieldData = fieldData.impl
      if fieldData is None: fieldData = ffi.NULL
      return lib.Date_onGetStringEn(ffi.cast("eC_Date *", self.impl), stringOutput, fieldData, onType)

class DateTime(Struct):
   def __init__(self, year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, dayOfTheWeek = 0, dayInTheYear = 0, _global = None, local = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_DateTime *", impl)
      else:
         if isinstance(year, SecSince1970):
            self.impl = ffi.new("eC_DateTime *")
            lib.DateTime_from_SecSince1970(self.impl, year.impl)
            return
         elif isinstance(year, Date):
            self.impl = ffi.new("eC_DateTime *")
            lib.DateTime_from_Date(self.impl, year.impl)
            return
         self.impl = ffi.new("eC_DateTime *", {
                                'year' : year,
                                'month' : month,
                                'day' : day,
                                'hour' : hour,
                                'minute' : minute,
                                'second' : second,
                                'dayOfTheWeek' : dayOfTheWeek,
                                'dayInTheYear' : dayInTheYear
                             })
         if _global is not None:      self._global           = _global
         if local is not None:        self.local             = local

   @property
   def year(self): return self.impl.year
   @year.setter
   def year(self, value): self.impl.year = value

   @property
   def month(self): return self.impl.month
   @month.setter
   def month(self, value): self.impl.month = value

   @property
   def day(self): return self.impl.day
   @day.setter
   def day(self, value): self.impl.day = value

   @property
   def hour(self): return self.impl.hour
   @hour.setter
   def hour(self, value): self.impl.hour = value

   @property
   def minute(self): return self.impl.minute
   @minute.setter
   def minute(self, value): self.impl.minute = value

   @property
   def second(self): return self.impl.second
   @second.setter
   def second(self, value): self.impl.second = value

   @property
   def dayOfTheWeek(self): return self.impl.dayOfTheWeek
   @dayOfTheWeek.setter
   def dayOfTheWeek(self, value): self.impl.dayOfTheWeek = value

   @property
   def dayInTheYear(self): return self.impl.dayInTheYear
   @dayInTheYear.setter
   def dayInTheYear(self, value): self.impl.dayInTheYear = value

   @property
   def _global(self): value = DateTime(); lib.DateTime_get_global(self.impl, ffi.cast("eC_DateTime *", value.impl)); return value
   @_global.setter
   def _global(self, value):
      if not isinstance(value, DateTime): value = DateTime(value)
      lib.DateTime_set_global(self.impl, ffi.cast("eC_DateTime *", value.impl))

   @property
   def local(self): value = DateTime(); lib.DateTime_get_local(self.impl, ffi.cast("eC_DateTime *", value.impl)); return value
   @local.setter
   def local(self, value):
      if not isinstance(value, DateTime): value = DateTime(value)
      lib.DateTime_set_local(self.impl, ffi.cast("eC_DateTime *", value.impl))

   @property
   def daysSince1970(self): return lib.DateTime_get_daysSince1970(self.impl)

   # def DateTime_to_SecSince1970(self): return SecSince1970(lib.DateTime_to_SecSince1970(self.impl))

   # here is an unhandled conversion: DateTime::SecSince1970 (StructClass 2 UnitClass)
   # DateTime_to_SecSince1970
   # DateTime_from_SecSince1970

   # def DateTime_to_Date(self): value = Date(); lib.DateTime_to_Date(self.impl, ffi.cast("eC_Date *", value.impl)); return

   # here is an unhandled conversion: DateTime::Date (StructClass 2 StructClass)
   # DateTime_to_Date
   # DateTime_from_Date

   def fixDayOfYear(self):
      return lib.DateTime_fixDayOfYear(ffi.cast("eC_DateTime *", self.impl))

   def getLocalTime(self):
      return lib.DateTime_getLocalTime(ffi.cast("eC_DateTime *", self.impl))

class DayOfTheWeek:
   sunday    = lib.DayOfTheWeek_sunday
   monday    = lib.DayOfTheWeek_monday
   tuesday   = lib.DayOfTheWeek_tuesday
   wednesday = lib.DayOfTheWeek_wednesday
   thursday  = lib.DayOfTheWeek_thursday
   friday    = lib.DayOfTheWeek_friday
   saturday  = lib.DayOfTheWeek_saturday

class Month:
   january   = lib.Month_january
   february  = lib.Month_february
   march     = lib.Month_march
   april     = lib.Month_april
   may       = lib.Month_may
   june      = lib.Month_june
   july      = lib.Month_july
   august    = lib.Month_august
   september = lib.Month_september
   october   = lib.Month_october
   november  = lib.Month_november
   december  = lib.Month_december

   def getNumDays(self, year):
      return lib.Month_getNumDays(self.impl, year)

class Seconds(Time):
   def __init__(self, value = 0, impl = None):
      if impl is not None: self.impl = impl
      elif isinstance(value, Time): self.impl = value.impl
      else: self.impl = value
   @property
   def value(self): return self.impl
   @value.setter
   def value(self, value): self.impl = value

Seconds.buc = Time

class TimeStamp(SecSince1970):
   def __init__(self, value = 0, impl = None):
      if impl is not None: self.impl = impl
      elif isinstance(value, SecSince1970): self.impl = value.impl
      else: self.impl = value
   @property
   def value(self): return self.impl
   @value.setter
   def value(self, value): self.impl = value

TimeStamp.buc = SecSince1970

class TimeStamp32(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

TimeStamp32.buc = TimeStamp32

def getRandom(lo, hi):
   return lib.eC_getRandom(lo, hi)

def getTime():
   return Time(impl = lib.eC_getTime())

def randomSeed(seed):
   lib.eC_randomSeed(seed)

def __sleep(seconds):
   if seconds is not None and not isinstance(seconds, Time): seconds = Seconds(seconds)
   if seconds is None: seconds = ffi.NULL
   lib.eC___sleep(seconds.impl)

class Angle(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

Angle.buc = Angle

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer, uint64)")
def cb_BuiltInContainer_add(__e, value):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_add(builtincontainer, value)

@ffi.callback("void(eC_BuiltInContainer, eC_Container)")
def cb_BuiltInContainer_copy(__e, source):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_copy(builtincontainer, pyOrNewObject(Container, source))

@ffi.callback("void(eC_BuiltInContainer, eC_IteratorPointer *)")
def cb_BuiltInContainer_delete(__e, it):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_delete(builtincontainer, IteratorPointer(impl = it))

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer, uint64)")
def cb_BuiltInContainer_find(__e, value):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_find(builtincontainer, value)

@ffi.callback("void(eC_BuiltInContainer)")
def cb_BuiltInContainer_free(__e):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_free(builtincontainer)

@ffi.callback("void(eC_BuiltInContainer, eC_IteratorPointer *)")
def cb_BuiltInContainer_freeIterator(__e, it):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_freeIterator(builtincontainer, IteratorPointer(impl = it))

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer, const uint64, eC_bool)")
def cb_BuiltInContainer_getAtPosition(__e, pos, create):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_getAtPosition(builtincontainer, pos, create)

@ffi.callback("int(eC_BuiltInContainer)")
def cb_BuiltInContainer_getCount(__e):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_getCount(builtincontainer)

@ffi.callback("uint64(eC_BuiltInContainer, eC_IteratorPointer *)")
def cb_BuiltInContainer_getData(__e, pointer):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_getData(builtincontainer, IteratorPointer(impl = pointer))

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer)")
def cb_BuiltInContainer_getFirst(__e):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_getFirst(builtincontainer)

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer)")
def cb_BuiltInContainer_getLast(__e):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_getLast(builtincontainer)

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer, eC_IteratorPointer *)")
def cb_BuiltInContainer_getNext(__e, pointer):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_getNext(builtincontainer, IteratorPointer(impl = pointer))

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer, eC_IteratorPointer *)")
def cb_BuiltInContainer_getPrev(__e, pointer):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_getPrev(builtincontainer, IteratorPointer(impl = pointer))

@ffi.callback("eC_IteratorPointer *(eC_BuiltInContainer, eC_IteratorPointer *, uint64)")
def cb_BuiltInContainer_insert(__e, after, value):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_insert(builtincontainer, IteratorPointer(impl = after), value)

@ffi.callback("void(eC_BuiltInContainer, eC_IteratorPointer *, eC_IteratorPointer *)")
def cb_BuiltInContainer_move(__e, it, after):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_move(builtincontainer, IteratorPointer(impl = it), IteratorPointer(impl = after))

@ffi.callback("void(eC_BuiltInContainer, eC_IteratorPointer *)")
def cb_BuiltInContainer_remove(__e, it):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_remove(builtincontainer, IteratorPointer(impl = it))

@ffi.callback("void(eC_BuiltInContainer)")
def cb_BuiltInContainer_removeAll(__e):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_removeAll(builtincontainer)

@ffi.callback("eC_bool(eC_BuiltInContainer, eC_IteratorPointer *, uint64)")
def cb_BuiltInContainer_setData(__e, pointer, data):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   return builtincontainer.fn_BuiltInContainer_setData(builtincontainer, IteratorPointer(impl = pointer), data)

@ffi.callback("void(eC_BuiltInContainer, eC_bool)")
def cb_BuiltInContainer_sort(__e, ascending):
   builtincontainer = pyOrNewObject(BuiltInContainer, __e)
   builtincontainer.fn_BuiltInContainer_sort(builtincontainer, ascending)

class BuiltInContainer(Struct):
   def __init__(self, _vTbl = None, _class = None, _refCount = 0, data = None, count = 0, type = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_BuiltInContainer *", impl)
      else:
         if isinstance(_vTbl, tuple):
            __tuple = _vTbl
            _vTbl = None
            if len(__tuple) > 0: _vTbl     = __tuple[0]
            if len(__tuple) > 1: _class    = __tuple[1]
            if len(__tuple) > 2: _refCount = __tuple[2]
            if len(__tuple) > 3: data      = __tuple[3]
         elif isinstance(_vTbl, Container):
            self.impl = ffi.new("eC_BuiltInContainer *")
            lib.BuiltInContainer_from_Container(self.impl, _vTbl)
            return
         if _class is not None:
            if not isinstance(_class, Class): _class = Class(_class)
            _class = _class.impl
         else:
            _class = ffi.NULL
         if type is not None:
            if not isinstance(type, Class): type = Class(type)
            type = type.impl
         else:
            type = ffi.NULL
         self.impl = ffi.new("eC_BuiltInContainer *", {
                                '_vTbl' : _vTbl,
                                '_class' : _class,
                                '_refCount' : _refCount,
                                'data' : data,
                                'count' : count,
                                'type' : type
                             })

   @property
   def _vTbl(self): return self.impl._vTbl
   @_vTbl.setter
   def _vTbl(self, value): self.impl._vTbl = value

   @property
   def _class(self): return self.impl._class
   @_class.setter
   def _class(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl._class = value.impl

   @property
   def _refCount(self): return self.impl._refCount
   @_refCount.setter
   def _refCount(self, value): self.impl._refCount = value

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

   @property
   def count(self): return self.impl.count
   @count.setter
   def count(self, value): self.impl.count = value

   @property
   def type(self): return self.impl.type
   @type.setter
   def type(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.type = value.impl

   # def BuiltInContainer_to_Container(self): return pyOrNewObject(Container, lib.BuiltInContainer_to_Container(self.impl))

   # here is an unhandled conversion: BuiltInContainer::Container (StructClass 2 NormalClass)
   # BuiltInContainer_to_Container
   # BuiltInContainer_from_Container

   def fn_unset_BuiltInContainer_add(self, value):
      return lib.BuiltInContainer_add(self.impl, value)

   @property
   def add(self):
      if hasattr(self, 'fn_BuiltInContainer_add'): return self.fn_BuiltInContainer_add
      else: return self.fn_unset_BuiltInContainer_add
   @add.setter
   def add(self, value):
      self.fn_BuiltInContainer_add = value
      lib.Instance_setMethod(self.impl, "Add".encode('u8'), cb_BuiltInContainer_add)

   def fn_unset_BuiltInContainer_copy(self, source):
      return lib.BuiltInContainer_copy(self.impl, ffi.NULL if source is None else source.impl)

   @property
   def copy(self):
      if hasattr(self, 'fn_BuiltInContainer_copy'): return self.fn_BuiltInContainer_copy
      else: return self.fn_unset_BuiltInContainer_copy
   @copy.setter
   def copy(self, value):
      self.fn_BuiltInContainer_copy = value
      lib.Instance_setMethod(self.impl, "Copy".encode('u8'), cb_BuiltInContainer_copy)

   def fn_unset_BuiltInContainer_delete(self, it):
      return lib.BuiltInContainer_delete(self.impl, it)

   @property
   def delete(self):
      if hasattr(self, 'fn_BuiltInContainer_delete'): return self.fn_BuiltInContainer_delete
      else: return self.fn_unset_BuiltInContainer_delete
   @delete.setter
   def delete(self, value):
      self.fn_BuiltInContainer_delete = value
      lib.Instance_setMethod(self.impl, "Delete".encode('u8'), cb_BuiltInContainer_delete)

   def fn_unset_BuiltInContainer_find(self, value):
      return lib.BuiltInContainer_find(self.impl, value)

   @property
   def find(self):
      if hasattr(self, 'fn_BuiltInContainer_find'): return self.fn_BuiltInContainer_find
      else: return self.fn_unset_BuiltInContainer_find
   @find.setter
   def find(self, value):
      self.fn_BuiltInContainer_find = value
      lib.Instance_setMethod(self.impl, "Find".encode('u8'), cb_BuiltInContainer_find)

   def fn_unset_BuiltInContainer_free(self):
      return lib.BuiltInContainer_free(self.impl)

   @property
   def free(self):
      if hasattr(self, 'fn_BuiltInContainer_free'): return self.fn_BuiltInContainer_free
      else: return self.fn_unset_BuiltInContainer_free
   @free.setter
   def free(self, value):
      self.fn_BuiltInContainer_free = value
      lib.Instance_setMethod(self.impl, "Free".encode('u8'), cb_BuiltInContainer_free)

   def fn_unset_BuiltInContainer_freeIterator(self, it):
      return lib.BuiltInContainer_freeIterator(self.impl, it)

   @property
   def freeIterator(self):
      if hasattr(self, 'fn_BuiltInContainer_freeIterator'): return self.fn_BuiltInContainer_freeIterator
      else: return self.fn_unset_BuiltInContainer_freeIterator
   @freeIterator.setter
   def freeIterator(self, value):
      self.fn_BuiltInContainer_freeIterator = value
      lib.Instance_setMethod(self.impl, "FreeIterator".encode('u8'), cb_BuiltInContainer_freeIterator)

   def fn_unset_BuiltInContainer_getAtPosition(self, pos, create):
      return lib.BuiltInContainer_getAtPosition(self.impl, pos, create)

   @property
   def getAtPosition(self):
      if hasattr(self, 'fn_BuiltInContainer_getAtPosition'): return self.fn_BuiltInContainer_getAtPosition
      else: return self.fn_unset_BuiltInContainer_getAtPosition
   @getAtPosition.setter
   def getAtPosition(self, value):
      self.fn_BuiltInContainer_getAtPosition = value
      lib.Instance_setMethod(self.impl, "GetAtPosition".encode('u8'), cb_BuiltInContainer_getAtPosition)

   def fn_unset_BuiltInContainer_getCount(self):
      return lib.BuiltInContainer_getCount(self.impl)

   @property
   def getCount(self):
      if hasattr(self, 'fn_BuiltInContainer_getCount'): return self.fn_BuiltInContainer_getCount
      else: return self.fn_unset_BuiltInContainer_getCount
   @getCount.setter
   def getCount(self, value):
      self.fn_BuiltInContainer_getCount = value
      lib.Instance_setMethod(self.impl, "GetCount".encode('u8'), cb_BuiltInContainer_getCount)

   def fn_unset_BuiltInContainer_getData(self, pointer):
      return lib.BuiltInContainer_getData(self.impl, pointer)

   @property
   def getData(self):
      if hasattr(self, 'fn_BuiltInContainer_getData'): return self.fn_BuiltInContainer_getData
      else: return self.fn_unset_BuiltInContainer_getData
   @getData.setter
   def getData(self, value):
      self.fn_BuiltInContainer_getData = value
      lib.Instance_setMethod(self.impl, "GetData".encode('u8'), cb_BuiltInContainer_getData)

   def fn_unset_BuiltInContainer_getFirst(self):
      return lib.BuiltInContainer_getFirst(self.impl)

   @property
   def getFirst(self):
      if hasattr(self, 'fn_BuiltInContainer_getFirst'): return self.fn_BuiltInContainer_getFirst
      else: return self.fn_unset_BuiltInContainer_getFirst
   @getFirst.setter
   def getFirst(self, value):
      self.fn_BuiltInContainer_getFirst = value
      lib.Instance_setMethod(self.impl, "GetFirst".encode('u8'), cb_BuiltInContainer_getFirst)

   def fn_unset_BuiltInContainer_getLast(self):
      return lib.BuiltInContainer_getLast(self.impl)

   @property
   def getLast(self):
      if hasattr(self, 'fn_BuiltInContainer_getLast'): return self.fn_BuiltInContainer_getLast
      else: return self.fn_unset_BuiltInContainer_getLast
   @getLast.setter
   def getLast(self, value):
      self.fn_BuiltInContainer_getLast = value
      lib.Instance_setMethod(self.impl, "GetLast".encode('u8'), cb_BuiltInContainer_getLast)

   def fn_unset_BuiltInContainer_getNext(self, pointer):
      return lib.BuiltInContainer_getNext(self.impl, pointer)

   @property
   def getNext(self):
      if hasattr(self, 'fn_BuiltInContainer_getNext'): return self.fn_BuiltInContainer_getNext
      else: return self.fn_unset_BuiltInContainer_getNext
   @getNext.setter
   def getNext(self, value):
      self.fn_BuiltInContainer_getNext = value
      lib.Instance_setMethod(self.impl, "GetNext".encode('u8'), cb_BuiltInContainer_getNext)

   def fn_unset_BuiltInContainer_getPrev(self, pointer):
      return lib.BuiltInContainer_getPrev(self.impl, pointer)

   @property
   def getPrev(self):
      if hasattr(self, 'fn_BuiltInContainer_getPrev'): return self.fn_BuiltInContainer_getPrev
      else: return self.fn_unset_BuiltInContainer_getPrev
   @getPrev.setter
   def getPrev(self, value):
      self.fn_BuiltInContainer_getPrev = value
      lib.Instance_setMethod(self.impl, "GetPrev".encode('u8'), cb_BuiltInContainer_getPrev)

   def fn_unset_BuiltInContainer_insert(self, after, value):
      return lib.BuiltInContainer_insert(self.impl, after, value)

   @property
   def insert(self):
      if hasattr(self, 'fn_BuiltInContainer_insert'): return self.fn_BuiltInContainer_insert
      else: return self.fn_unset_BuiltInContainer_insert
   @insert.setter
   def insert(self, value):
      self.fn_BuiltInContainer_insert = value
      lib.Instance_setMethod(self.impl, "Insert".encode('u8'), cb_BuiltInContainer_insert)

   def fn_unset_BuiltInContainer_move(self, it, after):
      return lib.BuiltInContainer_move(self.impl, it, after)

   @property
   def move(self):
      if hasattr(self, 'fn_BuiltInContainer_move'): return self.fn_BuiltInContainer_move
      else: return self.fn_unset_BuiltInContainer_move
   @move.setter
   def move(self, value):
      self.fn_BuiltInContainer_move = value
      lib.Instance_setMethod(self.impl, "Move".encode('u8'), cb_BuiltInContainer_move)

   def fn_unset_BuiltInContainer_remove(self, it):
      return lib.BuiltInContainer_remove(self.impl, it)

   @property
   def remove(self):
      if hasattr(self, 'fn_BuiltInContainer_remove'): return self.fn_BuiltInContainer_remove
      else: return self.fn_unset_BuiltInContainer_remove
   @remove.setter
   def remove(self, value):
      self.fn_BuiltInContainer_remove = value
      lib.Instance_setMethod(self.impl, "Remove".encode('u8'), cb_BuiltInContainer_remove)

   def fn_unset_BuiltInContainer_removeAll(self):
      return lib.BuiltInContainer_removeAll(self.impl)

   @property
   def removeAll(self):
      if hasattr(self, 'fn_BuiltInContainer_removeAll'): return self.fn_BuiltInContainer_removeAll
      else: return self.fn_unset_BuiltInContainer_removeAll
   @removeAll.setter
   def removeAll(self, value):
      self.fn_BuiltInContainer_removeAll = value
      lib.Instance_setMethod(self.impl, "RemoveAll".encode('u8'), cb_BuiltInContainer_removeAll)

   def fn_unset_BuiltInContainer_setData(self, pointer, data):
      return lib.BuiltInContainer_setData(self.impl, pointer, data)

   @property
   def setData(self):
      if hasattr(self, 'fn_BuiltInContainer_setData'): return self.fn_BuiltInContainer_setData
      else: return self.fn_unset_BuiltInContainer_setData
   @setData.setter
   def setData(self, value):
      self.fn_BuiltInContainer_setData = value
      lib.Instance_setMethod(self.impl, "SetData".encode('u8'), cb_BuiltInContainer_setData)

   def fn_unset_BuiltInContainer_sort(self, ascending):
      return lib.BuiltInContainer_sort(self.impl, ascending)

   @property
   def sort(self):
      if hasattr(self, 'fn_BuiltInContainer_sort'): return self.fn_BuiltInContainer_sort
      else: return self.fn_unset_BuiltInContainer_sort
   @sort.setter
   def sort(self, value):
      self.fn_BuiltInContainer_sort = value
      lib.Instance_setMethod(self.impl, "Sort".encode('u8'), cb_BuiltInContainer_sort)

class Distance(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

Distance.buc = Distance

class Centimeters(Distance):
   def __init__(self, value = 0, impl = None):
      if impl is not None: self.impl = impl
      elif isinstance(value, Distance): self.impl = value.impl
      else: self.value = value

   # conv eC_Distance <-> eC_Meters
   @property
   def value(self): return self.impl * 100
   @value.setter
   def value(self, value): self.impl = value * 0.01

Centimeters.buc = Distance

@ffi.callback("eC_IteratorPointer *(eC_Container, tparam_Container_T)")
def cb_Container_add(__e, value):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_add(container, TA(value))

@ffi.callback("void(eC_Container, eC_Container)")
def cb_Container_copy(__e, source):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_copy(container, Container("", impl = source))

@ffi.callback("void(eC_Container, eC_IteratorPointer *)")
def cb_Container_delete(__e, i):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_delete(container, IteratorPointer(impl = i))

@ffi.callback("eC_IteratorPointer *(eC_Container, tparam_Container_D)")
def cb_Container_find(__e, value):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_find(container, TA(value))

@ffi.callback("void(eC_Container)")
def cb_Container_free(__e):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_free(container)

@ffi.callback("void(eC_Container, eC_IteratorPointer *)")
def cb_Container_freeIterator(__e, it):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_freeIterator(container, IteratorPointer(impl = it))

@ffi.callback("eC_IteratorPointer *(eC_Container, tparam_Container_I, eC_bool, eC_bool *)")
def cb_Container_getAtPosition(__e, pos, create, justAdded):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_getAtPosition(container, TA(pos), create, justAdded)

@ffi.callback("int(eC_Container)")
def cb_Container_getCount(__e):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_getCount(container)

@ffi.callback("uint64_t(eC_Container, eC_IteratorPointer *)")
def cb_Container_getData(__e, pointer):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_getData(container, IteratorPointer(impl = pointer))

@ffi.callback("eC_IteratorPointer *(eC_Container)")
def cb_Container_getFirst(__e):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_getFirst(container)

@ffi.callback("eC_IteratorPointer *(eC_Container)")
def cb_Container_getLast(__e):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_getLast(container)

@ffi.callback("eC_IteratorPointer *(eC_Container, eC_IteratorPointer *)")
def cb_Container_getNext(__e, pointer):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_getNext(container, IteratorPointer(impl = pointer))

@ffi.callback("eC_IteratorPointer *(eC_Container, eC_IteratorPointer *)")
def cb_Container_getPrev(__e, pointer):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_getPrev(container, IteratorPointer(impl = pointer))

@ffi.callback("eC_IteratorPointer *(eC_Container, eC_IteratorPointer *, tparam_Container_T)")
def cb_Container_insert(__e, after, value):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_insert(container, IteratorPointer(impl = after), TA(value))

@ffi.callback("void(eC_Container, eC_IteratorPointer *, eC_IteratorPointer *)")
def cb_Container_move(__e, it, after):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_move(container, IteratorPointer(impl = it), IteratorPointer(impl = after))

@ffi.callback("void(eC_Container, eC_IteratorPointer *)")
def cb_Container_remove(__e, it):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_remove(container, IteratorPointer(impl = it))

@ffi.callback("void(eC_Container)")
def cb_Container_removeAll(__e):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_removeAll(container)

@ffi.callback("eC_bool(eC_Container, eC_IteratorPointer *, tparam_Container_D)")
def cb_Container_setData(__e, pointer, data):
   container = pyOrNewObject(Container, __e)
   return container.fn_Container_setData(container, IteratorPointer(impl = pointer), TA(data))

@ffi.callback("void(eC_Container, eC_bool)")
def cb_Container_sort(__e, ascending):
   container = pyOrNewObject(Container, __e)
   container.fn_Container_sort(container, ascending)

class Container(Instance):
   # hack: hardcoded content
   class_members = [ "copySrc" ]
   def __init__(self, *args, **kwArgs): self.init_args(list(args), kwArgs)
   def init_args(self, args, kwArgs):
      if kwArgs.get("impl") is None:
         templateParams = kwArgs.get("templateParams")
         if templateParams is None:
            if len(args) > 0 and type(args[0]) == str:
               kwArgs["templateParams"] = args[0]
               del args[0]
            else:
               copySrc = kwArgs.get("copySrc")
               if copySrc is None and len(args) > 0: copySrc = args[0]
               if isinstance(copySrc, list) and len(copySrc) > 0:
                  co = coFromPy(copySrc[0])
                  if co is not None:
                     type(co)
                     kwArgs["templateParams"] = "<" + ffis(co.name) + ">"
      init_args(Container, self, args, kwArgs)

   def __getitem__(self, index):
      itPtr = self.getAtPosition(index, False, None)
      if itPtr == ffi.NULL: raise IndexError()
      d = OTA(self.impl._class.templateArgs[0].dataTypeClass, self.getData(itPtr))
      return d

   def __setitem__(self, index, d):
      itPtr = self.getAtPosition(index, False, None)
      if itPtr == ffi.NULL: raise IndexError()
      self.impl._class.templateArgs[0].dataTypeClass, self.setData(itPtr, TA(d))

   def __len__(self): return self.getCount()

   @property
   def copySrc(self): return lib.Container_get_copySrc(self.impl)
   @copySrc.setter
   def copySrc(self, value):
      if isinstance(value, Container):
         lib.Container_set_copySrc(self.impl, value.impl)
      if isinstance(value, list):
         count = len(value)
         co = ffi.NULL
         dt = self.impl._class.templateArgs[0].dataTypeClass
         if dt != ffi.NULL:
            co = dt
         if count > 0:
            v0 = value[0]
            pc = None
            if co is None: co = coFromPy(v0)
            if co is not None:
               pcn = ffi.string(co.name).decode('u8')
               for ag in app.appGlobals:
                  pc = ag.get(pcn, None)
                  if pc is not None: break

            if co == ffi.NULL:
               # TODO: Have more type checks?
               data = ffi.NULL
            else:
               ag_ffi = app.ffi
               if co.type == lib.ClassType_normalClass or co.type == lib.ClassType_noHeadClass:
                  data = ag_ffi.new("void *[]", count)
                  for i in range(0, count):
                     v = value[i]
                     if isinstance(v, tuple) and pc is not None: v = pc(v)
                     data[i] = v.impl if v is not None else ffi.NULL
               elif co.type == lib.ClassType_structClass:
                  data = ag_ffi.new(ffi.string(co.name).decode('u8') + "[]", count)
                  for i in range(0, count):
                     v = value[i]
                     if pc and not isinstance(v, pc): v = pc(v)
                     if v is not None: data[i] = v.impl[0]
               else:
                  if co == lib.class_int: data = ag_ffi.new("int []", value)
                  elif co == lib.class_float: data = ag_ffi.new("float []", value)
                  elif co == lib.class_double: data = ag_ffi.new("double []", value)
                  else:
                     data = ag_ffi.new("uint[]", count)   # TODO: Determine proper data type / size (bit classes, units, enums, system)
                     for i in range(0, count):
                        v = value[i]
                        if isinstance(v, tuple) and pc is not None: v = pc(v)
                        data[i] = v.impl if v is not None else 0
         else:
            data = ffi.NULL

         bic = ffi.new("eC_BuiltInContainer *", {
               '_vTbl'     : lib.class_BuiltInContainer._vTbl,
               '_class'    : lib.class_BuiltInContainer,
               'data'      : data,
               '_refCount' : 0,
               'count'     : count,
               'type'      : co
               })

         lib.Container_set_copySrc(self.impl, ffi.cast("eC_Container", bic))
   # hack: end of hardcoded content

   def fn_unset_Container_add(self, value):
      return lib.Container_add(self.impl, value)

   @property
   def add(self):
      if hasattr(self, 'fn_Container_add'): return self.fn_Container_add
      else: return self.fn_unset_Container_add
   @add.setter
   def add(self, value):
      self.fn_Container_add = value
      lib.Instance_setMethod(self.impl, "Add".encode('u8'), cb_Container_add)

   def fn_unset_Container_copy(self, source):
      return lib.Container_copy(self.impl, ffi.NULL if source is None else source.impl)

   @property
   def copy(self):
      if hasattr(self, 'fn_Container_copy'): return self.fn_Container_copy
      else: return self.fn_unset_Container_copy
   @copy.setter
   def copy(self, value):
      self.fn_Container_copy = value
      lib.Instance_setMethod(self.impl, "Copy".encode('u8'), cb_Container_copy)

   def fn_unset_Container_delete(self, i):
      return lib.Container_delete(self.impl, i)

   @property
   def delete(self):
      if hasattr(self, 'fn_Container_delete'): return self.fn_Container_delete
      else: return self.fn_unset_Container_delete
   @delete.setter
   def delete(self, value):
      self.fn_Container_delete = value
      lib.Instance_setMethod(self.impl, "Delete".encode('u8'), cb_Container_delete)

   def fn_unset_Container_find(self, value):
      return lib.Container_find(self.impl, value)

   @property
   def find(self):
      if hasattr(self, 'fn_Container_find'): return self.fn_Container_find
      else: return self.fn_unset_Container_find
   @find.setter
   def find(self, value):
      self.fn_Container_find = value
      lib.Instance_setMethod(self.impl, "Find".encode('u8'), cb_Container_find)

   def fn_unset_Container_free(self):
      return lib.Container_free(self.impl)

   @property
   def free(self):
      if hasattr(self, 'fn_Container_free'): return self.fn_Container_free
      else: return self.fn_unset_Container_free
   @free.setter
   def free(self, value):
      self.fn_Container_free = value
      lib.Instance_setMethod(self.impl, "Free".encode('u8'), cb_Container_free)

   def fn_unset_Container_freeIterator(self, it):
      return lib.Container_freeIterator(self.impl, it)

   @property
   def freeIterator(self):
      if hasattr(self, 'fn_Container_freeIterator'): return self.fn_Container_freeIterator
      else: return self.fn_unset_Container_freeIterator
   @freeIterator.setter
   def freeIterator(self, value):
      self.fn_Container_freeIterator = value
      lib.Instance_setMethod(self.impl, "FreeIterator".encode('u8'), cb_Container_freeIterator)

   def fn_unset_Container_getAtPosition(self, pos, create, justAdded):
      if justAdded is None: justAdded = ffi.NULL
      return lib.Container_getAtPosition(self.impl, pos, create, justAdded)

   @property
   def getAtPosition(self):
      if hasattr(self, 'fn_Container_getAtPosition'): return self.fn_Container_getAtPosition
      else: return self.fn_unset_Container_getAtPosition
   @getAtPosition.setter
   def getAtPosition(self, value):
      self.fn_Container_getAtPosition = value
      lib.Instance_setMethod(self.impl, "GetAtPosition".encode('u8'), cb_Container_getAtPosition)

   def fn_unset_Container_getCount(self):
      return lib.Container_getCount(self.impl)

   @property
   def getCount(self):
      if hasattr(self, 'fn_Container_getCount'): return self.fn_Container_getCount
      else: return self.fn_unset_Container_getCount
   @getCount.setter
   def getCount(self, value):
      self.fn_Container_getCount = value
      lib.Instance_setMethod(self.impl, "GetCount".encode('u8'), cb_Container_getCount)

   def fn_unset_Container_getData(self, pointer):
      return lib.Container_getData(self.impl, pointer)

   @property
   def getData(self):
      if hasattr(self, 'fn_Container_getData'): return self.fn_Container_getData
      else: return self.fn_unset_Container_getData
   @getData.setter
   def getData(self, value):
      self.fn_Container_getData = value
      lib.Instance_setMethod(self.impl, "GetData".encode('u8'), cb_Container_getData)

   def fn_unset_Container_getFirst(self):
      return lib.Container_getFirst(self.impl)

   @property
   def getFirst(self):
      if hasattr(self, 'fn_Container_getFirst'): return self.fn_Container_getFirst
      else: return self.fn_unset_Container_getFirst
   @getFirst.setter
   def getFirst(self, value):
      self.fn_Container_getFirst = value
      lib.Instance_setMethod(self.impl, "GetFirst".encode('u8'), cb_Container_getFirst)

   def fn_unset_Container_getLast(self):
      return lib.Container_getLast(self.impl)

   @property
   def getLast(self):
      if hasattr(self, 'fn_Container_getLast'): return self.fn_Container_getLast
      else: return self.fn_unset_Container_getLast
   @getLast.setter
   def getLast(self, value):
      self.fn_Container_getLast = value
      lib.Instance_setMethod(self.impl, "GetLast".encode('u8'), cb_Container_getLast)

   def fn_unset_Container_getNext(self, pointer):
      return lib.Container_getNext(self.impl, pointer)

   @property
   def getNext(self):
      if hasattr(self, 'fn_Container_getNext'): return self.fn_Container_getNext
      else: return self.fn_unset_Container_getNext
   @getNext.setter
   def getNext(self, value):
      self.fn_Container_getNext = value
      lib.Instance_setMethod(self.impl, "GetNext".encode('u8'), cb_Container_getNext)

   def fn_unset_Container_getPrev(self, pointer):
      return lib.Container_getPrev(self.impl, pointer)

   @property
   def getPrev(self):
      if hasattr(self, 'fn_Container_getPrev'): return self.fn_Container_getPrev
      else: return self.fn_unset_Container_getPrev
   @getPrev.setter
   def getPrev(self, value):
      self.fn_Container_getPrev = value
      lib.Instance_setMethod(self.impl, "GetPrev".encode('u8'), cb_Container_getPrev)

   def fn_unset_Container_insert(self, after, value):
      return lib.Container_insert(self.impl, after, value)

   @property
   def insert(self):
      if hasattr(self, 'fn_Container_insert'): return self.fn_Container_insert
      else: return self.fn_unset_Container_insert
   @insert.setter
   def insert(self, value):
      self.fn_Container_insert = value
      lib.Instance_setMethod(self.impl, "Insert".encode('u8'), cb_Container_insert)

   def fn_unset_Container_move(self, it, after):
      return lib.Container_move(self.impl, it, after)

   @property
   def move(self):
      if hasattr(self, 'fn_Container_move'): return self.fn_Container_move
      else: return self.fn_unset_Container_move
   @move.setter
   def move(self, value):
      self.fn_Container_move = value
      lib.Instance_setMethod(self.impl, "Move".encode('u8'), cb_Container_move)

   def fn_unset_Container_remove(self, it):
      return lib.Container_remove(self.impl, it)

   @property
   def remove(self):
      if hasattr(self, 'fn_Container_remove'): return self.fn_Container_remove
      else: return self.fn_unset_Container_remove
   @remove.setter
   def remove(self, value):
      self.fn_Container_remove = value
      lib.Instance_setMethod(self.impl, "Remove".encode('u8'), cb_Container_remove)

   def fn_unset_Container_removeAll(self):
      return lib.Container_removeAll(self.impl)

   @property
   def removeAll(self):
      if hasattr(self, 'fn_Container_removeAll'): return self.fn_Container_removeAll
      else: return self.fn_unset_Container_removeAll
   @removeAll.setter
   def removeAll(self, value):
      self.fn_Container_removeAll = value
      lib.Instance_setMethod(self.impl, "RemoveAll".encode('u8'), cb_Container_removeAll)

   def fn_unset_Container_setData(self, pointer, data):
      return lib.Container_setData(self.impl, pointer, data)

   @property
   def setData(self):
      if hasattr(self, 'fn_Container_setData'): return self.fn_Container_setData
      else: return self.fn_unset_Container_setData
   @setData.setter
   def setData(self, value):
      self.fn_Container_setData = value
      lib.Instance_setMethod(self.impl, "SetData".encode('u8'), cb_Container_setData)

   def fn_unset_Container_sort(self, ascending):
      return lib.Container_sort(self.impl, ascending)

   @property
   def sort(self):
      if hasattr(self, 'fn_Container_sort'): return self.fn_Container_sort
      else: return self.fn_unset_Container_sort
   @sort.setter
   def sort(self, value):
      self.fn_Container_sort = value
      lib.Instance_setMethod(self.impl, "Sort".encode('u8'), cb_Container_sort)

   def takeOut(self, d):
      return lib.Container_takeOut(self.impl, TA(d))

@ffi.callback("uintsize(eC_IOChannel, void *, uintsize)")
def cb_IOChannel_readData(__e, data, numBytes):
   iochannel = pyOrNewObject(IOChannel, __e)
   return iochannel.fn_IOChannel_readData(iochannel, data, numBytes)

@ffi.callback("uintsize(eC_IOChannel, const void *, uintsize)")
def cb_IOChannel_writeData(__e, data, numBytes):
   iochannel = pyOrNewObject(IOChannel, __e)
   return iochannel.fn_IOChannel_writeData(iochannel, data, numBytes)

class IOChannel(Instance):
   class_members = [
                      'readData',
                      'writeData',
                   ]

   def init_args(self, args, kwArgs): init_args(IOChannel, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   def get(self, data):
      _data = pyAddrTypedObject(data)
      lib.IOChannel_get(self.impl, *_data)
      return pyRetAddrTypedObject(data, _data[1])

   def put(self, data):
      lib.IOChannel_put(self.impl, *pyTypedObject(data))

   def fn_unset_IOChannel_readData(self, data, numBytes):
      return lib.IOChannel_readData(self.impl, data, numBytes)

   @property
   def readData(self):
      if hasattr(self, 'fn_IOChannel_readData'): return self.fn_IOChannel_readData
      else: return self.fn_unset_IOChannel_readData
   @readData.setter
   def readData(self, value):
      self.fn_IOChannel_readData = value
      lib.Instance_setMethod(self.impl, "ReadData".encode('u8'), cb_IOChannel_readData)

   def serialize(self, data):
      lib.IOChannel_serialize(self.impl, *pyTypedObject(data))

   def unserialize(self, data):
      _data = pyAddrTypedObject(data)
      lib.IOChannel_unserialize(self.impl, *_data)
      return pyRetAddrTypedObject(data, _data[1])

   def fn_unset_IOChannel_writeData(self, data, numBytes):
      return lib.IOChannel_writeData(self.impl, data, numBytes)

   @property
   def writeData(self):
      if hasattr(self, 'fn_IOChannel_writeData'): return self.fn_IOChannel_writeData
      else: return self.fn_unset_IOChannel_writeData
   @writeData.setter
   def writeData(self, value):
      self.fn_IOChannel_writeData = value
      lib.Instance_setMethod(self.impl, "WriteData".encode('u8'), cb_IOChannel_writeData)

class Meters(Distance):
   def __init__(self, value = 0, impl = None):
      if impl is not None: self.impl = impl
      elif isinstance(value, Distance): self.impl = value.impl
      else: self.impl = value
   @property
   def value(self): return self.impl
   @value.setter
   def value(self, value): self.impl = value

Meters.buc = Distance

class Radians(Angle):
   def __init__(self, value = 0, impl = None):
      if impl is not None: self.impl = impl
      elif isinstance(value, Angle): self.impl = value.impl
      else: self.impl = value
   @property
   def value(self): return self.impl
   @value.setter
   def value(self, value): self.impl = value

Radians.buc = Angle

class String:
   def __init__(self, impl = None):
      if impl is not None: self.impl = impl
      else: self.impl = ffi.NULL

   def __str__(self): return ffi.string(self.impl).decode('u8') if self.impl != ffi.NULL else str()

class ZString(Instance):
   def __new__(cls,
               _string = None,
               len = None,
               allocType = None,
               size = None,
               minSize = None,
               maxSize = None,
               string = None,
               impl = None):
      if isinstance(_string, str):
         impl = lib.ZString_from_String(_string.encode('u8'))
         if impl == ffi.NULL: return None
      o = Instance.__new__(cls)
      if impl is not None: o.impl = impl
      return o;
   class_members = [
                      '_string',
                      'len',
                      'allocType',
                      'size',
                      'minSize',
                      'maxSize',
                      'string',
                   ]

   def init_args(self, args, kwArgs): init_args(ZString, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      if hasattr(self, 'impl'): return
      self.init_args(list(args), kwArgs)

   @property
   def _string(self): return IPTR(lib, ffi, self, ZString)._string
   @_string.setter
   def _string(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      IPTR(lib, ffi, self, ZString)._string = value

   @property
   def len(self): return IPTR(lib, ffi, self, ZString).len
   @len.setter
   def len(self, value): IPTR(lib, ffi, self, ZString).len = value

   @property
   def allocType(self): return StringAllocType(impl = IPTR(lib, ffi, self, ZString).allocType)
   @allocType.setter
   def allocType(self, value): IPTR(lib, ffi, self, ZString).allocType = value.impl

   @property
   def size(self): return IPTR(lib, ffi, self, ZString).size
   @size.setter
   def size(self, value): IPTR(lib, ffi, self, ZString).size = value

   @property
   def minSize(self): return IPTR(lib, ffi, self, ZString).minSize
   @minSize.setter
   def minSize(self, value): IPTR(lib, ffi, self, ZString).minSize = value

   @property
   def maxSize(self): return IPTR(lib, ffi, self, ZString).maxSize
   @maxSize.setter
   def maxSize(self, value): IPTR(lib, ffi, self, ZString).maxSize = value

   @property
   def string(self): value = lib.ZString_get_string(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @string.setter
   def string(self, value):
      lib.ZString_set_string(self.impl, value.encode('u8'))

   def __str__(self): return ffi.string(lib.ZString_to_char_ptr(self.impl)).decode('u8') if self.impl != ffi.NULL else str()

   # def ZString_to_String(self): return pyOrNewObject(String, lib.ZString_to_String(self.impl))

   # here is an unhandled conversion: ZString::String (NormalClass 2 NormalClass)
   # ZString_to_String
   # ZString_from_String

   def concat(self, s = None):
      if s is not None and not isinstance(s, ZString): s = ZString(s)
      s = ffi.NULL if s is None else s.impl
      lib.ZString_concat(self.impl, s)

   def concatf(self, format, *args):
      if isinstance(format, str): format = ffi.new("char[]", format.encode('u8'))
      elif format is None: format = ffi.NULL
      lib.ZString_concatf(self.impl, format, *ellipsisArgs(args))

   def concatn(self, s, l):
      if s is not None and not isinstance(s, ZString): s = ZString(s)
      s = ffi.NULL if s is None else s.impl
      lib.ZString_concatn(self.impl, s, l)

   def concatx(self, *args): lib.ZString_concatx(self.impl, *convertTypedArgs(args))

   def copy(self, s = None):
      if s is not None and not isinstance(s, ZString): s = ZString(s)
      s = ffi.NULL if s is None else s.impl
      lib.ZString_copy(self.impl, s)

   def copyString(self, value, newLen):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      lib.ZString_copyString(self.impl, value, newLen)

Pi = Radians ( 3.1415926535897932384626433832795028841971 )

class AccessMode:
   defaultAccess    = lib.AccessMode_defaultAccess
   publicAccess     = lib.AccessMode_publicAccess
   privateAccess    = lib.AccessMode_privateAccess
   staticAccess     = lib.AccessMode_staticAccess
   baseSystemAccess = lib.AccessMode_baseSystemAccess

class BTNamedLink:
   def __init__(self, name = None, parent = None, left = None, right = None, depth = None, data = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_BTNamedLink *", lib.Instance_new(lib.class_BTNamedLink))
         if isinstance(name, tuple):
            __tuple = name
            name = None
            if len(__tuple) > 0: name   = __tuple[0]
            if len(__tuple) > 1: parent = __tuple[1]
            if len(__tuple) > 2: left   = __tuple[2]
            if len(__tuple) > 3: right  = __tuple[3]
         if name is not None:   self.name   = name
         if parent is not None: self.parent = parent
         if left is not None:   self.left   = left
         if right is not None:  self.right  = right
         if depth is not None:  self.depth  = depth
         if data is not None:   self.data   = data

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def parent(self): return self.impl.parent
   @parent.setter
   def parent(self, value):
      if not isinstance(value, BTNamedLink): value = BTNamedLink(value)
      self.impl.parent = value.impl

   @property
   def left(self): return self.impl.left
   @left.setter
   def left(self, value):
      if not isinstance(value, BTNamedLink): value = BTNamedLink(value)
      self.impl.left = value.impl

   @property
   def right(self): return self.impl.right
   @right.setter
   def right(self, value):
      if not isinstance(value, BTNamedLink): value = BTNamedLink(value)
      self.impl.right = value.impl

   @property
   def depth(self): return self.impl.depth
   @depth.setter
   def depth(self, value): self.impl.depth = value

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

class BackSlashEscaping(Bool):
   forArgsPassing = bool(lib.BackSlashEscaping_forArgsPassing)

class BitMember:
   def __init__(self, prev = None, next = None, name = None, isProperty = None, memberAccess = None, id = None, _class = None, dataTypeString = None, dataTypeClass = None, dataType = None, type = None, size = None, pos = None, mask = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_BitMember *", lib.Instance_new(lib.class_BitMember))
         if prev is not None:           self.prev           = prev
         if next is not None:           self.next           = next
         if name is not None:           self.name           = name
         if isProperty is not None:     self.isProperty     = isProperty
         if memberAccess is not None:   self.memberAccess   = memberAccess
         if id is not None:             self.id             = id
         if _class is not None:         self._class         = _class
         if dataTypeString is not None: self.dataTypeString = dataTypeString
         if dataTypeClass is not None:  self.dataTypeClass  = dataTypeClass
         if dataType is not None:       self.dataType       = dataType
         if type is not None:           self.type           = type
         if size is not None:           self.size           = size
         if pos is not None:            self.pos            = pos
         if mask is not None:           self.mask           = mask

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, BitMember): value = BitMember(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, BitMember): value = BitMember(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def isProperty(self): return self.impl.isProperty
   @isProperty.setter
   def isProperty(self, value): self.impl.isProperty = value

   @property
   def memberAccess(self): return self.impl.memberAccess
   @memberAccess.setter
   def memberAccess(self, value): self.impl.memberAccess = value

   @property
   def id(self): return self.impl.id
   @id.setter
   def id(self, value): self.impl.id = value

   @property
   def _class(self): return self.impl._class
   @_class.setter
   def _class(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl._class = value.impl

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataTypeClass(self): return self.impl.dataTypeClass
   @dataTypeClass.setter
   def dataTypeClass(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.dataTypeClass = value.impl

   @property
   def dataType(self): return self.impl.dataType
   @dataType.setter
   def dataType(self, value): self.impl.dataType = value.impl

   @property
   def type(self): return self.impl.type
   @type.setter
   def type(self, value): self.impl.type = value

   @property
   def size(self): return self.impl.size
   @size.setter
   def size(self, value): self.impl.size = value

   @property
   def pos(self): return self.impl.pos
   @pos.setter
   def pos(self, value): self.impl.pos = value

   @property
   def mask(self): return self.impl.mask
   @mask.setter
   def mask(self, value): self.impl.mask = value

class Box(Struct):
   def __init__(self, left = 0, top = 0, right = 0, bottom = 0, width = None, height = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_Box *", impl)
      else:
         if isinstance(left, tuple):
            __tuple = left
            left = 0
            if len(__tuple) > 0: left   = __tuple[0]
            if len(__tuple) > 1: top    = __tuple[1]
            if len(__tuple) > 2: right  = __tuple[2]
            if len(__tuple) > 3: bottom = __tuple[3]
         self.impl = ffi.new("eC_Box *", { 'left' : left, 'top' : top, 'right' : right, 'bottom' : bottom })
         if width is not None:  self.width       = width
         if height is not None: self.height      = height

   @property
   def left(self): return self.impl.left
   @left.setter
   def left(self, value): self.impl.left = value

   @property
   def top(self): return self.impl.top
   @top.setter
   def top(self, value): self.impl.top = value

   @property
   def right(self): return self.impl.right
   @right.setter
   def right(self, value): self.impl.right = value

   @property
   def bottom(self): return self.impl.bottom
   @bottom.setter
   def bottom(self, value): self.impl.bottom = value

   @property
   def width(self): return lib.Box_get_width(self.impl)
   @width.setter
   def width(self, value):
      lib.Box_set_width(self.impl, value)

   @property
   def height(self): return lib.Box_get_height(self.impl)
   @height.setter
   def height(self, value):
      lib.Box_set_height(self.impl, value)

   def clip(self, against = None):
      if against is not None and not isinstance(against, Box): against = Box(against)
      against = ffi.NULL if against is None else against.impl
      lib.Box_clip(ffi.cast("eC_Box *", self.impl), ffi.cast("eC_Box *", against))

   def clipOffset(self, against, x, y):
      if against is not None and not isinstance(against, Box): against = Box(against)
      against = ffi.NULL if against is None else against.impl
      lib.Box_clipOffset(ffi.cast("eC_Box *", self.impl), ffi.cast("eC_Box *", against), x, y)

   def isPointInside(self, point = None):
      if point is not None and not isinstance(point, Point): point = Point(point)
      point = ffi.NULL if point is None else point.impl
      return lib.Box_isPointInside(ffi.cast("eC_Box *", self.impl), ffi.cast("eC_Point *", point))

   def overlap(self, box = None):
      if box is not None and not isinstance(box, Box): box = Box(box)
      box = ffi.NULL if box is None else box.impl
      return lib.Box_overlap(ffi.cast("eC_Box *", self.impl), ffi.cast("eC_Box *", box))

class CIString:
   def __init__(self, impl = None):
      String.__init__(self, impl = impl)
      if impl is not None: self.impl = impl
      else: self.impl = ffi.NULL

class Class:
   def __init__(self,
                prev = None,
                next = None,
                name = None,
                offset = None,
                structSize = None,
                _vTbl = None,
                vTblSize = None,
                Constructor = None,
                Destructor = None,
                offsetClass = None,
                sizeClass = None,
                base = None,
                methods = None,
                members = None,
                prop = None,
                membersAndProperties = None,
                classProperties = None,
                derivatives = None,
                memberID = None,
                startMemberID = None,
                type = None,
                module = None,
                nameSpace = None,
                dataTypeString = None,
                dataType = None,
                typeSize = None,
                defaultAlignment = None,
                Initialize = None,
                memberOffset = None,
                selfWatchers = None,
                designerClass = None,
                noExpansion = None,
                defaultProperty = None,
                comRedefinition = None,
                count = None,
                isRemote = None,
                internalDecl = None,
                data = None,
                computeSize = None,
                structAlignment = None,
                pointerAlignment = None,
                destructionWatchOffset = None,
                fixed = None,
                delayedCPValues = None,
                inheritanceAccess = None,
                fullName = None,
                symbol = None,
                conversions = None,
                templateParams = None,
                templateArgs = None,
                templateClass = None,
                templatized = None,
                numParams = None,
                isInstanceClass = None,
                byValueSystemClass = None,
                bindingsClass = None,
                impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(prev, pyType):
         self.impl = getattr(app.lib, 'class_' + prev.__name__)
      else:
         self.impl = ffi.cast("eC_Class *", lib.Instance_new(lib.class_Class))
         if prev is not None:                   self.prev                   = prev
         if next is not None:                   self.next                   = next
         if name is not None:                   self.name                   = name
         if offset is not None:                 self.offset                 = offset
         if structSize is not None:             self.structSize             = structSize
         if _vTbl is not None:                  self._vTbl                  = _vTbl
         if vTblSize is not None:               self.vTblSize               = vTblSize
         if Constructor is not None:            self.Constructor            = Constructor
         if Destructor is not None:             self.Destructor             = Destructor
         if offsetClass is not None:            self.offsetClass            = offsetClass
         if sizeClass is not None:              self.sizeClass              = sizeClass
         if base is not None:                   self.base                   = base
         if methods is not None:                self.methods                = methods
         if members is not None:                self.members                = members
         if prop is not None:                   self.prop                   = prop
         if membersAndProperties is not None:   self.membersAndProperties   = membersAndProperties
         if classProperties is not None:        self.classProperties        = classProperties
         if derivatives is not None:            self.derivatives            = derivatives
         if memberID is not None:               self.memberID               = memberID
         if startMemberID is not None:          self.startMemberID          = startMemberID
         if type is not None:                   self.type                   = type
         if module is not None:                 self.module                 = module
         if nameSpace is not None:              self.nameSpace              = nameSpace
         if dataTypeString is not None:         self.dataTypeString         = dataTypeString
         if dataType is not None:               self.dataType               = dataType
         if typeSize is not None:               self.typeSize               = typeSize
         if defaultAlignment is not None:       self.defaultAlignment       = defaultAlignment
         if Initialize is not None:             self.Initialize             = Initialize
         if memberOffset is not None:           self.memberOffset           = memberOffset
         if selfWatchers is not None:           self.selfWatchers           = selfWatchers
         if designerClass is not None:          self.designerClass          = designerClass
         if noExpansion is not None:            self.noExpansion            = noExpansion
         if defaultProperty is not None:        self.defaultProperty        = defaultProperty
         if comRedefinition is not None:        self.comRedefinition        = comRedefinition
         if count is not None:                  self.count                  = count
         if isRemote is not None:               self.isRemote               = isRemote
         if internalDecl is not None:           self.internalDecl           = internalDecl
         if data is not None:                   self.data                   = data
         if computeSize is not None:            self.computeSize            = computeSize
         if structAlignment is not None:        self.structAlignment        = structAlignment
         if pointerAlignment is not None:       self.pointerAlignment       = pointerAlignment
         if destructionWatchOffset is not None: self.destructionWatchOffset = destructionWatchOffset
         if fixed is not None:                  self.fixed                  = fixed
         if delayedCPValues is not None:        self.delayedCPValues        = delayedCPValues
         if inheritanceAccess is not None:      self.inheritanceAccess      = inheritanceAccess
         if fullName is not None:               self.fullName               = fullName
         if symbol is not None:                 self.symbol                 = symbol
         if conversions is not None:            self.conversions            = conversions
         if templateParams is not None:         self.templateParams         = templateParams
         if templateArgs is not None:           self.templateArgs           = templateArgs
         if templateClass is not None:          self.templateClass          = templateClass
         if templatized is not None:            self.templatized            = templatized
         if numParams is not None:              self.numParams              = numParams
         if isInstanceClass is not None:        self.isInstanceClass        = isInstanceClass
         if byValueSystemClass is not None:     self.byValueSystemClass     = byValueSystemClass
         if bindingsClass is not None:          self.bindingsClass          = bindingsClass

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def offset(self): return self.impl.offset
   @offset.setter
   def offset(self, value): self.impl.offset = value

   @property
   def structSize(self): return self.impl.structSize
   @structSize.setter
   def structSize(self, value): self.impl.structSize = value

   @property
   def _vTbl(self): return self.impl._vTbl
   @_vTbl.setter
   def _vTbl(self, value): self.impl._vTbl = value

   @property
   def vTblSize(self): return self.impl.vTblSize
   @vTblSize.setter
   def vTblSize(self, value): self.impl.vTblSize = value

   @property
   def Constructor(self): return self.impl.Constructor
   @Constructor.setter
   def Constructor(self, value): self.impl.Constructor = value

   @property
   def Destructor(self): return self.impl.Destructor
   @Destructor.setter
   def Destructor(self, value): self.impl.Destructor = value

   @property
   def offsetClass(self): return self.impl.offsetClass
   @offsetClass.setter
   def offsetClass(self, value): self.impl.offsetClass = value

   @property
   def sizeClass(self): return self.impl.sizeClass
   @sizeClass.setter
   def sizeClass(self, value): self.impl.sizeClass = value

   @property
   def base(self): return self.impl.base
   @base.setter
   def base(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.base = value.impl

   @property
   def methods(self): return BinaryTree(impl = self.impl.methods)
   @methods.setter
   def methods(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.methods = value.impl[0]

   @property
   def members(self): return BinaryTree(impl = self.impl.members)
   @members.setter
   def members(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.members = value.impl[0]

   @property
   def prop(self): return BinaryTree(impl = self.impl.prop)
   @prop.setter
   def prop(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.prop = value.impl[0]

   @property
   def membersAndProperties(self): return OldList(impl = self.impl.membersAndProperties)
   @membersAndProperties.setter
   def membersAndProperties(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.membersAndProperties = value.impl[0]

   @property
   def classProperties(self): return BinaryTree(impl = self.impl.classProperties)
   @classProperties.setter
   def classProperties(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.classProperties = value.impl[0]

   @property
   def derivatives(self): return OldList(impl = self.impl.derivatives)
   @derivatives.setter
   def derivatives(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.derivatives = value.impl[0]

   @property
   def memberID(self): return self.impl.memberID
   @memberID.setter
   def memberID(self, value): self.impl.memberID = value

   @property
   def startMemberID(self): return self.impl.startMemberID
   @startMemberID.setter
   def startMemberID(self, value): self.impl.startMemberID = value

   @property
   def type(self): return self.impl.type
   @type.setter
   def type(self, value): self.impl.type = value

   @property
   def module(self): return pyOrNewObject(Module, self.impl.module)
   @module.setter
   def module(self, value):
      if not isinstance(value, Module): value = Module(value)
      self.impl.module = value.impl

   @property
   def nameSpace(self): return self.impl.nameSpace
   @nameSpace.setter
   def nameSpace(self, value): self.impl.nameSpace = value

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataType(self): return self.impl.dataType
   @dataType.setter
   def dataType(self, value): self.impl.dataType = value.impl

   @property
   def typeSize(self): return self.impl.typeSize
   @typeSize.setter
   def typeSize(self, value): self.impl.typeSize = value

   @property
   def defaultAlignment(self): return self.impl.defaultAlignment
   @defaultAlignment.setter
   def defaultAlignment(self, value): self.impl.defaultAlignment = value

   @property
   def Initialize(self): return self.impl.Initialize
   @Initialize.setter
   def Initialize(self, value): self.impl.Initialize = value

   @property
   def memberOffset(self): return self.impl.memberOffset
   @memberOffset.setter
   def memberOffset(self, value): self.impl.memberOffset = value

   @property
   def selfWatchers(self): return OldList(impl = self.impl.selfWatchers)
   @selfWatchers.setter
   def selfWatchers(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.selfWatchers = value.impl[0]

   @property
   def designerClass(self): return self.impl.designerClass
   @designerClass.setter
   def designerClass(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.designerClass = value

   @property
   def noExpansion(self): return self.impl.noExpansion
   @noExpansion.setter
   def noExpansion(self, value): self.impl.noExpansion = value

   @property
   def defaultProperty(self): return self.impl.defaultProperty
   @defaultProperty.setter
   def defaultProperty(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.defaultProperty = value

   @property
   def comRedefinition(self): return self.impl.comRedefinition
   @comRedefinition.setter
   def comRedefinition(self, value): self.impl.comRedefinition = value

   @property
   def count(self): return self.impl.count
   @count.setter
   def count(self, value): self.impl.count = value

   @property
   def isRemote(self): return self.impl.isRemote
   @isRemote.setter
   def isRemote(self, value): self.impl.isRemote = value

   @property
   def internalDecl(self): return self.impl.internalDecl
   @internalDecl.setter
   def internalDecl(self, value): self.impl.internalDecl = value

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

   @property
   def computeSize(self): return self.impl.computeSize
   @computeSize.setter
   def computeSize(self, value): self.impl.computeSize = value

   @property
   def structAlignment(self): return self.impl.structAlignment
   @structAlignment.setter
   def structAlignment(self, value): self.impl.structAlignment = value

   @property
   def pointerAlignment(self): return self.impl.pointerAlignment
   @pointerAlignment.setter
   def pointerAlignment(self, value): self.impl.pointerAlignment = value

   @property
   def destructionWatchOffset(self): return self.impl.destructionWatchOffset
   @destructionWatchOffset.setter
   def destructionWatchOffset(self, value): self.impl.destructionWatchOffset = value

   @property
   def fixed(self): return self.impl.fixed
   @fixed.setter
   def fixed(self, value): self.impl.fixed = value

   @property
   def delayedCPValues(self): return OldList(impl = self.impl.delayedCPValues)
   @delayedCPValues.setter
   def delayedCPValues(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.delayedCPValues = value.impl[0]

   @property
   def inheritanceAccess(self): return self.impl.inheritanceAccess
   @inheritanceAccess.setter
   def inheritanceAccess(self, value): self.impl.inheritanceAccess = value

   @property
   def fullName(self): return self.impl.fullName
   @fullName.setter
   def fullName(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.fullName = value

   @property
   def symbol(self): return self.impl.symbol
   @symbol.setter
   def symbol(self, value): self.impl.symbol = value

   @property
   def conversions(self): return OldList(impl = self.impl.conversions)
   @conversions.setter
   def conversions(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.conversions = value.impl[0]

   @property
   def templateParams(self): return OldList(impl = self.impl.templateParams)
   @templateParams.setter
   def templateParams(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.templateParams = value.impl[0]

   @property
   def templateArgs(self): return self.impl.templateArgs
   @templateArgs.setter
   def templateArgs(self, value): self.impl.templateArgs = value

   @property
   def templateClass(self): return self.impl.templateClass
   @templateClass.setter
   def templateClass(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.templateClass = value.impl

   @property
   def templatized(self): return OldList(impl = self.impl.templatized)
   @templatized.setter
   def templatized(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.templatized = value.impl[0]

   @property
   def numParams(self): return self.impl.numParams
   @numParams.setter
   def numParams(self, value): self.impl.numParams = value

   @property
   def isInstanceClass(self): return self.impl.isInstanceClass
   @isInstanceClass.setter
   def isInstanceClass(self, value): self.impl.isInstanceClass = value

   @property
   def byValueSystemClass(self): return self.impl.byValueSystemClass
   @byValueSystemClass.setter
   def byValueSystemClass(self, value): self.impl.byValueSystemClass = value

   @property
   def bindingsClass(self): return self.impl.bindingsClass
   @bindingsClass.setter
   def bindingsClass(self, value): self.impl.bindingsClass = value

   def __str__(self): return ffi.string(lib.Class_to_char_ptr(self.impl)).decode('u8') if self.impl != ffi.NULL else str()

@ffi.callback("void(eC_ClassDesignerBase)")
def cb_ClassDesignerBase_addObject(__e):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_addObject(classdesignerbase)

@ffi.callback("void(eC_ClassDesignerBase, eC_Instance, eC_Size *, const char *, const char *)")
def cb_ClassDesignerBase_createNew(__e, editBox, clientSize, name, inherit):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_createNew(classdesignerbase, pyOrNewObject(Instance, editBox), Size(impl = clientSize), name.encode('u8'), inherit.encode('u8'))

@ffi.callback("void(eC_ClassDesignerBase, eC_DesignerBase, eC_Instance, eC_ObjectInfo *, eC_bool, eC_Instance)")
def cb_ClassDesignerBase_createObject(__e, designer, instance, object, isClass, _class):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_createObject(classdesignerbase, pyOrNewObject(DesignerBase, designer), pyOrNewObject(Instance, instance), ObjectInfo(impl = object), isClass, pyOrNewObject(Instance, _class))

@ffi.callback("void(eC_ClassDesignerBase, eC_Instance)")
def cb_ClassDesignerBase_destroyObject(__e, object):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_destroyObject(classdesignerbase, pyOrNewObject(Instance, object))

@ffi.callback("void(eC_ClassDesignerBase, eC_Instance, eC_ObjectInfo *, eC_bool, eC_Instance)")
def cb_ClassDesignerBase_droppedObject(__e, instance, object, isClass, _class):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_droppedObject(classdesignerbase, pyOrNewObject(Instance, instance), ObjectInfo(impl = object), isClass, pyOrNewObject(Instance, _class))

@ffi.callback("void(eC_ClassDesignerBase, eC_Property *, eC_Instance)")
def cb_ClassDesignerBase_fixProperty(__e, prop, object):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_fixProperty(classdesignerbase, Property(impl = prop), pyOrNewObject(Instance, object))

@ffi.callback("void(eC_ClassDesignerBase, eC_DesignerBase)")
def cb_ClassDesignerBase_listToolBoxClasses(__e, designer):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_listToolBoxClasses(classdesignerbase, pyOrNewObject(DesignerBase, designer))

@ffi.callback("void(eC_ClassDesignerBase, eC_Instance, eC_ObjectInfo *, eC_bool, eC_Instance)")
def cb_ClassDesignerBase_postCreateObject(__e, instance, object, isClass, _class):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_postCreateObject(classdesignerbase, pyOrNewObject(Instance, instance), ObjectInfo(impl = object), isClass, pyOrNewObject(Instance, _class))

@ffi.callback("void(eC_ClassDesignerBase, eC_DesignerBase, eC_Instance)")
def cb_ClassDesignerBase_prepareTestObject(__e, designer, test):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_prepareTestObject(classdesignerbase, pyOrNewObject(DesignerBase, designer), pyOrNewObject(Instance, test))

@ffi.callback("void(eC_ClassDesignerBase)")
def cb_ClassDesignerBase_reset(__e):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_reset(classdesignerbase)

@ffi.callback("void(eC_ClassDesignerBase, eC_ObjectInfo *, eC_Instance)")
def cb_ClassDesignerBase_selectObject(__e, object, control):
   classdesignerbase = pyOrNewObject(ClassDesignerBase, __e)
   classdesignerbase.fn_ClassDesignerBase_selectObject(classdesignerbase, ObjectInfo(impl = object), pyOrNewObject(Instance, control))

class ClassDesignerBase(Instance):
   class_members = [
                      'addObject',
                      'createNew',
                      'createObject',
                      'destroyObject',
                      'droppedObject',
                      'fixProperty',
                      'listToolBoxClasses',
                      'postCreateObject',
                      'prepareTestObject',
                      'reset',
                      'selectObject',
                   ]

   def init_args(self, args, kwArgs): init_args(ClassDesignerBase, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   def fn_unset_ClassDesignerBase_addObject(self):
      return lib.ClassDesignerBase_addObject(self.impl)

   @property
   def addObject(self):
      if hasattr(self, 'fn_ClassDesignerBase_addObject'): return self.fn_ClassDesignerBase_addObject
      else: return self.fn_unset_ClassDesignerBase_addObject
   @addObject.setter
   def addObject(self, value):
      self.fn_ClassDesignerBase_addObject = value
      lib.Instance_setMethod(self.impl, "AddObject".encode('u8'), cb_ClassDesignerBase_addObject)

   def fn_unset_ClassDesignerBase_createNew(self, editBox, clientSize, name, inherit):
      return lib.ClassDesignerBase_createNew(self.impl, ffi.NULL if editBox is None else editBox.impl, ffi.NULL if clientSize is None else clientSize.impl, name, inherit)

   @property
   def createNew(self):
      if hasattr(self, 'fn_ClassDesignerBase_createNew'): return self.fn_ClassDesignerBase_createNew
      else: return self.fn_unset_ClassDesignerBase_createNew
   @createNew.setter
   def createNew(self, value):
      self.fn_ClassDesignerBase_createNew = value
      lib.Instance_setMethod(self.impl, "CreateNew".encode('u8'), cb_ClassDesignerBase_createNew)

   def fn_unset_ClassDesignerBase_createObject(self, designer, instance, object, isClass, _class):
      return lib.ClassDesignerBase_createObject(self.impl, ffi.NULL if designer is None else designer.impl, ffi.NULL if instance is None else instance.impl, ffi.NULL if object is None else object.impl, isClass, ffi.NULL if _class is None else _class.impl)

   @property
   def createObject(self):
      if hasattr(self, 'fn_ClassDesignerBase_createObject'): return self.fn_ClassDesignerBase_createObject
      else: return self.fn_unset_ClassDesignerBase_createObject
   @createObject.setter
   def createObject(self, value):
      self.fn_ClassDesignerBase_createObject = value
      lib.Instance_setMethod(self.impl, "CreateObject".encode('u8'), cb_ClassDesignerBase_createObject)

   def fn_unset_ClassDesignerBase_destroyObject(self, object):
      return lib.ClassDesignerBase_destroyObject(self.impl, ffi.NULL if object is None else object.impl)

   @property
   def destroyObject(self):
      if hasattr(self, 'fn_ClassDesignerBase_destroyObject'): return self.fn_ClassDesignerBase_destroyObject
      else: return self.fn_unset_ClassDesignerBase_destroyObject
   @destroyObject.setter
   def destroyObject(self, value):
      self.fn_ClassDesignerBase_destroyObject = value
      lib.Instance_setMethod(self.impl, "DestroyObject".encode('u8'), cb_ClassDesignerBase_destroyObject)

   def fn_unset_ClassDesignerBase_droppedObject(self, instance, object, isClass, _class):
      return lib.ClassDesignerBase_droppedObject(self.impl, ffi.NULL if instance is None else instance.impl, ffi.NULL if object is None else object.impl, isClass, ffi.NULL if _class is None else _class.impl)

   @property
   def droppedObject(self):
      if hasattr(self, 'fn_ClassDesignerBase_droppedObject'): return self.fn_ClassDesignerBase_droppedObject
      else: return self.fn_unset_ClassDesignerBase_droppedObject
   @droppedObject.setter
   def droppedObject(self, value):
      self.fn_ClassDesignerBase_droppedObject = value
      lib.Instance_setMethod(self.impl, "DroppedObject".encode('u8'), cb_ClassDesignerBase_droppedObject)

   def fn_unset_ClassDesignerBase_fixProperty(self, prop, object):
      return lib.ClassDesignerBase_fixProperty(self.impl, ffi.NULL if prop is None else prop.impl, ffi.NULL if object is None else object.impl)

   @property
   def fixProperty(self):
      if hasattr(self, 'fn_ClassDesignerBase_fixProperty'): return self.fn_ClassDesignerBase_fixProperty
      else: return self.fn_unset_ClassDesignerBase_fixProperty
   @fixProperty.setter
   def fixProperty(self, value):
      self.fn_ClassDesignerBase_fixProperty = value
      lib.Instance_setMethod(self.impl, "FixProperty".encode('u8'), cb_ClassDesignerBase_fixProperty)

   def fn_unset_ClassDesignerBase_listToolBoxClasses(self, designer):
      return lib.ClassDesignerBase_listToolBoxClasses(self.impl, ffi.NULL if designer is None else designer.impl)

   @property
   def listToolBoxClasses(self):
      if hasattr(self, 'fn_ClassDesignerBase_listToolBoxClasses'): return self.fn_ClassDesignerBase_listToolBoxClasses
      else: return self.fn_unset_ClassDesignerBase_listToolBoxClasses
   @listToolBoxClasses.setter
   def listToolBoxClasses(self, value):
      self.fn_ClassDesignerBase_listToolBoxClasses = value
      lib.Instance_setMethod(self.impl, "ListToolBoxClasses".encode('u8'), cb_ClassDesignerBase_listToolBoxClasses)

   def fn_unset_ClassDesignerBase_postCreateObject(self, instance, object, isClass, _class):
      return lib.ClassDesignerBase_postCreateObject(self.impl, ffi.NULL if instance is None else instance.impl, ffi.NULL if object is None else object.impl, isClass, ffi.NULL if _class is None else _class.impl)

   @property
   def postCreateObject(self):
      if hasattr(self, 'fn_ClassDesignerBase_postCreateObject'): return self.fn_ClassDesignerBase_postCreateObject
      else: return self.fn_unset_ClassDesignerBase_postCreateObject
   @postCreateObject.setter
   def postCreateObject(self, value):
      self.fn_ClassDesignerBase_postCreateObject = value
      lib.Instance_setMethod(self.impl, "PostCreateObject".encode('u8'), cb_ClassDesignerBase_postCreateObject)

   def fn_unset_ClassDesignerBase_prepareTestObject(self, designer, test):
      return lib.ClassDesignerBase_prepareTestObject(self.impl, ffi.NULL if designer is None else designer.impl, ffi.NULL if test is None else test.impl)

   @property
   def prepareTestObject(self):
      if hasattr(self, 'fn_ClassDesignerBase_prepareTestObject'): return self.fn_ClassDesignerBase_prepareTestObject
      else: return self.fn_unset_ClassDesignerBase_prepareTestObject
   @prepareTestObject.setter
   def prepareTestObject(self, value):
      self.fn_ClassDesignerBase_prepareTestObject = value
      lib.Instance_setMethod(self.impl, "PrepareTestObject".encode('u8'), cb_ClassDesignerBase_prepareTestObject)

   def fn_unset_ClassDesignerBase_reset(self):
      return lib.ClassDesignerBase_reset(self.impl)

   @property
   def reset(self):
      if hasattr(self, 'fn_ClassDesignerBase_reset'): return self.fn_ClassDesignerBase_reset
      else: return self.fn_unset_ClassDesignerBase_reset
   @reset.setter
   def reset(self, value):
      self.fn_ClassDesignerBase_reset = value
      lib.Instance_setMethod(self.impl, "Reset".encode('u8'), cb_ClassDesignerBase_reset)

   def fn_unset_ClassDesignerBase_selectObject(self, object, control):
      return lib.ClassDesignerBase_selectObject(self.impl, ffi.NULL if object is None else object.impl, ffi.NULL if control is None else control.impl)

   @property
   def selectObject(self):
      if hasattr(self, 'fn_ClassDesignerBase_selectObject'): return self.fn_ClassDesignerBase_selectObject
      else: return self.fn_unset_ClassDesignerBase_selectObject
   @selectObject.setter
   def selectObject(self, value):
      self.fn_ClassDesignerBase_selectObject = value
      lib.Instance_setMethod(self.impl, "SelectObject".encode('u8'), cb_ClassDesignerBase_selectObject)

class ClassProperty:
   def __init__(self, name = None, parent = None, left = None, right = None, depth = None, Set = None, Get = None, dataTypeString = None, dataType = None, constant = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_ClassProperty *", lib.Instance_new(lib.class_ClassProperty))
         if name is not None:           self.name           = name
         if parent is not None:         self.parent         = parent
         if left is not None:           self.left           = left
         if right is not None:          self.right          = right
         if depth is not None:          self.depth          = depth
         if Set is not None:            self.Set            = Set
         if Get is not None:            self.Get            = Get
         if dataTypeString is not None: self.dataTypeString = dataTypeString
         if dataType is not None:       self.dataType       = dataType
         if constant is not None:       self.constant       = constant

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def parent(self): return self.impl.parent
   @parent.setter
   def parent(self, value):
      if not isinstance(value, ClassProperty): value = ClassProperty(value)
      self.impl.parent = value.impl

   @property
   def left(self): return self.impl.left
   @left.setter
   def left(self, value):
      if not isinstance(value, ClassProperty): value = ClassProperty(value)
      self.impl.left = value.impl

   @property
   def right(self): return self.impl.right
   @right.setter
   def right(self, value):
      if not isinstance(value, ClassProperty): value = ClassProperty(value)
      self.impl.right = value.impl

   @property
   def depth(self): return self.impl.depth
   @depth.setter
   def depth(self, value): self.impl.depth = value

   @property
   def Set(self): return self.impl.Set
   @Set.setter
   def Set(self, value): self.impl.Set = value

   @property
   def Get(self): return self.impl.Get
   @Get.setter
   def Get(self, value): self.impl.Get = value

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataType(self): return self.impl.dataType
   @dataType.setter
   def dataType(self, value): self.impl.dataType = value.impl

   @property
   def constant(self): return self.impl.constant
   @constant.setter
   def constant(self, value): self.impl.constant = value

class ClassTemplateArgument(Struct):
   def __init__(self, dataTypeString = None, dataTypeClass = None, expression = None, memberString = None, member = None, prop = None, method = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_ClassTemplateArgument *", impl)
      else:
         if dataTypeClass is not None:
            if not isinstance(dataTypeClass, Class): dataTypeClass = Class(dataTypeClass)
         if expression is not None:
            if not isinstance(expression, DataValue): expression = DataValue(expression)
         if member is not None:
            if not isinstance(member, DataMember): member = DataMember(member)
         if prop is not None:
            if not isinstance(prop, Property): prop = Property(prop)
         if method is not None:
            if not isinstance(method, Method): method = Method(method)
         __members = { }
         if dataTypeString is not None: __members['dataTypeString'] = dataTypeString
         if dataTypeClass is not None:  __members['dataTypeClass']  = dataTypeClass.impl
         if expression is not None:     __members['expression']     = expression.impl[0]
         if memberString is not None:   __members['memberString']   = memberString
         if member is not None:         __members['member']         = member.impl
         if prop is not None:           __members['prop']           = prop.impl
         if method is not None:         __members['method']         = method.impl
         self.impl = ffi.new("eC_ClassTemplateArgument *", __members)

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataTypeClass(self): return self.impl.dataTypeClass
   @dataTypeClass.setter
   def dataTypeClass(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.dataTypeClass = value.impl

   @property
   def expression(self): return DataValue(impl = self.impl.expression)
   @expression.setter
   def expression(self, value):
      if not isinstance(value, DataValue): value = DataValue(value)
      self.impl.expression = value.impl[0]

   @property
   def memberString(self): return self.impl.memberString
   @memberString.setter
   def memberString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.memberString = value

   @property
   def member(self): return self.impl.member
   @member.setter
   def member(self, value):
      if not isinstance(value, DataMember): value = DataMember(value)
      self.impl.member = value.impl

   @property
   def prop(self): return self.impl.prop
   @prop.setter
   def prop(self, value):
      if not isinstance(value, Property): value = Property(value)
      self.impl.prop = value.impl

   @property
   def method(self): return self.impl.method
   @method.setter
   def method(self, value):
      if not isinstance(value, Method): value = Method(value)
      self.impl.method = value.impl

class ClassTemplateParameter:
   def __init__(self, prev = None, next = None, name = None, type = None, dataTypeString = None, defaultArg = None, param = None, memberType = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_ClassTemplateParameter *", lib.Instance_new(lib.class_ClassTemplateParameter))
         if prev is not None:           self.prev           = prev
         if next is not None:           self.next           = next
         if name is not None:           self.name           = name
         if type is not None:           self.type           = type
         if dataTypeString is not None: self.dataTypeString = dataTypeString
         if defaultArg is not None:     self.defaultArg     = defaultArg
         if param is not None:          self.param          = param
         if memberType is not None:     self.memberType     = memberType

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, ClassTemplateParameter): value = ClassTemplateParameter(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, ClassTemplateParameter): value = ClassTemplateParameter(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def type(self): return self.impl.type
   @type.setter
   def type(self, value): self.impl.type = value

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def memberType(self): return self.impl.memberType
   @memberType.setter
   def memberType(self, value): self.impl.memberType = value

   @property
   def defaultArg(self): return ClassTemplateArgument(impl = self.impl.defaultArg)
   @defaultArg.setter
   def defaultArg(self, value):
      if not isinstance(value, ClassTemplateArgument): value = ClassTemplateArgument(value)
      self.impl.defaultArg = value.impl[0]

   @property
   def param(self): return self.impl.param
   @param.setter
   def param(self, value): self.impl.param = value

class ClassType:
   normalClass = lib.ClassType_normalClass
   structClass = lib.ClassType_structClass
   bitClass    = lib.ClassType_bitClass
   unitClass   = lib.ClassType_unitClass
   enumClass   = lib.ClassType_enumClass
   noHeadClass = lib.ClassType_noHeadClass
   unionClass  = lib.ClassType_unionClass
   systemClass = lib.ClassType_systemClass

class DataMember:
   def __init__(self, prev = None, next = None, name = None, isProperty = None, memberAccess = None, id = None, _class = None, dataTypeString = None, dataTypeClass = None, dataType = None, type = None, offset = None, memberID = None, members = None, membersAlpha = None, memberOffset = None, structAlignment = None, pointerAlignment = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_DataMember *", lib.Instance_new(lib.class_DataMember))
         if prev is not None:             self.prev             = prev
         if next is not None:             self.next             = next
         if name is not None:             self.name             = name
         if isProperty is not None:       self.isProperty       = isProperty
         if memberAccess is not None:     self.memberAccess     = memberAccess
         if id is not None:               self.id               = id
         if _class is not None:           self._class           = _class
         if dataTypeString is not None:   self.dataTypeString   = dataTypeString
         if dataTypeClass is not None:    self.dataTypeClass    = dataTypeClass
         if dataType is not None:         self.dataType         = dataType
         if type is not None:             self.type             = type
         if offset is not None:           self.offset           = offset
         if memberID is not None:         self.memberID         = memberID
         if members is not None:          self.members          = members
         if membersAlpha is not None:     self.membersAlpha     = membersAlpha
         if memberOffset is not None:     self.memberOffset     = memberOffset
         if structAlignment is not None:  self.structAlignment  = structAlignment
         if pointerAlignment is not None: self.pointerAlignment = pointerAlignment

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, DataMember): value = DataMember(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, DataMember): value = DataMember(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def isProperty(self): return self.impl.isProperty
   @isProperty.setter
   def isProperty(self, value): self.impl.isProperty = value

   @property
   def memberAccess(self): return self.impl.memberAccess
   @memberAccess.setter
   def memberAccess(self, value): self.impl.memberAccess = value

   @property
   def id(self): return self.impl.id
   @id.setter
   def id(self, value): self.impl.id = value

   @property
   def _class(self): return self.impl._class
   @_class.setter
   def _class(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl._class = value.impl

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataTypeClass(self): return self.impl.dataTypeClass
   @dataTypeClass.setter
   def dataTypeClass(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.dataTypeClass = value.impl

   @property
   def dataType(self): return self.impl.dataType
   @dataType.setter
   def dataType(self, value): self.impl.dataType = value.impl

   @property
   def type(self): return self.impl.type
   @type.setter
   def type(self, value): self.impl.type = value

   @property
   def offset(self): return self.impl.offset
   @offset.setter
   def offset(self, value): self.impl.offset = value

   @property
   def memberID(self): return self.impl.memberID
   @memberID.setter
   def memberID(self, value): self.impl.memberID = value

   @property
   def members(self): return OldList(impl = self.impl.members)
   @members.setter
   def members(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.members = value.impl[0]

   @property
   def membersAlpha(self): return BinaryTree(impl = self.impl.membersAlpha)
   @membersAlpha.setter
   def membersAlpha(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.membersAlpha = value.impl[0]

   @property
   def memberOffset(self): return self.impl.memberOffset
   @memberOffset.setter
   def memberOffset(self, value): self.impl.memberOffset = value

   @property
   def structAlignment(self): return self.impl.structAlignment
   @structAlignment.setter
   def structAlignment(self, value): self.impl.structAlignment = value

   @property
   def pointerAlignment(self): return self.impl.pointerAlignment
   @pointerAlignment.setter
   def pointerAlignment(self, value): self.impl.pointerAlignment = value

class DataMemberType:
   normalMember = lib.DataMemberType_normalMember
   unionMember  = lib.DataMemberType_unionMember
   structMember = lib.DataMemberType_structMember

class DataValue(Struct):
   def __init__(self, c = None, uc = None, s = None, us = None, i = None, ui = None, p = None, f = None, d = None, i64 = None, ui64 = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_DataValue *", impl)
      else:
         __members = { }
         if c is not None:    __members['c']    = c
         if uc is not None:   __members['uc']   = uc
         if s is not None:    __members['s']    = s
         if us is not None:   __members['us']   = us
         if i is not None:    __members['i']    = i
         if ui is not None:   __members['ui']   = ui
         if p is not None:    __members['p']    = p
         if f is not None:    __members['f']    = f
         if d is not None:    __members['d']    = d
         if i64 is not None:  __members['i64']  = i64
         if ui64 is not None: __members['ui64'] = ui64
         self.impl = ffi.new("eC_DataValue *", __members)

   @property
   def c(self): return self.impl.c
   @c.setter
   def c(self, value): self.impl.c = value

   @property
   def uc(self): return self.impl.uc
   @uc.setter
   def uc(self, value): self.impl.uc = value

   @property
   def s(self): return self.impl.s
   @s.setter
   def s(self, value): self.impl.s = value

   @property
   def us(self): return self.impl.us
   @us.setter
   def us(self, value): self.impl.us = value

   @property
   def i(self): return self.impl.i
   @i.setter
   def i(self, value): self.impl.i = value

   @property
   def ui(self): return self.impl.ui
   @ui.setter
   def ui(self, value): self.impl.ui = value

   @property
   def p(self): return self.impl.p
   @p.setter
   def p(self, value): self.impl.p = value

   @property
   def f(self): return self.impl.f
   @f.setter
   def f(self, value): self.impl.f = value

   @property
   def d(self): return self.impl.d
   @d.setter
   def d(self, value): self.impl.d = value

   @property
   def i64(self): return self.impl.i64
   @i64.setter
   def i64(self, value): self.impl.i64 = value

   @property
   def ui64(self): return self.impl.ui64
   @ui64.setter
   def ui64(self, value): self.impl.ui64 = value

class DefinedExpression:
   def __init__(self, prev = None, next = None, name = None, value = None, nameSpace = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_DefinedExpression *", lib.Instance_new(lib.class_DefinedExpression))
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev      = __tuple[0]
            if len(__tuple) > 1: next      = __tuple[1]
            if len(__tuple) > 2: name      = __tuple[2]
            if len(__tuple) > 3: value     = __tuple[3]
         if prev is not None:      self.prev      = prev
         if next is not None:      self.next      = next
         if name is not None:      self.name      = name
         if value is not None:     self.value     = value
         if nameSpace is not None: self.nameSpace = nameSpace

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, DefinedExpression): value = DefinedExpression(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, DefinedExpression): value = DefinedExpression(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def value(self): return self.impl.value
   @value.setter
   def value(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.value = value

   @property
   def nameSpace(self): return self.impl.nameSpace
   @nameSpace.setter
   def nameSpace(self, value): self.impl.nameSpace = value

class Degrees(Angle):
   def __init__(self, value = 0, impl = None):
      if impl is not None: self.impl = impl
      elif isinstance(value, Angle): self.impl = value.impl
      else: self.value = value

   def __str__(self):
      return str(float(self.value))

   # conv eC_Angle <-> eC_Radians
   @property
   def value(self): return self.impl * 57.2957795130823
   @value.setter
   def value(self, value): self.impl = value * 0.0174532925199433

Degrees.buc = Angle

@ffi.callback("void(eC_DesignerBase, eC_Instance, eC_Instance)")
def cb_DesignerBase_addDefaultMethod(__e, instance, classInstance):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_addDefaultMethod(designerbase, pyOrNewObject(Instance, instance), pyOrNewObject(Instance, classInstance))

@ffi.callback("void(eC_DesignerBase, eC_Class *)")
def cb_DesignerBase_addToolBoxClass(__e, _class):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_addToolBoxClass(designerbase, Class(impl = _class))

@ffi.callback("void(eC_DesignerBase, eC_Instance, eC_ObjectInfo *)")
def cb_DesignerBase_codeAddObject(__e, instance, object):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_codeAddObject(designerbase, pyOrNewObject(Instance, instance), object)

@ffi.callback("void(eC_DesignerBase, eC_ObjectInfo *)")
def cb_DesignerBase_deleteObject(__e, object):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_deleteObject(designerbase, ObjectInfo(impl = object))

@ffi.callback("eC_bool(eC_DesignerBase, eC_Instance *, const char *)")
def cb_DesignerBase_findObject(__e, instance, string):
   designerbase = pyOrNewObject(DesignerBase, __e)
   return designerbase.fn_DesignerBase_findObject(designerbase, instance, string.encode('u8'))

@ffi.callback("void(eC_DesignerBase)")
def cb_DesignerBase_modifyCode(__e):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_modifyCode(designerbase)

@ffi.callback("eC_bool(eC_DesignerBase, eC_ObjectInfo *)")
def cb_DesignerBase_objectContainsCode(__e, object):
   designerbase = pyOrNewObject(DesignerBase, __e)
   return designerbase.fn_DesignerBase_objectContainsCode(designerbase, ObjectInfo(impl = object))

@ffi.callback("void(eC_DesignerBase, eC_ObjectInfo *, const char *)")
def cb_DesignerBase_renameObject(__e, object, name):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_renameObject(designerbase, ObjectInfo(impl = object), name.encode('u8'))

@ffi.callback("void(eC_DesignerBase, eC_ObjectInfo *)")
def cb_DesignerBase_selectObjectFromDesigner(__e, object):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_selectObjectFromDesigner(designerbase, ObjectInfo(impl = object))

@ffi.callback("void(eC_DesignerBase, eC_ObjectInfo *)")
def cb_DesignerBase_sheetAddObject(__e, object):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_sheetAddObject(designerbase, ObjectInfo(impl = object))

@ffi.callback("void(eC_DesignerBase)")
def cb_DesignerBase_updateProperties(__e):
   designerbase = pyOrNewObject(DesignerBase, __e)
   designerbase.fn_DesignerBase_updateProperties(designerbase)

class DesignerBase(Instance):
   class_members = [
                      'classDesigner',
                      'objectClass',
                      'isDragging',
                      'addDefaultMethod',
                      'addToolBoxClass',
                      'codeAddObject',
                      'deleteObject',
                      'findObject',
                      'modifyCode',
                      'objectContainsCode',
                      'renameObject',
                      'selectObjectFromDesigner',
                      'sheetAddObject',
                      'updateProperties',
                   ]

   def init_args(self, args, kwArgs): init_args(DesignerBase, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def classDesigner(self): return pyOrNewObject(ClassDesignerBase, lib.DesignerBase_get_classDesigner(self.impl))
   @classDesigner.setter
   def classDesigner(self, value):
      if not isinstance(value, ClassDesignerBase): value = ClassDesignerBase(value)
      lib.DesignerBase_set_classDesigner(self.impl, value.impl)

   @property
   def objectClass(self): value = lib.DesignerBase_get_objectClass(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @objectClass.setter
   def objectClass(self, value):
      lib.DesignerBase_set_objectClass(self.impl, value.encode('u8'))

   @property
   def isDragging(self): return lib.DesignerBase_get_isDragging(self.impl)
   @isDragging.setter
   def isDragging(self, value):
      lib.DesignerBase_set_isDragging(self.impl, value)

   def fn_unset_DesignerBase_addDefaultMethod(self, instance, classInstance):
      return lib.DesignerBase_addDefaultMethod(self.impl, ffi.NULL if instance is None else instance.impl, ffi.NULL if classInstance is None else classInstance.impl)

   @property
   def addDefaultMethod(self):
      if hasattr(self, 'fn_DesignerBase_addDefaultMethod'): return self.fn_DesignerBase_addDefaultMethod
      else: return self.fn_unset_DesignerBase_addDefaultMethod
   @addDefaultMethod.setter
   def addDefaultMethod(self, value):
      self.fn_DesignerBase_addDefaultMethod = value
      lib.Instance_setMethod(self.impl, "AddDefaultMethod".encode('u8'), cb_DesignerBase_addDefaultMethod)

   def fn_unset_DesignerBase_addToolBoxClass(self, _class):
      return lib.DesignerBase_addToolBoxClass(self.impl, ffi.NULL if _class is None else _class.impl)

   @property
   def addToolBoxClass(self):
      if hasattr(self, 'fn_DesignerBase_addToolBoxClass'): return self.fn_DesignerBase_addToolBoxClass
      else: return self.fn_unset_DesignerBase_addToolBoxClass
   @addToolBoxClass.setter
   def addToolBoxClass(self, value):
      self.fn_DesignerBase_addToolBoxClass = value
      lib.Instance_setMethod(self.impl, "AddToolBoxClass".encode('u8'), cb_DesignerBase_addToolBoxClass)

   def fn_unset_DesignerBase_codeAddObject(self, instance, object):
      return lib.DesignerBase_codeAddObject(self.impl, ffi.NULL if instance is None else instance.impl, object)

   @property
   def codeAddObject(self):
      if hasattr(self, 'fn_DesignerBase_codeAddObject'): return self.fn_DesignerBase_codeAddObject
      else: return self.fn_unset_DesignerBase_codeAddObject
   @codeAddObject.setter
   def codeAddObject(self, value):
      self.fn_DesignerBase_codeAddObject = value
      lib.Instance_setMethod(self.impl, "CodeAddObject".encode('u8'), cb_DesignerBase_codeAddObject)

   def fn_unset_DesignerBase_deleteObject(self, object):
      return lib.DesignerBase_deleteObject(self.impl, ffi.NULL if object is None else object.impl)

   @property
   def deleteObject(self):
      if hasattr(self, 'fn_DesignerBase_deleteObject'): return self.fn_DesignerBase_deleteObject
      else: return self.fn_unset_DesignerBase_deleteObject
   @deleteObject.setter
   def deleteObject(self, value):
      self.fn_DesignerBase_deleteObject = value
      lib.Instance_setMethod(self.impl, "DeleteObject".encode('u8'), cb_DesignerBase_deleteObject)

   def fn_unset_DesignerBase_findObject(self, instance, string):
      return lib.DesignerBase_findObject(self.impl, instance, string)

   @property
   def findObject(self):
      if hasattr(self, 'fn_DesignerBase_findObject'): return self.fn_DesignerBase_findObject
      else: return self.fn_unset_DesignerBase_findObject
   @findObject.setter
   def findObject(self, value):
      self.fn_DesignerBase_findObject = value
      lib.Instance_setMethod(self.impl, "FindObject".encode('u8'), cb_DesignerBase_findObject)

   def fn_unset_DesignerBase_modifyCode(self):
      return lib.DesignerBase_modifyCode(self.impl)

   @property
   def modifyCode(self):
      if hasattr(self, 'fn_DesignerBase_modifyCode'): return self.fn_DesignerBase_modifyCode
      else: return self.fn_unset_DesignerBase_modifyCode
   @modifyCode.setter
   def modifyCode(self, value):
      self.fn_DesignerBase_modifyCode = value
      lib.Instance_setMethod(self.impl, "ModifyCode".encode('u8'), cb_DesignerBase_modifyCode)

   def fn_unset_DesignerBase_objectContainsCode(self, object):
      return lib.DesignerBase_objectContainsCode(self.impl, ffi.NULL if object is None else object.impl)

   @property
   def objectContainsCode(self):
      if hasattr(self, 'fn_DesignerBase_objectContainsCode'): return self.fn_DesignerBase_objectContainsCode
      else: return self.fn_unset_DesignerBase_objectContainsCode
   @objectContainsCode.setter
   def objectContainsCode(self, value):
      self.fn_DesignerBase_objectContainsCode = value
      lib.Instance_setMethod(self.impl, "ObjectContainsCode".encode('u8'), cb_DesignerBase_objectContainsCode)

   def fn_unset_DesignerBase_renameObject(self, object, name):
      return lib.DesignerBase_renameObject(self.impl, ffi.NULL if object is None else object.impl, name)

   @property
   def renameObject(self):
      if hasattr(self, 'fn_DesignerBase_renameObject'): return self.fn_DesignerBase_renameObject
      else: return self.fn_unset_DesignerBase_renameObject
   @renameObject.setter
   def renameObject(self, value):
      self.fn_DesignerBase_renameObject = value
      lib.Instance_setMethod(self.impl, "RenameObject".encode('u8'), cb_DesignerBase_renameObject)

   def fn_unset_DesignerBase_selectObjectFromDesigner(self, object):
      return lib.DesignerBase_selectObjectFromDesigner(self.impl, ffi.NULL if object is None else object.impl)

   @property
   def selectObjectFromDesigner(self):
      if hasattr(self, 'fn_DesignerBase_selectObjectFromDesigner'): return self.fn_DesignerBase_selectObjectFromDesigner
      else: return self.fn_unset_DesignerBase_selectObjectFromDesigner
   @selectObjectFromDesigner.setter
   def selectObjectFromDesigner(self, value):
      self.fn_DesignerBase_selectObjectFromDesigner = value
      lib.Instance_setMethod(self.impl, "SelectObjectFromDesigner".encode('u8'), cb_DesignerBase_selectObjectFromDesigner)

   def fn_unset_DesignerBase_sheetAddObject(self, object):
      return lib.DesignerBase_sheetAddObject(self.impl, ffi.NULL if object is None else object.impl)

   @property
   def sheetAddObject(self):
      if hasattr(self, 'fn_DesignerBase_sheetAddObject'): return self.fn_DesignerBase_sheetAddObject
      else: return self.fn_unset_DesignerBase_sheetAddObject
   @sheetAddObject.setter
   def sheetAddObject(self, value):
      self.fn_DesignerBase_sheetAddObject = value
      lib.Instance_setMethod(self.impl, "SheetAddObject".encode('u8'), cb_DesignerBase_sheetAddObject)

   def fn_unset_DesignerBase_updateProperties(self):
      return lib.DesignerBase_updateProperties(self.impl)

   @property
   def updateProperties(self):
      if hasattr(self, 'fn_DesignerBase_updateProperties'): return self.fn_DesignerBase_updateProperties
      else: return self.fn_unset_DesignerBase_updateProperties
   @updateProperties.setter
   def updateProperties(self, value):
      self.fn_DesignerBase_updateProperties = value
      lib.Instance_setMethod(self.impl, "UpdateProperties".encode('u8'), cb_DesignerBase_updateProperties)

class EnumClassData:
   def __init__(self, values = None, largest = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_EnumClassData *", lib.Instance_new(lib.class_EnumClassData))
         if isinstance(values, tuple):
            __tuple = values
            values = None
            if len(__tuple) > 0: values  = __tuple[0]
            if len(__tuple) > 1: largest = __tuple[1]
         if values is not None:  self.values  = values
         if largest is not None: self.largest = largest

   @property
   def values(self): return OldList(impl = self.impl.values)
   @values.setter
   def values(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.values = value.impl[0]

   @property
   def largest(self): return self.impl.largest
   @largest.setter
   def largest(self, value): self.impl.largest = value

class EscapeCStringOptions(pyBaseClass):
   def __init__(self, escapeSingleQuote = False, escapeDoubleQuotes = False, writeQuotes = False, multiLine = False, indent = 0, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(escapeSingleQuote, EscapeCStringOptions):
         self.impl = escapeSingleQuote.impl
      else:
         if isinstance(escapeSingleQuote, tuple):
            __tuple = escapeSingleQuote
            escapeSingleQuote = False
            if len(__tuple) > 0: escapeSingleQuote = __tuple[0]
            if len(__tuple) > 1: escapeDoubleQuotes = __tuple[1]
            if len(__tuple) > 2: writeQuotes = __tuple[2]
            if len(__tuple) > 3: multiLine = __tuple[3]
            if len(__tuple) > 4: indent = __tuple[4]
         self.impl = (
            (escapeSingleQuote  << lib.ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT)  |
            (escapeDoubleQuotes << lib.ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT) |
            (writeQuotes        << lib.ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT)        |
            (multiLine          << lib.ESCAPECSTRINGOPTIONS_multiLine_SHIFT)          |
            (indent             << lib.ESCAPECSTRINGOPTIONS_indent_SHIFT)             )

   @property
   def escapeSingleQuote(self): return ((((self.impl)) & lib.ESCAPECSTRINGOPTIONS_escapeSingleQuote_MASK) >> lib.ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT)
   @escapeSingleQuote.setter
   def escapeSingleQuote(self, value): self.impl = ((self.impl) & ~(lib.ESCAPECSTRINGOPTIONS_escapeSingleQuote_MASK)) | (((value)) << lib.ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT)

   @property
   def escapeDoubleQuotes(self): return ((((self.impl)) & lib.ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_MASK) >> lib.ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT)
   @escapeDoubleQuotes.setter
   def escapeDoubleQuotes(self, value): self.impl = ((self.impl) & ~(lib.ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_MASK)) | (((value)) << lib.ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT)

   @property
   def writeQuotes(self): return ((((self.impl)) & lib.ESCAPECSTRINGOPTIONS_writeQuotes_MASK) >> lib.ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT)
   @writeQuotes.setter
   def writeQuotes(self, value): self.impl = ((self.impl) & ~(lib.ESCAPECSTRINGOPTIONS_writeQuotes_MASK)) | (((value)) << lib.ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT)

   @property
   def multiLine(self): return ((((self.impl)) & lib.ESCAPECSTRINGOPTIONS_multiLine_MASK) >> lib.ESCAPECSTRINGOPTIONS_multiLine_SHIFT)
   @multiLine.setter
   def multiLine(self, value): self.impl = ((self.impl) & ~(lib.ESCAPECSTRINGOPTIONS_multiLine_MASK)) | (((value)) << lib.ESCAPECSTRINGOPTIONS_multiLine_SHIFT)

   @property
   def indent(self): return ((((self.impl)) & lib.ESCAPECSTRINGOPTIONS_indent_MASK) >> lib.ESCAPECSTRINGOPTIONS_indent_SHIFT)
   @indent.setter
   def indent(self, value): self.impl = ((self.impl) & ~(lib.ESCAPECSTRINGOPTIONS_indent_MASK)) | (((value)) << lib.ESCAPECSTRINGOPTIONS_indent_SHIFT)

class Feet(Distance):
   def __init__(self, value = 0, impl = None):
      if impl is not None: self.impl = impl
      elif isinstance(value, Distance): self.impl = value.impl
      else: self.value = value

   # conv eC_Distance <-> eC_Meters
   @property
   def value(self): return self.impl * 3.28083985446533
   @value.setter
   def value(self, value): self.impl = value * 0.304800003767014

Feet.buc = Distance

class GlobalFunction:
   def __init__(self, prev = None, next = None, name = None, function = None, module = None, nameSpace = None, dataTypeString = None, dataType = None, symbol = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_GlobalFunction *", lib.Instance_new(lib.class_GlobalFunction))
         if prev is not None:           self.prev           = prev
         if next is not None:           self.next           = next
         if name is not None:           self.name           = name
         if function is not None:       self.function       = function
         if module is not None:         self.module         = module
         if nameSpace is not None:      self.nameSpace      = nameSpace
         if dataTypeString is not None: self.dataTypeString = dataTypeString
         if dataType is not None:       self.dataType       = dataType
         if symbol is not None:         self.symbol         = symbol

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, GlobalFunction): value = GlobalFunction(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, GlobalFunction): value = GlobalFunction(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def function(self): return self.impl.function
   @function.setter
   def function(self, value): self.impl.function = value

   @property
   def module(self): return pyOrNewObject(Module, self.impl.module)
   @module.setter
   def module(self, value):
      if not isinstance(value, Module): value = Module(value)
      self.impl.module = value.impl

   @property
   def nameSpace(self): return self.impl.nameSpace
   @nameSpace.setter
   def nameSpace(self, value): self.impl.nameSpace = value

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataType(self): return self.impl.dataType
   @dataType.setter
   def dataType(self, value): self.impl.dataType = value.impl

   @property
   def symbol(self): return self.impl.symbol
   @symbol.setter
   def symbol(self, value): self.impl.symbol = value

class ImportType:
   normalImport   = lib.ImportType_normalImport
   staticImport   = lib.ImportType_staticImport
   remoteImport   = lib.ImportType_remoteImport
   preDeclImport  = lib.ImportType_preDeclImport
   comCheckImport = lib.ImportType_comCheckImport

class Method:
   def __init__(self, name = None, parent = None, left = None, right = None, depth = None, function = None, vid = None, type = None, _class = None, symbol = None, dataTypeString = None, dataType = None, memberAccess = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_Method *", lib.Instance_new(lib.class_Method))
         if name is not None:           self.name           = name
         if parent is not None:         self.parent         = parent
         if left is not None:           self.left           = left
         if right is not None:          self.right          = right
         if depth is not None:          self.depth          = depth
         if function is not None:       self.function       = function
         if vid is not None:            self.vid            = vid
         if type is not None:           self.type           = type
         if _class is not None:         self._class         = _class
         if symbol is not None:         self.symbol         = symbol
         if dataTypeString is not None: self.dataTypeString = dataTypeString
         if dataType is not None:       self.dataType       = dataType
         if memberAccess is not None:   self.memberAccess   = memberAccess

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def parent(self): return self.impl.parent
   @parent.setter
   def parent(self, value):
      if not isinstance(value, Method): value = Method(value)
      self.impl.parent = value.impl

   @property
   def left(self): return self.impl.left
   @left.setter
   def left(self, value):
      if not isinstance(value, Method): value = Method(value)
      self.impl.left = value.impl

   @property
   def right(self): return self.impl.right
   @right.setter
   def right(self, value):
      if not isinstance(value, Method): value = Method(value)
      self.impl.right = value.impl

   @property
   def depth(self): return self.impl.depth
   @depth.setter
   def depth(self, value): self.impl.depth = value

   @property
   def function(self): return self.impl.function
   @function.setter
   def function(self, value): self.impl.function = value

   @property
   def vid(self): return self.impl.vid
   @vid.setter
   def vid(self, value): self.impl.vid = value

   @property
   def type(self): return self.impl.type
   @type.setter
   def type(self, value): self.impl.type = value

   @property
   def _class(self): return self.impl._class
   @_class.setter
   def _class(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl._class = value.impl

   @property
   def symbol(self): return self.impl.symbol
   @symbol.setter
   def symbol(self, value): self.impl.symbol = value

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataType(self): return self.impl.dataType
   @dataType.setter
   def dataType(self, value): self.impl.dataType = value.impl

   @property
   def memberAccess(self): return self.impl.memberAccess
   @memberAccess.setter
   def memberAccess(self, value): self.impl.memberAccess = value

class MethodType:
   normalMethod  = lib.MethodType_normalMethod
   virtualMethod = lib.MethodType_virtualMethod

class MinMaxValue(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

MinMaxValue.buc = MinMaxValue

class NameSpace(Struct):
   def __init__(self, name = None, btParent = None, left = None, right = None, depth = 0, parent = None, nameSpaces = None, classes = None, defines = None, functions = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_NameSpace *", impl)
      else:
         if nameSpaces is not None:
            if not isinstance(nameSpaces, BinaryTree): nameSpaces = BinaryTree(nameSpaces)
            nameSpaces = nameSpaces.impl[0]
         else:
            nameSpaces = BinaryTree()
            nameSpaces = nameSpaces.impl[0]
         if classes is not None:
            if not isinstance(classes, BinaryTree): classes = BinaryTree(classes)
            classes = classes.impl[0]
         else:
            classes = BinaryTree()
            classes = classes.impl[0]
         if defines is not None:
            if not isinstance(defines, BinaryTree): defines = BinaryTree(defines)
            defines = defines.impl[0]
         else:
            defines = BinaryTree()
            defines = defines.impl[0]
         if functions is not None:
            if not isinstance(functions, BinaryTree): functions = BinaryTree(functions)
            functions = functions.impl[0]
         else:
            functions = BinaryTree()
            functions = functions.impl[0]
         self.impl = ffi.new("eC_NameSpace *", {
                                'name' : name,
                                'btParent' : btParent,
                                'left' : left,
                                'right' : right,
                                'depth' : depth,
                                'parent' : parent,
                                'nameSpaces' : nameSpaces,
                                'classes' : classes,
                                'defines' : defines,
                                'functions' : functions
                             })

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def btParent(self): return self.impl.btParent
   @btParent.setter
   def btParent(self, value): self.impl.btParent = value

   @property
   def left(self): return self.impl.left
   @left.setter
   def left(self, value): self.impl.left = value

   @property
   def right(self): return self.impl.right
   @right.setter
   def right(self, value): self.impl.right = value

   @property
   def depth(self): return self.impl.depth
   @depth.setter
   def depth(self, value): self.impl.depth = value

   @property
   def parent(self): return self.impl.parent
   @parent.setter
   def parent(self, value): self.impl.parent = value

   @property
   def nameSpaces(self): return BinaryTree(impl = self.impl.nameSpaces)
   @nameSpaces.setter
   def nameSpaces(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.nameSpaces = value.impl[0]

   @property
   def classes(self): return BinaryTree(impl = self.impl.classes)
   @classes.setter
   def classes(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.classes = value.impl[0]

   @property
   def defines(self): return BinaryTree(impl = self.impl.defines)
   @defines.setter
   def defines(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.defines = value.impl[0]

   @property
   def functions(self): return BinaryTree(impl = self.impl.functions)
   @functions.setter
   def functions(self, value):
      if not isinstance(value, BinaryTree): value = BinaryTree(value)
      self.impl.functions = value.impl[0]

class ObjectInfo:
   def __init__(self, prev = None, next = None, instance = None, name = None, instCode = None, deleted = None, oClass = None, instances = None, classDefinition = None, modified = None, i18nStrings = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_ObjectInfo *", lib.Instance_new(lib.class_ObjectInfo))
         if prev is not None:            self.prev            = prev
         if next is not None:            self.next            = next
         if instance is not None:        self.instance        = instance
         if name is not None:            self.name            = name
         if instCode is not None:        self.instCode        = instCode
         if deleted is not None:         self.deleted         = deleted
         if oClass is not None:          self.oClass          = oClass
         if instances is not None:       self.instances       = instances
         if classDefinition is not None: self.classDefinition = classDefinition
         if modified is not None:        self.modified        = modified
         if i18nStrings is not None:     self.i18nStrings     = i18nStrings

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, ObjectInfo): value = ObjectInfo(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, ObjectInfo): value = ObjectInfo(value)
      self.impl.next = value.impl

   @property
   def instance(self): return pyOrNewObject(Instance, self.impl.instance)
   @instance.setter
   def instance(self, value):
      if not isinstance(value, Instance): value = Instance(value)
      self.impl.instance = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def instCode(self): return self.impl.instCode
   @instCode.setter
   def instCode(self, value): self.impl.instCode = value.impl

   @property
   def deleted(self): return self.impl.deleted
   @deleted.setter
   def deleted(self, value): self.impl.deleted = value

   @property
   def oClass(self): return self.impl.oClass
   @oClass.setter
   def oClass(self, value):
      if not isinstance(value, ObjectInfo): value = ObjectInfo(value)
      self.impl.oClass = value.impl

   @property
   def instances(self): return OldList(impl = self.impl.instances)
   @instances.setter
   def instances(self, value):
      if not isinstance(value, OldList): value = OldList(value)
      self.impl.instances = value.impl[0]

   @property
   def classDefinition(self): return self.impl.classDefinition
   @classDefinition.setter
   def classDefinition(self, value): self.impl.classDefinition = value.impl

   @property
   def modified(self): return self.impl.modified
   @modified.setter
   def modified(self, value): self.impl.modified = value

   @property
   def i18nStrings(self): return self.impl.i18nStrings
   @i18nStrings.setter
   def i18nStrings(self, value): self.impl.i18nStrings = value

class ObjectNotationType(Bool):
   none = bool(lib.ObjectNotationType_none)
   econ = bool(lib.ObjectNotationType_econ)
   json = bool(lib.ObjectNotationType_json)

class Platform:
   unknown = lib.Platform_unknown
   win32   = lib.Platform_win32
   tux     = lib.Platform_tux
   apple   = lib.Platform_apple

   def __str__(self): return ffi.string(lib.Platform_to_char_ptr(self.impl)).decode('u8') if self.impl != ffi.NULL else str()

class Point(Struct):
   def __init__(self, x = 0, y = 0, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_Point *", impl)
      else:
         if isinstance(x, tuple):
            __tuple = x
            x = 0
            if len(__tuple) > 0: x = __tuple[0]
            if len(__tuple) > 1: y = __tuple[1]
         self.impl = ffi.new("eC_Point *", { 'x' : x, 'y' : y })

   @property
   def x(self): return self.impl.x
   @x.setter
   def x(self, value): self.impl.x = value

   @property
   def y(self): return self.impl.y
   @y.setter
   def y(self, value): self.impl.y = value

class Pointd(Struct):
   def __init__(self, x = 0.0, y = 0.0, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_Pointd *", impl)
      else:
         if isinstance(x, tuple):
            __tuple = x
            x = 0.0
            if len(__tuple) > 0: x = __tuple[0]
            if len(__tuple) > 1: y = __tuple[1]
         self.impl = ffi.new("eC_Pointd *", { 'x' : x, 'y' : y })

   @property
   def x(self): return self.impl.x
   @x.setter
   def x(self, value): self.impl.x = value

   @property
   def y(self): return self.impl.y
   @y.setter
   def y(self, value): self.impl.y = value

class Pointf(Struct):
   def __init__(self, x = 0.0, y = 0.0, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_Pointf *", impl)
      else:
         if isinstance(x, tuple):
            __tuple = x
            x = 0.0
            if len(__tuple) > 0: x = __tuple[0]
            if len(__tuple) > 1: y = __tuple[1]
         self.impl = ffi.new("eC_Pointf *", { 'x' : x, 'y' : y })

   @property
   def x(self): return self.impl.x
   @x.setter
   def x(self, value): self.impl.x = value

   @property
   def y(self): return self.impl.y
   @y.setter
   def y(self, value): self.impl.y = value

class Property:
   def __init__(self, prev = None, next = None, name = None, isProperty = None, memberAccess = None, id = None, _class = None, dataTypeString = None, dataTypeClass = None, dataType = None, Set = None, Get = None, IsSet = None, data = None, symbol = None, vid = None, conversion = None, watcherOffset = None, category = None, compiled = None, selfWatchable = None, isWatchable = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_Property *", lib.Instance_new(lib.class_Property))
         if prev is not None:           self.prev           = prev
         if next is not None:           self.next           = next
         if name is not None:           self.name           = name
         if isProperty is not None:     self.isProperty     = isProperty
         if memberAccess is not None:   self.memberAccess   = memberAccess
         if id is not None:             self.id             = id
         if _class is not None:         self._class         = _class
         if dataTypeString is not None: self.dataTypeString = dataTypeString
         if dataTypeClass is not None:  self.dataTypeClass  = dataTypeClass
         if dataType is not None:       self.dataType       = dataType
         if Set is not None:            self.Set            = Set
         if Get is not None:            self.Get            = Get
         if IsSet is not None:          self.IsSet          = IsSet
         if data is not None:           self.data           = data
         if symbol is not None:         self.symbol         = symbol
         if vid is not None:            self.vid            = vid
         if conversion is not None:     self.conversion     = conversion
         if watcherOffset is not None:  self.watcherOffset  = watcherOffset
         if category is not None:       self.category       = category
         if compiled is not None:       self.compiled       = compiled
         if selfWatchable is not None:  self.selfWatchable  = selfWatchable
         if isWatchable is not None:    self.isWatchable    = isWatchable

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, Property): value = Property(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, Property): value = Property(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def isProperty(self): return self.impl.isProperty
   @isProperty.setter
   def isProperty(self, value): self.impl.isProperty = value

   @property
   def memberAccess(self): return self.impl.memberAccess
   @memberAccess.setter
   def memberAccess(self, value): self.impl.memberAccess = value

   @property
   def id(self): return self.impl.id
   @id.setter
   def id(self, value): self.impl.id = value

   @property
   def _class(self): return self.impl._class
   @_class.setter
   def _class(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl._class = value.impl

   @property
   def dataTypeString(self): return self.impl.dataTypeString
   @dataTypeString.setter
   def dataTypeString(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.dataTypeString = value

   @property
   def dataTypeClass(self): return self.impl.dataTypeClass
   @dataTypeClass.setter
   def dataTypeClass(self, value):
      if not isinstance(value, Class): value = Class(value)
      self.impl.dataTypeClass = value.impl

   @property
   def dataType(self): return self.impl.dataType
   @dataType.setter
   def dataType(self, value): self.impl.dataType = value.impl

   @property
   def Set(self): return self.impl.Set
   @Set.setter
   def Set(self, value): self.impl.Set = value

   @property
   def Get(self): return self.impl.Get
   @Get.setter
   def Get(self, value): self.impl.Get = value

   @property
   def IsSet(self): return self.impl.IsSet
   @IsSet.setter
   def IsSet(self, value): self.impl.IsSet = value

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

   @property
   def symbol(self): return self.impl.symbol
   @symbol.setter
   def symbol(self, value): self.impl.symbol = value

   @property
   def vid(self): return self.impl.vid
   @vid.setter
   def vid(self, value): self.impl.vid = value

   @property
   def conversion(self): return self.impl.conversion
   @conversion.setter
   def conversion(self, value): self.impl.conversion = value

   @property
   def watcherOffset(self): return self.impl.watcherOffset
   @watcherOffset.setter
   def watcherOffset(self, value): self.impl.watcherOffset = value

   @property
   def category(self): return self.impl.category
   @category.setter
   def category(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.category = value

   @property
   def compiled(self): return self.impl.compiled
   @compiled.setter
   def compiled(self, value): self.impl.compiled = value

   @property
   def selfWatchable(self): return self.impl.selfWatchable
   @selfWatchable.setter
   def selfWatchable(self, value): self.impl.selfWatchable = value

   @property
   def isWatchable(self): return self.impl.isWatchable
   @isWatchable.setter
   def isWatchable(self, value): self.impl.isWatchable = value

class SerialBuffer(IOChannel):
   class_members = [
                      '_buffer',
                      'count',
                      '_size',
                      'pos',
                      'buffer',
                      'size',
                   ]

   def init_args(self, args, kwArgs): init_args(SerialBuffer, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def _buffer(self): return IPTR(lib, ffi, self, SerialBuffer)._buffer
   @_buffer.setter
   def _buffer(self, value): IPTR(lib, ffi, self, SerialBuffer)._buffer = value

   @property
   def count(self): return IPTR(lib, ffi, self, SerialBuffer).count
   @count.setter
   def count(self, value): IPTR(lib, ffi, self, SerialBuffer).count = value

   @property
   def _size(self): return IPTR(lib, ffi, self, SerialBuffer)._size
   @_size.setter
   def _size(self, value): IPTR(lib, ffi, self, SerialBuffer)._size = value

   @property
   def pos(self): return IPTR(lib, ffi, self, SerialBuffer).pos
   @pos.setter
   def pos(self, value): IPTR(lib, ffi, self, SerialBuffer).pos = value

   @property
   def buffer(self): return lib.SerialBuffer_get_buffer(self.impl)
   @buffer.setter
   def buffer(self, value):
      lib.SerialBuffer_set_buffer(self.impl, value)

   @property
   def size(self): return lib.SerialBuffer_get_size(self.impl)
   @size.setter
   def size(self, value):
      lib.SerialBuffer_set_size(self.impl, value)

   def free(self):
      lib.SerialBuffer_free(self.impl)

class Size(Struct):
   def __init__(self, w = 0, h = 0, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_Size *", impl)
      else:
         if isinstance(w, tuple):
            __tuple = w
            w = 0
            if len(__tuple) > 0: w = __tuple[0]
            if len(__tuple) > 1: h = __tuple[1]
         if w is not None:
            if not isinstance(w, MinMaxValue): w = MinMaxValue(w)
            w = w.impl
         else:
            w = MinMaxValue()
         if h is not None:
            if not isinstance(h, MinMaxValue): h = MinMaxValue(h)
            h = h.impl
         else:
            h = MinMaxValue()
         self.impl = ffi.new("eC_Size *", { 'w' : w, 'h' : h })

   @property
   def w(self): return MinMaxValue(impl = self.impl.w)
   @w.setter
   def w(self, value):
      if not isinstance(value, MinMaxValue): value = MinMaxValue(value)
      self.impl.w = value.impl

   @property
   def h(self): return MinMaxValue(impl = self.impl.h)
   @h.setter
   def h(self, value):
      if not isinstance(value, MinMaxValue): value = MinMaxValue(value)
      self.impl.h = value.impl

class StaticString(Struct):
   def __init__(self, string = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_StaticString *", impl)
      else:
         self.impl = ffi.new("eC_StaticString *", { 'string' : string })

   @property
   def string(self): return self.impl.string
   @string.setter
   def string(self, value): self.impl.string = value

class StringAllocType:
   pointer = lib.StringAllocType_pointer
   stack   = lib.StringAllocType_stack
   heap    = lib.StringAllocType_heap

class SubModule:
   def __init__(self, prev = None, next = None, module = None, importMode = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_SubModule *", lib.Instance_new(lib.class_SubModule))
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev       = __tuple[0]
            if len(__tuple) > 1: next       = __tuple[1]
            if len(__tuple) > 2: module     = __tuple[2]
            if len(__tuple) > 3: importMode = __tuple[3]
         if prev is not None:       self.prev       = prev
         if next is not None:       self.next       = next
         if module is not None:     self.module     = module
         if importMode is not None: self.importMode = importMode

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, SubModule): value = SubModule(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, SubModule): value = SubModule(value)
      self.impl.next = value.impl

   @property
   def module(self): return pyOrNewObject(Module, self.impl.module)
   @module.setter
   def module(self, value):
      if not isinstance(value, Module): value = Module(value)
      self.impl.module = value.impl

   @property
   def importMode(self): return self.impl.importMode
   @importMode.setter
   def importMode(self, value): self.impl.importMode = value

class TemplateMemberType:
   dataMember = lib.TemplateMemberType_dataMember
   method     = lib.TemplateMemberType_method
   prop       = lib.TemplateMemberType_prop

class TemplateParameterType:
   type       = lib.TemplateParameterType_type
   identifier = lib.TemplateParameterType_identifier
   expression = lib.TemplateParameterType_expression

def changeCh(string, ch1, ch2):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   lib.eC_changeCh(string, ch1, ch2)

def changeChars(string, chars, alt):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(chars, str): chars = ffi.new("char[]", chars.encode('u8'))
   elif chars is None: chars = ffi.NULL
   lib.eC_changeChars(string, chars, alt)

def changeExtension(string, ext, output):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(ext, str): ext = ffi.new("char[]", ext.encode('u8'))
   elif ext is None: ext = ffi.NULL
   if isinstance(output, str): output = ffi.new("char[]", output.encode('u8'))
   elif output is None: output = ffi.NULL
   return lib.eC_changeExtension(string, ext, output)

def checkConsistency():
   lib.eC_checkConsistency()

def checkMemory():
   lib.eC_checkMemory()

def copyBytes(dest, source, count):
   if hasattr(dest, 'impl'): dest = dest.impl
   if dest is None: dest = ffi.NULL
   if hasattr(source, 'impl'): source = source.impl
   if source is None: source = ffi.NULL
   lib.eC_copyBytes(dest, source, count)

def copyBytesBy2(dest, source, count):
   if hasattr(dest, 'impl'): dest = dest.impl
   if dest is None: dest = ffi.NULL
   if hasattr(source, 'impl'): source = source.impl
   if source is None: source = ffi.NULL
   lib.eC_copyBytesBy2(dest, source, count)

def copyBytesBy4(dest, source, count):
   if hasattr(dest, 'impl'): dest = dest.impl
   if dest is None: dest = ffi.NULL
   if hasattr(source, 'impl'): source = source.impl
   if source is None: source = ffi.NULL
   lib.eC_copyBytesBy4(dest, source, count)

def copyString(string):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return lib.eC_copyString(string)

def escapeCString(outString, bufferLen, s, options):
   if isinstance(outString, str): outString = ffi.new("char[]", outString.encode('u8'))
   elif outString is None: outString = ffi.NULL
   if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
   elif s is None: s = ffi.NULL
   if options is not None and not isinstance(options, EscapeCStringOptions): options = EscapeCStringOptions(options)
   if options is None: options = ffi.NULL
   return lib.eC_escapeCString(outString, bufferLen, s, options)

def fillBytes(area, value, count):
   if hasattr(area, 'impl'): area = area.impl
   if area is None: area = ffi.NULL
   lib.eC_fillBytes(area, value, count)

def fillBytesBy2(area, value, count):
   if hasattr(area, 'impl'): area = area.impl
   if area is None: area = ffi.NULL
   lib.eC_fillBytesBy2(area, value, count)

def fillBytesBy4(area, value, count):
   if hasattr(area, 'impl'): area = area.impl
   if area is None: area = ffi.NULL
   lib.eC_fillBytesBy4(area, value, count)

def floatFromString(string):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return lib.eC_floatFromString(string)

def getActiveDesigner():
   return pyOrNewObject(DesignerBase, lib.eC_getActiveDesigner())

def getExtension(string, output):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(output, str): output = ffi.new("char[]", output.encode('u8'))
   elif output is None: output = ffi.NULL
   return lib.eC_getExtension(string, output)

def getHexValue(buffer):
   if isinstance(buffer, str): buffer = ffi.new("char[]", buffer.encode('u8'))
   elif buffer is None: buffer = ffi.NULL
   return lib.eC_getHexValue(buffer)

def getLastDirectory(string):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif isinstance(string, String): string = string.impl
   elif string is None: string = ffi.NULL
   outputArray = ffi.new("char[]", lib.MAX_LOCATION)
   outputArray[0] = b'\0'
   lib.eC_getLastDirectory(string, outputArray)
   return String(outputArray)

def getRuntimePlatform():
   return lib.eC_getRuntimePlatform()

def getString(buffer, string, max):
   if isinstance(buffer, str): buffer = ffi.new("char[]", buffer.encode('u8'))
   elif buffer is None: buffer = ffi.NULL
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return lib.eC_getString(buffer, string, max)

def getValue(buffer):
   if isinstance(buffer, str): buffer = ffi.new("char[]", buffer.encode('u8'))
   elif buffer is None: buffer = ffi.NULL
   return lib.eC_getValue(buffer)

def isPathInsideOf(path, of):
   if isinstance(path, str): path = ffi.new("char[]", path.encode('u8'))
   elif path is None: path = ffi.NULL
   if isinstance(of, str): of = ffi.new("char[]", of.encode('u8'))
   elif of is None: of = ffi.NULL
   return lib.eC_isPathInsideOf(path, of)

def locateModule(name, fileName):
   if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
   elif name is None: name = ffi.NULL
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   return lib.eC_locateModule(name, fileName)

def makePathRelative(path, to, destination):
   if isinstance(path, str): path = ffi.new("char[]", path.encode('u8'))
   elif path is None: path = ffi.NULL
   if isinstance(to, str): to = ffi.new("char[]", to.encode('u8'))
   elif to is None: to = ffi.NULL
   if isinstance(destination, str): destination = ffi.new("char[]", destination.encode('u8'))
   elif destination is None: destination = ffi.NULL
   return lib.eC_makePathRelative(path, to, destination)

def moveBytes(dest, source, count):
   if hasattr(dest, 'impl'): dest = dest.impl
   if dest is None: dest = ffi.NULL
   if hasattr(source, 'impl'): source = source.impl
   if source is None: source = ffi.NULL
   lib.eC_moveBytes(dest, source, count)

def pathCat(string, addedPath):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(addedPath, str): addedPath = ffi.new("char[]", addedPath.encode('u8'))
   elif addedPath is None: addedPath = ffi.NULL
   return lib.eC_pathCat(string, addedPath)

def pathCatSlash(string, addedPath):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(addedPath, str): addedPath = ffi.new("char[]", addedPath.encode('u8'))
   elif addedPath is None: addedPath = ffi.NULL
   return lib.eC_pathCatSlash(string, addedPath)

def printx(*args): lib.eC_printx(*convertTypedArgs(args))

def printBigSize(string, size, prec):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   lib.eC_printBigSize(string, size, prec)

def printBuf(*args): lib.eC_printBuf(*convertTypedArgs(args))

def printLn(*args): lib.eC_printLn(*convertTypedArgs(args))

def printLnBuf(*args): lib.eC_printLnBuf(*convertTypedArgs(args))

def printLnString(*args): lib.eC_printLnString(*convertTypedArgs(args))

def printSize(string, size, prec):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   lib.eC_printSize(string, size, prec)

def printString(*args): lib.eC_printString(*convertTypedArgs(args))

def rSearchString(buffer, subStr, maxLen, matchCase, matchWord):
   if isinstance(buffer, str): buffer = ffi.new("char[]", buffer.encode('u8'))
   elif buffer is None: buffer = ffi.NULL
   if isinstance(subStr, str): subStr = ffi.new("char[]", subStr.encode('u8'))
   elif subStr is None: subStr = ffi.NULL
   return lib.eC_rSearchString(buffer, subStr, maxLen, matchCase, matchWord)

def repeatCh(string, count, ch):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   lib.eC_repeatCh(string, count, ch)

def searchString(buffer, start, subStr, matchCase, matchWord):
   if isinstance(buffer, str): buffer = ffi.new("char[]", buffer.encode('u8'))
   elif buffer is None: buffer = ffi.NULL
   if isinstance(subStr, str): subStr = ffi.new("char[]", subStr.encode('u8'))
   elif subStr is None: subStr = ffi.NULL
   return lib.eC_searchString(buffer, start, subStr, matchCase, matchWord)

def setActiveDesigner(designer = None):
   if designer is not None and not isinstance(designer, DesignerBase): designer = DesignerBase(designer)
   designer = ffi.NULL if designer is None else designer.impl
   lib.eC_setActiveDesigner(designer)

def splitArchivePath(fileName, archiveName, archiveFile):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   if isinstance(archiveName, str): archiveName = ffi.new("char[]", archiveName.encode('u8'))
   elif archiveName is None: archiveName = ffi.NULL
   if isinstance(archiveFile, str): archiveFile = ffi.new("char[]", archiveFile.encode('u8'))
   elif archiveFile is None: archiveFile = ffi.NULL
   return lib.eC_splitArchivePath(fileName, archiveName, archiveFile)

def splitDirectory(string, part, rest):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(part, str): part = ffi.new("char[]", part.encode('u8'))
   elif part is None: part = ffi.NULL
   if isinstance(rest, str): rest = ffi.new("char[]", rest.encode('u8'))
   elif rest is None: rest = ffi.NULL
   return lib.eC_splitDirectory(string, part, rest)

def stringLikePattern(string = None, pattern = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(pattern, str): pattern = ffi.new("char[]", pattern.encode('u8'))
   elif pattern is None: pattern = ffi.NULL
   return lib.eC_stringLikePattern(string, pattern)

def stripChars(string = None, chars = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(chars, str): chars = ffi.new("char[]", chars.encode('u8'))
   elif chars is None: chars = ffi.NULL
   return lib.eC_stripChars(string, chars)

def stripExtension(string):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return lib.eC_stripExtension(string)

def stripLastDirectory(string, output):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(output, str): output = ffi.new("char[]", output.encode('u8'))
   elif output is None: output = ffi.NULL
   return lib.eC_stripLastDirectory(string, output)

def stripQuotes(string, output):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(output, str): output = ffi.new("char[]", output.encode('u8'))
   elif output is None: output = ffi.NULL
   return lib.eC_stripQuotes(string, output)

def tokenize(string, maxTokens, tokens, esc):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return lib.eC_tokenize(string, maxTokens, tokens, esc)

def tokenizeWith(string, maxTokens, tokenizers, escapeBackSlashes):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(tokenizers, str): tokenizers = ffi.new("char[]", tokenizers.encode('u8'))
   elif tokenizers is None: tokenizers = ffi.NULL
   tokensArray = ffi.new("eC_String[" + str(maxTokens) + "]")
   nTokens = lib.eC_tokenizeWith(string, maxTokens, tokensArray, tokenizers, escapeBackSlashes)
   tokens = []
   for i in range(nTokens):
      tokens.append(ffi.string(tokensArray[i]).decode('u8'))
   return tokens

def trimLSpaces(string, output):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(output, str): output = ffi.new("char[]", output.encode('u8'))
   elif output is None: output = ffi.NULL
   return lib.eC_trimLSpaces(string, output)

def trimRSpaces(string, output):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if isinstance(output, str): output = ffi.new("char[]", output.encode('u8'))
   elif output is None: output = ffi.NULL
   return lib.eC_trimRSpaces(string, output)

def unescapeCString(d, s, len):
   if isinstance(d, str): d = ffi.new("char[]", d.encode('u8'))
   elif d is None: d = ffi.NULL
   if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
   elif s is None: s = ffi.NULL
   return lib.eC_unescapeCString(d, s, len)

def unescapeCStringLoose(d, s, len):
   if isinstance(d, str): d = ffi.new("char[]", d.encode('u8'))
   elif d is None: d = ffi.NULL
   if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
   elif s is None: s = ffi.NULL
   return lib.eC_unescapeCStringLoose(d, s, len)

def eSystem_LockMem():
   lib.eC_eSystem_LockMem()

def eSystem_UnlockMem():
   lib.eC_eSystem_UnlockMem()

def ishexdigit(x):
   return lib.eC_ishexdigit(x)

def log2i(number):
   return lib.eC_log2i(number)

def memswap(a, b, size):
   lib.eC_memswap(a, b, size)

def pow2i(number):
   return lib.eC_pow2i(number)

def queryMemInfo(string):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   lib.eC_queryMemInfo(string)

def strchrmax(s, c, max):
   if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
   elif s is None: s = ffi.NULL
   return lib.eC_strchrmax(s, c, max)

class ErrorCode(pyBaseClass):
   def __init__(self, level = 0, code = 0, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(level, ErrorCode):
         self.impl = level.impl
      else:
         if isinstance(level, tuple):
            __tuple = level
            level = 0
            if len(__tuple) > 0: level = __tuple[0]
            if len(__tuple) > 1: code = __tuple[1]
         self.impl = (
            (level << lib.ERRORCODE_level_SHIFT) |
            (code  << lib.ERRORCODE_code_SHIFT)  )

   @property
   def level(self): return ((((self.impl)) & lib.ERRORCODE_level_MASK) >> lib.ERRORCODE_level_SHIFT)
   @level.setter
   def level(self, value): self.impl = ((self.impl) & ~(lib.ERRORCODE_level_MASK)) | (((value)) << lib.ERRORCODE_level_SHIFT)

   @property
   def code(self): return ((((self.impl)) & lib.ERRORCODE_code_MASK) >> lib.ERRORCODE_code_SHIFT)
   @code.setter
   def code(self, value): self.impl = ((self.impl) & ~(lib.ERRORCODE_code_MASK)) | (((value)) << lib.ERRORCODE_code_SHIFT)

@ffi.callback("void(eC_File)")
def cb_File_close(__e):
   file = pyOrNewObject(File, __e)
   file.fn_File_close(file)

@ffi.callback("void(eC_File)")
def cb_File_closeInput(__e):
   file = pyOrNewObject(File, __e)
   file.fn_File_closeInput(file)

@ffi.callback("void(eC_File)")
def cb_File_closeOutput(__e):
   file = pyOrNewObject(File, __e)
   file.fn_File_closeOutput(file)

@ffi.callback("eC_bool(eC_File)")
def cb_File_eof(__e):
   file = pyOrNewObject(File, __e)
   return file.fn_File_eof(file)

@ffi.callback("uint64(eC_File)")
def cb_File_getSize(__e):
   file = pyOrNewObject(File, __e)
   return file.fn_File_getSize(file)

@ffi.callback("eC_bool(eC_File, char *)")
def cb_File_getc(__e, ch):
   file = pyOrNewObject(File, __e)
   return file.fn_File_getc(file, ch.encode('u8'))

@ffi.callback("eC_bool(eC_File, eC_FileLock, uint64, uint64, eC_bool)")
def cb_File_lock(__e, type, start, length, wait):
   file = pyOrNewObject(File, __e)
   return file.fn_File_lock(file, FileLock(impl = type), start, length, wait)

@ffi.callback("eC_bool(eC_File, char)")
def cb_File_putc(__e, ch):
   file = pyOrNewObject(File, __e)
   return file.fn_File_putc(file, ch)

@ffi.callback("eC_bool(eC_File, const char *)")
def cb_File_puts(__e, string):
   file = pyOrNewObject(File, __e)
   return file.fn_File_puts(file, string.encode('u8'))

@ffi.callback("uintsize(eC_File, void *, uintsize, uintsize)")
def cb_File_read(__e, buffer, size, count):
   file = pyOrNewObject(File, __e)
   return file.fn_File_read(file, buffer, size, count)

@ffi.callback("eC_bool(eC_File, int64, eC_FileSeekMode)")
def cb_File_seek(__e, pos, mode):
   file = pyOrNewObject(File, __e)
   return file.fn_File_seek(file, pos, FileSeekMode(impl = mode))

@ffi.callback("uint64(eC_File)")
def cb_File_tell(__e):
   file = pyOrNewObject(File, __e)
   return file.fn_File_tell(file)

@ffi.callback("eC_bool(eC_File, uint64)")
def cb_File_truncate(__e, size):
   file = pyOrNewObject(File, __e)
   return file.fn_File_truncate(file, size)

@ffi.callback("eC_bool(eC_File, uint64, uint64, eC_bool)")
def cb_File_unlock(__e, start, length, wait):
   file = pyOrNewObject(File, __e)
   return file.fn_File_unlock(file, start, length, wait)

@ffi.callback("uintsize(eC_File, const void *, uintsize, uintsize)")
def cb_File_write(__e, buffer, size, count):
   file = pyOrNewObject(File, __e)
   return file.fn_File_write(file, buffer, size, count)

class File(IOChannel):
   class_members = [
                      'input',
                      'output',
                      'buffered',
                      'eof',
                      'close',
                      'closeInput',
                      'closeOutput',
                      'eof',
                      'getSize',
                      'getc',
                      'lock',
                      'putc',
                      'puts',
                      'read',
                      'seek',
                      'tell',
                      'truncate',
                      'unlock',
                      'write',
                   ]

   def init_args(self, args, kwArgs): init_args(File, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def input(self): return lib.File_get_input(self.impl)
   @input.setter
   def input(self, value):
      lib.File_set_input(self.impl, value)

   @property
   def output(self): return lib.File_get_output(self.impl)
   @output.setter
   def output(self, value):
      lib.File_set_output(self.impl, value)

   @property
   def buffered(self): return None
   @buffered.setter
   def buffered(self, value):
      lib.File_set_buffered(self.impl, value)

   @property
   def eof(self): return lib.File_get_eof(self.impl)

   def fn_unset_File_close(self):
      return lib.File_close(self.impl)

   @property
   def close(self):
      if hasattr(self, 'fn_File_close'): return self.fn_File_close
      else: return self.fn_unset_File_close
   @close.setter
   def close(self, value):
      self.fn_File_close = value
      lib.Instance_setMethod(self.impl, "Close".encode('u8'), cb_File_close)

   def fn_unset_File_closeInput(self):
      return lib.File_closeInput(self.impl)

   @property
   def closeInput(self):
      if hasattr(self, 'fn_File_closeInput'): return self.fn_File_closeInput
      else: return self.fn_unset_File_closeInput
   @closeInput.setter
   def closeInput(self, value):
      self.fn_File_closeInput = value
      lib.Instance_setMethod(self.impl, "CloseInput".encode('u8'), cb_File_closeInput)

   def fn_unset_File_closeOutput(self):
      return lib.File_closeOutput(self.impl)

   @property
   def closeOutput(self):
      if hasattr(self, 'fn_File_closeOutput'): return self.fn_File_closeOutput
      else: return self.fn_unset_File_closeOutput
   @closeOutput.setter
   def closeOutput(self, value):
      self.fn_File_closeOutput = value
      lib.Instance_setMethod(self.impl, "CloseOutput".encode('u8'), cb_File_closeOutput)

   def copyTo(self, outputFileName):
      if isinstance(outputFileName, str): outputFileName = ffi.new("char[]", outputFileName.encode('u8'))
      elif outputFileName is None: outputFileName = ffi.NULL
      return lib.File_copyTo(self.impl, outputFileName)

   def copyToFile(self, f = None):
      if f is not None and not isinstance(f, File): f = File(f)
      f = ffi.NULL if f is None else f.impl
      return lib.File_copyToFile(self.impl, f)

   def fn_unset_File_eof(self):
      return lib.File_eof(self.impl)

   @property
   def eof(self):
      if hasattr(self, 'fn_File_eof'): return self.fn_File_eof
      else: return self.fn_unset_File_eof
   @eof.setter
   def eof(self, value):
      self.fn_File_eof = value
      lib.Instance_setMethod(self.impl, "Eof".encode('u8'), cb_File_eof)

   def flush(self):
      return lib.File_flush(self.impl)

   def getDouble(self):
      return lib.File_getDouble(self.impl)

   def getFloat(self):
      return lib.File_getFloat(self.impl)

   def getHexValue(self):
      return lib.File_getHexValue(self.impl)

   def getLine(self, s, max):
      if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
      elif s is None: s = ffi.NULL
      return lib.File_getLine(self.impl, s, max)

   def getLineEx(self, s, max):
      if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
      elif s is None: s = ffi.NULL
      hasNewLineChar = ffi.new("eC_bool *")
      r = lib.File_getLineEx(self.impl, s, max, hasNewLineChar)
      return r, hasNewLineChar[0]

   def fn_unset_File_getSize(self):
      return lib.File_getSize(self.impl)

   @property
   def getSize(self):
      if hasattr(self, 'fn_File_getSize'): return self.fn_File_getSize
      else: return self.fn_unset_File_getSize
   @getSize.setter
   def getSize(self, value):
      self.fn_File_getSize = value
      lib.Instance_setMethod(self.impl, "GetSize".encode('u8'), cb_File_getSize)

   def getString(self, string, max):
      if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
      elif string is None: string = ffi.NULL
      return lib.File_getString(self.impl, string, max)

   def getValue(self):
      return lib.File_getValue(self.impl)

   def fn_unset_File_getc(self, ch):
      return lib.File_getc(self.impl, ch)

   @property
   def getc(self):
      if hasattr(self, 'fn_File_getc'): return self.fn_File_getc
      else: return self.fn_unset_File_getc
   @getc.setter
   def getc(self, value):
      self.fn_File_getc = value
      lib.Instance_setMethod(self.impl, "Getc".encode('u8'), cb_File_getc)

   def fn_unset_File_lock(self, type, start, length, wait):
      return lib.File_lock(self.impl, type, start, length, wait)

   @property
   def lock(self):
      if hasattr(self, 'fn_File_lock'): return self.fn_File_lock
      else: return self.fn_unset_File_lock
   @lock.setter
   def lock(self, value):
      self.fn_File_lock = value
      lib.Instance_setMethod(self.impl, "Lock".encode('u8'), cb_File_lock)

   def _print(self, *args): lib.File_print(self.impl, *convertTypedArgs(args))

   def printLn(self, *args): lib.File_printLn(self.impl, *convertTypedArgs(args))

   def printf(self, format, *args):
      if isinstance(format, str): format = ffi.new("char[]", format.encode('u8'))
      elif format is None: format = ffi.NULL
      return lib.File_printf(self.impl, format, *ellipsisArgs(args))

   def fn_unset_File_putc(self, ch):
      return lib.File_putc(self.impl, ch)

   @property
   def putc(self):
      if hasattr(self, 'fn_File_putc'): return self.fn_File_putc
      else: return self.fn_unset_File_putc
   @putc.setter
   def putc(self, value):
      self.fn_File_putc = value
      lib.Instance_setMethod(self.impl, "Putc".encode('u8'), cb_File_putc)

   def fn_unset_File_puts(self, string):
      return lib.File_puts(self.impl, string)

   @property
   def puts(self):
      if hasattr(self, 'fn_File_puts'): return self.fn_File_puts
      else: return self.fn_unset_File_puts
   @puts.setter
   def puts(self, value):
      self.fn_File_puts = value
      lib.Instance_setMethod(self.impl, "Puts".encode('u8'), cb_File_puts)

   def fn_unset_File_read(self, buffer, size, count):
      return lib.File_read(self.impl, buffer, size, count)

   @property
   def read(self):
      if hasattr(self, 'fn_File_read'): return self.fn_File_read
      else: return self.fn_unset_File_read
   @read.setter
   def read(self, value):
      self.fn_File_read = value
      lib.Instance_setMethod(self.impl, "Read".encode('u8'), cb_File_read)

   def fn_unset_File_seek(self, pos, mode):
      return lib.File_seek(self.impl, pos, mode)

   @property
   def seek(self):
      if hasattr(self, 'fn_File_seek'): return self.fn_File_seek
      else: return self.fn_unset_File_seek
   @seek.setter
   def seek(self, value):
      self.fn_File_seek = value
      lib.Instance_setMethod(self.impl, "Seek".encode('u8'), cb_File_seek)

   def fn_unset_File_tell(self):
      return lib.File_tell(self.impl)

   @property
   def tell(self):
      if hasattr(self, 'fn_File_tell'): return self.fn_File_tell
      else: return self.fn_unset_File_tell
   @tell.setter
   def tell(self, value):
      self.fn_File_tell = value
      lib.Instance_setMethod(self.impl, "Tell".encode('u8'), cb_File_tell)

   def fn_unset_File_truncate(self, size):
      return lib.File_truncate(self.impl, size)

   @property
   def truncate(self):
      if hasattr(self, 'fn_File_truncate'): return self.fn_File_truncate
      else: return self.fn_unset_File_truncate
   @truncate.setter
   def truncate(self, value):
      self.fn_File_truncate = value
      lib.Instance_setMethod(self.impl, "Truncate".encode('u8'), cb_File_truncate)

   def fn_unset_File_unlock(self, start, length, wait):
      return lib.File_unlock(self.impl, start, length, wait)

   @property
   def unlock(self):
      if hasattr(self, 'fn_File_unlock'): return self.fn_File_unlock
      else: return self.fn_unset_File_unlock
   @unlock.setter
   def unlock(self, value):
      self.fn_File_unlock = value
      lib.Instance_setMethod(self.impl, "Unlock".encode('u8'), cb_File_unlock)

   def fn_unset_File_write(self, buffer, size, count):
      return lib.File_write(self.impl, buffer, size, count)

   @property
   def write(self):
      if hasattr(self, 'fn_File_write'): return self.fn_File_write
      else: return self.fn_unset_File_write
   @write.setter
   def write(self, value):
      self.fn_File_write = value
      lib.Instance_setMethod(self.impl, "Write".encode('u8'), cb_File_write)

class FileChange(pyBaseClass):
   def __init__(self, created = False, renamed = False, modified = False, deleted = False, attribs = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(created, FileChange):
         self.impl = created.impl
      else:
         if isinstance(created, tuple):
            __tuple = created
            created = False
            if len(__tuple) > 0: created = __tuple[0]
            if len(__tuple) > 1: renamed = __tuple[1]
            if len(__tuple) > 2: modified = __tuple[2]
            if len(__tuple) > 3: deleted = __tuple[3]
            if len(__tuple) > 4: attribs = __tuple[4]
         self.impl = (
            (created  << lib.FILECHANGE_created_SHIFT)  |
            (renamed  << lib.FILECHANGE_renamed_SHIFT)  |
            (modified << lib.FILECHANGE_modified_SHIFT) |
            (deleted  << lib.FILECHANGE_deleted_SHIFT)  |
            (attribs  << lib.FILECHANGE_attribs_SHIFT)  )

   @property
   def created(self): return ((((self.impl)) & lib.FILECHANGE_created_MASK) >> lib.FILECHANGE_created_SHIFT)
   @created.setter
   def created(self, value): self.impl = ((self.impl) & ~(lib.FILECHANGE_created_MASK)) | (((value)) << lib.FILECHANGE_created_SHIFT)

   @property
   def renamed(self): return ((((self.impl)) & lib.FILECHANGE_renamed_MASK) >> lib.FILECHANGE_renamed_SHIFT)
   @renamed.setter
   def renamed(self, value): self.impl = ((self.impl) & ~(lib.FILECHANGE_renamed_MASK)) | (((value)) << lib.FILECHANGE_renamed_SHIFT)

   @property
   def modified(self): return ((((self.impl)) & lib.FILECHANGE_modified_MASK) >> lib.FILECHANGE_modified_SHIFT)
   @modified.setter
   def modified(self, value): self.impl = ((self.impl) & ~(lib.FILECHANGE_modified_MASK)) | (((value)) << lib.FILECHANGE_modified_SHIFT)

   @property
   def deleted(self): return ((((self.impl)) & lib.FILECHANGE_deleted_MASK) >> lib.FILECHANGE_deleted_SHIFT)
   @deleted.setter
   def deleted(self, value): self.impl = ((self.impl) & ~(lib.FILECHANGE_deleted_MASK)) | (((value)) << lib.FILECHANGE_deleted_SHIFT)

   @property
   def attribs(self): return ((((self.impl)) & lib.FILECHANGE_attribs_MASK) >> lib.FILECHANGE_attribs_SHIFT)
   @attribs.setter
   def attribs(self, value): self.impl = ((self.impl) & ~(lib.FILECHANGE_attribs_MASK)) | (((value)) << lib.FILECHANGE_attribs_SHIFT)

AnyFileChange = FileChange ( True, True, True, True, True )

@ffi.callback("eC_bool(eC_Archive)")
def cb_Archive_clear(__e):
   archive = pyOrNewObject(Archive, __e)
   return archive.fn_Archive_clear(archive)

@ffi.callback("eC_FileAttribs(eC_Archive, const char *)")
def cb_Archive_fileExists(__e, fileName):
   archive = pyOrNewObject(Archive, __e)
   return archive.fn_Archive_fileExists(archive, fileName.encode('u8'))

@ffi.callback("eC_File(eC_Archive, const char *)")
def cb_Archive_fileOpen(__e, fileName):
   archive = pyOrNewObject(Archive, __e)
   return archive.fn_Archive_fileOpen(archive, fileName.encode('u8'))

@ffi.callback("eC_File(eC_Archive, uint)")
def cb_Archive_fileOpenAtPosition(__e, position):
   archive = pyOrNewObject(Archive, __e)
   return archive.fn_Archive_fileOpenAtPosition(archive, position)

@ffi.callback("eC_File(eC_Archive, const char *, eC_bool *, uint64 *)")
def cb_Archive_fileOpenCompressed(__e, fileName, isCompressed, ucSize):
   archive = pyOrNewObject(Archive, __e)
   return archive.fn_Archive_fileOpenCompressed(archive, fileName.encode('u8'), isCompressed, ucSize)

@ffi.callback("eC_ArchiveDir(eC_Archive, const char *, eC_FileStats *, eC_ArchiveAddMode)")
def cb_Archive_openDirectory(__e, name, stats, addMode):
   archive = pyOrNewObject(Archive, __e)
   return archive.fn_Archive_openDirectory(archive, name.encode('u8'), FileStats(impl = stats), ArchiveAddMode(impl = addMode))

@ffi.callback("void(eC_Archive, uint)")
def cb_Archive_setBufferRead(__e, bufferRead):
   archive = pyOrNewObject(Archive, __e)
   archive.fn_Archive_setBufferRead(archive, bufferRead)

@ffi.callback("void(eC_Archive, uint)")
def cb_Archive_setBufferSize(__e, bufferSize):
   archive = pyOrNewObject(Archive, __e)
   archive.fn_Archive_setBufferSize(archive, bufferSize)

class Archive(Instance):
   class_members = [
                      'totalSize',
                      'bufferSize',
                      'bufferRead',
                      'clear',
                      'fileExists',
                      'fileOpen',
                      'fileOpenAtPosition',
                      'fileOpenCompressed',
                      'openDirectory',
                      'setBufferRead',
                      'setBufferSize',
                   ]

   def init_args(self, args, kwArgs): init_args(Archive, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def totalSize(self): return FileSize(impl = lib.Archive_get_totalSize(self.impl))
   @totalSize.setter
   def totalSize(self, value):
      if not isinstance(value, FileSize): value = FileSize(value)
      lib.Archive_set_totalSize(self.impl, value.impl)

   @property
   def bufferSize(self): return None
   @bufferSize.setter
   def bufferSize(self, value):
      lib.Archive_set_bufferSize(self.impl, value)

   @property
   def bufferRead(self): return None
   @bufferRead.setter
   def bufferRead(self, value):
      lib.Archive_set_bufferRead(self.impl, value)

   def fn_unset_Archive_clear(self):
      return lib.Archive_clear(self.impl)

   @property
   def clear(self):
      if hasattr(self, 'fn_Archive_clear'): return self.fn_Archive_clear
      else: return self.fn_unset_Archive_clear
   @clear.setter
   def clear(self, value):
      self.fn_Archive_clear = value
      lib.Instance_setMethod(self.impl, "Clear".encode('u8'), cb_Archive_clear)

   def fn_unset_Archive_fileExists(self, fileName):
      return lib.Archive_fileExists(self.impl, fileName)

   @property
   def fileExists(self):
      if hasattr(self, 'fn_Archive_fileExists'): return self.fn_Archive_fileExists
      else: return self.fn_unset_Archive_fileExists
   @fileExists.setter
   def fileExists(self, value):
      self.fn_Archive_fileExists = value
      lib.Instance_setMethod(self.impl, "FileExists".encode('u8'), cb_Archive_fileExists)

   def fn_unset_Archive_fileOpen(self, fileName):
      return pyOrNewObject(File, lib.Archive_fileOpen(self.impl, fileName))

   @property
   def fileOpen(self):
      if hasattr(self, 'fn_Archive_fileOpen'): return self.fn_Archive_fileOpen
      else: return self.fn_unset_Archive_fileOpen
   @fileOpen.setter
   def fileOpen(self, value):
      self.fn_Archive_fileOpen = value
      lib.Instance_setMethod(self.impl, "FileOpen".encode('u8'), cb_Archive_fileOpen)

   def fn_unset_Archive_fileOpenAtPosition(self, position):
      return pyOrNewObject(File, lib.Archive_fileOpenAtPosition(self.impl, position))

   @property
   def fileOpenAtPosition(self):
      if hasattr(self, 'fn_Archive_fileOpenAtPosition'): return self.fn_Archive_fileOpenAtPosition
      else: return self.fn_unset_Archive_fileOpenAtPosition
   @fileOpenAtPosition.setter
   def fileOpenAtPosition(self, value):
      self.fn_Archive_fileOpenAtPosition = value
      lib.Instance_setMethod(self.impl, "FileOpenAtPosition".encode('u8'), cb_Archive_fileOpenAtPosition)

   def fn_unset_Archive_fileOpenCompressed(self, fileName, isCompressed, ucSize):
      if isCompressed is None: isCompressed = ffi.NULL
      if ucSize is None: ucSize = ffi.NULL
      return pyOrNewObject(File, lib.Archive_fileOpenCompressed(self.impl, fileName, isCompressed, ucSize))

   @property
   def fileOpenCompressed(self):
      if hasattr(self, 'fn_Archive_fileOpenCompressed'): return self.fn_Archive_fileOpenCompressed
      else: return self.fn_unset_Archive_fileOpenCompressed
   @fileOpenCompressed.setter
   def fileOpenCompressed(self, value):
      self.fn_Archive_fileOpenCompressed = value
      lib.Instance_setMethod(self.impl, "FileOpenCompressed".encode('u8'), cb_Archive_fileOpenCompressed)

   def fn_unset_Archive_openDirectory(self, name, stats, addMode):
      return pyOrNewObject(ArchiveDir, lib.Archive_openDirectory(self.impl, name, ffi.NULL if stats is None else stats.impl, addMode))

   @property
   def openDirectory(self):
      if hasattr(self, 'fn_Archive_openDirectory'): return self.fn_Archive_openDirectory
      else: return self.fn_unset_Archive_openDirectory
   @openDirectory.setter
   def openDirectory(self, value):
      self.fn_Archive_openDirectory = value
      lib.Instance_setMethod(self.impl, "OpenDirectory".encode('u8'), cb_Archive_openDirectory)

   def fn_unset_Archive_setBufferRead(self, bufferRead):
      return lib.Archive_setBufferRead(self.impl, bufferRead)

   @property
   def setBufferRead(self):
      if hasattr(self, 'fn_Archive_setBufferRead'): return self.fn_Archive_setBufferRead
      else: return self.fn_unset_Archive_setBufferRead
   @setBufferRead.setter
   def setBufferRead(self, value):
      self.fn_Archive_setBufferRead = value
      lib.Instance_setMethod(self.impl, "SetBufferRead".encode('u8'), cb_Archive_setBufferRead)

   def fn_unset_Archive_setBufferSize(self, bufferSize):
      return lib.Archive_setBufferSize(self.impl, bufferSize)

   @property
   def setBufferSize(self):
      if hasattr(self, 'fn_Archive_setBufferSize'): return self.fn_Archive_setBufferSize
      else: return self.fn_unset_Archive_setBufferSize
   @setBufferSize.setter
   def setBufferSize(self, value):
      self.fn_Archive_setBufferSize = value
      lib.Instance_setMethod(self.impl, "SetBufferSize".encode('u8'), cb_Archive_setBufferSize)

class ArchiveAddMode:
   replace     = lib.ArchiveAddMode_replace
   refresh     = lib.ArchiveAddMode_refresh
   update      = lib.ArchiveAddMode_update
   readOnlyDir = lib.ArchiveAddMode_readOnlyDir

@ffi.callback("eC_bool(eC_ArchiveDir, const char *, eC_File, eC_FileStats *, eC_ArchiveAddMode, int, int *, uint *)")
def cb_ArchiveDir_addFromFile(__e, name, input, stats, addMode, compression, ratio, newPosition):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_addFromFile(archivedir, name.encode('u8'), pyOrNewObject(File, input), FileStats(impl = stats), ArchiveAddMode(impl = addMode), compression, ratio, newPosition)

@ffi.callback("eC_bool(eC_ArchiveDir, uint, const char *, eC_File, eC_FileStats *, eC_ArchiveAddMode, int, int *, uint *)")
def cb_ArchiveDir_addFromFileAtPosition(__e, position, name, input, stats, addMode, compression, ratio, newPosition):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_addFromFileAtPosition(archivedir, position, name.encode('u8'), pyOrNewObject(File, input), FileStats(impl = stats), ArchiveAddMode(impl = addMode), compression, ratio, newPosition)

@ffi.callback("eC_bool(eC_ArchiveDir, const char *)")
def cb_ArchiveDir_delete(__e, fileName):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_delete(archivedir, fileName.encode('u8'))

@ffi.callback("eC_FileAttribs(eC_ArchiveDir, const char *)")
def cb_ArchiveDir_fileExists(__e, fileName):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_fileExists(archivedir, fileName.encode('u8'))

@ffi.callback("eC_File(eC_ArchiveDir, const char *)")
def cb_ArchiveDir_fileOpen(__e, fileName):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_fileOpen(archivedir, fileName.encode('u8'))

@ffi.callback("eC_bool(eC_ArchiveDir, const char *, eC_ArchiveDir)")
def cb_ArchiveDir_move(__e, name, to):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_move(archivedir, name.encode('u8'), pyOrNewObject(ArchiveDir, to))

@ffi.callback("eC_ArchiveDir(eC_ArchiveDir, const char *, eC_FileStats *, eC_ArchiveAddMode)")
def cb_ArchiveDir_openDirectory(__e, name, stats, addMode):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_openDirectory(archivedir, name.encode('u8'), FileStats(impl = stats), ArchiveAddMode(impl = addMode))

@ffi.callback("eC_bool(eC_ArchiveDir, const char *, const char *)")
def cb_ArchiveDir_rename(__e, name, newName):
   archivedir = pyOrNewObject(ArchiveDir, __e)
   return archivedir.fn_ArchiveDir_rename(archivedir, name.encode('u8'), newName.encode('u8'))

class ArchiveDir(Instance):
   class_members = [
                      'addFromFile',
                      'addFromFileAtPosition',
                      'delete',
                      'fileExists',
                      'fileOpen',
                      'move',
                      'openDirectory',
                      'rename',
                   ]

   def init_args(self, args, kwArgs): init_args(ArchiveDir, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   def add(self, name, path, addMode, compression):
      if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
      elif name is None: name = ffi.NULL
      if isinstance(path, str): path = ffi.new("char[]", path.encode('u8'))
      elif path is None: path = ffi.NULL
      ratio = ffi.new("int *")
      newPosition = ffi.new("uint *")
      r = lib.ArchiveDir_add(self.impl, name, path, addMode, compression, ratio, newPosition)
      return r, ratio[0], newPosition[0]

   def fn_unset_ArchiveDir_addFromFile(self, name, input, stats, addMode, compression, ratio, newPosition):
      if ratio is None: ratio = ffi.NULL
      if newPosition is None: newPosition = ffi.NULL
      return lib.ArchiveDir_addFromFile(self.impl, name, ffi.NULL if input is None else input.impl, ffi.NULL if stats is None else stats.impl, addMode, compression, ratio, newPosition)

   @property
   def addFromFile(self):
      if hasattr(self, 'fn_ArchiveDir_addFromFile'): return self.fn_ArchiveDir_addFromFile
      else: return self.fn_unset_ArchiveDir_addFromFile
   @addFromFile.setter
   def addFromFile(self, value):
      self.fn_ArchiveDir_addFromFile = value
      lib.Instance_setMethod(self.impl, "AddFromFile".encode('u8'), cb_ArchiveDir_addFromFile)

   def fn_unset_ArchiveDir_addFromFileAtPosition(self, position, name, input, stats, addMode, compression, ratio, newPosition):
      if ratio is None: ratio = ffi.NULL
      if newPosition is None: newPosition = ffi.NULL
      return lib.ArchiveDir_addFromFileAtPosition(self.impl, position, name, ffi.NULL if input is None else input.impl, ffi.NULL if stats is None else stats.impl, addMode, compression, ratio, newPosition)

   @property
   def addFromFileAtPosition(self):
      if hasattr(self, 'fn_ArchiveDir_addFromFileAtPosition'): return self.fn_ArchiveDir_addFromFileAtPosition
      else: return self.fn_unset_ArchiveDir_addFromFileAtPosition
   @addFromFileAtPosition.setter
   def addFromFileAtPosition(self, value):
      self.fn_ArchiveDir_addFromFileAtPosition = value
      lib.Instance_setMethod(self.impl, "AddFromFileAtPosition".encode('u8'), cb_ArchiveDir_addFromFileAtPosition)

   def fn_unset_ArchiveDir_delete(self, fileName):
      return lib.ArchiveDir_delete(self.impl, fileName)

   @property
   def delete(self):
      if hasattr(self, 'fn_ArchiveDir_delete'): return self.fn_ArchiveDir_delete
      else: return self.fn_unset_ArchiveDir_delete
   @delete.setter
   def delete(self, value):
      self.fn_ArchiveDir_delete = value
      lib.Instance_setMethod(self.impl, "Delete".encode('u8'), cb_ArchiveDir_delete)

   def fn_unset_ArchiveDir_fileExists(self, fileName):
      return lib.ArchiveDir_fileExists(self.impl, fileName)

   @property
   def fileExists(self):
      if hasattr(self, 'fn_ArchiveDir_fileExists'): return self.fn_ArchiveDir_fileExists
      else: return self.fn_unset_ArchiveDir_fileExists
   @fileExists.setter
   def fileExists(self, value):
      self.fn_ArchiveDir_fileExists = value
      lib.Instance_setMethod(self.impl, "FileExists".encode('u8'), cb_ArchiveDir_fileExists)

   def fn_unset_ArchiveDir_fileOpen(self, fileName):
      return pyOrNewObject(File, lib.ArchiveDir_fileOpen(self.impl, fileName))

   @property
   def fileOpen(self):
      if hasattr(self, 'fn_ArchiveDir_fileOpen'): return self.fn_ArchiveDir_fileOpen
      else: return self.fn_unset_ArchiveDir_fileOpen
   @fileOpen.setter
   def fileOpen(self, value):
      self.fn_ArchiveDir_fileOpen = value
      lib.Instance_setMethod(self.impl, "FileOpen".encode('u8'), cb_ArchiveDir_fileOpen)

   def fn_unset_ArchiveDir_move(self, name, to):
      return lib.ArchiveDir_move(self.impl, name, ffi.NULL if to is None else to.impl)

   @property
   def move(self):
      if hasattr(self, 'fn_ArchiveDir_move'): return self.fn_ArchiveDir_move
      else: return self.fn_unset_ArchiveDir_move
   @move.setter
   def move(self, value):
      self.fn_ArchiveDir_move = value
      lib.Instance_setMethod(self.impl, "Move".encode('u8'), cb_ArchiveDir_move)

   def fn_unset_ArchiveDir_openDirectory(self, name, stats, addMode):
      return pyOrNewObject(ArchiveDir, lib.ArchiveDir_openDirectory(self.impl, name, ffi.NULL if stats is None else stats.impl, addMode))

   @property
   def openDirectory(self):
      if hasattr(self, 'fn_ArchiveDir_openDirectory'): return self.fn_ArchiveDir_openDirectory
      else: return self.fn_unset_ArchiveDir_openDirectory
   @openDirectory.setter
   def openDirectory(self, value):
      self.fn_ArchiveDir_openDirectory = value
      lib.Instance_setMethod(self.impl, "OpenDirectory".encode('u8'), cb_ArchiveDir_openDirectory)

   def fn_unset_ArchiveDir_rename(self, name, newName):
      return lib.ArchiveDir_rename(self.impl, name, newName)

   @property
   def rename(self):
      if hasattr(self, 'fn_ArchiveDir_rename'): return self.fn_ArchiveDir_rename
      else: return self.fn_unset_ArchiveDir_rename
   @rename.setter
   def rename(self, value):
      self.fn_ArchiveDir_rename = value
      lib.Instance_setMethod(self.impl, "Rename".encode('u8'), cb_ArchiveDir_rename)

class ArchiveOpenFlags(pyBaseClass):
   def __init__(self, writeAccess = False, buffered = False, exclusive = False, waitLock = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(writeAccess, ArchiveOpenFlags):
         self.impl = writeAccess.impl
      else:
         if isinstance(writeAccess, tuple):
            __tuple = writeAccess
            writeAccess = False
            if len(__tuple) > 0: writeAccess = __tuple[0]
            if len(__tuple) > 1: buffered = __tuple[1]
            if len(__tuple) > 2: exclusive = __tuple[2]
            if len(__tuple) > 3: waitLock = __tuple[3]
         self.impl = (
            (writeAccess << lib.ARCHIVEOPENFLAGS_writeAccess_SHIFT) |
            (buffered    << lib.ARCHIVEOPENFLAGS_buffered_SHIFT)    |
            (exclusive   << lib.ARCHIVEOPENFLAGS_exclusive_SHIFT)   |
            (waitLock    << lib.ARCHIVEOPENFLAGS_waitLock_SHIFT)    )

   @property
   def writeAccess(self): return ((((self.impl)) & lib.ARCHIVEOPENFLAGS_writeAccess_MASK) >> lib.ARCHIVEOPENFLAGS_writeAccess_SHIFT)
   @writeAccess.setter
   def writeAccess(self, value): self.impl = ((self.impl) & ~(lib.ARCHIVEOPENFLAGS_writeAccess_MASK)) | (((value)) << lib.ARCHIVEOPENFLAGS_writeAccess_SHIFT)

   @property
   def buffered(self): return ((((self.impl)) & lib.ARCHIVEOPENFLAGS_buffered_MASK) >> lib.ARCHIVEOPENFLAGS_buffered_SHIFT)
   @buffered.setter
   def buffered(self, value): self.impl = ((self.impl) & ~(lib.ARCHIVEOPENFLAGS_buffered_MASK)) | (((value)) << lib.ARCHIVEOPENFLAGS_buffered_SHIFT)

   @property
   def exclusive(self): return ((((self.impl)) & lib.ARCHIVEOPENFLAGS_exclusive_MASK) >> lib.ARCHIVEOPENFLAGS_exclusive_SHIFT)
   @exclusive.setter
   def exclusive(self, value): self.impl = ((self.impl) & ~(lib.ARCHIVEOPENFLAGS_exclusive_MASK)) | (((value)) << lib.ARCHIVEOPENFLAGS_exclusive_SHIFT)

   @property
   def waitLock(self): return ((((self.impl)) & lib.ARCHIVEOPENFLAGS_waitLock_MASK) >> lib.ARCHIVEOPENFLAGS_waitLock_SHIFT)
   @waitLock.setter
   def waitLock(self, value): self.impl = ((self.impl) & ~(lib.ARCHIVEOPENFLAGS_waitLock_MASK)) | (((value)) << lib.ARCHIVEOPENFLAGS_waitLock_SHIFT)

class BufferedFile(File):
   class_members = [
                      'handle',
                      'bufferSize',
                      'bufferRead',
                   ]

   def init_args(self, args, kwArgs): init_args(BufferedFile, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def handle(self): return pyOrNewObject(File, lib.BufferedFile_get_handle(self.impl))
   @handle.setter
   def handle(self, value):
      if not isinstance(value, File): value = File(value)
      lib.BufferedFile_set_handle(self.impl, value.impl)

   @property
   def bufferSize(self): return lib.BufferedFile_get_bufferSize(self.impl)
   @bufferSize.setter
   def bufferSize(self, value):
      lib.BufferedFile_set_bufferSize(self.impl, value)

   @property
   def bufferRead(self): return lib.BufferedFile_get_bufferRead(self.impl)
   @bufferRead.setter
   def bufferRead(self, value):
      lib.BufferedFile_set_bufferRead(self.impl, value)

class ConsoleFile(File):
   class_members = []

   def init_args(self, args, kwArgs): init_args(ConsoleFile, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class DualPipe(File):
   class_members = []

   def init_args(self, args, kwArgs): init_args(DualPipe, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   def getExitCode(self):
      return lib.DualPipe_getExitCode(self.impl)

   def getLinePeek(self, s, max):
      if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
      elif s is None: s = ffi.NULL
      charsRead = ffi.new("int *")
      r = lib.DualPipe_getLinePeek(self.impl, s, max, charsRead)
      return r, charsRead[0]

   def getProcessID(self):
      return lib.DualPipe_getProcessID(self.impl)

   def peek(self):
      return lib.DualPipe_peek(self.impl)

   def terminate(self):
      lib.DualPipe_terminate(self.impl)

   def wait(self):
      lib.DualPipe_wait(self.impl)

class ErrorLevel:
   veryFatal = lib.ErrorLevel_veryFatal
   fatal     = lib.ErrorLevel_fatal
   major     = lib.ErrorLevel_major
   minor     = lib.ErrorLevel_minor

class FileAttribs(Bool):
   def __init__(self, isFile = False, isArchive = False, isHidden = False, isReadOnly = False, isSystem = False, isTemporary = False, isDirectory = False, isDrive = False, isCDROM = False, isRemote = False, isRemovable = False, isServer = False, isShare = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(isFile, FileAttribs):
         self.impl = isFile.impl
      else:
         self.impl = (
            (isFile      << lib.FILEATTRIBS_isFile_SHIFT)      |
            (isArchive   << lib.FILEATTRIBS_isArchive_SHIFT)   |
            (isHidden    << lib.FILEATTRIBS_isHidden_SHIFT)    |
            (isReadOnly  << lib.FILEATTRIBS_isReadOnly_SHIFT)  |
            (isSystem    << lib.FILEATTRIBS_isSystem_SHIFT)    |
            (isTemporary << lib.FILEATTRIBS_isTemporary_SHIFT) |
            (isDirectory << lib.FILEATTRIBS_isDirectory_SHIFT) |
            (isDrive     << lib.FILEATTRIBS_isDrive_SHIFT)     |
            (isCDROM     << lib.FILEATTRIBS_isCDROM_SHIFT)     |
            (isRemote    << lib.FILEATTRIBS_isRemote_SHIFT)    |
            (isRemovable << lib.FILEATTRIBS_isRemovable_SHIFT) |
            (isServer    << lib.FILEATTRIBS_isServer_SHIFT)    |
            (isShare     << lib.FILEATTRIBS_isShare_SHIFT)     )

   @property
   def isFile(self): return ((((self.impl)) & lib.FILEATTRIBS_isFile_MASK) >> lib.FILEATTRIBS_isFile_SHIFT)
   @isFile.setter
   def isFile(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isFile_MASK)) | (((value)) << lib.FILEATTRIBS_isFile_SHIFT)

   @property
   def isArchive(self): return ((((self.impl)) & lib.FILEATTRIBS_isArchive_MASK) >> lib.FILEATTRIBS_isArchive_SHIFT)
   @isArchive.setter
   def isArchive(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isArchive_MASK)) | (((value)) << lib.FILEATTRIBS_isArchive_SHIFT)

   @property
   def isHidden(self): return ((((self.impl)) & lib.FILEATTRIBS_isHidden_MASK) >> lib.FILEATTRIBS_isHidden_SHIFT)
   @isHidden.setter
   def isHidden(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isHidden_MASK)) | (((value)) << lib.FILEATTRIBS_isHidden_SHIFT)

   @property
   def isReadOnly(self): return ((((self.impl)) & lib.FILEATTRIBS_isReadOnly_MASK) >> lib.FILEATTRIBS_isReadOnly_SHIFT)
   @isReadOnly.setter
   def isReadOnly(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isReadOnly_MASK)) | (((value)) << lib.FILEATTRIBS_isReadOnly_SHIFT)

   @property
   def isSystem(self): return ((((self.impl)) & lib.FILEATTRIBS_isSystem_MASK) >> lib.FILEATTRIBS_isSystem_SHIFT)
   @isSystem.setter
   def isSystem(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isSystem_MASK)) | (((value)) << lib.FILEATTRIBS_isSystem_SHIFT)

   @property
   def isTemporary(self): return ((((self.impl)) & lib.FILEATTRIBS_isTemporary_MASK) >> lib.FILEATTRIBS_isTemporary_SHIFT)
   @isTemporary.setter
   def isTemporary(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isTemporary_MASK)) | (((value)) << lib.FILEATTRIBS_isTemporary_SHIFT)

   @property
   def isDirectory(self): return ((((self.impl)) & lib.FILEATTRIBS_isDirectory_MASK) >> lib.FILEATTRIBS_isDirectory_SHIFT)
   @isDirectory.setter
   def isDirectory(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isDirectory_MASK)) | (((value)) << lib.FILEATTRIBS_isDirectory_SHIFT)

   @property
   def isDrive(self): return ((((self.impl)) & lib.FILEATTRIBS_isDrive_MASK) >> lib.FILEATTRIBS_isDrive_SHIFT)
   @isDrive.setter
   def isDrive(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isDrive_MASK)) | (((value)) << lib.FILEATTRIBS_isDrive_SHIFT)

   @property
   def isCDROM(self): return ((((self.impl)) & lib.FILEATTRIBS_isCDROM_MASK) >> lib.FILEATTRIBS_isCDROM_SHIFT)
   @isCDROM.setter
   def isCDROM(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isCDROM_MASK)) | (((value)) << lib.FILEATTRIBS_isCDROM_SHIFT)

   @property
   def isRemote(self): return ((((self.impl)) & lib.FILEATTRIBS_isRemote_MASK) >> lib.FILEATTRIBS_isRemote_SHIFT)
   @isRemote.setter
   def isRemote(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isRemote_MASK)) | (((value)) << lib.FILEATTRIBS_isRemote_SHIFT)

   @property
   def isRemovable(self): return ((((self.impl)) & lib.FILEATTRIBS_isRemovable_MASK) >> lib.FILEATTRIBS_isRemovable_SHIFT)
   @isRemovable.setter
   def isRemovable(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isRemovable_MASK)) | (((value)) << lib.FILEATTRIBS_isRemovable_SHIFT)

   @property
   def isServer(self): return ((((self.impl)) & lib.FILEATTRIBS_isServer_MASK) >> lib.FILEATTRIBS_isServer_SHIFT)
   @isServer.setter
   def isServer(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isServer_MASK)) | (((value)) << lib.FILEATTRIBS_isServer_SHIFT)

   @property
   def isShare(self): return ((((self.impl)) & lib.FILEATTRIBS_isShare_MASK) >> lib.FILEATTRIBS_isShare_SHIFT)
   @isShare.setter
   def isShare(self, value): self.impl = ((self.impl) & ~(lib.FILEATTRIBS_isShare_MASK)) | (((value)) << lib.FILEATTRIBS_isShare_SHIFT)

class FileListing(Struct):
   def __init__(self, directory = None, extensions = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_FileListing *", impl)
      else:
         if isinstance(directory, tuple):
            __tuple = directory
            directory = None
            if len(__tuple) > 0: directory  = __tuple[0]
            if len(__tuple) > 1: extensions = __tuple[1]
         self.impl = ffi.new("eC_FileListing *", { 'directory' : directory, 'extensions' : extensions })

   @property
   def directory(self): return self.impl.directory
   @directory.setter
   def directory(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.directory = value

   @property
   def extensions(self): return self.impl.extensions
   @extensions.setter
   def extensions(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.extensions = value

   @property
   def name(self): value = lib.FileListing_get_name(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')

   @property
   def path(self): value = lib.FileListing_get_path(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')

   @property
   def stats(self): value = FileStats(); lib.FileListing_get_stats(self.impl, ffi.cast("eC_FileStats *", value.impl)); return value

   def find(self):
      return lib.FileListing_find(ffi.cast("eC_FileListing *", self.impl))

   def stop(self):
      lib.FileListing_stop(ffi.cast("eC_FileListing *", self.impl))

class FileLock:
   unlocked  = lib.FileLock_unlocked
   shared    = lib.FileLock_shared
   exclusive = lib.FileLock_exclusive

@ffi.callback("eC_bool(eC_FileMonitor, eC_FileChange, const char *, const char *)")
def cb_FileMonitor_onDirNotify(__e, action, fileName, param):
   filemonitor = pyOrNewObject(FileMonitor, __e)
   return filemonitor.fn_FileMonitor_onDirNotify(filemonitor.userData, action, fileName.encode('u8'), param.encode('u8'))

@ffi.callback("eC_bool(eC_FileMonitor, eC_FileChange, const char *)")
def cb_FileMonitor_onFileNotify(__e, action, param):
   filemonitor = pyOrNewObject(FileMonitor, __e)
   return filemonitor.fn_FileMonitor_onFileNotify(filemonitor.userData, action, param.encode('u8'))

class FileMonitor(Instance):
   class_members = [
                      'userData',
                      'fileChange',
                      'fileName',
                      'directoryName',
                      'onDirNotify',
                      'onFileNotify',
                   ]

   def init_args(self, args, kwArgs): init_args(FileMonitor, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)
      if self.impl != ffi.NULL: lib.FileMonitor_set_userData(self.impl, self.impl)

   userData = None

   @property
   def fileChange(self): return None
   @fileChange.setter
   def fileChange(self, value):
      if not isinstance(value, FileChange): value = FileChange(value)
      lib.FileMonitor_set_fileChange(self.impl, value.impl)

   @property
   def fileName(self): value = lib.FileMonitor_get_fileName(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @fileName.setter
   def fileName(self, value):
      lib.FileMonitor_set_fileName(self.impl, value.encode('u8'))

   @property
   def directoryName(self): value = lib.FileMonitor_get_directoryName(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @directoryName.setter
   def directoryName(self, value):
      lib.FileMonitor_set_directoryName(self.impl, value.encode('u8'))

   def fn_unset_FileMonitor_onDirNotify(self, _ec_instance, action, fileName, param):
      return lib.FileMonitor_onDirNotify(self.impl, _ec_instance.impl, action, fileName, param)

   @property
   def onDirNotify(self):
      if hasattr(self, 'fn_FileMonitor_onDirNotify'): return self.fn_FileMonitor_onDirNotify
      else: return self.fn_unset_FileMonitor_onDirNotify
   @onDirNotify.setter
   def onDirNotify(self, value):
      self.fn_FileMonitor_onDirNotify = value
      lib.Instance_setMethod(self.impl, "OnDirNotify".encode('u8'), cb_FileMonitor_onDirNotify)

   def fn_unset_FileMonitor_onFileNotify(self, _ec_instance, action, param):
      return lib.FileMonitor_onFileNotify(self.impl, _ec_instance.impl, action, param)

   @property
   def onFileNotify(self):
      if hasattr(self, 'fn_FileMonitor_onFileNotify'): return self.fn_FileMonitor_onFileNotify
      else: return self.fn_unset_FileMonitor_onFileNotify
   @onFileNotify.setter
   def onFileNotify(self, value):
      self.fn_FileMonitor_onFileNotify = value
      lib.Instance_setMethod(self.impl, "OnFileNotify".encode('u8'), cb_FileMonitor_onFileNotify)

   def startMonitoring(self):
      lib.FileMonitor_startMonitoring(self.impl)

   def stopMonitoring(self):
      lib.FileMonitor_stopMonitoring(self.impl)

class FileOpenMode:
   read       = lib.FileOpenMode_read
   write      = lib.FileOpenMode_write
   append     = lib.FileOpenMode_append
   readWrite  = lib.FileOpenMode_readWrite
   writeRead  = lib.FileOpenMode_writeRead
   appendRead = lib.FileOpenMode_appendRead

class FileSeekMode:
   start   = lib.FileSeekMode_start
   current = lib.FileSeekMode_current
   end     = lib.FileSeekMode_end

class FileSize(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

FileSize.buc = FileSize

class FileSize64(pyBaseClass):
   def __init__(self, impl = 0):
      self.impl = impl

FileSize64.buc = FileSize64

class FileStats(Struct):
   def __init__(self, attribs = None, size = 0, accessed = 0, modified = 0, created = 0, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_FileStats *", impl)
      else:
         if isinstance(attribs, tuple):
            __tuple = attribs
            attribs = 0
            if len(__tuple) > 0: attribs  = __tuple[0]
            if len(__tuple) > 1: size     = __tuple[1]
            if len(__tuple) > 2: accessed = __tuple[2]
            if len(__tuple) > 3: modified = __tuple[3]
         if attribs is not None:
            if not isinstance(attribs, FileAttribs): attribs = FileAttribs(attribs)
            attribs = attribs.impl
         else:
            attribs = FileAttribs()
         if accessed is not None:
            if not isinstance(accessed, SecSince1970): accessed = SecSince1970(accessed)
            accessed = accessed.impl
         else:
            accessed = SecSince1970()
         if modified is not None:
            if not isinstance(modified, SecSince1970): modified = SecSince1970(modified)
            modified = modified.impl
         else:
            modified = SecSince1970()
         if created is not None:
            if not isinstance(created, SecSince1970): created = SecSince1970(created)
            created = created.impl
         else:
            created = SecSince1970()
         self.impl = ffi.new("eC_FileStats *", {
                                'attribs' : attribs,
                                'size' : size,
                                'accessed' : accessed,
                                'modified' : modified,
                                'created' : created
                             })

   @property
   def attribs(self): return FileAttribs(impl = self.impl.attribs)
   @attribs.setter
   def attribs(self, value):
      if not isinstance(value, FileAttribs): value = FileAttribs(value)
      self.impl.attribs = value.impl

   @property
   def size(self): return self.impl.size
   @size.setter
   def size(self, value): self.impl.size = value

   @property
   def accessed(self): return SecSince1970(impl = self.impl.accessed)
   @accessed.setter
   def accessed(self, value):
      if not isinstance(value, SecSince1970): value = SecSince1970(value)
      self.impl.accessed = value.impl

   @property
   def modified(self): return SecSince1970(impl = self.impl.modified)
   @modified.setter
   def modified(self, value):
      if not isinstance(value, SecSince1970): value = SecSince1970(value)
      self.impl.modified = value.impl

   @property
   def created(self): return SecSince1970(impl = self.impl.created)
   @created.setter
   def created(self, value):
      if not isinstance(value, SecSince1970): value = SecSince1970(value)
      self.impl.created = value.impl

class GuiErrorCode(ErrorCode):
   driverNotSupported    = ErrorCode(impl = lib.GuiErrorCode_driverNotSupported)
   windowCreationFailed  = ErrorCode(impl = lib.GuiErrorCode_windowCreationFailed)
   graphicsLoadingFailed = ErrorCode(impl = lib.GuiErrorCode_graphicsLoadingFailed)
   modeSwitchFailed      = ErrorCode(impl = lib.GuiErrorCode_modeSwitchFailed)

class LoggingMode:
   noLogging = lib.LoggingMode_noLogging
   stdOut    = lib.LoggingMode_stdOut
   stdErr    = lib.LoggingMode_stdErr
   debug     = lib.LoggingMode_debug
   logFile   = lib.LoggingMode_logFile
   msgBox    = lib.LoggingMode_msgBox
   buffer    = lib.LoggingMode_buffer

class MoveFileOptions(pyBaseClass):
   def __init__(self, overwrite = False, sync = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(overwrite, MoveFileOptions):
         self.impl = overwrite.impl
      else:
         if isinstance(overwrite, tuple):
            __tuple = overwrite
            overwrite = False
            if len(__tuple) > 0: overwrite = __tuple[0]
            if len(__tuple) > 1: sync = __tuple[1]
         self.impl = (
            (overwrite << lib.MOVEFILEOPTIONS_overwrite_SHIFT) |
            (sync      << lib.MOVEFILEOPTIONS_sync_SHIFT)      )

   @property
   def overwrite(self): return ((((self.impl)) & lib.MOVEFILEOPTIONS_overwrite_MASK) >> lib.MOVEFILEOPTIONS_overwrite_SHIFT)
   @overwrite.setter
   def overwrite(self, value): self.impl = ((self.impl) & ~(lib.MOVEFILEOPTIONS_overwrite_MASK)) | (((value)) << lib.MOVEFILEOPTIONS_overwrite_SHIFT)

   @property
   def sync(self): return ((((self.impl)) & lib.MOVEFILEOPTIONS_sync_MASK) >> lib.MOVEFILEOPTIONS_sync_SHIFT)
   @sync.setter
   def sync(self, value): self.impl = ((self.impl) & ~(lib.MOVEFILEOPTIONS_sync_MASK)) | (((value)) << lib.MOVEFILEOPTIONS_sync_SHIFT)

class PipeOpenMode(pyBaseClass):
   def __init__(self, output = False, error = False, input = False, showWindow = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(output, PipeOpenMode):
         self.impl = output.impl
      else:
         if isinstance(output, tuple):
            __tuple = output
            output = False
            if len(__tuple) > 0: output = __tuple[0]
            if len(__tuple) > 1: error = __tuple[1]
            if len(__tuple) > 2: input = __tuple[2]
            if len(__tuple) > 3: showWindow = __tuple[3]
         self.impl = (
            (output     << lib.PIPEOPENMODE_output_SHIFT)     |
            (error      << lib.PIPEOPENMODE_error_SHIFT)      |
            (input      << lib.PIPEOPENMODE_input_SHIFT)      |
            (showWindow << lib.PIPEOPENMODE_showWindow_SHIFT) )

   @property
   def output(self): return ((((self.impl)) & lib.PIPEOPENMODE_output_MASK) >> lib.PIPEOPENMODE_output_SHIFT)
   @output.setter
   def output(self, value): self.impl = ((self.impl) & ~(lib.PIPEOPENMODE_output_MASK)) | (((value)) << lib.PIPEOPENMODE_output_SHIFT)

   @property
   def error(self): return ((((self.impl)) & lib.PIPEOPENMODE_error_MASK) >> lib.PIPEOPENMODE_error_SHIFT)
   @error.setter
   def error(self, value): self.impl = ((self.impl) & ~(lib.PIPEOPENMODE_error_MASK)) | (((value)) << lib.PIPEOPENMODE_error_SHIFT)

   @property
   def input(self): return ((((self.impl)) & lib.PIPEOPENMODE_input_MASK) >> lib.PIPEOPENMODE_input_SHIFT)
   @input.setter
   def input(self, value): self.impl = ((self.impl) & ~(lib.PIPEOPENMODE_input_MASK)) | (((value)) << lib.PIPEOPENMODE_input_SHIFT)

   @property
   def showWindow(self): return ((((self.impl)) & lib.PIPEOPENMODE_showWindow_MASK) >> lib.PIPEOPENMODE_showWindow_SHIFT)
   @showWindow.setter
   def showWindow(self, value): self.impl = ((self.impl) & ~(lib.PIPEOPENMODE_showWindow_MASK)) | (((value)) << lib.PIPEOPENMODE_showWindow_SHIFT)

class SysErrorCode(ErrorCode):
   allocationFailed = ErrorCode(impl = lib.SysErrorCode_allocationFailed)
   nameInexistant   = ErrorCode(impl = lib.SysErrorCode_nameInexistant)
   nameExists       = ErrorCode(impl = lib.SysErrorCode_nameExists)
   missingLibrary   = ErrorCode(impl = lib.SysErrorCode_missingLibrary)
   fileNotFound     = ErrorCode(impl = lib.SysErrorCode_fileNotFound)
   writeFailed      = ErrorCode(impl = lib.SysErrorCode_writeFailed)

class TempFile(File):
   class_members = [
                      'openMode',
                      'buffer',
                      'size',
                      'allocated',
                   ]

   def init_args(self, args, kwArgs): init_args(TempFile, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def openMode(self): return lib.TempFile_get_openMode(self.impl)
   @openMode.setter
   def openMode(self, value):
      lib.TempFile_set_openMode(self.impl, value)

   @property
   def buffer(self): return lib.TempFile_get_buffer(self.impl)
   @buffer.setter
   def buffer(self, value):
      lib.TempFile_set_buffer(self.impl, value)

   @property
   def size(self): return lib.TempFile_get_size(self.impl)
   @size.setter
   def size(self, value):
      lib.TempFile_set_size(self.impl, value)

   @property
   def allocated(self): return lib.TempFile_get_allocated(self.impl)
   @allocated.setter
   def allocated(self, value):
      lib.TempFile_set_allocated(self.impl, value)

   def stealBuffer(self):
      return lib.TempFile_stealBuffer(self.impl)

def archiveOpen(fileName, flags):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   if flags is not None and not isinstance(flags, ArchiveOpenFlags): flags = ArchiveOpenFlags(flags)
   if flags is None: flags = ffi.NULL
   return pyOrNewObject(Archive, lib.eC_archiveOpen(fileName, flags))

def archiveQuerySize(fileName):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   size = ffi.new("eC_FileSize *")
   r = lib.eC_archiveQuerySize(fileName, size)
   return r, FileSize(impl = size[0])

def changeWorkingDir(buf):
   if isinstance(buf, str): buf = ffi.new("char[]", buf.encode('u8'))
   elif buf is None: buf = ffi.NULL
   return lib.eC_changeWorkingDir(buf)

def copySystemPath(p):
   if isinstance(p, str): p = ffi.new("char[]", p.encode('u8'))
   elif p is None: p = ffi.NULL
   return lib.eC_copySystemPath(p)

def copyUnixPath(p):
   if isinstance(p, str): p = ffi.new("char[]", p.encode('u8'))
   elif p is None: p = ffi.NULL
   return lib.eC_copyUnixPath(p)

def createTemporaryDir(tempFileName, template):
   if isinstance(tempFileName, str): tempFileName = ffi.new("char[]", tempFileName.encode('u8'))
   elif tempFileName is None: tempFileName = ffi.NULL
   if isinstance(template, str): template = ffi.new("char[]", template.encode('u8'))
   elif template is None: template = ffi.NULL
   lib.eC_createTemporaryDir(tempFileName, template)

def createTemporaryFile(tempFileName, template):
   if isinstance(tempFileName, str): tempFileName = ffi.new("char[]", tempFileName.encode('u8'))
   elif tempFileName is None: tempFileName = ffi.NULL
   if isinstance(template, str): template = ffi.new("char[]", template.encode('u8'))
   elif template is None: template = ffi.NULL
   return pyOrNewObject(File, lib.eC_createTemporaryFile(tempFileName, template))

def deleteFile(fileName):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   return lib.eC_deleteFile(fileName)

def dualPipeOpen(mode, commandLine):
   if mode is not None and not isinstance(mode, PipeOpenMode): mode = PipeOpenMode(mode)
   if mode is None: mode = ffi.NULL
   if isinstance(commandLine, str): commandLine = ffi.new("char[]", commandLine.encode('u8'))
   elif commandLine is None: commandLine = ffi.NULL
   return pyOrNewObject(DualPipe, lib.eC_dualPipeOpen(mode, commandLine))

def dualPipeOpenEnv(mode, env, commandLine):
   if mode is not None and not isinstance(mode, PipeOpenMode): mode = PipeOpenMode(mode)
   if mode is None: mode = ffi.NULL
   if isinstance(env, str): env = ffi.new("char[]", env.encode('u8'))
   elif env is None: env = ffi.NULL
   if isinstance(commandLine, str): commandLine = ffi.new("char[]", commandLine.encode('u8'))
   elif commandLine is None: commandLine = ffi.NULL
   return pyOrNewObject(DualPipe, lib.eC_dualPipeOpenEnv(mode, env, commandLine))

def dualPipeOpenEnvf(mode, env, command, *args):
   if mode is not None and not isinstance(mode, PipeOpenMode): mode = PipeOpenMode(mode)
   if mode is None: mode = ffi.NULL
   if isinstance(env, str): env = ffi.new("char[]", env.encode('u8'))
   elif env is None: env = ffi.NULL
   if isinstance(command, str): command = ffi.new("char[]", command.encode('u8'))
   elif command is None: command = ffi.NULL
   return pyOrNewObject(DualPipe, lib.eC_dualPipeOpenEnvf(mode, env, command, *ellipsisArgs(args)))

def dualPipeOpenf(mode, command, *args):
   if mode is not None and not isinstance(mode, PipeOpenMode): mode = PipeOpenMode(mode)
   if mode is None: mode = ffi.NULL
   if isinstance(command, str): command = ffi.new("char[]", command.encode('u8'))
   elif command is None: command = ffi.NULL
   return pyOrNewObject(DualPipe, lib.eC_dualPipeOpenf(mode, command, *ellipsisArgs(args)))

def dumpErrors(display):
   lib.eC_dumpErrors(display)

def execute(command, *args):
   if isinstance(command, str): command = ffi.new("char[]", command.encode('u8'))
   elif command is None: command = ffi.NULL
   return lib.eC_execute(command, *ellipsisArgs(args))

def executeEnv(env, command, *args):
   if isinstance(env, str): env = ffi.new("char[]", env.encode('u8'))
   elif env is None: env = ffi.NULL
   if isinstance(command, str): command = ffi.new("char[]", command.encode('u8'))
   elif command is None: command = ffi.NULL
   return lib.eC_executeEnv(env, command, *ellipsisArgs(args))

def executeWait(command, *args):
   if isinstance(command, str): command = ffi.new("char[]", command.encode('u8'))
   elif command is None: command = ffi.NULL
   return lib.eC_executeWait(command, *ellipsisArgs(args))

def fileExists(fileName):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   return FileAttribs(impl = lib.eC_fileExists(fileName))

def fileFixCase(file):
   if isinstance(file, str): file = ffi.new("char[]", file.encode('u8'))
   elif file is None: file = ffi.NULL
   lib.eC_fileFixCase(file)

def fileGetSize(fileName):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   size = ffi.new("eC_FileSize *")
   r = lib.eC_fileGetSize(fileName, size)
   return r, FileSize(impl = size[0])

def fileGetStats(fileName, stats = None):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   if stats is not None and not isinstance(stats, FileStats): stats = FileStats(stats)
   stats = ffi.NULL if stats is None else stats.impl
   return lib.eC_fileGetStats(fileName, ffi.cast("eC_FileStats *", stats))

def fileOpen(fileName, mode):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   return pyOrNewObject(File, lib.eC_fileOpen(fileName, mode))

def fileOpenBuffered(fileName, mode):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   return pyOrNewObject(BufferedFile, lib.eC_fileOpenBuffered(fileName, mode))

def fileSetAttribs(fileName, attribs):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   if attribs is not None and not isinstance(attribs, FileAttribs): attribs = FileAttribs(attribs)
   if attribs is None: attribs = ffi.NULL
   return lib.eC_fileSetAttribs(fileName, attribs)

def fileSetTime(fileName, created, accessed, modified):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   if created is not None and not isinstance(created, SecSince1970): created = TimeStamp(created)
   if created is None: created = ffi.NULL
   if accessed is not None and not isinstance(accessed, SecSince1970): accessed = TimeStamp(accessed)
   if accessed is None: accessed = ffi.NULL
   if modified is not None and not isinstance(modified, SecSince1970): modified = TimeStamp(modified)
   if modified is None: modified = ffi.NULL
   return lib.eC_fileSetTime(fileName, created.impl, accessed.impl, modified.impl)

def fileTruncate(fileName, size):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   return lib.eC_fileTruncate(fileName, size)

def getEnvironment(envName, envValue, max):
   if isinstance(envName, str): envName = ffi.new("char[]", envName.encode('u8'))
   elif envName is None: envName = ffi.NULL
   if isinstance(envValue, str): envValue = ffi.new("char[]", envValue.encode('u8'))
   elif envValue is None: envValue = ffi.NULL
   return lib.eC_getEnvironment(envName, envValue, max)

def getFreeSpace(path):
   if isinstance(path, str): path = ffi.new("char[]", path.encode('u8'))
   elif path is None: path = ffi.NULL
   size = ffi.new("eC_FileSize64 *")
   lib.eC_getFreeSpace(path, size)
   return FileSize64(impl = size[0])

def getLastErrorCode():
   return lib.eC_getLastErrorCode()

def getSlashPathBuffer(d, p):
   if isinstance(d, str): d = ffi.new("char[]", d.encode('u8'))
   elif d is None: d = ffi.NULL
   if isinstance(p, str): p = ffi.new("char[]", p.encode('u8'))
   elif p is None: p = ffi.NULL
   return lib.eC_getSlashPathBuffer(d, p)

def getSystemPathBuffer(d, p):
   if isinstance(d, str): d = ffi.new("char[]", d.encode('u8'))
   elif d is None: d = ffi.NULL
   if isinstance(p, str): p = ffi.new("char[]", p.encode('u8'))
   elif p is None: p = ffi.NULL
   return lib.eC_getSystemPathBuffer(d, p)

def getWorkingDir(buf, size):
   if isinstance(buf, str): buf = ffi.new("char[]", buf.encode('u8'))
   elif buf is None: buf = ffi.NULL
   return lib.eC_getWorkingDir(buf, size)

def __e_log(text):
   if isinstance(text, str): text = ffi.new("char[]", text.encode('u8'))
   elif text is None: text = ffi.NULL
   lib.eC___e_log(text)

def logErrorCode(errorCode, details):
   if errorCode is not None and not isinstance(errorCode, ErrorCode): errorCode = ErrorCode(errorCode)
   if errorCode is None: errorCode = ffi.NULL
   if isinstance(details, str): details = ffi.new("char[]", details.encode('u8'))
   elif details is None: details = ffi.NULL
   lib.eC_logErrorCode(errorCode, details)

def __e_logf(format, *args):
   if isinstance(format, str): format = ffi.new("char[]", format.encode('u8'))
   elif format is None: format = ffi.NULL
   lib.eC___e_logf(format, *ellipsisArgs(args))

def makeDir(path):
   if isinstance(path, str): path = ffi.new("char[]", path.encode('u8'))
   elif path is None: path = ffi.NULL
   return lib.eC_makeDir(path)

def makeSlashPath(p):
   if isinstance(p, str): p = ffi.new("char[]", p.encode('u8'))
   elif p is None: p = ffi.NULL
   lib.eC_makeSlashPath(p)

def makeSystemPath(p):
   if isinstance(p, str): p = ffi.new("char[]", p.encode('u8'))
   elif p is None: p = ffi.NULL
   lib.eC_makeSystemPath(p)

def moveFile(source, dest):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   if isinstance(dest, str): dest = ffi.new("char[]", dest.encode('u8'))
   elif dest is None: dest = ffi.NULL
   return lib.eC_moveFile(source, dest)

def moveFileEx(source, dest, options):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   if isinstance(dest, str): dest = ffi.new("char[]", dest.encode('u8'))
   elif dest is None: dest = ffi.NULL
   if options is not None and not isinstance(options, MoveFileOptions): options = MoveFileOptions(options)
   if options is None: options = ffi.NULL
   return lib.eC_moveFileEx(source, dest, options)

def removeDir(path):
   if isinstance(path, str): path = ffi.new("char[]", path.encode('u8'))
   elif path is None: path = ffi.NULL
   return lib.eC_removeDir(path)

def renameFile(oldName, newName):
   if isinstance(oldName, str): oldName = ffi.new("char[]", oldName.encode('u8'))
   elif oldName is None: oldName = ffi.NULL
   if isinstance(newName, str): newName = ffi.new("char[]", newName.encode('u8'))
   elif newName is None: newName = ffi.NULL
   return lib.eC_renameFile(oldName, newName)

def resetError():
   lib.eC_resetError()

def setEnvironment(envName, envValue):
   if isinstance(envName, str): envName = ffi.new("char[]", envName.encode('u8'))
   elif envName is None: envName = ffi.NULL
   if isinstance(envValue, str): envValue = ffi.new("char[]", envValue.encode('u8'))
   elif envValue is None: envValue = ffi.NULL
   lib.eC_setEnvironment(envName, envValue)

def setErrorLevel(level):
   lib.eC_setErrorLevel(level)

def setLoggingMode(mode, where):
   if hasattr(where, 'impl'): where = where.impl
   if where is None: where = ffi.NULL
   lib.eC_setLoggingMode(mode, where)

def shellOpen(fileName, *args):
   if isinstance(fileName, str): fileName = ffi.new("char[]", fileName.encode('u8'))
   elif fileName is None: fileName = ffi.NULL
   return lib.eC_shellOpen(fileName, *ellipsisArgs(args))

def unsetEnvironment(envName):
   if isinstance(envName, str): envName = ffi.new("char[]", envName.encode('u8'))
   elif envName is None: envName = ffi.NULL
   lib.eC_unsetEnvironment(envName)

def debugBreakpoint():
   lib.eC_debugBreakpoint()

class FieldType:
   integer = lib.FieldType_integer
   real    = lib.FieldType_real
   text    = lib.FieldType_text
   blob    = lib.FieldType_blob
   nil     = lib.FieldType_nil
   array   = lib.FieldType_array
   map     = lib.FieldType_map

class FieldTypeEx:
   def __init__(self, type = 0, mustFree = False, format = 0, isUnsigned = False, isDateTime = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(type, FieldTypeEx):
         self.impl = type.impl
      else:
         if isinstance(type, tuple):
            __tuple = type
            type = 0
            if len(__tuple) > 0: type = __tuple[0]
            if len(__tuple) > 1: mustFree = __tuple[1]
            if len(__tuple) > 2: format = __tuple[2]
            if len(__tuple) > 3: isUnsigned = __tuple[3]
            if len(__tuple) > 4: isDateTime = __tuple[4]
         self.impl = (
            (type       << lib.FIELDTYPEEX_type_SHIFT)       |
            (mustFree   << lib.FIELDTYPEEX_mustFree_SHIFT)   |
            (format     << lib.FIELDTYPEEX_format_SHIFT)     |
            (isUnsigned << lib.FIELDTYPEEX_isUnsigned_SHIFT) |
            (isDateTime << lib.FIELDTYPEEX_isDateTime_SHIFT) )

   @property
   def type(self): return ((((self.impl)) & lib.FIELDTYPEEX_type_MASK) >> lib.FIELDTYPEEX_type_SHIFT)
   @type.setter
   def type(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_type_MASK)) | (((value)) << lib.FIELDTYPEEX_type_SHIFT)

   @property
   def mustFree(self): return ((((self.impl)) & lib.FIELDTYPEEX_mustFree_MASK) >> lib.FIELDTYPEEX_mustFree_SHIFT)
   @mustFree.setter
   def mustFree(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_mustFree_MASK)) | (((value)) << lib.FIELDTYPEEX_mustFree_SHIFT)

   @property
   def format(self): return ((((self.impl)) & lib.FIELDTYPEEX_format_MASK) >> lib.FIELDTYPEEX_format_SHIFT)
   @format.setter
   def format(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_format_MASK)) | (((value)) << lib.FIELDTYPEEX_format_SHIFT)

   @property
   def isUnsigned(self): return ((((self.impl)) & lib.FIELDTYPEEX_isUnsigned_MASK) >> lib.FIELDTYPEEX_isUnsigned_SHIFT)
   @isUnsigned.setter
   def isUnsigned(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_isUnsigned_MASK)) | (((value)) << lib.FIELDTYPEEX_isUnsigned_SHIFT)

   @property
   def isDateTime(self): return ((((self.impl)) & lib.FIELDTYPEEX_isDateTime_MASK) >> lib.FIELDTYPEEX_isDateTime_SHIFT)
   @isDateTime.setter
   def isDateTime(self, value): self.impl = ((self.impl) & ~(lib.FIELDTYPEEX_isDateTime_MASK)) | (((value)) << lib.FIELDTYPEEX_isDateTime_SHIFT)

class FieldValue(Struct):
   def __init__(self, type = None, i = None, r = None, s = None, b = None, a = None, m = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_FieldValue *", impl)
      else:
         if type is not None:
            if not isinstance(type, FieldTypeEx): type = FieldTypeEx(type)
         if s is not None:
            if not isinstance(s, String): s = String(s)
         __members = { }
         if type is not None: __members['type'] = type.impl
         if i is not None:    __members['i']    = i
         if r is not None:    __members['r']    = r
         if s is not None:    __members['s']    = s
         if b is not None:    __members['b']    = b
         if a is not None:    __members['a']    = a.impl
         if m is not None:    __members['m']    = m.impl
         self.impl = ffi.new("eC_FieldValue *", __members)

   @property
   def type(self): return FieldTypeEx(impl = self.impl.type)
   @type.setter
   def type(self, value):
      if not isinstance(value, FieldTypeEx): value = FieldTypeEx(value)
      self.impl.type = value.impl

   @property
   def i(self): return self.impl.i
   @i.setter
   def i(self, value): self.impl.i = value

   @property
   def r(self): return self.impl.r
   @r.setter
   def r(self, value): self.impl.r = value

   @property
   def s(self): return String(self.impl.s)
   @s.setter
   def s(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.s = value

   @property
   def b(self): return self.impl.b
   @b.setter
   def b(self, value): self.impl.b = value

   @property
   def a(self): return pyOrNewObject(Array, self.impl.a)
   @a.setter
   def a(self, value): self.impl.a = value.impl

   @property
   def m(self): return pyOrNewObject(Map, self.impl.m)
   @m.setter
   def m(self, value): self.impl.m = value.impl

   def compareInt(self, other = None):
      if other is not None and not isinstance(other, FieldValue): other = FieldValue(other)
      other = ffi.NULL if other is None else other.impl
      return lib.FieldValue_compareInt(ffi.cast("eC_FieldValue *", self.impl), ffi.cast("eC_FieldValue *", other))

   def compareReal(self, other = None):
      if other is not None and not isinstance(other, FieldValue): other = FieldValue(other)
      other = ffi.NULL if other is None else other.impl
      return lib.FieldValue_compareReal(ffi.cast("eC_FieldValue *", self.impl), ffi.cast("eC_FieldValue *", other))

   def compareText(self, other = None):
      if other is not None and not isinstance(other, FieldValue): other = FieldValue(other)
      other = ffi.NULL if other is None else other.impl
      return lib.FieldValue_compareText(ffi.cast("eC_FieldValue *", self.impl), ffi.cast("eC_FieldValue *", other))

   def formatArray(self, tempString, fieldData, onType):
      if isinstance(tempString, str): tempString = ffi.new("char[]", tempString.encode('u8'))
      elif tempString is None: tempString = ffi.NULL
      if hasattr(fieldData, 'impl'): fieldData = fieldData.impl
      if fieldData is None: fieldData = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatArray(self.impl, tempString, fieldData, onType))

   def formatFloat(self, stringOutput, fixDot):
      if isinstance(stringOutput, str): stringOutput = ffi.new("char[]", stringOutput.encode('u8'))
      elif stringOutput is None: stringOutput = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatFloat(self.impl, stringOutput, fixDot))

   def formatInteger(self, stringOutput):
      if isinstance(stringOutput, str): stringOutput = ffi.new("char[]", stringOutput.encode('u8'))
      elif stringOutput is None: stringOutput = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatInteger(self.impl, stringOutput))

   def formatMap(self, tempString, fieldData, onType):
      if isinstance(tempString, str): tempString = ffi.new("char[]", tempString.encode('u8'))
      elif tempString is None: tempString = ffi.NULL
      if hasattr(fieldData, 'impl'): fieldData = fieldData.impl
      if fieldData is None: fieldData = ffi.NULL
      return pyOrNewObject(String, lib.FieldValue_formatMap(self.impl, tempString, fieldData, onType))

   def getArrayOrMap(string, destClass = None):
      if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
      elif string is None: string = ffi.NULL
      if destClass is not None and not isinstance(destClass, Class): destClass = Class(destClass)
      destClass = ffi.NULL if destClass is None else destClass.impl
      destination = ffi.new("void * *")
      r = lib.FieldValue_getArrayOrMap(string, ffi.cast("struct eC_Class *", destClass), destination)
      if destination[0] == ffi.NULL: _destination = None
      else:
         if destClass.type == ClassType.normalClass:
            i = ffi.cast("eC_Instance", destination[0])
            n = ffi.string(i._class.name).decode('u8')
         else:
            n = ffi.string(destClass.name).decode('u8')
         t = pyTypeByName(n)
         ct = n + " * " if destClass.type == ClassType.noHeadClass else n
         _destination = t(impl = pyFFI().cast(ct, destination[0]))
      return r, _destination

   def stringify(self):
      return pyOrNewObject(String, lib.FieldValue_stringify(self.impl))

class FieldValueFormat:
   decimal     = lib.FieldValueFormat_decimal
   unset       = lib.FieldValueFormat_unset
   hex         = lib.FieldValueFormat_hex
   octal       = lib.FieldValueFormat_octal
   binary      = lib.FieldValueFormat_binary
   exponential = lib.FieldValueFormat_exponential
   boolean     = lib.FieldValueFormat_boolean
   textObj     = lib.FieldValueFormat_textObj
   color       = lib.FieldValueFormat_color

class IteratorPointer:
   def __init__(self, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_IteratorPointer *", lib.Instance_new(lib.class_IteratorPointer))

class AVLNode(IteratorPointer):
   def __init__(self, key = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_AVLNode *", lib.Instance_new(lib.class_AVLNode))
         if key is not None: self.key = key

   @property
   def key(self): return self.impl.key
   @key.setter
   def key(self, value): self.impl.key = value

#    @property
#    def prev(self): 

#    @property
#    def next(self): 

#    @property
#    def minimum(self): 

#    @property
#    def maximum(self): 

   @property
   def count(self): return lib.AVLNode_get_count(ffi.cast("struct eC_AVLNode *", self.impl))

   @property
   def depthProp(self): return lib.AVLNode_get_depthProp(ffi.cast("struct eC_AVLNode *", self.impl))

   def find(self, Tclass, key):
      if Tclass is not None and not isinstance(Tclass, Class): Tclass = Class(Tclass)
      Tclass = ffi.NULL if Tclass is None else Tclass.impl
      return lib.AVLNode_find(ffi.cast("struct eC_AVLNode *", self.impl), ffi.cast("struct eC_Class *", Tclass), TA(key))

class BinaryTree(Struct):
   def __init__(self, root = None, count = 0, CompareKey = None, FreeKey = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_BinaryTree *", impl)
      else:
         if isinstance(root, tuple):
            __tuple = root
            root = None
            if len(__tuple) > 0: root       = __tuple[0]
            if len(__tuple) > 1: count      = __tuple[1]
            if len(__tuple) > 2: CompareKey = __tuple[2]
            if len(__tuple) > 3: FreeKey    = __tuple[3]
         if root is not None:
            if not isinstance(root, BTNode): root = BTNode(root)
            root = root.impl
         else:
            root = ffi.NULL
         self.impl = ffi.new("eC_BinaryTree *", { 'root' : root, 'count' : count, 'CompareKey' : CompareKey, 'FreeKey' : FreeKey })

   @property
   def root(self): return self.impl.root
   @root.setter
   def root(self, value):
      if not isinstance(value, BTNode): value = BTNode(value)
      self.impl.root = value.impl

   @property
   def count(self): return self.impl.count
   @count.setter
   def count(self, value): self.impl.count = value

   @property
   def CompareKey(self): return self.impl.CompareKey
   @CompareKey.setter
   def CompareKey(self, value): self.impl.CompareKey = value

   @property
   def FreeKey(self): return self.impl.FreeKey
   @FreeKey.setter
   def FreeKey(self, value): self.impl.FreeKey = value

   @property
   def first(self): return BTNode(impl = lib.BinaryTree_get_first(self.impl))

   @property
   def last(self): return BTNode(impl = lib.BinaryTree_get_last(self.impl))

   def add(self, node = None):
      if node is not None and not isinstance(node, BTNode): node = BTNode(node)
      node = ffi.NULL if node is None else node.impl
      return lib.BinaryTree_add(ffi.cast("eC_BinaryTree *", self.impl), ffi.cast("struct eC_BTNode *", node))

   def check(self):
      return lib.BinaryTree_check(ffi.cast("eC_BinaryTree *", self.impl))

   def compareInt(self, a, b):
      return lib.BinaryTree_compareInt(ffi.cast("eC_BinaryTree *", self.impl), a, b)

   def compareString(self, a, b):
      if isinstance(a, str): a = ffi.new("char[]", a.encode('u8'))
      elif a is None: a = ffi.NULL
      if isinstance(b, str): b = ffi.new("char[]", b.encode('u8'))
      elif b is None: b = ffi.NULL
      return lib.BinaryTree_compareString(ffi.cast("eC_BinaryTree *", self.impl), a, b)

   def delete(self, node = None):
      if node is not None and not isinstance(node, BTNode): node = BTNode(node)
      node = ffi.NULL if node is None else node.impl
      lib.BinaryTree_delete(ffi.cast("eC_BinaryTree *", self.impl), ffi.cast("struct eC_BTNode *", node))

   def find(self, key):
      return lib.BinaryTree_find(ffi.cast("eC_BinaryTree *", self.impl), key)

   def findAll(self, key):
      return lib.BinaryTree_findAll(ffi.cast("eC_BinaryTree *", self.impl), key)

   def findPrefix(self, key):
      if isinstance(key, str): key = ffi.new("char[]", key.encode('u8'))
      elif key is None: key = ffi.NULL
      return lib.BinaryTree_findPrefix(ffi.cast("eC_BinaryTree *", self.impl), key)

   def findString(self, key):
      if isinstance(key, str): key = ffi.new("char[]", key.encode('u8'))
      elif key is None: key = ffi.NULL
      return lib.BinaryTree_findString(ffi.cast("eC_BinaryTree *", self.impl), key)

   def free(self):
      lib.BinaryTree_free(ffi.cast("eC_BinaryTree *", self.impl))

   def freeString(string):
      if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
      elif string is None: string = ffi.NULL
      lib.BinaryTree_freeString(string)

   def _print(self, output, tps):
      if isinstance(output, str): output = ffi.new("char[]", output.encode('u8'))
      elif output is None: output = ffi.NULL
      return lib.BinaryTree_print(ffi.cast("eC_BinaryTree *", self.impl), output, tps)

   def remove(self, node = None):
      if node is not None and not isinstance(node, BTNode): node = BTNode(node)
      node = ffi.NULL if node is None else node.impl
      lib.BinaryTree_remove(ffi.cast("eC_BinaryTree *", self.impl), ffi.cast("struct eC_BTNode *", node))

class CustomAVLTree(Container):
   class_members = [
                      'root',
                      'count',
                   ]

   def init_args(self, args, kwArgs): init_args(CustomAVLTree, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<BT, I = KT>"
      self.init_args(list(args), kwArgs)

   @property
   def root(self): return IPTR(lib, ffi, self, CustomAVLTree).root
   @root.setter
   def root(self, value): IPTR(lib, ffi, self, CustomAVLTree).root = value

   @property
   def count(self): return IPTR(lib, ffi, self, CustomAVLTree).count
   @count.setter
   def count(self, value): IPTR(lib, ffi, self, CustomAVLTree).count = value

   def check(self):
      return lib.CustomAVLTree_check(self.impl)

   def freeKey(self, item = None):
      lib.CustomAVLTree_freeKey(self.impl, AVLNode)

class Iterator(Struct):
   def __init__(self, container = None, pointer = None, data = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_Iterator *", impl)
      else:
         if isinstance(container, tuple):
            __tuple = container
            container = None
            if len(__tuple) > 0: container = __tuple[0]
            if len(__tuple) > 1: pointer   = __tuple[1]
            if len(__tuple) > 2: data      = __tuple[2]
         if pointer is not None:
            if not isinstance(pointer, IteratorPointer): pointer = IteratorPointer(pointer)
            pointer = pointer.impl
         else:
            pointer = ffi.NULL
         self.impl = ffi.new("eC_Iterator *", { 'container' : container, 'pointer' : pointer })
         if data is not None:      self.data           = data

   @property
   def container(self): return pyOrNewObject(Container, self.impl.container)
   @container.setter
   def container(self, value): self.impl.container = value.impl

   @property
   def pointer(self): return self.impl.pointer
   @pointer.setter
   def pointer(self, value):
      if not isinstance(value, IteratorPointer): value = IteratorPointer(value)
      self.impl.pointer = value.impl

   @property
   def data(self): value = lib.Iterator_get_data(self.impl); return pyOrNewObject(Instance, lib.oTAInstance(value))
   @data.setter
   def data(self, value):
      lib.Iterator_set_data(self.impl, TA(value))

   def find(self, value):
      return lib.Iterator_find(ffi.cast("eC_Iterator *", self.impl), TA(value))

   def free(self):
      lib.Iterator_free(ffi.cast("eC_Iterator *", self.impl))

   def getData(self):
      return lib.Iterator_getData(ffi.cast("eC_Iterator *", self.impl))

   def index(self, index, create):
      return lib.Iterator_index(ffi.cast("eC_Iterator *", self.impl), TA(index), create)

   def next(self):
      return lib.Iterator_next(ffi.cast("eC_Iterator *", self.impl))

   def prev(self):
      return lib.Iterator_prev(ffi.cast("eC_Iterator *", self.impl))

   def remove(self):
      lib.Iterator_remove(ffi.cast("eC_Iterator *", self.impl))

   def setData(self, value):
      return lib.Iterator_setData(ffi.cast("eC_Iterator *", self.impl), TA(value))

class LinkList(Container):
   class_members = [
                      'first',
                      'last',
                      'count',
                   ]

   def init_args(self, args, kwArgs): init_args(LinkList, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<LT>"
      self.init_args(list(args), kwArgs)

   @property
   def first(self): return IPTR(lib, ffi, self, LinkList).first
   @first.setter
   def first(self, value): IPTR(lib, ffi, self, LinkList).first = value

   @property
   def last(self): return IPTR(lib, ffi, self, LinkList).last
   @last.setter
   def last(self, value): IPTR(lib, ffi, self, LinkList).last = value

   @property
   def count(self): return IPTR(lib, ffi, self, LinkList).count
   @count.setter
   def count(self, value): IPTR(lib, ffi, self, LinkList).count = value

   def _Sort(self, ascending, lists):
      lib.LinkList__Sort(self.impl, ascending, lists)

class ListItem(IteratorPointer):
   def __init__(self, link = None, prev = None, next = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_ListItem *", lib.Instance_new(lib.class_ListItem))
         if isinstance(link, tuple):
            __tuple = link
            link = None
            if len(__tuple) > 0: link = __tuple[0]
         if link is not None: self.link = link
         if prev is not None: self.prev = prev
         if next is not None: self.next = next

   @property
   def link(self): return LinkElement(impl = self.impl.link)
   @link.setter
   def link(self, value): self.impl.link = value.impl[0]

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value): self.impl.prev = value

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value): self.impl.next = value

class AVLTree(CustomAVLTree):
   class_members = []

   def init_args(self, args, kwArgs): init_args(AVLTree, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<BT = AVLNode<AT>, KT = AT, T = AT, D = AT>"
      self.init_args(list(args), kwArgs)

class Array(Container):
   # hack: hardcoded content
   def __init__(self, templateParams = None, copySrc = None, impl = None):
      Container.__init__(self, templateParams, copySrc, impl=impl)
   # hack: end of hardcoded content

   @property
   def array(self): return IPTR(lib, ffi, self, Array).array
   @array.setter
   def array(self, value): IPTR(lib, ffi, self, Array).array = value

   @property
   def count(self): return IPTR(lib, ffi, self, Array).count
   @count.setter
   def count(self, value): IPTR(lib, ffi, self, Array).count = value

   @property
   def size(self): return lib.Array_get_size(self.impl)
   @size.setter
   def size(self, value):
      lib.Array_set_size(self.impl, value)

   @property
   def minAllocSize(self): return lib.Array_get_minAllocSize(self.impl)
   @minAllocSize.setter
   def minAllocSize(self, value):
      lib.Array_set_minAllocSize(self.impl, value)

class BTNode:
   def __init__(self, key = None, parent = None, left = None, right = None, depth = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_BTNode *", lib.Instance_new(lib.class_BTNode))
         if isinstance(key, tuple):
            __tuple = key
            key = None
            if len(__tuple) > 0: key    = __tuple[0]
            if len(__tuple) > 1: parent = __tuple[1]
            if len(__tuple) > 2: left   = __tuple[2]
            if len(__tuple) > 3: right  = __tuple[3]
         if key is not None:    self.key    = key
         if parent is not None: self.parent = parent
         if left is not None:   self.left   = left
         if right is not None:  self.right  = right
         if depth is not None:  self.depth  = depth

   @property
   def key(self): return self.impl.key
   @key.setter
   def key(self, value): self.impl.key = value

   @property
   def parent(self): return self.impl.parent
   @parent.setter
   def parent(self, value):
      if not isinstance(value, BTNode): value = BTNode(value)
      self.impl.parent = value.impl

   @property
   def left(self): return self.impl.left
   @left.setter
   def left(self, value):
      if not isinstance(value, BTNode): value = BTNode(value)
      self.impl.left = value.impl

   @property
   def right(self): return self.impl.right
   @right.setter
   def right(self, value):
      if not isinstance(value, BTNode): value = BTNode(value)
      self.impl.right = value.impl

   @property
   def depth(self): return self.impl.depth
   @depth.setter
   def depth(self, value): self.impl.depth = value

   @property
   def prev(self): return BTNode(impl = lib.BTNode_get_prev(ffi.cast("struct eC_BTNode *", self.impl)))

   @property
   def next(self): return BTNode(impl = lib.BTNode_get_next(ffi.cast("struct eC_BTNode *", self.impl)))

   @property
   def minimum(self): return BTNode(impl = lib.BTNode_get_minimum(ffi.cast("struct eC_BTNode *", self.impl)))

   @property
   def maximum(self): return BTNode(impl = lib.BTNode_get_maximum(ffi.cast("struct eC_BTNode *", self.impl)))

   @property
   def count(self): return lib.BTNode_get_count(ffi.cast("struct eC_BTNode *", self.impl))

   @property
   def depthProp(self): return lib.BTNode_get_depthProp(ffi.cast("struct eC_BTNode *", self.impl))

   def findPrefix(self, key):
      if isinstance(key, str): key = ffi.new("char[]", key.encode('u8'))
      elif key is None: key = ffi.NULL
      return lib.BTNode_findPrefix(ffi.cast("struct eC_BTNode *", self.impl), key)

   def findString(self, key):
      if isinstance(key, str): key = ffi.new("char[]", key.encode('u8'))
      elif key is None: key = ffi.NULL
      return lib.BTNode_findString(ffi.cast("struct eC_BTNode *", self.impl), key)

class HashMap(Container):
   class_members = [
                      'noRemResize',
                      'count',
                      'initSize',
                   ]

   def init_args(self, args, kwArgs): init_args(HashMap, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<VT, I = KT>"
      self.init_args(list(args), kwArgs)

   @property
   def noRemResize(self): return IPTR(lib, ffi, self, HashMap).noRemResize
   @noRemResize.setter
   def noRemResize(self, value): IPTR(lib, ffi, self, HashMap).noRemResize = value

   @property
   def count(self): return lib.HashMap_get_count(self.impl)

   @property
   def initSize(self): return None
   @initSize.setter
   def initSize(self, value):
      lib.HashMap_set_initSize(self.impl, value)

   def removeIterating(self, it):
      lib.HashMap_removeIterating(self.impl, it)

   def resize(self, movedEntry):
      lib.HashMap_resize(self.impl, movedEntry)

class HashMapIterator(Iterator):
   def __init__(self, container = None, pointer = None, data = None, map = None, value = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_HashMapIterator *", impl)
      else:
         if isinstance(container, tuple):
            __tuple = container
            container = None
            if len(__tuple) > 0: container = __tuple[0]
            if len(__tuple) > 1: pointer   = __tuple[1]
            if len(__tuple) > 2: data      = __tuple[2]
            if len(__tuple) > 3: map       = __tuple[3]
         if pointer is not None:
            if not isinstance(pointer, IteratorPointer): pointer = IteratorPointer(pointer)
            pointer = pointer.impl
         else:
            pointer = ffi.NULL
         self.impl = ffi.new("eC_HashMapIterator *", { 'container' : container, 'pointer' : pointer })
         if data is not None:      self.data           = data
         if map is not None:       self.map            = map
         if value is not None:     self.value          = value

   @property
   def map(self): return pyOrNewObject(HashMap, lib.HashMapIterator_get_map(self.impl))
   @map.setter
   def map(self, value):
      lib.HashMapIterator_set_map(self.impl, value.impl)

   @property
   def key(self): value = lib.HashMapIterator_get_key(self.impl); return pyOrNewObject(Instance, lib.oTAInstance(value))

   @property
   def value(self): value = lib.HashMapIterator_get_value(self.impl); return pyOrNewObject(Instance, lib.oTAInstance(value))
   @value.setter
   def value(self, value):
      lib.HashMapIterator_set_value(self.impl, TA(value))

class HashTable(Container):
   class_members = [
                      'initSize',
                   ]

   def init_args(self, args, kwArgs): init_args(HashTable, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<KT, I = KT>"
      self.init_args(list(args), kwArgs)

   @property
   def initSize(self): return None
   @initSize.setter
   def initSize(self, value):
      lib.HashTable_set_initSize(self.impl, value)

class Item:
   def __init__(self, prev = None, next = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_Item *", lib.Instance_new(lib.class_Item))
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev = __tuple[0]
            if len(__tuple) > 1: next = __tuple[1]
         if prev is not None: self.prev = prev
         if next is not None: self.next = next

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, Item): value = Item(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, Item): value = Item(value)
      self.impl.next = value.impl

   def copy(self, src, size):
      if src is not None and not isinstance(src, Item): src = Item(src)
      src = ffi.NULL if src is None else src.impl
      lib.Item_copy(ffi.cast("struct eC_Item *", self.impl), ffi.cast("struct eC_Item *", src), size)

class Link(ListItem):
   def __init__(self, link = None, data = None, prev = None, next = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_Link *", lib.Instance_new(lib.class_Link))
         if isinstance(link, tuple):
            __tuple = link
            link = None
            if len(__tuple) > 0: link = __tuple[0]
            if len(__tuple) > 1: data = __tuple[1]
         if link is not None: self.link = link
         if data is not None: self.data = data
         if prev is not None: self.prev = prev
         if next is not None: self.next = next

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

class LinkElement(Struct):
   def __init__(self, prev = None, next = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_LinkElement *", impl)
      else:
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev = __tuple[0]
            if len(__tuple) > 1: next = __tuple[1]
         self.impl = ffi.new("eC_LinkElement *", { 'prev' : prev, 'next' : next })

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value): self.impl.prev = value

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value): self.impl.next = value

class List(LinkList):
   class_members = []

   def init_args(self, args, kwArgs): init_args(List, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<Link, T = LLT, D = LLT>"
      self.init_args(list(args), kwArgs)

class Map(CustomAVLTree):
   class_members = [
                      'mapSrc',
                   ]

   def init_args(self, args, kwArgs): init_args(Map, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<MapNode<MT, V>, I = MT, D = V, KT = MT>"
      self.init_args(list(args), kwArgs)

   @property
   def mapSrc(self): return None
   @mapSrc.setter
   def mapSrc(self, value):
      if not isinstance(value, Map): value = Map(value)
      lib.Map_set_mapSrc(self.impl, value.impl)

class MapIterator(Iterator):
   def __init__(self, container = None, pointer = None, data = None, map = None, value = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_MapIterator *", impl)
      else:
         if isinstance(container, tuple):
            __tuple = container
            container = None
            if len(__tuple) > 0: map = __tuple[0]
            if len(__tuple) > 1: pointer   = __tuple[1]
         if pointer is not None:
            if not isinstance(pointer, IteratorPointer): pointer = IteratorPointer(pointer)
            pointer = pointer.impl
         else:
            pointer = ffi.NULL
         if map is not None:
            if not isinstance(map, Map): map = map.impl
            map = map.impl
         else:
            map = ffi.NULL
         self.impl = ffi.new("eC_MapIterator *", { 'container' : map, 'pointer' : pointer })

   @property
   def map(self): return pyOrNewObject(Map, lib.MapIterator_get_map(self.impl))
   @map.setter
   def map(self, value):
      lib.MapIterator_set_map(self.impl, value.impl)

   @property
   def key(self):
      k = lib.MapIterator_get_key(self.impl);
      kc = self.map.impl._class.templateArgs[1].dataTypeClass
      return OTA(kc, k)

   @property
   def value(self):
      v = lib.MapIterator_get_value(self.impl);
      kc = self.map.impl._class.templateArgs[2].dataTypeClass
      return OTA(kc, v)
   @value.setter
   def value(self, value):
      lib.MapIterator_set_value(self.impl, TA(value))

class MapNode(AVLNode):
   def __init__(self, key = None, value = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_MapNode *", lib.Instance_new(lib.class_MapNode))
         if isinstance(key, tuple):
            __tuple = key
            key = None
            if len(__tuple) > 0: key   = __tuple[0]
            if len(__tuple) > 1: value = __tuple[1]
         if key is not None:   self.key        = key
         if value is not None: self.value      = value
         if key is not None:   self.key   = key
         if value is not None: self.value = value

   @property
   def key(self): value = lib.MapNode_get_key(ffi.cast("struct eC_MapNode *", self.impl)); return pyOrNewObject(Instance, lib.oTAInstance(value))
   @key.setter
   def key(self, value):
      lib.MapNode_set_key(ffi.cast("struct eC_MapNode *", self.impl), TA(value))

   @property
   def value(self): value = lib.MapNode_get_value(ffi.cast("struct eC_MapNode *", self.impl)); return pyOrNewObject(Instance, lib.oTAInstance(value))
   @value.setter
   def value(self, value):
      lib.MapNode_set_value(ffi.cast("struct eC_MapNode *", self.impl), TA(value))

#    @property
#    def prev(self): 

#    @property
#    def next(self): 

#    @property
#    def minimum(self): 

#    @property
#    def maximum(self): 

class NamedItem:
   def __init__(self, prev = None, next = None, name = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_NamedItem *", lib.Instance_new(lib.class_NamedItem))
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev = __tuple[0]
            if len(__tuple) > 1: next = __tuple[1]
            if len(__tuple) > 2: name = __tuple[2]
         if prev is not None: self.prev = prev
         if next is not None: self.next = next
         if name is not None: self.name = name

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, NamedItem): value = NamedItem(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, NamedItem): value = NamedItem(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

class NamedLink:
   def __init__(self, prev = None, next = None, name = None, data = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_NamedLink *", lib.Instance_new(lib.class_NamedLink))
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev = __tuple[0]
            if len(__tuple) > 1: next = __tuple[1]
            if len(__tuple) > 2: name = __tuple[2]
            if len(__tuple) > 3: data = __tuple[3]
         if prev is not None: self.prev = prev
         if next is not None: self.next = next
         if name is not None: self.name = name
         if data is not None: self.data = data

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, NamedLink): value = NamedLink(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, NamedLink): value = NamedLink(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

class NamedLink64:
   def __init__(self, prev = None, next = None, name = None, data = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_NamedLink64 *", lib.Instance_new(lib.class_NamedLink64))
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev = __tuple[0]
            if len(__tuple) > 1: next = __tuple[1]
            if len(__tuple) > 2: name = __tuple[2]
            if len(__tuple) > 3: data = __tuple[3]
         if prev is not None: self.prev = prev
         if next is not None: self.next = next
         if name is not None: self.name = name
         if data is not None: self.data = data

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, NamedLink64): value = NamedLink64(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, NamedLink64): value = NamedLink64(value)
      self.impl.next = value.impl

   @property
   def name(self): return self.impl.name
   @name.setter
   def name(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.name = value

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

class OldLink:
   def __init__(self, prev = None, next = None, data = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_OldLink *", lib.Instance_new(lib.class_OldLink))
         if isinstance(prev, tuple):
            __tuple = prev
            prev = None
            if len(__tuple) > 0: prev = __tuple[0]
            if len(__tuple) > 1: next = __tuple[1]
            if len(__tuple) > 2: data = __tuple[2]
         if prev is not None: self.prev = prev
         if next is not None: self.next = next
         if data is not None: self.data = data

   @property
   def prev(self): return self.impl.prev
   @prev.setter
   def prev(self, value):
      if not isinstance(value, OldLink): value = OldLink(value)
      self.impl.prev = value.impl

   @property
   def next(self): return self.impl.next
   @next.setter
   def next(self, value):
      if not isinstance(value, OldLink): value = OldLink(value)
      self.impl.next = value.impl

   @property
   def data(self): return self.impl.data
   @data.setter
   def data(self, value): self.impl.data = value

   def free(self):
      lib.OldLink_free(ffi.cast("struct eC_OldLink *", self.impl))

class OldList(Struct):
   def __init__(self, first = None, last = None, count = 0, offset = 0, circ = False, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_OldList *", impl)
      else:
         if isinstance(first, tuple):
            __tuple = first
            first = None
            if len(__tuple) > 0: first  = __tuple[0]
            if len(__tuple) > 1: last   = __tuple[1]
            if len(__tuple) > 2: count  = __tuple[2]
            if len(__tuple) > 3: offset = __tuple[3]
         self.impl = ffi.new("eC_OldList *", {
                                'first' : first,
                                'last' : last,
                                'count' : count,
                                'offset' : offset,
                                'circ' : circ
                             })

   @property
   def first(self): return self.impl.first
   @first.setter
   def first(self, value): self.impl.first = value

   @property
   def last(self): return self.impl.last
   @last.setter
   def last(self, value): self.impl.last = value

   @property
   def count(self): return self.impl.count
   @count.setter
   def count(self, value): self.impl.count = value

   @property
   def offset(self): return self.impl.offset
   @offset.setter
   def offset(self, value): self.impl.offset = value

   @property
   def circ(self): return self.impl.circ
   @circ.setter
   def circ(self, value): self.impl.circ = value

   def add(self, item):
      if hasattr(item, 'impl'): item = item.impl
      if item is None: item = ffi.NULL
      lib.OldList_add(ffi.cast("eC_OldList *", self.impl), item)

   def addName(self, item):
      if hasattr(item, 'impl'): item = item.impl
      if item is None: item = ffi.NULL
      return lib.OldList_addName(ffi.cast("eC_OldList *", self.impl), item)

   def clear(self):
      lib.OldList_clear(ffi.cast("eC_OldList *", self.impl))

   def copy(self, src, size, copy):
      if src is not None and not isinstance(src, OldList): src = OldList(src)
      src = ffi.NULL if src is None else src.impl
      lib.OldList_copy(ffi.cast("eC_OldList *", self.impl), ffi.cast("eC_OldList *", src), size)

   def delete(self, item):
      if hasattr(item, 'impl'): item = item.impl
      if item is None: item = ffi.NULL
      lib.OldList_delete(ffi.cast("eC_OldList *", self.impl), item)

   def findLink(self, data):
      if hasattr(data, 'impl'): data = data.impl
      if data is None: data = ffi.NULL
      return lib.OldList_findLink(ffi.cast("eC_OldList *", self.impl), data)

   def findName(self, name, warn):
      if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
      elif name is None: name = ffi.NULL
      return lib.OldList_findName(ffi.cast("eC_OldList *", self.impl), name, warn)

   def findNamedLink(self, name, warn):
      if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
      elif name is None: name = ffi.NULL
      return lib.OldList_findNamedLink(ffi.cast("eC_OldList *", self.impl), name, warn)

   def free(self, freeFn):
      lib.OldList_free(ffi.cast("eC_OldList *", self.impl))

   def insert(self, prevItem, item):
      if hasattr(prevItem, 'impl'): prevItem = prevItem.impl
      if prevItem is None: prevItem = ffi.NULL
      if hasattr(item, 'impl'): item = item.impl
      if item is None: item = ffi.NULL
      return lib.OldList_insert(ffi.cast("eC_OldList *", self.impl), prevItem, item)

   def move(self, item, prevItem):
      if hasattr(item, 'impl'): item = item.impl
      if item is None: item = ffi.NULL
      if hasattr(prevItem, 'impl'): prevItem = prevItem.impl
      if prevItem is None: prevItem = ffi.NULL
      lib.OldList_move(ffi.cast("eC_OldList *", self.impl), item, prevItem)

   def placeName(self, name):
      if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
      elif name is None: name = ffi.NULL
      place = ffi.new("void * *")
      r = lib.OldList_placeName(ffi.cast("eC_OldList *", self.impl), name, place)
      if place[0] == ffi.NULL: _place = None
      else: _place = place[0]
      return r, _place

   def remove(self, item):
      if hasattr(item, 'impl'): item = item.impl
      if item is None: item = ffi.NULL
      lib.OldList_remove(ffi.cast("eC_OldList *", self.impl), item)

   def removeAll(self, freeFn):
      lib.OldList_removeAll(ffi.cast("eC_OldList *", self.impl))

   def sort(self, compare, data):
      if hasattr(data, 'impl'): data = data.impl
      if data is None: data = ffi.NULL
      lib.OldList_sort(ffi.cast("eC_OldList *", self.impl), data)

   def swap(self, item1, item2):
      if hasattr(item1, 'impl'): item1 = item1.impl
      if item1 is None: item1 = ffi.NULL
      if hasattr(item2, 'impl'): item2 = item2.impl
      if item2 is None: item2 = ffi.NULL
      lib.OldList_swap(ffi.cast("eC_OldList *", self.impl), item1, item2)

class StringBTNode:
   def __init__(self, key = None, parent = None, left = None, right = None, depth = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_StringBTNode *", lib.Instance_new(lib.class_StringBTNode))
         if isinstance(key, tuple):
            __tuple = key
            key = None
            if len(__tuple) > 0: key    = __tuple[0]
            if len(__tuple) > 1: parent = __tuple[1]
            if len(__tuple) > 2: left   = __tuple[2]
            if len(__tuple) > 3: right  = __tuple[3]
         if key is not None:    self.key    = key
         if parent is not None: self.parent = parent
         if left is not None:   self.left   = left
         if right is not None:  self.right  = right
         if depth is not None:  self.depth  = depth

   @property
   def key(self): return String(self.impl.key)
   @key.setter
   def key(self, value):
      if isinstance(value, str): value = ffi.new("char[]", value.encode('u8'))
      elif value is None: value = ffi.NULL
      self.impl.key = value

   @property
   def parent(self): return self.impl.parent
   @parent.setter
   def parent(self, value):
      if not isinstance(value, StringBTNode): value = StringBTNode(value)
      self.impl.parent = value.impl

   @property
   def left(self): return self.impl.left
   @left.setter
   def left(self, value):
      if not isinstance(value, StringBTNode): value = StringBTNode(value)
      self.impl.left = value.impl

   @property
   def right(self): return self.impl.right
   @right.setter
   def right(self, value):
      if not isinstance(value, StringBTNode): value = StringBTNode(value)
      self.impl.right = value.impl

   @property
   def depth(self): return self.impl.depth
   @depth.setter
   def depth(self, value): self.impl.depth = value

class StringBinaryTree(BinaryTree):
   def __init__(self, root = None, count = 0, CompareKey = None, FreeKey = None, impl = None):
      if impl is not None:
         self.impl = ffi.new("eC_StringBinaryTree *", impl)
      else:
         if isinstance(root, tuple):
            __tuple = root
            root = None
            if len(__tuple) > 0: root       = __tuple[0]
            if len(__tuple) > 1: count      = __tuple[1]
            if len(__tuple) > 2: CompareKey = __tuple[2]
            if len(__tuple) > 3: FreeKey    = __tuple[3]
         if root is not None:
            if not isinstance(root, BTNode): root = BTNode(root)
            root = root.impl
         else:
            root = ffi.NULL
         self.impl = ffi.new("eC_StringBinaryTree *", { 'root' : root, 'count' : count, 'CompareKey' : CompareKey, 'FreeKey' : FreeKey })

class TreePrintStyle:
   inOrder    = lib.TreePrintStyle_inOrder
   postOrder  = lib.TreePrintStyle_postOrder
   preOrder   = lib.TreePrintStyle_preOrder
   depthOrder = lib.TreePrintStyle_depthOrder

def qsortr(base, nel, width, compare, arg):
   if hasattr(base, 'impl'): base = base.impl
   if base is None: base = ffi.NULL
   if hasattr(arg, 'impl'): arg = arg.impl
   if arg is None: arg = ffi.NULL
   lib.eC_qsortr(base, nel, width, arg)

def qsortrx(base, nel, width, compare, optCompareArgLast, arg, deref, ascending):
   if hasattr(base, 'impl'): base = base.impl
   if base is None: base = ffi.NULL
   if hasattr(arg, 'impl'): arg = arg.impl
   if arg is None: arg = ffi.NULL
   lib.eC_qsortrx(base, nel, width, arg, deref, ascending)

class CharCategories(pyBaseClass):
   def __init__(self, none = False, markNonSpacing = False, markSpacing = False, markEnclosing = False, numberDecimalDigit = False, numberLetter = False, numberOther = False, separatorSpace = False, separatorLine = False, separatorParagraph = False, otherControl = False, otherFormat = False, otherSurrogate = False, otherPrivateUse = False, otherNotAssigned = False, letterUpperCase = False, letterLowerCase = False, letterTitleCase = False, letterModifier = False, letterOther = False, punctuationConnector = False, punctuationDash = False, punctuationOpen = False, punctuationClose = False, punctuationInitial = False, punctuationFinal = False, punctuationOther = False, symbolMath = False, symbolCurrency = False, symbolModifier = False, symbolOther = False,
                impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(none, CharCategories):
         self.impl = none.impl
      else:
         self.impl = (
            (none                 << lib.CHARCATEGORIES_none_SHIFT)                 |
            (markNonSpacing       << lib.CHARCATEGORIES_markNonSpacing_SHIFT)       |
            (markSpacing          << lib.CHARCATEGORIES_markSpacing_SHIFT)          |
            (markEnclosing        << lib.CHARCATEGORIES_markEnclosing_SHIFT)        |
            (numberDecimalDigit   << lib.CHARCATEGORIES_numberDecimalDigit_SHIFT)   |
            (numberLetter         << lib.CHARCATEGORIES_numberLetter_SHIFT)         |
            (numberOther          << lib.CHARCATEGORIES_numberOther_SHIFT)          |
            (separatorSpace       << lib.CHARCATEGORIES_separatorSpace_SHIFT)       |
            (separatorLine        << lib.CHARCATEGORIES_separatorLine_SHIFT)        |
            (separatorParagraph   << lib.CHARCATEGORIES_separatorParagraph_SHIFT)   |
            (otherControl         << lib.CHARCATEGORIES_otherControl_SHIFT)         |
            (otherFormat          << lib.CHARCATEGORIES_otherFormat_SHIFT)          |
            (otherSurrogate       << lib.CHARCATEGORIES_otherSurrogate_SHIFT)       |
            (otherPrivateUse      << lib.CHARCATEGORIES_otherPrivateUse_SHIFT)      |
            (otherNotAssigned     << lib.CHARCATEGORIES_otherNotAssigned_SHIFT)     |
            (letterUpperCase      << lib.CHARCATEGORIES_letterUpperCase_SHIFT)      |
            (letterLowerCase      << lib.CHARCATEGORIES_letterLowerCase_SHIFT)      |
            (letterTitleCase      << lib.CHARCATEGORIES_letterTitleCase_SHIFT)      |
            (letterModifier       << lib.CHARCATEGORIES_letterModifier_SHIFT)       |
            (letterOther          << lib.CHARCATEGORIES_letterOther_SHIFT)          |
            (punctuationConnector << lib.CHARCATEGORIES_punctuationConnector_SHIFT) |
            (punctuationDash      << lib.CHARCATEGORIES_punctuationDash_SHIFT)      |
            (punctuationOpen      << lib.CHARCATEGORIES_punctuationOpen_SHIFT)      |
            (punctuationClose     << lib.CHARCATEGORIES_punctuationClose_SHIFT)     |
            (punctuationInitial   << lib.CHARCATEGORIES_punctuationInitial_SHIFT)   |
            (punctuationFinal     << lib.CHARCATEGORIES_punctuationFinal_SHIFT)     |
            (punctuationOther     << lib.CHARCATEGORIES_punctuationOther_SHIFT)     |
            (symbolMath           << lib.CHARCATEGORIES_symbolMath_SHIFT)           |
            (symbolCurrency       << lib.CHARCATEGORIES_symbolCurrency_SHIFT)       |
            (symbolModifier       << lib.CHARCATEGORIES_symbolModifier_SHIFT)       |
            (symbolOther          << lib.CHARCATEGORIES_symbolOther_SHIFT)          )

   @property
   def none(self): return ((((self.impl)) & lib.CHARCATEGORIES_none_MASK) >> lib.CHARCATEGORIES_none_SHIFT)
   @none.setter
   def none(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_none_MASK)) | (((value)) << lib.CHARCATEGORIES_none_SHIFT)

   @property
   def markNonSpacing(self): return ((((self.impl)) & lib.CHARCATEGORIES_markNonSpacing_MASK) >> lib.CHARCATEGORIES_markNonSpacing_SHIFT)
   @markNonSpacing.setter
   def markNonSpacing(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_markNonSpacing_MASK)) | (((value)) << lib.CHARCATEGORIES_markNonSpacing_SHIFT)

   @property
   def markSpacing(self): return ((((self.impl)) & lib.CHARCATEGORIES_markSpacing_MASK) >> lib.CHARCATEGORIES_markSpacing_SHIFT)
   @markSpacing.setter
   def markSpacing(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_markSpacing_MASK)) | (((value)) << lib.CHARCATEGORIES_markSpacing_SHIFT)

   @property
   def markEnclosing(self): return ((((self.impl)) & lib.CHARCATEGORIES_markEnclosing_MASK) >> lib.CHARCATEGORIES_markEnclosing_SHIFT)
   @markEnclosing.setter
   def markEnclosing(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_markEnclosing_MASK)) | (((value)) << lib.CHARCATEGORIES_markEnclosing_SHIFT)

   @property
   def numberDecimalDigit(self): return ((((self.impl)) & lib.CHARCATEGORIES_numberDecimalDigit_MASK) >> lib.CHARCATEGORIES_numberDecimalDigit_SHIFT)
   @numberDecimalDigit.setter
   def numberDecimalDigit(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_numberDecimalDigit_MASK)) | (((value)) << lib.CHARCATEGORIES_numberDecimalDigit_SHIFT)

   @property
   def numberLetter(self): return ((((self.impl)) & lib.CHARCATEGORIES_numberLetter_MASK) >> lib.CHARCATEGORIES_numberLetter_SHIFT)
   @numberLetter.setter
   def numberLetter(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_numberLetter_MASK)) | (((value)) << lib.CHARCATEGORIES_numberLetter_SHIFT)

   @property
   def numberOther(self): return ((((self.impl)) & lib.CHARCATEGORIES_numberOther_MASK) >> lib.CHARCATEGORIES_numberOther_SHIFT)
   @numberOther.setter
   def numberOther(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_numberOther_MASK)) | (((value)) << lib.CHARCATEGORIES_numberOther_SHIFT)

   @property
   def separatorSpace(self): return ((((self.impl)) & lib.CHARCATEGORIES_separatorSpace_MASK) >> lib.CHARCATEGORIES_separatorSpace_SHIFT)
   @separatorSpace.setter
   def separatorSpace(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_separatorSpace_MASK)) | (((value)) << lib.CHARCATEGORIES_separatorSpace_SHIFT)

   @property
   def separatorLine(self): return ((((self.impl)) & lib.CHARCATEGORIES_separatorLine_MASK) >> lib.CHARCATEGORIES_separatorLine_SHIFT)
   @separatorLine.setter
   def separatorLine(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_separatorLine_MASK)) | (((value)) << lib.CHARCATEGORIES_separatorLine_SHIFT)

   @property
   def separatorParagraph(self): return ((((self.impl)) & lib.CHARCATEGORIES_separatorParagraph_MASK) >> lib.CHARCATEGORIES_separatorParagraph_SHIFT)
   @separatorParagraph.setter
   def separatorParagraph(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_separatorParagraph_MASK)) | (((value)) << lib.CHARCATEGORIES_separatorParagraph_SHIFT)

   @property
   def otherControl(self): return ((((self.impl)) & lib.CHARCATEGORIES_otherControl_MASK) >> lib.CHARCATEGORIES_otherControl_SHIFT)
   @otherControl.setter
   def otherControl(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_otherControl_MASK)) | (((value)) << lib.CHARCATEGORIES_otherControl_SHIFT)

   @property
   def otherFormat(self): return ((((self.impl)) & lib.CHARCATEGORIES_otherFormat_MASK) >> lib.CHARCATEGORIES_otherFormat_SHIFT)
   @otherFormat.setter
   def otherFormat(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_otherFormat_MASK)) | (((value)) << lib.CHARCATEGORIES_otherFormat_SHIFT)

   @property
   def otherSurrogate(self): return ((((self.impl)) & lib.CHARCATEGORIES_otherSurrogate_MASK) >> lib.CHARCATEGORIES_otherSurrogate_SHIFT)
   @otherSurrogate.setter
   def otherSurrogate(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_otherSurrogate_MASK)) | (((value)) << lib.CHARCATEGORIES_otherSurrogate_SHIFT)

   @property
   def otherPrivateUse(self): return ((((self.impl)) & lib.CHARCATEGORIES_otherPrivateUse_MASK) >> lib.CHARCATEGORIES_otherPrivateUse_SHIFT)
   @otherPrivateUse.setter
   def otherPrivateUse(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_otherPrivateUse_MASK)) | (((value)) << lib.CHARCATEGORIES_otherPrivateUse_SHIFT)

   @property
   def otherNotAssigned(self): return ((((self.impl)) & lib.CHARCATEGORIES_otherNotAssigned_MASK) >> lib.CHARCATEGORIES_otherNotAssigned_SHIFT)
   @otherNotAssigned.setter
   def otherNotAssigned(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_otherNotAssigned_MASK)) | (((value)) << lib.CHARCATEGORIES_otherNotAssigned_SHIFT)

   @property
   def letterUpperCase(self): return ((((self.impl)) & lib.CHARCATEGORIES_letterUpperCase_MASK) >> lib.CHARCATEGORIES_letterUpperCase_SHIFT)
   @letterUpperCase.setter
   def letterUpperCase(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_letterUpperCase_MASK)) | (((value)) << lib.CHARCATEGORIES_letterUpperCase_SHIFT)

   @property
   def letterLowerCase(self): return ((((self.impl)) & lib.CHARCATEGORIES_letterLowerCase_MASK) >> lib.CHARCATEGORIES_letterLowerCase_SHIFT)
   @letterLowerCase.setter
   def letterLowerCase(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_letterLowerCase_MASK)) | (((value)) << lib.CHARCATEGORIES_letterLowerCase_SHIFT)

   @property
   def letterTitleCase(self): return ((((self.impl)) & lib.CHARCATEGORIES_letterTitleCase_MASK) >> lib.CHARCATEGORIES_letterTitleCase_SHIFT)
   @letterTitleCase.setter
   def letterTitleCase(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_letterTitleCase_MASK)) | (((value)) << lib.CHARCATEGORIES_letterTitleCase_SHIFT)

   @property
   def letterModifier(self): return ((((self.impl)) & lib.CHARCATEGORIES_letterModifier_MASK) >> lib.CHARCATEGORIES_letterModifier_SHIFT)
   @letterModifier.setter
   def letterModifier(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_letterModifier_MASK)) | (((value)) << lib.CHARCATEGORIES_letterModifier_SHIFT)

   @property
   def letterOther(self): return ((((self.impl)) & lib.CHARCATEGORIES_letterOther_MASK) >> lib.CHARCATEGORIES_letterOther_SHIFT)
   @letterOther.setter
   def letterOther(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_letterOther_MASK)) | (((value)) << lib.CHARCATEGORIES_letterOther_SHIFT)

   @property
   def punctuationConnector(self): return ((((self.impl)) & lib.CHARCATEGORIES_punctuationConnector_MASK) >> lib.CHARCATEGORIES_punctuationConnector_SHIFT)
   @punctuationConnector.setter
   def punctuationConnector(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_punctuationConnector_MASK)) | (((value)) << lib.CHARCATEGORIES_punctuationConnector_SHIFT)

   @property
   def punctuationDash(self): return ((((self.impl)) & lib.CHARCATEGORIES_punctuationDash_MASK) >> lib.CHARCATEGORIES_punctuationDash_SHIFT)
   @punctuationDash.setter
   def punctuationDash(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_punctuationDash_MASK)) | (((value)) << lib.CHARCATEGORIES_punctuationDash_SHIFT)

   @property
   def punctuationOpen(self): return ((((self.impl)) & lib.CHARCATEGORIES_punctuationOpen_MASK) >> lib.CHARCATEGORIES_punctuationOpen_SHIFT)
   @punctuationOpen.setter
   def punctuationOpen(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_punctuationOpen_MASK)) | (((value)) << lib.CHARCATEGORIES_punctuationOpen_SHIFT)

   @property
   def punctuationClose(self): return ((((self.impl)) & lib.CHARCATEGORIES_punctuationClose_MASK) >> lib.CHARCATEGORIES_punctuationClose_SHIFT)
   @punctuationClose.setter
   def punctuationClose(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_punctuationClose_MASK)) | (((value)) << lib.CHARCATEGORIES_punctuationClose_SHIFT)

   @property
   def punctuationInitial(self): return ((((self.impl)) & lib.CHARCATEGORIES_punctuationInitial_MASK) >> lib.CHARCATEGORIES_punctuationInitial_SHIFT)
   @punctuationInitial.setter
   def punctuationInitial(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_punctuationInitial_MASK)) | (((value)) << lib.CHARCATEGORIES_punctuationInitial_SHIFT)

   @property
   def punctuationFinal(self): return ((((self.impl)) & lib.CHARCATEGORIES_punctuationFinal_MASK) >> lib.CHARCATEGORIES_punctuationFinal_SHIFT)
   @punctuationFinal.setter
   def punctuationFinal(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_punctuationFinal_MASK)) | (((value)) << lib.CHARCATEGORIES_punctuationFinal_SHIFT)

   @property
   def punctuationOther(self): return ((((self.impl)) & lib.CHARCATEGORIES_punctuationOther_MASK) >> lib.CHARCATEGORIES_punctuationOther_SHIFT)
   @punctuationOther.setter
   def punctuationOther(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_punctuationOther_MASK)) | (((value)) << lib.CHARCATEGORIES_punctuationOther_SHIFT)

   @property
   def symbolMath(self): return ((((self.impl)) & lib.CHARCATEGORIES_symbolMath_MASK) >> lib.CHARCATEGORIES_symbolMath_SHIFT)
   @symbolMath.setter
   def symbolMath(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_symbolMath_MASK)) | (((value)) << lib.CHARCATEGORIES_symbolMath_SHIFT)

   @property
   def symbolCurrency(self): return ((((self.impl)) & lib.CHARCATEGORIES_symbolCurrency_MASK) >> lib.CHARCATEGORIES_symbolCurrency_SHIFT)
   @symbolCurrency.setter
   def symbolCurrency(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_symbolCurrency_MASK)) | (((value)) << lib.CHARCATEGORIES_symbolCurrency_SHIFT)

   @property
   def symbolModifier(self): return ((((self.impl)) & lib.CHARCATEGORIES_symbolModifier_MASK) >> lib.CHARCATEGORIES_symbolModifier_SHIFT)
   @symbolModifier.setter
   def symbolModifier(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_symbolModifier_MASK)) | (((value)) << lib.CHARCATEGORIES_symbolModifier_SHIFT)

   @property
   def symbolOther(self): return ((((self.impl)) & lib.CHARCATEGORIES_symbolOther_MASK) >> lib.CHARCATEGORIES_symbolOther_SHIFT)
   @symbolOther.setter
   def symbolOther(self, value): self.impl = ((self.impl) & ~(lib.CHARCATEGORIES_symbolOther_MASK)) | (((value)) << lib.CHARCATEGORIES_symbolOther_SHIFT)

class CharCategory:
   none                 = lib.CharCategory_none
   Mn                   = lib.CharCategory_Mn
   markNonSpacing       = lib.CharCategory_markNonSpacing
   Mc                   = lib.CharCategory_Mc
   markSpacing          = lib.CharCategory_markSpacing
   Me                   = lib.CharCategory_Me
   markEnclosing        = lib.CharCategory_markEnclosing
   Nd                   = lib.CharCategory_Nd
   numberDecimalDigit   = lib.CharCategory_numberDecimalDigit
   Nl                   = lib.CharCategory_Nl
   numberLetter         = lib.CharCategory_numberLetter
   No                   = lib.CharCategory_No
   numberOther          = lib.CharCategory_numberOther
   Zs                   = lib.CharCategory_Zs
   separatorSpace       = lib.CharCategory_separatorSpace
   Zl                   = lib.CharCategory_Zl
   separatorLine        = lib.CharCategory_separatorLine
   Zp                   = lib.CharCategory_Zp
   separatorParagraph   = lib.CharCategory_separatorParagraph
   Cc                   = lib.CharCategory_Cc
   otherControl         = lib.CharCategory_otherControl
   Cf                   = lib.CharCategory_Cf
   otherFormat          = lib.CharCategory_otherFormat
   Cs                   = lib.CharCategory_Cs
   otherSurrogate       = lib.CharCategory_otherSurrogate
   Co                   = lib.CharCategory_Co
   otherPrivateUse      = lib.CharCategory_otherPrivateUse
   Cn                   = lib.CharCategory_Cn
   otherNotAssigned     = lib.CharCategory_otherNotAssigned
   Lu                   = lib.CharCategory_Lu
   letterUpperCase      = lib.CharCategory_letterUpperCase
   Ll                   = lib.CharCategory_Ll
   letterLowerCase      = lib.CharCategory_letterLowerCase
   Lt                   = lib.CharCategory_Lt
   letterTitleCase      = lib.CharCategory_letterTitleCase
   Lm                   = lib.CharCategory_Lm
   letterModifier       = lib.CharCategory_letterModifier
   Lo                   = lib.CharCategory_Lo
   letterOther          = lib.CharCategory_letterOther
   Pc                   = lib.CharCategory_Pc
   punctuationConnector = lib.CharCategory_punctuationConnector
   Pd                   = lib.CharCategory_Pd
   punctuationDash      = lib.CharCategory_punctuationDash
   Ps                   = lib.CharCategory_Ps
   punctuationOpen      = lib.CharCategory_punctuationOpen
   Pe                   = lib.CharCategory_Pe
   punctuationClose     = lib.CharCategory_punctuationClose
   Pi                   = lib.CharCategory_Pi
   punctuationInitial   = lib.CharCategory_punctuationInitial
   Pf                   = lib.CharCategory_Pf
   punctuationFinal     = lib.CharCategory_punctuationFinal
   Po                   = lib.CharCategory_Po
   punctuationOther     = lib.CharCategory_punctuationOther
   Sm                   = lib.CharCategory_Sm
   symbolMath           = lib.CharCategory_symbolMath
   Sc                   = lib.CharCategory_Sc
   symbolCurrency       = lib.CharCategory_symbolCurrency
   Sk                   = lib.CharCategory_Sk
   symbolModifier       = lib.CharCategory_symbolModifier
   So                   = lib.CharCategory_So
   symbolOther          = lib.CharCategory_symbolOther

class PredefinedCharCategories(CharCategories):
   none        = CharCategories(impl = lib.PredefinedCharCategories_none)
   marks       = CharCategories(impl = lib.PredefinedCharCategories_marks)
   numbers     = CharCategories(impl = lib.PredefinedCharCategories_numbers)
   separators  = CharCategories(impl = lib.PredefinedCharCategories_separators)
   others      = CharCategories(impl = lib.PredefinedCharCategories_others)
   letters     = CharCategories(impl = lib.PredefinedCharCategories_letters)
   punctuation = CharCategories(impl = lib.PredefinedCharCategories_punctuation)
   symbols     = CharCategories(impl = lib.PredefinedCharCategories_symbols)
   connector   = CharCategories(impl = lib.PredefinedCharCategories_connector)

class UnicodeDecomposition(pyBaseClass):
   def __init__(self, canonical = False, compat = False, fraction = False, font = False, noBreak = False, initial = False, final = False, medial = False, isolated = False, circle = False, square = False, sub = False, super = False, small = False, vertical = False, wide = False, narrow = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(canonical, UnicodeDecomposition):
         self.impl = canonical.impl
      else:
         self.impl = (
            (canonical << lib.UNICODEDECOMPOSITION_canonical_SHIFT) |
            (compat    << lib.UNICODEDECOMPOSITION_compat_SHIFT)    |
            (fraction  << lib.UNICODEDECOMPOSITION_fraction_SHIFT)  |
            (font      << lib.UNICODEDECOMPOSITION_font_SHIFT)      |
            (noBreak   << lib.UNICODEDECOMPOSITION_noBreak_SHIFT)   |
            (initial   << lib.UNICODEDECOMPOSITION_initial_SHIFT)   |
            (final     << lib.UNICODEDECOMPOSITION_final_SHIFT)     |
            (medial    << lib.UNICODEDECOMPOSITION_medial_SHIFT)    |
            (isolated  << lib.UNICODEDECOMPOSITION_isolated_SHIFT)  |
            (circle    << lib.UNICODEDECOMPOSITION_circle_SHIFT)    |
            (square    << lib.UNICODEDECOMPOSITION_square_SHIFT)    |
            (sub       << lib.UNICODEDECOMPOSITION_sub_SHIFT)       |
            (super     << lib.UNICODEDECOMPOSITION_super_SHIFT)     |
            (small     << lib.UNICODEDECOMPOSITION_small_SHIFT)     |
            (vertical  << lib.UNICODEDECOMPOSITION_vertical_SHIFT)  |
            (wide      << lib.UNICODEDECOMPOSITION_wide_SHIFT)      |
            (narrow    << lib.UNICODEDECOMPOSITION_narrow_SHIFT)    )

   @property
   def canonical(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_canonical_MASK) >> lib.UNICODEDECOMPOSITION_canonical_SHIFT)
   @canonical.setter
   def canonical(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_canonical_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_canonical_SHIFT)

   @property
   def compat(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_compat_MASK) >> lib.UNICODEDECOMPOSITION_compat_SHIFT)
   @compat.setter
   def compat(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_compat_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_compat_SHIFT)

   @property
   def fraction(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_fraction_MASK) >> lib.UNICODEDECOMPOSITION_fraction_SHIFT)
   @fraction.setter
   def fraction(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_fraction_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_fraction_SHIFT)

   @property
   def font(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_font_MASK) >> lib.UNICODEDECOMPOSITION_font_SHIFT)
   @font.setter
   def font(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_font_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_font_SHIFT)

   @property
   def noBreak(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_noBreak_MASK) >> lib.UNICODEDECOMPOSITION_noBreak_SHIFT)
   @noBreak.setter
   def noBreak(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_noBreak_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_noBreak_SHIFT)

   @property
   def initial(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_initial_MASK) >> lib.UNICODEDECOMPOSITION_initial_SHIFT)
   @initial.setter
   def initial(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_initial_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_initial_SHIFT)

   @property
   def final(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_final_MASK) >> lib.UNICODEDECOMPOSITION_final_SHIFT)
   @final.setter
   def final(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_final_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_final_SHIFT)

   @property
   def medial(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_medial_MASK) >> lib.UNICODEDECOMPOSITION_medial_SHIFT)
   @medial.setter
   def medial(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_medial_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_medial_SHIFT)

   @property
   def isolated(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_isolated_MASK) >> lib.UNICODEDECOMPOSITION_isolated_SHIFT)
   @isolated.setter
   def isolated(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_isolated_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_isolated_SHIFT)

   @property
   def circle(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_circle_MASK) >> lib.UNICODEDECOMPOSITION_circle_SHIFT)
   @circle.setter
   def circle(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_circle_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_circle_SHIFT)

   @property
   def square(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_square_MASK) >> lib.UNICODEDECOMPOSITION_square_SHIFT)
   @square.setter
   def square(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_square_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_square_SHIFT)

   @property
   def sub(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_sub_MASK) >> lib.UNICODEDECOMPOSITION_sub_SHIFT)
   @sub.setter
   def sub(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_sub_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_sub_SHIFT)

   @property
   def super(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_super_MASK) >> lib.UNICODEDECOMPOSITION_super_SHIFT)
   @super.setter
   def super(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_super_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_super_SHIFT)

   @property
   def small(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_small_MASK) >> lib.UNICODEDECOMPOSITION_small_SHIFT)
   @small.setter
   def small(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_small_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_small_SHIFT)

   @property
   def vertical(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_vertical_MASK) >> lib.UNICODEDECOMPOSITION_vertical_SHIFT)
   @vertical.setter
   def vertical(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_vertical_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_vertical_SHIFT)

   @property
   def wide(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_wide_MASK) >> lib.UNICODEDECOMPOSITION_wide_SHIFT)
   @wide.setter
   def wide(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_wide_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_wide_SHIFT)

   @property
   def narrow(self): return ((((self.impl)) & lib.UNICODEDECOMPOSITION_narrow_MASK) >> lib.UNICODEDECOMPOSITION_narrow_SHIFT)
   @narrow.setter
   def narrow(self, value): self.impl = ((self.impl) & ~(lib.UNICODEDECOMPOSITION_narrow_MASK)) | (((value)) << lib.UNICODEDECOMPOSITION_narrow_SHIFT)

def charMatchCategories(ch, categories):
   if ch is not None and not isinstance(ch, unichar): ch = unichar(ch)
   if ch is None: ch = ffi.NULL
   if categories is not None and not isinstance(categories, CharCategories): categories = CharCategories(categories)
   if categories is None: categories = ffi.NULL
   return lib.eC_charMatchCategories(ch, categories)

def getAlNum(input, string, max):
   if isinstance(input, str): input = ffi.new("char[]", input.encode('u8'))
   elif input is None: input = ffi.NULL
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return lib.eC_getAlNum(input, string, max)

def getCharCategory(ch):
   if ch is not None and not isinstance(ch, unichar): ch = unichar(ch)
   if ch is None: ch = ffi.NULL
   return lib.eC_getCharCategory(ch)

def getCombiningClass(ch):
   if ch is not None and not isinstance(ch, unichar): ch = unichar(ch)
   if ch is None: ch = ffi.NULL
   return lib.eC_getCombiningClass(ch)

def iSO8859_1toUTF8(source, dest, max):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   if isinstance(dest, str): dest = ffi.new("char[]", dest.encode('u8'))
   elif dest is None: dest = ffi.NULL
   return lib.eC_iSO8859_1toUTF8(source, dest, max)

def uTF16BEtoUTF8Buffer(source, dest, max):
   return lib.eC_uTF16BEtoUTF8Buffer(source, dest, max)

def uTF16toUTF8(source):
   return lib.eC_uTF16toUTF8(source)

def uTF16toUTF8Buffer(source, dest, max):
   if isinstance(dest, str): dest = ffi.new("char[]", dest.encode('u8'))
   elif dest is None: dest = ffi.NULL
   return lib.eC_uTF16toUTF8Buffer(source, dest, max)

def uTF32toUTF8Len(source, count, dest, max):
   if isinstance(dest, str): dest = ffi.new("char[]", dest.encode('u8'))
   elif dest is None: dest = ffi.NULL
   return lib.eC_uTF32toUTF8Len(source, count, dest, max)

def uTF8GetChar(string):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   numBytes = ffi.new("int *")
   r = lib.eC_uTF8GetChar(string, numBytes)
   return r, numBytes[0]

def uTF8Validate(source):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   return lib.eC_uTF8Validate(source)

def uTF8toISO8859_1(source, dest, max):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   if isinstance(dest, str): dest = ffi.new("char[]", dest.encode('u8'))
   elif dest is None: dest = ffi.NULL
   return lib.eC_uTF8toISO8859_1(source, dest, max)

def uTF8toUTF16(source):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   wordCount = ffi.new("int *")
   r = lib.eC_uTF8toUTF16(source, wordCount)
   return r, wordCount[0]

def uTF8toUTF16Buffer(source, max):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   dest = ffi.new("uint16 *")
   r = lib.eC_uTF8toUTF16Buffer(source, dest, max)
   return r, dest[0]

def uTF8toUTF16BufferLen(source, max, len):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   dest = ffi.new("uint16 *")
   r = lib.eC_uTF8toUTF16BufferLen(source, dest, max, len)
   return r, dest[0]

def uTF8toUTF16Len(source, byteCount):
   if isinstance(source, str): source = ffi.new("char[]", source.encode('u8'))
   elif source is None: source = ffi.NULL
   wordCount = ffi.new("int *")
   r = lib.eC_uTF8toUTF16Len(source, byteCount, wordCount)
   return r, wordCount[0]

def accenti(string = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(String, lib.eC_accenti(string))

def casei(string = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(String, lib.eC_casei(string))

def encodeArrayToString(array = None):
   return pyOrNewObject(String, lib.eC_encodeArrayToString(Array.impl))

def normalizeNFC(string = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(String, lib.eC_normalizeNFC(string))

def normalizeNFD(string = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(String, lib.eC_normalizeNFD(string))

def normalizeNFKC(string = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(String, lib.eC_normalizeNFKC(string))

def normalizeNFKD(string = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(String, lib.eC_normalizeNFKD(string))

def normalizeNFKDArray(string = None):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(Array, lib.eC_normalizeNFKDArray(string), "<unichar>")

def normalizeUnicode(string, type, compose):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if type is not None and not isinstance(type, UnicodeDecomposition): type = UnicodeDecomposition(type)
   if type is None: type = ffi.NULL
   return pyOrNewObject(String, lib.eC_normalizeUnicode(string, type, compose))

def normalizeUnicodeArray(string, type, compose):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   if type is not None and not isinstance(type, UnicodeDecomposition): type = UnicodeDecomposition(type)
   if type is None: type = ffi.NULL
   return pyOrNewObject(Array, lib.eC_normalizeUnicodeArray(string, type, compose), "<unichar>")

def stripUnicodeCategory(string, c):
   if isinstance(string, str): string = ffi.new("char[]", string.encode('u8'))
   elif string is None: string = ffi.NULL
   return pyOrNewObject(String, lib.eC_stripUnicodeCategory(string, c))

@ffi.callback("eC_SettingsIOResult(eC_GlobalSettings)")
def cb_GlobalSettings_load(__e):
   globalsettings = pyOrNewObject(GlobalSettings, __e)
   return globalsettings.fn_GlobalSettings_load(globalsettings)

@ffi.callback("void(eC_GlobalSettings)")
def cb_GlobalSettings_onAskReloadSettings(__e):
   globalsettings = pyOrNewObject(GlobalSettings, __e)
   globalsettings.fn_GlobalSettings_onAskReloadSettings(globalsettings)

@ffi.callback("eC_SettingsIOResult(eC_GlobalSettings)")
def cb_GlobalSettings_save(__e):
   globalsettings = pyOrNewObject(GlobalSettings, __e)
   return globalsettings.fn_GlobalSettings_save(globalsettings)

class GlobalSettings(Instance):
   class_members = [
                      'settingsName',
                      'settingsExtension',
                      'settingsDirectory',
                      'settingsLocation',
                      'settingsFilePath',
                      'allowDefaultLocations',
                      'allUsers',
                      'portable',
                      'driver',
                      'data',
                      'dataOwner',
                      'dataClass',
                      'isGlobalPath',
                      'load',
                      'onAskReloadSettings',
                      'save',
                   ]

   def init_args(self, args, kwArgs): init_args(GlobalSettings, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def settingsName(self): value = lib.GlobalSettings_get_settingsName(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @settingsName.setter
   def settingsName(self, value):
      lib.GlobalSettings_set_settingsName(self.impl, value.encode('u8'))

   @property
   def settingsExtension(self): value = lib.GlobalSettings_get_settingsExtension(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @settingsExtension.setter
   def settingsExtension(self, value):
      lib.GlobalSettings_set_settingsExtension(self.impl, value.encode('u8'))

   @property
   def settingsDirectory(self): value = lib.GlobalSettings_get_settingsDirectory(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @settingsDirectory.setter
   def settingsDirectory(self, value):
      lib.GlobalSettings_set_settingsDirectory(self.impl, value.encode('u8'))

   @property
   def settingsLocation(self): value = lib.GlobalSettings_get_settingsLocation(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @settingsLocation.setter
   def settingsLocation(self, value):
      lib.GlobalSettings_set_settingsLocation(self.impl, value.encode('u8'))

   @property
   def settingsFilePath(self): value = lib.GlobalSettings_get_settingsFilePath(self.impl); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @settingsFilePath.setter
   def settingsFilePath(self, value):
      lib.GlobalSettings_set_settingsFilePath(self.impl, value.encode('u8'))

   @property
   def allowDefaultLocations(self): return lib.GlobalSettings_get_allowDefaultLocations(self.impl)
   @allowDefaultLocations.setter
   def allowDefaultLocations(self, value):
      lib.GlobalSettings_set_allowDefaultLocations(self.impl, value)

   @property
   def allUsers(self): return lib.GlobalSettings_get_allUsers(self.impl)
   @allUsers.setter
   def allUsers(self, value):
      lib.GlobalSettings_set_allUsers(self.impl, value)

   @property
   def portable(self): return lib.GlobalSettings_get_portable(self.impl)
   @portable.setter
   def portable(self, value):
      lib.GlobalSettings_set_portable(self.impl, value)

   @property
   def driver(self): value = lib.GlobalSettings_get_driver(self.impl) if self is not None and self.impl != ffi.NULL else ffi.NULL; return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @driver.setter
   def driver(self, value):
      if not isinstance(value, String): value = String(value)
      lib.GlobalSettings_set_driver(self.impl, value.impl.encode('u8'))

   @property
   def data(self): return pyOrNewObject(GlobalSettingsData, IPTR(lib, ffi, self, GlobalSettings).data)
   @data.setter
   def data(self, value):
      if not isinstance(value, GlobalSettingsData): value = GlobalSettingsData(value)
      IPTR(lib, ffi, self, GlobalSettings).data = value.impl

   @property
   def dataOwner(self): return IPTR(lib, ffi, self, GlobalSettings).dataOwner
   @dataOwner.setter
   def dataOwner(self, value): IPTR(lib, ffi, self, GlobalSettings).dataOwner = value

   @property
   def dataClass(self): return IPTR(lib, ffi, self, GlobalSettings).dataClass
   @dataClass.setter
   def dataClass(self, value): IPTR(lib, ffi, self, GlobalSettings).dataClass = value

   @property
   def isGlobalPath(self): return lib.GlobalSettings_get_isGlobalPath(self.impl)

   def close(self):
      lib.GlobalSettings_close(self.impl)

   def closeAndMonitor(self):
      lib.GlobalSettings_closeAndMonitor(self.impl)

   def fn_unset_GlobalSettings_load(self):
      return lib.GlobalSettings_load(self.impl)

   @property
   def load(self):
      if hasattr(self, 'fn_GlobalSettings_load'): return self.fn_GlobalSettings_load
      else: return self.fn_unset_GlobalSettings_load
   @load.setter
   def load(self, value):
      self.fn_GlobalSettings_load = value
      lib.Instance_setMethod(self.impl, "Load".encode('u8'), cb_GlobalSettings_load)

   def fn_unset_GlobalSettings_onAskReloadSettings(self):
      return lib.GlobalSettings_onAskReloadSettings(self.impl)

   @property
   def onAskReloadSettings(self):
      if hasattr(self, 'fn_GlobalSettings_onAskReloadSettings'): return self.fn_GlobalSettings_onAskReloadSettings
      else: return self.fn_unset_GlobalSettings_onAskReloadSettings
   @onAskReloadSettings.setter
   def onAskReloadSettings(self, value):
      self.fn_GlobalSettings_onAskReloadSettings = value
      lib.Instance_setMethod(self.impl, "OnAskReloadSettings".encode('u8'), cb_GlobalSettings_onAskReloadSettings)

   def openAndLock(self):
      fileSize = ffi.new("eC_FileSize *")
      r = lib.GlobalSettings_openAndLock(self.impl, fileSize)
      return r, FileSize(impl = fileSize[0])

   def fn_unset_GlobalSettings_save(self):
      return lib.GlobalSettings_save(self.impl)

   @property
   def save(self):
      if hasattr(self, 'fn_GlobalSettings_save'): return self.fn_GlobalSettings_save
      else: return self.fn_unset_GlobalSettings_save
   @save.setter
   def save(self, value):
      self.fn_GlobalSettings_save = value
      lib.Instance_setMethod(self.impl, "Save".encode('u8'), cb_GlobalSettings_save)

@ffi.callback("eC_SettingsIOResult(eC_GlobalSettingsDriver, eC_File, eC_GlobalSettings)")
def cb_GlobalSettingsDriver_load(__e, f, globalSettings):
   globalsettingsdriver = pyOrNewObject(GlobalSettingsDriver, __e)
   return globalsettingsdriver.fn_GlobalSettingsDriver_load(globalsettingsdriver, pyOrNewObject(File, f), pyOrNewObject(GlobalSettings, globalSettings))

@ffi.callback("eC_SettingsIOResult(eC_GlobalSettingsDriver, eC_File, eC_GlobalSettings)")
def cb_GlobalSettingsDriver_save(__e, f, globalSettings):
   globalsettingsdriver = pyOrNewObject(GlobalSettingsDriver, __e)
   return globalsettingsdriver.fn_GlobalSettingsDriver_save(globalsettingsdriver, pyOrNewObject(File, f), pyOrNewObject(GlobalSettings, globalSettings))

class GlobalSettingsDriver(Instance):
   class_members = [
                      'load',
                      'save',
                   ]

   def init_args(self, args, kwArgs): init_args(GlobalSettingsDriver, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   def fn_unset_GlobalSettingsDriver_load(self, f, globalSettings):
      return lib.GlobalSettingsDriver_load(self.impl, ffi.NULL if f is None else f.impl, ffi.NULL if globalSettings is None else globalSettings.impl)

   @property
   def load(self):
      if hasattr(self, 'fn_GlobalSettingsDriver_load'): return self.fn_GlobalSettingsDriver_load
      else: return self.fn_unset_GlobalSettingsDriver_load
   @load.setter
   def load(self, value):
      self.fn_GlobalSettingsDriver_load = value
      lib.Instance_setMethod(self.impl, "Load".encode('u8'), cb_GlobalSettingsDriver_load)

   def fn_unset_GlobalSettingsDriver_save(self, f, globalSettings):
      return lib.GlobalSettingsDriver_save(self.impl, ffi.NULL if f is None else f.impl, ffi.NULL if globalSettings is None else globalSettings.impl)

   @property
   def save(self):
      if hasattr(self, 'fn_GlobalSettingsDriver_save'): return self.fn_GlobalSettingsDriver_save
      else: return self.fn_unset_GlobalSettingsDriver_save
   @save.setter
   def save(self, value):
      self.fn_GlobalSettingsDriver_save = value
      lib.Instance_setMethod(self.impl, "Save".encode('u8'), cb_GlobalSettingsDriver_save)

class JSONParser(Instance):
   class_members = [
                      'f',
                      'customJsonOptions',
                      'debug',
                      'warnings',
                   ]

   def init_args(self, args, kwArgs): init_args(JSONParser, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def f(self): return pyOrNewObject(File, IPTR(lib, ffi, self, JSONParser).f)
   @f.setter
   def f(self, value):
      if not isinstance(value, File): value = File(value)
      IPTR(lib, ffi, self, JSONParser).f = value.impl

   @property
   def customJsonOptions(self): return pyOrNewObject(OptionsMap, IPTR(lib, ffi, self, JSONParser).customJsonOptions)
   @customJsonOptions.setter
   def customJsonOptions(self, value):
      if not isinstance(value, OptionsMap): value = OptionsMap(value)
      IPTR(lib, ffi, self, JSONParser).customJsonOptions = value.impl

   @property
   def debug(self): return lib.JSONParser_get_debug(self.impl)
   @debug.setter
   def debug(self, value):
      lib.JSONParser_set_debug(self.impl, value)

   @property
   def warnings(self): return lib.JSONParser_get_warnings(self.impl)
   @warnings.setter
   def warnings(self, value):
      lib.JSONParser_set_warnings(self.impl, value)

   def getObject(self, objectType = None):
      if objectType is not None and not isinstance(objectType, Class): objectType = Class(objectType)
      objectType = ffi.NULL if objectType is None else objectType.impl
      object = ffi.new("void * *")
      r = lib.JSONParser_getObject(self.impl, ffi.cast("struct eC_Class *", objectType), object)
      if object[0] == ffi.NULL: _object = None
      else:
         if objectType.type == ClassType.normalClass:
            i = ffi.cast("eC_Instance", object[0])
            n = ffi.string(i._class.name).decode('u8')
         else:
            n = ffi.string(objectType.name).decode('u8')
         t = pyTypeByName(n)
         ct = n + " * " if objectType.type == ClassType.noHeadClass else n
         _object = t(impl = pyFFI().cast(ct, object[0]))
      return r, _object

class ECONGlobalSettings(GlobalSettingsDriver):
   class_members = []

   def init_args(self, args, kwArgs): init_args(ECONGlobalSettings, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class ECONParser(JSONParser):
   class_members = []

   def init_args(self, args, kwArgs): init_args(ECONParser, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class GlobalAppSettings(GlobalSettings):
   class_members = []

   def init_args(self, args, kwArgs): init_args(GlobalAppSettings, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   def getGlobalValue(self, section, name, type, value):
      if isinstance(section, str): section = ffi.new("char[]", section.encode('u8'))
      elif section is None: section = ffi.NULL
      if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
      elif name is None: name = ffi.NULL
      if hasattr(value, 'impl'): value = value.impl
      if value is None: value = ffi.NULL
      return lib.GlobalAppSettings_getGlobalValue(self.impl, section, name, type, value)

   def putGlobalValue(self, section, name, type, value):
      if isinstance(section, str): section = ffi.new("char[]", section.encode('u8'))
      elif section is None: section = ffi.NULL
      if isinstance(name, str): name = ffi.new("char[]", name.encode('u8'))
      elif name is None: name = ffi.NULL
      if hasattr(value, 'impl'): value = value.impl
      if value is None: value = ffi.NULL
      return lib.GlobalAppSettings_putGlobalValue(self.impl, section, name, type, value)

class GlobalSettingType:
   integer      = lib.GlobalSettingType_integer
   singleString = lib.GlobalSettingType_singleString
   stringList   = lib.GlobalSettingType_stringList

class GlobalSettingsData(Instance):
   class_members = []

   def init_args(self, args, kwArgs): init_args(GlobalSettingsData, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class JSONFirstLetterCapitalization:
   keepCase  = lib.JSONFirstLetterCapitalization_keepCase
   upperCase = lib.JSONFirstLetterCapitalization_upperCase
   lowerCase = lib.JSONFirstLetterCapitalization_lowerCase

class JSONGlobalSettings(GlobalSettingsDriver):
   class_members = []

   def init_args(self, args, kwArgs): init_args(JSONGlobalSettings, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

class JSONResult:
   syntaxError  = lib.JSONResult_syntaxError
   success      = lib.JSONResult_success
   typeMismatch = lib.JSONResult_typeMismatch
   noItem       = lib.JSONResult_noItem

class JSONTypeOptions(pyBaseClass):
   def __init__(self, numbersUseOGDFS = False, boolUseOGDFS = False, nullUseOGDFS = False, stringUseOGDFS = False, arrayUseOGDFS = False, objectUseOGDFS = False, stripQuotesForOGDFS = False, strictOGDFS = False, impl = None):
      if impl is not None:
         self.impl = impl
      elif isinstance(numbersUseOGDFS, JSONTypeOptions):
         self.impl = numbersUseOGDFS.impl
      else:
         self.impl = (
            (numbersUseOGDFS     << lib.JSONTYPEOPTIONS_numbersUseOGDFS_SHIFT)     |
            (boolUseOGDFS        << lib.JSONTYPEOPTIONS_boolUseOGDFS_SHIFT)        |
            (nullUseOGDFS        << lib.JSONTYPEOPTIONS_nullUseOGDFS_SHIFT)        |
            (stringUseOGDFS      << lib.JSONTYPEOPTIONS_stringUseOGDFS_SHIFT)      |
            (arrayUseOGDFS       << lib.JSONTYPEOPTIONS_arrayUseOGDFS_SHIFT)       |
            (objectUseOGDFS      << lib.JSONTYPEOPTIONS_objectUseOGDFS_SHIFT)      |
            (stripQuotesForOGDFS << lib.JSONTYPEOPTIONS_stripQuotesForOGDFS_SHIFT) |
            (strictOGDFS         << lib.JSONTYPEOPTIONS_strictOGDFS_SHIFT)         )

   @property
   def numbersUseOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_numbersUseOGDFS_MASK) >> lib.JSONTYPEOPTIONS_numbersUseOGDFS_SHIFT)
   @numbersUseOGDFS.setter
   def numbersUseOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_numbersUseOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_numbersUseOGDFS_SHIFT)

   @property
   def boolUseOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_boolUseOGDFS_MASK) >> lib.JSONTYPEOPTIONS_boolUseOGDFS_SHIFT)
   @boolUseOGDFS.setter
   def boolUseOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_boolUseOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_boolUseOGDFS_SHIFT)

   @property
   def nullUseOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_nullUseOGDFS_MASK) >> lib.JSONTYPEOPTIONS_nullUseOGDFS_SHIFT)
   @nullUseOGDFS.setter
   def nullUseOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_nullUseOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_nullUseOGDFS_SHIFT)

   @property
   def stringUseOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_stringUseOGDFS_MASK) >> lib.JSONTYPEOPTIONS_stringUseOGDFS_SHIFT)
   @stringUseOGDFS.setter
   def stringUseOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_stringUseOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_stringUseOGDFS_SHIFT)

   @property
   def arrayUseOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_arrayUseOGDFS_MASK) >> lib.JSONTYPEOPTIONS_arrayUseOGDFS_SHIFT)
   @arrayUseOGDFS.setter
   def arrayUseOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_arrayUseOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_arrayUseOGDFS_SHIFT)

   @property
   def objectUseOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_objectUseOGDFS_MASK) >> lib.JSONTYPEOPTIONS_objectUseOGDFS_SHIFT)
   @objectUseOGDFS.setter
   def objectUseOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_objectUseOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_objectUseOGDFS_SHIFT)

   @property
   def stripQuotesForOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_stripQuotesForOGDFS_MASK) >> lib.JSONTYPEOPTIONS_stripQuotesForOGDFS_SHIFT)
   @stripQuotesForOGDFS.setter
   def stripQuotesForOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_stripQuotesForOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_stripQuotesForOGDFS_SHIFT)

   @property
   def strictOGDFS(self): return ((((self.impl)) & lib.JSONTYPEOPTIONS_strictOGDFS_MASK) >> lib.JSONTYPEOPTIONS_strictOGDFS_SHIFT)
   @strictOGDFS.setter
   def strictOGDFS(self, value): self.impl = ((self.impl) & ~(lib.JSONTYPEOPTIONS_strictOGDFS_MASK)) | (((value)) << lib.JSONTYPEOPTIONS_strictOGDFS_SHIFT)

class OptionsMap(Map):
   class_members = []

   def init_args(self, args, kwArgs): init_args(OptionsMap, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      kwArgs['templateParams'] = "<String, JSONTypeOptions>"
      self.init_args(list(args), kwArgs)

class SetBool:
   unset = lib.SetBool_unset
   false = lib.SetBool_false
   true  = lib.SetBool_true

class SettingsIOResult:
   error                       = lib.SettingsIOResult_error
   success                     = lib.SettingsIOResult_success
   fileNotFound                = lib.SettingsIOResult_fileNotFound
   fileNotCompatibleWithDriver = lib.SettingsIOResult_fileNotCompatibleWithDriver

def printECONObject(objectType, object, indent):
   if objectType is not None and not isinstance(objectType, Class): objectType = Class(objectType)
   objectType = ffi.NULL if objectType is None else objectType.impl
   if hasattr(object, 'impl'): object = object.impl
   if object is None: object = ffi.NULL
   return pyOrNewObject(String, lib.eC_printECONObject(ffi.cast("struct eC_Class *", objectType), object, indent))

def printObjectNotationString(objectType, object, onType, indent, indentFirst, capitalize):
   if objectType is not None and not isinstance(objectType, Class): objectType = Class(objectType)
   objectType = ffi.NULL if objectType is None else objectType.impl
   if hasattr(object, 'impl'): object = object.impl
   if object is None: object = ffi.NULL
   return pyOrNewObject(String, lib.eC_printObjectNotationString(ffi.cast("struct eC_Class *", objectType), object, onType, indent, indentFirst, capitalize))

def stringIndent(base, nSpaces, indentFirst):
   if isinstance(base, str): base = ffi.new("char[]", base.encode('u8'))
   elif base is None: base = ffi.NULL
   return pyOrNewObject(String, lib.eC_stringIndent(base, nSpaces, indentFirst))

def writeECONObject(f, objectType, object, indent):
   if f is not None and not isinstance(f, File): f = File(f)
   f = ffi.NULL if f is None else f.impl
   if objectType is not None and not isinstance(objectType, Class): objectType = Class(objectType)
   objectType = ffi.NULL if objectType is None else objectType.impl
   if hasattr(object, 'impl'): object = object.impl
   if object is None: object = ffi.NULL
   return lib.eC_writeECONObject(f, ffi.cast("struct eC_Class *", objectType), object, indent)

def writeJSONObject(f, objectType, object, indent):
   if f is not None and not isinstance(f, File): f = File(f)
   f = ffi.NULL if f is None else f.impl
   if objectType is not None and not isinstance(objectType, Class): objectType = Class(objectType)
   objectType = ffi.NULL if objectType is None else objectType.impl
   if hasattr(object, 'impl'): object = object.impl
   if object is None: object = ffi.NULL
   return lib.eC_writeJSONObject(f, ffi.cast("struct eC_Class *", objectType), object, indent)

def writeJSONObject2(f, objectType, object, indent, capitalize):
   if f is not None and not isinstance(f, File): f = File(f)
   f = ffi.NULL if f is None else f.impl
   if objectType is not None and not isinstance(objectType, Class): objectType = Class(objectType)
   objectType = ffi.NULL if objectType is None else objectType.impl
   if hasattr(object, 'impl'): object = object.impl
   if object is None: object = ffi.NULL
   return lib.eC_writeJSONObject2(f, ffi.cast("struct eC_Class *", objectType), object, indent, capitalize)

def writeJSONObjectMapped(f, objectType, object, indent, stringMap = None):
   if f is not None and not isinstance(f, File): f = File(f)
   f = ffi.NULL if f is None else f.impl
   if objectType is not None and not isinstance(objectType, Class): objectType = Class(objectType)
   objectType = ffi.NULL if objectType is None else objectType.impl
   if hasattr(object, 'impl'): object = object.impl
   if object is None: object = ffi.NULL
   return lib.eC_writeJSONObjectMapped(f, ffi.cast("struct eC_Class *", objectType), object, indent, Map.impl)

def writeONString(f, s, eCON, indent):
   if f is not None and not isinstance(f, File): f = File(f)
   f = ffi.NULL if f is None else f.impl
   if isinstance(s, str): s = ffi.new("char[]", s.encode('u8'))
   elif s is None: s = ffi.NULL
   return lib.eC_writeONString(f, s, eCON, indent)

class Condition:
   def __init__(self, name = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_Condition *", lib.Instance_new(lib.class_Condition))
         if name is not None: self.name      = name
         if name is not None: self.name = name

   @property
   def name(self): value = lib.Condition_get_name(ffi.cast("struct eC_Condition *", self.impl)); return None if value == ffi.NULL else ffi.string(value).decode('u8')
   @name.setter
   def name(self, value):
      lib.Condition_set_name(ffi.cast("struct eC_Condition *", self.impl), value.encode('u8'))

   def signal(self):
      lib.Condition_signal(ffi.cast("struct eC_Condition *", self.impl))

   def wait(self, mutex = None):
      if mutex is not None and not isinstance(mutex, Mutex): mutex = Mutex(mutex)
      mutex = ffi.NULL if mutex is None else mutex.impl
      lib.Condition_wait(ffi.cast("struct eC_Condition *", self.impl), ffi.cast("struct eC_Mutex *", mutex))

class Mutex:
   def __init__(self, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_Mutex *", lib.Instance_new(lib.class_Mutex))

   @property
   def lockCount(self): return lib.Mutex_get_lockCount(ffi.cast("struct eC_Mutex *", self.impl))

   @property
   def owningThread(self): return lib.Mutex_get_owningThread(ffi.cast("struct eC_Mutex *", self.impl))

   def release(self):
      lib.Mutex_release(ffi.cast("struct eC_Mutex *", self.impl))

   def wait(self):
      lib.Mutex_wait(ffi.cast("struct eC_Mutex *", self.impl))

class Semaphore:
   def __init__(self, initCount = None, maxCount = None, impl = None):
      if impl is not None:
         self.impl = impl
      else:
         self.impl = ffi.cast("eC_Semaphore *", lib.Instance_new(lib.class_Semaphore))
         if isinstance(initCount, tuple):
            __tuple = initCount
            initCount = 0
            if len(__tuple) > 0: initCount = __tuple[0]
            if len(__tuple) > 1: maxCount  = __tuple[1]
         if initCount is not None: self.initCount      = initCount
         if maxCount is not None:  self.maxCount       = maxCount
         if initCount is not None: self.initCount = initCount
         if maxCount is not None:  self.maxCount  = maxCount

   @property
   def initCount(self): return lib.Semaphore_get_initCount(ffi.cast("struct eC_Semaphore *", self.impl))
   @initCount.setter
   def initCount(self, value):
      lib.Semaphore_set_initCount(ffi.cast("struct eC_Semaphore *", self.impl), value)

   @property
   def maxCount(self): return lib.Semaphore_get_maxCount(ffi.cast("struct eC_Semaphore *", self.impl))
   @maxCount.setter
   def maxCount(self, value):
      lib.Semaphore_set_maxCount(ffi.cast("struct eC_Semaphore *", self.impl), value)

   def release(self):
      lib.Semaphore_release(ffi.cast("struct eC_Semaphore *", self.impl))

   def tryWait(self):
      return lib.Semaphore_tryWait(ffi.cast("struct eC_Semaphore *", self.impl))

   def wait(self):
      lib.Semaphore_wait(ffi.cast("struct eC_Semaphore *", self.impl))

@ffi.callback("uint(eC_Thread)")
def cb_Thread_main(__e):
   thread = pyOrNewObject(Thread, __e)
   return thread.fn_Thread_main(thread)

class Thread(Instance):
   class_members = [
                      'created',
                      'main',
                   ]

   def init_args(self, args, kwArgs): init_args(Thread, self, args, kwArgs)
   def __init__(self, *args, **kwArgs):
      self.init_args(list(args), kwArgs)

   @property
   def created(self): return lib.Thread_get_created(self.impl)

   def create(self):
      lib.Thread_create(self.impl)

   def kill(self):
      lib.Thread_kill(self.impl)

   def fn_unset_Thread_main(self):
      return lib.Thread_main(self.impl)

   @property
   def main(self):
      if hasattr(self, 'fn_Thread_main'): return self.fn_Thread_main
      else: return self.fn_unset_Thread_main
   @main.setter
   def main(self, value):
      self.fn_Thread_main = value
      lib.Instance_setMethod(self.impl, "Main".encode('u8'), cb_Thread_main)

   def setPriority(self, priority):
      lib.Thread_setPriority(self.impl, priority)

   def wait(self):
      lib.Thread_wait(self.impl)

class ThreadPriority:
   normal       = lib.ThreadPriority_normal
   aboveNormal  = lib.ThreadPriority_aboveNormal
   belowNormal  = lib.ThreadPriority_belowNormal
   highest      = lib.ThreadPriority_highest
   lowest       = lib.ThreadPriority_lowest
   idle         = lib.ThreadPriority_idle
   timeCritical = lib.ThreadPriority_timeCritical

def getCurrentThreadID():
   return lib.eC_getCurrentThreadID()
# hardcoded content
import math

def tan(x):
   if not isinstance(x, Angle): x = Angle(x)
   return math.tan(x.impl)
def sin(x):
   if not isinstance(x, Angle): x = Angle(x)
   return math.sin(x.impl)
def cos(x):
   if not isinstance(x, Angle): x = Angle(x)
   return math.cos(x.impl)

def atan(x):   return Angle(math.atan(x))
def sin(x):    return Angle(math.asin(x))
def cos(x):    return Angle(math.acos(x))

def log(x):    return math.log(x)
def log10(x):  return math.log10(x)
def pow(x, y): return math.pow(x, y)
def ceil(x):   return math.ceil(x)
def floor(x):  return math.floor(x)

# end of hardcoded content

def strnicmp(a, b, n):
   if isinstance(a, str): a = ffi.new("char[]", a.encode('u8'))
   elif isinstance(a, String): a = a.impl
   elif a is None: a = ffi.NULL

   if isinstance(b, str): b = ffi.new("char[]", b.encode('u8'))
   elif isinstance(b, String): b = b.impl
   elif b is None: b = ffi.NULL
   return lib.strnicmp(a, b, n)

def strcmpi(a, b):
   if isinstance(a, str): a = ffi.new("char[]", a.encode('u8'))
   elif isinstance(a, String): a = a.impl
   elif a is None: a = ffi.NULL

   if isinstance(b, str): b = ffi.new("char[]", b.encode('u8'))
   elif isinstance(b, String): b = b.impl
   elif b is None: b = ffi.NULL
   return lib.strncmpi(a, b)
