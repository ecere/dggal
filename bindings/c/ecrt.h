//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
////                                                                        //// ////////////////////////
////    Core eC Library                                                     //// ////////////////////////
////                                                                        //// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////


#if !defined(__ECRT_H__)
#define __ECRT_H__

#define CPP11 (defined(__cplusplus) && __cplusplus >= 201103L)

#ifdef __cplusplus

extern "C" {

#endif

////////////////////////////////////////////////////////////// includes //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>


#if !defined(ECRT_MODULE_NAME)
#define ECRT_MODULE_NAME "ecrt"
#endif

#if !defined(BINDINGS_SHARED)
#define LIB_EXPORT
#define LIB_IMPORT
#elif defined(__WIN32__)
#define LIB_EXPORT __attribute__((dllexport)) __attribute__ ((visibility("default")))
#define LIB_IMPORT __attribute__((dllimport))
#else
#define LIB_EXPORT __attribute__ ((visibility("default")))
#define LIB_IMPORT
#endif

#undef THIS_LIB_IMPORT
#ifdef ECRT_EXPORT
#define THIS_LIB_IMPORT LIB_EXPORT
#elif defined(BINDINGS_SHARED)
#define THIS_LIB_IMPORT LIB_IMPORT
#else
#define THIS_LIB_IMPORT
#endif



///////////////////////////////////////////////////////////// functions //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

#define getTranslatedString              __eCNameSpace__eC__i18n__GetTranslatedString
#define loadTranslatedStrings            __eCNameSpace__eC__i18n__LoadTranslatedStrings
#define unloadTranslatedStrings          __eCNameSpace__eC__i18n__UnloadTranslatedStrings
#define memoryGuard_popLoc               __eCNameSpace__eC__types__MemoryGuard_PopLoc
#define memoryGuard_pushLoc              __eCNameSpace__eC__types__MemoryGuard_PushLoc
#define Class_addBitMember               __eCNameSpace__eC__types__eClass_AddBitMember
#define Class_addClassProperty           __eCNameSpace__eC__types__eClass_AddClassProperty
#define Class_addDataMember              __eCNameSpace__eC__types__eClass_AddDataMember
#define Class_addMember                  __eCNameSpace__eC__types__eClass_AddMember
#define Class_addMethod                  __eCNameSpace__eC__types__eClass_AddMethod
#define Class_addProperty                __eCNameSpace__eC__types__eClass_AddProperty
#define Class_addTemplateParameter       __eCNameSpace__eC__types__eClass_AddTemplateParameter
#define Class_addVirtualMethod           __eCNameSpace__eC__types__eClass_AddVirtualMethod
#define Class_destructionWatchable       __eCNameSpace__eC__types__eClass_DestructionWatchable
#define Class_doneAddingTemplateParameters __eCNameSpace__eC__types__eClass_DoneAddingTemplateParameters
#define Class_findClassProperty          __eCNameSpace__eC__types__eClass_FindClassProperty
#define Class_findDataMember             __eCNameSpace__eC__types__eClass_FindDataMember
#define Class_findDataMemberAndId        __eCNameSpace__eC__types__eClass_FindDataMemberAndId
#define Class_findDataMemberAndOffset    __eCNameSpace__eC__types__eClass_FindDataMemberAndOffset
#define Class_findMethod                 __eCNameSpace__eC__types__eClass_FindMethod
#define Class_findNextMember             __eCNameSpace__eC__types__eClass_FindNextMember
#define Class_findProperty               __eCNameSpace__eC__types__eClass_FindProperty
#define Class_getDesigner                __eCNameSpace__eC__types__eClass_GetDesigner
#define Class_getProperty                __eCNameSpace__eC__types__eClass_GetProperty
#define Class_isDerived                  __eCNameSpace__eC__types__eClass_IsDerived
#define Class_resize                     __eCNameSpace__eC__types__eClass_Resize
#define Class_setProperty                __eCNameSpace__eC__types__eClass_SetProperty
#define Class_unregister                 __eCNameSpace__eC__types__eClass_Unregister
#define eC_initApp                       __eCNameSpace__eC__types__eCrt_Initialize
#define Enum_addFixedValue               __eCNameSpace__eC__types__eEnum_AddFixedValue
#define Enum_addValue                    __eCNameSpace__eC__types__eEnum_AddValue
#define Instance_decRef                  __eCNameSpace__eC__types__eInstance_DecRef
#define Instance_delete                  __eCNameSpace__eC__types__eInstance_Delete
#define Instance_evolve                  __eCNameSpace__eC__types__eInstance_Evolve
#define Instance_fireSelfWatchers        __eCNameSpace__eC__types__eInstance_FireSelfWatchers
#define Instance_fireWatchers            __eCNameSpace__eC__types__eInstance_FireWatchers
#define Instance_getDesigner             __eCNameSpace__eC__types__eInstance_GetDesigner
#define Instance_incRef                  __eCNameSpace__eC__types__eInstance_IncRef
#define Instance_isDerived               __eCNameSpace__eC__types__eInstance_IsDerived
#define Instance_new                     __eCNameSpace__eC__types__eInstance_New
#define Instance_newEx                   __eCNameSpace__eC__types__eInstance_NewEx
#define Instance_setMethod               __eCNameSpace__eC__types__eInstance_SetMethod
#define Instance_stopWatching            __eCNameSpace__eC__types__eInstance_StopWatching
#define Instance_watch                   __eCNameSpace__eC__types__eInstance_Watch
#define Instance_watchDestruction        __eCNameSpace__eC__types__eInstance_WatchDestruction
#define Member_addDataMember             __eCNameSpace__eC__types__eMember_AddDataMember
#define Member_addMember                 __eCNameSpace__eC__types__eMember_AddMember
#define Member_new                       __eCNameSpace__eC__types__eMember_New
#define Module_load                      __eCNameSpace__eC__types__eModule_Load
#define Module_loadStatic                __eCNameSpace__eC__types__eModule_LoadStatic
#define Module_loadStrict                __eCNameSpace__eC__types__eModule_LoadStrict
#define Module_unload                    __eCNameSpace__eC__types__eModule_Unload
#define Property_selfWatch               __eCNameSpace__eC__types__eProperty_SelfWatch
#define Property_watchable               __eCNameSpace__eC__types__eProperty_Watchable
#define eC_delete                        __eCNameSpace__eC__types__eSystem_Delete
#define eC_findClass                     __eCNameSpace__eC__types__eSystem_FindClass
#define eC_findDefine                    __eCNameSpace__eC__types__eSystem_FindDefine
#define eC_findFunction                  __eCNameSpace__eC__types__eSystem_FindFunction
#define eC_new                           __eCNameSpace__eC__types__eSystem_New
#define eC_new0                          __eCNameSpace__eC__types__eSystem_New0
#define eC_registerClass                 __eCNameSpace__eC__types__eSystem_RegisterClass
#define eC_registerDefine                __eCNameSpace__eC__types__eSystem_RegisterDefine
#define eC_registerFunction              __eCNameSpace__eC__types__eSystem_RegisterFunction
#define eC_renew                         __eCNameSpace__eC__types__eSystem_Renew
#define eC_renew0                        __eCNameSpace__eC__types__eSystem_Renew0
#define eC_setArgs                       __eCNameSpace__eC__types__eSystem_SetArgs
#define eC_setPoolingDisabled            __eCNameSpace__eC__types__eSystem_SetPoolingDisabled


//////////////////////////////////////////////////////// binding macros //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

#ifndef ECPRFX
#define ECPRFX
#endif

#if defined(_WIN32)
#define __runtimePlatform 1
#elif defined(__APPLE__)
#define __runtimePlatform 3
#else
#define __runtimePlatform 2
#endif

// Macros to select proper other macro with proper number of arguments
#define NARG_(...)                           ARGN(__VA_ARGS__)
#define NARG(...)                            NARG_(__VA_ARGS__, 6, 5, 4, 3, 2)
#define ARGN(_1, _2, _3, _4, _5, _6, x, ...) x

// CONCATI(): Two level of indirection so that the preprocessor resolve macros rather than concatenating the macro name
// _CONCAT(): One level of indirection for use by the var args macros
#define _CONCAT(a, b)                  a ## b
#define CONCATI(a, b)                  _CONCAT(a, b)

// Apply the ECPRFX to an eC class/type
#define C(x)                           CONCATI(ECPRFX, x)

// CONCATU(): Concatenate with underscores
#define _CONCATU_2(a, b)               a ## _ ## b
#define _CONCATU_3(a, b, c)            a ## _ ## b ## _ ## c
#define _CONCATU_4(a, b, c, d)         a ## _ ## b ## _ ## c ## _ ## d
#define _CONCATU_5(a, b, c, d, e)      a ## _ ## b ## _ ## c ## _ ## d ## _ ## e
#define _CONCATU_6(a, b, c, d, e, f)   a ## _ ## b ## _ ## c ## _ ## d ## _ ## e ## _ ## f
#define _CONCATUN(n, ...)              _CONCAT(_CONCATU_,n)(__VA_ARGS__)
#define CONCATU(...)                   _CONCATUN(NARG(__VA_ARGS__), __VA_ARGS__)

// T(): Used to typedef a templatized class to its template class, so as to clarify expectations (note: typedef is equivalent, cannot be type-checked)
//      e.g. typedef C(Array) T(Array, Color); void printColors(T(Array, Color) colors) { printLn(colors); };
#define T(n...)                        CONCATU(template, n)

// CO(): Used to access or define a first-class Class object
//       Classes already used in modules are predefined and can be directly accesses and instantiated
//          e.g. C(Window) win = newi(Window);
//       New classes can be defined and initialized with return value from registerClass();
//          e.g. Class * CO(Foo); CO(Foo) = registerClass(module, Foo, Map<String, int>);
//       Templatized classes can be defined and initialized with eC_findClass()
//          e.g. C(Class) * CO(Array, ColorAlpha); CO(Array, ColorAlpha) = eC_findClass(module, "Array<ColorAlpha>");
#define CO(n...)                       CONCATU(class, n)

/* CM(): Used to define/access the class member structure for regular eC classes at a specific level of inheritance
         e.g.
            struct CM(Foo) { int foo; };
            typedef Instance Foo;
            C(Class) * CO(Foo);
            bool Foo_constructor(Foo this)
            {
               struct CM(Foo) * self = IPTR(this, Foo);
               return true;
            }
            bool Foo_destructor(Foo this) { }
            ...
               CO(Foo) = registerClass(module, Foo, Instance);
            ...
*/
#define CM(c)                          CONCATU(class_members, c)

// S(c: class name, x...: for all the initializers):
//       Used to quickly pass structs by reference
#define S(c, x...) ({c __i = { x }; &__i;})

// TP(n: class name, p: template parameter name):
//       Used to specify member method types with a template parameter of a class
//       e.g.  void Container_add(C(Container) c, TP(Container, T) value);
#define TP(n, p)                       CONCATU(tparam, n, p)

// template argument macros type abbreviations:
//          c: char
//          uc: unsigned char (byte)
//          s: short,
//          us: unsigned short (uint16)
//          i: int
//          ui: uint
//          i64: int64
//          ui64: uint64
//          f: float
//          d: double
//          p: pointer
//          st: struct
//          o: object

// TA...(): Used to pass data values as template arguments
//          e.g. Container_add(ad, TAd(3.14159));

#define TAc(x)     ({ C(DataValue) p = { .c = x };    p.ui64; })
#define TAb(x)     ({ C(DataValue) p = { .uc = x };   p.ui64; })
#define TAs(x)     ({ C(DataValue) p = { .s = x };    p.ui64; })
#define TAus(x)    ({ C(DataValue) p = { .us = x };   p.ui64; })
#define TAi(x)     ({ C(DataValue) p = { .i = x };    p.ui64; })
#define TAui(x)    ({ C(DataValue) p = { .ui = x };   p.ui64; })
#define TAi64(x)   ({ C(DataValue) p = { .i64 = x };  p.ui64; })
#define TAui64(x)  (x)
#define TAf(x)     ({ C(DataValue) p = { .f = x };    p.ui64; })
#define TAd(x)     ({ C(DataValue) p = { .d = x };    p.ui64; })
#define TAp(x)     ({ C(DataValue) p = { .p = x };    p.ui64; })
#define TAst(x)    TAp(&(x))
#define TAo(x)     TAp(x)

// ...TA(): Used to retreieve data values from template arguments
//          e.g. void Foo_add(Foo this, TP(Foo, A) value) { double d = dTA(value); }

#define cTA(x)     ({ C(DataValue) p = { .ui64 = x }; p.c; })
#define bTA(x)     ({ C(DataValue) p = { .ui64 = x }; p.uc; })
#define sTA(x)     ({ C(DataValue) p = { .ui64 = x }; p.s; })
#define usTA(x)    ({ C(DataValue) p = { .ui64 = x }; p.us; })
#define iTA(x)     ({ C(DataValue) p = { .ui64 = x }; p.i; })
#define uiTA(x)    ({ C(DataValue) p = { .ui64 = x }; p.ui; })
#define i64TA(x)   ({ C(DataValue) p = { .ui64 = x }; p.i64; })
#define ui64TA(x)  (x)
#define fTA(x)     ({ C(DataValue) p = { .ui64 = x }; p.f; })
#define dTA(x)     ({ C(DataValue) p = { .ui64 = x }; p.d; })
#define pTA(t, x)  ({ C(DataValue) p = { .ui64 = x }; (t *)p.p; })
#define stTA(s, x) ({ C(DataValue) p = { .ui64 = x }; *(s *)p.p; })
#define oTA(c, x)  ((C(c)) pTA(struct CM(Instance), x))

#define oTAInstance(x)  oTA(Instance, x)
#define pTAvoid(x)      pTA(void, x)

// thisclass(): Used to specify an eC 'thisclass' type, which morphs into the derived class type when inherited
//              Note: Resolves to the same type so cannot be type checked
#define thisclass(x)                   C(x)
#define subclass(x)                    C(Class) *

// ECPRFX for eC global functions
#define F(x)                           CONCATI(ECPRFX, x)

// prefixes/suffices for reflection objects/values
#define PROPERTY(c, p)   CONCATU(property, c, p)
#define METHOD(c, m)     CONCATU(method, c, m)
#define M_VTBLID(c, m)   CONCATU(c, m, vTblID)
#define FUNCTION(f)      CONCATU(function, f)

// _ARG: Used to specify comma for parameters/arguments list being passed to macros
#define _ARG ,
#define _PRN_OPN (
#define _PRN_CLS )

// VMETHOD(c: class object, mc: method class, m: method name, i: instance object, r: return type, p: parameters, a: arguments)
//    Used to define a virtual method invoker, e.g.:
//    #define Container_add(__i, value) VMETHOD(Container, add, __i, C(IteratorPointer), C(Container) _ARG TP(Container, T), __i _ARG value)
#define VMETHOD(c, mc, m, i, r, p, a)   ({ r (* method)(p) = (r (*)(p))(((i) && (i)->_vTbl) ? (i)->_vTbl : (c)->_vTbl)[M_VTBLID(mc, m)]; method ? method(a) : (r)0; })
#define SVMETHOD(c, mc, m, i, r, p, a)   ({ r (* method)(p) = (r (*)(p))((c)->_vTbl)[M_VTBLID(mc, m)]; method ? method(a) : (r)0; })

// Macros for translated strings
#define $(x)      (constString)getTranslatedString(MODULE_NAME, x, null)
#define $$(x, c)  (constString)getTranslatedString(MODULE_NAME, x, c)

// More macros
#define IPTR(x, c) ((struct CM(c) *)((x) ? (((char *)(x)) + CO(c)->offset) : 0))

#define incref(x) (x)->_refCount++;

#define newb(c) eC_new(C(c))
#define newb0(c) eC_new0(C(c))
#define newi(c...) ({ C(Instance) i = Instance_new(CO(c)); incref(i); i; })
#define newcs(c...) ((C(c) *)Instance_new(CO(c)))
#define newc(t, c) (C(t) *)eC_new(CO(t)->structSize * c)
#define newc0(t, c) (C(t) *)eC_new0(CO(t)->structSize * c)

#define renewb(p, c) eC_renew(p, c)
#define renewb0(p, c) eC_renew0(p, c)
#define renewc(t, p, c) (C(t) *)eC_renew(p, CO(t)->structSize * c)
#define renewc0(t, p, c) (C(t) *)eC_renew0(p, CO(t)->structSize * c)

#define newt(t, c) (t *)eC_new(sizeof(t) * c)
#define newt0(t, c) (t *)eC_new0(sizeof(t) * c)

#define renewt(t, p, c) (t *)eC_renew(p, sizeof(t) * c)
#define renewt0(t, p, c) (t *)eC_renew0(p, sizeof(t) * c)

#define deletei(v) Instance_decRef(v), v = null
#ifndef __cplusplus
   #define delete(v) eC_delete(v), v = null
#endif

#define BIC(c, d) \
   ({ struct C(BuiltInContainer) src = { \
      CO(BuiltInContainer)->_vTbl, CO(BuiltInContainer), 0, \
      d, sizeof(d) / sizeof((d)[0]), CO(c) }; \
   (C(Container))&src; })

#ifndef __cplusplus
   #define registerClass(a, n, b...)  \
      eC_registerClass(ClassType_normalClass, #n, #b, sizeof(struct CM(n)), 0, \
         (void *)n ## _constructor, (void *)n ## _destructor, a, AccessMode_publicAccess, AccessMode_publicAccess)
   #define registerClassEx(a, constructor, destructor, n, b...)  \
      eC_registerClass(ClassType_normalClass, #n, #b, sizeof(struct CM(n)), 0, \
         (void *)constructor, (void *)destructor, a, AccessMode_publicAccess, AccessMode_publicAccess)
#endif

#define addMethod(c, n, m) \
   Class_addMethod(c, n, null, (void *)m, AccessMode_publicAccess)

#if defined(__WIN32__) && !defined(__CONSOLE_APP__)
   typedef void * HINSTANCE;
   #define WINAPI __stdcall
   #define MAIN_DECLARATION int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, char * cmdLine, int show)
   #define ecrt_init_CALL(linkEcere) ecrt_init(null, linkEcere, true, 0, null)
#else
   #define MAIN_DECLARATION int main(int argc, char * argv[])
   #ifdef __cplusplus
      #if defined(__CONSOLE_APP__)
         #define ecrt_init_CALL(linkEcere) ecrt_init(null, linkEcere, false, null, null)
      #else
         #define ecrt_init_CALL(linkEcere) ecrt_init(null, linkEcere, true, null, null)
      #endif
   #else
      #if defined(__CONSOLE_APP__)
         #define ecrt_init_CALL(linkEcere) ecrt_init(null, linkEcere, false, argc, argv)
      #else
         #define ecrt_init_CALL(linkEcere) ecrt_init(null, linkEcere, true, argc, argv)
      #endif
   #endif
#endif

#define APP_INTRO(linkEcere) \
   MAIN_DECLARATION \
   { \
      C(Application) app = ecrt_init_CALL(linkEcere);

#define APP_OUTRO \
      int exitCode = IPTR(app, Application)->exitCode; \
      deletei(app); \
      return exitCode; \
   }

#if defined(__WIN32__)
   #define MODULE_EXPORT extern __attribute__((dllexport))
#else
   #define MODULE_EXPORT extern __attribute__ ((visibility("default")))
#endif

#if defined(__WIN32__)
   #define MODULE_EXPORT extern __attribute__((dllexport))
   #define stdcall       __stdcall
#else
   #define MODULE_EXPORT extern __attribute__ ((visibility("default")))
   #define stdcall
#endif

#define MODULE_LOAD     MODULE_EXPORT uint stdcall __eCDll_Load(C(Module) module)

#define MODULE_UNLOAD   MODULE_EXPORT uint stdcall __eCDll_Unload(C(Module) module)

///////////////////////////////////////////////////////////// HARDCODED //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

// special types
typedef const void * any_object;
typedef const char * constString;

// ecere::gui stuff that bleeds into ecrt
#if CPP11
enum C(Alignment) : int
#else
typedef int C(Alignment);
enum C(Alignment)
#endif
{
   Alignment_left = 0x0,
   Alignment_right = 0x1,
   Alignment_center = 0x2
};
typedef uint32_t C(DataDisplayFlags);

// libec stuff
typedef void C(Type);
typedef void C(Instantiation);
typedef void C(ClassDefinition);

typedef struct CM(Instance) * C(Instance);
typedef C(Instance) C(Surface);
typedef C(Instance) C(Window);
typedef C(Window) C(CommonControl);
typedef C(CommonControl) C(DataBox);
typedef C(CommonControl) C(EditBox);

typedef int C(MinMaxValue);
struct C(Size)
{
   C(MinMaxValue) w;
   C(MinMaxValue) h;
};
typedef struct C(Size) C(Size);

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||types  /////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef struct C(BTNode) C(BTNode);
typedef uintptr_t uintptr;
struct C(BTNode)
{
   uintptr key;
   C(BTNode) * parent;
   C(BTNode) * left;
   C(BTNode) * right;
   int depth;
};
typedef struct C(BinaryTree) C(BinaryTree);
struct C(BinaryTree)
{
   C(BTNode) * root;
   int count;
   int (* CompareKey)(C(BinaryTree) * tree, uintptr a, uintptr b);
   void (* FreeKey)(void * key);
};
typedef struct C(NameSpace) C(NameSpace);
struct C(NameSpace)
{
   const char * name;
   C(NameSpace) * btParent;
   C(NameSpace) * left;
   C(NameSpace) * right;
   int depth;
   C(NameSpace) * parent;
   C(BinaryTree) nameSpaces;
   C(BinaryTree) classes;
   C(BinaryTree) defines;
   C(BinaryTree) functions;
};
typedef uint32_t uint;
typedef uint C(bool);
#if !defined(__bool_true_false_are_defined) && !defined(__cplusplus)
enum boolean {
   true = 0x1,
   false = 0x0
};
#endif
#define eC_true   ((C(bool))1)
#define eC_false  ((C(bool))0)

struct C(OldList)
{
   void * first;
   void * last;
   int count;
   uint offset;
   C(bool) circ;
};
typedef struct CM(Instance) * C(Instance);
typedef C(Instance) C(Module);
typedef C(Module) C(Application);
#if CPP11
enum C(ImportType) : int
#else
typedef int C(ImportType);
enum C(ImportType)
#endif
{
   ImportType_normalImport = 0x0,
   ImportType_staticImport = 0x1,
   ImportType_remoteImport = 0x2,
   ImportType_preDeclImport = 0x3,
   ImportType_comCheckImport = 0x4
};

typedef struct C(OldList) C(OldList);
struct CM(Module)
{
   C(Application) application;
   C(OldList) classes;
   C(OldList) defines;
   C(OldList) functions;
   C(OldList) modules;
   C(Module) prev;
   C(Module) next;
   const char * name;
   void * library;
   void * Unload;
   C(ImportType) importType;
   C(ImportType) origImportType;
   C(NameSpace) privateNameSpace;
   C(NameSpace) publicNameSpace;
};
#if CPP11
enum C(AccessMode) : int
#else
typedef int C(AccessMode);
enum C(AccessMode)
#endif
{
   AccessMode_defaultAccess = 0x0,
   AccessMode_publicAccess = 0x1,
   AccessMode_privateAccess = 0x2,
   AccessMode_staticAccess = 0x3,
   AccessMode_baseSystemAccess = 0x4
};

typedef struct C(Class) C(Class);
#ifdef __cplusplus
typedef void * typed_object_class_ptr;
#else
typedef C(Class) * typed_object_class_ptr;
#endif
typedef struct C(ClassTemplateArgument) C(ClassTemplateArgument);
#if CPP11
enum C(ClassType) : int
#else
typedef int C(ClassType);
enum C(ClassType)
#endif
{
   ClassType_normalClass = 0x0,
   ClassType_structClass = 0x1,
   ClassType_bitClass = 0x2,
   ClassType_unitClass = 0x3,
   ClassType_enumClass = 0x4,
   ClassType_noHeadClass = 0x5,
   ClassType_unionClass = 0x6,
   ClassType_systemClass = 0x3E8
};

struct C(Class)
{
   C(Class) * prev;
   C(Class) * next;
   const char * name;
   int offset;
   int structSize;
   void ** _vTbl;
   int vTblSize;
   C(bool) (* Constructor)(void *);
   void (* Destructor)(void *);
   int offsetClass;
   int sizeClass;
   C(Class) * base;
   C(BinaryTree) methods;
   C(BinaryTree) members;
   C(BinaryTree) prop;
   C(OldList) membersAndProperties;
   C(BinaryTree) classProperties;
   C(OldList) derivatives;
   int memberID;
   int startMemberID;
   C(ClassType) type;
   C(Module) module;
   C(NameSpace) * nameSpace;
   const char * dataTypeString;
   C(Type) * dataType;
   int typeSize;
   int defaultAlignment;
   void (* Initialize)(void);
   int memberOffset;
   C(OldList) selfWatchers;
   const char * designerClass;
   C(bool) noExpansion;
   const char * defaultProperty;
   C(bool) comRedefinition;
   int count;
   int isRemote;
   C(bool) internalDecl;
   void * data;
   C(bool) computeSize;
   short structAlignment;
   short pointerAlignment;
   int destructionWatchOffset;
   C(bool) fixed;
   C(OldList) delayedCPValues;
   C(AccessMode) inheritanceAccess;
   const char * fullName;
   void * symbol;
   C(OldList) conversions;
   C(OldList) templateParams;
   C(ClassTemplateArgument) * templateArgs;
   C(Class) * templateClass;
   C(OldList) templatized;
   int numParams;
   C(bool) isInstanceClass;
   C(bool) byValueSystemClass;
   void * bindingsClass;
};
typedef struct C(Method) C(Method);
// end -- moved backwards outputs
struct CM(Application)
{
   int argc;
   const char ** argv;
   int exitCode;
   C(bool) isGUIApp;
   C(OldList) allModules;
   char * parsedCommand;
   C(NameSpace) systemNameSpace;
};
extern THIS_LIB_IMPORT int M_VTBLID(Application, main);
// void Application_main(C(Application) __i);
#define Application_main(__i) \
   VMETHOD(CO(Application), Application, main, __i, void, \
      C(Application), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Application, main);

struct CM(Instance)
{
   void ** _vTbl;
   C(Class) * _class;
   int _refCount;
};

extern THIS_LIB_IMPORT int M_VTBLID(Module, onLoad);
// C(bool) Module_onLoad(C(Module) __i);
#define Module_onLoad(__i) \
   VMETHOD(CO(Module), Module, onLoad, __i, C(bool), \
      C(Module), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Module, onLoad);

extern THIS_LIB_IMPORT int M_VTBLID(Module, onUnload);
// void Module_onUnload(C(Module) __i);
#define Module_onUnload(__i) \
   VMETHOD(CO(Module), Module, onUnload, __i, void, \
      C(Module), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Module, onUnload);



// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||types  /////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef struct C(BTNamedLink) C(BTNamedLink);
struct C(BTNamedLink)
{
   const char * name;
   C(BTNamedLink) * parent;
   C(BTNamedLink) * left;
   C(BTNamedLink) * right;
   int depth;
   void * data;
};
typedef struct C(BitMember) C(BitMember);
#if CPP11
enum C(DataMemberType) : int
#else
typedef int C(DataMemberType);
enum C(DataMemberType)
#endif
{
   DataMemberType_normalMember = 0x0,
   DataMemberType_unionMember = 0x1,
   DataMemberType_structMember = 0x2
};

typedef uint64_t uint64;
struct C(BitMember)
{
   C(BitMember) * prev;
   C(BitMember) * next;
   const char * name;
   C(bool) isProperty;
   C(AccessMode) memberAccess;
   int id;
   C(Class) * _class;
   const char * dataTypeString;
   C(Class) * dataTypeClass;
   C(Type) * dataType;
   C(DataMemberType) type;
   int size;
   int pos;
   uint64 mask;
};
typedef struct C(ClassProperty) C(ClassProperty);
typedef int64_t int64;
struct C(ClassProperty)
{
   const char * name;
   C(ClassProperty) * parent;
   C(ClassProperty) * left;
   C(ClassProperty) * right;
   int depth;
   void (* Set)(C(Class) *, int64);
   int64 (* Get)(C(Class) *);
   const char * dataTypeString;
   C(Type) * dataType;
   C(bool) constant;
};
typedef struct C(DataMember) C(DataMember);
struct C(DataMember)
{
   C(DataMember) * prev;
   C(DataMember) * next;
   const char * name;
   C(bool) isProperty;
   C(AccessMode) memberAccess;
   int id;
   C(Class) * _class;
   const char * dataTypeString;
   C(Class) * dataTypeClass;
   C(Type) * dataType;
   C(DataMemberType) type;
   int offset;
   int memberID;
   C(OldList) members;
   C(BinaryTree) membersAlpha;
   int memberOffset;
   short structAlignment;
   short pointerAlignment;
};
typedef uint8_t byte;
typedef uint16_t uint16;
struct C(DataValue)
{
   union
   {
      char c;
      byte uc;
      short s;
      uint16 us;
      int i;
      uint ui;
      void * p;
      float f;
      double d;
      int64 i64;
      uint64 ui64;
   };
};
#if CPP11
enum C(MethodType) : int
#else
typedef int C(MethodType);
enum C(MethodType)
#endif
{
   MethodType_normalMethod = 0x0,
   MethodType_virtualMethod = 0x1
};

struct C(Method)
{
   const char * name;
   C(Method) * parent;
   C(Method) * left;
   C(Method) * right;
   int depth;
   int (* function)(void);
   int vid;
   C(MethodType) type;
   C(Class) * _class;
   void * symbol;
   const char * dataTypeString;
   C(Type) * dataType;
   C(AccessMode) memberAccess;
};
typedef struct C(Property) C(Property);
struct C(Property)
{
   C(Property) * prev;
   C(Property) * next;
   const char * name;
   C(bool) isProperty;
   C(AccessMode) memberAccess;
   int id;
   C(Class) * _class;
   const char * dataTypeString;
   C(Class) * dataTypeClass;
   C(Type) * dataType;
   void (* Set)(void *, int);
   int (* Get)(void *);
   C(bool) (* IsSet)(void *);
   void * data;
   void * symbol;
   int vid;
   C(bool) conversion;
   uint watcherOffset;
   const char * category;
   C(bool) compiled;
   C(bool) selfWatchable;
   C(bool) isWatchable;
};
typedef struct C(DataValue) C(DataValue);
struct C(ClassTemplateArgument)
{
   union
   {
      struct
      {
         const char * dataTypeString;
         C(Class) * dataTypeClass;
      };
      C(DataValue) expression;
      struct
      {
         const char * memberString;
         union
         {
            C(DataMember) * member;
            C(Property) * prop;
            C(Method) * method;
         };
      };
   };
};
typedef struct C(ClassTemplateParameter) C(ClassTemplateParameter);
#if CPP11
enum C(TemplateMemberType) : int
#else
typedef int C(TemplateMemberType);
enum C(TemplateMemberType)
#endif
{
   TemplateMemberType_dataMember = 0x0,
   TemplateMemberType_method = 0x1,
   TemplateMemberType_prop = 0x2
};

#if CPP11
enum C(TemplateParameterType) : int
#else
typedef int C(TemplateParameterType);
enum C(TemplateParameterType)
#endif
{
   TemplateParameterType_type = 0x0,
   TemplateParameterType_identifier = 0x1,
   TemplateParameterType_expression = 0x2
};

struct C(ClassTemplateParameter)
{
   C(ClassTemplateParameter) * prev;
   C(ClassTemplateParameter) * next;
   const char * name;
   C(TemplateParameterType) type;
   union
   {
      const char * dataTypeString;
      C(TemplateMemberType) memberType;
   };
   C(ClassTemplateArgument) defaultArg;
   void * param;
};
typedef struct C(DefinedExpression) C(DefinedExpression);
struct C(DefinedExpression)
{
   C(DefinedExpression) * prev;
   C(DefinedExpression) * next;
   const char * name;
   const char * value;
   C(NameSpace) * nameSpace;
};
typedef struct C(GlobalFunction) C(GlobalFunction);
struct C(GlobalFunction)
{
   C(GlobalFunction) * prev;
   C(GlobalFunction) * next;
   const char * name;
   int (* function)(void);
   C(Module) module;
   C(NameSpace) * nameSpace;
   const char * dataTypeString;
   C(Type) * dataType;
   void * symbol;
};
typedef struct C(ObjectInfo) C(ObjectInfo);
struct C(ObjectInfo)
{
   C(ObjectInfo) * prev;
   C(ObjectInfo) * next;
   C(Instance) instance;
   char * name;
   C(Instantiation) * instCode;
   C(bool) deleted;
   C(ObjectInfo) * oClass;
   C(OldList) instances;
   C(ClassDefinition) * classDefinition;
   C(bool) modified;
   void * i18nStrings;
};
typedef struct C(SubModule) C(SubModule);
struct C(SubModule)
{
   C(SubModule) * prev;
   C(SubModule) * next;
   C(Module) module;
   C(AccessMode) importMode;
};
#if CPP11
enum C(BackSlashEscaping) : uint
#else
typedef C(bool) C(BackSlashEscaping);
enum C(BackSlashEscaping)
#endif
{
   BackSlashEscaping_forArgsPassing = 0x2
};

typedef struct C(Box) C(Box);
typedef C(Instance) C(ClassDesignerBase);
typedef C(Instance) C(DesignerBase);
typedef uint32_t uint32;
typedef uint32 C(EscapeCStringOptions);
typedef C(Instance) C(IOChannel);
typedef int C(MinMaxValue);
#if CPP11
enum C(Platform) : int
#else
typedef int C(Platform);
enum C(Platform)
#endif
{
   Platform_unknown = 0x0,
   Platform_win32 = 0x1,
   Platform_tux = 0x2,
   Platform_apple = 0x3
};

typedef struct C(Point) C(Point);
typedef C(IOChannel) C(SerialBuffer);
typedef struct C(Size) C(Size);
typedef char * C(String);
#if CPP11
enum C(StringAllocType) : int
#else
typedef int C(StringAllocType);
enum C(StringAllocType)
#endif
{
   StringAllocType_pointer = 0x0,
   StringAllocType_stack = 0x1,
   StringAllocType_heap = 0x2
};

typedef C(Instance) C(ZString);
typedef size_t uintsize;
// end -- moved backwards outputs
#define DIR_SEP ((__runtimePlatform == Platform_win32) ? '\\' : '/')

#define DIR_SEPS ((__runtimePlatform == Platform_win32) ? "\\" : "/")

#define FORMAT64D ((__runtimePlatform == Platform_win32) ? "%I64d" : "%lld")

#define FORMAT64DLL ((__runtimePlatform == Platform_win32) ? "%I64dLL" : "%lldLL")

#define FORMAT64HEX ((__runtimePlatform == Platform_win32) ? "0x%I64X" : "0x%llX")

#define FORMAT64HEXLL ((__runtimePlatform == Platform_win32) ? "0x%I64XLL" : "0x%llXLL")

#define FORMAT64U ((__runtimePlatform == Platform_win32) ? "%I64u" : "%llu")

#define MAXBYTE (0xff)

#define MAXDOUBLE (1.7976931348623158e+308)

#define MAXDWORD (0xffffffff)

#ifndef MAXFLOAT
#define MAXFLOAT (3.40282346638528860e+38f)
#endif

#define MAXINT (((int)0x7fffffff))

#define MAXINT64 (((long long)0x7fffffffffffffffLL))

#define MAXQWORD (0xffffffffffffffffLL)

#define MAXWORD (0xffff)

#define MAX_DIRECTORY (534)

#define MAX_EXTENSION (17)

#define MAX_FILENAME (274)

#define MAX_F_STRING (1025)

#define MAX_LOCATION (797)

#define MINDOUBLE (2.2250738585072014e-308)

#define MINFLOAT (1.17549435082228750e-38f)

#define MININT (((int)0x80000000))

#define MININT64 (((long long)0x8000000000000000LL))

#define Pi (((C(Angle))(3.1415926535897932384626433832795028841971)))

#if defined(__cplusplus)
#define null 0
#else
#define null ((void *)0)
#endif

typedef double C(Angle);
typedef char * C(CIString);
typedef double C(Distance);
typedef struct C(EnumClassData) C(EnumClassData);
#if CPP11
enum C(ObjectNotationType) : uint
#else
typedef C(bool) C(ObjectNotationType);
enum C(ObjectNotationType)
#endif
{
   ObjectNotationType_none = 0x0,
   ObjectNotationType_econ = 0x1,
   ObjectNotationType_json = 0x2
};

typedef struct C(Pointd) C(Pointd);
typedef struct C(Pointf) C(Pointf);
typedef struct C(StaticString) C(StaticString);
struct C(Box)
{
   int left;
   int top;
   int right;
   int bottom;
};
extern THIS_LIB_IMPORT void (* Box_clip)(C(Box) * __this, C(Box) * against);

extern THIS_LIB_IMPORT void (* Box_clipOffset)(C(Box) * __this, C(Box) * against, int x, int y);

extern THIS_LIB_IMPORT C(bool) (* Box_isPointInside)(C(Box) * __this, C(Point) * point);

extern THIS_LIB_IMPORT C(bool) (* Box_overlap)(C(Box) * __this, C(Box) * box);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Box, width);
extern THIS_LIB_IMPORT void (* Box_set_width)(const C(Box) * b, int value);
extern THIS_LIB_IMPORT int (* Box_get_width)(const C(Box) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Box, height);
extern THIS_LIB_IMPORT void (* Box_set_height)(const C(Box) * b, int value);
extern THIS_LIB_IMPORT int (* Box_get_height)(const C(Box) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Centimeters, Meters);
extern THIS_LIB_IMPORT double (* Centimeters_from_Meters)(const C(Distance) meters);
extern THIS_LIB_IMPORT C(Distance) (* Centimeters_to_Meters)(const double centimeters);
#define Centimeters(x) CENTIMETERS(x)
#define CENTIMETERS(x)  ((C(Distance))((x) * 0.01))
#define Distance_in_Centimeters(x)  ((double)((x) * 100))

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Class, char_ptr);
extern THIS_LIB_IMPORT void (* Class_from_char_ptr)(const C(Class) * c, const char * value);
extern THIS_LIB_IMPORT const char * (* Class_to_char_ptr)(const C(Class) * c);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, addObject);
// void ClassDesignerBase_addObject(C(ClassDesignerBase) __i);
#define ClassDesignerBase_addObject(__i) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, addObject, __i, void, \
      C(ClassDesignerBase), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, addObject);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, createNew);
// void ClassDesignerBase_createNew(C(ClassDesignerBase) __i, C(Instance) editBox, C(Size) * clientSize, const char * name, const char * inherit);
#define ClassDesignerBase_createNew(__i, editBox, clientSize, name, inherit) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, createNew, __i, void, \
      C(Instance) _ARG C(Size) * _ARG const char * _ARG const char *, \
      editBox _ARG clientSize _ARG name _ARG inherit)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, createNew);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, createObject);
// void ClassDesignerBase_createObject(C(ClassDesignerBase) __i, C(DesignerBase) designer, C(Instance) instance, C(ObjectInfo) * object, C(bool) isClass, C(Instance) _class);
#define ClassDesignerBase_createObject(__i, designer, instance, object, isClass, _class) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, createObject, __i, void, \
      C(DesignerBase) _ARG C(Instance) _ARG C(ObjectInfo) * _ARG C(bool) _ARG C(Instance), \
      designer _ARG instance _ARG object _ARG isClass _ARG _class)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, createObject);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, destroyObject);
// void ClassDesignerBase_destroyObject(C(ClassDesignerBase) __i, C(Instance) object);
#define ClassDesignerBase_destroyObject(__i, object) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, destroyObject, __i, void, \
      C(Instance), \
      object)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, destroyObject);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, droppedObject);
// void ClassDesignerBase_droppedObject(C(ClassDesignerBase) __i, C(Instance) instance, C(ObjectInfo) * object, C(bool) isClass, C(Instance) _class);
#define ClassDesignerBase_droppedObject(__i, instance, object, isClass, _class) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, droppedObject, __i, void, \
      C(Instance) _ARG C(ObjectInfo) * _ARG C(bool) _ARG C(Instance), \
      instance _ARG object _ARG isClass _ARG _class)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, droppedObject);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, fixProperty);
// void ClassDesignerBase_fixProperty(C(ClassDesignerBase) __i, C(Property) * prop, C(Instance) object);
#define ClassDesignerBase_fixProperty(__i, prop, object) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, fixProperty, __i, void, \
      C(Property) * _ARG C(Instance), \
      prop _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, fixProperty);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, listToolBoxClasses);
// void ClassDesignerBase_listToolBoxClasses(C(ClassDesignerBase) __i, C(DesignerBase) designer);
#define ClassDesignerBase_listToolBoxClasses(__i, designer) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, listToolBoxClasses, __i, void, \
      C(ClassDesignerBase) _ARG C(DesignerBase), \
      __i _ARG designer)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, listToolBoxClasses);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, postCreateObject);
// void ClassDesignerBase_postCreateObject(C(ClassDesignerBase) __i, C(Instance) instance, C(ObjectInfo) * object, C(bool) isClass, C(Instance) _class);
#define ClassDesignerBase_postCreateObject(__i, instance, object, isClass, _class) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, postCreateObject, __i, void, \
      C(Instance) _ARG C(ObjectInfo) * _ARG C(bool) _ARG C(Instance), \
      instance _ARG object _ARG isClass _ARG _class)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, postCreateObject);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, prepareTestObject);
// void ClassDesignerBase_prepareTestObject(C(ClassDesignerBase) __i, C(DesignerBase) designer, C(Instance) test);
#define ClassDesignerBase_prepareTestObject(__i, designer, test) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, prepareTestObject, __i, void, \
      C(DesignerBase) _ARG C(Instance), \
      designer _ARG test)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, prepareTestObject);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, reset);
// void ClassDesignerBase_reset(C(ClassDesignerBase) __i);
#define ClassDesignerBase_reset(__i) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, reset, __i, void, \
      C(ClassDesignerBase), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, reset);

extern THIS_LIB_IMPORT int M_VTBLID(ClassDesignerBase, selectObject);
// void ClassDesignerBase_selectObject(C(ClassDesignerBase) __i, C(ObjectInfo) * object, C(Instance) control);
#define ClassDesignerBase_selectObject(__i, object, control) \
   VMETHOD(CO(ClassDesignerBase), ClassDesignerBase, selectObject, __i, void, \
      C(ClassDesignerBase) _ARG C(ObjectInfo) * _ARG C(Instance), \
      __i _ARG object _ARG control)
extern THIS_LIB_IMPORT C(Method) * METHOD(ClassDesignerBase, selectObject);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Degrees, Radians);
extern THIS_LIB_IMPORT double (* Degrees_from_Radians)(const C(Angle) radians);
extern THIS_LIB_IMPORT C(Angle) (* Degrees_to_Radians)(const double degrees);
#define Degrees(x) DEGREES(x)
#define DEGREES(x)  ((C(Angle))((x) * 0.0174532925199433))
#define Angle_in_Degrees(x)  ((double)((x) * 57.2957795130823))

struct CM(DesignerBase)
{
   C(ClassDesignerBase) classDesigner;
   const char * objectClass;
   C(bool) isDragging;
};
extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, addDefaultMethod);
// void DesignerBase_addDefaultMethod(C(DesignerBase) __i, C(Instance) instance, C(Instance) classInstance);
#define DesignerBase_addDefaultMethod(__i, instance, classInstance) \
   VMETHOD(CO(DesignerBase), DesignerBase, addDefaultMethod, __i, void, \
      C(DesignerBase) _ARG C(Instance) _ARG C(Instance), \
      __i _ARG instance _ARG classInstance)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, addDefaultMethod);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, addToolBoxClass);
// void DesignerBase_addToolBoxClass(C(DesignerBase) __i, C(Class) * _class);
#define DesignerBase_addToolBoxClass(__i, _class) \
   VMETHOD(CO(DesignerBase), DesignerBase, addToolBoxClass, __i, void, \
      C(DesignerBase) _ARG C(Class) *, \
      __i _ARG _class)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, addToolBoxClass);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, codeAddObject);
// void DesignerBase_codeAddObject(C(DesignerBase) __i, C(Instance) instance, C(ObjectInfo) * object);
#define DesignerBase_codeAddObject(__i, instance, object) \
   VMETHOD(CO(DesignerBase), DesignerBase, codeAddObject, __i, void, \
      C(DesignerBase) _ARG C(Instance) _ARG C(ObjectInfo) *, \
      __i _ARG instance _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, codeAddObject);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, deleteObject);
// void DesignerBase_deleteObject(C(DesignerBase) __i, C(ObjectInfo) * object);
#define DesignerBase_deleteObject(__i, object) \
   VMETHOD(CO(DesignerBase), DesignerBase, deleteObject, __i, void, \
      C(DesignerBase) _ARG C(ObjectInfo) *, \
      __i _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, deleteObject);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, findObject);
// C(bool) DesignerBase_findObject(C(DesignerBase) __i, C(Instance) * instance, const char * string);
#define DesignerBase_findObject(__i, instance, string) \
   VMETHOD(CO(DesignerBase), DesignerBase, findObject, __i, C(bool), \
      C(DesignerBase) _ARG C(Instance) * _ARG const char *, \
      __i _ARG instance _ARG string)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, findObject);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, modifyCode);
// void DesignerBase_modifyCode(C(DesignerBase) __i);
#define DesignerBase_modifyCode(__i) \
   VMETHOD(CO(DesignerBase), DesignerBase, modifyCode, __i, void, \
      C(DesignerBase), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, modifyCode);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, objectContainsCode);
// C(bool) DesignerBase_objectContainsCode(C(DesignerBase) __i, C(ObjectInfo) * object);
#define DesignerBase_objectContainsCode(__i, object) \
   VMETHOD(CO(DesignerBase), DesignerBase, objectContainsCode, __i, C(bool), \
      C(DesignerBase) _ARG C(ObjectInfo) *, \
      __i _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, objectContainsCode);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, renameObject);
// void DesignerBase_renameObject(C(DesignerBase) __i, C(ObjectInfo) * object, const char * name);
#define DesignerBase_renameObject(__i, object, name) \
   VMETHOD(CO(DesignerBase), DesignerBase, renameObject, __i, void, \
      C(DesignerBase) _ARG C(ObjectInfo) * _ARG const char *, \
      __i _ARG object _ARG name)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, renameObject);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, selectObjectFromDesigner);
// void DesignerBase_selectObjectFromDesigner(C(DesignerBase) __i, C(ObjectInfo) * object);
#define DesignerBase_selectObjectFromDesigner(__i, object) \
   VMETHOD(CO(DesignerBase), DesignerBase, selectObjectFromDesigner, __i, void, \
      C(DesignerBase) _ARG C(ObjectInfo) *, \
      __i _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, selectObjectFromDesigner);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, sheetAddObject);
// void DesignerBase_sheetAddObject(C(DesignerBase) __i, C(ObjectInfo) * object);
#define DesignerBase_sheetAddObject(__i, object) \
   VMETHOD(CO(DesignerBase), DesignerBase, sheetAddObject, __i, void, \
      C(DesignerBase) _ARG C(ObjectInfo) *, \
      __i _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, sheetAddObject);

extern THIS_LIB_IMPORT int M_VTBLID(DesignerBase, updateProperties);
// void DesignerBase_updateProperties(C(DesignerBase) __i);
#define DesignerBase_updateProperties(__i) \
   VMETHOD(CO(DesignerBase), DesignerBase, updateProperties, __i, void, \
      C(DesignerBase), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(DesignerBase, updateProperties);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DesignerBase, classDesigner);
extern THIS_LIB_IMPORT void (* DesignerBase_set_classDesigner)(const C(DesignerBase) d, C(ClassDesignerBase) value);
extern THIS_LIB_IMPORT C(ClassDesignerBase) (* DesignerBase_get_classDesigner)(const C(DesignerBase) d);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DesignerBase, objectClass);
extern THIS_LIB_IMPORT void (* DesignerBase_set_objectClass)(const C(DesignerBase) d, const char * value);
extern THIS_LIB_IMPORT const char * (* DesignerBase_get_objectClass)(const C(DesignerBase) d);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DesignerBase, isDragging);
extern THIS_LIB_IMPORT void (* DesignerBase_set_isDragging)(const C(DesignerBase) d, C(bool) value);
extern THIS_LIB_IMPORT C(bool) (* DesignerBase_get_isDragging)(const C(DesignerBase) d);

struct C(EnumClassData)
{
   C(OldList) values;
   int64 largest;
};
#define ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT     0
#define ESCAPECSTRINGOPTIONS_escapeSingleQuote_MASK      0x1
#define ESCAPECSTRINGOPTIONS_escapeSingleQuote(x)        ((((C(EscapeCStringOptions))(x)) & ESCAPECSTRINGOPTIONS_escapeSingleQuote_MASK) >> ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT)
#define ESCAPECSTRINGOPTIONS_SET_escapeSingleQuote(x, escapeSingleQuote) (x) = ((C(EscapeCStringOptions))(x) & ~((C(EscapeCStringOptions))ESCAPECSTRINGOPTIONS_escapeSingleQuote_MASK)) | (((C(EscapeCStringOptions))(escapeSingleQuote)) << ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT)
#define ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT    1
#define ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_MASK     0x2
#define ESCAPECSTRINGOPTIONS_escapeDoubleQuotes(x)       ((((C(EscapeCStringOptions))(x)) & ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_MASK) >> ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT)
#define ESCAPECSTRINGOPTIONS_SET_escapeDoubleQuotes(x, escapeDoubleQuotes) (x) = ((C(EscapeCStringOptions))(x) & ~((C(EscapeCStringOptions))ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_MASK)) | (((C(EscapeCStringOptions))(escapeDoubleQuotes)) << ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT)
#define ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT           2
#define ESCAPECSTRINGOPTIONS_writeQuotes_MASK            0x4
#define ESCAPECSTRINGOPTIONS_writeQuotes(x)              ((((C(EscapeCStringOptions))(x)) & ESCAPECSTRINGOPTIONS_writeQuotes_MASK) >> ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT)
#define ESCAPECSTRINGOPTIONS_SET_writeQuotes(x, writeQuotes)       (x) = ((C(EscapeCStringOptions))(x) & ~((C(EscapeCStringOptions))ESCAPECSTRINGOPTIONS_writeQuotes_MASK)) | (((C(EscapeCStringOptions))(writeQuotes)) << ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT)
#define ESCAPECSTRINGOPTIONS_multiLine_SHIFT             3
#define ESCAPECSTRINGOPTIONS_multiLine_MASK              0x8
#define ESCAPECSTRINGOPTIONS_multiLine(x)                ((((C(EscapeCStringOptions))(x)) & ESCAPECSTRINGOPTIONS_multiLine_MASK) >> ESCAPECSTRINGOPTIONS_multiLine_SHIFT)
#define ESCAPECSTRINGOPTIONS_SET_multiLine(x, multiLine)         (x) = ((C(EscapeCStringOptions))(x) & ~((C(EscapeCStringOptions))ESCAPECSTRINGOPTIONS_multiLine_MASK)) | (((C(EscapeCStringOptions))(multiLine)) << ESCAPECSTRINGOPTIONS_multiLine_SHIFT)
#define ESCAPECSTRINGOPTIONS_indent_SHIFT                4
#define ESCAPECSTRINGOPTIONS_indent_MASK                 0xFFFF0
#define ESCAPECSTRINGOPTIONS_indent(x)                   ((((C(EscapeCStringOptions))(x)) & ESCAPECSTRINGOPTIONS_indent_MASK) >> ESCAPECSTRINGOPTIONS_indent_SHIFT)
#define ESCAPECSTRINGOPTIONS_SET_indent(x, indent)            (x) = ((C(EscapeCStringOptions))(x) & ~((C(EscapeCStringOptions))ESCAPECSTRINGOPTIONS_indent_MASK)) | (((C(EscapeCStringOptions))(indent)) << ESCAPECSTRINGOPTIONS_indent_SHIFT)
#define ESCAPECSTRINGOPTIONS(escapeSingleQuote, escapeDoubleQuotes, writeQuotes, multiLine, indent)                (((((((C(EscapeCStringOptions))(escapeSingleQuote)) << ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT) | ((C(EscapeCStringOptions))(escapeDoubleQuotes)) << ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT) | ((C(EscapeCStringOptions))(writeQuotes)) << ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT) | ((C(EscapeCStringOptions))(multiLine)) << ESCAPECSTRINGOPTIONS_multiLine_SHIFT) | ((C(EscapeCStringOptions))(indent)) << ESCAPECSTRINGOPTIONS_indent_SHIFT)


extern THIS_LIB_IMPORT C(Property) * PROPERTY(Feet, Meters);
extern THIS_LIB_IMPORT double (* Feet_from_Meters)(const C(Distance) meters);
extern THIS_LIB_IMPORT C(Distance) (* Feet_to_Meters)(const double feet);
#define Feet(x) FEET(x)
#define FEET(x)  ((C(Distance))((x) * 0.304800003767014))
#define Distance_in_Feet(x)  ((double)((x) * 3.28083985446533))

extern THIS_LIB_IMPORT void (* IOChannel_get)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);

extern THIS_LIB_IMPORT void (* IOChannel_put)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);

extern THIS_LIB_IMPORT int M_VTBLID(IOChannel, readData);
// uintsize IOChannel_readData(C(IOChannel) __i, void * data, uintsize numBytes);
#define IOChannel_readData(__i, data, numBytes) \
   VMETHOD(CO(IOChannel), IOChannel, readData, __i, uintsize, \
      C(IOChannel) _ARG void * _ARG uintsize, \
      __i _ARG data _ARG numBytes)
extern THIS_LIB_IMPORT C(Method) * METHOD(IOChannel, readData);

extern THIS_LIB_IMPORT void (* IOChannel_serialize)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);

extern THIS_LIB_IMPORT void (* IOChannel_unserialize)(C(IOChannel) __this, typed_object_class_ptr class_data, void * data);

extern THIS_LIB_IMPORT int M_VTBLID(IOChannel, writeData);
// uintsize IOChannel_writeData(C(IOChannel) __i, const void * data, uintsize numBytes);
#define IOChannel_writeData(__i, data, numBytes) \
   VMETHOD(CO(IOChannel), IOChannel, writeData, __i, uintsize, \
      C(IOChannel) _ARG const void * _ARG uintsize, \
      __i _ARG data _ARG numBytes)
extern THIS_LIB_IMPORT C(Method) * METHOD(IOChannel, writeData);

#define Meters(x) METERS(x)
#define METERS(x)  ((C(Distance))(x))
#define Distance_in_Meters(x)  ((double)(x))

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Platform, char_ptr);
extern THIS_LIB_IMPORT C(Platform) (* Platform_from_char_ptr)(const char * c);
extern THIS_LIB_IMPORT const char * (* Platform_to_char_ptr)(const C(Platform) platform);

struct C(Point)
{
   int x;
   int y;
};
struct C(Pointd)
{
   double x;
   double y;
};
struct C(Pointf)
{
   float x;
   float y;
};
#define Radians(x) RADIANS(x)
#define RADIANS(x)  ((C(Angle))(x))
#define Angle_in_Radians(x)  ((double)(x))

struct CM(SerialBuffer)
{
   byte * _buffer;
   uintsize count;
   uintsize _size;
   uintsize pos;
};
extern THIS_LIB_IMPORT void (* SerialBuffer_free)(C(SerialBuffer) __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(SerialBuffer, buffer);
extern THIS_LIB_IMPORT void (* SerialBuffer_set_buffer)(const C(SerialBuffer) s, byte * value);
extern THIS_LIB_IMPORT byte * (* SerialBuffer_get_buffer)(const C(SerialBuffer) s);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(SerialBuffer, size);
extern THIS_LIB_IMPORT void (* SerialBuffer_set_size)(const C(SerialBuffer) s, uint value);
extern THIS_LIB_IMPORT uint (* SerialBuffer_get_size)(const C(SerialBuffer) s);

/*struct C(Size)
{
   C(MinMaxValue) w;
   C(MinMaxValue) h;
};*/
struct C(StaticString)
{
   char string[1];
};
struct CM(ZString)
{
   char * _string;
   int len;
   C(StringAllocType) allocType;
   int size;
   int minSize;
   int maxSize;
};
extern THIS_LIB_IMPORT void (* ZString_concat)(C(ZString) __this, C(ZString) s);

extern THIS_LIB_IMPORT void (* ZString_concatf)(C(ZString) __this, const char * format, ...);

extern THIS_LIB_IMPORT void (* ZString_concatn)(C(ZString) __this, C(ZString) s, int l);

extern THIS_LIB_IMPORT void (* ZString_concatx)(C(ZString) __this, typed_object_class_ptr class_object, const void * object, ...);

extern THIS_LIB_IMPORT void (* ZString_copy)(C(ZString) __this, C(ZString) s);

extern THIS_LIB_IMPORT void (* ZString_copyString)(C(ZString) __this, const char * value, int newLen);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(ZString, string);
extern THIS_LIB_IMPORT void (* ZString_set_string)(const C(ZString) z, const char * value);
extern THIS_LIB_IMPORT const char * (* ZString_get_string)(const C(ZString) z);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(ZString, char_ptr);
extern THIS_LIB_IMPORT C(ZString) (* ZString_from_char_ptr)(const char * c);
extern THIS_LIB_IMPORT const char * (* ZString_to_char_ptr)(const C(ZString) z);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(ZString, String);
extern THIS_LIB_IMPORT C(ZString) (* ZString_from_String)(const C(String) string);
extern THIS_LIB_IMPORT constString (* ZString_to_String)(const C(ZString) z);

extern THIS_LIB_IMPORT void (* F(changeCh))(char * string, char ch1, char ch2);
extern THIS_LIB_IMPORT void (* F(changeChars))(char * string, const char * chars, char alt);
extern THIS_LIB_IMPORT char * (* F(changeExtension))(const char * string, const char * ext, char * output);
extern THIS_LIB_IMPORT void (* F(checkConsistency))(void);
extern THIS_LIB_IMPORT void (* F(checkMemory))(void);
extern THIS_LIB_IMPORT void (* F(copyBytes))(void * dest, const void * source, uint64 count);
extern THIS_LIB_IMPORT void (* F(copyBytesBy2))(void * dest, const void * source, uint count);
extern THIS_LIB_IMPORT void (* F(copyBytesBy4))(void * dest, const void * source, uint count);
extern THIS_LIB_IMPORT char * (* F(copyString))(const char * string);
extern THIS_LIB_IMPORT int (* F(escapeCString))(C(String) outString, int bufferLen, constString s, C(EscapeCStringOptions) options);
extern THIS_LIB_IMPORT void (* F(fillBytes))(void * area, byte value, uint count);
extern THIS_LIB_IMPORT void (* F(fillBytesBy2))(void * area, uint16 value, uint count);
extern THIS_LIB_IMPORT void (* F(fillBytesBy4))(void * area, uint value, uint count);
extern THIS_LIB_IMPORT double (* F(floatFromString))(const char * string);
extern THIS_LIB_IMPORT C(DesignerBase) (* F(getActiveDesigner))(void);
extern THIS_LIB_IMPORT char * (* F(getExtension))(const char * string, char * output);
extern THIS_LIB_IMPORT uint (* F(getHexValue))(const char ** buffer);
extern THIS_LIB_IMPORT char * (* F(getLastDirectory))(const char * string, char * output);
extern THIS_LIB_IMPORT C(Platform) (* F(getRuntimePlatform))(void);
extern THIS_LIB_IMPORT C(bool) (* F(getString))(const char ** buffer, char * string, int max);
extern THIS_LIB_IMPORT int (* F(getValue))(const char ** buffer);
extern THIS_LIB_IMPORT C(bool) (* F(isPathInsideOf))(const char * path, const char * of);
extern THIS_LIB_IMPORT C(bool) (* F(locateModule))(const char * name, const char * fileName);
extern THIS_LIB_IMPORT char * (* F(makePathRelative))(const char * path, const char * to, char * destination);
extern THIS_LIB_IMPORT void (* F(moveBytes))(void * dest, const void * source, uint count);
extern THIS_LIB_IMPORT char * (* F(pathCat))(char * string, const char * addedPath);
extern THIS_LIB_IMPORT char * (* F(pathCatSlash))(char * string, const char * addedPath);
extern THIS_LIB_IMPORT void (* F(printx))(typed_object_class_ptr class_object, const void * object, ...);
extern THIS_LIB_IMPORT void (* F(printBigSize))(char * string, double size, int prec);
extern THIS_LIB_IMPORT int (* F(printBuf))(char * buffer, int maxLen, typed_object_class_ptr class_object, const void * object, ...);
extern THIS_LIB_IMPORT void (* F(printLn))(typed_object_class_ptr class_object, const void * object, ...);
extern THIS_LIB_IMPORT int (* F(printLnBuf))(char * buffer, int maxLen, typed_object_class_ptr class_object, const void * object, ...);
extern THIS_LIB_IMPORT char * (* F(printLnString))(typed_object_class_ptr class_object, const void * object, ...);
extern THIS_LIB_IMPORT void (* F(printSize))(char * string, uint64 size, int prec);
extern THIS_LIB_IMPORT int (* F(printStdArgsToBuffer))(char * buffer, int maxLen, typed_object_class_ptr class_object, const void * object, va_list args);
extern THIS_LIB_IMPORT char * (* F(printString))(typed_object_class_ptr class_object, const void * object, ...);
extern THIS_LIB_IMPORT char * (* F(rSearchString))(const char * buffer, const char * subStr, int maxLen, C(bool) matchCase, C(bool) matchWord);
extern THIS_LIB_IMPORT void (* F(repeatCh))(char * string, int count, char ch);
extern THIS_LIB_IMPORT char * (* F(searchString))(const char * buffer, int start, const char * subStr, C(bool) matchCase, C(bool) matchWord);
extern THIS_LIB_IMPORT void (* F(setActiveDesigner))(C(DesignerBase) designer);
extern THIS_LIB_IMPORT C(bool) (* F(splitArchivePath))(const char * fileName, char * archiveName, const char ** archiveFile);
extern THIS_LIB_IMPORT char * (* F(splitDirectory))(const char * string, char * part, char * rest);
extern THIS_LIB_IMPORT C(bool) (* F(stringLikePattern))(constString string, constString pattern);
extern THIS_LIB_IMPORT char * (* F(stripChars))(C(String) string, constString chars);
extern THIS_LIB_IMPORT C(bool) (* F(stripExtension))(char * string);
extern THIS_LIB_IMPORT char * (* F(stripLastDirectory))(const char * string, char * output);
extern THIS_LIB_IMPORT char * (* F(stripQuotes))(const char * string, char * output);
extern THIS_LIB_IMPORT int (* F(tokenize))(char * string, int maxTokens, char * tokens[], C(BackSlashEscaping) esc);
extern THIS_LIB_IMPORT int (* F(tokenizeWith))(char * string, int maxTokens, char * tokens[], const char * tokenizers, C(bool) escapeBackSlashes);
extern THIS_LIB_IMPORT char * (* F(trimLSpaces))(const char * string, char * output);
extern THIS_LIB_IMPORT char * (* F(trimRSpaces))(const char * string, char * output);
extern THIS_LIB_IMPORT int (* F(unescapeCString))(char * d, const char * s, int len);
extern THIS_LIB_IMPORT int (* F(unescapeCStringLoose))(char * d, const char * s, int len);
extern THIS_LIB_IMPORT void (* F(eSystem_LockMem))(void);
extern THIS_LIB_IMPORT void (* F(eSystem_UnlockMem))(void);
extern THIS_LIB_IMPORT C(bool) (* F(ishexdigit))(char x);
extern THIS_LIB_IMPORT uint (* F(log2i))(uint number);
extern THIS_LIB_IMPORT void (* F(memswap))(byte * a, byte * b, uint size);
extern THIS_LIB_IMPORT uint (* F(pow2i))(uint number);
extern THIS_LIB_IMPORT void (* F(queryMemInfo))(char * string);
extern THIS_LIB_IMPORT char * (* F(strchrmax))(const char * s, int c, int max);

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////    //////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef C(Instance) C(Container);
typedef C(Container) C(Array);
typedef C(Container) C(CustomAVLTree);
typedef C(CustomAVLTree) C(Map);
// end -- moved backwards outputs
#define fstrcmp ((__runtimePlatform == Platform_win32) ? strcmpi : strcmp)

#define strcmpi (strcasecmp)

#define strnicmp (strncasecmp)

typedef intptr_t intptr;
typedef ssize_t intsize;
typedef C(Map) T(Map, String, FieldValue);
typedef C(Array) T(Array, FieldValue);
typedef C(Map) T(Map, String, JSONTypeOptions);

extern THIS_LIB_IMPORT int M_VTBLID(class, onCompare);
// int _onCompare(C(Class) * __c, any_object __i, any_object object);
#define _onCompare(__c, __i, object) \
   SVMETHOD(__c, class, onCompare, (C(Instance))null, int, \
      C(Class) * _ARG any_object _ARG any_object, \
      __c _ARG __i _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onCompare);

extern THIS_LIB_IMPORT int M_VTBLID(class, onCopy);
// void _onCopy(C(Class) * __c, any_object __i, any_object newData);
#define _onCopy(__c, __i, newData) \
   SVMETHOD(__c, class, onCopy, (C(Instance))null, void, \
      C(Class) * _ARG any_object _ARG any_object, \
      __c _ARG __i _ARG newData)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onCopy);

extern THIS_LIB_IMPORT int M_VTBLID(class, onDisplay);
// void _onDisplay(C(Class) * __c, any_object __i, C(Instance) surface, int x, int y, int width, void * fieldData, int alignment, uint displayFlags);
#define _onDisplay(__c, __i, surface, x, y, width, fieldData, alignment, displayFlags) \
   SVMETHOD(__c, class, onDisplay, (C(Instance))null, void, \
      C(Class) * _ARG any_object _ARG C(Instance) _ARG int _ARG int _ARG int _ARG void * _ARG int _ARG uint, \
      __c _ARG __i _ARG surface _ARG x _ARG y _ARG width _ARG fieldData _ARG alignment _ARG displayFlags)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onDisplay);

extern THIS_LIB_IMPORT int M_VTBLID(class, onEdit);
// C(Instance) _onEdit(C(Class) * __c, any_object __i, C(Instance) dataBox, C(Instance) obsolete, int x, int y, int w, int h, void * userData);
#define _onEdit(__c, __i, dataBox, obsolete, x, y, w, h, userData) \
   SVMETHOD(__c, class, onEdit, (C(Instance))null, C(Instance), \
      C(Class) * _ARG any_object _ARG C(Instance) _ARG C(Instance) _ARG int _ARG int _ARG int _ARG int _ARG void *, \
      __c _ARG __i _ARG dataBox _ARG obsolete _ARG x _ARG y _ARG w _ARG h _ARG userData)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onEdit);

extern THIS_LIB_IMPORT int M_VTBLID(class, onFree);
// void _onFree(C(Class) * __c, any_object __i);
#define _onFree(__c, __i) \
   SVMETHOD(__c, class, onFree, (C(Instance))null, void, \
      C(Class) * _ARG any_object, \
      __c _ARG __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onFree);

extern THIS_LIB_IMPORT int M_VTBLID(class, onGetDataFromString);
// C(bool) _onGetDataFromString(C(Class) * __c, any_object __i, const char * string);
#define _onGetDataFromString(__c, __i, string) \
   SVMETHOD(__c, class, onGetDataFromString, (C(Instance))null, C(bool), \
      C(Class) * _ARG any_object _ARG const char *, \
      __c _ARG __i _ARG string)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onGetDataFromString);

extern THIS_LIB_IMPORT int M_VTBLID(class, onGetString);
// const char * _onGetString(C(Class) * __c, any_object __i, char * tempString, void * reserved, C(ObjectNotationType) * onType);
#define _onGetString(__c, __i, tempString, reserved, onType) \
   SVMETHOD(__c, class, onGetString, (C(Instance))null, const char *, \
      C(Class) * _ARG any_object _ARG char * _ARG void * _ARG C(ObjectNotationType) *, \
      __c _ARG __i _ARG tempString _ARG reserved _ARG onType)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onGetString);

extern THIS_LIB_IMPORT int M_VTBLID(class, onSaveEdit);
// C(bool) _onSaveEdit(C(Class) * __c, any_object __i, C(Instance) window, void * object);
#define _onSaveEdit(__c, __i, window, object) \
   SVMETHOD(__c, class, onSaveEdit, (C(Instance))null, C(bool), \
      C(Class) * _ARG any_object _ARG C(Instance) _ARG void *, \
      __c _ARG __i _ARG window _ARG object)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onSaveEdit);

extern THIS_LIB_IMPORT int M_VTBLID(class, onSerialize);
// void _onSerialize(C(Class) * __c, any_object __i, C(IOChannel) channel);
#define _onSerialize(__c, __i, channel) \
   SVMETHOD(__c, class, onSerialize, (C(Instance))null, void, \
      C(Class) * _ARG any_object _ARG C(IOChannel), \
      __c _ARG __i _ARG channel)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onSerialize);

extern THIS_LIB_IMPORT int M_VTBLID(class, onUnserialize);
// void _onUnserialize(C(Class) * __c, any_object __i, C(IOChannel) channel);
#define _onUnserialize(__c, __i, channel) \
   SVMETHOD(__c, class, onUnserialize, (C(Instance))null, void, \
      C(Class) * _ARG any_object _ARG C(IOChannel), \
      __c _ARG __i _ARG channel)
extern THIS_LIB_IMPORT C(Method) * METHOD(class, onUnserialize);

extern THIS_LIB_IMPORT int M_VTBLID(class, onCompare);
// int Instance_onCompare(C(Class) * __c, C(Instance) __i, any_object object);
#define Instance_onCompare(__c, __i, object) \
   SVMETHOD(__c, class, onCompare, __i, int, \
      C(Class) * _ARG C(Instance) _ARG any_object, \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG object)

extern THIS_LIB_IMPORT int M_VTBLID(class, onCopy);
// void Instance_onCopy(C(Class) * __c, C(Instance) __i, any_object newData);
#define Instance_onCopy(__c, __i, newData) \
   SVMETHOD(__c, class, onCopy, __i, void, \
      C(Class) * _ARG C(Instance) _ARG any_object, \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG newData)

extern THIS_LIB_IMPORT int M_VTBLID(class, onDisplay);
// void Instance_onDisplay(C(Class) * __c, C(Instance) __i, C(Instance) surface, int x, int y, int width, void * fieldData, int alignment, uint displayFlags);
#define Instance_onDisplay(__c, __i, surface, x, y, width, fieldData, alignment, displayFlags) \
   SVMETHOD(__c, class, onDisplay, __i, void, \
      C(Class) * _ARG C(Instance) _ARG C(Instance) _ARG int _ARG int _ARG int _ARG void * _ARG int _ARG uint, \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG surface _ARG x _ARG y _ARG width _ARG fieldData _ARG alignment _ARG displayFlags)

extern THIS_LIB_IMPORT int M_VTBLID(class, onEdit);
// C(Instance) Instance_onEdit(C(Class) * __c, C(Instance) __i, C(Instance) dataBox, C(Instance) obsolete, int x, int y, int w, int h, void * userData);
#define Instance_onEdit(__c, __i, dataBox, obsolete, x, y, w, h, userData) \
   SVMETHOD(__c, class, onEdit, __i, C(Instance), \
      C(Class) * _ARG C(Instance) _ARG C(Instance) _ARG C(Instance) _ARG int _ARG int _ARG int _ARG int _ARG void *, \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG dataBox _ARG obsolete _ARG x _ARG y _ARG w _ARG h _ARG userData)

extern THIS_LIB_IMPORT int M_VTBLID(class, onFree);
// void Instance_onFree(C(Class) * __c, C(Instance) __i);
#define Instance_onFree(__c, __i) \
   SVMETHOD(__c, class, onFree, __i, void, \
      C(Class) * _ARG C(Instance), \
      (__i) ? (__i)->_class : (__c) _ARG __i)

extern THIS_LIB_IMPORT int M_VTBLID(class, onGetDataFromString);
// C(bool) Instance_onGetDataFromString(C(Class) * __c, C(Instance) __i, const char * string);
#define Instance_onGetDataFromString(__c, __i, string) \
   SVMETHOD(__c, class, onGetDataFromString, __i, C(bool), \
      C(Class) * _ARG C(Instance) _ARG const char *, \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG string)

extern THIS_LIB_IMPORT int M_VTBLID(class, onGetString);
// const char * Instance_onGetString(C(Class) * __c, C(Instance) __i, char * tempString, void * reserved, C(ObjectNotationType) * onType);
#define Instance_onGetString(__c, __i, tempString, reserved, onType) \
   SVMETHOD(__c, class, onGetString, __i, const char *, \
      C(Class) * _ARG C(Instance) _ARG char * _ARG void * _ARG C(ObjectNotationType) *, \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG tempString _ARG reserved _ARG onType)

extern THIS_LIB_IMPORT int M_VTBLID(class, onSaveEdit);
// C(bool) Instance_onSaveEdit(C(Class) * __c, C(Instance) __i, C(Instance) window, void * object);
#define Instance_onSaveEdit(__c, __i, window, object) \
   SVMETHOD(__c, class, onSaveEdit, __i, C(bool), \
      C(Class) * _ARG C(Instance) _ARG C(Instance) _ARG void *, \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG window _ARG object)

extern THIS_LIB_IMPORT int M_VTBLID(class, onSerialize);
// void Instance_onSerialize(C(Class) * __c, C(Instance) __i, C(IOChannel) channel);
#define Instance_onSerialize(__c, __i, channel) \
   SVMETHOD(__c, class, onSerialize, __i, void, \
      C(Class) * _ARG C(Instance) _ARG C(IOChannel), \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG channel)

extern THIS_LIB_IMPORT int M_VTBLID(class, onUnserialize);
// void Instance_onUnserialize(C(Class) * __c, C(Instance) __i, C(IOChannel) channel);
#define Instance_onUnserialize(__c, __i, channel) \
   SVMETHOD(__c, class, onUnserialize, __i, void, \
      C(Class) * _ARG C(Instance) _ARG C(IOChannel), \
      (__i) ? (__i)->_class : (__c) _ARG __i _ARG channel)

extern THIS_LIB_IMPORT double (* double_inf)(void);

extern THIS_LIB_IMPORT double (* double_nan)(void);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(double, isNan);
extern THIS_LIB_IMPORT C(bool) (* double_get_isNan)(const double d);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(double, isInf);
extern THIS_LIB_IMPORT C(bool) (* double_get_isInf)(const double d);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(double, signBit);
extern THIS_LIB_IMPORT int (* double_get_signBit)(const double d);

extern THIS_LIB_IMPORT float (* float_inf)(void);

extern THIS_LIB_IMPORT float (* float_nan)(void);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(float, isNan);
extern THIS_LIB_IMPORT C(bool) (* float_get_isNan)(const float f);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(float, isInf);
extern THIS_LIB_IMPORT C(bool) (* float_get_isInf)(const float f);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(float, signBit);
extern THIS_LIB_IMPORT int (* float_get_signBit)(const float f);


// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC  ////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////



// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////    //////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


#define jsonIndentWidth (3)

typedef uint32_t unichar;

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC  ////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef uint64_t TP(Container, T);
struct CM(Array)
{
   TP(Container, T) * array;
   uint count;
   uint minAllocSize;
};
typedef int C(FieldTypeEx);
typedef struct C(FieldValue) C(FieldValue);
// end -- moved backwards outputs
#if CPP11
enum C(FieldType) : int
#else
typedef int C(FieldType);
enum C(FieldType)
#endif
{
   FieldType_integer = 0x1,
   FieldType_real = 0x2,
   FieldType_text = 0x3,
   FieldType_blob = 0x4,
   FieldType_nil = 0x5,
   FieldType_array = 0x6,
   FieldType_map = 0x7
};

#if CPP11
enum C(FieldValueFormat) : int
#else
typedef int C(FieldValueFormat);
enum C(FieldValueFormat)
#endif
{
   FieldValueFormat_decimal = 0x0,
   FieldValueFormat_unset = 0x0,
   FieldValueFormat_hex = 0x1,
   FieldValueFormat_octal = 0x2,
   FieldValueFormat_binary = 0x3,
   FieldValueFormat_exponential = 0x4,
   FieldValueFormat_boolean = 0x5,
   FieldValueFormat_textObj = 0x6,
   FieldValueFormat_color = 0x7
};

#define FIELDTYPEEX_type_SHIFT                           0
#define FIELDTYPEEX_type_MASK                            0x7
#define FIELDTYPEEX_type(x)                              ((((C(FieldTypeEx))(x)) & FIELDTYPEEX_type_MASK) >> FIELDTYPEEX_type_SHIFT)
#define FIELDTYPEEX_SET_type(x, type)                       (x) = ((C(FieldTypeEx))(x) & ~((C(FieldTypeEx))FIELDTYPEEX_type_MASK)) | (((C(FieldTypeEx))(type)) << FIELDTYPEEX_type_SHIFT)
#define FIELDTYPEEX_mustFree_SHIFT                       3
#define FIELDTYPEEX_mustFree_MASK                        0x8
#define FIELDTYPEEX_mustFree(x)                          ((((C(FieldTypeEx))(x)) & FIELDTYPEEX_mustFree_MASK) >> FIELDTYPEEX_mustFree_SHIFT)
#define FIELDTYPEEX_SET_mustFree(x, mustFree)                   (x) = ((C(FieldTypeEx))(x) & ~((C(FieldTypeEx))FIELDTYPEEX_mustFree_MASK)) | (((C(FieldTypeEx))(mustFree)) << FIELDTYPEEX_mustFree_SHIFT)
#define FIELDTYPEEX_format_SHIFT                         4
#define FIELDTYPEEX_format_MASK                          0xF0
#define FIELDTYPEEX_format(x)                            ((((C(FieldTypeEx))(x)) & FIELDTYPEEX_format_MASK) >> FIELDTYPEEX_format_SHIFT)
#define FIELDTYPEEX_SET_format(x, format)                     (x) = ((C(FieldTypeEx))(x) & ~((C(FieldTypeEx))FIELDTYPEEX_format_MASK)) | (((C(FieldTypeEx))(format)) << FIELDTYPEEX_format_SHIFT)
#define FIELDTYPEEX_isUnsigned_SHIFT                     8
#define FIELDTYPEEX_isUnsigned_MASK                      0x100
#define FIELDTYPEEX_isUnsigned(x)                        ((((C(FieldTypeEx))(x)) & FIELDTYPEEX_isUnsigned_MASK) >> FIELDTYPEEX_isUnsigned_SHIFT)
#define FIELDTYPEEX_SET_isUnsigned(x, isUnsigned)                 (x) = ((C(FieldTypeEx))(x) & ~((C(FieldTypeEx))FIELDTYPEEX_isUnsigned_MASK)) | (((C(FieldTypeEx))(isUnsigned)) << FIELDTYPEEX_isUnsigned_SHIFT)
#define FIELDTYPEEX_isDateTime_SHIFT                     9
#define FIELDTYPEEX_isDateTime_MASK                      0x200
#define FIELDTYPEEX_isDateTime(x)                        ((((C(FieldTypeEx))(x)) & FIELDTYPEEX_isDateTime_MASK) >> FIELDTYPEEX_isDateTime_SHIFT)
#define FIELDTYPEEX_SET_isDateTime(x, isDateTime)                 (x) = ((C(FieldTypeEx))(x) & ~((C(FieldTypeEx))FIELDTYPEEX_isDateTime_MASK)) | (((C(FieldTypeEx))(isDateTime)) << FIELDTYPEEX_isDateTime_SHIFT)
#define FIELDTYPEEX(type, mustFree, format, isUnsigned, isDateTime)                         (((((((C(FieldTypeEx))(type)) << FIELDTYPEEX_type_SHIFT) | ((C(FieldTypeEx))(mustFree)) << FIELDTYPEEX_mustFree_SHIFT) | ((C(FieldTypeEx))(format)) << FIELDTYPEEX_format_SHIFT) | ((C(FieldTypeEx))(isUnsigned)) << FIELDTYPEEX_isUnsigned_SHIFT) | ((C(FieldTypeEx))(isDateTime)) << FIELDTYPEEX_isDateTime_SHIFT)


struct C(FieldValue)
{
   C(FieldTypeEx) type;
   union
   {
      int64 i;
      double r;
      C(String) s;
      void * b;
      C(Array) a;
      C(Map) m;
   };
};
extern THIS_LIB_IMPORT int (* FieldValue_compareInt)(C(FieldValue) * __this, C(FieldValue) * other);

extern THIS_LIB_IMPORT int (* FieldValue_compareReal)(C(FieldValue) * __this, C(FieldValue) * other);

extern THIS_LIB_IMPORT int (* FieldValue_compareText)(C(FieldValue) * __this, C(FieldValue) * other);

extern THIS_LIB_IMPORT C(String) (* FieldValue_formatArray)(C(FieldValue) * __this, char * tempString, void * fieldData, C(ObjectNotationType) * onType);

extern THIS_LIB_IMPORT C(String) (* FieldValue_formatFloat)(C(FieldValue) * __this, char * stringOutput, C(bool) fixDot);

extern THIS_LIB_IMPORT C(String) (* FieldValue_formatInteger)(C(FieldValue) * __this, char * stringOutput);

extern THIS_LIB_IMPORT C(String) (* FieldValue_formatMap)(C(FieldValue) * __this, char * tempString, void * fieldData, C(ObjectNotationType) * onType);

extern THIS_LIB_IMPORT C(bool) (* FieldValue_getArrayOrMap)(const char * string, C(Class) * destClass, void ** destination);

extern THIS_LIB_IMPORT C(String) (* FieldValue_stringify)(C(FieldValue) * __this);


// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||containers  ////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef uint64_t TP(AVLNode, T);
typedef uint64_t TP(CustomAVLTree, BT);
typedef uint64_t TP(LinkElement, T);
typedef uint64_t TP(LinkList, LT);
typedef uint64_t TP(MapNode, V);
struct CM(HashMap)
{
   byte __eCPrivateData0[8];
   C(bool) noRemResize;
   byte __ecere_padding[4];
};
typedef struct C(Item) C(Item);
struct C(Item)
{
   C(Item) * prev;
   C(Item) * next;
};
struct C(IteratorPointer)
{
};
typedef struct C(IteratorPointer) C(IteratorPointer);
struct C(Iterator)
{
   C(Container) container;
   C(IteratorPointer) * pointer;
};
struct C(LinkElement)
{
   TP(LinkElement, T) prev;
   TP(LinkElement, T) next;
};
typedef struct C(NamedItem) C(NamedItem);
struct C(NamedItem)
{
   C(NamedItem) * prev;
   C(NamedItem) * next;
   char * name;
};
typedef struct C(NamedLink) C(NamedLink);
struct C(NamedLink)
{
   C(NamedLink) * prev;
   C(NamedLink) * next;
   char * name;
   void * data;
};
typedef struct C(NamedLink64) C(NamedLink64);
struct C(NamedLink64)
{
   C(NamedLink64) * prev;
   C(NamedLink64) * next;
   char * name;
   int64 data;
};
typedef struct C(OldLink) C(OldLink);
struct C(OldLink)
{
   C(OldLink) * prev;
   C(OldLink) * next;
   void * data;
};
typedef struct C(StringBTNode) C(StringBTNode);
struct C(StringBTNode)
{
   C(String) key;
   C(StringBTNode) * parent;
   C(StringBTNode) * left;
   C(StringBTNode) * right;
   int depth;
};
typedef struct C(AVLNode) C(AVLNode);
typedef struct C(BuiltInContainer) C(BuiltInContainer);
typedef C(Container) C(HashMap);
typedef struct C(Iterator) C(Iterator);
typedef struct C(LinkElement) C(LinkElement);
typedef C(Container) C(LinkList);
#if CPP11
enum C(TreePrintStyle) : int
#else
typedef int C(TreePrintStyle);
enum C(TreePrintStyle)
#endif
{
   TreePrintStyle_inOrder = 0x0,
   TreePrintStyle_postOrder = 0x1,
   TreePrintStyle_preOrder = 0x2,
   TreePrintStyle_depthOrder = 0x3
};

// end -- moved backwards outputs
typedef C(CustomAVLTree) C(AVLTree);
typedef struct C(HashMapIterator) C(HashMapIterator);
typedef C(Container) C(HashTable);
typedef struct C(Link) C(Link);
typedef C(LinkList) C(List);
typedef struct C(ListItem) C(ListItem);
typedef struct C(MapIterator) C(MapIterator);
typedef struct C(MapNode) C(MapNode);
typedef struct C(StringBinaryTree) C(StringBinaryTree);
typedef uint64_t TP(Container, D);
typedef uint64_t TP(HashMapIterator, KT);
typedef uint64_t TP(Iterator, T);
typedef uint64_t TP(HashMapIterator, VT);
typedef uint64_t TP(MapIterator, V);
typedef uint64_t TP(Container, I);
typedef uint64_t TP(MapIterator, KT);
typedef uint64_t TP(MapNode, KT);
typedef uint64_t TP(Iterator, IT);
struct C(AVLNode)
{
   byte __eCPrivateData0[32];
   TP(AVLNode, T) key;
};
extern THIS_LIB_IMPORT thisclass(AVLNode *) (* AVLNode_find)(C(AVLNode) * __this, C(Class) * Tclass, TP(AVLNode, T) key);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(AVLNode, prev);
extern THIS_LIB_IMPORT thisclass(AVLNode *) (* AVLNode_get_prev)(const C(AVLNode) * a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(AVLNode, next);
extern THIS_LIB_IMPORT thisclass(AVLNode *) (* AVLNode_get_next)(const C(AVLNode) * a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(AVLNode, minimum);
extern THIS_LIB_IMPORT thisclass(AVLNode *) (* AVLNode_get_minimum)(const C(AVLNode) * a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(AVLNode, maximum);
extern THIS_LIB_IMPORT thisclass(AVLNode *) (* AVLNode_get_maximum)(const C(AVLNode) * a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(AVLNode, count);
extern THIS_LIB_IMPORT int (* AVLNode_get_count)(const C(AVLNode) * a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(AVLNode, depthProp);
extern THIS_LIB_IMPORT int (* AVLNode_get_depthProp)(const C(AVLNode) * a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Array, size);
extern THIS_LIB_IMPORT void (* Array_set_size)(const C(Array) a, uint value);
extern THIS_LIB_IMPORT uint (* Array_get_size)(const C(Array) a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Array, minAllocSize);
extern THIS_LIB_IMPORT void (* Array_set_minAllocSize)(const C(Array) a, uint value);
extern THIS_LIB_IMPORT uint (* Array_get_minAllocSize)(const C(Array) a);

extern THIS_LIB_IMPORT C(BTNode) * (* BTNode_findPrefix)(C(BTNode) * __this, const char * key);

extern THIS_LIB_IMPORT C(BTNode) * (* BTNode_findString)(C(BTNode) * __this, const char * key);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BTNode, prev);
extern THIS_LIB_IMPORT C(BTNode) * (* BTNode_get_prev)(const C(BTNode) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BTNode, next);
extern THIS_LIB_IMPORT C(BTNode) * (* BTNode_get_next)(const C(BTNode) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BTNode, minimum);
extern THIS_LIB_IMPORT C(BTNode) * (* BTNode_get_minimum)(const C(BTNode) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BTNode, maximum);
extern THIS_LIB_IMPORT C(BTNode) * (* BTNode_get_maximum)(const C(BTNode) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BTNode, count);
extern THIS_LIB_IMPORT int (* BTNode_get_count)(const C(BTNode) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BTNode, depthProp);
extern THIS_LIB_IMPORT int (* BTNode_get_depthProp)(const C(BTNode) * b);

extern THIS_LIB_IMPORT C(bool) (* BinaryTree_add)(C(BinaryTree) * __this, C(BTNode) * node);

extern THIS_LIB_IMPORT C(bool) (* BinaryTree_check)(C(BinaryTree) * __this);

extern THIS_LIB_IMPORT int (* BinaryTree_compareInt)(C(BinaryTree) * __this, uintptr a, uintptr b);

extern THIS_LIB_IMPORT int (* BinaryTree_compareString)(C(BinaryTree) * __this, const char * a, const char * b);

extern THIS_LIB_IMPORT void (* BinaryTree_delete)(C(BinaryTree) * __this, C(BTNode) * node);

extern THIS_LIB_IMPORT C(BTNode) * (* BinaryTree_find)(C(BinaryTree) * __this, uintptr key);

extern THIS_LIB_IMPORT C(BTNode) * (* BinaryTree_findAll)(C(BinaryTree) * __this, uintptr key);

extern THIS_LIB_IMPORT C(BTNode) * (* BinaryTree_findPrefix)(C(BinaryTree) * __this, const char * key);

extern THIS_LIB_IMPORT C(BTNode) * (* BinaryTree_findString)(C(BinaryTree) * __this, const char * key);

extern THIS_LIB_IMPORT void (* BinaryTree_free)(C(BinaryTree) * __this);

extern THIS_LIB_IMPORT void (* BinaryTree_freeString)(char * string);

extern THIS_LIB_IMPORT char * (* BinaryTree_print)(C(BinaryTree) * __this, char * output, C(TreePrintStyle) tps);

extern THIS_LIB_IMPORT void (* BinaryTree_remove)(C(BinaryTree) * __this, C(BTNode) * node);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BinaryTree, first);
extern THIS_LIB_IMPORT C(BTNode) * (* BinaryTree_get_first)(const C(BinaryTree) * b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BinaryTree, last);
extern THIS_LIB_IMPORT C(BTNode) * (* BinaryTree_get_last)(const C(BinaryTree) * b);

struct C(BuiltInContainer)
{
   void ** _vTbl;
   C(Class) * _class;
   int _refCount;
   void * data;
   int count;
   C(Class) * type;
};
extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, add);
// C(IteratorPointer) * BuiltInContainer_add(C(BuiltInContainer) * __i, uint64 value);
#define BuiltInContainer_add(__i, value) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, add, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) * _ARG uint64, \
      __i _ARG value)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, add);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, copy);
// void BuiltInContainer_copy(C(BuiltInContainer) * __i, C(Container) source);
#define BuiltInContainer_copy(__i, source) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, copy, __i, void, \
      C(BuiltInContainer) * _ARG C(Container), \
      __i _ARG source)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, copy);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, delete);
// void BuiltInContainer_delete(C(BuiltInContainer) * __i, C(IteratorPointer) * it);
#define BuiltInContainer_delete(__i, it) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, delete, __i, void, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) *, \
      __i _ARG it)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, delete);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, find);
// C(IteratorPointer) * BuiltInContainer_find(C(BuiltInContainer) * __i, uint64 value);
#define BuiltInContainer_find(__i, value) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, find, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) * _ARG uint64, \
      __i _ARG value)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, find);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, free);
// void BuiltInContainer_free(C(BuiltInContainer) * __i);
#define BuiltInContainer_free(__i) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, free, __i, void, \
      C(BuiltInContainer) *, \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, free);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, freeIterator);
// void BuiltInContainer_freeIterator(C(BuiltInContainer) * __i, C(IteratorPointer) * it);
#define BuiltInContainer_freeIterator(__i, it) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, freeIterator, __i, void, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) *, \
      __i _ARG it)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, freeIterator);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, getAtPosition);
// C(IteratorPointer) * BuiltInContainer_getAtPosition(C(BuiltInContainer) * __i, const uint64 pos, C(bool) create);
#define BuiltInContainer_getAtPosition(__i, pos, create) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, getAtPosition, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) * _ARG const uint64 _ARG C(bool), \
      __i _ARG pos _ARG create)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, getAtPosition);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, getCount);
// int BuiltInContainer_getCount(C(BuiltInContainer) * __i);
#define BuiltInContainer_getCount(__i) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, getCount, __i, int, \
      C(BuiltInContainer) *, \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, getCount);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, getData);
// uint64 BuiltInContainer_getData(C(BuiltInContainer) * __i, C(IteratorPointer) * pointer);
#define BuiltInContainer_getData(__i, pointer) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, getData, __i, uint64, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) *, \
      __i _ARG pointer)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, getData);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, getFirst);
// C(IteratorPointer) * BuiltInContainer_getFirst(C(BuiltInContainer) * __i);
#define BuiltInContainer_getFirst(__i) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, getFirst, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) *, \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, getFirst);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, getLast);
// C(IteratorPointer) * BuiltInContainer_getLast(C(BuiltInContainer) * __i);
#define BuiltInContainer_getLast(__i) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, getLast, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) *, \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, getLast);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, getNext);
// C(IteratorPointer) * BuiltInContainer_getNext(C(BuiltInContainer) * __i, C(IteratorPointer) * pointer);
#define BuiltInContainer_getNext(__i, pointer) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, getNext, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) *, \
      __i _ARG pointer)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, getNext);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, getPrev);
// C(IteratorPointer) * BuiltInContainer_getPrev(C(BuiltInContainer) * __i, C(IteratorPointer) * pointer);
#define BuiltInContainer_getPrev(__i, pointer) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, getPrev, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) *, \
      __i _ARG pointer)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, getPrev);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, insert);
// C(IteratorPointer) * BuiltInContainer_insert(C(BuiltInContainer) * __i, C(IteratorPointer) * after, uint64 value);
#define BuiltInContainer_insert(__i, after, value) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, insert, __i, C(IteratorPointer) *, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) * _ARG uint64, \
      __i _ARG after _ARG value)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, insert);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, move);
// void BuiltInContainer_move(C(BuiltInContainer) * __i, C(IteratorPointer) * it, C(IteratorPointer) * after);
#define BuiltInContainer_move(__i, it, after) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, move, __i, void, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) * _ARG C(IteratorPointer) *, \
      __i _ARG it _ARG after)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, move);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, remove);
// void BuiltInContainer_remove(C(BuiltInContainer) * __i, C(IteratorPointer) * it);
#define BuiltInContainer_remove(__i, it) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, remove, __i, void, \
      C(BuiltInContainer) * _ARG C(IteratorPointer) *, \
      __i _ARG it)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, remove);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, removeAll);
// void BuiltInContainer_removeAll(C(BuiltInContainer) * __i);
#define BuiltInContainer_removeAll(__i) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, removeAll, __i, void, \
      C(BuiltInContainer) *, \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, removeAll);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, setData);
// C(bool) BuiltInContainer_setData(C(BuiltInContainer) * __i, C(IteratorPointer) * pointer, uint64 data);
#define BuiltInContainer_setData(__i, pointer, data) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, setData, __i, C(bool), \
      C(BuiltInContainer) * _ARG C(IteratorPointer) * _ARG uint64, \
      __i _ARG pointer _ARG data)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, setData);

extern THIS_LIB_IMPORT int M_VTBLID(BuiltInContainer, sort);
// void BuiltInContainer_sort(C(BuiltInContainer) * __i, C(bool) ascending);
#define BuiltInContainer_sort(__i, ascending) \
   SVMETHOD(CO(BuiltInContainer), BuiltInContainer, sort, __i, void, \
      C(BuiltInContainer) * _ARG C(bool), \
      __i _ARG ascending)
extern THIS_LIB_IMPORT C(Method) * METHOD(BuiltInContainer, sort);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BuiltInContainer, Container);
extern THIS_LIB_IMPORT C(Container) (* BuiltInContainer_to_Container)(const C(BuiltInContainer) * b);

extern THIS_LIB_IMPORT int M_VTBLID(Container, add);
// C(IteratorPointer) * Container_add(C(Container) __i, TP(Container, T) value);
#define Container_add(__i, value) \
   VMETHOD(CO(Container), Container, add, __i, C(IteratorPointer) *, \
      C(Container) _ARG TP(Container, T), \
      __i _ARG value)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, add);

extern THIS_LIB_IMPORT int M_VTBLID(Container, copy);
// void Container_copy(C(Container) __i, C(Container) source);
#define Container_copy(__i, source) \
   VMETHOD(CO(Container), Container, copy, __i, void, \
      C(Container) _ARG C(Container), \
      __i _ARG source)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, copy);

extern THIS_LIB_IMPORT int M_VTBLID(Container, delete);
// void Container_delete(C(Container) __i, C(IteratorPointer) * i);
#define Container_delete(__i, i) \
   VMETHOD(CO(Container), Container, delete, __i, void, \
      C(Container) _ARG C(IteratorPointer) *, \
      __i _ARG i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, delete);

extern THIS_LIB_IMPORT int M_VTBLID(Container, find);
// C(IteratorPointer) * Container_find(C(Container) __i, TP(Container, D) value);
#define Container_find(__i, value) \
   VMETHOD(CO(Container), Container, find, __i, C(IteratorPointer) *, \
      C(Container) _ARG TP(Container, D), \
      __i _ARG value)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, find);

extern THIS_LIB_IMPORT int M_VTBLID(Container, free);
// void Container_free(C(Container) __i);
#define Container_free(__i) \
   VMETHOD(CO(Container), Container, free, __i, void, \
      C(Container), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, free);

extern THIS_LIB_IMPORT int M_VTBLID(Container, freeIterator);
// void Container_freeIterator(C(Container) __i, C(IteratorPointer) * it);
#define Container_freeIterator(__i, it) \
   VMETHOD(CO(Container), Container, freeIterator, __i, void, \
      C(Container) _ARG C(IteratorPointer) *, \
      __i _ARG it)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, freeIterator);

extern THIS_LIB_IMPORT int M_VTBLID(Container, getAtPosition);
// C(IteratorPointer) * Container_getAtPosition(C(Container) __i, TP(Container, I) pos, C(bool) create, C(bool) * justAdded);
#define Container_getAtPosition(__i, pos, create, justAdded) \
   VMETHOD(CO(Container), Container, getAtPosition, __i, C(IteratorPointer) *, \
      C(Container) _ARG TP(Container, I) _ARG C(bool) _ARG C(bool) *, \
      __i _ARG pos _ARG create _ARG justAdded)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, getAtPosition);

extern THIS_LIB_IMPORT int M_VTBLID(Container, getCount);
// int Container_getCount(C(Container) __i);
#define Container_getCount(__i) \
   VMETHOD(CO(Container), Container, getCount, __i, int, \
      C(Container), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, getCount);

extern THIS_LIB_IMPORT int M_VTBLID(Container, getData);
// TP(Container, D) Container_getData(C(Container) __i, C(IteratorPointer) * pointer);
#define Container_getData(__i, pointer) \
   VMETHOD(CO(Container), Container, getData, __i, TP(Container, D), \
      C(Container) _ARG C(IteratorPointer) *, \
      __i _ARG pointer)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, getData);

extern THIS_LIB_IMPORT int M_VTBLID(Container, getFirst);
// C(IteratorPointer) * Container_getFirst(C(Container) __i);
#define Container_getFirst(__i) \
   VMETHOD(CO(Container), Container, getFirst, __i, C(IteratorPointer) *, \
      C(Container), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, getFirst);

extern THIS_LIB_IMPORT int M_VTBLID(Container, getLast);
// C(IteratorPointer) * Container_getLast(C(Container) __i);
#define Container_getLast(__i) \
   VMETHOD(CO(Container), Container, getLast, __i, C(IteratorPointer) *, \
      C(Container), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, getLast);

extern THIS_LIB_IMPORT int M_VTBLID(Container, getNext);
// C(IteratorPointer) * Container_getNext(C(Container) __i, C(IteratorPointer) * pointer);
#define Container_getNext(__i, pointer) \
   VMETHOD(CO(Container), Container, getNext, __i, C(IteratorPointer) *, \
      C(Container) _ARG C(IteratorPointer) *, \
      __i _ARG pointer)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, getNext);

extern THIS_LIB_IMPORT int M_VTBLID(Container, getPrev);
// C(IteratorPointer) * Container_getPrev(C(Container) __i, C(IteratorPointer) * pointer);
#define Container_getPrev(__i, pointer) \
   VMETHOD(CO(Container), Container, getPrev, __i, C(IteratorPointer) *, \
      C(Container) _ARG C(IteratorPointer) *, \
      __i _ARG pointer)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, getPrev);

extern THIS_LIB_IMPORT int M_VTBLID(Container, insert);
// C(IteratorPointer) * Container_insert(C(Container) __i, C(IteratorPointer) * after, TP(Container, T) value);
#define Container_insert(__i, after, value) \
   VMETHOD(CO(Container), Container, insert, __i, C(IteratorPointer) *, \
      C(Container) _ARG C(IteratorPointer) * _ARG TP(Container, T), \
      __i _ARG after _ARG value)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, insert);

extern THIS_LIB_IMPORT int M_VTBLID(Container, move);
// void Container_move(C(Container) __i, C(IteratorPointer) * it, C(IteratorPointer) * after);
#define Container_move(__i, it, after) \
   VMETHOD(CO(Container), Container, move, __i, void, \
      C(Container) _ARG C(IteratorPointer) * _ARG C(IteratorPointer) *, \
      __i _ARG it _ARG after)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, move);

extern THIS_LIB_IMPORT int M_VTBLID(Container, remove);
// void Container_remove(C(Container) __i, C(IteratorPointer) * it);
#define Container_remove(__i, it) \
   VMETHOD(CO(Container), Container, remove, __i, void, \
      C(Container) _ARG C(IteratorPointer) *, \
      __i _ARG it)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, remove);

extern THIS_LIB_IMPORT int M_VTBLID(Container, removeAll);
// void Container_removeAll(C(Container) __i);
#define Container_removeAll(__i) \
   VMETHOD(CO(Container), Container, removeAll, __i, void, \
      C(Container), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, removeAll);

extern THIS_LIB_IMPORT int M_VTBLID(Container, setData);
// C(bool) Container_setData(C(Container) __i, C(IteratorPointer) * pointer, TP(Container, D) data);
#define Container_setData(__i, pointer, data) \
   VMETHOD(CO(Container), Container, setData, __i, C(bool), \
      C(Container) _ARG C(IteratorPointer) * _ARG TP(Container, D), \
      __i _ARG pointer _ARG data)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, setData);

extern THIS_LIB_IMPORT int M_VTBLID(Container, sort);
// void Container_sort(C(Container) __i, C(bool) ascending);
#define Container_sort(__i, ascending) \
   VMETHOD(CO(Container), Container, sort, __i, void, \
      C(Container) _ARG C(bool), \
      __i _ARG ascending)
extern THIS_LIB_IMPORT C(Method) * METHOD(Container, sort);

extern THIS_LIB_IMPORT C(bool) (* Container_takeOut)(C(Container) __this, TP(Container, D) d);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Container, copySrc);
extern THIS_LIB_IMPORT void (* Container_set_copySrc)(const C(Container) c, C(Container) value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Container, firstIterator);
extern THIS_LIB_IMPORT void (* Container_get_firstIterator)(const C(Container) c, C(Iterator) * value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Container, lastIterator);
extern THIS_LIB_IMPORT void (* Container_get_lastIterator)(const C(Container) c, C(Iterator) * value);

struct CM(CustomAVLTree)
{
   TP(CustomAVLTree, BT) root;
   int count;
};
extern THIS_LIB_IMPORT C(bool) (* CustomAVLTree_check)(C(CustomAVLTree) __this);

extern THIS_LIB_IMPORT void (* CustomAVLTree_freeKey)(C(CustomAVLTree) __this, C(AVLNode) * item);

extern THIS_LIB_IMPORT void (* HashMap_removeIterating)(C(HashMap) __this, C(IteratorPointer) * it);

extern THIS_LIB_IMPORT void (* HashMap_resize)(C(HashMap) __this, C(IteratorPointer) * movedEntry);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(HashMap, count);
extern THIS_LIB_IMPORT int (* HashMap_get_count)(const C(HashMap) h);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(HashMap, initSize);
extern THIS_LIB_IMPORT void (* HashMap_set_initSize)(const C(HashMap) h, int value);

struct C(HashMapIterator)
{
   C(Container) container;
   C(IteratorPointer) * pointer;
};
extern THIS_LIB_IMPORT C(Property) * PROPERTY(HashMapIterator, map);
extern THIS_LIB_IMPORT void (* HashMapIterator_set_map)(const C(HashMapIterator) * h, C(HashMap) value);
extern THIS_LIB_IMPORT C(HashMap) (* HashMapIterator_get_map)(const C(HashMapIterator) * h);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(HashMapIterator, key);
extern THIS_LIB_IMPORT TP(HashMapIterator, KT) (* HashMapIterator_get_key)(const C(HashMapIterator) * h);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(HashMapIterator, value);
extern THIS_LIB_IMPORT void (* HashMapIterator_set_value)(const C(HashMapIterator) * h, TP(HashMapIterator, VT) value);
extern THIS_LIB_IMPORT TP(HashMapIterator, VT) (* HashMapIterator_get_value)(const C(HashMapIterator) * h);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(HashTable, initSize);
extern THIS_LIB_IMPORT void (* HashTable_set_initSize)(const C(HashTable) h, int value);

extern THIS_LIB_IMPORT void (* Item_copy)(C(Item) * __this, C(Item) * src, int size);

extern THIS_LIB_IMPORT C(bool) (* Iterator_find)(C(Iterator) * __this, TP(Iterator, T) value);

extern THIS_LIB_IMPORT void (* Iterator_free)(C(Iterator) * __this);

extern THIS_LIB_IMPORT TP(Iterator, T) (* Iterator_getData)(C(Iterator) * __this);

extern THIS_LIB_IMPORT C(bool) (* Iterator_index)(C(Iterator) * __this, TP(Iterator, IT) index, C(bool) create);

extern THIS_LIB_IMPORT C(bool) (* Iterator_next)(C(Iterator) * __this);

extern THIS_LIB_IMPORT C(bool) (* Iterator_prev)(C(Iterator) * __this);

extern THIS_LIB_IMPORT void (* Iterator_remove)(C(Iterator) * __this);

extern THIS_LIB_IMPORT C(bool) (* Iterator_setData)(C(Iterator) * __this, TP(Iterator, T) value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Iterator, data);
extern THIS_LIB_IMPORT void (* Iterator_set_data)(const C(Iterator) * i, TP(Iterator, T) value);
extern THIS_LIB_IMPORT TP(Iterator, T) (* Iterator_get_data)(const C(Iterator) * i);

struct C(Link)
{
   union
   {
      C(LinkElement) link;
      struct
      {
         thisclass(ListItem *) prev;
         thisclass(ListItem *) next;
      };
   };
   uint64 data;
};
struct CM(LinkList)
{
   TP(LinkList, LT) first;
   TP(LinkList, LT) last;
   int count;
};
extern THIS_LIB_IMPORT void (* LinkList__Sort)(C(LinkList) __this, C(bool) ascending, C(LinkList) * lists);

struct C(ListItem)
{
   union
   {
      C(LinkElement) link;
      struct
      {
         thisclass(ListItem *) prev;
         thisclass(ListItem *) next;
      };
   };
};
extern THIS_LIB_IMPORT C(Property) * PROPERTY(Map, mapSrc);
extern THIS_LIB_IMPORT void (* Map_set_mapSrc)(const C(Map) m, C(Map) value);

struct C(MapIterator)
{
   C(Container) container;
   C(IteratorPointer) * pointer;
};
extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapIterator, map);
extern THIS_LIB_IMPORT void (* MapIterator_set_map)(const C(MapIterator) * m, C(Map) value);
extern THIS_LIB_IMPORT C(Map) (* MapIterator_get_map)(const C(MapIterator) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapIterator, key);
extern THIS_LIB_IMPORT TP(MapIterator, KT) (* MapIterator_get_key)(const C(MapIterator) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapIterator, value);
extern THIS_LIB_IMPORT void (* MapIterator_set_value)(const C(MapIterator) * m, TP(MapIterator, V) value);
extern THIS_LIB_IMPORT TP(MapIterator, V) (* MapIterator_get_value)(const C(MapIterator) * m);

struct C(MapNode)
{
   byte __eCPrivateData0[32];
   TP(AVLNode, T) key;
   TP(MapNode, V) value;
};
extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapNode, key);
extern THIS_LIB_IMPORT void (* MapNode_set_key)(const C(MapNode) * m, TP(MapNode, KT) value);
extern THIS_LIB_IMPORT TP(MapNode, KT) (* MapNode_get_key)(const C(MapNode) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapNode, value);
extern THIS_LIB_IMPORT void (* MapNode_set_value)(const C(MapNode) * m, TP(MapNode, V) value);
extern THIS_LIB_IMPORT TP(MapNode, V) (* MapNode_get_value)(const C(MapNode) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapNode, prev);
extern THIS_LIB_IMPORT thisclass(MapNode *) (* MapNode_get_prev)(const C(MapNode) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapNode, next);
extern THIS_LIB_IMPORT thisclass(MapNode *) (* MapNode_get_next)(const C(MapNode) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapNode, minimum);
extern THIS_LIB_IMPORT thisclass(MapNode *) (* MapNode_get_minimum)(const C(MapNode) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(MapNode, maximum);
extern THIS_LIB_IMPORT thisclass(MapNode *) (* MapNode_get_maximum)(const C(MapNode) * m);

extern THIS_LIB_IMPORT void (* OldLink_free)(C(OldLink) * __this);

extern THIS_LIB_IMPORT void (* OldList_add)(C(OldList) * __this, void * item);

extern THIS_LIB_IMPORT C(bool) (* OldList_addName)(C(OldList) * __this, void * item);

extern THIS_LIB_IMPORT void (* OldList_clear)(C(OldList) * __this);

extern THIS_LIB_IMPORT void (* OldList_copy)(C(OldList) * __this, C(OldList) * src, int size, void (* copy)(void * dest, void * src));

extern THIS_LIB_IMPORT void (* OldList_delete)(C(OldList) * __this, void * item);

extern THIS_LIB_IMPORT C(OldLink) * (* OldList_findLink)(C(OldList) * __this, void * data);

extern THIS_LIB_IMPORT void * (* OldList_findName)(C(OldList) * __this, const char * name, C(bool) warn);

extern THIS_LIB_IMPORT void * (* OldList_findNamedLink)(C(OldList) * __this, const char * name, C(bool) warn);

extern THIS_LIB_IMPORT void (* OldList_free)(C(OldList) * __this, void (* freeFn)(void *));

extern THIS_LIB_IMPORT C(bool) (* OldList_insert)(C(OldList) * __this, void * prevItem, void * item);

extern THIS_LIB_IMPORT void (* OldList_move)(C(OldList) * __this, void * item, void * prevItem);

extern THIS_LIB_IMPORT C(bool) (* OldList_placeName)(C(OldList) * __this, const char * name, void ** place);

extern THIS_LIB_IMPORT void (* OldList_remove)(C(OldList) * __this, void * item);

extern THIS_LIB_IMPORT void (* OldList_removeAll)(C(OldList) * __this, void (* freeFn)(void *));

extern THIS_LIB_IMPORT void (* OldList_sort)(C(OldList) * __this, int (* compare)(void *, void *, void *), void * data);

extern THIS_LIB_IMPORT void (* OldList_swap)(C(OldList) * __this, void * item1, void * item2);

struct C(StringBinaryTree)
{
   C(BTNode) * root;
   int count;
   int (* CompareKey)(C(BinaryTree) * tree, uintptr a, uintptr b);
   void (* FreeKey)(void * key);
};
extern THIS_LIB_IMPORT void (* F(qsortr))(void * base, uintsize nel, uintsize width, int (* compare)(void * arg, const void * a, const void * b), void * arg);
extern THIS_LIB_IMPORT void (* F(qsortrx))(void * base, uintsize nel, uintsize width, int (* compare)(void * arg, const void * a, const void * b), int (* optCompareArgLast)(const void * a, const void * b, void * arg), void * arg, C(bool) deref, C(bool) ascending);

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||time  //////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef struct C(Date) C(Date);
typedef struct C(DateTime) C(DateTime);
#if CPP11
enum C(DayOfTheWeek) : int
#else
typedef int C(DayOfTheWeek);
enum C(DayOfTheWeek)
#endif
{
   DayOfTheWeek_sunday = 0x0,
   DayOfTheWeek_monday = 0x1,
   DayOfTheWeek_tuesday = 0x2,
   DayOfTheWeek_wednesday = 0x3,
   DayOfTheWeek_thursday = 0x4,
   DayOfTheWeek_friday = 0x5,
   DayOfTheWeek_saturday = 0x6
};

#if CPP11
enum C(Month) : int
#else
typedef int C(Month);
enum C(Month)
#endif
{
   Month_january = 0x0,
   Month_february = 0x1,
   Month_march = 0x2,
   Month_april = 0x3,
   Month_may = 0x4,
   Month_june = 0x5,
   Month_july = 0x6,
   Month_august = 0x7,
   Month_september = 0x8,
   Month_october = 0x9,
   Month_november = 0xA,
   Month_december = 0xB
};

typedef double C(Time);
// end -- moved backwards outputs
typedef int64 C(SecSince1970);
typedef uint C(TimeStamp32);
struct C(Date)
{
   int year;
   C(Month) month;
   int day;
};
extern THIS_LIB_IMPORT const char * (* Date_onGetStringEn)(C(Date) * __this, char * stringOutput, void * fieldData, C(ObjectNotationType) * onType);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Date, dayOfTheWeek);
extern THIS_LIB_IMPORT C(DayOfTheWeek) (* Date_get_dayOfTheWeek)(const C(Date) * d);

struct C(DateTime)
{
   int year;
   C(Month) month;
   int day;
   int hour;
   int minute;
   int second;
   C(DayOfTheWeek) dayOfTheWeek;
   int dayInTheYear;
};
extern THIS_LIB_IMPORT C(bool) (* DateTime_fixDayOfYear)(C(DateTime) * __this);

extern THIS_LIB_IMPORT C(bool) (* DateTime_getLocalTime)(C(DateTime) * __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DateTime, global);
extern THIS_LIB_IMPORT void (* DateTime_set_global)(const C(DateTime) * d, const C(DateTime) * value);
extern THIS_LIB_IMPORT void (* DateTime_get_global)(const C(DateTime) * d, C(DateTime) * value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DateTime, local);
extern THIS_LIB_IMPORT void (* DateTime_set_local)(const C(DateTime) * d, const C(DateTime) * value);
extern THIS_LIB_IMPORT void (* DateTime_get_local)(const C(DateTime) * d, C(DateTime) * value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DateTime, daysSince1970);
extern THIS_LIB_IMPORT int64 (* DateTime_get_daysSince1970)(const C(DateTime) * d);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DateTime, SecSince1970);
extern THIS_LIB_IMPORT void (* DateTime_from_SecSince1970)(const C(DateTime) * d, C(SecSince1970) value);
extern THIS_LIB_IMPORT C(SecSince1970) (* DateTime_to_SecSince1970)(const C(DateTime) * d);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(DateTime, Date);
extern THIS_LIB_IMPORT void (* DateTime_from_Date)(const C(DateTime) * d, const C(Date) * value);
extern THIS_LIB_IMPORT void (* DateTime_to_Date)(const C(DateTime) * d, C(Date) * value);

extern THIS_LIB_IMPORT int (* Month_getNumDays)(C(Month) __this, int year);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(SecSince1970, global);
extern THIS_LIB_IMPORT C(SecSince1970) (* SecSince1970_get_global)(const int64 s);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(SecSince1970, local);
extern THIS_LIB_IMPORT C(SecSince1970) (* SecSince1970_get_local)(const int64 s);

#define Seconds(x) SECONDS(x)
#define SECONDS(x)  ((C(Time))(x))
#define Time_in_Seconds(x)  ((double)(x))

#define TimeStamp(x) TIMESTAMP(x)
#define TIMESTAMP(x)  ((C(SecSince1970))(x))
#define SecSince1970_in_TimeStamp(x)  ((int64)(x))

extern THIS_LIB_IMPORT int (* F(getRandom))(int lo, int hi);
extern THIS_LIB_IMPORT C(Time) (* F(getTime))(void);
extern THIS_LIB_IMPORT void (* F(randomSeed))(uint seed);
extern THIS_LIB_IMPORT void (* F(__sleep))(C(Time) seconds);

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||files  /////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef uint32 C(FileAttribs);
struct C(FileStats)
{
   C(FileAttribs) attribs;
   uint64 size;
   C(SecSince1970) accessed;
   C(SecSince1970) modified;
   C(SecSince1970) created;
};
typedef C(Instance) C(Archive);
#if CPP11
enum C(ArchiveAddMode) : int
#else
typedef int C(ArchiveAddMode);
enum C(ArchiveAddMode)
#endif
{
   ArchiveAddMode_replace = 0x0,
   ArchiveAddMode_refresh = 0x1,
   ArchiveAddMode_update = 0x2,
   ArchiveAddMode_readOnlyDir = 0x3
};

typedef C(Instance) C(ArchiveDir);
typedef uint32 C(ArchiveOpenFlags);
typedef C(IOChannel) C(File);
typedef C(File) C(BufferedFile);
typedef C(File) C(DualPipe);
typedef uint32 C(ErrorCode);
#if CPP11
enum C(ErrorLevel) : int
#else
typedef int C(ErrorLevel);
enum C(ErrorLevel)
#endif
{
   ErrorLevel_veryFatal = 0x0,
   ErrorLevel_fatal = 0x1,
   ErrorLevel_major = 0x2,
   ErrorLevel_minor = 0x3
};

typedef uint32 C(FileChange);
typedef struct C(FileListing) C(FileListing);
#if CPP11
enum C(FileLock) : int
#else
typedef int C(FileLock);
enum C(FileLock)
#endif
{
   FileLock_unlocked = 0x0,
   FileLock_shared = 0x1,
   FileLock_exclusive = 0x2
};

typedef C(Instance) C(FileMonitor);
#if CPP11
enum C(FileOpenMode) : int
#else
typedef int C(FileOpenMode);
enum C(FileOpenMode)
#endif
{
   FileOpenMode_read = 0x1,
   FileOpenMode_write = 0x2,
   FileOpenMode_append = 0x3,
   FileOpenMode_readWrite = 0x4,
   FileOpenMode_writeRead = 0x5,
   FileOpenMode_appendRead = 0x6
};

#if CPP11
enum C(FileSeekMode) : int
#else
typedef int C(FileSeekMode);
enum C(FileSeekMode)
#endif
{
   FileSeekMode_start = 0x0,
   FileSeekMode_current = 0x1,
   FileSeekMode_end = 0x2
};

typedef uint C(FileSize);
typedef uint64 C(FileSize64);
typedef struct C(FileStats) C(FileStats);
#if CPP11
enum C(LoggingMode) : int
#else
typedef int C(LoggingMode);
enum C(LoggingMode)
#endif
{
   LoggingMode_noLogging = 0x0,
   LoggingMode_stdOut = 0x1,
   LoggingMode_stdErr = 0x2,
   LoggingMode_debug = 0x3,
   LoggingMode_logFile = 0x4,
   LoggingMode_msgBox = 0x5,
   LoggingMode_buffer = 0x6
};

typedef uint32 C(MoveFileOptions);
typedef uint32 C(PipeOpenMode);
typedef C(File) C(TempFile);
// end -- moved backwards outputs
#define AllErrors (ErrorLevel_minor)

#define AnyFileChange ((((unsigned int)(true)) | (((unsigned int)(true)) << 1) | (((unsigned int)(true)) << 2) | (((unsigned int)(true)) << 3) | (((unsigned int)(true)) << 4)))

typedef C(File) C(ConsoleFile);
#if CPP11
enum C(GuiErrorCode) : uint
#else
typedef C(ErrorCode) C(GuiErrorCode);
enum C(GuiErrorCode)
#endif
{
   GuiErrorCode_driverNotSupported = 0x101,
   GuiErrorCode_windowCreationFailed = 0x102,
   GuiErrorCode_graphicsLoadingFailed = 0x103,
   GuiErrorCode_modeSwitchFailed = 0x104
};

#if CPP11
enum C(SysErrorCode) : uint
#else
typedef C(ErrorCode) C(SysErrorCode);
enum C(SysErrorCode)
#endif
{
   SysErrorCode_allocationFailed = 0x1001,
   SysErrorCode_nameInexistant = 0x1002,
   SysErrorCode_nameExists = 0x1003,
   SysErrorCode_missingLibrary = 0x1004,
   SysErrorCode_fileNotFound = 0x3005,
   SysErrorCode_writeFailed = 0x2006
};

extern THIS_LIB_IMPORT int M_VTBLID(Archive, clear);
// C(bool) Archive_clear(C(Archive) __i);
#define Archive_clear(__i) \
   VMETHOD(CO(Archive), Archive, clear, __i, C(bool), \
      C(Archive), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, clear);

extern THIS_LIB_IMPORT int M_VTBLID(Archive, fileExists);
// C(FileAttribs) Archive_fileExists(C(Archive) __i, const char * fileName);
#define Archive_fileExists(__i, fileName) \
   VMETHOD(CO(Archive), Archive, fileExists, __i, C(FileAttribs), \
      C(Archive) _ARG const char *, \
      __i _ARG fileName)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, fileExists);

extern THIS_LIB_IMPORT int M_VTBLID(Archive, fileOpen);
// C(File) Archive_fileOpen(C(Archive) __i, const char * fileName);
#define Archive_fileOpen(__i, fileName) \
   VMETHOD(CO(Archive), Archive, fileOpen, __i, C(File), \
      C(Archive) _ARG const char *, \
      __i _ARG fileName)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, fileOpen);

extern THIS_LIB_IMPORT int M_VTBLID(Archive, fileOpenAtPosition);
// C(File) Archive_fileOpenAtPosition(C(Archive) __i, uint position);
#define Archive_fileOpenAtPosition(__i, position) \
   VMETHOD(CO(Archive), Archive, fileOpenAtPosition, __i, C(File), \
      C(Archive) _ARG uint, \
      __i _ARG position)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, fileOpenAtPosition);

extern THIS_LIB_IMPORT int M_VTBLID(Archive, fileOpenCompressed);
// C(File) Archive_fileOpenCompressed(C(Archive) __i, const char * fileName, C(bool) * isCompressed, uint64 * ucSize);
#define Archive_fileOpenCompressed(__i, fileName, isCompressed, ucSize) \
   VMETHOD(CO(Archive), Archive, fileOpenCompressed, __i, C(File), \
      C(Archive) _ARG const char * _ARG C(bool) * _ARG uint64 *, \
      __i _ARG fileName _ARG isCompressed _ARG ucSize)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, fileOpenCompressed);

extern THIS_LIB_IMPORT int M_VTBLID(Archive, openDirectory);
// C(ArchiveDir) Archive_openDirectory(C(Archive) __i, const char * name, C(FileStats) * stats, C(ArchiveAddMode) addMode);
#define Archive_openDirectory(__i, name, stats, addMode) \
   VMETHOD(CO(Archive), Archive, openDirectory, __i, C(ArchiveDir), \
      C(Archive) _ARG const char * _ARG C(FileStats) * _ARG C(ArchiveAddMode), \
      __i _ARG name _ARG stats _ARG addMode)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, openDirectory);

extern THIS_LIB_IMPORT int M_VTBLID(Archive, setBufferRead);
// void Archive_setBufferRead(C(Archive) __i, uint bufferRead);
#define Archive_setBufferRead(__i, bufferRead) \
   VMETHOD(CO(Archive), Archive, setBufferRead, __i, void, \
      C(Archive) _ARG uint, \
      __i _ARG bufferRead)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, setBufferRead);

extern THIS_LIB_IMPORT int M_VTBLID(Archive, setBufferSize);
// void Archive_setBufferSize(C(Archive) __i, uint bufferSize);
#define Archive_setBufferSize(__i, bufferSize) \
   VMETHOD(CO(Archive), Archive, setBufferSize, __i, void, \
      C(Archive) _ARG uint, \
      __i _ARG bufferSize)
extern THIS_LIB_IMPORT C(Method) * METHOD(Archive, setBufferSize);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Archive, totalSize);
extern THIS_LIB_IMPORT void (* Archive_set_totalSize)(const C(Archive) a, C(FileSize) value);
extern THIS_LIB_IMPORT C(FileSize) (* Archive_get_totalSize)(const C(Archive) a);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Archive, bufferSize);
extern THIS_LIB_IMPORT void (* Archive_set_bufferSize)(const C(Archive) a, uint value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Archive, bufferRead);
extern THIS_LIB_IMPORT void (* Archive_set_bufferRead)(const C(Archive) a, uint value);

extern THIS_LIB_IMPORT C(bool) (* ArchiveDir_add)(C(ArchiveDir) __this, const char * name, const char * path, C(ArchiveAddMode) addMode, int compression, int * ratio, uint * newPosition);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, addFromFile);
// C(bool) ArchiveDir_addFromFile(C(ArchiveDir) __i, const char * name, C(File) input, C(FileStats) * stats, C(ArchiveAddMode) addMode, int compression, int * ratio, uint * newPosition);
#define ArchiveDir_addFromFile(__i, name, input, stats, addMode, compression, ratio, newPosition) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, addFromFile, __i, C(bool), \
      C(ArchiveDir) _ARG const char * _ARG C(File) _ARG C(FileStats) * _ARG C(ArchiveAddMode) _ARG int _ARG int * _ARG uint *, \
      __i _ARG name _ARG input _ARG stats _ARG addMode _ARG compression _ARG ratio _ARG newPosition)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, addFromFile);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, addFromFileAtPosition);
// C(bool) ArchiveDir_addFromFileAtPosition(C(ArchiveDir) __i, uint position, const char * name, C(File) input, C(FileStats) * stats, C(ArchiveAddMode) addMode, int compression, int * ratio, uint * newPosition);
#define ArchiveDir_addFromFileAtPosition(__i, position, name, input, stats, addMode, compression, ratio, newPosition) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, addFromFileAtPosition, __i, C(bool), \
      C(ArchiveDir) _ARG uint _ARG const char * _ARG C(File) _ARG C(FileStats) * _ARG C(ArchiveAddMode) _ARG int _ARG int * _ARG uint *, \
      __i _ARG position _ARG name _ARG input _ARG stats _ARG addMode _ARG compression _ARG ratio _ARG newPosition)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, addFromFileAtPosition);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, delete);
// C(bool) ArchiveDir_delete(C(ArchiveDir) __i, const char * fileName);
#define ArchiveDir_delete(__i, fileName) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, delete, __i, C(bool), \
      C(ArchiveDir) _ARG const char *, \
      __i _ARG fileName)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, delete);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, fileExists);
// C(FileAttribs) ArchiveDir_fileExists(C(ArchiveDir) __i, const char * fileName);
#define ArchiveDir_fileExists(__i, fileName) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, fileExists, __i, C(FileAttribs), \
      C(ArchiveDir) _ARG const char *, \
      __i _ARG fileName)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, fileExists);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, fileOpen);
// C(File) ArchiveDir_fileOpen(C(ArchiveDir) __i, const char * fileName);
#define ArchiveDir_fileOpen(__i, fileName) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, fileOpen, __i, C(File), \
      C(ArchiveDir) _ARG const char *, \
      __i _ARG fileName)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, fileOpen);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, move);
// C(bool) ArchiveDir_move(C(ArchiveDir) __i, const char * name, C(ArchiveDir) to);
#define ArchiveDir_move(__i, name, to) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, move, __i, C(bool), \
      C(ArchiveDir) _ARG const char * _ARG C(ArchiveDir), \
      __i _ARG name _ARG to)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, move);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, openDirectory);
// C(ArchiveDir) ArchiveDir_openDirectory(C(ArchiveDir) __i, const char * name, C(FileStats) * stats, C(ArchiveAddMode) addMode);
#define ArchiveDir_openDirectory(__i, name, stats, addMode) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, openDirectory, __i, C(ArchiveDir), \
      C(ArchiveDir) _ARG const char * _ARG C(FileStats) * _ARG C(ArchiveAddMode), \
      __i _ARG name _ARG stats _ARG addMode)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, openDirectory);

extern THIS_LIB_IMPORT int M_VTBLID(ArchiveDir, rename);
// C(bool) ArchiveDir_rename(C(ArchiveDir) __i, const char * name, const char * newName);
#define ArchiveDir_rename(__i, name, newName) \
   VMETHOD(CO(ArchiveDir), ArchiveDir, rename, __i, C(bool), \
      C(ArchiveDir) _ARG const char * _ARG const char *, \
      __i _ARG name _ARG newName)
extern THIS_LIB_IMPORT C(Method) * METHOD(ArchiveDir, rename);

#define ARCHIVEOPENFLAGS_writeAccess_SHIFT               0
#define ARCHIVEOPENFLAGS_writeAccess_MASK                0x1
#define ARCHIVEOPENFLAGS_writeAccess(x)                  ((((C(ArchiveOpenFlags))(x)) & ARCHIVEOPENFLAGS_writeAccess_MASK) >> ARCHIVEOPENFLAGS_writeAccess_SHIFT)
#define ARCHIVEOPENFLAGS_SET_writeAccess(x, writeAccess)           (x) = ((C(ArchiveOpenFlags))(x) & ~((C(ArchiveOpenFlags))ARCHIVEOPENFLAGS_writeAccess_MASK)) | (((C(ArchiveOpenFlags))(writeAccess)) << ARCHIVEOPENFLAGS_writeAccess_SHIFT)
#define ARCHIVEOPENFLAGS_buffered_SHIFT                  1
#define ARCHIVEOPENFLAGS_buffered_MASK                   0x2
#define ARCHIVEOPENFLAGS_buffered(x)                     ((((C(ArchiveOpenFlags))(x)) & ARCHIVEOPENFLAGS_buffered_MASK) >> ARCHIVEOPENFLAGS_buffered_SHIFT)
#define ARCHIVEOPENFLAGS_SET_buffered(x, buffered)              (x) = ((C(ArchiveOpenFlags))(x) & ~((C(ArchiveOpenFlags))ARCHIVEOPENFLAGS_buffered_MASK)) | (((C(ArchiveOpenFlags))(buffered)) << ARCHIVEOPENFLAGS_buffered_SHIFT)
#define ARCHIVEOPENFLAGS_exclusive_SHIFT                 2
#define ARCHIVEOPENFLAGS_exclusive_MASK                  0x4
#define ARCHIVEOPENFLAGS_exclusive(x)                    ((((C(ArchiveOpenFlags))(x)) & ARCHIVEOPENFLAGS_exclusive_MASK) >> ARCHIVEOPENFLAGS_exclusive_SHIFT)
#define ARCHIVEOPENFLAGS_SET_exclusive(x, exclusive)             (x) = ((C(ArchiveOpenFlags))(x) & ~((C(ArchiveOpenFlags))ARCHIVEOPENFLAGS_exclusive_MASK)) | (((C(ArchiveOpenFlags))(exclusive)) << ARCHIVEOPENFLAGS_exclusive_SHIFT)
#define ARCHIVEOPENFLAGS_waitLock_SHIFT                  3
#define ARCHIVEOPENFLAGS_waitLock_MASK                   0x8
#define ARCHIVEOPENFLAGS_waitLock(x)                     ((((C(ArchiveOpenFlags))(x)) & ARCHIVEOPENFLAGS_waitLock_MASK) >> ARCHIVEOPENFLAGS_waitLock_SHIFT)
#define ARCHIVEOPENFLAGS_SET_waitLock(x, waitLock)              (x) = ((C(ArchiveOpenFlags))(x) & ~((C(ArchiveOpenFlags))ARCHIVEOPENFLAGS_waitLock_MASK)) | (((C(ArchiveOpenFlags))(waitLock)) << ARCHIVEOPENFLAGS_waitLock_SHIFT)
#define ARCHIVEOPENFLAGS(writeAccess, buffered, exclusive, waitLock)                      ((((((C(ArchiveOpenFlags))(writeAccess)) << ARCHIVEOPENFLAGS_writeAccess_SHIFT) | ((C(ArchiveOpenFlags))(buffered)) << ARCHIVEOPENFLAGS_buffered_SHIFT) | ((C(ArchiveOpenFlags))(exclusive)) << ARCHIVEOPENFLAGS_exclusive_SHIFT) | ((C(ArchiveOpenFlags))(waitLock)) << ARCHIVEOPENFLAGS_waitLock_SHIFT)


extern THIS_LIB_IMPORT C(Property) * PROPERTY(BufferedFile, handle);
extern THIS_LIB_IMPORT void (* BufferedFile_set_handle)(const C(BufferedFile) b, C(File) value);
extern THIS_LIB_IMPORT C(File) (* BufferedFile_get_handle)(const C(BufferedFile) b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BufferedFile, bufferSize);
extern THIS_LIB_IMPORT void (* BufferedFile_set_bufferSize)(const C(BufferedFile) b, uintsize value);
extern THIS_LIB_IMPORT uintsize (* BufferedFile_get_bufferSize)(const C(BufferedFile) b);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(BufferedFile, bufferRead);
extern THIS_LIB_IMPORT void (* BufferedFile_set_bufferRead)(const C(BufferedFile) b, uintsize value);
extern THIS_LIB_IMPORT uintsize (* BufferedFile_get_bufferRead)(const C(BufferedFile) b);

extern THIS_LIB_IMPORT int (* DualPipe_getExitCode)(C(DualPipe) __this);

extern THIS_LIB_IMPORT C(bool) (* DualPipe_getLinePeek)(C(DualPipe) __this, char * s, int max, int * charsRead);

extern THIS_LIB_IMPORT int (* DualPipe_getProcessID)(C(DualPipe) __this);

extern THIS_LIB_IMPORT C(bool) (* DualPipe_peek)(C(DualPipe) __this);

extern THIS_LIB_IMPORT void (* DualPipe_terminate)(C(DualPipe) __this);

extern THIS_LIB_IMPORT void (* DualPipe_wait)(C(DualPipe) __this);

#define ERRORCODE_level_SHIFT                            12
#define ERRORCODE_level_MASK                             0x3000
#define ERRORCODE_level(x)                               ((((C(ErrorCode))(x)) & ERRORCODE_level_MASK) >> ERRORCODE_level_SHIFT)
#define ERRORCODE_SET_level(x, level)                        (x) = ((C(ErrorCode))(x) & ~((C(ErrorCode))ERRORCODE_level_MASK)) | (((C(ErrorCode))(level)) << ERRORCODE_level_SHIFT)
#define ERRORCODE_code_SHIFT                             0
#define ERRORCODE_code_MASK                              0xFFF
#define ERRORCODE_code(x)                                ((((C(ErrorCode))(x)) & ERRORCODE_code_MASK) >> ERRORCODE_code_SHIFT)
#define ERRORCODE_SET_code(x, code)                         (x) = ((C(ErrorCode))(x) & ~((C(ErrorCode))ERRORCODE_code_MASK)) | (((C(ErrorCode))(code)) << ERRORCODE_code_SHIFT)
#define ERRORCODE(level, code)                                 ((((C(ErrorCode))(level)) << ERRORCODE_level_SHIFT) | ((C(ErrorCode))(code)) << ERRORCODE_code_SHIFT)


extern THIS_LIB_IMPORT int M_VTBLID(File, close);
// void File_close(C(File) __i);
#define File_close(__i) \
   VMETHOD(CO(File), File, close, __i, void, \
      C(File), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, close);

extern THIS_LIB_IMPORT int M_VTBLID(File, closeInput);
// void File_closeInput(C(File) __i);
#define File_closeInput(__i) \
   VMETHOD(CO(File), File, closeInput, __i, void, \
      C(File), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, closeInput);

extern THIS_LIB_IMPORT int M_VTBLID(File, closeOutput);
// void File_closeOutput(C(File) __i);
#define File_closeOutput(__i) \
   VMETHOD(CO(File), File, closeOutput, __i, void, \
      C(File), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, closeOutput);

extern THIS_LIB_IMPORT C(bool) (* File_copyTo)(C(File) __this, const char * outputFileName);

extern THIS_LIB_IMPORT C(bool) (* File_copyToFile)(C(File) __this, C(File) f);

extern THIS_LIB_IMPORT int M_VTBLID(File, eof);
// C(bool) File_eof(C(File) __i);
#define File_eof(__i) \
   VMETHOD(CO(File), File, eof, __i, C(bool), \
      C(File), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, eof);

extern THIS_LIB_IMPORT C(bool) (* File_flush)(C(File) __this);

extern THIS_LIB_IMPORT double (* File_getDouble)(C(File) __this);

extern THIS_LIB_IMPORT float (* File_getFloat)(C(File) __this);

extern THIS_LIB_IMPORT uint (* File_getHexValue)(C(File) __this);

extern THIS_LIB_IMPORT C(bool) (* File_getLine)(C(File) __this, char * s, int max);

extern THIS_LIB_IMPORT int (* File_getLineEx)(C(File) __this, char * s, int max, C(bool) * hasNewLineChar);

extern THIS_LIB_IMPORT int M_VTBLID(File, getSize);
// uint64 File_getSize(C(File) __i);
#define File_getSize(__i) \
   VMETHOD(CO(File), File, getSize, __i, uint64, \
      C(File), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, getSize);

extern THIS_LIB_IMPORT C(bool) (* File_getString)(C(File) __this, char * string, int max);

extern THIS_LIB_IMPORT int (* File_getValue)(C(File) __this);

extern THIS_LIB_IMPORT int M_VTBLID(File, getc);
// C(bool) File_getc(C(File) __i, char * ch);
#define File_getc(__i, ch) \
   VMETHOD(CO(File), File, getc, __i, C(bool), \
      C(File) _ARG char *, \
      __i _ARG ch)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, getc);

extern THIS_LIB_IMPORT int M_VTBLID(File, lock);
// C(bool) File_lock(C(File) __i, C(FileLock) type, uint64 start, uint64 length, C(bool) wait);
#define File_lock(__i, type, start, length, wait) \
   VMETHOD(CO(File), File, lock, __i, C(bool), \
      C(File) _ARG C(FileLock) _ARG uint64 _ARG uint64 _ARG C(bool), \
      __i _ARG type _ARG start _ARG length _ARG wait)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, lock);

extern THIS_LIB_IMPORT void (* File_print)(C(File) __this, typed_object_class_ptr class_object, const void * object, ...);

extern THIS_LIB_IMPORT void (* File_printLn)(C(File) __this, typed_object_class_ptr class_object, const void * object, ...);

extern THIS_LIB_IMPORT int (* File_printf)(C(File) __this, const char * format, ...);

extern THIS_LIB_IMPORT int M_VTBLID(File, putc);
// C(bool) File_putc(C(File) __i, char ch);
#define File_putc(__i, ch) \
   VMETHOD(CO(File), File, putc, __i, C(bool), \
      C(File) _ARG char, \
      __i _ARG ch)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, putc);

extern THIS_LIB_IMPORT int M_VTBLID(File, puts);
// C(bool) File_puts(C(File) __i, const char * string);
#define File_puts(__i, string) \
   VMETHOD(CO(File), File, puts, __i, C(bool), \
      C(File) _ARG const char *, \
      __i _ARG string)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, puts);

extern THIS_LIB_IMPORT int M_VTBLID(File, read);
// uintsize File_read(C(File) __i, void * buffer, uintsize size, uintsize count);
#define File_read(__i, buffer, size, count) \
   VMETHOD(CO(File), File, read, __i, uintsize, \
      C(File) _ARG void * _ARG uintsize _ARG uintsize, \
      __i _ARG buffer _ARG size _ARG count)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, read);

extern THIS_LIB_IMPORT int M_VTBLID(File, seek);
// C(bool) File_seek(C(File) __i, int64 pos, C(FileSeekMode) mode);
#define File_seek(__i, pos, mode) \
   VMETHOD(CO(File), File, seek, __i, C(bool), \
      C(File) _ARG int64 _ARG C(FileSeekMode), \
      __i _ARG pos _ARG mode)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, seek);

extern THIS_LIB_IMPORT int M_VTBLID(File, tell);
// uint64 File_tell(C(File) __i);
#define File_tell(__i) \
   VMETHOD(CO(File), File, tell, __i, uint64, \
      C(File), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, tell);

extern THIS_LIB_IMPORT int M_VTBLID(File, truncate);
// C(bool) File_truncate(C(File) __i, uint64 size);
#define File_truncate(__i, size) \
   VMETHOD(CO(File), File, truncate, __i, C(bool), \
      C(File) _ARG uint64, \
      __i _ARG size)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, truncate);

extern THIS_LIB_IMPORT int M_VTBLID(File, unlock);
// C(bool) File_unlock(C(File) __i, uint64 start, uint64 length, C(bool) wait);
#define File_unlock(__i, start, length, wait) \
   VMETHOD(CO(File), File, unlock, __i, C(bool), \
      C(File) _ARG uint64 _ARG uint64 _ARG C(bool), \
      __i _ARG start _ARG length _ARG wait)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, unlock);

extern THIS_LIB_IMPORT int M_VTBLID(File, write);
// uintsize File_write(C(File) __i, const void * buffer, uintsize size, uintsize count);
#define File_write(__i, buffer, size, count) \
   VMETHOD(CO(File), File, write, __i, uintsize, \
      C(File) _ARG const void * _ARG uintsize _ARG uintsize, \
      __i _ARG buffer _ARG size _ARG count)
extern THIS_LIB_IMPORT C(Method) * METHOD(File, write);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(File, input);
extern THIS_LIB_IMPORT void (* File_set_input)(const C(File) f, void * value);
extern THIS_LIB_IMPORT void * (* File_get_input)(const C(File) f);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(File, output);
extern THIS_LIB_IMPORT void (* File_set_output)(const C(File) f, void * value);
extern THIS_LIB_IMPORT void * (* File_get_output)(const C(File) f);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(File, buffered);
extern THIS_LIB_IMPORT void (* File_set_buffered)(const C(File) f, C(bool) value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(File, eof);
extern THIS_LIB_IMPORT C(bool) (* File_get_eof)(const C(File) f);

#define FILEATTRIBS_isFile_SHIFT                         0
#define FILEATTRIBS_isFile_MASK                          0x1
#define FILEATTRIBS_isFile(x)                            ((((C(FileAttribs))(x)) & FILEATTRIBS_isFile_MASK) >> FILEATTRIBS_isFile_SHIFT)
#define FILEATTRIBS_SET_isFile(x, isFile)                     (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isFile_MASK)) | (((C(FileAttribs))(isFile)) << FILEATTRIBS_isFile_SHIFT)
#define FILEATTRIBS_isArchive_SHIFT                      1
#define FILEATTRIBS_isArchive_MASK                       0x2
#define FILEATTRIBS_isArchive(x)                         ((((C(FileAttribs))(x)) & FILEATTRIBS_isArchive_MASK) >> FILEATTRIBS_isArchive_SHIFT)
#define FILEATTRIBS_SET_isArchive(x, isArchive)                  (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isArchive_MASK)) | (((C(FileAttribs))(isArchive)) << FILEATTRIBS_isArchive_SHIFT)
#define FILEATTRIBS_isHidden_SHIFT                       2
#define FILEATTRIBS_isHidden_MASK                        0x4
#define FILEATTRIBS_isHidden(x)                          ((((C(FileAttribs))(x)) & FILEATTRIBS_isHidden_MASK) >> FILEATTRIBS_isHidden_SHIFT)
#define FILEATTRIBS_SET_isHidden(x, isHidden)                   (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isHidden_MASK)) | (((C(FileAttribs))(isHidden)) << FILEATTRIBS_isHidden_SHIFT)
#define FILEATTRIBS_isReadOnly_SHIFT                     3
#define FILEATTRIBS_isReadOnly_MASK                      0x8
#define FILEATTRIBS_isReadOnly(x)                        ((((C(FileAttribs))(x)) & FILEATTRIBS_isReadOnly_MASK) >> FILEATTRIBS_isReadOnly_SHIFT)
#define FILEATTRIBS_SET_isReadOnly(x, isReadOnly)                 (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isReadOnly_MASK)) | (((C(FileAttribs))(isReadOnly)) << FILEATTRIBS_isReadOnly_SHIFT)
#define FILEATTRIBS_isSystem_SHIFT                       4
#define FILEATTRIBS_isSystem_MASK                        0x10
#define FILEATTRIBS_isSystem(x)                          ((((C(FileAttribs))(x)) & FILEATTRIBS_isSystem_MASK) >> FILEATTRIBS_isSystem_SHIFT)
#define FILEATTRIBS_SET_isSystem(x, isSystem)                   (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isSystem_MASK)) | (((C(FileAttribs))(isSystem)) << FILEATTRIBS_isSystem_SHIFT)
#define FILEATTRIBS_isTemporary_SHIFT                    5
#define FILEATTRIBS_isTemporary_MASK                     0x20
#define FILEATTRIBS_isTemporary(x)                       ((((C(FileAttribs))(x)) & FILEATTRIBS_isTemporary_MASK) >> FILEATTRIBS_isTemporary_SHIFT)
#define FILEATTRIBS_SET_isTemporary(x, isTemporary)                (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isTemporary_MASK)) | (((C(FileAttribs))(isTemporary)) << FILEATTRIBS_isTemporary_SHIFT)
#define FILEATTRIBS_isDirectory_SHIFT                    6
#define FILEATTRIBS_isDirectory_MASK                     0x40
#define FILEATTRIBS_isDirectory(x)                       ((((C(FileAttribs))(x)) & FILEATTRIBS_isDirectory_MASK) >> FILEATTRIBS_isDirectory_SHIFT)
#define FILEATTRIBS_SET_isDirectory(x, isDirectory)                (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isDirectory_MASK)) | (((C(FileAttribs))(isDirectory)) << FILEATTRIBS_isDirectory_SHIFT)
#define FILEATTRIBS_isDrive_SHIFT                        7
#define FILEATTRIBS_isDrive_MASK                         0x80
#define FILEATTRIBS_isDrive(x)                           ((((C(FileAttribs))(x)) & FILEATTRIBS_isDrive_MASK) >> FILEATTRIBS_isDrive_SHIFT)
#define FILEATTRIBS_SET_isDrive(x, isDrive)                    (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isDrive_MASK)) | (((C(FileAttribs))(isDrive)) << FILEATTRIBS_isDrive_SHIFT)
#define FILEATTRIBS_isCDROM_SHIFT                        8
#define FILEATTRIBS_isCDROM_MASK                         0x100
#define FILEATTRIBS_isCDROM(x)                           ((((C(FileAttribs))(x)) & FILEATTRIBS_isCDROM_MASK) >> FILEATTRIBS_isCDROM_SHIFT)
#define FILEATTRIBS_SET_isCDROM(x, isCDROM)                    (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isCDROM_MASK)) | (((C(FileAttribs))(isCDROM)) << FILEATTRIBS_isCDROM_SHIFT)
#define FILEATTRIBS_isRemote_SHIFT                       9
#define FILEATTRIBS_isRemote_MASK                        0x200
#define FILEATTRIBS_isRemote(x)                          ((((C(FileAttribs))(x)) & FILEATTRIBS_isRemote_MASK) >> FILEATTRIBS_isRemote_SHIFT)
#define FILEATTRIBS_SET_isRemote(x, isRemote)                   (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isRemote_MASK)) | (((C(FileAttribs))(isRemote)) << FILEATTRIBS_isRemote_SHIFT)
#define FILEATTRIBS_isRemovable_SHIFT                    10
#define FILEATTRIBS_isRemovable_MASK                     0x400
#define FILEATTRIBS_isRemovable(x)                       ((((C(FileAttribs))(x)) & FILEATTRIBS_isRemovable_MASK) >> FILEATTRIBS_isRemovable_SHIFT)
#define FILEATTRIBS_SET_isRemovable(x, isRemovable)                (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isRemovable_MASK)) | (((C(FileAttribs))(isRemovable)) << FILEATTRIBS_isRemovable_SHIFT)
#define FILEATTRIBS_isServer_SHIFT                       11
#define FILEATTRIBS_isServer_MASK                        0x800
#define FILEATTRIBS_isServer(x)                          ((((C(FileAttribs))(x)) & FILEATTRIBS_isServer_MASK) >> FILEATTRIBS_isServer_SHIFT)
#define FILEATTRIBS_SET_isServer(x, isServer)                   (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isServer_MASK)) | (((C(FileAttribs))(isServer)) << FILEATTRIBS_isServer_SHIFT)
#define FILEATTRIBS_isShare_SHIFT                        12
#define FILEATTRIBS_isShare_MASK                         0x1000
#define FILEATTRIBS_isShare(x)                           ((((C(FileAttribs))(x)) & FILEATTRIBS_isShare_MASK) >> FILEATTRIBS_isShare_SHIFT)
#define FILEATTRIBS_SET_isShare(x, isShare)                    (x) = ((C(FileAttribs))(x) & ~((C(FileAttribs))FILEATTRIBS_isShare_MASK)) | (((C(FileAttribs))(isShare)) << FILEATTRIBS_isShare_SHIFT)


#define FILECHANGE_created_SHIFT                         0
#define FILECHANGE_created_MASK                          0x1
#define FILECHANGE_created(x)                            ((((C(FileChange))(x)) & FILECHANGE_created_MASK) >> FILECHANGE_created_SHIFT)
#define FILECHANGE_SET_created(x, created)                     (x) = ((C(FileChange))(x) & ~((C(FileChange))FILECHANGE_created_MASK)) | (((C(FileChange))(created)) << FILECHANGE_created_SHIFT)
#define FILECHANGE_renamed_SHIFT                         1
#define FILECHANGE_renamed_MASK                          0x2
#define FILECHANGE_renamed(x)                            ((((C(FileChange))(x)) & FILECHANGE_renamed_MASK) >> FILECHANGE_renamed_SHIFT)
#define FILECHANGE_SET_renamed(x, renamed)                     (x) = ((C(FileChange))(x) & ~((C(FileChange))FILECHANGE_renamed_MASK)) | (((C(FileChange))(renamed)) << FILECHANGE_renamed_SHIFT)
#define FILECHANGE_modified_SHIFT                        2
#define FILECHANGE_modified_MASK                         0x4
#define FILECHANGE_modified(x)                           ((((C(FileChange))(x)) & FILECHANGE_modified_MASK) >> FILECHANGE_modified_SHIFT)
#define FILECHANGE_SET_modified(x, modified)                    (x) = ((C(FileChange))(x) & ~((C(FileChange))FILECHANGE_modified_MASK)) | (((C(FileChange))(modified)) << FILECHANGE_modified_SHIFT)
#define FILECHANGE_deleted_SHIFT                         3
#define FILECHANGE_deleted_MASK                          0x8
#define FILECHANGE_deleted(x)                            ((((C(FileChange))(x)) & FILECHANGE_deleted_MASK) >> FILECHANGE_deleted_SHIFT)
#define FILECHANGE_SET_deleted(x, deleted)                     (x) = ((C(FileChange))(x) & ~((C(FileChange))FILECHANGE_deleted_MASK)) | (((C(FileChange))(deleted)) << FILECHANGE_deleted_SHIFT)
#define FILECHANGE_attribs_SHIFT                         4
#define FILECHANGE_attribs_MASK                          0x10
#define FILECHANGE_attribs(x)                            ((((C(FileChange))(x)) & FILECHANGE_attribs_MASK) >> FILECHANGE_attribs_SHIFT)
#define FILECHANGE_SET_attribs(x, attribs)                     (x) = ((C(FileChange))(x) & ~((C(FileChange))FILECHANGE_attribs_MASK)) | (((C(FileChange))(attribs)) << FILECHANGE_attribs_SHIFT)
#define FILECHANGE(created, renamed, modified, deleted, attribs)                          (((((((C(FileChange))(created)) << FILECHANGE_created_SHIFT) | ((C(FileChange))(renamed)) << FILECHANGE_renamed_SHIFT) | ((C(FileChange))(modified)) << FILECHANGE_modified_SHIFT) | ((C(FileChange))(deleted)) << FILECHANGE_deleted_SHIFT) | ((C(FileChange))(attribs)) << FILECHANGE_attribs_SHIFT)


struct C(FileListing)
{
   const char * directory;
   const char * extensions;
   byte __ecere_padding[8];
};
extern THIS_LIB_IMPORT C(bool) (* FileListing_find)(C(FileListing) * __this);

extern THIS_LIB_IMPORT void (* FileListing_stop)(C(FileListing) * __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(FileListing, name);
extern THIS_LIB_IMPORT const char * (* FileListing_get_name)(const C(FileListing) * f);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(FileListing, path);
extern THIS_LIB_IMPORT const char * (* FileListing_get_path)(const C(FileListing) * f);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(FileListing, stats);
extern THIS_LIB_IMPORT void (* FileListing_get_stats)(const C(FileListing) * f, C(FileStats) * value);

extern THIS_LIB_IMPORT int M_VTBLID(FileMonitor, onDirNotify);
// C(bool) FileMonitor_onDirNotify(C(FileMonitor) __i, any_object __t, C(FileChange) action, const char * fileName, const char * param);
#define FileMonitor_onDirNotify(__i, __t, action, fileName, param) \
   VMETHOD(CO(FileMonitor), FileMonitor, onDirNotify, __i, C(bool), \
      any_object _ARG C(FileChange) _ARG const char * _ARG const char *, \
      __t _ARG action _ARG fileName _ARG param)
extern THIS_LIB_IMPORT C(Method) * METHOD(FileMonitor, onDirNotify);

extern THIS_LIB_IMPORT int M_VTBLID(FileMonitor, onFileNotify);
// C(bool) FileMonitor_onFileNotify(C(FileMonitor) __i, any_object __t, C(FileChange) action, const char * param);
#define FileMonitor_onFileNotify(__i, __t, action, param) \
   VMETHOD(CO(FileMonitor), FileMonitor, onFileNotify, __i, C(bool), \
      any_object _ARG C(FileChange) _ARG const char *, \
      __t _ARG action _ARG param)
extern THIS_LIB_IMPORT C(Method) * METHOD(FileMonitor, onFileNotify);

extern THIS_LIB_IMPORT void (* FileMonitor_startMonitoring)(C(FileMonitor) __this);

extern THIS_LIB_IMPORT void (* FileMonitor_stopMonitoring)(C(FileMonitor) __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(FileMonitor, userData);
extern THIS_LIB_IMPORT void (* FileMonitor_set_userData)(const C(FileMonitor) f, void * value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(FileMonitor, fileChange);
extern THIS_LIB_IMPORT void (* FileMonitor_set_fileChange)(const C(FileMonitor) f, C(FileChange) value);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(FileMonitor, fileName);
extern THIS_LIB_IMPORT void (* FileMonitor_set_fileName)(const C(FileMonitor) f, const char * value);
extern THIS_LIB_IMPORT const char * (* FileMonitor_get_fileName)(const C(FileMonitor) f);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(FileMonitor, directoryName);
extern THIS_LIB_IMPORT void (* FileMonitor_set_directoryName)(const C(FileMonitor) f, const char * value);
extern THIS_LIB_IMPORT const char * (* FileMonitor_get_directoryName)(const C(FileMonitor) f);

#define MOVEFILEOPTIONS_overwrite_SHIFT                  0
#define MOVEFILEOPTIONS_overwrite_MASK                   0x1
#define MOVEFILEOPTIONS_overwrite(x)                     ((((C(MoveFileOptions))(x)) & MOVEFILEOPTIONS_overwrite_MASK) >> MOVEFILEOPTIONS_overwrite_SHIFT)
#define MOVEFILEOPTIONS_SET_overwrite(x, overwrite)              (x) = ((C(MoveFileOptions))(x) & ~((C(MoveFileOptions))MOVEFILEOPTIONS_overwrite_MASK)) | (((C(MoveFileOptions))(overwrite)) << MOVEFILEOPTIONS_overwrite_SHIFT)
#define MOVEFILEOPTIONS_sync_SHIFT                       1
#define MOVEFILEOPTIONS_sync_MASK                        0x2
#define MOVEFILEOPTIONS_sync(x)                          ((((C(MoveFileOptions))(x)) & MOVEFILEOPTIONS_sync_MASK) >> MOVEFILEOPTIONS_sync_SHIFT)
#define MOVEFILEOPTIONS_SET_sync(x, sync)                   (x) = ((C(MoveFileOptions))(x) & ~((C(MoveFileOptions))MOVEFILEOPTIONS_sync_MASK)) | (((C(MoveFileOptions))(sync)) << MOVEFILEOPTIONS_sync_SHIFT)
#define MOVEFILEOPTIONS(overwrite, sync)                           ((((C(MoveFileOptions))(overwrite)) << MOVEFILEOPTIONS_overwrite_SHIFT) | ((C(MoveFileOptions))(sync)) << MOVEFILEOPTIONS_sync_SHIFT)


#define PIPEOPENMODE_output_SHIFT                        0
#define PIPEOPENMODE_output_MASK                         0x1
#define PIPEOPENMODE_output(x)                           ((((C(PipeOpenMode))(x)) & PIPEOPENMODE_output_MASK) >> PIPEOPENMODE_output_SHIFT)
#define PIPEOPENMODE_SET_output(x, output)                    (x) = ((C(PipeOpenMode))(x) & ~((C(PipeOpenMode))PIPEOPENMODE_output_MASK)) | (((C(PipeOpenMode))(output)) << PIPEOPENMODE_output_SHIFT)
#define PIPEOPENMODE_error_SHIFT                         1
#define PIPEOPENMODE_error_MASK                          0x2
#define PIPEOPENMODE_error(x)                            ((((C(PipeOpenMode))(x)) & PIPEOPENMODE_error_MASK) >> PIPEOPENMODE_error_SHIFT)
#define PIPEOPENMODE_SET_error(x, error)                     (x) = ((C(PipeOpenMode))(x) & ~((C(PipeOpenMode))PIPEOPENMODE_error_MASK)) | (((C(PipeOpenMode))(error)) << PIPEOPENMODE_error_SHIFT)
#define PIPEOPENMODE_input_SHIFT                         2
#define PIPEOPENMODE_input_MASK                          0x4
#define PIPEOPENMODE_input(x)                            ((((C(PipeOpenMode))(x)) & PIPEOPENMODE_input_MASK) >> PIPEOPENMODE_input_SHIFT)
#define PIPEOPENMODE_SET_input(x, input)                     (x) = ((C(PipeOpenMode))(x) & ~((C(PipeOpenMode))PIPEOPENMODE_input_MASK)) | (((C(PipeOpenMode))(input)) << PIPEOPENMODE_input_SHIFT)
#define PIPEOPENMODE_showWindow_SHIFT                    3
#define PIPEOPENMODE_showWindow_MASK                     0x8
#define PIPEOPENMODE_showWindow(x)                       ((((C(PipeOpenMode))(x)) & PIPEOPENMODE_showWindow_MASK) >> PIPEOPENMODE_showWindow_SHIFT)
#define PIPEOPENMODE_SET_showWindow(x, showWindow)                (x) = ((C(PipeOpenMode))(x) & ~((C(PipeOpenMode))PIPEOPENMODE_showWindow_MASK)) | (((C(PipeOpenMode))(showWindow)) << PIPEOPENMODE_showWindow_SHIFT)
#define PIPEOPENMODE(output, error, input, showWindow)                          ((((((C(PipeOpenMode))(output)) << PIPEOPENMODE_output_SHIFT) | ((C(PipeOpenMode))(error)) << PIPEOPENMODE_error_SHIFT) | ((C(PipeOpenMode))(input)) << PIPEOPENMODE_input_SHIFT) | ((C(PipeOpenMode))(showWindow)) << PIPEOPENMODE_showWindow_SHIFT)


extern THIS_LIB_IMPORT byte * (* TempFile_stealBuffer)(C(TempFile) __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(TempFile, openMode);
extern THIS_LIB_IMPORT void (* TempFile_set_openMode)(const C(TempFile) t, C(FileOpenMode) value);
extern THIS_LIB_IMPORT C(FileOpenMode) (* TempFile_get_openMode)(const C(TempFile) t);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(TempFile, buffer);
extern THIS_LIB_IMPORT void (* TempFile_set_buffer)(const C(TempFile) t, byte * value);
extern THIS_LIB_IMPORT byte * (* TempFile_get_buffer)(const C(TempFile) t);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(TempFile, size);
extern THIS_LIB_IMPORT void (* TempFile_set_size)(const C(TempFile) t, uintsize value);
extern THIS_LIB_IMPORT uintsize (* TempFile_get_size)(const C(TempFile) t);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(TempFile, allocated);
extern THIS_LIB_IMPORT void (* TempFile_set_allocated)(const C(TempFile) t, uintsize value);
extern THIS_LIB_IMPORT uintsize (* TempFile_get_allocated)(const C(TempFile) t);

extern THIS_LIB_IMPORT C(Archive) (* F(archiveOpen))(const char * fileName, C(ArchiveOpenFlags) flags);
extern THIS_LIB_IMPORT C(bool) (* F(archiveQuerySize))(const char * fileName, C(FileSize) * size);
extern THIS_LIB_IMPORT C(bool) (* F(changeWorkingDir))(const char * buf);
extern THIS_LIB_IMPORT char * (* F(copySystemPath))(const char * p);
extern THIS_LIB_IMPORT char * (* F(copyUnixPath))(const char * p);
extern THIS_LIB_IMPORT void (* F(createTemporaryDir))(char * tempFileName, const char * _template);
extern THIS_LIB_IMPORT C(File) (* F(createTemporaryFile))(char * tempFileName, const char * _template);
extern THIS_LIB_IMPORT C(bool) (* F(deleteFile))(const char * fileName);
extern THIS_LIB_IMPORT C(DualPipe) (* F(dualPipeOpen))(C(PipeOpenMode) mode, const char * commandLine);
extern THIS_LIB_IMPORT C(DualPipe) (* F(dualPipeOpenEnv))(C(PipeOpenMode) mode, const char * env, const char * commandLine);
extern THIS_LIB_IMPORT C(DualPipe) (* F(dualPipeOpenEnvf))(C(PipeOpenMode) mode, const char * env, const char * command, ...);
extern THIS_LIB_IMPORT C(DualPipe) (* F(dualPipeOpenf))(C(PipeOpenMode) mode, const char * command, ...);
extern THIS_LIB_IMPORT void (* F(dumpErrors))(C(bool) display);
extern THIS_LIB_IMPORT C(bool) (* F(execute))(const char * command, ...);
extern THIS_LIB_IMPORT C(bool) (* F(executeEnv))(const char * env, const char * command, ...);
extern THIS_LIB_IMPORT C(bool) (* F(executeWait))(const char * command, ...);
extern THIS_LIB_IMPORT C(FileAttribs) (* F(fileExists))(const char * fileName);
extern THIS_LIB_IMPORT void (* F(fileFixCase))(char * file);
extern THIS_LIB_IMPORT C(bool) (* F(fileGetSize))(const char * fileName, C(FileSize) * size);
extern THIS_LIB_IMPORT C(bool) (* F(fileGetStats))(const char * fileName, C(FileStats) * stats);
extern THIS_LIB_IMPORT C(File) (* F(fileOpen))(const char * fileName, C(FileOpenMode) mode);
extern THIS_LIB_IMPORT C(BufferedFile) (* F(fileOpenBuffered))(const char * fileName, C(FileOpenMode) mode);
extern THIS_LIB_IMPORT C(bool) (* F(fileSetAttribs))(const char * fileName, C(FileAttribs) attribs);
extern THIS_LIB_IMPORT C(bool) (* F(fileSetTime))(const char * fileName, C(SecSince1970) created, C(SecSince1970) accessed, C(SecSince1970) modified);
extern THIS_LIB_IMPORT C(bool) (* F(fileTruncate))(const char * fileName, uint64 size);
extern THIS_LIB_IMPORT char * (* F(getEnvironment))(const char * envName, char * envValue, int max);
extern THIS_LIB_IMPORT void (* F(getFreeSpace))(const char * path, C(FileSize64) * size);
extern THIS_LIB_IMPORT uint (* F(getLastErrorCode))(void);
extern THIS_LIB_IMPORT char * (* F(getSlashPathBuffer))(char * d, const char * p);
extern THIS_LIB_IMPORT char * (* F(getSystemPathBuffer))(char * d, const char * p);
extern THIS_LIB_IMPORT char * (* F(getWorkingDir))(char * buf, int size);
extern THIS_LIB_IMPORT void (* F(__e_log))(const char * text);
extern THIS_LIB_IMPORT void (* F(logErrorCode))(C(ErrorCode) errorCode, const char * details);
extern THIS_LIB_IMPORT void (* F(__e_logf))(const char * format, ...);
extern THIS_LIB_IMPORT C(bool) (* F(makeDir))(const char * path);
extern THIS_LIB_IMPORT void (* F(makeSlashPath))(char * p);
extern THIS_LIB_IMPORT void (* F(makeSystemPath))(char * p);
extern THIS_LIB_IMPORT C(bool) (* F(moveFile))(const char * source, const char * dest);
extern THIS_LIB_IMPORT C(bool) (* F(moveFileEx))(const char * source, const char * dest, C(MoveFileOptions) options);
extern THIS_LIB_IMPORT C(bool) (* F(removeDir))(const char * path);
extern THIS_LIB_IMPORT C(bool) (* F(renameFile))(const char * oldName, const char * newName);
extern THIS_LIB_IMPORT void (* F(resetError))(void);
extern THIS_LIB_IMPORT void (* F(setEnvironment))(const char * envName, const char * envValue);
extern THIS_LIB_IMPORT void (* F(setErrorLevel))(C(ErrorLevel) level);
extern THIS_LIB_IMPORT void (* F(setLoggingMode))(C(LoggingMode) mode, void * where);
extern THIS_LIB_IMPORT C(bool) (* F(shellOpen))(const char * fileName, ...);
extern THIS_LIB_IMPORT void (* F(unsetEnvironment))(const char * envName);
extern THIS_LIB_IMPORT void (* F(debugBreakpoint))(void);

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||i18n  //////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef uint32 C(CharCategories);
#if CPP11
enum C(CharCategory) : int
#else
typedef int C(CharCategory);
enum C(CharCategory)
#endif
{
   CharCategory_none = 0x0,
   CharCategory_Mn = 0x1,
   CharCategory_markNonSpacing = 0x1,
   CharCategory_Mc = 0x2,
   CharCategory_markSpacing = 0x2,
   CharCategory_Me = 0x3,
   CharCategory_markEnclosing = 0x3,
   CharCategory_Nd = 0x4,
   CharCategory_numberDecimalDigit = 0x4,
   CharCategory_Nl = 0x5,
   CharCategory_numberLetter = 0x5,
   CharCategory_No = 0x6,
   CharCategory_numberOther = 0x6,
   CharCategory_Zs = 0x7,
   CharCategory_separatorSpace = 0x7,
   CharCategory_Zl = 0x8,
   CharCategory_separatorLine = 0x8,
   CharCategory_Zp = 0x9,
   CharCategory_separatorParagraph = 0x9,
   CharCategory_Cc = 0xA,
   CharCategory_otherControl = 0xA,
   CharCategory_Cf = 0xB,
   CharCategory_otherFormat = 0xB,
   CharCategory_Cs = 0xC,
   CharCategory_otherSurrogate = 0xC,
   CharCategory_Co = 0xD,
   CharCategory_otherPrivateUse = 0xD,
   CharCategory_Cn = 0xE,
   CharCategory_otherNotAssigned = 0xE,
   CharCategory_Lu = 0xF,
   CharCategory_letterUpperCase = 0xF,
   CharCategory_Ll = 0x10,
   CharCategory_letterLowerCase = 0x10,
   CharCategory_Lt = 0x11,
   CharCategory_letterTitleCase = 0x11,
   CharCategory_Lm = 0x12,
   CharCategory_letterModifier = 0x12,
   CharCategory_Lo = 0x13,
   CharCategory_letterOther = 0x13,
   CharCategory_Pc = 0x14,
   CharCategory_punctuationConnector = 0x14,
   CharCategory_Pd = 0x15,
   CharCategory_punctuationDash = 0x15,
   CharCategory_Ps = 0x16,
   CharCategory_punctuationOpen = 0x16,
   CharCategory_Pe = 0x17,
   CharCategory_punctuationClose = 0x17,
   CharCategory_Pi = 0x18,
   CharCategory_punctuationInitial = 0x18,
   CharCategory_Pf = 0x19,
   CharCategory_punctuationFinal = 0x19,
   CharCategory_Po = 0x1A,
   CharCategory_punctuationOther = 0x1A,
   CharCategory_Sm = 0x1B,
   CharCategory_symbolMath = 0x1B,
   CharCategory_Sc = 0x1C,
   CharCategory_symbolCurrency = 0x1C,
   CharCategory_Sk = 0x1D,
   CharCategory_symbolModifier = 0x1D,
   CharCategory_So = 0x1E,
   CharCategory_symbolOther = 0x1E
};

typedef uint32 C(UnicodeDecomposition);
// end -- moved backwards outputs
#define unicodeCompatibilityMappings (MAXDWORD)

#if CPP11
enum C(PredefinedCharCategories) : uint
#else
typedef C(CharCategories) C(PredefinedCharCategories);
enum C(PredefinedCharCategories)
#endif
{
   PredefinedCharCategories_none = 0x1,
   PredefinedCharCategories_marks = 0xE,
   PredefinedCharCategories_numbers = 0x70,
   PredefinedCharCategories_separators = 0x380,
   PredefinedCharCategories_others = 0x7C00,
   PredefinedCharCategories_letters = 0xF8000,
   PredefinedCharCategories_punctuation = 0x7F00000,
   PredefinedCharCategories_symbols = 0x78000000,
   PredefinedCharCategories_connector = 0x100000
};

#define CHARCATEGORIES_none_SHIFT                        0
#define CHARCATEGORIES_none_MASK                         0x1
#define CHARCATEGORIES_none(x)                           ((((C(CharCategories))(x)) & CHARCATEGORIES_none_MASK) >> CHARCATEGORIES_none_SHIFT)
#define CHARCATEGORIES_SET_none(x, none)                    (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_none_MASK)) | (((C(CharCategories))(none)) << CHARCATEGORIES_none_SHIFT)
#define CHARCATEGORIES_markNonSpacing_SHIFT              1
#define CHARCATEGORIES_markNonSpacing_MASK               0x2
#define CHARCATEGORIES_markNonSpacing(x)                 ((((C(CharCategories))(x)) & CHARCATEGORIES_markNonSpacing_MASK) >> CHARCATEGORIES_markNonSpacing_SHIFT)
#define CHARCATEGORIES_SET_markNonSpacing(x, markNonSpacing)          (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_markNonSpacing_MASK)) | (((C(CharCategories))(markNonSpacing)) << CHARCATEGORIES_markNonSpacing_SHIFT)
#define CHARCATEGORIES_markSpacing_SHIFT                 2
#define CHARCATEGORIES_markSpacing_MASK                  0x4
#define CHARCATEGORIES_markSpacing(x)                    ((((C(CharCategories))(x)) & CHARCATEGORIES_markSpacing_MASK) >> CHARCATEGORIES_markSpacing_SHIFT)
#define CHARCATEGORIES_SET_markSpacing(x, markSpacing)             (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_markSpacing_MASK)) | (((C(CharCategories))(markSpacing)) << CHARCATEGORIES_markSpacing_SHIFT)
#define CHARCATEGORIES_markEnclosing_SHIFT               3
#define CHARCATEGORIES_markEnclosing_MASK                0x8
#define CHARCATEGORIES_markEnclosing(x)                  ((((C(CharCategories))(x)) & CHARCATEGORIES_markEnclosing_MASK) >> CHARCATEGORIES_markEnclosing_SHIFT)
#define CHARCATEGORIES_SET_markEnclosing(x, markEnclosing)           (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_markEnclosing_MASK)) | (((C(CharCategories))(markEnclosing)) << CHARCATEGORIES_markEnclosing_SHIFT)
#define CHARCATEGORIES_numberDecimalDigit_SHIFT          4
#define CHARCATEGORIES_numberDecimalDigit_MASK           0x10
#define CHARCATEGORIES_numberDecimalDigit(x)             ((((C(CharCategories))(x)) & CHARCATEGORIES_numberDecimalDigit_MASK) >> CHARCATEGORIES_numberDecimalDigit_SHIFT)
#define CHARCATEGORIES_SET_numberDecimalDigit(x, numberDecimalDigit)      (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_numberDecimalDigit_MASK)) | (((C(CharCategories))(numberDecimalDigit)) << CHARCATEGORIES_numberDecimalDigit_SHIFT)
#define CHARCATEGORIES_numberLetter_SHIFT                5
#define CHARCATEGORIES_numberLetter_MASK                 0x20
#define CHARCATEGORIES_numberLetter(x)                   ((((C(CharCategories))(x)) & CHARCATEGORIES_numberLetter_MASK) >> CHARCATEGORIES_numberLetter_SHIFT)
#define CHARCATEGORIES_SET_numberLetter(x, numberLetter)            (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_numberLetter_MASK)) | (((C(CharCategories))(numberLetter)) << CHARCATEGORIES_numberLetter_SHIFT)
#define CHARCATEGORIES_numberOther_SHIFT                 6
#define CHARCATEGORIES_numberOther_MASK                  0x40
#define CHARCATEGORIES_numberOther(x)                    ((((C(CharCategories))(x)) & CHARCATEGORIES_numberOther_MASK) >> CHARCATEGORIES_numberOther_SHIFT)
#define CHARCATEGORIES_SET_numberOther(x, numberOther)             (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_numberOther_MASK)) | (((C(CharCategories))(numberOther)) << CHARCATEGORIES_numberOther_SHIFT)
#define CHARCATEGORIES_separatorSpace_SHIFT              7
#define CHARCATEGORIES_separatorSpace_MASK               0x80
#define CHARCATEGORIES_separatorSpace(x)                 ((((C(CharCategories))(x)) & CHARCATEGORIES_separatorSpace_MASK) >> CHARCATEGORIES_separatorSpace_SHIFT)
#define CHARCATEGORIES_SET_separatorSpace(x, separatorSpace)          (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_separatorSpace_MASK)) | (((C(CharCategories))(separatorSpace)) << CHARCATEGORIES_separatorSpace_SHIFT)
#define CHARCATEGORIES_separatorLine_SHIFT               8
#define CHARCATEGORIES_separatorLine_MASK                0x100
#define CHARCATEGORIES_separatorLine(x)                  ((((C(CharCategories))(x)) & CHARCATEGORIES_separatorLine_MASK) >> CHARCATEGORIES_separatorLine_SHIFT)
#define CHARCATEGORIES_SET_separatorLine(x, separatorLine)           (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_separatorLine_MASK)) | (((C(CharCategories))(separatorLine)) << CHARCATEGORIES_separatorLine_SHIFT)
#define CHARCATEGORIES_separatorParagraph_SHIFT          9
#define CHARCATEGORIES_separatorParagraph_MASK           0x200
#define CHARCATEGORIES_separatorParagraph(x)             ((((C(CharCategories))(x)) & CHARCATEGORIES_separatorParagraph_MASK) >> CHARCATEGORIES_separatorParagraph_SHIFT)
#define CHARCATEGORIES_SET_separatorParagraph(x, separatorParagraph)      (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_separatorParagraph_MASK)) | (((C(CharCategories))(separatorParagraph)) << CHARCATEGORIES_separatorParagraph_SHIFT)
#define CHARCATEGORIES_otherControl_SHIFT                10
#define CHARCATEGORIES_otherControl_MASK                 0x400
#define CHARCATEGORIES_otherControl(x)                   ((((C(CharCategories))(x)) & CHARCATEGORIES_otherControl_MASK) >> CHARCATEGORIES_otherControl_SHIFT)
#define CHARCATEGORIES_SET_otherControl(x, otherControl)            (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_otherControl_MASK)) | (((C(CharCategories))(otherControl)) << CHARCATEGORIES_otherControl_SHIFT)
#define CHARCATEGORIES_otherFormat_SHIFT                 11
#define CHARCATEGORIES_otherFormat_MASK                  0x800
#define CHARCATEGORIES_otherFormat(x)                    ((((C(CharCategories))(x)) & CHARCATEGORIES_otherFormat_MASK) >> CHARCATEGORIES_otherFormat_SHIFT)
#define CHARCATEGORIES_SET_otherFormat(x, otherFormat)             (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_otherFormat_MASK)) | (((C(CharCategories))(otherFormat)) << CHARCATEGORIES_otherFormat_SHIFT)
#define CHARCATEGORIES_otherSurrogate_SHIFT              12
#define CHARCATEGORIES_otherSurrogate_MASK               0x1000
#define CHARCATEGORIES_otherSurrogate(x)                 ((((C(CharCategories))(x)) & CHARCATEGORIES_otherSurrogate_MASK) >> CHARCATEGORIES_otherSurrogate_SHIFT)
#define CHARCATEGORIES_SET_otherSurrogate(x, otherSurrogate)          (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_otherSurrogate_MASK)) | (((C(CharCategories))(otherSurrogate)) << CHARCATEGORIES_otherSurrogate_SHIFT)
#define CHARCATEGORIES_otherPrivateUse_SHIFT             13
#define CHARCATEGORIES_otherPrivateUse_MASK              0x2000
#define CHARCATEGORIES_otherPrivateUse(x)                ((((C(CharCategories))(x)) & CHARCATEGORIES_otherPrivateUse_MASK) >> CHARCATEGORIES_otherPrivateUse_SHIFT)
#define CHARCATEGORIES_SET_otherPrivateUse(x, otherPrivateUse)         (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_otherPrivateUse_MASK)) | (((C(CharCategories))(otherPrivateUse)) << CHARCATEGORIES_otherPrivateUse_SHIFT)
#define CHARCATEGORIES_otherNotAssigned_SHIFT            14
#define CHARCATEGORIES_otherNotAssigned_MASK             0x4000
#define CHARCATEGORIES_otherNotAssigned(x)               ((((C(CharCategories))(x)) & CHARCATEGORIES_otherNotAssigned_MASK) >> CHARCATEGORIES_otherNotAssigned_SHIFT)
#define CHARCATEGORIES_SET_otherNotAssigned(x, otherNotAssigned)        (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_otherNotAssigned_MASK)) | (((C(CharCategories))(otherNotAssigned)) << CHARCATEGORIES_otherNotAssigned_SHIFT)
#define CHARCATEGORIES_letterUpperCase_SHIFT             15
#define CHARCATEGORIES_letterUpperCase_MASK              0x8000
#define CHARCATEGORIES_letterUpperCase(x)                ((((C(CharCategories))(x)) & CHARCATEGORIES_letterUpperCase_MASK) >> CHARCATEGORIES_letterUpperCase_SHIFT)
#define CHARCATEGORIES_SET_letterUpperCase(x, letterUpperCase)         (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_letterUpperCase_MASK)) | (((C(CharCategories))(letterUpperCase)) << CHARCATEGORIES_letterUpperCase_SHIFT)
#define CHARCATEGORIES_letterLowerCase_SHIFT             16
#define CHARCATEGORIES_letterLowerCase_MASK              0x10000
#define CHARCATEGORIES_letterLowerCase(x)                ((((C(CharCategories))(x)) & CHARCATEGORIES_letterLowerCase_MASK) >> CHARCATEGORIES_letterLowerCase_SHIFT)
#define CHARCATEGORIES_SET_letterLowerCase(x, letterLowerCase)         (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_letterLowerCase_MASK)) | (((C(CharCategories))(letterLowerCase)) << CHARCATEGORIES_letterLowerCase_SHIFT)
#define CHARCATEGORIES_letterTitleCase_SHIFT             17
#define CHARCATEGORIES_letterTitleCase_MASK              0x20000
#define CHARCATEGORIES_letterTitleCase(x)                ((((C(CharCategories))(x)) & CHARCATEGORIES_letterTitleCase_MASK) >> CHARCATEGORIES_letterTitleCase_SHIFT)
#define CHARCATEGORIES_SET_letterTitleCase(x, letterTitleCase)         (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_letterTitleCase_MASK)) | (((C(CharCategories))(letterTitleCase)) << CHARCATEGORIES_letterTitleCase_SHIFT)
#define CHARCATEGORIES_letterModifier_SHIFT              18
#define CHARCATEGORIES_letterModifier_MASK               0x40000
#define CHARCATEGORIES_letterModifier(x)                 ((((C(CharCategories))(x)) & CHARCATEGORIES_letterModifier_MASK) >> CHARCATEGORIES_letterModifier_SHIFT)
#define CHARCATEGORIES_SET_letterModifier(x, letterModifier)          (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_letterModifier_MASK)) | (((C(CharCategories))(letterModifier)) << CHARCATEGORIES_letterModifier_SHIFT)
#define CHARCATEGORIES_letterOther_SHIFT                 19
#define CHARCATEGORIES_letterOther_MASK                  0x80000
#define CHARCATEGORIES_letterOther(x)                    ((((C(CharCategories))(x)) & CHARCATEGORIES_letterOther_MASK) >> CHARCATEGORIES_letterOther_SHIFT)
#define CHARCATEGORIES_SET_letterOther(x, letterOther)             (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_letterOther_MASK)) | (((C(CharCategories))(letterOther)) << CHARCATEGORIES_letterOther_SHIFT)
#define CHARCATEGORIES_punctuationConnector_SHIFT        20
#define CHARCATEGORIES_punctuationConnector_MASK         0x100000
#define CHARCATEGORIES_punctuationConnector(x)           ((((C(CharCategories))(x)) & CHARCATEGORIES_punctuationConnector_MASK) >> CHARCATEGORIES_punctuationConnector_SHIFT)
#define CHARCATEGORIES_SET_punctuationConnector(x, punctuationConnector)    (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_punctuationConnector_MASK)) | (((C(CharCategories))(punctuationConnector)) << CHARCATEGORIES_punctuationConnector_SHIFT)
#define CHARCATEGORIES_punctuationDash_SHIFT             21
#define CHARCATEGORIES_punctuationDash_MASK              0x200000
#define CHARCATEGORIES_punctuationDash(x)                ((((C(CharCategories))(x)) & CHARCATEGORIES_punctuationDash_MASK) >> CHARCATEGORIES_punctuationDash_SHIFT)
#define CHARCATEGORIES_SET_punctuationDash(x, punctuationDash)         (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_punctuationDash_MASK)) | (((C(CharCategories))(punctuationDash)) << CHARCATEGORIES_punctuationDash_SHIFT)
#define CHARCATEGORIES_punctuationOpen_SHIFT             22
#define CHARCATEGORIES_punctuationOpen_MASK              0x400000
#define CHARCATEGORIES_punctuationOpen(x)                ((((C(CharCategories))(x)) & CHARCATEGORIES_punctuationOpen_MASK) >> CHARCATEGORIES_punctuationOpen_SHIFT)
#define CHARCATEGORIES_SET_punctuationOpen(x, punctuationOpen)         (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_punctuationOpen_MASK)) | (((C(CharCategories))(punctuationOpen)) << CHARCATEGORIES_punctuationOpen_SHIFT)
#define CHARCATEGORIES_punctuationClose_SHIFT            23
#define CHARCATEGORIES_punctuationClose_MASK             0x800000
#define CHARCATEGORIES_punctuationClose(x)               ((((C(CharCategories))(x)) & CHARCATEGORIES_punctuationClose_MASK) >> CHARCATEGORIES_punctuationClose_SHIFT)
#define CHARCATEGORIES_SET_punctuationClose(x, punctuationClose)        (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_punctuationClose_MASK)) | (((C(CharCategories))(punctuationClose)) << CHARCATEGORIES_punctuationClose_SHIFT)
#define CHARCATEGORIES_punctuationInitial_SHIFT          24
#define CHARCATEGORIES_punctuationInitial_MASK           0x1000000
#define CHARCATEGORIES_punctuationInitial(x)             ((((C(CharCategories))(x)) & CHARCATEGORIES_punctuationInitial_MASK) >> CHARCATEGORIES_punctuationInitial_SHIFT)
#define CHARCATEGORIES_SET_punctuationInitial(x, punctuationInitial)      (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_punctuationInitial_MASK)) | (((C(CharCategories))(punctuationInitial)) << CHARCATEGORIES_punctuationInitial_SHIFT)
#define CHARCATEGORIES_punctuationFinal_SHIFT            25
#define CHARCATEGORIES_punctuationFinal_MASK             0x2000000
#define CHARCATEGORIES_punctuationFinal(x)               ((((C(CharCategories))(x)) & CHARCATEGORIES_punctuationFinal_MASK) >> CHARCATEGORIES_punctuationFinal_SHIFT)
#define CHARCATEGORIES_SET_punctuationFinal(x, punctuationFinal)        (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_punctuationFinal_MASK)) | (((C(CharCategories))(punctuationFinal)) << CHARCATEGORIES_punctuationFinal_SHIFT)
#define CHARCATEGORIES_punctuationOther_SHIFT            26
#define CHARCATEGORIES_punctuationOther_MASK             0x4000000
#define CHARCATEGORIES_punctuationOther(x)               ((((C(CharCategories))(x)) & CHARCATEGORIES_punctuationOther_MASK) >> CHARCATEGORIES_punctuationOther_SHIFT)
#define CHARCATEGORIES_SET_punctuationOther(x, punctuationOther)        (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_punctuationOther_MASK)) | (((C(CharCategories))(punctuationOther)) << CHARCATEGORIES_punctuationOther_SHIFT)
#define CHARCATEGORIES_symbolMath_SHIFT                  27
#define CHARCATEGORIES_symbolMath_MASK                   0x8000000
#define CHARCATEGORIES_symbolMath(x)                     ((((C(CharCategories))(x)) & CHARCATEGORIES_symbolMath_MASK) >> CHARCATEGORIES_symbolMath_SHIFT)
#define CHARCATEGORIES_SET_symbolMath(x, symbolMath)              (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_symbolMath_MASK)) | (((C(CharCategories))(symbolMath)) << CHARCATEGORIES_symbolMath_SHIFT)
#define CHARCATEGORIES_symbolCurrency_SHIFT              28
#define CHARCATEGORIES_symbolCurrency_MASK               0x10000000
#define CHARCATEGORIES_symbolCurrency(x)                 ((((C(CharCategories))(x)) & CHARCATEGORIES_symbolCurrency_MASK) >> CHARCATEGORIES_symbolCurrency_SHIFT)
#define CHARCATEGORIES_SET_symbolCurrency(x, symbolCurrency)          (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_symbolCurrency_MASK)) | (((C(CharCategories))(symbolCurrency)) << CHARCATEGORIES_symbolCurrency_SHIFT)
#define CHARCATEGORIES_symbolModifier_SHIFT              29
#define CHARCATEGORIES_symbolModifier_MASK               0x20000000
#define CHARCATEGORIES_symbolModifier(x)                 ((((C(CharCategories))(x)) & CHARCATEGORIES_symbolModifier_MASK) >> CHARCATEGORIES_symbolModifier_SHIFT)
#define CHARCATEGORIES_SET_symbolModifier(x, symbolModifier)          (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_symbolModifier_MASK)) | (((C(CharCategories))(symbolModifier)) << CHARCATEGORIES_symbolModifier_SHIFT)
#define CHARCATEGORIES_symbolOther_SHIFT                 30
#define CHARCATEGORIES_symbolOther_MASK                  0x40000000
#define CHARCATEGORIES_symbolOther(x)                    ((((C(CharCategories))(x)) & CHARCATEGORIES_symbolOther_MASK) >> CHARCATEGORIES_symbolOther_SHIFT)
#define CHARCATEGORIES_SET_symbolOther(x, symbolOther)             (x) = ((C(CharCategories))(x) & ~((C(CharCategories))CHARCATEGORIES_symbolOther_MASK)) | (((C(CharCategories))(symbolOther)) << CHARCATEGORIES_symbolOther_SHIFT)


#define UNICODEDECOMPOSITION_canonical_SHIFT             0
#define UNICODEDECOMPOSITION_canonical_MASK              0x1
#define UNICODEDECOMPOSITION_canonical(x)                ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_canonical_MASK) >> UNICODEDECOMPOSITION_canonical_SHIFT)
#define UNICODEDECOMPOSITION_SET_canonical(x, canonical)         (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_canonical_MASK)) | (((C(UnicodeDecomposition))(canonical)) << UNICODEDECOMPOSITION_canonical_SHIFT)
#define UNICODEDECOMPOSITION_compat_SHIFT                1
#define UNICODEDECOMPOSITION_compat_MASK                 0x2
#define UNICODEDECOMPOSITION_compat(x)                   ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_compat_MASK) >> UNICODEDECOMPOSITION_compat_SHIFT)
#define UNICODEDECOMPOSITION_SET_compat(x, compat)            (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_compat_MASK)) | (((C(UnicodeDecomposition))(compat)) << UNICODEDECOMPOSITION_compat_SHIFT)
#define UNICODEDECOMPOSITION_fraction_SHIFT              2
#define UNICODEDECOMPOSITION_fraction_MASK               0x4
#define UNICODEDECOMPOSITION_fraction(x)                 ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_fraction_MASK) >> UNICODEDECOMPOSITION_fraction_SHIFT)
#define UNICODEDECOMPOSITION_SET_fraction(x, fraction)          (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_fraction_MASK)) | (((C(UnicodeDecomposition))(fraction)) << UNICODEDECOMPOSITION_fraction_SHIFT)
#define UNICODEDECOMPOSITION_font_SHIFT                  3
#define UNICODEDECOMPOSITION_font_MASK                   0x8
#define UNICODEDECOMPOSITION_font(x)                     ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_font_MASK) >> UNICODEDECOMPOSITION_font_SHIFT)
#define UNICODEDECOMPOSITION_SET_font(x, font)              (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_font_MASK)) | (((C(UnicodeDecomposition))(font)) << UNICODEDECOMPOSITION_font_SHIFT)
#define UNICODEDECOMPOSITION_noBreak_SHIFT               4
#define UNICODEDECOMPOSITION_noBreak_MASK                0x10
#define UNICODEDECOMPOSITION_noBreak(x)                  ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_noBreak_MASK) >> UNICODEDECOMPOSITION_noBreak_SHIFT)
#define UNICODEDECOMPOSITION_SET_noBreak(x, noBreak)           (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_noBreak_MASK)) | (((C(UnicodeDecomposition))(noBreak)) << UNICODEDECOMPOSITION_noBreak_SHIFT)
#define UNICODEDECOMPOSITION_initial_SHIFT               5
#define UNICODEDECOMPOSITION_initial_MASK                0x20
#define UNICODEDECOMPOSITION_initial(x)                  ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_initial_MASK) >> UNICODEDECOMPOSITION_initial_SHIFT)
#define UNICODEDECOMPOSITION_SET_initial(x, initial)           (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_initial_MASK)) | (((C(UnicodeDecomposition))(initial)) << UNICODEDECOMPOSITION_initial_SHIFT)
#define UNICODEDECOMPOSITION_final_SHIFT                 6
#define UNICODEDECOMPOSITION_final_MASK                  0x40
#define UNICODEDECOMPOSITION_final(x)                    ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_final_MASK) >> UNICODEDECOMPOSITION_final_SHIFT)
#define UNICODEDECOMPOSITION_SET_final(x, final)             (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_final_MASK)) | (((C(UnicodeDecomposition))(final)) << UNICODEDECOMPOSITION_final_SHIFT)
#define UNICODEDECOMPOSITION_medial_SHIFT                7
#define UNICODEDECOMPOSITION_medial_MASK                 0x80
#define UNICODEDECOMPOSITION_medial(x)                   ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_medial_MASK) >> UNICODEDECOMPOSITION_medial_SHIFT)
#define UNICODEDECOMPOSITION_SET_medial(x, medial)            (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_medial_MASK)) | (((C(UnicodeDecomposition))(medial)) << UNICODEDECOMPOSITION_medial_SHIFT)
#define UNICODEDECOMPOSITION_isolated_SHIFT              8
#define UNICODEDECOMPOSITION_isolated_MASK               0x100
#define UNICODEDECOMPOSITION_isolated(x)                 ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_isolated_MASK) >> UNICODEDECOMPOSITION_isolated_SHIFT)
#define UNICODEDECOMPOSITION_SET_isolated(x, isolated)          (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_isolated_MASK)) | (((C(UnicodeDecomposition))(isolated)) << UNICODEDECOMPOSITION_isolated_SHIFT)
#define UNICODEDECOMPOSITION_circle_SHIFT                9
#define UNICODEDECOMPOSITION_circle_MASK                 0x200
#define UNICODEDECOMPOSITION_circle(x)                   ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_circle_MASK) >> UNICODEDECOMPOSITION_circle_SHIFT)
#define UNICODEDECOMPOSITION_SET_circle(x, circle)            (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_circle_MASK)) | (((C(UnicodeDecomposition))(circle)) << UNICODEDECOMPOSITION_circle_SHIFT)
#define UNICODEDECOMPOSITION_square_SHIFT                10
#define UNICODEDECOMPOSITION_square_MASK                 0x400
#define UNICODEDECOMPOSITION_square(x)                   ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_square_MASK) >> UNICODEDECOMPOSITION_square_SHIFT)
#define UNICODEDECOMPOSITION_SET_square(x, square)            (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_square_MASK)) | (((C(UnicodeDecomposition))(square)) << UNICODEDECOMPOSITION_square_SHIFT)
#define UNICODEDECOMPOSITION_sub_SHIFT                   11
#define UNICODEDECOMPOSITION_sub_MASK                    0x800
#define UNICODEDECOMPOSITION_sub(x)                      ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_sub_MASK) >> UNICODEDECOMPOSITION_sub_SHIFT)
#define UNICODEDECOMPOSITION_SET_sub(x, sub)               (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_sub_MASK)) | (((C(UnicodeDecomposition))(sub)) << UNICODEDECOMPOSITION_sub_SHIFT)
#define UNICODEDECOMPOSITION_super_SHIFT                 12
#define UNICODEDECOMPOSITION_super_MASK                  0x1000
#define UNICODEDECOMPOSITION_super(x)                    ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_super_MASK) >> UNICODEDECOMPOSITION_super_SHIFT)
#define UNICODEDECOMPOSITION_SET_super(x, super)             (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_super_MASK)) | (((C(UnicodeDecomposition))(super)) << UNICODEDECOMPOSITION_super_SHIFT)
#define UNICODEDECOMPOSITION_small_SHIFT                 13
#define UNICODEDECOMPOSITION_small_MASK                  0x2000
#define UNICODEDECOMPOSITION_small(x)                    ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_small_MASK) >> UNICODEDECOMPOSITION_small_SHIFT)
#define UNICODEDECOMPOSITION_SET_small(x, small)             (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_small_MASK)) | (((C(UnicodeDecomposition))(small)) << UNICODEDECOMPOSITION_small_SHIFT)
#define UNICODEDECOMPOSITION_vertical_SHIFT              14
#define UNICODEDECOMPOSITION_vertical_MASK               0x4000
#define UNICODEDECOMPOSITION_vertical(x)                 ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_vertical_MASK) >> UNICODEDECOMPOSITION_vertical_SHIFT)
#define UNICODEDECOMPOSITION_SET_vertical(x, vertical)          (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_vertical_MASK)) | (((C(UnicodeDecomposition))(vertical)) << UNICODEDECOMPOSITION_vertical_SHIFT)
#define UNICODEDECOMPOSITION_wide_SHIFT                  15
#define UNICODEDECOMPOSITION_wide_MASK                   0x8000
#define UNICODEDECOMPOSITION_wide(x)                     ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_wide_MASK) >> UNICODEDECOMPOSITION_wide_SHIFT)
#define UNICODEDECOMPOSITION_SET_wide(x, wide)              (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_wide_MASK)) | (((C(UnicodeDecomposition))(wide)) << UNICODEDECOMPOSITION_wide_SHIFT)
#define UNICODEDECOMPOSITION_narrow_SHIFT                16
#define UNICODEDECOMPOSITION_narrow_MASK                 0x10000
#define UNICODEDECOMPOSITION_narrow(x)                   ((((C(UnicodeDecomposition))(x)) & UNICODEDECOMPOSITION_narrow_MASK) >> UNICODEDECOMPOSITION_narrow_SHIFT)
#define UNICODEDECOMPOSITION_SET_narrow(x, narrow)            (x) = ((C(UnicodeDecomposition))(x) & ~((C(UnicodeDecomposition))UNICODEDECOMPOSITION_narrow_MASK)) | (((C(UnicodeDecomposition))(narrow)) << UNICODEDECOMPOSITION_narrow_SHIFT)


extern THIS_LIB_IMPORT C(bool) (* F(charMatchCategories))(unichar ch, C(CharCategories) categories);
extern THIS_LIB_IMPORT C(bool) (* F(getAlNum))(const char ** input, char * string, int max);
extern THIS_LIB_IMPORT C(CharCategory) (* F(getCharCategory))(unichar ch);
extern THIS_LIB_IMPORT uint (* F(getCombiningClass))(unichar ch);
extern THIS_LIB_IMPORT int (* F(iSO8859_1toUTF8))(const char * source, char * dest, int max);
extern THIS_LIB_IMPORT int (* F(uTF16BEtoUTF8Buffer))(const uint16 * source, byte * dest, int max);
extern THIS_LIB_IMPORT char * (* F(uTF16toUTF8))(const uint16 * source);
extern THIS_LIB_IMPORT int (* F(uTF16toUTF8Buffer))(const uint16 * source, char * dest, int max);
extern THIS_LIB_IMPORT int (* F(uTF32toUTF8Len))(const unichar * source, int count, char * dest, int max);
extern THIS_LIB_IMPORT unichar (* F(uTF8GetChar))(const char * string, int * numBytes);
extern THIS_LIB_IMPORT C(bool) (* F(uTF8Validate))(const char * source);
extern THIS_LIB_IMPORT int (* F(uTF8toISO8859_1))(const char * source, char * dest, int max);
extern THIS_LIB_IMPORT uint16 * (* F(uTF8toUTF16))(const char * source, int * wordCount);
extern THIS_LIB_IMPORT int (* F(uTF8toUTF16Buffer))(const char * source, uint16 * dest, int max);
extern THIS_LIB_IMPORT int (* F(uTF8toUTF16BufferLen))(const char * source, uint16 * dest, int max, int len);
extern THIS_LIB_IMPORT uint16 * (* F(uTF8toUTF16Len))(const char * source, int byteCount, int * wordCount);
extern THIS_LIB_IMPORT C(String) (* F(accenti))(constString string);
extern THIS_LIB_IMPORT C(String) (* F(casei))(constString string);
extern THIS_LIB_IMPORT C(String) (* F(encodeArrayToString))(C(Array) array);
extern THIS_LIB_IMPORT C(String) (* F(normalizeNFC))(constString string);
extern THIS_LIB_IMPORT C(String) (* F(normalizeNFD))(constString string);
extern THIS_LIB_IMPORT C(String) (* F(normalizeNFKC))(constString string);
extern THIS_LIB_IMPORT C(String) (* F(normalizeNFKD))(constString string);
extern THIS_LIB_IMPORT C(Array) (* F(normalizeNFKDArray))(constString string);
extern THIS_LIB_IMPORT C(String) (* F(normalizeUnicode))(constString string, C(UnicodeDecomposition) type, C(bool) compose);
extern THIS_LIB_IMPORT C(Array) (* F(normalizeUnicodeArray))(constString string, C(UnicodeDecomposition) type, C(bool) compose);
extern THIS_LIB_IMPORT C(String) (* F(stripUnicodeCategory))(constString string, C(CharCategory) c);

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||json  //////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef C(Instance) C(GlobalSettings);
typedef C(GlobalSettings) C(GlobalAppSettings);
#if CPP11
enum C(GlobalSettingType) : int
#else
typedef int C(GlobalSettingType);
enum C(GlobalSettingType)
#endif
{
   GlobalSettingType_integer = 0x0,
   GlobalSettingType_singleString = 0x1,
   GlobalSettingType_stringList = 0x2
};

typedef C(Instance) C(GlobalSettingsData);
typedef C(Instance) C(GlobalSettingsDriver);
#if CPP11
enum C(JSONFirstLetterCapitalization) : int
#else
typedef int C(JSONFirstLetterCapitalization);
enum C(JSONFirstLetterCapitalization)
#endif
{
   JSONFirstLetterCapitalization_keepCase = 0x0,
   JSONFirstLetterCapitalization_upperCase = 0x1,
   JSONFirstLetterCapitalization_lowerCase = 0x2
};

typedef C(Instance) C(JSONParser);
#if CPP11
enum C(JSONResult) : int
#else
typedef int C(JSONResult);
enum C(JSONResult)
#endif
{
   JSONResult_syntaxError = 0x0,
   JSONResult_success = 0x1,
   JSONResult_typeMismatch = 0x2,
   JSONResult_noItem = 0x3
};

typedef T(Map, String, JSONTypeOptions) C(OptionsMap);
#if CPP11
enum C(SettingsIOResult) : int
#else
typedef int C(SettingsIOResult);
enum C(SettingsIOResult)
#endif
{
   SettingsIOResult_error = 0x0,
   SettingsIOResult_success = 0x1,
   SettingsIOResult_fileNotFound = 0x2,
   SettingsIOResult_fileNotCompatibleWithDriver = 0x3
};

// end -- moved backwards outputs
typedef C(GlobalSettingsDriver) C(ECONGlobalSettings);
typedef C(JSONParser) C(ECONParser);
typedef C(GlobalSettingsDriver) C(JSONGlobalSettings);
typedef uint32 C(JSONTypeOptions);
#if CPP11
enum C(SetBool) : uint
#else
typedef uint C(SetBool);
enum C(SetBool)
#endif
{
   SetBool_unset = 0x0,
   SetBool_false = 0x1,
   SetBool_true = 0x2
};

extern THIS_LIB_IMPORT C(bool) (* GlobalAppSettings_getGlobalValue)(C(GlobalAppSettings) __this, const char * section, const char * name, C(GlobalSettingType) type, void * value);

extern THIS_LIB_IMPORT C(bool) (* GlobalAppSettings_putGlobalValue)(C(GlobalAppSettings) __this, const char * section, const char * name, C(GlobalSettingType) type, const void * value);

struct CM(GlobalSettings)
{
   C(GlobalSettingsData) data;
   C(GlobalSettingsData) * dataOwner;
   subclass(GlobalSettingsData) dataClass;
   byte __ecere_padding[96];
};
extern THIS_LIB_IMPORT void (* GlobalSettings_close)(C(GlobalSettings) __this);

extern THIS_LIB_IMPORT void (* GlobalSettings_closeAndMonitor)(C(GlobalSettings) __this);

extern THIS_LIB_IMPORT int M_VTBLID(GlobalSettings, load);
// C(SettingsIOResult) GlobalSettings_load(C(GlobalSettings) __i);
#define GlobalSettings_load(__i) \
   VMETHOD(CO(GlobalSettings), GlobalSettings, load, __i, C(SettingsIOResult), \
      C(GlobalSettings), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(GlobalSettings, load);

extern THIS_LIB_IMPORT int M_VTBLID(GlobalSettings, onAskReloadSettings);
// void GlobalSettings_onAskReloadSettings(C(GlobalSettings) __i);
#define GlobalSettings_onAskReloadSettings(__i) \
   VMETHOD(CO(GlobalSettings), GlobalSettings, onAskReloadSettings, __i, void, \
      C(GlobalSettings), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(GlobalSettings, onAskReloadSettings);

extern THIS_LIB_IMPORT C(bool) (* GlobalSettings_openAndLock)(C(GlobalSettings) __this, C(FileSize) * fileSize);

extern THIS_LIB_IMPORT int M_VTBLID(GlobalSettings, save);
// C(SettingsIOResult) GlobalSettings_save(C(GlobalSettings) __i);
#define GlobalSettings_save(__i) \
   VMETHOD(CO(GlobalSettings), GlobalSettings, save, __i, C(SettingsIOResult), \
      C(GlobalSettings), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(GlobalSettings, save);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, settingsName);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_settingsName)(const C(GlobalSettings) g, const char * value);
extern THIS_LIB_IMPORT const char * (* GlobalSettings_get_settingsName)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, settingsExtension);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_settingsExtension)(const C(GlobalSettings) g, const char * value);
extern THIS_LIB_IMPORT const char * (* GlobalSettings_get_settingsExtension)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, settingsDirectory);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_settingsDirectory)(const C(GlobalSettings) g, const char * value);
extern THIS_LIB_IMPORT const char * (* GlobalSettings_get_settingsDirectory)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, settingsLocation);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_settingsLocation)(const C(GlobalSettings) g, const char * value);
extern THIS_LIB_IMPORT const char * (* GlobalSettings_get_settingsLocation)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, settingsFilePath);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_settingsFilePath)(const C(GlobalSettings) g, const char * value);
extern THIS_LIB_IMPORT const char * (* GlobalSettings_get_settingsFilePath)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, allowDefaultLocations);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_allowDefaultLocations)(const C(GlobalSettings) g, C(bool) value);
extern THIS_LIB_IMPORT C(bool) (* GlobalSettings_get_allowDefaultLocations)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, allUsers);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_allUsers)(const C(GlobalSettings) g, C(bool) value);
extern THIS_LIB_IMPORT C(bool) (* GlobalSettings_get_allUsers)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, portable);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_portable)(const C(GlobalSettings) g, C(bool) value);
extern THIS_LIB_IMPORT C(bool) (* GlobalSettings_get_portable)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, driver);
extern THIS_LIB_IMPORT void (* GlobalSettings_set_driver)(const C(GlobalSettings) g, constString value);
extern THIS_LIB_IMPORT constString (* GlobalSettings_get_driver)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(GlobalSettings, isGlobalPath);
extern THIS_LIB_IMPORT C(bool) (* GlobalSettings_get_isGlobalPath)(const C(GlobalSettings) g);

extern THIS_LIB_IMPORT int M_VTBLID(GlobalSettingsDriver, load);
// C(SettingsIOResult) GlobalSettingsDriver_load(C(GlobalSettingsDriver) __i, C(File) f, C(GlobalSettings) globalSettings);
#define GlobalSettingsDriver_load(__i, f, globalSettings) \
   VMETHOD(CO(GlobalSettingsDriver), GlobalSettingsDriver, load, __i, C(SettingsIOResult), \
      C(File) _ARG C(GlobalSettings), \
      f _ARG globalSettings)
extern THIS_LIB_IMPORT C(Method) * METHOD(GlobalSettingsDriver, load);

extern THIS_LIB_IMPORT int M_VTBLID(GlobalSettingsDriver, save);
// C(SettingsIOResult) GlobalSettingsDriver_save(C(GlobalSettingsDriver) __i, C(File) f, C(GlobalSettings) globalSettings);
#define GlobalSettingsDriver_save(__i, f, globalSettings) \
   VMETHOD(CO(GlobalSettingsDriver), GlobalSettingsDriver, save, __i, C(SettingsIOResult), \
      C(File) _ARG C(GlobalSettings), \
      f _ARG globalSettings)
extern THIS_LIB_IMPORT C(Method) * METHOD(GlobalSettingsDriver, save);

struct CM(JSONParser)
{
   C(File) f;
   C(OptionsMap) customJsonOptions;
   byte __ecere_padding[32];
};
extern THIS_LIB_IMPORT C(JSONResult) (* JSONParser_getObject)(C(JSONParser) __this, C(Class) * objectType, void ** object);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONParser, debug);
extern THIS_LIB_IMPORT void (* JSONParser_set_debug)(const C(JSONParser) j, C(bool) value);
extern THIS_LIB_IMPORT C(bool) (* JSONParser_get_debug)(const C(JSONParser) j);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(JSONParser, warnings);
extern THIS_LIB_IMPORT void (* JSONParser_set_warnings)(const C(JSONParser) j, C(bool) value);
extern THIS_LIB_IMPORT C(bool) (* JSONParser_get_warnings)(const C(JSONParser) j);

#define JSONTYPEOPTIONS_numbersUseOGDFS_SHIFT            0
#define JSONTYPEOPTIONS_numbersUseOGDFS_MASK             0x1
#define JSONTYPEOPTIONS_numbersUseOGDFS(x)               ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_numbersUseOGDFS_MASK) >> JSONTYPEOPTIONS_numbersUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_numbersUseOGDFS(x, numbersUseOGDFS)        (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_numbersUseOGDFS_MASK)) | (((C(JSONTypeOptions))(numbersUseOGDFS)) << JSONTYPEOPTIONS_numbersUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_boolUseOGDFS_SHIFT               1
#define JSONTYPEOPTIONS_boolUseOGDFS_MASK                0x2
#define JSONTYPEOPTIONS_boolUseOGDFS(x)                  ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_boolUseOGDFS_MASK) >> JSONTYPEOPTIONS_boolUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_boolUseOGDFS(x, boolUseOGDFS)           (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_boolUseOGDFS_MASK)) | (((C(JSONTypeOptions))(boolUseOGDFS)) << JSONTYPEOPTIONS_boolUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_nullUseOGDFS_SHIFT               2
#define JSONTYPEOPTIONS_nullUseOGDFS_MASK                0x4
#define JSONTYPEOPTIONS_nullUseOGDFS(x)                  ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_nullUseOGDFS_MASK) >> JSONTYPEOPTIONS_nullUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_nullUseOGDFS(x, nullUseOGDFS)           (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_nullUseOGDFS_MASK)) | (((C(JSONTypeOptions))(nullUseOGDFS)) << JSONTYPEOPTIONS_nullUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_stringUseOGDFS_SHIFT             3
#define JSONTYPEOPTIONS_stringUseOGDFS_MASK              0x8
#define JSONTYPEOPTIONS_stringUseOGDFS(x)                ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_stringUseOGDFS_MASK) >> JSONTYPEOPTIONS_stringUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_stringUseOGDFS(x, stringUseOGDFS)         (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_stringUseOGDFS_MASK)) | (((C(JSONTypeOptions))(stringUseOGDFS)) << JSONTYPEOPTIONS_stringUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_arrayUseOGDFS_SHIFT              4
#define JSONTYPEOPTIONS_arrayUseOGDFS_MASK               0x10
#define JSONTYPEOPTIONS_arrayUseOGDFS(x)                 ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_arrayUseOGDFS_MASK) >> JSONTYPEOPTIONS_arrayUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_arrayUseOGDFS(x, arrayUseOGDFS)          (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_arrayUseOGDFS_MASK)) | (((C(JSONTypeOptions))(arrayUseOGDFS)) << JSONTYPEOPTIONS_arrayUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_objectUseOGDFS_SHIFT             5
#define JSONTYPEOPTIONS_objectUseOGDFS_MASK              0x20
#define JSONTYPEOPTIONS_objectUseOGDFS(x)                ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_objectUseOGDFS_MASK) >> JSONTYPEOPTIONS_objectUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_objectUseOGDFS(x, objectUseOGDFS)         (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_objectUseOGDFS_MASK)) | (((C(JSONTypeOptions))(objectUseOGDFS)) << JSONTYPEOPTIONS_objectUseOGDFS_SHIFT)
#define JSONTYPEOPTIONS_stripQuotesForOGDFS_SHIFT        6
#define JSONTYPEOPTIONS_stripQuotesForOGDFS_MASK         0x40
#define JSONTYPEOPTIONS_stripQuotesForOGDFS(x)           ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_stripQuotesForOGDFS_MASK) >> JSONTYPEOPTIONS_stripQuotesForOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_stripQuotesForOGDFS(x, stripQuotesForOGDFS)    (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_stripQuotesForOGDFS_MASK)) | (((C(JSONTypeOptions))(stripQuotesForOGDFS)) << JSONTYPEOPTIONS_stripQuotesForOGDFS_SHIFT)
#define JSONTYPEOPTIONS_strictOGDFS_SHIFT                7
#define JSONTYPEOPTIONS_strictOGDFS_MASK                 0x80
#define JSONTYPEOPTIONS_strictOGDFS(x)                   ((((C(JSONTypeOptions))(x)) & JSONTYPEOPTIONS_strictOGDFS_MASK) >> JSONTYPEOPTIONS_strictOGDFS_SHIFT)
#define JSONTYPEOPTIONS_SET_strictOGDFS(x, strictOGDFS)            (x) = ((C(JSONTypeOptions))(x) & ~((C(JSONTypeOptions))JSONTYPEOPTIONS_strictOGDFS_MASK)) | (((C(JSONTypeOptions))(strictOGDFS)) << JSONTYPEOPTIONS_strictOGDFS_SHIFT)


extern THIS_LIB_IMPORT C(String) (* F(printECONObject))(C(Class) * objectType, void * object, int indent);
extern THIS_LIB_IMPORT C(String) (* F(printObjectNotationString))(C(Class) * objectType, void * object, C(ObjectNotationType) onType, int indent, C(bool) indentFirst, C(JSONFirstLetterCapitalization) capitalize);
extern THIS_LIB_IMPORT C(String) (* F(stringIndent))(constString base, int nSpaces, C(bool) indentFirst);
extern THIS_LIB_IMPORT C(bool) (* F(writeECONObject))(C(File) f, C(Class) * objectType, void * object, int indent);
extern THIS_LIB_IMPORT C(bool) (* F(writeJSONObject))(C(File) f, C(Class) * objectType, void * object, int indent);
extern THIS_LIB_IMPORT C(bool) (* F(writeJSONObject2))(C(File) f, C(Class) * objectType, void * object, int indent, C(JSONFirstLetterCapitalization) capitalize);
extern THIS_LIB_IMPORT C(bool) (* F(writeJSONObjectMapped))(C(File) f, C(Class) * objectType, void * object, int indent, C(Map) stringMap);
extern THIS_LIB_IMPORT C(bool) (* F(writeONString))(C(File) f, constString s, C(bool) eCON, int indent);

// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// ////////  eC||mt  ////////////////////////////////////////////////// ////////////////////////////////
// namespace /////////////// //////////////////////////////////////////////////////////////////// ////////////////////////////////


// start -- moved backwards outputs
typedef struct C(Condition) C(Condition);
typedef struct C(Mutex) C(Mutex);
typedef struct C(Semaphore) C(Semaphore);
typedef C(Instance) C(Thread);
#if CPP11
enum C(ThreadPriority) : int
#else
typedef int C(ThreadPriority);
enum C(ThreadPriority)
#endif
{
   ThreadPriority_normal = 0x0,
   ThreadPriority_aboveNormal = 0x1,
   ThreadPriority_belowNormal = -1,
   ThreadPriority_highest = 0x2,
   ThreadPriority_lowest = -2,
   ThreadPriority_idle = -15,
   ThreadPriority_timeCritical = 0xF
};

// end -- moved backwards outputs
struct C(Condition)
{
   byte __ecere_padding[40];
};
extern THIS_LIB_IMPORT void (* Condition_signal)(C(Condition) * __this);

extern THIS_LIB_IMPORT void (* Condition_wait)(C(Condition) * __this, C(Mutex) * mutex);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Condition, name);
extern THIS_LIB_IMPORT void (* Condition_set_name)(const C(Condition) * c, const char * value);
extern THIS_LIB_IMPORT const char * (* Condition_get_name)(const C(Condition) * c);

struct C(Mutex)
{
   byte __ecere_padding[56];
};
extern THIS_LIB_IMPORT void (* Mutex_release)(C(Mutex) * __this);

extern THIS_LIB_IMPORT void (* Mutex_wait)(C(Mutex) * __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Mutex, lockCount);
extern THIS_LIB_IMPORT int (* Mutex_get_lockCount)(const C(Mutex) * m);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Mutex, owningThread);
extern THIS_LIB_IMPORT int64 (* Mutex_get_owningThread)(const C(Mutex) * m);

struct C(Semaphore)
{
   byte __ecere_padding[40];
};
extern THIS_LIB_IMPORT void (* Semaphore_release)(C(Semaphore) * __this);

extern THIS_LIB_IMPORT C(bool) (* Semaphore_tryWait)(C(Semaphore) * __this);

extern THIS_LIB_IMPORT void (* Semaphore_wait)(C(Semaphore) * __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Semaphore, initCount);
extern THIS_LIB_IMPORT void (* Semaphore_set_initCount)(const C(Semaphore) * s, int value);
extern THIS_LIB_IMPORT int (* Semaphore_get_initCount)(const C(Semaphore) * s);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Semaphore, maxCount);
extern THIS_LIB_IMPORT void (* Semaphore_set_maxCount)(const C(Semaphore) * s, int value);
extern THIS_LIB_IMPORT int (* Semaphore_get_maxCount)(const C(Semaphore) * s);

extern THIS_LIB_IMPORT void (* Thread_create)(C(Thread) __this);

extern THIS_LIB_IMPORT void (* Thread_kill)(C(Thread) __this);

extern THIS_LIB_IMPORT int M_VTBLID(Thread, main);
// uint Thread_main(C(Thread) __i);
#define Thread_main(__i) \
   VMETHOD(CO(Thread), Thread, main, __i, uint, \
      C(Thread), \
      __i)
extern THIS_LIB_IMPORT C(Method) * METHOD(Thread, main);

extern THIS_LIB_IMPORT void (* Thread_setPriority)(C(Thread) __this, C(ThreadPriority) priority);

extern THIS_LIB_IMPORT void (* Thread_wait)(C(Thread) __this);

extern THIS_LIB_IMPORT C(Property) * PROPERTY(Thread, created);
extern THIS_LIB_IMPORT C(bool) (* Thread_get_created)(const C(Thread) t);

extern THIS_LIB_IMPORT int64 (* F(getCurrentThreadID))(void);
extern THIS_LIB_IMPORT C(Class) * CO(Application);
extern THIS_LIB_IMPORT C(Class) * CO(Instance);
extern THIS_LIB_IMPORT C(Class) * CO(Module);
extern THIS_LIB_IMPORT C(Class) * CO(AccessMode);
extern THIS_LIB_IMPORT C(Class) * CO(Angle);
extern THIS_LIB_IMPORT C(Class) * CO(BTNamedLink);
extern THIS_LIB_IMPORT C(Class) * CO(BackSlashEscaping);
extern THIS_LIB_IMPORT C(Class) * CO(BitMember);
extern THIS_LIB_IMPORT C(Class) * CO(Box);
extern THIS_LIB_IMPORT C(Class) * CO(CIString);
extern THIS_LIB_IMPORT C(Class) * CO(Centimeters);
extern THIS_LIB_IMPORT C(Class) * CO(Class);
extern THIS_LIB_IMPORT C(Class) * CO(ClassDesignerBase);
extern THIS_LIB_IMPORT C(Class) * CO(ClassProperty);
extern THIS_LIB_IMPORT C(Class) * CO(ClassTemplateArgument);
extern THIS_LIB_IMPORT C(Class) * CO(ClassTemplateParameter);
extern THIS_LIB_IMPORT C(Class) * CO(ClassType);
extern THIS_LIB_IMPORT C(Class) * CO(DataMember);
extern THIS_LIB_IMPORT C(Class) * CO(DataMemberType);
extern THIS_LIB_IMPORT C(Class) * CO(DataValue);
extern THIS_LIB_IMPORT C(Class) * CO(DefinedExpression);
extern THIS_LIB_IMPORT C(Class) * CO(Degrees);
extern THIS_LIB_IMPORT C(Class) * CO(DesignerBase);
extern THIS_LIB_IMPORT C(Class) * CO(Distance);
extern THIS_LIB_IMPORT C(Class) * CO(EnumClassData);
extern THIS_LIB_IMPORT C(Class) * CO(EscapeCStringOptions);
extern THIS_LIB_IMPORT C(Class) * CO(Feet);
extern THIS_LIB_IMPORT C(Class) * CO(GlobalFunction);
extern THIS_LIB_IMPORT C(Class) * CO(IOChannel);
extern THIS_LIB_IMPORT C(Class) * CO(ImportType);
extern THIS_LIB_IMPORT C(Class) * CO(Meters);
extern THIS_LIB_IMPORT C(Class) * CO(Method);
extern THIS_LIB_IMPORT C(Class) * CO(MethodType);
extern THIS_LIB_IMPORT C(Class) * CO(MinMaxValue);
extern THIS_LIB_IMPORT C(Class) * CO(NameSpace);
extern THIS_LIB_IMPORT C(Class) * CO(ObjectInfo);
extern THIS_LIB_IMPORT C(Class) * CO(ObjectNotationType);
extern THIS_LIB_IMPORT C(Class) * CO(Platform);
extern THIS_LIB_IMPORT C(Class) * CO(Point);
extern THIS_LIB_IMPORT C(Class) * CO(Pointd);
extern THIS_LIB_IMPORT C(Class) * CO(Pointf);
extern THIS_LIB_IMPORT C(Class) * CO(Property);
extern THIS_LIB_IMPORT C(Class) * CO(Radians);
extern THIS_LIB_IMPORT C(Class) * CO(SerialBuffer);
extern THIS_LIB_IMPORT C(Class) * CO(Size);
extern THIS_LIB_IMPORT C(Class) * CO(StaticString);
extern THIS_LIB_IMPORT C(Class) * CO(StringAllocType);
extern THIS_LIB_IMPORT C(Class) * CO(SubModule);
extern THIS_LIB_IMPORT C(Class) * CO(TemplateMemberType);
extern THIS_LIB_IMPORT C(Class) * CO(TemplateParameterType);
extern THIS_LIB_IMPORT C(Class) * CO(ZString);
extern THIS_LIB_IMPORT C(Class) * CO(String);
extern THIS_LIB_IMPORT C(Class) * CO(byte);
extern THIS_LIB_IMPORT C(Class) * CO(char);
extern THIS_LIB_IMPORT C(Class) * CO(class);
extern THIS_LIB_IMPORT C(Class) * CO(double);
extern THIS_LIB_IMPORT C(Class) * CO(enum);
extern THIS_LIB_IMPORT C(Class) * CO(float);
extern THIS_LIB_IMPORT C(Class) * CO(int);
extern THIS_LIB_IMPORT C(Class) * CO(int64);
extern THIS_LIB_IMPORT C(Class) * CO(intptr);
extern THIS_LIB_IMPORT C(Class) * CO(intsize);
extern THIS_LIB_IMPORT C(Class) * CO(short);
extern THIS_LIB_IMPORT C(Class) * CO(struct);
extern THIS_LIB_IMPORT C(Class) * CO(uint);
extern THIS_LIB_IMPORT C(Class) * CO(uint16);
extern THIS_LIB_IMPORT C(Class) * CO(uint32);
extern THIS_LIB_IMPORT C(Class) * CO(uint64);
extern THIS_LIB_IMPORT C(Class) * CO(uintptr);
extern THIS_LIB_IMPORT C(Class) * CO(uintsize);
extern THIS_LIB_IMPORT C(Class) * CO(FieldType);
extern THIS_LIB_IMPORT C(Class) * CO(FieldTypeEx);
extern THIS_LIB_IMPORT C(Class) * CO(FieldValue);
extern THIS_LIB_IMPORT C(Class) * CO(FieldValueFormat);
extern THIS_LIB_IMPORT C(Class) * CO(AVLNode);
extern THIS_LIB_IMPORT C(Class) * CO(AVLTree);
extern THIS_LIB_IMPORT C(Class) * CO(Array);
extern THIS_LIB_IMPORT C(Class) * CO(BTNode);
extern THIS_LIB_IMPORT C(Class) * CO(BinaryTree);
extern THIS_LIB_IMPORT C(Class) * CO(BuiltInContainer);
extern THIS_LIB_IMPORT C(Class) * CO(Container);
extern THIS_LIB_IMPORT C(Class) * CO(CustomAVLTree);
extern THIS_LIB_IMPORT C(Class) * CO(HashMap);
extern THIS_LIB_IMPORT C(Class) * CO(HashMapIterator);
extern THIS_LIB_IMPORT C(Class) * CO(HashTable);
extern THIS_LIB_IMPORT C(Class) * CO(Item);
extern THIS_LIB_IMPORT C(Class) * CO(Iterator);
extern THIS_LIB_IMPORT C(Class) * CO(IteratorPointer);
extern THIS_LIB_IMPORT C(Class) * CO(Link);
extern THIS_LIB_IMPORT C(Class) * CO(LinkElement);
extern THIS_LIB_IMPORT C(Class) * CO(LinkList);
extern THIS_LIB_IMPORT C(Class) * CO(List);
extern THIS_LIB_IMPORT C(Class) * CO(ListItem);
extern THIS_LIB_IMPORT C(Class) * CO(Map);
extern THIS_LIB_IMPORT C(Class) * CO(MapIterator);
extern THIS_LIB_IMPORT C(Class) * CO(MapNode);
extern THIS_LIB_IMPORT C(Class) * CO(NamedItem);
extern THIS_LIB_IMPORT C(Class) * CO(NamedLink);
extern THIS_LIB_IMPORT C(Class) * CO(NamedLink64);
extern THIS_LIB_IMPORT C(Class) * CO(OldLink);
extern THIS_LIB_IMPORT C(Class) * CO(OldList);
extern THIS_LIB_IMPORT C(Class) * CO(StringBTNode);
extern THIS_LIB_IMPORT C(Class) * CO(StringBinaryTree);
extern THIS_LIB_IMPORT C(Class) * CO(TreePrintStyle);
extern THIS_LIB_IMPORT C(Class) * CO(Date);
extern THIS_LIB_IMPORT C(Class) * CO(DateTime);
extern THIS_LIB_IMPORT C(Class) * CO(DayOfTheWeek);
extern THIS_LIB_IMPORT C(Class) * CO(Month);
extern THIS_LIB_IMPORT C(Class) * CO(SecSince1970);
extern THIS_LIB_IMPORT C(Class) * CO(Seconds);
extern THIS_LIB_IMPORT C(Class) * CO(Time);
extern THIS_LIB_IMPORT C(Class) * CO(TimeStamp);
extern THIS_LIB_IMPORT C(Class) * CO(TimeStamp32);
extern THIS_LIB_IMPORT C(Class) * CO(Archive);
extern THIS_LIB_IMPORT C(Class) * CO(ArchiveAddMode);
extern THIS_LIB_IMPORT C(Class) * CO(ArchiveDir);
extern THIS_LIB_IMPORT C(Class) * CO(ArchiveOpenFlags);
extern THIS_LIB_IMPORT C(Class) * CO(BufferedFile);
extern THIS_LIB_IMPORT C(Class) * CO(ConsoleFile);
extern THIS_LIB_IMPORT C(Class) * CO(DualPipe);
extern THIS_LIB_IMPORT C(Class) * CO(ErrorCode);
extern THIS_LIB_IMPORT C(Class) * CO(ErrorLevel);
extern THIS_LIB_IMPORT C(Class) * CO(File);
extern THIS_LIB_IMPORT C(Class) * CO(FileAttribs);
extern THIS_LIB_IMPORT C(Class) * CO(FileChange);
extern THIS_LIB_IMPORT C(Class) * CO(FileListing);
extern THIS_LIB_IMPORT C(Class) * CO(FileLock);
extern THIS_LIB_IMPORT C(Class) * CO(FileMonitor);
extern THIS_LIB_IMPORT C(Class) * CO(FileOpenMode);
extern THIS_LIB_IMPORT C(Class) * CO(FileSeekMode);
extern THIS_LIB_IMPORT C(Class) * CO(FileSize);
extern THIS_LIB_IMPORT C(Class) * CO(FileSize64);
extern THIS_LIB_IMPORT C(Class) * CO(FileStats);
extern THIS_LIB_IMPORT C(Class) * CO(GuiErrorCode);
extern THIS_LIB_IMPORT C(Class) * CO(LoggingMode);
extern THIS_LIB_IMPORT C(Class) * CO(MoveFileOptions);
extern THIS_LIB_IMPORT C(Class) * CO(PipeOpenMode);
extern THIS_LIB_IMPORT C(Class) * CO(SysErrorCode);
extern THIS_LIB_IMPORT C(Class) * CO(TempFile);
extern THIS_LIB_IMPORT C(Class) * CO(CharCategories);
extern THIS_LIB_IMPORT C(Class) * CO(CharCategory);
extern THIS_LIB_IMPORT C(Class) * CO(PredefinedCharCategories);
extern THIS_LIB_IMPORT C(Class) * CO(UnicodeDecomposition);
extern THIS_LIB_IMPORT C(Class) * CO(ECONGlobalSettings);
extern THIS_LIB_IMPORT C(Class) * CO(ECONParser);
extern THIS_LIB_IMPORT C(Class) * CO(GlobalAppSettings);
extern THIS_LIB_IMPORT C(Class) * CO(GlobalSettingType);
extern THIS_LIB_IMPORT C(Class) * CO(GlobalSettings);
extern THIS_LIB_IMPORT C(Class) * CO(GlobalSettingsData);
extern THIS_LIB_IMPORT C(Class) * CO(GlobalSettingsDriver);
extern THIS_LIB_IMPORT C(Class) * CO(JSONFirstLetterCapitalization);
extern THIS_LIB_IMPORT C(Class) * CO(JSONGlobalSettings);
extern THIS_LIB_IMPORT C(Class) * CO(JSONParser);
extern THIS_LIB_IMPORT C(Class) * CO(JSONResult);
extern THIS_LIB_IMPORT C(Class) * CO(JSONTypeOptions);
extern THIS_LIB_IMPORT C(Class) * CO(OptionsMap);
extern THIS_LIB_IMPORT C(Class) * CO(SetBool);
extern THIS_LIB_IMPORT C(Class) * CO(SettingsIOResult);
extern THIS_LIB_IMPORT C(Class) * CO(Condition);
extern THIS_LIB_IMPORT C(Class) * CO(Mutex);
extern THIS_LIB_IMPORT C(Class) * CO(Semaphore);
extern THIS_LIB_IMPORT C(Class) * CO(Thread);
extern THIS_LIB_IMPORT C(Class) * CO(ThreadPriority);


////////////////////////////////////////////////// dll function imports //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


extern LIB_IMPORT const char * __eCNameSpace__eC__i18n__GetTranslatedString(constString name, const char * string, const char * stringAndContext);
extern LIB_IMPORT void __eCNameSpace__eC__i18n__LoadTranslatedStrings(constString moduleName, const char * name);
extern LIB_IMPORT void __eCNameSpace__eC__i18n__UnloadTranslatedStrings(constString name);
extern LIB_IMPORT void __eCNameSpace__eC__types__MemoryGuard_PopLoc(void);
extern LIB_IMPORT void __eCNameSpace__eC__types__MemoryGuard_PushLoc(const char * loc);
extern LIB_IMPORT C(BitMember) * __eCNameSpace__eC__types__eClass_AddBitMember(C(Class) * _class, const char * name, const char * type, int bitSize, int bitPos, C(AccessMode) declMode);
extern LIB_IMPORT C(ClassProperty) * __eCNameSpace__eC__types__eClass_AddClassProperty(C(Class) * _class, const char * name, const char * dataType, void * setStmt, void * getStmt);
extern LIB_IMPORT C(DataMember) * __eCNameSpace__eC__types__eClass_AddDataMember(C(Class) * _class, const char * name, const char * type, uint size, uint alignment, C(AccessMode) declMode);
extern LIB_IMPORT C(bool) __eCNameSpace__eC__types__eClass_AddMember(C(Class) * _class, C(DataMember) * dataMember);
extern LIB_IMPORT C(Method) * __eCNameSpace__eC__types__eClass_AddMethod(C(Class) * _class, const char * name, const char * type, void * function, C(AccessMode) declMode);
extern LIB_IMPORT C(Property) * __eCNameSpace__eC__types__eClass_AddProperty(C(Class) * _class, const char * name, const char * dataType, void * setStmt, void * getStmt, C(AccessMode) declMode);
extern LIB_IMPORT C(ClassTemplateParameter) * __eCNameSpace__eC__types__eClass_AddTemplateParameter(C(Class) * _class, const char * name, C(TemplateParameterType) type, const void * info, C(ClassTemplateArgument) * defaultArg);
extern LIB_IMPORT C(Method) * __eCNameSpace__eC__types__eClass_AddVirtualMethod(C(Class) * _class, const char * name, const char * type, void * function, C(AccessMode) declMode);
extern LIB_IMPORT void __eCNameSpace__eC__types__eClass_DestructionWatchable(C(Class) * _class);
extern LIB_IMPORT void __eCNameSpace__eC__types__eClass_DoneAddingTemplateParameters(C(Class) * base);
extern LIB_IMPORT C(ClassProperty) * __eCNameSpace__eC__types__eClass_FindClassProperty(C(Class) * _class, const char * name);
extern LIB_IMPORT C(DataMember) * __eCNameSpace__eC__types__eClass_FindDataMember(C(Class) * _class, const char * name, C(Module) module, C(DataMember) * subMemberStack, int * subMemberStackPos);
extern LIB_IMPORT C(DataMember) * __eCNameSpace__eC__types__eClass_FindDataMemberAndId(C(Class) * _class, const char * name, int * id, C(Module) module, C(DataMember) * subMemberStack, int * subMemberStackPos);
extern LIB_IMPORT C(DataMember) * __eCNameSpace__eC__types__eClass_FindDataMemberAndOffset(C(Class) * _class, const char * name, uint * offset, C(Module) module, C(DataMember) * subMemberStack, int * subMemberStackPos);
extern LIB_IMPORT C(Method) * __eCNameSpace__eC__types__eClass_FindMethod(C(Class) * _class, const char * name, C(Module) module);
extern LIB_IMPORT void __eCNameSpace__eC__types__eClass_FindNextMember(C(Class) * _class, C(Class) * curClass, C(DataMember) * curMember, C(DataMember) * subMemberStack, int * subMemberStackPos);
extern LIB_IMPORT C(Property) * __eCNameSpace__eC__types__eClass_FindProperty(C(Class) * _class, const char * name, C(Module) module);
extern LIB_IMPORT subclass(ClassDesignerBase) __eCNameSpace__eC__types__eClass_GetDesigner(C(Class) * _class);
extern LIB_IMPORT int64 __eCNameSpace__eC__types__eClass_GetProperty(C(Class) * _class, const char * name);
extern LIB_IMPORT C(bool) __eCNameSpace__eC__types__eClass_IsDerived(C(Class) * _class, C(Class) * from);
extern LIB_IMPORT void __eCNameSpace__eC__types__eClass_Resize(C(Class) * _class, int newSize);
extern LIB_IMPORT void __eCNameSpace__eC__types__eClass_SetProperty(C(Class) * _class, const char * name, int64 value);
extern LIB_IMPORT void __eCNameSpace__eC__types__eClass_Unregister(C(Class) * _class);
extern LIB_IMPORT C(Application) __eCNameSpace__eC__types__eCrt_Initialize(C(bool) guiApp, int argc, char * argv[]);
extern LIB_IMPORT void __eCNameSpace__eC__types__eEnum_AddFixedValue(C(Class) * _class, const char * string, int64 value);
extern LIB_IMPORT int64 __eCNameSpace__eC__types__eEnum_AddValue(C(Class) * _class, const char * string);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_DecRef(C(Instance) instance);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_Delete(C(Instance) instance);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_Evolve(C(Instance) * instancePtr, C(Class) * _class);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_FireSelfWatchers(C(Instance) instance, C(Property) * _property);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_FireWatchers(C(Instance) instance, C(Property) * _property);
extern LIB_IMPORT subclass(ClassDesignerBase) __eCNameSpace__eC__types__eInstance_GetDesigner(C(Instance) instance);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_IncRef(C(Instance) instance);
extern LIB_IMPORT C(bool) __eCNameSpace__eC__types__eInstance_IsDerived(C(Instance) instance, C(Class) * from);
extern LIB_IMPORT void * __eCNameSpace__eC__types__eInstance_New(C(Class) * _class);
extern LIB_IMPORT void * __eCNameSpace__eC__types__eInstance_NewEx(C(Class) * _class, C(bool) bindingsAlloc);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_SetMethod(C(Instance) instance, const char * name, void * function);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_StopWatching(C(Instance) instance, C(Property) * _property, C(Instance) object);
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_Watch(C(Instance) instance, C(Property) * _property, void * object, void (* callback)(void *, void *));
extern LIB_IMPORT void __eCNameSpace__eC__types__eInstance_WatchDestruction(C(Instance) instance, C(Instance) object, void (* callback)(void *, void *));
extern LIB_IMPORT C(DataMember) * __eCNameSpace__eC__types__eMember_AddDataMember(C(DataMember) * member, const char * name, const char * type, uint size, uint alignment, C(AccessMode) declMode);
extern LIB_IMPORT C(bool) __eCNameSpace__eC__types__eMember_AddMember(C(DataMember) * addTo, C(DataMember) * dataMember);
extern LIB_IMPORT C(DataMember) * __eCNameSpace__eC__types__eMember_New(C(DataMemberType) type, C(AccessMode) declMode);
extern LIB_IMPORT C(Module) __eCNameSpace__eC__types__eModule_Load(C(Module) fromModule, const char * name, C(AccessMode) importAccess);
extern LIB_IMPORT C(Module) __eCNameSpace__eC__types__eModule_LoadStatic(C(Module) fromModule, const char * name, C(AccessMode) importAccess, C(bool) (* Load)(C(Module) module), C(bool) (* Unload)(C(Module) module));
extern LIB_IMPORT C(Module) __eCNameSpace__eC__types__eModule_LoadStrict(C(Module) fromModule, const char * name, C(AccessMode) importAccess);
extern LIB_IMPORT void __eCNameSpace__eC__types__eModule_Unload(C(Module) fromModule, C(Module) module);
extern LIB_IMPORT void __eCNameSpace__eC__types__eProperty_SelfWatch(C(Class) * _class, const char * name, void (* callback)(void *));
extern LIB_IMPORT void __eCNameSpace__eC__types__eProperty_Watchable(C(Property) * _property);
extern LIB_IMPORT void __eCNameSpace__eC__types__eSystem_Delete(void * memory);
extern LIB_IMPORT C(Class) * __eCNameSpace__eC__types__eSystem_FindClass(C(Module) module, const char * name);
extern LIB_IMPORT C(DefinedExpression) * __eCNameSpace__eC__types__eSystem_FindDefine(C(Module) module, const char * name);
extern LIB_IMPORT C(GlobalFunction) * __eCNameSpace__eC__types__eSystem_FindFunction(C(Module) module, const char * name);
extern LIB_IMPORT void * __eCNameSpace__eC__types__eSystem_New(uint size);
extern LIB_IMPORT void * __eCNameSpace__eC__types__eSystem_New0(uint size);
extern LIB_IMPORT C(Class) * __eCNameSpace__eC__types__eSystem_RegisterClass(C(ClassType) type, const char * name, const char * baseName, int size, int sizeClass, C(bool) (* Constructor)(void *), void (* Destructor)(void *), C(Module) module, C(AccessMode) declMode, C(AccessMode) inheritanceAccess);
extern LIB_IMPORT C(DefinedExpression) * __eCNameSpace__eC__types__eSystem_RegisterDefine(const char * name, const char * value, C(Module) module, C(AccessMode) declMode);
extern LIB_IMPORT C(GlobalFunction) * __eCNameSpace__eC__types__eSystem_RegisterFunction(const char * name, const char * type, void * func, C(Module) module, C(AccessMode) declMode);
extern LIB_IMPORT void * __eCNameSpace__eC__types__eSystem_Renew(void * memory, uint size);
extern LIB_IMPORT void * __eCNameSpace__eC__types__eSystem_Renew0(void * memory, uint size);
extern LIB_IMPORT void __eCNameSpace__eC__types__eSystem_SetArgs(C(Application) app, int argc, char * argv[]);
extern LIB_IMPORT void __eCNameSpace__eC__types__eSystem_SetPoolingDisabled(C(bool) disabled);

extern C(Module) __thisModule;

extern THIS_LIB_IMPORT C(Application) ecrt_init(C(Module) fromModule, C(bool) loadEcere, C(bool) guiApp, int argc, char * argv[]);



#ifdef __cplusplus

};

#endif

#endif // !defined(__ECRT_H__)
