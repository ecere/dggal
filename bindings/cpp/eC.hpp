// Preprocessor directives can be added at the beginning (Can't store them in AST)

/****************************************************************************
===========================================================================
   Core eC Library
===========================================================================
****************************************************************************/
#if !defined(__EC_HPP__)
#define __EC_HPP__

#define ECPRFX eC_

#include "eC.h"

#include <initializer_list>
#include <tuple>
#include <array>
#include <string_view>
#include <type_traits>
#include <cstdio>

// Syntactic Sugar (NOT GENERATED)

#define UNPACK(...) __VA_ARGS__

// INSTANCEL, INSTANCE: get the C++ instance out of supplied eC instance
//            x: pointer to eC instance
//            c: eC 'Class' object representing the C++ class
#define INSTANCEL(x, c) (*(void **)((char *)(x) + (c)->offset))   // For when an l-value is needed
#define INSTANCE(x, c)  ({ void * xx = x; xx ? INSTANCEL(xx, c) : 0; }) // New extension version avoiding to evaluate x twice

#define BINDINGS_CLASS(eo) (eo && eo->_class && eo->_class->bindingsClass)

#undef   newi
#define  newi(c) Instance_newEx(c, true)

#define _REGISTER_CLASS(n, ns, bs, a) \
   (XClass *)eC_registerClass( \
         ClassType_normalClass, \
         ns, bs, \
         sizeof(Instance *), 0, \
         (C(bool) (*)(void *)) n::constructor, \
         (void(*)(void *)) n::destructor, \
         (a).impl, \
         AccessMode_privateAccess, AccessMode_publicAccess)
// For defining _class and registereing together (single translation unit)
#define REGISTER_CLASS_DEF(n, b, a)    TCPPClass<n> n::_cpp_class(_REGISTER_CLASS(n,     #n, b::_cpp_class.impl->name, a));

// For defining _class and registering separately (multiple translation units)
#define CLASS_DEF(n)                   TCPPClass<n> n::_cpp_class;

#define REGISTER_CLASS(n, b, a)        n::_cpp_class.setup(_REGISTER_CLASS(n,       #n, b::_cpp_class.impl->name, a));

// For C++ classes proxying eC classes:
#define REGISTER_CPP_CLASS(n, a)       n::_cpp_class.setup(_REGISTER_CLASS(n, "CPP" #n, #n, a));

#define EVOLVE_APP(ac, a) \
   Instance_evolve(&(a).impl, ac::_cpp_class.impl); \
   INSTANCEL((a).impl, (a).impl->_class) = &(a); \
   __thisModule = (a).impl; \
   (a).vTbl = _cpp_class.vTbl;

#define REGISTER_APP_CLASS(ac, b, a) \
   REGISTER_CLASS(ac, b, a); \
   EVOLVE_APP(ac, a)

#define _CONSTRUCT(c, b) \
   INSTANCE_VIRTUAL_METHODS_PROTO(c) \
   static TCPPClass<c> _cpp_class; \
   static C(bool) constructor(C(Instance) i, C(bool) alloc) \
   { \
      if(alloc && !INSTANCEL(i, i->_class)) \
      { \
         c * inst = new c(i, _cpp_class); \
         if(inst) \
         { \
            /* printf("Must free!\n");*/ \
            inst->mustFree = true; \
         } \
         return inst != null; \
      } \
      return true; \
   } \
   static void destructor(C(Instance) i) \
   { \
      c * inst = (c *)INSTANCEL(i, i->_class); \
      if(inst) \
      { \
         if(_cpp_class.destructor) \
            ((void (*)(c & self))_cpp_class.destructor)(*inst); \
         if(inst->mustFree) \
            delete inst; \
      } \
   } \
   explicit inline c(C(Instance) _impl, CPPClass & cl = _cpp_class) : b(_impl, cl)

// NOTE: For some reason not having the move constructors and assignment operator repeated in derived classes causes strange errors
//       e.g. with DisplaySystem::pixelFormat and DisplaySystem::flags properties
#define MOVE_CONSTRUCTORS(c) \
   inline c(c && i) \
   { \
      Instance * self = (Instance *)this; \
      self->impl = i.impl; \
      self->vTbl = i.vTbl; \
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */ \
      i.impl = null; \
      i.vTbl = null; \
   } \
   inline c & operator= (c && i) \
   { \
      Instance * self = (Instance *)this; \
      if(self->impl) \
      { \
         C(Instance) impl = self->impl; \
         int refCount = impl->_refCount; \
         Instance_decRef(impl); \
         if(refCount > 1) \
         { \
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class); \
            if(inst && *inst == self) \
               *inst = null; \
         } \
      } \
      self->impl = i.impl; \
      self->vTbl = i.vTbl; \
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */ \
      i.impl = null; \
      i.vTbl = null; \
      return *this; \
   }

// NOTE: For some reason not having the move constructors and assignment operator repeated in derived classes causes strange errors
//       e.g. with DisplaySystem::pixelFormat and DisplaySystem::flags properties
#define TMOVE_CONSTRUCTORS(c, t) \
   inline c UNPACK t(c UNPACK t && i) \
   { \
      Instance * self = (Instance *)this; \
      self->impl = i.impl; \
      self->vTbl = i.vTbl; \
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */ \
      i.impl = null; \
      i.vTbl = null; \
   } \
   inline c UNPACK t & operator= (c UNPACK t && i) \
   { \
      Instance * self = (Instance *)this; \
      if(self->impl) \
      { \
         C(Instance) impl = self->impl; \
         int refCount = impl->_refCount; \
         Instance_decRef(impl); \
         if(refCount > 1) \
         { \
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class); \
            if(inst && *inst == self) \
               *inst = null; \
         } \
      } \
      self->impl = i.impl; \
      self->vTbl = i.vTbl; \
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */ \
      i.impl = null; \
      i.vTbl = null; \
      return *this; \
   }

#define CONSTRUCT(c, b) \
   MOVE_CONSTRUCTORS(c) \
   c() : c((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { } \
   _CONSTRUCT(c, b)

#define TCONSTRUCT(c, b, t) \
   TMOVE_CONSTRUCTORS(T ## c,  (UNPACK t)) \
   T ## c() : T ## c((C(Instance))Instance_newEx(ensureTemplatized  UNPACK t (_cpp_class, #c).impl, false), ensureTemplatized  UNPACK t (_cpp_class, #c)) { } \
   INSTANCE_VIRTUAL_METHODS_PROTO(T ## c) \
   static TCPPClass<T ## c> _cpp_class; \
   static C(bool) constructor(C(Instance) i, C(bool) alloc) \
   { \
      if(alloc && !INSTANCEL(i, i->_class)) \
      { \
         T ## c  UNPACK t  * inst = new T ## c  UNPACK t (i, _cpp_class); \
         if(inst) \
         { \
            /*printf("Must free!\n");*/ \
            inst->mustFree = true; \
         } \
         return inst != null; \
      } \
      return true; \
   } \
   static void destructor(C(Instance) i) \
   { \
      T ## c  UNPACK t * inst = (T ## c  UNPACK t *)INSTANCEL(i, i->_class); \
      if(inst) \
      { \
         if(_cpp_class.destructor) ((void (*)(T ## c  UNPACK t  & self))_cpp_class.destructor)(*inst); \
         if(inst->mustFree) delete inst; \
      } \
   } \
   explicit inline T ## c  UNPACK t (C(Instance) _impl, CPPClass & cl = _cpp_class) : b(_impl, cl)

#define DESTRUCT(c) \
   ((TCPPClass<c> &)_cpp_class).destructor

#define REGISTER() \
   static void class_registration(CPPClass & _cpp_class)
#define REGISTER_PROTO() \
   static void class_registration(CPPClass & _cpp_class);
#define REGISTER_IMPL(c) \
   void c::class_registration(CPPClass & _cpp_class)
#define TREGISTER_PROTO(t) \
   static void class_registration(CPPClass & _cpp_class);
#define TREGISTER_IMPL(c, t) \
   t void T ## ca::class_registration(CPPClass & _cpp_class)
#if !defined(__LINK_ECERE__)
#define __LINK_ECERE__ 1
#endif

#define APP_CONSTRUCT(c, b) \
   inline c() : c(eC_init_CALL(__LINK_ECERE__)) { } \
   _CONSTRUCT(c, b)

#if !defined(__WIN32__) || defined(__CONSOLE_APP__)
   #define APP_SET_ARGS(a) eC_setArgs(a.impl, argc, argv);
#else
   #define APP_SET_ARGS(a)
#endif

#define MAIN_DEFINITION \
   extern "C" MAIN_DECLARATION \
   { \
      APP_SET_ARGS(app); \
      app.main(); \
      unloadTranslatedStrings(MODULE_NAME); \
      return app.exitCode; \
   }

// SELF: get C++ class instance pointer from within hackish member of the class
#define CONTAINER_OF(ptr, type, member) ((type *)(((char *)ptr) + 0x10 - (char *)&((type *)0x10)->member))
#define TCONTAINER_OF(ptr, type, t, member) ((T ## type UNPACK t *)(((char *)ptr) + 0x10 - (char *)&((T ## type UNPACK t *)0x10)->member))
#define SELF(c, member)  __attribute__((unused)) c * self = CONTAINER_OF(this, c, member);
#define TSELF(c, t, member)  __attribute__((unused)) T ## c UNPACK t * self = TCONTAINER_OF(this, c, (UNPACK t), member);
#define getimpli(i)  ((Instance)i).impl

#define VIRTUAL_METHOD_PROTO(n, ncpp, c, b, r, p0, ep, p) \
   struct c ## _ ## n ## _Functor \
   { \
      [[no_unique_address]] int _[0]; \
      typedef r (* FunctionType)(p0 p); \
      inline FunctionType operator= (FunctionType func); \
      inline r operator()(ep p); \
   } ncpp;
   // inline static void register_ ## n(CPPClass & cl, c::c ## _ ## n ## _Functor::FunctionType func)

#define VIRTUAL_METHOD(n, ncpp, c, b, r, p0, ep, p, d) \
   inline c::c ## _ ## n ## _Functor::FunctionType c::c ## _ ## n ## _Functor::operator= (FunctionType func) \
   { \
      SELF(c, ncpp); \
      if(self->vTbl == c::_cpp_class.vTbl) \
      { \
         uint size = c::_cpp_class.impl->vTblSize; \
         self->vTbl = (void (**)())newt(c::c ## _ ## n ## _Functor::FunctionType, size); \
         memcpy(self->vTbl, c::_cpp_class.vTbl, sizeof(c::c ## _ ## n ## _Functor::FunctionType) * size); \
      } \
      ((c::c ## _ ## n ## _Functor::FunctionType *)self->vTbl)[M_VTBLID(b, n)] = func; \
      return func; \
   } \
   inline r c::c ## _ ## n ## _Functor::operator()(ep p) \
   { \
      SELF(c, ncpp); \
      d \
   }
   // inline void c::register_ ## n(CPPClass & cl, c::c ## _ ## n ## _Functor::FunctionType func)
   // {
   //    ((c::c ## _ ## n ## _Functor::FunctionType *)cl.vTbl)[M_VTBLID(b, n)] = func;
   // }

#define TVIRTUAL_METHOD_PROTO(n, ncpp, c, b, r, p0, ep, p) \
   struct c ## _ ## n ## _Functor \
   { \
      [[no_unique_address]] int _[0]; \
      typedef r (* FunctionType)(p0 p); \
      inline FunctionType operator= (FunctionType func); \
      inline r operator()(ep p); \
   } ncpp;
   // inline static void register_ ## n(CPPClass & cl, c::c ## _ ## n ## _Functor::FunctionType func)

#define TVIRTUAL_METHOD(n, ncpp, c, t, t2, b, r, p0, ep, p, d) \
   t2 \
   inline typename T ## c UNPACK t::c ## _ ## n ## _Functor::FunctionType T ## c UNPACK t::c ## _ ## n ## _Functor::operator= (FunctionType func) \
   { \
      TSELF(c, (t), ncpp); \
      if(self->vTbl == T ## c UNPACK t::_cpp_class.vTbl) \
      { \
         uint size = T ## c UNPACK t::_cpp_class.impl->vTblSize; \
         self->vTbl = (void (**)())eC_new(sizeof(T ## c UNPACK t::c ## _ ## n ## _Functor::FunctionType) * size); \
         memcpy(self->vTbl, T ## c UNPACK t::_cpp_class.vTbl, sizeof(T ## c UNPACK t::c ## _ ## n ## _Functor::FunctionType) * size); \
      } \
      ((T ## c UNPACK t::c ## _ ## n ## _Functor::FunctionType *)self->vTbl)[M_VTBLID(b, n)] = func; \
      return func; \
   } \
   t2 \
   inline r T ## c UNPACK t::c ## _ ## n ## _Functor::operator()(ep p) \
   { \
      TSELF(c, (t), ncpp); \
      d \
   }
   // inline void c::register_ ## n(CPPClass & cl, c::c ## _ ## n ## _Functor::FunctionType func)
   // {
   //    ((c::c ## _ ## n ## _Functor::FunctionType *)cl.vTbl)[M_VTBLID(b, n)] = func;
   // }

#define _REGISTER_METHOD(cp1, cp2, ns, n, bc, c, r, p, ocl, oi, code, ea, rv) \
   addMethod(_cpp_class.impl, ns, (void *) +[]p \
   { \
      XClass * cl = (ocl) ? (XClass *)(ocl)->_class : null; \
cp1 \
      c * i = (oi) ? (c *)INSTANCEL(oi, cl) : null; \
      int vid = M_VTBLID(bc, n); \
      bc ## _ ## n ## _Functor::FunctionType fn; \
      if(i && i->vTbl && i->vTbl[vid]) \
      { \
         fn = (bc ## _ ## n ## _Functor::FunctionType) i->vTbl[vid]; \
         code; \
      } \
cp2 \
      else \
      { \
         auto method = ((r (*) p)(CO(c)->_vTbl)[M_VTBLID(bc, n)]); \
         if(method) return method ea; \
      } \
      return rv; \
   })

#define REGISTER_METHOD(ns, n, bc, c, r, p, ocl, oi, code, ea, rv) \
   _REGISTER_METHOD(, , ns, n, bc, c, r, p, ocl, oi, code, ea, rv)

#define REGISTER_TYPED_METHOD(ns, n, bc, c, r, p, ocl, oi, code, ea, rv) \
   _REGISTER_METHOD(      CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;,       else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid]) \
      { \
         fn = (bc ## _ ## n ## _Functor::FunctionType) cppcl->vTbl[vid]; \
code; \
      }, ns, n, bc, c, r, p, ocl, oi, code, ea, rv)

#define _TREGISTER_METHOD(cp1, cp2, ns, n, bc, c, t, r, p, ocl, oi, code, ea, rv) \
   addMethod(_cpp_class.impl, ns, (void *) +[]p \
   { \
      XClass * cl = (ocl) ? (XClass *)(ocl)->_class : null; \
cp1 \
      T ## c t * i = (oi) ? (T ## c t *)INSTANCEL(oi, cl) : null; \
      int vid = M_VTBLID(bc, n); \
      typename bc ## _ ## n ## _Functor::FunctionType fn; \
      if(i && i->vTbl && i->vTbl[vid]) \
      { \
         fn = (typename bc ## _ ## n ## _Functor::FunctionType) i->vTbl[vid]; \
         code; \
      } \
cp2 \
      else \
      { \
         auto method = ((r (*) p)(CO( ## c)->_vTbl)[M_VTBLID(bc, n)]); \
         if(method) return method ea; \
      } \
      return rv; \
   })

#define TREGISTER_METHOD(ns, n, bc, c, t, r, p, ocl, oi, code, ea, rv) \
   _TREGISTER_METHOD(, , ns, n, bc, c, (UNPACK t), r, p, ocl, oi, code, ea, rv)

#define TREGISTER_TYPED_METHOD(ns, n, bc, c, t, r, p, ocl, oi, code, ea, rv) \
   _TREGISTER_METHOD(      CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;,       else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid]) \
      { \
         fn = (bc ## _ ## n ## _Functor::FunctionType) cppcl->vTbl[vid]; \
code; \
      }, ns, n, bc, c, (UNPACK t), r, p, ocl, oi, code, ea, rv)

template<typename T, typename U> struct is_same       { static const bool value = false; };
template<typename T>             struct is_same<T, T> { static const bool value = true; };
template<typename T, typename U> bool eqTypes()       { return is_same<T, U>::value; }

template <std::string_view const&... Strs>
struct join
{
    // Helper to get a string literal from a std::array
    template <std::size_t N, std::array<char, N> const& S, typename>
    struct to_char_array;
    template <std::size_t N, std::array<char, N> const& S, std::size_t... I>
    struct to_char_array<N, S, std::index_sequence<I...>>
    {
        static constexpr const char value[]{S[I]..., 0};
    };
    // Join all strings into a single std::array of chars
    static constexpr auto impl() noexcept
    {
        constexpr std::size_t len = (Strs.size() + ... + ((sizeof...(Strs) - 1) * 2));
        std::array<char, len + 1> arr{};
        auto append = [i = 0, first = true, &arr](auto const& s) mutable {
            if (!first) {
                arr[i++] = ',';
                arr[i++] = ' ';
            }
            first = false;
            for (auto c : s) arr[i++] = c;
        };
        (append(Strs), ...);
        arr[len] = 0;
        return arr;
    }
    // Give the joined string static storage
    static constexpr auto arr = impl();
    // Convert to a string literal, then view as a std::string_view
    static constexpr std::string_view value =
        to_char_array<arr.size(), arr, std::make_index_sequence<arr.size()>>::value;
};

template <std::string_view const&... Strs>
struct join_nocomma
{
    // Helper to get a string literal from a std::array
    template <std::size_t N, std::array<char, N> const& S, typename>
    struct to_char_array;
    template <std::size_t N, std::array<char, N> const& S, std::size_t... I>
    struct to_char_array<N, S, std::index_sequence<I...>>
    {
        static constexpr const char value[]{S[I]..., 0};
    };
    // Join all strings into a single std::array of chars
    static constexpr auto impl() noexcept
    {
        constexpr std::size_t len = (Strs.size() + ...);
        std::array<char, len + 1> arr{};
        auto append = [i = 0, &arr](auto const& s) mutable {
            for (auto c : s) arr[i++] = c;
        };
        (append(Strs), ...);
        arr[len] = 0;
        return arr;
    }
    // Give the joined string static storage
    static constexpr auto arr = impl();
    // Convert to a string literal, then view as a std::string_view
    static constexpr std::string_view value =
        to_char_array<arr.size(), arr, std::make_index_sequence<arr.size()>>::value;
};

template<typename T> struct is_const            { static const bool value = false; };
template<typename T> struct is_const<const T>   { static const bool value = true; };
template<typename T> constexpr std::string_view isConst()   { return is_const<T>::value ? "const " : ""; }

template <typename T> struct TypeName  { static constexpr const char* get() { return "int";    } };
template <> struct TypeName<int>       { static constexpr const char* get() { return "int";    } };
template <> struct TypeName<double>    { static constexpr const char* get() { return "double"; } };
template <> struct TypeName<const char *>    { static constexpr const char* get() { return "const char *"; } };

template <typename T> static constexpr std::string_view stringifyConst = isConst<T>();
template <typename T> static constexpr std::string_view stringifyTypeName = TypeName<T>::get();
template <typename T> static constexpr std::string_view stringifyType = join_nocomma<stringifyConst<T>, stringifyTypeName<T>>::value;
template <typename... T> static constexpr const char * getTemplateArgsString = join<stringifyType<T>...>::value.data();

#define REGVMETHOD(b, n, m, p, t, a) \
    if(!eqTypes<decltype(&m), decltype(&b::n)>()) \
       ((b::b ## _ ## n ## _Functor::FunctionType *)_cpp_class.vTbl)[M_VTBLID(b, n)] = +[]p { return ((t &)self).m a; };

#define TREGVMETHOD(tl, b, n, m, p, t, a) \
    if(!eqTypes<decltype(&m), decltype(&tl::n)>()) \
       ((tl::b ## _ ## n ## _Functor::FunctionType *)_cpp_class.vTbl)[M_VTBLID(b, n)] = +[]p { return ((t &)self).m a; };

#define propertyProto(n, sg) \
struct n ## _Prop \
   { \
      constexpr n ## _Prop() :_() { }; \
      [[no_unique_address]] int _[0]; \
      sg \
   } n;

#if 0 // FIXME: These macros are broken
#define _setProto(t, t2, n, c) \
   /*nstSet*/inline t ## t2 operator= (ttAmp vt2);

#define setProto(t, t2, n, c) \
   _setProto(t, t2, n, c/*l5 c:c te:False tp:tp tp2:tp2 */)   /*regSet*/inline c::n ## _Prop & operator= (c::n ## _Prop & prop);

#endif

#define getProto(r, t, t2, n, c) \
   /*regGet*/inline r operator t () const;

#define propertyImpl(n, sg) \
   sg \

#define _setImpl(t, t2, n, c, d) \
   /*nstSet*/inline t ## t2 c::n ## _Prop::operator= (ttAmp v ## t2) \
   { \
      SELF(c, n); \
      d \
      return v; \
   }

#define setImpl(t, t2, n, c, d) \
   _setImpl(t, t2, n, c/*l6 c:c te:False tp:tp tp2:tp2 */, d)   /*regSet*/inline c::n ## _Prop & c::n ## _Prop::operator= (c::n ## _Prop & prop) \
   { \
      SELF(c, n); \
      t ## t2 v = prop; \
      d \
      return prop; \
   }

#define getImpl(r, t, t2, n, c, d) \
   /*regGet*/inline r c::n ## _Prop::operator t () const \
   { \
      SELF(c, n); \
      d \
   }

#define TCTCO  <TC, TCO>

#define _nhsetImpl(t, t2, n, c, d) \
   /*nstSet*/template <class TC, C(Class) ** TCO> inline t ## t2 T ## c<TC, TCO>::n ## _Prop::operator= (ttAmp v ## t2) \
   { \
      SELF(c, n); \
      d \
      return v; \
   }

#define nhsetImpl(t, t2, n, c, d) \
   _nhsetImpl(t, t2, n, c/*l6 c:c te:False tp:tp tp2:tp2 */, d)   /*regSet*/template <class TC, C(Class) ** TCO> inline typename T ## c<TC, TCO>::n ## _Prop & T ## c<TC, TCO>::n ## _Prop::operator= (typename T ## c<TC, TCO>::n ## _Prop & prop) \
   { \
      SELF(c, n); \
      t ## t2 v = prop; \
      d \
      return prop; \
   }

#define nhgetImpl(r, t, t2, n, c, d) \
   /*regGet*/template <class TC, C(Class) ** TCO> inline r T ## c<TC, TCO>::n ## _Prop::operator t () const \
   { \
      __attribute__((unused)) T ## c TCTCO * self = CONTAINER_OF(this, T ## c TCTCO, n); \
      d \
   }

#define _tsetProto(t, t2, n, c, tp) \
   /*nstSet*/inline t ## t2 operator= (ttAmp vt2);

#define tsetProto(t, t2, n, c, tp) \
   _setProto(t, t2, n, c/*l5 c:c te:False tp:tp tp2:tp2 */)   /*regSet*/inline T ## c UNPACK tp ::n ## _Prop & operator= (T ## c UNPACK tp ::n ## _Prop & prop);

#define tgetProto(r, t, t2, n, c, tp) \
   /*regGet*/inline r operator t () const;

#define _tsetImpl(t, t2, n, c, tp, tp2, d) \
   /*nstSet*/tp2 inline t ## t2 T ## c UNPACK tp ::n ## _Prop::operator= (ttAmp v ## t2) \
   { \
      TSELF(c,  (UNPACK tp), n); \
      d \
      return v; \
   }

#define tsetImpl(t, t2, n, c, tp, tp2, d) \
   _tsetImpl(t, t2, n, c/*l6 c:c te:True tp:tp tp2:tp2 */, (tp), (tp2), d)   /*regSet*/tp2 inline typename T ## c UNPACK tp ::n ## _Prop & T ## c UNPACK tp ::n ## _Prop::operator= (T ## c UNPACK tp ::n ## _Prop & prop) \
   { \
      TSELF(c,  (UNPACK tp), n); \
      t ## t2 v = prop; \
      d \
      return prop; \
   }

#define tgetImpl(r, t, t2, n, c, tp, tp2, d) \
   /*regGet*/tp2 inline r T ## c UNPACK tp ::n ## _Prop::operator t () const \
   { \
      TSELF(c,  (UNPACK tp), n); \
      d \
   }

extern "C" C(Module) ecere_init(C(Module) fromModule);

class XClass : public C(Class) { };

static int cppClassCount;

class CPPClass
{
public:
   void * operator new   (uintsize count) { return eC_new(count); }
   void * operator new [](uintsize count) { return eC_new(count); }
   void operator delete   (void * ptr) { eC_delete(ptr); }
   void operator delete [](void * ptr) { eC_delete(ptr); }

   typedef void (* Function)(void);
   XClass * impl;
   Function * vTbl;
   char * name;
   inline CPPClass() { cppClassCount++; };
   inline CPPClass(const CPPClass & c) = delete;
   inline CPPClass(CPPClass && c)
   {
      name = c.name;
      impl = c.impl;
      vTbl = c.vTbl;
      c.name = null;
      c.impl = null;
      c.vTbl = null;
   }
   ~CPPClass()
   {
      if(!--cppClassCount)
      {
         //printf("No more classes\n");
         F(checkMemory)();
      }
   }
};

template <class TC>
class TCPPClass : public CPPClass
{
public:
   TCPPClass() { }
   TCPPClass(XClass * _impl)
   {
      setup(_impl);
   }
   void (*destructor)(TC &);
   void setup(XClass * _impl)
   {
      eC_delete(name);
      impl = _impl;
      if(impl)
      {
         name = F(copyString)(impl->name);
         _impl->bindingsClass = this;
         if(vTbl)
            eC_delete(vTbl);
         vTbl = newt(Function, impl->vTblSize);
         memset(vTbl, 0, sizeof(Function) * impl->vTblSize);
         TC::class_registration(*this);
      }
   }
   ~TCPPClass()
   {
      if(impl)
         eC_delete(vTbl);
      eC_delete(name);
   }
};

template<typename ... TPs> inline CPPClass & ensureTemplatized(CPPClass & _class, const char * name)
{
   if(!_class.impl || !_class.impl->templateClass)
   {
      char type[1024];
      strcpy(type, "CPP");
      strcat(type, name);
      strcat(type, "<");
      strcat(type, getTemplateArgsString<TPs...>);
      strcat(type, ">");
      _class.impl = (XClass *)eC_findClass(__thisModule, type);
      if(_class.impl) _class.impl->bindingsClass = &_class;
   }
   return _class;
}

template <class TC>
class TIH
{
public:
   void * operator new   (uintsize count) { return eC_new(count); }
   void * operator new [](uintsize count) { return eC_new(count); }
   void operator delete   (void * ptr) { eC_delete(ptr); }
   void operator delete [](void * ptr) { eC_delete(ptr); }
   TC * object;

   bool ownRef = false;

   TIH(C(Instance) eo) : TIH(*(BINDINGS_CLASS(eo) ? (TC *)INSTANCE(eo, eo->_class) : new TC(eo)))
   {
      if(!BINDINGS_CLASS(eo) || !INSTANCE(eo, eo->_class))
      {
         ownRef = true;
      }
      else
      {
         INSTANCEL(eo, eo->_class) = object;
         eo->_refCount++;
         ownRef = false;
      }
   }
   TIH(TC & o) : object(&o)
   {
      if(o.impl)
         o.impl->_refCount++;
   }

   TIH(const TIH & h) : object(h.object)
   {
      if(object && object->impl)
         object->impl->_refCount++;
      ownRef = true;
   }

   TIH & operator= (const TIH & h)
   {
      if(object && object->impl)
         deletei(object->impl);

      ownRef = h.ownRef;
      object = h.object;
      if(object && object->impl)
         object->impl->_refCount++;
      return *this;
   }

   ~TIH()
   {
      if(object)
      {
         if(object->impl)
            object->impl->_refCount--;
         if(ownRef)
            delete object;
      }
   }

   TC& operator*() const { return *object; }
   TC* operator->() const { return object; }
};

class DataBox;
class IOChannel;
class Surface;
class Window;

template <class TC, C(Class) ** TCO> class TAVLNode;
typedef TAVLNode<C(AVLNode), &CO(AVLNode)> AVLNode;
class AVLTree;
template <typename TP_AT> class TAVLTree;
enum class Access_Mode : int;
class Angle;
class Application;
class Array;
template <typename TP_T, typename TP_I, typename TP_D> class TArray;
template <class TC, C(Class) ** TCO> class TBTNamedLink;
typedef TBTNamedLink<C(BTNamedLink), &CO(BTNamedLink)> BTNamedLink;
template <class TC, C(Class) ** TCO> class TBitMember;
typedef TBitMember<C(BitMember), &CO(BitMember)> BitMember;
struct BuiltInContainer;
template <class TC, C(Class) ** TCO> class TClass;
typedef TClass<C(Class), &CO(Class)> Class;
template <class TC, C(Class) ** TCO> class TClassProperty;
typedef TClassProperty<C(ClassProperty), &CO(ClassProperty)> ClassProperty;
struct ClassTemplateArgument;
template <class TC, C(Class) ** TCO> class TClassTemplateParameter;
typedef TClassTemplateParameter<C(ClassTemplateParameter), &CO(ClassTemplateParameter)> ClassTemplateParameter;
enum class ClassType : int;
class Container;
template <typename TP_T, typename TP_I, typename TP_D> class TContainer;
class CustomAVLTree;
template <typename TP_BT, typename TP_KT> class TCustomAVLTree;
template <class TC, C(Class) ** TCO> class TDataMember;
typedef TDataMember<C(DataMember), &CO(DataMember)> DataMember;
enum class DataMemberType : int;
struct DataValue;
template <class TC, C(Class) ** TCO> class TDefinedExpression;
typedef TDefinedExpression<C(DefinedExpression), &CO(DefinedExpression)> DefinedExpression;
template <class TC, C(Class) ** TCO> class TEnumClassData;
typedef TEnumClassData<C(EnumClassData), &CO(EnumClassData)> EnumClassData;
template <class TC, C(Class) ** TCO> class TGlobalFunction;
typedef TGlobalFunction<C(GlobalFunction), &CO(GlobalFunction)> GlobalFunction;
class HashMap;
template <typename TP_KT, typename TP_VT> class THashMap;
struct HashMapIterator;
template <typename TP_KT, typename TP_VT> class THashMapIterator;
class HashTable;
template <typename TP_KT> class THashTable;
class IOChannel;
enum class ImportType : int;
class Instance;
struct Iterator;
template <typename TP_T, typename TP_IT> class TIterator;
template <class TC, C(Class) ** TCO> class TIteratorPointer;
typedef TIteratorPointer<C(IteratorPointer), &CO(IteratorPointer)> IteratorPointer;
template <class TC, C(Class) ** TCO> class TLink;
typedef TLink<C(Link), &CO(Link)> Link;
struct LinkElement;
template <typename TP_T> class TLinkElement;
class LinkList;
template <typename TP_LT> class TLinkList;
class List;
template <typename TP_LLT> class TList;
template <class TC, C(Class) ** TCO> class TListItem;
typedef TListItem<C(ListItem), &CO(ListItem)> ListItem;
class Map;
template <typename TP_MT, typename TP_V> class TMap;
struct MapIterator;
template <typename TP_KT, typename TP_V> class TMapIterator;
template <class TC, C(Class) ** TCO> class TMapNode;
typedef TMapNode<C(MapNode), &CO(MapNode)> MapNode;
template <class TC, C(Class) ** TCO> class TMethod;
typedef TMethod<C(Method), &CO(Method)> Method;
enum class MethodType : int;
class Module;
struct NameSpace;
template <class TC, C(Class) ** TCO> class TObjectInfo;
typedef TObjectInfo<C(ObjectInfo), &CO(ObjectInfo)> ObjectInfo;
enum class ObjectNotationType : uint;
enum class Platform : int;
template <class TC, C(Class) ** TCO> class TProperty;
typedef TProperty<C(Property), &CO(Property)> Property;
class SerialBuffer;
struct StaticString;
template <class TC, C(Class) ** TCO> class TSubModule;
typedef TSubModule<C(SubModule), &CO(SubModule)> SubModule;
enum class TemplateMemberType : int;
enum class TemplateParameterType : int;
template <class TC, C(Class) ** TCO> class TBTNode;
typedef TBTNode<C(BTNode), &CO(BTNode)> BTNode;
enum class BackSlashEscaping : uint;
struct BinaryTree;
class EscapeCStringOptions;
template <class TC, C(Class) ** TCO> class TItem;
typedef TItem<C(Item), &CO(Item)> Item;
template <class TC, C(Class) ** TCO> class TMutex;
typedef TMutex<C(Mutex), &CO(Mutex)> Mutex;
template <class TC, C(Class) ** TCO> class TNamedItem;
typedef TNamedItem<C(NamedItem), &CO(NamedItem)> NamedItem;
template <class TC, C(Class) ** TCO> class TNamedLink;
typedef TNamedLink<C(NamedLink), &CO(NamedLink)> NamedLink;
template <class TC, C(Class) ** TCO> class TNamedLink64;
typedef TNamedLink64<C(NamedLink64), &CO(NamedLink64)> NamedLink64;
template <class TC, C(Class) ** TCO> class TOldLink;
typedef TOldLink<C(OldLink), &CO(OldLink)> OldLink;
struct OldList;
enum class StringAllocType : int;
template <class TC, C(Class) ** TCO> class TStringBTNode;
typedef TStringBTNode<C(StringBTNode), &CO(StringBTNode)> StringBTNode;
struct StringBinaryTree;
enum class TreePrintStyle : int;
class ZString;

int eC_cpp_init(const Module & module);


enum class Alignment : int;

class DataDisplayFlags
{
public:
   C(DataDisplayFlags) impl;
   constexpr DataDisplayFlags() : impl(0) { }
   constexpr DataDisplayFlags(C(DataDisplayFlags) impl) : impl(impl) { }
   operator C(DataDisplayFlags)() { return impl; }
   DataDisplayFlags & operator =(C(DataDisplayFlags) impl) { impl = impl; return *this; }
   bool operator ==(const DataDisplayFlags & value) const { return impl == value.impl; }
   bool operator !=(const DataDisplayFlags & value) const { return impl != value.impl; }

   struct selected_Prop
   {
      constexpr selected_Prop() :_() { };
      [[no_unique_address]] int _[0];
      inline bool operator= (bool v);
      inline DataDisplayFlags::selected_Prop & operator= (DataDisplayFlags::selected_Prop & prop);
      inline operator bool () const;
   } selected;
   struct fullRow_Prop
   {
      constexpr fullRow_Prop() :_() { };
      [[no_unique_address]] int _[0];
      inline bool operator= (bool v);
      inline DataDisplayFlags::fullRow_Prop & operator= (DataDisplayFlags::fullRow_Prop & prop);
      inline operator bool () const;
   } fullRow;
   struct current_Prop
   {
      constexpr current_Prop() :_() { };
      [[no_unique_address]] int _[0];
      inline bool operator= (bool v);
      inline DataDisplayFlags::current_Prop & operator= (DataDisplayFlags::current_Prop & prop);
      inline operator bool () const;
   } current;
   struct active_Prop
   {
      constexpr active_Prop() :_() { };
      [[no_unique_address]] int _[0];
      inline bool operator= (bool v);
      inline DataDisplayFlags::active_Prop & operator= (DataDisplayFlags::active_Prop & prop);
      inline operator bool () const;
   } active;
   struct dropBox_Prop
   {
      constexpr dropBox_Prop() :_() { };
      [[no_unique_address]] int _[0];
      inline bool operator= (bool v);
      inline DataDisplayFlags::dropBox_Prop & operator= (DataDisplayFlags::dropBox_Prop & prop);
      inline operator bool () const;
   } dropBox;
   struct header_Prop
   {
      constexpr header_Prop() :_() { };
      [[no_unique_address]] int _[0];
      inline bool operator= (bool v);
      inline DataDisplayFlags::header_Prop & operator= (DataDisplayFlags::header_Prop & prop);
      inline operator bool () const;
   } header;
   struct firstField_Prop
   {
      constexpr firstField_Prop() :_() { };
      [[no_unique_address]] int _[0];
      inline bool operator= (bool v);
      inline DataDisplayFlags::firstField_Prop & operator= (DataDisplayFlags::firstField_Prop & prop);
      inline operator bool () const;
   } firstField;
};

struct Struct
{
   [[no_unique_address]] int _[0];
   void * operator new   (uintsize count) { return eC_new(count); }
   void * operator new [](uintsize count) { return eC_new(count); }
   void operator delete   (void * ptr) { eC_delete(ptr); }
   void operator delete [](void * ptr) { eC_delete(ptr); }
};

template <typename TSC>
struct TStruct : public Struct
{
   static C(Class) * _class;
   struct Struct_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      inline int operator()( /*6Bj*/Instance & object);
   } onCompare;

   struct Struct_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Bj*/Instance & newData);
   } onCopy;

   struct Struct_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;

   struct Struct_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      inline Instance & operator()( /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;

   struct Struct_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( );
   } onFree;

   struct Struct_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      inline bool operator()( /*6Fj*/const char * string);
   } onGetDataFromString;

   struct Struct_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      inline const char * operator()( /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;

   struct Struct_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      inline bool operator()( /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;

   struct Struct_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;

   struct Struct_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;

};

template<typename TSC> C(Class) * TStruct<TSC>::_class;


#define Instance_onCompare_vTblID           class_onCompare_vTblID
#define Instance_onCopy_vTblID              class_onCopy_vTblID
#define Instance_onDisplay_vTblID           class_onDisplay_vTblID
#define Instance_onEdit_vTblID              class_onEdit_vTblID
#define Instance_onFree_vTblID              class_onFree_vTblID
#define Instance_onGetDataFromString_vTblID class_onGetDataFromString_vTblID
#define Instance_onGetString_vTblID         class_onGetString_vTblID
#define Instance_onSaveEdit_vTblID          class_onSaveEdit_vTblID
#define Instance_onSerialize_vTblID         class_onSerialize_vTblID
#define Instance_onUnserialize_vTblID       class_onUnserialize_vTblID

#define REG_Instance_onCompare(m, c)           REGVMETHOD(Instance, onCompare,           c::m, (XClass * _class, /*1Ab*/C(Instance) self, /*1Ab*/C(Instance) object),                                                                                                                                                c, (/*4Am*/*self_l, /*4Am*/*object_l))
#define REG_Instance_onCopy(m, c)              REGVMETHOD(Instance, onCopy,              c::m, (XClass * _class, /*1Ab*/C(Instance) * self, /*1Ab*/C(Instance) newData),                                                                                                                                             c, (/*4Am*/*self_l, /*4Am*/*newData_l))
#define REG_Instance_onDisplay(m, c)           REGVMETHOD(Instance, onDisplay,           c::m, (XClass * _class, /*1Ab*/C(Instance) self, /*1Ab*/Instance & surface, /*1Ab*/int x, /*1Ab*/int y, /*1Ab*/int width, /*1Ab*/void * fieldData, /*1Ab*/C(Alignment) alignment, /*1Ab*/C(DataDisplayFlags) displayFlags), c, (/*4Am*/*self_l, /*4Im*/surface, /*4Im*/x, /*4Im*/y, /*4Im*/width, /*4Im*/fieldData, /*4Hm*/(Alignment)alignment, /*4Im*/displayFlags))
#define REG_Instance_onEdit(m, c)              REGVMETHOD(Instance, onEdit,              c::m, (XClass * _class, /*1Ab*/C(Instance) self, /*1Ab*/Instance & dataBox, /*1Ab*/Instance & obsolete, /*1Ab*/int x, /*1Ab*/int y, /*1Ab*/int w, /*1Ab*/int h, /*1Ab*/void * userData),                                    c, (/*4Am*/*self_l, /*4Im*/dataBox, /*4Im*/obsolete, /*4Im*/x, /*4Im*/y, /*4Im*/w, /*4Im*/h, /*4Im*/userData))
#define REG_Instance_onFree(m, c)              REGVMETHOD(Instance, onFree,              c::m, (XClass * _class, /*1Ab*/C(Instance) self),                                                                                                                                                                           c, (/*4Am*/*self_l))
#define REG_Instance_onGetDataFromString(m, c) REGVMETHOD(Instance, onGetDataFromString, c::m, (XClass * _class, /*1Ab*/C(Instance) * self, /*1Ab*/const char * string),                                                                                                                                             c, (/*4Am*/*self_l, /*4Im*/string))
#define REG_Instance_onGetString(m, c)         REGVMETHOD(Instance, onGetString,         c::m, (XClass * _class, /*1Ab*/C(Instance) self, /*1Ab*/char * tempString, /*1Ab*/void * reserved, /*1Ab*/ObjectNotationType * onType),                                                                                     c, (/*4Am*/*self_l, /*4Im*/tempString, /*4Im*/reserved, /*4Hm*/(ObjectNotationType *)onType))
#define REG_Instance_onSaveEdit(m, c)          REGVMETHOD(Instance, onSaveEdit,          c::m, (XClass * _class, /*1Ab*/C(Instance) * self, /*1Ab*/Window & window, /*1Ab*/void * object),                                                                                                                           c, (/*4Am*/*self_l, /*4Im*/window, /*4Im*/object))
#define REG_Instance_onSerialize(m, c)         REGVMETHOD(Instance, onSerialize,         c::m, (XClass * _class, /*1Ab*/C(Instance) self, /*1Ab*/IOChannel & channel),                                                                                                                                               c, (/*4Am*/*self_l, /*4Im*/channel))
#define REG_Instance_onUnserialize(m, c)       REGVMETHOD(Instance, onUnserialize,       c::m, (XClass * _class, /*1Ab*/C(Instance) * self, /*1Ab*/IOChannel & channel),                                                                                                                                             c, (/*4Am*/*self_l, /*4Im*/channel))

#define REG_Instance(c) \
      Instance::class_registration(_cpp_class);

#define INSTANCE_VIRTUAL_METHODS_PROTO(c) \
   VIRTUAL_METHOD_PROTO(onCompare, onCompare, c, Instance, \
      int, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Bj*/c & object); \
   VIRTUAL_METHOD_PROTO(onCopy, onCopy, c, Instance, \
      void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Bj*/c & newData); \
   VIRTUAL_METHOD_PROTO(onDisplay, onDisplay, c, Instance, \
      void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*Surface*/ & surface _ARG /*6Fj*/int x _ARG /*6Fj*/int y _ARG /*6Fj*/int width _ARG /*6Fj*/void * fieldData _ARG /*6Fj*/Alignment alignment _ARG /*6Fj*/DataDisplayFlags displayFlags); \
   VIRTUAL_METHOD_PROTO(onEdit, onEdit, c, Instance, \
      Instance &, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*DataBox*/ & dataBox _ARG /*6Aj*/Instance /*DataBox*/ & obsolete _ARG /*6Fj*/int x _ARG /*6Fj*/int y _ARG /*6Fj*/int w _ARG /*6Fj*/int h _ARG /*6Fj*/void * userData); \
   VIRTUAL_METHOD_PROTO(onFree, onFree, c, Instance, \
      void, c &, /*6Bk*/c & o_, ); \
   VIRTUAL_METHOD_PROTO(onGetDataFromString, onGetDataFromString, c, Instance, \
      bool, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Fj*/const char * string); \
   VIRTUAL_METHOD_PROTO(onGetString, onGetString, c, Instance, \
      const char *, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Fj*/char * tempString _ARG /*6Fj*/void * reserved _ARG /*6Fj*/ObjectNotationType * onType); \
   VIRTUAL_METHOD_PROTO(onSaveEdit, onSaveEdit, c, Instance, \
      bool, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*Window*/ & window _ARG /*6Fj*/void * object); \
   VIRTUAL_METHOD_PROTO(onSerialize, onSerialize, c, Instance, \
      void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*IOChannel*/ & channel); \
   VIRTUAL_METHOD_PROTO(onUnserialize, onUnserialize, c, Instance, \
      void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*IOChannel*/ & channel);

#define INSTANCE_VIRTUAL_METHODS(c) \
VIRTUAL_METHOD(onCompare, onCompare, c, Instance, \
   int, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Bj*/c & object, \
   return Instance_onCompare(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/object.impl);); \
VIRTUAL_METHOD(onCopy, onCopy, c, Instance, \
   void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Bj*/c & newData, \
   Instance_onCopy(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/newData.impl);); \
VIRTUAL_METHOD(onDisplay, onDisplay, c, Instance, \
   void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*Surface*/ & surface _ARG /*6Fj*/int x _ARG /*6Fj*/int y _ARG /*6Fj*/int width _ARG /*6Fj*/void * fieldData _ARG /*6Fj*/Alignment alignment _ARG /*6Fj*/DataDisplayFlags displayFlags, \
   Instance_onDisplay(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/surface.impl, /*7Al*/x, /*7Al*/y, /*7Al*/width, /*7Al*/fieldData, /*7Al*/(C(Alignment))alignment, /*7Al*/displayFlags);); \
VIRTUAL_METHOD(onEdit, onEdit, c, Instance, \
   Instance &, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*DataBox*/ & dataBox _ARG /*6Aj*/Instance /*DataBox*/ & obsolete _ARG /*6Fj*/int x _ARG /*6Fj*/int y _ARG /*6Fj*/int w _ARG /*6Fj*/int h _ARG /*6Fj*/void * userData, \
   C(Instance) retInstance = Instance_onEdit(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/dataBox.impl, /*7Al*/obsolete.impl, /*7Al*/x, /*7Al*/y, /*7Al*/w, /*7Al*/h, /*7Al*/userData); \
      return *(Instance *)INSTANCEL(retInstance, retInstance->_class);); \
VIRTUAL_METHOD(onFree, onFree, c, Instance, \
   void, c &, /*6Bk*/c & o_, , \
   Instance_onFree(_cpp_class.impl, self ? self->impl : (C(Instance))null);); \
VIRTUAL_METHOD(onGetDataFromString, onGetDataFromString, c, Instance, \
   bool, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Fj*/const char * string, \
   return (bool)Instance_onGetDataFromString(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/string);); \
VIRTUAL_METHOD(onGetString, onGetString, c, Instance, \
   const char *, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Fj*/char * tempString _ARG /*6Fj*/void * reserved _ARG /*6Fj*/ObjectNotationType * onType, \
   return Instance_onGetString(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/tempString, /*7Al*/reserved, /*7Al*/(C(ObjectNotationType) *)onType);); \
VIRTUAL_METHOD(onSaveEdit, onSaveEdit, c, Instance, \
   bool, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*Window*/ & window _ARG /*6Fj*/void * object, \
   return (bool)Instance_onSaveEdit(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/window.impl, /*7Al*/object);); \
VIRTUAL_METHOD(onSerialize, onSerialize, c, Instance, \
   void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*IOChannel*/ & channel, \
   Instance_onSerialize(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/channel.impl);); \
VIRTUAL_METHOD(onUnserialize, onUnserialize, c, Instance, \
   void, c & _ARG, /*6Bk*/c & o_ _ARG, /*6Aj*/Instance /*IOChannel*/ & channel, \
   Instance_onUnserialize(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/channel.impl););

class Instance
{
public:
   // hardcoded content start
   static TCPPClass<Instance> _cpp_class;
   C(Instance) impl;
   void (**vTbl)(void);
   bool mustFree = 0;

   void * operator new   (uintsize count) { return eC_new(count); }
   void * operator new [](uintsize count) { return eC_new(count); }
   void operator delete   (void * ptr) { eC_delete(ptr); }
   void operator delete [](void * ptr) { eC_delete(ptr); }

   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         Instance * inst = new Instance(i, _cpp_class);
         if(inst)
            inst->mustFree = true;
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      Instance * inst = (Instance *)INSTANCEL(i, _cpp_class.impl);
      if(inst && inst->mustFree)
         delete inst;
   }
   static void class_registration(CPPClass & _class);

   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   explicit inline Instance(C(Instance) _impl, CPPClass & cl = _cpp_class)
   {
      XClass * c = cl.impl;
      impl = _impl;
      vTbl = cl.vTbl;
      if(impl)
      {
         if(c && BINDINGS_CLASS(impl) && !INSTANCEL(impl, c))
            INSTANCEL(impl, c) = this;
         impl->_refCount++;
      }
   }
   inline Instance()
   {
      impl = null;
      vTbl = null;
   }
   inline ~Instance()
   {
#ifdef _DEBUG
      bool isApp = false;
#endif
      if(impl && impl->_class)
      {
#ifdef _DEBUG
         isApp = Class_isDerived(impl->_class, eC_findClass(__thisModule, "Application"));
#endif
         int refCount = impl->_refCount;
         if(impl->_class->bindingsClass)
         {
            Instance ** i = (Instance **)&INSTANCEL(impl, impl->_class);
            if(i && *i == this && vTbl)
            {
               CPPClass * cl = (CPPClass *)impl->_class->bindingsClass;
               if(cl && vTbl != cl->vTbl)
                  eC_delete(vTbl);
            }
         }
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** i = (Instance **)&INSTANCEL(impl, impl->_class);
            if(i && *i == this)
               *i = null;
         }
#ifdef _DEBUG
         if(isApp)
         {
            // printf("Is app\n");
            // F(checkMemory)();
         }
#endif
      }
   }
   Instance(const Instance & i) = delete;
   Instance operator= (const Instance & i) = delete;

   inline Instance(Instance && i)
   {
      impl = i.impl;
      vTbl = i.vTbl;
      i.impl = null;
      i.vTbl = null;
   }

   inline Instance & operator= (Instance && i)
   {
      impl = i.impl;
      vTbl = i.vTbl;
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   // end of hardcoded content
};

template <class TC, C(Class) ** TCO>
class TNHInstance
{
public:
   void * operator new   (uintsize count) { return eC_new(count); }
   void * operator new [](uintsize count) { return eC_new(count); }
   void operator delete   (void * ptr) { eC_delete(ptr); }
   void operator delete [](void * ptr) { eC_delete(ptr); }
   struct NHInstance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      inline int operator()( /*6Bj*/Instance & object);
   } onCompare;

   struct NHInstance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Bj*/Instance & newData);
   } onCopy;

   struct NHInstance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;

   struct NHInstance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      inline Instance & operator()( /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;

   struct NHInstance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( );
   } onFree;

   struct NHInstance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      inline bool operator()( /*6Fj*/const char * string);
   } onGetDataFromString;

   struct NHInstance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      inline const char * operator()( /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;

   struct NHInstance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      inline bool operator()( /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;

   struct NHInstance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;

   struct NHInstance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      inline void operator()( /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;

   TC * impl;
   TNHInstance() { impl = (TC*)Instance_new(*TCO); }
   TNHInstance(TC * _impl) { impl = _impl; }
   ~TNHInstance()
   {
      eC_delete(impl);
   }
};

template<typename TTT> inline typename std::enable_if<std::is_base_of<Instance, TTT>::value, uint64>::type
  toTA(TTT & x) { C(DataValue) p = { }; p.p = x.impl; return p.ui64; }

template<typename TTT> inline typename std::enable_if<std::is_base_of<Instance, TTT>::value, uint64>::type
  toTA(TTT * x) { C(DataValue) p = { }; p.p = x ? x->impl : null; return p.ui64; }

template<typename TTT> inline typename std::enable_if<std::is_base_of<Struct, TTT>::value, uint64>::type
  toTA(TTT x) { C(DataValue) p = { }; p.p = (void *)&x; return p.ui64; }

template<typename TTT> inline typename std::enable_if<!std::is_base_of<Instance, TTT>::value && !std::is_base_of<Struct, TTT>::value, uint64>::type
  toTA(TTT x) { C(DataValue) p = { }; p.p = (void *)x; return p.ui64; }

template<> inline uint64 toTA(double x) { C(DataValue) p = { }; p.d = x; return p.ui64; }
template<> inline uint64 toTA(float x)  { C(DataValue) p = { }; p.f = x; return p.ui64; }
template<> inline uint64 toTA(int x)    { C(DataValue) p = { }; p.i = x; return p.ui64; }

template<typename TTT> inline typename
   std::enable_if<std::is_pointer<TTT>::value &&
      std::is_base_of<Instance, typename std::remove_pointer<TTT>::type>::value, TTT>::type
   fromTA(uint64 x)
   {
      C(DataValue) p = { };
      p.ui64 = x;
      C(Instance) i = (C(Instance))p.p;
      return BINDINGS_CLASS(i) ? (TTT)INSTANCEL(i, i->_class) : (TTT)0;
   }

template<typename TTT> inline typename std::enable_if<!std::is_pointer<TTT>::value &&
      std::is_base_of<Instance, TTT>::value, TTT>::type
   fromTA(uint64 x)
   {
      C(DataValue) p = { };
      p.ui64 = x;
      return TTT((C(Instance))p.p);
   }

template<typename TTT> inline typename std::enable_if<!std::is_pointer<TTT>::value &&
      std::is_base_of<Struct, TTT>::value, TTT>::type
   fromTA(uint64 x)
   {
      C(DataValue) p = { };
      p.ui64 = x;
      return *(TTT *)p.p;
   }

template<typename TTT> inline typename std::enable_if<std::is_pointer<TTT>::value &&
      std::is_base_of<Struct, typename std::remove_pointer<TTT>::type>::value, TTT>::type
   fromTA(uint64 x)
   {
      C(DataValue) p = { };
      p.ui64 = x;
      return (TTT *)p.p;
   }

template<typename TTT> inline
   typename std::enable_if<
      !std::is_base_of<Instance, typename std::remove_pointer<TTT>::type>::value &&
      !std::is_base_of<Struct,   typename std::remove_pointer<TTT>::type>::value,
      TTT>::type
   fromTA(uint64 x) { C(DataValue) p = { }; p.ui64 = x; return (TTT)p.p; }

template<> inline double fromTA<double>(uint64 x) { C(DataValue) p = { }; p.ui64 = x; return p.d; }
template<> inline float  fromTA<float>(uint64 x) { C(DataValue) p = { }; p.ui64 = x; return p.f; }
template<> inline int    fromTA<int>(uint64 x) { C(DataValue) p = { }; p.ui64 = x; return p.i; }


template <class TC, C(Class) ** TCO>
inline int TNHInstance<TC, TCO>::NHInstance_onCompare_Functor::operator()( /*6Bj*/Instance & object)
{
   return Instance_onCompare((*TCO), impl, /*7Al*/object.impl);
}

template <class TC, C(Class) ** TCO>
inline void TNHInstance<TC, TCO>::NHInstance_onCopy_Functor::operator()( /*6Bj*/Instance & newData)
{
   _onCopy((*TCO), impl, /*7Al*/newData.impl);
}

template <class TC, C(Class) ** TCO>
inline void TNHInstance<TC, TCO>::NHInstance_onDisplay_Functor::operator()( /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags)
{
   _onDisplay((*TCO), impl, /*7Al*/surface.impl, /*7Al*/x, /*7Al*/y, /*7Al*/width, /*7Al*/fieldData, /*7Al*/(C(Alignment))alignment, /*7Al*/displayFlags);
}

template <class TC, C(Class) ** TCO>
inline Instance & TNHInstance<TC, TCO>::NHInstance_onEdit_Functor::operator()( /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData)
{
   C(Instance) retInstance = Instance_onEdit((*TCO), impl, /*7Al*/dataBox.impl, /*7Al*/obsolete.impl, /*7Al*/x, /*7Al*/y, /*7Al*/w, /*7Al*/h, /*7Al*/userData);
         return *(Instance *)INSTANCEL(retInstance, retInstance->_class);
}

template <class TC, C(Class) ** TCO>
inline void TNHInstance<TC, TCO>::NHInstance_onFree_Functor::operator()( )
{
   _onFree((*TCO), impl);
}

template <class TC, C(Class) ** TCO>
inline bool TNHInstance<TC, TCO>::NHInstance_onGetDataFromString_Functor::operator()( /*6Fj*/const char * string)
{
   return (bool)Instance_onGetDataFromString((*TCO), impl, /*7Al*/string);
}

template <class TC, C(Class) ** TCO>
inline const char * TNHInstance<TC, TCO>::NHInstance_onGetString_Functor::operator()( /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType)
{
   return Instance_onGetString((*TCO), impl, /*7Al*/tempString, /*7Al*/reserved, /*7Al*/(C(ObjectNotationType) *)onType);
}

template <class TC, C(Class) ** TCO>
inline bool TNHInstance<TC, TCO>::NHInstance_onSaveEdit_Functor::operator()( /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object)
{
   return (bool)Instance_onSaveEdit((*TCO), impl, /*7Al*/window.impl, /*7Al*/object);
}

template <class TC, C(Class) ** TCO>
inline void TNHInstance<TC, TCO>::NHInstance_onSerialize_Functor::operator()( /*6Aj*/Instance /*IOChannel*/ & channel)
{
   _onSerialize((*TCO), impl, /*7Al*/channel.impl);
}

template <class TC, C(Class) ** TCO>
inline void TNHInstance<TC, TCO>::NHInstance_onUnserialize_Functor::operator()( /*6Aj*/Instance /*IOChannel*/ & channel)
{
   _onUnserialize((*TCO), impl, /*7Al*/channel.impl);
}


template <class TC, C(Class) ** TCO>
class TIteratorPointer : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;
   ~TIteratorPointer()
   {
      this->impl = null; // How to know not to delete?
   }
};

class Container
{
public:
   static TCPPClass<Container> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};

#define REG_Container_copy(m, c, TP_T, TP_I, TP_D)         TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, copy,         c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/Container & source),                               c, (/*4Im*/(c &)source))
#define REG_Container_delete(m, c, TP_T, TP_I, TP_D)       TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, delete,       c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/IteratorPointer it),                               c, (/*4Im*/it))
#define REG_Container_free(m, c, TP_T, TP_I, TP_D)         TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, free,         c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self),                                                          c, ())
#define REG_Container_freeIterator(m, c, TP_T, TP_I, TP_D) TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, freeIterator, c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/IteratorPointer it),                               c, (/*4Im*/it))
#define REG_Container_getCount(m, c, TP_T, TP_I, TP_D)     TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, getCount,     c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self),                                                          c, ())
#define REG_Container_getData(m, c, TP_T, TP_I, TP_D)      TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, getData,      c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/IteratorPointer pointer),                          c, (/*4Im*/pointer))
#define REG_Container_getFirst(m, c, TP_T, TP_I, TP_D)     TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, getFirst,     c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self),                                                          c, ())
#define REG_Container_getLast(m, c, TP_T, TP_I, TP_D)      TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, getLast,      c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self),                                                          c, ())
#define REG_Container_getNext(m, c, TP_T, TP_I, TP_D)      TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, getNext,      c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/IteratorPointer pointer),                          c, (/*4Im*/pointer))
#define REG_Container_getPrev(m, c, TP_T, TP_I, TP_D)      TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, getPrev,      c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/IteratorPointer pointer),                          c, (/*4Im*/pointer))
#define REG_Container_move(m, c, TP_T, TP_I, TP_D)         TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, move,         c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/IteratorPointer it, /*1Ab*/IteratorPointer after), c, (/*4Im*/it, /*4Im*/after))
#define REG_Container_remove(m, c, TP_T, TP_I, TP_D)       TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, remove,       c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/IteratorPointer it),                               c, (/*4Im*/it))
#define REG_Container_removeAll(m, c, TP_T, TP_I, TP_D)    TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, removeAll,    c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self),                                                          c, ())
#define REG_Container_sort(m, c, TP_T, TP_I, TP_D)         TREGVMETHOD(TContainer<TP_T, TP_I, TP_D>, Container, sort,         c::m, (/*1Ab*/TContainer<TP_T, TP_I, TP_D> & self, /*1Ab*/bool ascending),                                   c, (/*4Hm*/(bool)ascending))

#define TREG_Container(c, TP_T, TP_I, TP_D) \
      TContainer::class_registration(_cpp_class); \
      REG_Container_copy(copy, c, TP_T, TP_I, TP_D); \
      REG_Container_delete(delete, c, TP_T, TP_I, TP_D); \
      REG_Container_free(free, c, TP_T, TP_I, TP_D); \
      REG_Container_freeIterator(freeIterator, c, TP_T, TP_I, TP_D); \
      REG_Container_getCount(getCount, c, TP_T, TP_I, TP_D); \
      REG_Container_getData(getData, c, TP_T, TP_I, TP_D); \
      REG_Container_getFirst(getFirst, c, TP_T, TP_I, TP_D); \
      REG_Container_getLast(getLast, c, TP_T, TP_I, TP_D); \
      REG_Container_getNext(getNext, c, TP_T, TP_I, TP_D); \
      REG_Container_getPrev(getPrev, c, TP_T, TP_I, TP_D); \
      REG_Container_move(move, c, TP_T, TP_I, TP_D); \
      REG_Container_remove(remove, c, TP_T, TP_I, TP_D); \
      REG_Container_removeAll(removeAll, c, TP_T, TP_I, TP_D); \
      REG_Container_sort(sort, c, TP_T, TP_I, TP_D);

#define CONTAINER_VIRTUAL_METHODS_PROTO(c) \
   TVIRTUAL_METHOD_PROTO(add, add, Container, TContainer, \
      IteratorPointer, c & _ARG, , /*6Ej*/TP_T value); \
   TVIRTUAL_METHOD_PROTO(copy, copy, Container, TContainer, \
      void, c & _ARG, , /*6Fj*/TContainer & source); \
   TVIRTUAL_METHOD_PROTO(delete, delete_, Container, TContainer, \
      void, c & _ARG, , /*6Fj*/IteratorPointer it); \
   TVIRTUAL_METHOD_PROTO(find, find, Container, TContainer, \
      IteratorPointer, c & _ARG, , /*6Ej*/TP_D value); \
   TVIRTUAL_METHOD_PROTO(free, free, Container, TContainer, \
      void, c &, , ); \
   TVIRTUAL_METHOD_PROTO(freeIterator, freeIterator, Container, TContainer, \
      void, c & _ARG, , /*6Fj*/IteratorPointer it); \
   TVIRTUAL_METHOD_PROTO(getAtPosition, getAtPosition, Container, TContainer, \
      IteratorPointer, c & _ARG, , /*6Ej*/TP_I pos _ARG /*6Fj*/bool create _ARG /*6Cj*/C(bool) * justAdded); \
   TVIRTUAL_METHOD_PROTO(getCount, getCount, Container, TContainer, \
      int, c &, , ); \
   TVIRTUAL_METHOD_PROTO(getData, getData, Container, TContainer, \
      TP_D, c & _ARG, , /*6Fj*/IteratorPointer pointer); \
   TVIRTUAL_METHOD_PROTO(getFirst, getFirst, Container, TContainer, \
      IteratorPointer, c &, , ); \
   TVIRTUAL_METHOD_PROTO(getLast, getLast, Container, TContainer, \
      IteratorPointer, c &, , ); \
   TVIRTUAL_METHOD_PROTO(getNext, getNext, Container, TContainer, \
      IteratorPointer, c & _ARG, , /*6Fj*/IteratorPointer pointer); \
   TVIRTUAL_METHOD_PROTO(getPrev, getPrev, Container, TContainer, \
      IteratorPointer, c & _ARG, , /*6Fj*/IteratorPointer pointer); \
   TVIRTUAL_METHOD_PROTO(insert, insert, Container, TContainer, \
      IteratorPointer, c & _ARG, , /*6Fj*/IteratorPointer after _ARG /*6Ej*/TP_T value); \
   TVIRTUAL_METHOD_PROTO(move, move, Container, TContainer, \
      void, c & _ARG, , /*6Fj*/IteratorPointer it _ARG /*6Fj*/IteratorPointer after); \
   TVIRTUAL_METHOD_PROTO(remove, remove, Container, TContainer, \
      void, c & _ARG, , /*6Fj*/IteratorPointer it); \
   TVIRTUAL_METHOD_PROTO(removeAll, removeAll, Container, TContainer, \
      void, c &, , ); \
   TVIRTUAL_METHOD_PROTO(setData, setData, Container, TContainer, \
      bool, c & _ARG, , /*6Fj*/IteratorPointer pointer _ARG /*6Ej*/TP_D data); \
   TVIRTUAL_METHOD_PROTO(sort, sort, Container, TContainer, \
      void, c & _ARG, , /*6Fj*/bool ascending);

#define CONTAINER_VIRTUAL_METHODS(c) \
TVIRTUAL_METHOD(add, add, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c & _ARG, , /*6Ej*/TP_T value, \
   C(IteratorPointer) * retIteratorPointer = Container_add(self ? self->impl : (C(Container))null, /*7Al*/toTA<TP_T>(value)); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(copy, copy, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c & _ARG, , /*6Fj*/TContainer & source, \
   Container_copy(self ? self->impl : (C(Container))null, /*7Al*/((Instance&)source).impl);); \
TVIRTUAL_METHOD(delete, delete_, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c & _ARG, , /*6Fj*/IteratorPointer it, \
   Container_delete(self ? self->impl : (C(Container))null, /*7Al*/it.impl);); \
TVIRTUAL_METHOD(find, find, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c & _ARG, , /*6Ej*/TP_D value, \
   C(IteratorPointer) * retIteratorPointer = Container_find(self ? self->impl : (C(Container))null, /*7Al*/toTA<TP_D>(value)); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(free, free, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c &, , , \
   Container_free(self ? self->impl : (C(Container))null);); \
TVIRTUAL_METHOD(freeIterator, freeIterator, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c & _ARG, , /*6Fj*/IteratorPointer it, \
   Container_freeIterator(self ? self->impl : (C(Container))null, /*7Al*/it.impl);); \
TVIRTUAL_METHOD(getAtPosition, getAtPosition, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c & _ARG, , /*6Ej*/TP_I pos _ARG /*6Fj*/bool create _ARG /*6Cj*/C(bool) * justAdded, \
   C(IteratorPointer) * retIteratorPointer = Container_getAtPosition(self ? self->impl : (C(Container))null, /*7Al*/toTA<TP_I>(pos), /*7Al*/(C(bool))create, /*7Al*/justAdded); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(getCount, getCount, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   int, c &, , , \
   return Container_getCount(self ? self->impl : (C(Container))null);); \
TVIRTUAL_METHOD(getData, getData, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   TP_D, c & _ARG, , /*6Fj*/IteratorPointer pointer, \
   return fromTA<TP_D>(Container_getData(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl));); \
TVIRTUAL_METHOD(getFirst, getFirst, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c &, , , \
   C(IteratorPointer) * retIteratorPointer = Container_getFirst(self ? self->impl : (C(Container))null); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(getLast, getLast, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c &, , , \
   C(IteratorPointer) * retIteratorPointer = Container_getLast(self ? self->impl : (C(Container))null); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(getNext, getNext, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c & _ARG, , /*6Fj*/IteratorPointer pointer, \
   C(IteratorPointer) * retIteratorPointer = Container_getNext(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(getPrev, getPrev, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c & _ARG, , /*6Fj*/IteratorPointer pointer, \
   C(IteratorPointer) * retIteratorPointer = Container_getPrev(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(insert, insert, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   IteratorPointer, c & _ARG, , /*6Fj*/IteratorPointer after _ARG /*6Ej*/TP_T value, \
   C(IteratorPointer) * retIteratorPointer = Container_insert(self ? self->impl : (C(Container))null, /*7Al*/after.impl, /*7Al*/toTA<TP_T>(value)); \
      return retIteratorPointer;); \
TVIRTUAL_METHOD(move, move, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c & _ARG, , /*6Fj*/IteratorPointer it _ARG /*6Fj*/IteratorPointer after, \
   Container_move(self ? self->impl : (C(Container))null, /*7Al*/it.impl, /*7Al*/after.impl);); \
TVIRTUAL_METHOD(remove, remove, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c & _ARG, , /*6Fj*/IteratorPointer it, \
   Container_remove(self ? self->impl : (C(Container))null, /*7Al*/it.impl);); \
TVIRTUAL_METHOD(removeAll, removeAll, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c &, , , \
   Container_removeAll(self ? self->impl : (C(Container))null);); \
TVIRTUAL_METHOD(setData, setData, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   bool, c & _ARG, , /*6Fj*/IteratorPointer pointer _ARG /*6Ej*/TP_D data, \
   return (bool)Container_setData(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl, /*7Al*/toTA<TP_D>(data));); \
TVIRTUAL_METHOD(sort, sort, Container, <TP_T _ARG TP_I _ARG TP_D>, template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>, Container, \
   void, c & _ARG, , /*6Fj*/bool ascending, \
   Container_sort(self ? self->impl : (C(Container))null, /*7Al*/(C(bool))ascending););

template <typename TP_T, typename TP_I = int, typename TP_D = TP_T>
class TContainer : public Instance, public Container
{
public:
   inline TContainer/*<TP_T, TP_I, TP_D>*/(TContainer<TP_T, TP_I, TP_D> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline TContainer<TP_T, TP_I, TP_D> & operator= (TContainer<TP_T, TP_I, TP_D> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   TContainer() : TContainer((C(Instance))Instance_newEx(ensureTemplatized <TP_T, TP_I, TP_D>(_cpp_class, "Container").impl, false), ensureTemplatized <TP_T, TP_I, TP_D>(_cpp_class, "Container")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(TContainer)
   static TCPPClass<TContainer> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         TContainer <TP_T, TP_I, TP_D> * inst = new TContainer <TP_T, TP_I, TP_D>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      TContainer <TP_T, TP_I, TP_D>* inst = (TContainer <TP_T, TP_I, TP_D>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(TContainer <TP_T, TP_I, TP_D> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline TContainer /*<TP_T, TP_I, TP_D>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl)
 { }

   TContainer(std::initializer_list<TP_D> list) : TContainer()
   {
      typename std::initializer_list<TP_D>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         add(*it);
   }

   TContainer & operator =(std::initializer_list<TP_D> list)
   {
      typename std::initializer_list<TP_D>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         add(*it);
      return *this;
   }

   inline TP_D operator [](TP_I index)
   {
      IteratorPointer ptr = getAtPosition(index, false, null);
      return getData(ptr);
   }

   inline C(bool) takeOut(TP_D d);


   struct Container_add_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer & , /*6Ej*/TP_T value);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( /*6Ej*/TP_T value);
   } add;
   // inline static void register_add(CPPClass & cl, Container::Container_add_Functor::FunctionType func)

   struct Container_copy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer & , /*6Fj*/TContainer & source);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/TContainer & source);
   } copy;
   // inline static void register_copy(CPPClass & cl, Container::Container_copy_Functor::FunctionType func)

   struct Container_delete_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer it);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/IteratorPointer it);
   } delete_;
   // inline static void register_delete(CPPClass & cl, Container::Container_delete_Functor::FunctionType func)

   struct Container_find_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer & , /*6Ej*/TP_D value);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( /*6Ej*/TP_D value);
   } find;
   // inline static void register_find(CPPClass & cl, Container::Container_find_Functor::FunctionType func)

   struct Container_free_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( );
   } free;
   // inline static void register_free(CPPClass & cl, Container::Container_free_Functor::FunctionType func)

   struct Container_freeIterator_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer it);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/IteratorPointer it);
   } freeIterator;
   // inline static void register_freeIterator(CPPClass & cl, Container::Container_freeIterator_Functor::FunctionType func)

   struct Container_getAtPosition_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer & , /*6Ej*/TP_I pos, /*6Fj*/bool create, /*6Cj*/C(bool) * justAdded);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( /*6Ej*/TP_I pos, /*6Fj*/bool create, /*6Cj*/C(bool) * justAdded);
   } getAtPosition;
   // inline static void register_getAtPosition(CPPClass & cl, Container::Container_getAtPosition_Functor::FunctionType func)

   struct Container_getCount_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(TContainer &);
      inline FunctionType operator= (FunctionType func);
      inline int operator()( );
   } getCount;
   // inline static void register_getCount(CPPClass & cl, Container::Container_getCount_Functor::FunctionType func)

   struct Container_getData_Functor
   {
      [[no_unique_address]] int _[0];
      typedef TP_D (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer pointer);
      inline FunctionType operator= (FunctionType func);
      inline TP_D operator()( /*6Fj*/IteratorPointer pointer);
   } getData;
   // inline static void register_getData(CPPClass & cl, Container::Container_getData_Functor::FunctionType func)

   struct Container_getFirst_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer &);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( );
   } getFirst;
   // inline static void register_getFirst(CPPClass & cl, Container::Container_getFirst_Functor::FunctionType func)

   struct Container_getLast_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer &);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( );
   } getLast;
   // inline static void register_getLast(CPPClass & cl, Container::Container_getLast_Functor::FunctionType func)

   struct Container_getNext_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer pointer);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( /*6Fj*/IteratorPointer pointer);
   } getNext;
   // inline static void register_getNext(CPPClass & cl, Container::Container_getNext_Functor::FunctionType func)

   struct Container_getPrev_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer pointer);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( /*6Fj*/IteratorPointer pointer);
   } getPrev;
   // inline static void register_getPrev(CPPClass & cl, Container::Container_getPrev_Functor::FunctionType func)

   struct Container_insert_Functor
   {
      [[no_unique_address]] int _[0];
      typedef IteratorPointer (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer after, /*6Ej*/TP_T value);
      inline FunctionType operator= (FunctionType func);
      inline IteratorPointer operator()( /*6Fj*/IteratorPointer after, /*6Ej*/TP_T value);
   } insert;
   // inline static void register_insert(CPPClass & cl, Container::Container_insert_Functor::FunctionType func)

   struct Container_move_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer it, /*6Fj*/IteratorPointer after);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/IteratorPointer it, /*6Fj*/IteratorPointer after);
   } move;
   // inline static void register_move(CPPClass & cl, Container::Container_move_Functor::FunctionType func)

   struct Container_remove_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer it);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/IteratorPointer it);
   } remove;
   // inline static void register_remove(CPPClass & cl, Container::Container_remove_Functor::FunctionType func)

   struct Container_removeAll_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( );
   } removeAll;
   // inline static void register_removeAll(CPPClass & cl, Container::Container_removeAll_Functor::FunctionType func)

   struct Container_setData_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(TContainer & , /*6Fj*/IteratorPointer pointer, /*6Ej*/TP_D data);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()( /*6Fj*/IteratorPointer pointer, /*6Ej*/TP_D data);
   } setData;
   // inline static void register_setData(CPPClass & cl, Container::Container_setData_Functor::FunctionType func)

   struct Container_sort_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(TContainer & , /*6Fj*/bool ascending);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( /*6Fj*/bool ascending);
   } sort;
   // inline static void register_sort(CPPClass & cl, Container::Container_sort_Functor::FunctionType func)

   static void class_registration(CPPClass & _cpp_class);

   struct copySrc_Prop
   {
      constexpr copySrc_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0A*/const TContainer<TP_T, TP_I, TP_D> & operator= (/*0A*/const TContainer<TP_T, TP_I, TP_D> & v);
      /*nstSet*/inline /*0C*/const TContainer<TP_T, TP_I, TP_D> * operator= (/*0C*/const TContainer<TP_T, TP_I, TP_D> * v);
   } copySrc;
   struct firstIterator_Prop
   {
      constexpr firstIterator_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/TIterator<TP_T, TP_D> () const;
   } firstIterator;
   struct lastIterator_Prop
   {
      constexpr lastIterator_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/TIterator<TP_T, TP_D> () const;
   } lastIterator;
};

class CustomAVLTree
{
public:
   static TCPPClass<CustomAVLTree> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_CustomAVLTree(c, TP_BT, TP_KT) \
      TCustomAVLTree::class_registration(_cpp_class);

#define CUSTOMAVLTREE_VIRTUAL_METHODS_PROTO(c) \

#define CUSTOMAVLTREE_VIRTUAL_METHODS(c) \

template <typename TP_BT, typename TP_KT = uint64>
class TCustomAVLTree : public TContainer<TP_BT, TP_KT>, public CustomAVLTree
{
public:
   inline TCustomAVLTree/*<TP_BT, TP_KT>*/(TCustomAVLTree<TP_BT, TP_KT> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline TCustomAVLTree<TP_BT, TP_KT> & operator= (TCustomAVLTree<TP_BT, TP_KT> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   TCustomAVLTree() : TCustomAVLTree((C(Instance))Instance_newEx(ensureTemplatized <TP_BT, TP_KT>(_cpp_class, "CustomAVLTree").impl, false), ensureTemplatized <TP_BT, TP_KT>(_cpp_class, "CustomAVLTree")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(TCustomAVLTree)
   static TCPPClass<TCustomAVLTree> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         TCustomAVLTree <TP_BT, TP_KT> * inst = new TCustomAVLTree <TP_BT, TP_KT>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      TCustomAVLTree <TP_BT, TP_KT>* inst = (TCustomAVLTree <TP_BT, TP_KT>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(TCustomAVLTree <TP_BT, TP_KT> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline TCustomAVLTree /*<TP_BT, TP_KT>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TContainer<TP_BT, TP_KT>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   TCustomAVLTree (std::initializer_list<TP_BT> list) : TCustomAVLTree<TP_BT, TP_KT> ()
   {
      typename std::initializer_list<TP_BT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   TCustomAVLTree<TP_BT, TP_KT> & operator =(std::initializer_list<TP_BT> list)
   {
      typename std::initializer_list<TP_BT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }

   struct root_Prop
   {
      constexpr root_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TP_BT operator= (/*0H*/TP_BT v);
      /*regSet*/inline TCustomAVLTree<TP_BT _ARG TP_KT>::root_Prop & operator= (TCustomAVLTree<TP_BT _ARG TP_KT>::root_Prop & prop);
      /*regGet*/inline operator /*0I*/TP_BT () const;
   } root;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline TCustomAVLTree<TP_BT _ARG TP_KT>::count_Prop & operator= (TCustomAVLTree<TP_BT _ARG TP_KT>::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
};

#define REG_IOChannel_readData(m, c)  REGVMETHOD(IOChannel, readData,  c::m, (/*1Ab*/IOChannel & self, /*1Ab*/void * data, /*1Ab*/uintsize numBytes),       c, (/*4Im*/data, /*4Im*/numBytes))
#define REG_IOChannel_writeData(m, c) REGVMETHOD(IOChannel, writeData, c::m, (/*1Ab*/IOChannel & self, /*1Ab*/const void * data, /*1Ab*/uintsize numBytes), c, (/*4Im*/data, /*4Im*/numBytes))

#define REG_IOChannel(c) \
      IOChannel::class_registration(_cpp_class); \
      REG_IOChannel_readData(readData, c); \
      REG_IOChannel_writeData(writeData, c);

#define IOCHANNEL_VIRTUAL_METHODS_PROTO(c) \
   VIRTUAL_METHOD_PROTO(readData, readData, c, IOChannel, \
      uintsize, c & _ARG, , /*6Fj*/void * data _ARG /*6Fj*/uintsize numBytes); \
   VIRTUAL_METHOD_PROTO(writeData, writeData, c, IOChannel, \
      uintsize, c & _ARG, , /*6Fj*/const void * data _ARG /*6Fj*/uintsize numBytes);

#define IOCHANNEL_VIRTUAL_METHODS(c) \
VIRTUAL_METHOD(readData, readData, c, IOChannel, \
   uintsize, c & _ARG, , /*6Fj*/void * data _ARG /*6Fj*/uintsize numBytes, \
   return IOChannel_readData(self ? self->impl : (C(IOChannel))null, /*7Al*/data, /*7Al*/numBytes);); \
VIRTUAL_METHOD(writeData, writeData, c, IOChannel, \
   uintsize, c & _ARG, , /*6Fj*/const void * data _ARG /*6Fj*/uintsize numBytes, \
   return IOChannel_writeData(self ? self->impl : (C(IOChannel))null, /*7Al*/data, /*7Al*/numBytes););

class IOChannel : public Instance
{
public:
   inline IOChannel(IOChannel && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline IOChannel & operator= (IOChannel && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   IOChannel() : IOChannel((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<IOChannel> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         IOChannel * inst = new IOChannel(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      IOChannel * inst = (IOChannel *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(IOChannel & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline IOChannel(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   inline void get(/*1Bb*/typed_object_class_ptr class_data, void * data); // IOChannel_get
   inline void put(/*1Bb*/typed_object_class_ptr class_data, void * data); // IOChannel_put
   inline void serialize(/*1Bb*/typed_object_class_ptr class_data, void * data); // IOChannel_serialize
   inline void unserialize(/*1Bb*/typed_object_class_ptr class_data, void * data); // IOChannel_unserialize

   struct IOChannel_readData_Functor
   {
      [[no_unique_address]] int _[0];
      typedef uintsize (* FunctionType)(IOChannel & , /*6Fj*/void * data, /*6Fj*/uintsize numBytes);
      inline FunctionType operator= (FunctionType func);
      inline uintsize operator()( /*6Fj*/void * data, /*6Fj*/uintsize numBytes);
   } readData;
   // inline static void register_readData(CPPClass & cl, IOChannel::IOChannel_readData_Functor::FunctionType func)

   struct IOChannel_writeData_Functor
   {
      [[no_unique_address]] int _[0];
      typedef uintsize (* FunctionType)(IOChannel & , /*6Fj*/const void * data, /*6Fj*/uintsize numBytes);
      inline FunctionType operator= (FunctionType func);
      inline uintsize operator()( /*6Fj*/const void * data, /*6Fj*/uintsize numBytes);
   } writeData;
   // inline static void register_writeData(CPPClass & cl, IOChannel::IOChannel_writeData_Functor::FunctionType func)

   static void class_registration(CPPClass & _cpp_class);
};

struct Iterator : Struct
{
public:
   static TCPPClass<Iterator> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};

template <typename TP_T, typename TP_IT = int>
class TIterator : Iterator
{
public:
   C(Iterator) impl;
   constexpr TIterator() : impl({}) { }
   constexpr TIterator(const C(Iterator) impl) : impl(impl) { }

   inline void free(); // Iterator_free
   inline TP_T getData(); // Iterator_getData
   inline C(bool) next(); // Iterator_next
   inline C(bool) prev(); // Iterator_prev
   inline void remove(); // Iterator_remove

   // FIXME: index() not generated?
   inline C(bool) index(TP_IT index, C(bool) create); // Iterator_index

   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0A*/const TP_T & operator= (/*0A*/const TP_T & v);
      /*regSet*/inline TIterator<TP_T _ARG TP_IT>::data_Prop & operator= (TIterator<TP_T _ARG TP_IT>::data_Prop & prop);
      /*nstSet*/inline /*0C*/const TP_T * operator= (/*0C*/const TP_T * v);
      /*regGet*/inline operator /*0B*/TIH<TP_T> () const;
      /*regGet*/inline TIH<TP_T> operator /*0D*/-> () const;
      /*regGet*/inline operator /*0E*/TP_T () const;
      /*regGet*/inline operator /*0F*/TP_T* () const;
   } data;

   struct container_Prop
   {
      constexpr container_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TContainer<TP_T, TP_IT> operator= (/*0H*/TContainer<TP_T, TP_IT> v);
      /*regSet*/inline TIterator<TP_T _ARG TP_IT>::container_Prop & operator= (TIterator<TP_T _ARG TP_IT>::container_Prop & prop);
      /*regGet*/inline operator /*0I*/TContainer<TP_T, TP_IT> () const;
   } container;
   struct pointer_Prop
   {
      constexpr pointer_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/IteratorPointer operator= (/*0H*/IteratorPointer & v);
      /*regSet*/inline TIterator<TP_T _ARG TP_IT>::pointer_Prop & operator= (TIterator<TP_T _ARG TP_IT>::pointer_Prop & prop);
      /*regGet*/inline operator /*0I*/IteratorPointer () const;
   } pointer;
};

class LinkList
{
public:
   static TCPPClass<LinkList> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_LinkList(c, TP_LT) \
      TLinkList::class_registration(_cpp_class);

#define LINKLIST_VIRTUAL_METHODS_PROTO(c) \

#define LINKLIST_VIRTUAL_METHODS(c) \

template <typename TP_LT = ListItem>
class TLinkList : public TContainer<TP_LT>, public LinkList
{
public:
   inline TLinkList/*<TP_LT>*/(TLinkList<TP_LT> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline TLinkList<TP_LT> & operator= (TLinkList<TP_LT> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   TLinkList() : TLinkList((C(Instance))Instance_newEx(ensureTemplatized <TP_LT>(_cpp_class, "LinkList").impl, false), ensureTemplatized <TP_LT>(_cpp_class, "LinkList")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(TLinkList)
   static TCPPClass<TLinkList> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         TLinkList <TP_LT> * inst = new TLinkList <TP_LT>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      TLinkList <TP_LT>* inst = (TLinkList <TP_LT>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(TLinkList <TP_LT> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline TLinkList /*<TP_LT>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TContainer<TP_LT>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   TLinkList (std::initializer_list<TP_LT> list) : TLinkList<TP_LT> ()
   {
      typename std::initializer_list<TP_LT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   TLinkList<TP_LT> & operator =(std::initializer_list<TP_LT> list)
   {
      typename std::initializer_list<TP_LT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }

   struct first_Prop
   {
      constexpr first_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TP_LT operator= (/*0H*/TP_LT v);
      /*regSet*/inline TLinkList<TP_LT>::first_Prop & operator= (TLinkList<TP_LT>::first_Prop & prop);
      /*regGet*/inline operator /*0I*/TP_LT () const;
   } first;
   struct last_Prop
   {
      constexpr last_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TP_LT operator= (/*0H*/TP_LT v);
      /*regSet*/inline TLinkList<TP_LT>::last_Prop & operator= (TLinkList<TP_LT>::last_Prop & prop);
      /*regGet*/inline operator /*0I*/TP_LT () const;
   } last;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline TLinkList<TP_LT>::count_Prop & operator= (TLinkList<TP_LT>::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
};

template <class TC, C(Class) ** TCO>
class TListItem : public TIteratorPointer<TC, TCO>
{
public:
   using TIteratorPointer<TC, TCO>::TIteratorPointer;
   inline operator IteratorPointer& () const { return *(IteratorPointer *)this; }

   struct link_Prop
   {
      constexpr link_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0Z*/const LinkElement & operator= (/*0Z*/const LinkElement & v);
      /*regSet*/inline typename TListItem<TC, TCO>::link_Prop & operator= (typename TListItem<TC, TCO>::link_Prop & prop);
      /*regGet*/inline operator /*0I*/LinkElement & () const;
   } link;
   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/C(Class) * operator= (/*0H*/C(Class) * v);
      /*regSet*/inline typename TListItem<TC, TCO>::prev_Prop & operator= (typename TListItem<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/C(Class) * () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/C(Class) * operator= (/*0H*/C(Class) * v);
      /*regSet*/inline typename TListItem<TC, TCO>::next_Prop & operator= (typename TListItem<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/C(Class) * () const;
   } next;
   ~TListItem()
   {
      this->impl = null; // How to know not to delete?
   }
};

#define REG_Module_onLoad(m, c)   REGVMETHOD(Module, onLoad,   c::m, (/*1Ab*/Module & self), c, ())
#define REG_Module_onUnload(m, c) REGVMETHOD(Module, onUnload, c::m, (/*1Ab*/Module & self), c, ())

#define REG_Module(c) \
      Module::class_registration(_cpp_class); \
      REG_Module_onLoad(onLoad, c); \
      REG_Module_onUnload(onUnload, c);

#define MODULE_VIRTUAL_METHODS_PROTO(c) \
   VIRTUAL_METHOD_PROTO(onLoad, onLoad, c, Module, \
      bool, c &, , ); \
   VIRTUAL_METHOD_PROTO(onUnload, onUnload, c, Module, \
      void, c &, , );

#define MODULE_VIRTUAL_METHODS(c) \
VIRTUAL_METHOD(onLoad, onLoad, c, Module, \
   bool, c &, , , \
   return (bool)Module_onLoad(self ? self->impl : (C(Module))null);); \
VIRTUAL_METHOD(onUnload, onUnload, c, Module, \
   void, c &, , , \
   Module_onUnload(self ? self->impl : (C(Module))null););

class Module : public Instance
{
public:
   inline Module(Module && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline Module & operator= (Module && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   Module() : Module((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<Module> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         Module * inst = new Module(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      Module * inst = (Module *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(Module & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline Module(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   struct Module_onLoad_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Module &);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()( );
   } onLoad;
   // inline static void register_onLoad(CPPClass & cl, Module::Module_onLoad_Functor::FunctionType func)

   struct Module_onUnload_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Module &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( );
   } onUnload;
   // inline static void register_onUnload(CPPClass & cl, Module::Module_onUnload_Functor::FunctionType func)

};

class AVLTree
{
public:
   static TCPPClass<AVLTree> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_AVLTree(c, TP_AT) \
      TAVLTree::class_registration(_cpp_class);

#define AVLTREE_VIRTUAL_METHODS_PROTO(c) \

#define AVLTREE_VIRTUAL_METHODS(c) \

template <typename TP_AT>
class TAVLTree : public TCustomAVLTree<TP_AT>, public AVLTree
{
public:
   inline TAVLTree/*<TP_AT>*/(TAVLTree<TP_AT> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline TAVLTree<TP_AT> & operator= (TAVLTree<TP_AT> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   TAVLTree() : TAVLTree((C(Instance))Instance_newEx(ensureTemplatized <TP_AT>(_cpp_class, "AVLTree").impl, false), ensureTemplatized <TP_AT>(_cpp_class, "AVLTree")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(TAVLTree)
   static TCPPClass<TAVLTree> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         TAVLTree <TP_AT> * inst = new TAVLTree <TP_AT>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      TAVLTree <TP_AT>* inst = (TAVLTree <TP_AT>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(TAVLTree <TP_AT> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline TAVLTree /*<TP_AT>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TCustomAVLTree<TP_AT>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   TAVLTree (std::initializer_list<TP_AT> list) : TAVLTree<TP_AT> ()
   {
      typename std::initializer_list<TP_AT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   TAVLTree<TP_AT> & operator =(std::initializer_list<TP_AT> list)
   {
      typename std::initializer_list<TP_AT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }
};

enum class Access_Mode : int
{
   defaultAccess = AccessMode_defaultAccess,
   publicAccess = AccessMode_publicAccess,
   privateAccess = AccessMode_privateAccess,
   staticAccess = AccessMode_staticAccess,
   baseSystemAccess = AccessMode_baseSystemAccess
};

class Angle
{
public:
   C(Angle) impl;
   constexpr Angle() : impl(0) { }
   constexpr Angle(C(Angle) value) : impl(value) { }
   operator C(Angle)() const { return impl; }
   constexpr Angle(const Angle & value) : impl(value.impl) { }
   Angle & operator =(double value) { impl = value; return *this; }
   Angle & operator +=(Angle value) { impl += value.impl; return *this; }
   Angle & operator -=(Angle value) { impl -= value.impl; return *this; }
   Angle operator +(const Angle & b) const { return Angle(impl + b.impl); }
   Angle operator -(const Angle & b) const { return Angle(impl - b.impl); }
   bool operator ==(const Angle & value) const { return impl == value.impl; }
   bool operator !=(const Angle & value) const { return impl != value.impl; }
   bool operator <(const Angle & value) const { return impl < value.impl; }
   bool operator >(const Angle & value) const { return impl > value.impl; }
   bool operator <=(const Angle & value) const { return impl <= value.impl; }
   bool operator >=(const Angle & value) const { return impl >= value.impl; }
   bool operator ==(double value) const { return impl == Angle(value).impl; }
   bool operator !=(double value) const { return impl != Angle(value).impl; }
   bool operator <(double value) const { return impl < Angle(value).impl; }
   bool operator >(double value) const { return impl > Angle(value).impl; }
   bool operator <=(double value) const { return impl <= Angle(value).impl; }
   bool operator >=(double value) const { return impl >= Angle(value).impl; }
   bool operator ==(int value) const { return impl == Angle(value).impl; }
   bool operator !=(int value) const { return impl != Angle(value).impl; }
   bool operator <(int value) const { return impl < Angle(value).impl; }
   bool operator >(int value) const { return impl > Angle(value).impl; }
   bool operator <=(int value) const { return impl <= Angle(value).impl; }
   bool operator >=(int value) const { return impl >= Angle(value).impl; }
};

#define REG_Application_main(m, c) REGVMETHOD(Application, main, c::m, (/*1Ab*/Application & self), c, ())

#define REG_Application(c) \
      Application::class_registration(_cpp_class); \
      REG_Module_onLoad(onLoad, c); \
      REG_Module_onUnload(onUnload, c); \
      REG_Application_main(main, c);

#define APPLICATION_VIRTUAL_METHODS_PROTO(c) \
   VIRTUAL_METHOD_PROTO(main, main, c, Application, \
      void, c &, , );

#define APPLICATION_VIRTUAL_METHODS(c) \
VIRTUAL_METHOD(main, main, c, Application, \
   void, c &, , , \
   Application_main(self ? self->impl : (C(Application))null););

int ecere_cpp_init(const Module & module);


class Application : public Module
{
public:
   inline Application(Application && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline Application & operator= (Application && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   inline Application() : Application(eC_init_CALL(__LINK_ECERE__)) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<Application> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         Application * inst = new Application(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      Application * inst = (Application *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(Application & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline Application(C(Instance) _impl, CPPClass & cl = _cpp_class) : Module(_impl, cl)
   {
      eC_cpp_init(*this);
#if !defined(ECERECOM_ONLY)
      ecere_init(impl);
      ecere_cpp_init(*this);
#endif
      INSTANCEL(impl, impl->_class) = this;
      incref(impl);
      vTbl = _cpp_class.vTbl;
      __thisModule = impl;
   }
   ~Application()
   {
      Instance_decRef(impl);
   }

   struct Application_main_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Application &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()( );
   } main;
   // inline static void register_main(CPPClass & cl, Application::Application_main_Functor::FunctionType func)

   static void class_registration(CPPClass & _cpp_class);

   inline Application(int argc, const char ** argv, int exitCode, bool isGUIApp, OldList allModules, char * parsedCommand, NameSpace systemNameSpace);

   struct argc_Prop
   {
      constexpr argc_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline Application::argc_Prop & operator= (Application::argc_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } argc;
   struct argv_Prop
   {
      constexpr argv_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char ** operator= (/*0H*/const char ** v);
      /*regSet*/inline Application::argv_Prop & operator= (Application::argv_Prop & prop);
      /*regGet*/inline operator /*0I*/const char ** () const;
   } argv;
   struct exitCode_Prop
   {
      constexpr exitCode_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline Application::exitCode_Prop & operator= (Application::exitCode_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } exitCode;
   struct isGUIApp_Prop
   {
      constexpr isGUIApp_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline Application::isGUIApp_Prop & operator= (Application::isGUIApp_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isGUIApp;
   struct allModules_Prop
   {
      constexpr allModules_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline Application::allModules_Prop & operator= (Application::allModules_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } allModules;
   struct parsedCommand_Prop
   {
      constexpr parsedCommand_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline Application::parsedCommand_Prop & operator= (Application::parsedCommand_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } parsedCommand;
   struct systemNameSpace_Prop
   {
      constexpr systemNameSpace_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/NameSpace operator= (/*0H*/NameSpace v);
      /*regSet*/inline Application::systemNameSpace_Prop & operator= (Application::systemNameSpace_Prop & prop);
      /*regGet*/inline operator /*0I*/NameSpace () const;
   } systemNameSpace;
};

class Array
{
public:
   static TCPPClass<Array> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_Array(c, TP_T, TP_I, TP_D) \
      TArray::class_registration(_cpp_class); \
      REG_Container_copy(copy, c, TP_T, TP_I, TP_D); \
      REG_Container_delete(delete, c, TP_T, TP_I, TP_D); \
      REG_Container_free(free, c, TP_T, TP_I, TP_D); \
      REG_Container_freeIterator(freeIterator, c, TP_T, TP_I, TP_D); \
      REG_Container_getCount(getCount, c, TP_T, TP_I, TP_D); \
      REG_Container_getData(getData, c, TP_T, TP_I, TP_D); \
      REG_Container_getFirst(getFirst, c, TP_T, TP_I, TP_D); \
      REG_Container_getLast(getLast, c, TP_T, TP_I, TP_D); \
      REG_Container_getNext(getNext, c, TP_T, TP_I, TP_D); \
      REG_Container_getPrev(getPrev, c, TP_T, TP_I, TP_D); \
      REG_Container_move(move, c, TP_T, TP_I, TP_D); \
      REG_Container_remove(remove, c, TP_T, TP_I, TP_D); \
      REG_Container_removeAll(removeAll, c, TP_T, TP_I, TP_D); \
      REG_Container_sort(sort, c, TP_T, TP_I, TP_D);

#define ARRAY_VIRTUAL_METHODS_PROTO(c) \

#define ARRAY_VIRTUAL_METHODS(c) \

template <typename TP_T, typename TP_I = int, typename TP_D = TP_T>
class TArray : public TContainer<TP_T, TP_I, TP_D>, public Array
{
public:
   inline TArray/*<TP_T, TP_I, TP_D>*/(TArray<TP_T, TP_I, TP_D> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline TArray<TP_T, TP_I, TP_D> & operator= (TArray<TP_T, TP_I, TP_D> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   TArray() : TArray((C(Instance))Instance_newEx(ensureTemplatized <TP_T, TP_I, TP_D>(_cpp_class, "Array").impl, false), ensureTemplatized <TP_T, TP_I, TP_D>(_cpp_class, "Array")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(TArray)
   static TCPPClass<TArray> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         TArray <TP_T, TP_I, TP_D> * inst = new TArray <TP_T, TP_I, TP_D>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      TArray <TP_T, TP_I, TP_D>* inst = (TArray <TP_T, TP_I, TP_D>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(TArray <TP_T, TP_I, TP_D> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline TArray /*<TP_T, TP_I, TP_D>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TContainer<TP_T, TP_I, TP_D>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   TArray (std::initializer_list<TP_T> list) : TArray<TP_T, TP_I, TP_D> ()
   {
      typename std::initializer_list<TP_T>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   TArray<TP_T, TP_I, TP_D> & operator =(std::initializer_list<TP_T> list)
   {
      typename std::initializer_list<TP_T>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }

   inline TArray/*<TP_T, TP_I, TP_D>*/(TP(Container, T) * array, uint count, uint minAllocSize, uint size);

   struct size_Prop
   {
      constexpr size_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline TArray<TP_T _ARG TP_I _ARG TP_D>::size_Prop & operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::size_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } size;
   struct minAllocSize_Prop
   {
      constexpr minAllocSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline TArray<TP_T _ARG TP_I _ARG TP_D>::minAllocSize_Prop & operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::minAllocSize_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } minAllocSize;

   struct array_Prop
   {
      constexpr array_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TP(Container, T) * operator= (/*0H*/TP(Container, T) * v);
      /*regSet*/inline TArray<TP_T _ARG TP_I _ARG TP_D>::array_Prop & operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::array_Prop & prop);
      /*regGet*/inline operator /*0I*/TP(Container, T) * () const;
   } array;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline TArray<TP_T _ARG TP_I _ARG TP_D>::count_Prop & operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::count_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } count;
};

template <class TC, C(Class) ** TCO>
class TBTNamedLink : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TBTNamedLink<TC, TCO>::name_Prop & operator= (typename TBTNamedLink<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct parent_Prop
   {
      constexpr parent_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNamedLink operator= (/*0H*/BTNamedLink & v);
      /*regSet*/inline typename TBTNamedLink<TC, TCO>::parent_Prop & operator= (typename TBTNamedLink<TC, TCO>::parent_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNamedLink () const;
   } parent;
   struct left_Prop
   {
      constexpr left_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNamedLink operator= (/*0H*/BTNamedLink & v);
      /*regSet*/inline typename TBTNamedLink<TC, TCO>::left_Prop & operator= (typename TBTNamedLink<TC, TCO>::left_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNamedLink () const;
   } left;
   struct right_Prop
   {
      constexpr right_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNamedLink operator= (/*0H*/BTNamedLink & v);
      /*regSet*/inline typename TBTNamedLink<TC, TCO>::right_Prop & operator= (typename TBTNamedLink<TC, TCO>::right_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNamedLink () const;
   } right;
   struct depth_Prop
   {
      constexpr depth_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TBTNamedLink<TC, TCO>::depth_Prop & operator= (typename TBTNamedLink<TC, TCO>::depth_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } depth;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TBTNamedLink<TC, TCO>::data_Prop & operator= (typename TBTNamedLink<TC, TCO>::data_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } data;
   ~TBTNamedLink()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TBitMember : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BitMember operator= (/*0H*/BitMember & v);
      /*regSet*/inline typename TBitMember<TC, TCO>::prev_Prop & operator= (typename TBitMember<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/BitMember () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BitMember operator= (/*0H*/BitMember & v);
      /*regSet*/inline typename TBitMember<TC, TCO>::next_Prop & operator= (typename TBitMember<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/BitMember () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TBitMember<TC, TCO>::name_Prop & operator= (typename TBitMember<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct isProperty_Prop
   {
      constexpr isProperty_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TBitMember<TC, TCO>::isProperty_Prop & operator= (typename TBitMember<TC, TCO>::isProperty_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isProperty;
   struct memberAccess_Prop
   {
      constexpr memberAccess_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Access_Mode operator= (/*0H*/Access_Mode v);
      /*regSet*/inline typename TBitMember<TC, TCO>::memberAccess_Prop & operator= (typename TBitMember<TC, TCO>::memberAccess_Prop & prop);
      /*regGet*/inline operator /*0I*/Access_Mode () const;
   } memberAccess;
   struct id_Prop
   {
      constexpr id_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TBitMember<TC, TCO>::id_Prop & operator= (typename TBitMember<TC, TCO>::id_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } id;
   struct _class_Prop
   {
      constexpr _class_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TBitMember<TC, TCO>::_class_Prop & operator= (typename TBitMember<TC, TCO>::_class_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } _class;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TBitMember<TC, TCO>::dataTypeString_Prop & operator= (typename TBitMember<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct dataTypeClass_Prop
   {
      constexpr dataTypeClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TBitMember<TC, TCO>::dataTypeClass_Prop & operator= (typename TBitMember<TC, TCO>::dataTypeClass_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } dataTypeClass;
   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DataMemberType operator= (/*0H*/DataMemberType v);
      /*regSet*/inline typename TBitMember<TC, TCO>::type_Prop & operator= (typename TBitMember<TC, TCO>::type_Prop & prop);
      /*regGet*/inline operator /*0I*/DataMemberType () const;
   } type;
   struct size_Prop
   {
      constexpr size_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TBitMember<TC, TCO>::size_Prop & operator= (typename TBitMember<TC, TCO>::size_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } size;
   struct pos_Prop
   {
      constexpr pos_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TBitMember<TC, TCO>::pos_Prop & operator= (typename TBitMember<TC, TCO>::pos_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } pos;
   struct mask_Prop
   {
      constexpr mask_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint64 operator= (/*0H*/uint64 v);
      /*regSet*/inline typename TBitMember<TC, TCO>::mask_Prop & operator= (typename TBitMember<TC, TCO>::mask_Prop & prop);
      /*regGet*/inline operator /*0I*/uint64 () const;
   } mask;
   ~TBitMember()
   {
      this->impl = null; // How to know not to delete?
   }
};

struct BuiltInContainer : public TStruct<BuiltInContainer>
{
public:
   C(BuiltInContainer) impl;
   constexpr BuiltInContainer() : impl({}) { }
   constexpr BuiltInContainer(const C(BuiltInContainer) impl) : impl(impl) { }

   inline BuiltInContainer(void ** _vTbl);

   struct _vTbl_Prop
   {
      constexpr _vTbl_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void ** operator= (/*0H*/void ** v);
      /*regSet*/inline BuiltInContainer::_vTbl_Prop & operator= (BuiltInContainer::_vTbl_Prop & prop);
      /*regGet*/inline operator /*0I*/void ** () const;
   } _vTbl;
   struct _class_Prop
   {
      constexpr _class_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline BuiltInContainer::_class_Prop & operator= (BuiltInContainer::_class_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } _class;
   struct _refCount_Prop
   {
      constexpr _refCount_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline BuiltInContainer::_refCount_Prop & operator= (BuiltInContainer::_refCount_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } _refCount;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline BuiltInContainer::data_Prop & operator= (BuiltInContainer::data_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } data;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline BuiltInContainer::count_Prop & operator= (BuiltInContainer::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline BuiltInContainer::type_Prop & operator= (BuiltInContainer::type_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } type;
};

template <class TC, C(Class) ** TCO>
class TClass : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TClass<TC, TCO>::prev_Prop & operator= (typename TClass<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TClass<TC, TCO>::next_Prop & operator= (typename TClass<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClass<TC, TCO>::name_Prop & operator= (typename TClass<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct offset_Prop
   {
      constexpr offset_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::offset_Prop & operator= (typename TClass<TC, TCO>::offset_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } offset;
   struct structSize_Prop
   {
      constexpr structSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::structSize_Prop & operator= (typename TClass<TC, TCO>::structSize_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } structSize;
   struct _vTbl_Prop
   {
      constexpr _vTbl_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void ** operator= (/*0H*/void ** v);
      /*regSet*/inline typename TClass<TC, TCO>::_vTbl_Prop & operator= (typename TClass<TC, TCO>::_vTbl_Prop & prop);
      /*regGet*/inline operator /*0I*/void ** () const;
   } _vTbl;
   struct vTblSize_Prop
   {
      constexpr vTblSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::vTblSize_Prop & operator= (typename TClass<TC, TCO>::vTblSize_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } vTblSize;
   struct offsetClass_Prop
   {
      constexpr offsetClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::offsetClass_Prop & operator= (typename TClass<TC, TCO>::offsetClass_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } offsetClass;
   struct sizeClass_Prop
   {
      constexpr sizeClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::sizeClass_Prop & operator= (typename TClass<TC, TCO>::sizeClass_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } sizeClass;
   struct base_Prop
   {
      constexpr base_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TClass<TC, TCO>::base_Prop & operator= (typename TClass<TC, TCO>::base_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } base;
   struct methods_Prop
   {
      constexpr methods_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline typename TClass<TC, TCO>::methods_Prop & operator= (typename TClass<TC, TCO>::methods_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } methods;
   struct members_Prop
   {
      constexpr members_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline typename TClass<TC, TCO>::members_Prop & operator= (typename TClass<TC, TCO>::members_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } members;
   struct prop_Prop
   {
      constexpr prop_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline typename TClass<TC, TCO>::prop_Prop & operator= (typename TClass<TC, TCO>::prop_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } prop;
   struct membersAndProperties_Prop
   {
      constexpr membersAndProperties_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TClass<TC, TCO>::membersAndProperties_Prop & operator= (typename TClass<TC, TCO>::membersAndProperties_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } membersAndProperties;
   struct classProperties_Prop
   {
      constexpr classProperties_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline typename TClass<TC, TCO>::classProperties_Prop & operator= (typename TClass<TC, TCO>::classProperties_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } classProperties;
   struct derivatives_Prop
   {
      constexpr derivatives_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TClass<TC, TCO>::derivatives_Prop & operator= (typename TClass<TC, TCO>::derivatives_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } derivatives;
   struct memberID_Prop
   {
      constexpr memberID_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::memberID_Prop & operator= (typename TClass<TC, TCO>::memberID_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } memberID;
   struct startMemberID_Prop
   {
      constexpr startMemberID_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::startMemberID_Prop & operator= (typename TClass<TC, TCO>::startMemberID_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } startMemberID;
   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ClassType operator= (/*0H*/ClassType v);
      /*regSet*/inline typename TClass<TC, TCO>::type_Prop & operator= (typename TClass<TC, TCO>::type_Prop & prop);
      /*regGet*/inline operator /*0I*/ClassType () const;
   } type;
   struct module_Prop
   {
      constexpr module_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Module operator= (/*0H*/Module v);
      /*regSet*/inline typename TClass<TC, TCO>::module_Prop & operator= (typename TClass<TC, TCO>::module_Prop & prop);
      /*regGet*/inline operator /*0I*/Module () const;
   } module;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClass<TC, TCO>::dataTypeString_Prop & operator= (typename TClass<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct typeSize_Prop
   {
      constexpr typeSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::typeSize_Prop & operator= (typename TClass<TC, TCO>::typeSize_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } typeSize;
   struct defaultAlignment_Prop
   {
      constexpr defaultAlignment_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::defaultAlignment_Prop & operator= (typename TClass<TC, TCO>::defaultAlignment_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } defaultAlignment;
   struct memberOffset_Prop
   {
      constexpr memberOffset_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::memberOffset_Prop & operator= (typename TClass<TC, TCO>::memberOffset_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } memberOffset;
   struct selfWatchers_Prop
   {
      constexpr selfWatchers_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TClass<TC, TCO>::selfWatchers_Prop & operator= (typename TClass<TC, TCO>::selfWatchers_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } selfWatchers;
   struct designerClass_Prop
   {
      constexpr designerClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClass<TC, TCO>::designerClass_Prop & operator= (typename TClass<TC, TCO>::designerClass_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } designerClass;
   struct noExpansion_Prop
   {
      constexpr noExpansion_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClass<TC, TCO>::noExpansion_Prop & operator= (typename TClass<TC, TCO>::noExpansion_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } noExpansion;
   struct defaultProperty_Prop
   {
      constexpr defaultProperty_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClass<TC, TCO>::defaultProperty_Prop & operator= (typename TClass<TC, TCO>::defaultProperty_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } defaultProperty;
   struct comRedefinition_Prop
   {
      constexpr comRedefinition_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClass<TC, TCO>::comRedefinition_Prop & operator= (typename TClass<TC, TCO>::comRedefinition_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } comRedefinition;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::count_Prop & operator= (typename TClass<TC, TCO>::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
   struct isRemote_Prop
   {
      constexpr isRemote_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::isRemote_Prop & operator= (typename TClass<TC, TCO>::isRemote_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } isRemote;
   struct internalDecl_Prop
   {
      constexpr internalDecl_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClass<TC, TCO>::internalDecl_Prop & operator= (typename TClass<TC, TCO>::internalDecl_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } internalDecl;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TClass<TC, TCO>::data_Prop & operator= (typename TClass<TC, TCO>::data_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } data;
   struct computeSize_Prop
   {
      constexpr computeSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClass<TC, TCO>::computeSize_Prop & operator= (typename TClass<TC, TCO>::computeSize_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } computeSize;
   struct structAlignment_Prop
   {
      constexpr structAlignment_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/short operator= (/*0H*/short v);
      /*regSet*/inline typename TClass<TC, TCO>::structAlignment_Prop & operator= (typename TClass<TC, TCO>::structAlignment_Prop & prop);
      /*regGet*/inline operator /*0I*/short () const;
   } structAlignment;
   struct pointerAlignment_Prop
   {
      constexpr pointerAlignment_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/short operator= (/*0H*/short v);
      /*regSet*/inline typename TClass<TC, TCO>::pointerAlignment_Prop & operator= (typename TClass<TC, TCO>::pointerAlignment_Prop & prop);
      /*regGet*/inline operator /*0I*/short () const;
   } pointerAlignment;
   struct destructionWatchOffset_Prop
   {
      constexpr destructionWatchOffset_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::destructionWatchOffset_Prop & operator= (typename TClass<TC, TCO>::destructionWatchOffset_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } destructionWatchOffset;
   struct fixed_Prop
   {
      constexpr fixed_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClass<TC, TCO>::fixed_Prop & operator= (typename TClass<TC, TCO>::fixed_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } fixed;
   struct delayedCPValues_Prop
   {
      constexpr delayedCPValues_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TClass<TC, TCO>::delayedCPValues_Prop & operator= (typename TClass<TC, TCO>::delayedCPValues_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } delayedCPValues;
   struct inheritanceAccess_Prop
   {
      constexpr inheritanceAccess_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Access_Mode operator= (/*0H*/Access_Mode v);
      /*regSet*/inline typename TClass<TC, TCO>::inheritanceAccess_Prop & operator= (typename TClass<TC, TCO>::inheritanceAccess_Prop & prop);
      /*regGet*/inline operator /*0I*/Access_Mode () const;
   } inheritanceAccess;
   struct fullName_Prop
   {
      constexpr fullName_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClass<TC, TCO>::fullName_Prop & operator= (typename TClass<TC, TCO>::fullName_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } fullName;
   struct symbol_Prop
   {
      constexpr symbol_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TClass<TC, TCO>::symbol_Prop & operator= (typename TClass<TC, TCO>::symbol_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } symbol;
   struct conversions_Prop
   {
      constexpr conversions_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TClass<TC, TCO>::conversions_Prop & operator= (typename TClass<TC, TCO>::conversions_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } conversions;
   struct templateParams_Prop
   {
      constexpr templateParams_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TClass<TC, TCO>::templateParams_Prop & operator= (typename TClass<TC, TCO>::templateParams_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } templateParams;
   struct templateClass_Prop
   {
      constexpr templateClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TClass<TC, TCO>::templateClass_Prop & operator= (typename TClass<TC, TCO>::templateClass_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } templateClass;
   struct templatized_Prop
   {
      constexpr templatized_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TClass<TC, TCO>::templatized_Prop & operator= (typename TClass<TC, TCO>::templatized_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } templatized;
   struct numParams_Prop
   {
      constexpr numParams_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClass<TC, TCO>::numParams_Prop & operator= (typename TClass<TC, TCO>::numParams_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } numParams;
   struct isInstanceClass_Prop
   {
      constexpr isInstanceClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClass<TC, TCO>::isInstanceClass_Prop & operator= (typename TClass<TC, TCO>::isInstanceClass_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isInstanceClass;
   struct byValueSystemClass_Prop
   {
      constexpr byValueSystemClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClass<TC, TCO>::byValueSystemClass_Prop & operator= (typename TClass<TC, TCO>::byValueSystemClass_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } byValueSystemClass;
   struct bindingsClass_Prop
   {
      constexpr bindingsClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TClass<TC, TCO>::bindingsClass_Prop & operator= (typename TClass<TC, TCO>::bindingsClass_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } bindingsClass;
   ~TClass()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TClassProperty : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClassProperty<TC, TCO>::name_Prop & operator= (typename TClassProperty<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct parent_Prop
   {
      constexpr parent_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ClassProperty operator= (/*0H*/ClassProperty & v);
      /*regSet*/inline typename TClassProperty<TC, TCO>::parent_Prop & operator= (typename TClassProperty<TC, TCO>::parent_Prop & prop);
      /*regGet*/inline operator /*0I*/ClassProperty () const;
   } parent;
   struct left_Prop
   {
      constexpr left_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ClassProperty operator= (/*0H*/ClassProperty & v);
      /*regSet*/inline typename TClassProperty<TC, TCO>::left_Prop & operator= (typename TClassProperty<TC, TCO>::left_Prop & prop);
      /*regGet*/inline operator /*0I*/ClassProperty () const;
   } left;
   struct right_Prop
   {
      constexpr right_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ClassProperty operator= (/*0H*/ClassProperty & v);
      /*regSet*/inline typename TClassProperty<TC, TCO>::right_Prop & operator= (typename TClassProperty<TC, TCO>::right_Prop & prop);
      /*regGet*/inline operator /*0I*/ClassProperty () const;
   } right;
   struct depth_Prop
   {
      constexpr depth_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TClassProperty<TC, TCO>::depth_Prop & operator= (typename TClassProperty<TC, TCO>::depth_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } depth;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClassProperty<TC, TCO>::dataTypeString_Prop & operator= (typename TClassProperty<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct constant_Prop
   {
      constexpr constant_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TClassProperty<TC, TCO>::constant_Prop & operator= (typename TClassProperty<TC, TCO>::constant_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } constant;
   ~TClassProperty()
   {
      this->impl = null; // How to know not to delete?
   }
};

struct ClassTemplateArgument : public TStruct<ClassTemplateArgument>
{
public:
   C(ClassTemplateArgument) impl;
   constexpr ClassTemplateArgument() : impl({}) { }
   constexpr ClassTemplateArgument(const C(ClassTemplateArgument) impl) : impl(impl) { }

   inline ClassTemplateArgument(const char * dataTypeString);

   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline ClassTemplateArgument::dataTypeString_Prop & operator= (ClassTemplateArgument::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct dataTypeClass_Prop
   {
      constexpr dataTypeClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline ClassTemplateArgument::dataTypeClass_Prop & operator= (ClassTemplateArgument::dataTypeClass_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } dataTypeClass;
   struct expression_Prop
   {
      constexpr expression_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DataValue operator= (/*0H*/DataValue v);
      /*regSet*/inline ClassTemplateArgument::expression_Prop & operator= (ClassTemplateArgument::expression_Prop & prop);
      /*regGet*/inline operator /*0I*/DataValue () const;
   } expression;
   struct memberString_Prop
   {
      constexpr memberString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline ClassTemplateArgument::memberString_Prop & operator= (ClassTemplateArgument::memberString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } memberString;
   struct member_Prop
   {
      constexpr member_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DataMember operator= (/*0H*/DataMember & v);
      /*regSet*/inline ClassTemplateArgument::member_Prop & operator= (ClassTemplateArgument::member_Prop & prop);
      /*regGet*/inline operator /*0I*/DataMember () const;
   } member;
   struct prop_Prop
   {
      constexpr prop_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Property operator= (/*0H*/Property & v);
      /*regSet*/inline ClassTemplateArgument::prop_Prop & operator= (ClassTemplateArgument::prop_Prop & prop);
      /*regGet*/inline operator /*0I*/Property () const;
   } prop;
   struct method_Prop
   {
      constexpr method_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Method operator= (/*0H*/Method & v);
      /*regSet*/inline ClassTemplateArgument::method_Prop & operator= (ClassTemplateArgument::method_Prop & prop);
      /*regGet*/inline operator /*0I*/Method () const;
   } method;
};

template <class TC, C(Class) ** TCO>
class TClassTemplateParameter : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ClassTemplateParameter operator= (/*0H*/ClassTemplateParameter & v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::prev_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/ClassTemplateParameter () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ClassTemplateParameter operator= (/*0H*/ClassTemplateParameter & v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::next_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/ClassTemplateParameter () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::name_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TemplateParameterType operator= (/*0H*/TemplateParameterType v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::type_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::type_Prop & prop);
      /*regGet*/inline operator /*0I*/TemplateParameterType () const;
   } type;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::dataTypeString_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct memberType_Prop
   {
      constexpr memberType_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TemplateMemberType operator= (/*0H*/TemplateMemberType v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::memberType_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::memberType_Prop & prop);
      /*regGet*/inline operator /*0I*/TemplateMemberType () const;
   } memberType;
   struct defaultArg_Prop
   {
      constexpr defaultArg_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ClassTemplateArgument operator= (/*0H*/ClassTemplateArgument v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::defaultArg_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::defaultArg_Prop & prop);
      /*regGet*/inline operator /*0I*/ClassTemplateArgument () const;
   } defaultArg;
   struct param_Prop
   {
      constexpr param_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TClassTemplateParameter<TC, TCO>::param_Prop & operator= (typename TClassTemplateParameter<TC, TCO>::param_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } param;
   ~TClassTemplateParameter()
   {
      this->impl = null; // How to know not to delete?
   }
};

enum class ClassType : int
{
   normalClass = ClassType_normalClass,
   structClass = ClassType_structClass,
   bitClass = ClassType_bitClass,
   unitClass = ClassType_unitClass,
   enumClass = ClassType_enumClass,
   noHeadClass = ClassType_noHeadClass,
   unionClass = ClassType_unionClass,
   systemClass = ClassType_systemClass
};

template <class TC, C(Class) ** TCO>
class TDataMember : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DataMember operator= (/*0H*/DataMember & v);
      /*regSet*/inline typename TDataMember<TC, TCO>::prev_Prop & operator= (typename TDataMember<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/DataMember () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DataMember operator= (/*0H*/DataMember & v);
      /*regSet*/inline typename TDataMember<TC, TCO>::next_Prop & operator= (typename TDataMember<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/DataMember () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TDataMember<TC, TCO>::name_Prop & operator= (typename TDataMember<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct isProperty_Prop
   {
      constexpr isProperty_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TDataMember<TC, TCO>::isProperty_Prop & operator= (typename TDataMember<TC, TCO>::isProperty_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isProperty;
   struct memberAccess_Prop
   {
      constexpr memberAccess_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Access_Mode operator= (/*0H*/Access_Mode v);
      /*regSet*/inline typename TDataMember<TC, TCO>::memberAccess_Prop & operator= (typename TDataMember<TC, TCO>::memberAccess_Prop & prop);
      /*regGet*/inline operator /*0I*/Access_Mode () const;
   } memberAccess;
   struct id_Prop
   {
      constexpr id_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TDataMember<TC, TCO>::id_Prop & operator= (typename TDataMember<TC, TCO>::id_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } id;
   struct _class_Prop
   {
      constexpr _class_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TDataMember<TC, TCO>::_class_Prop & operator= (typename TDataMember<TC, TCO>::_class_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } _class;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TDataMember<TC, TCO>::dataTypeString_Prop & operator= (typename TDataMember<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct dataTypeClass_Prop
   {
      constexpr dataTypeClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TDataMember<TC, TCO>::dataTypeClass_Prop & operator= (typename TDataMember<TC, TCO>::dataTypeClass_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } dataTypeClass;
   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DataMemberType operator= (/*0H*/DataMemberType v);
      /*regSet*/inline typename TDataMember<TC, TCO>::type_Prop & operator= (typename TDataMember<TC, TCO>::type_Prop & prop);
      /*regGet*/inline operator /*0I*/DataMemberType () const;
   } type;
   struct offset_Prop
   {
      constexpr offset_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TDataMember<TC, TCO>::offset_Prop & operator= (typename TDataMember<TC, TCO>::offset_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } offset;
   struct memberID_Prop
   {
      constexpr memberID_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TDataMember<TC, TCO>::memberID_Prop & operator= (typename TDataMember<TC, TCO>::memberID_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } memberID;
   struct members_Prop
   {
      constexpr members_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TDataMember<TC, TCO>::members_Prop & operator= (typename TDataMember<TC, TCO>::members_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } members;
   struct membersAlpha_Prop
   {
      constexpr membersAlpha_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline typename TDataMember<TC, TCO>::membersAlpha_Prop & operator= (typename TDataMember<TC, TCO>::membersAlpha_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } membersAlpha;
   struct memberOffset_Prop
   {
      constexpr memberOffset_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TDataMember<TC, TCO>::memberOffset_Prop & operator= (typename TDataMember<TC, TCO>::memberOffset_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } memberOffset;
   struct structAlignment_Prop
   {
      constexpr structAlignment_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/short operator= (/*0H*/short v);
      /*regSet*/inline typename TDataMember<TC, TCO>::structAlignment_Prop & operator= (typename TDataMember<TC, TCO>::structAlignment_Prop & prop);
      /*regGet*/inline operator /*0I*/short () const;
   } structAlignment;
   struct pointerAlignment_Prop
   {
      constexpr pointerAlignment_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/short operator= (/*0H*/short v);
      /*regSet*/inline typename TDataMember<TC, TCO>::pointerAlignment_Prop & operator= (typename TDataMember<TC, TCO>::pointerAlignment_Prop & prop);
      /*regGet*/inline operator /*0I*/short () const;
   } pointerAlignment;
   ~TDataMember()
   {
      this->impl = null; // How to know not to delete?
   }
};

enum class DataMemberType : int
{
   normalMember = DataMemberType_normalMember,
   unionMember = DataMemberType_unionMember,
   structMember = DataMemberType_structMember
};

struct DataValue : public TStruct<DataValue>
{
public:
   C(DataValue) impl;
   constexpr DataValue() : impl({}) { }
   constexpr DataValue(const C(DataValue) impl) : impl(impl) { }

   inline DataValue(char c, unsigned char uc, short s, uint16 us, int i, uint ui, void * p, float f, double d, int64 i64, uint64 ui64);

   struct c_Prop
   {
      constexpr c_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char operator= (/*0H*/char v);
      /*regSet*/inline DataValue::c_Prop & operator= (DataValue::c_Prop & prop);
      /*regGet*/inline operator /*0I*/char () const;
   } c;
   struct uc_Prop
   {
      constexpr uc_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/unsigned char operator= (/*0H*/unsigned char v);
      /*regSet*/inline DataValue::uc_Prop & operator= (DataValue::uc_Prop & prop);
      /*regGet*/inline operator /*0I*/unsigned char () const;
   } uc;
   struct s_Prop
   {
      constexpr s_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/short operator= (/*0H*/short v);
      /*regSet*/inline DataValue::s_Prop & operator= (DataValue::s_Prop & prop);
      /*regGet*/inline operator /*0I*/short () const;
   } s;
   struct us_Prop
   {
      constexpr us_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint16 operator= (/*0H*/uint16 v);
      /*regSet*/inline DataValue::us_Prop & operator= (DataValue::us_Prop & prop);
      /*regGet*/inline operator /*0I*/uint16 () const;
   } us;
   struct i_Prop
   {
      constexpr i_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline DataValue::i_Prop & operator= (DataValue::i_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } i;
   struct ui_Prop
   {
      constexpr ui_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline DataValue::ui_Prop & operator= (DataValue::ui_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } ui;
   struct p_Prop
   {
      constexpr p_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline DataValue::p_Prop & operator= (DataValue::p_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } p;
   struct f_Prop
   {
      constexpr f_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/float operator= (/*0H*/float v);
      /*regSet*/inline DataValue::f_Prop & operator= (DataValue::f_Prop & prop);
      /*regGet*/inline operator /*0I*/float () const;
   } f;
   struct d_Prop
   {
      constexpr d_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/double operator= (/*0H*/double v);
      /*regSet*/inline DataValue::d_Prop & operator= (DataValue::d_Prop & prop);
      /*regGet*/inline operator /*0I*/double () const;
   } d;
   struct i64_Prop
   {
      constexpr i64_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int64 operator= (/*0H*/int64 v);
      /*regSet*/inline DataValue::i64_Prop & operator= (DataValue::i64_Prop & prop);
      /*regGet*/inline operator /*0I*/int64 () const;
   } i64;
   struct ui64_Prop
   {
      constexpr ui64_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint64 operator= (/*0H*/uint64 v);
      /*regSet*/inline DataValue::ui64_Prop & operator= (DataValue::ui64_Prop & prop);
      /*regGet*/inline operator /*0I*/uint64 () const;
   } ui64;
};

template <class TC, C(Class) ** TCO>
class TDefinedExpression : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DefinedExpression operator= (/*0H*/DefinedExpression & v);
      /*regSet*/inline typename TDefinedExpression<TC, TCO>::prev_Prop & operator= (typename TDefinedExpression<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/DefinedExpression () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/DefinedExpression operator= (/*0H*/DefinedExpression & v);
      /*regSet*/inline typename TDefinedExpression<TC, TCO>::next_Prop & operator= (typename TDefinedExpression<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/DefinedExpression () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TDefinedExpression<TC, TCO>::name_Prop & operator= (typename TDefinedExpression<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct value_Prop
   {
      constexpr value_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TDefinedExpression<TC, TCO>::value_Prop & operator= (typename TDefinedExpression<TC, TCO>::value_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } value;
   ~TDefinedExpression()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TEnumClassData : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct values_Prop
   {
      constexpr values_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TEnumClassData<TC, TCO>::values_Prop & operator= (typename TEnumClassData<TC, TCO>::values_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } values;
   struct largest_Prop
   {
      constexpr largest_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int64 operator= (/*0H*/int64 v);
      /*regSet*/inline typename TEnumClassData<TC, TCO>::largest_Prop & operator= (typename TEnumClassData<TC, TCO>::largest_Prop & prop);
      /*regGet*/inline operator /*0I*/int64 () const;
   } largest;
   ~TEnumClassData()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TGlobalFunction : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/GlobalFunction operator= (/*0H*/GlobalFunction & v);
      /*regSet*/inline typename TGlobalFunction<TC, TCO>::prev_Prop & operator= (typename TGlobalFunction<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/GlobalFunction () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/GlobalFunction operator= (/*0H*/GlobalFunction & v);
      /*regSet*/inline typename TGlobalFunction<TC, TCO>::next_Prop & operator= (typename TGlobalFunction<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/GlobalFunction () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TGlobalFunction<TC, TCO>::name_Prop & operator= (typename TGlobalFunction<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct module_Prop
   {
      constexpr module_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Module operator= (/*0H*/Module v);
      /*regSet*/inline typename TGlobalFunction<TC, TCO>::module_Prop & operator= (typename TGlobalFunction<TC, TCO>::module_Prop & prop);
      /*regGet*/inline operator /*0I*/Module () const;
   } module;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TGlobalFunction<TC, TCO>::dataTypeString_Prop & operator= (typename TGlobalFunction<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct symbol_Prop
   {
      constexpr symbol_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TGlobalFunction<TC, TCO>::symbol_Prop & operator= (typename TGlobalFunction<TC, TCO>::symbol_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } symbol;
   ~TGlobalFunction()
   {
      this->impl = null; // How to know not to delete?
   }
};

class HashMap
{
public:
   static TCPPClass<HashMap> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_HashMap(c, TP_KT, TP_VT) \
      THashMap::class_registration(_cpp_class);

#define HASHMAP_VIRTUAL_METHODS_PROTO(c) \

#define HASHMAP_VIRTUAL_METHODS(c) \

template <typename TP_KT = int64, typename TP_VT = uintptr>
class THashMap : public TContainer<TP_KT, TP_VT>, public HashMap
{
public:
   inline THashMap/*<TP_KT, TP_VT>*/(THashMap<TP_KT, TP_VT> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline THashMap<TP_KT, TP_VT> & operator= (THashMap<TP_KT, TP_VT> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   THashMap() : THashMap((C(Instance))Instance_newEx(ensureTemplatized <TP_KT, TP_VT>(_cpp_class, "HashMap").impl, false), ensureTemplatized <TP_KT, TP_VT>(_cpp_class, "HashMap")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(THashMap)
   static TCPPClass<THashMap> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         THashMap <TP_KT, TP_VT> * inst = new THashMap <TP_KT, TP_VT>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      THashMap <TP_KT, TP_VT>* inst = (THashMap <TP_KT, TP_VT>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(THashMap <TP_KT, TP_VT> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline THashMap /*<TP_KT, TP_VT>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TContainer<TP_KT, TP_VT>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   THashMap (std::initializer_list<TP_KT> list) : THashMap<TP_KT, TP_VT> ()
   {
      typename std::initializer_list<TP_KT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   THashMap<TP_KT, TP_VT> & operator =(std::initializer_list<TP_KT> list)
   {
      typename std::initializer_list<TP_KT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }

   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } count;
   struct initSize_Prop
   {
      constexpr initSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0G*/int operator= (/*0G*/int v);
   } initSize;

   struct noRemResize_Prop
   {
      constexpr noRemResize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline THashMap<TP_KT _ARG TP_VT>::noRemResize_Prop & operator= (THashMap<TP_KT _ARG TP_VT>::noRemResize_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } noRemResize;
};

struct HashMapIterator : Struct
{
public:
   static TCPPClass<HashMapIterator> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};

template <typename TP_KT, typename TP_VT>
class THashMapIterator : HashMapIterator
{
public:
   C(HashMapIterator) impl;
   constexpr THashMapIterator() : impl({}) { }
   constexpr THashMapIterator(const C(HashMapIterator) impl) : impl(impl) { }

   struct map_Prop
   {
      constexpr map_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0A*/const THashMap<TP_KT, TP_VT> & operator= (/*0A*/const THashMap<TP_KT, TP_VT> & v);
      /*regSet*/inline THashMapIterator<TP_KT _ARG TP_VT>::map_Prop & operator= (THashMapIterator<TP_KT _ARG TP_VT>::map_Prop & prop);
      /*nstSet*/inline /*0C*/const THashMap<TP_KT, TP_VT> * operator= (/*0C*/const THashMap<TP_KT, TP_VT> * v);
      /*regGet*/inline operator /*0B*/TIH<THashMap<TP_KT, TP_VT>> () const;
      /*regGet*/inline TIH<THashMap<TP_KT, TP_VT>> operator /*0D*/-> () const;
      /*regGet*/inline operator /*0E*/THashMap<TP_KT, TP_VT> () const;
      /*regGet*/inline operator /*0F*/THashMap<TP_KT, TP_VT>* () const;
   } map;
   struct key_Prop
   {
      constexpr key_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0B*/TIH<TP_KT> () const;
      /*regGet*/inline TIH<TP_KT> operator /*0D*/-> () const;
      /*regGet*/inline operator /*0E*/TP_KT () const;
      /*regGet*/inline operator /*0F*/TP_KT* () const;
   } key;
   struct value_Prop
   {
      constexpr value_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0A*/const TP_VT & operator= (/*0A*/const TP_VT & v);
      /*regSet*/inline THashMapIterator<TP_KT _ARG TP_VT>::value_Prop & operator= (THashMapIterator<TP_KT _ARG TP_VT>::value_Prop & prop);
      /*nstSet*/inline /*0C*/const TP_VT * operator= (/*0C*/const TP_VT * v);
      /*regGet*/inline operator /*0B*/TIH<TP_VT> () const;
      /*regGet*/inline TIH<TP_VT> operator /*0D*/-> () const;
      /*regGet*/inline operator /*0E*/TP_VT () const;
      /*regGet*/inline operator /*0F*/TP_VT* () const;
   } value;
};

class HashTable
{
public:
   static TCPPClass<HashTable> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_HashTable(c, TP_KT) \
      THashTable::class_registration(_cpp_class);

#define HASHTABLE_VIRTUAL_METHODS_PROTO(c) \

#define HASHTABLE_VIRTUAL_METHODS(c) \

template <typename TP_KT = int64>
class THashTable : public TContainer<TP_KT>, public HashTable
{
public:
   inline THashTable/*<TP_KT>*/(THashTable<TP_KT> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline THashTable<TP_KT> & operator= (THashTable<TP_KT> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   THashTable() : THashTable((C(Instance))Instance_newEx(ensureTemplatized <TP_KT>(_cpp_class, "HashTable").impl, false), ensureTemplatized <TP_KT>(_cpp_class, "HashTable")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(THashTable)
   static TCPPClass<THashTable> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         THashTable <TP_KT> * inst = new THashTable <TP_KT>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      THashTable <TP_KT>* inst = (THashTable <TP_KT>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(THashTable <TP_KT> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline THashTable /*<TP_KT>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TContainer<TP_KT>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   THashTable (std::initializer_list<TP_KT> list) : THashTable<TP_KT> ()
   {
      typename std::initializer_list<TP_KT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   THashTable<TP_KT> & operator =(std::initializer_list<TP_KT> list)
   {
      typename std::initializer_list<TP_KT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }

   struct initSize_Prop
   {
      constexpr initSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0G*/int operator= (/*0G*/int v);
   } initSize;
};

enum class ImportType : int
{
   normalImport = ImportType_normalImport,
   staticImport = ImportType_staticImport,
   remoteImport = ImportType_remoteImport,
   preDeclImport = ImportType_preDeclImport,
   comCheckImport = ImportType_comCheckImport
};

template <class TC, C(Class) ** TCO>
class TLink : public TListItem<TC, TCO>
{
public:
   using TListItem<TC, TCO>::TListItem;
   inline operator ListItem& () const { return *(ListItem *)this; }

   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint64 operator= (/*0H*/uint64 v);
      /*regSet*/inline typename TLink<TC, TCO>::data_Prop & operator= (typename TLink<TC, TCO>::data_Prop & prop);
      /*regGet*/inline operator /*0I*/uint64 () const;
   } data;
   ~TLink()
   {
      this->impl = null; // How to know not to delete?
   }
};

struct LinkElement : Struct
{
public:
   static TCPPClass<LinkElement> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};

template <typename TP_T>
class TLinkElement : LinkElement
{
public:
   C(LinkElement) impl;
   constexpr TLinkElement() : impl({}) { }
   constexpr TLinkElement(const C(LinkElement) impl) : impl(impl) { }

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TP_T operator= (/*0H*/TP_T v);
      /*regSet*/inline TLinkElement<TP_T>::prev_Prop & operator= (TLinkElement<TP_T>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/TP_T () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/TP_T operator= (/*0H*/TP_T v);
      /*regSet*/inline TLinkElement<TP_T>::next_Prop & operator= (TLinkElement<TP_T>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/TP_T () const;
   } next;
};

class List
{
public:
   static TCPPClass<List> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_List(c, TP_LLT) \
      TList::class_registration(_cpp_class);

#define LIST_VIRTUAL_METHODS_PROTO(c) \

#define LIST_VIRTUAL_METHODS(c) \

template <typename TP_LLT>
class TList : public TLinkList<TP_LLT>, public List
{
public:
   inline TList/*<TP_LLT>*/(TList<TP_LLT> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline TList<TP_LLT> & operator= (TList<TP_LLT> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   TList() : TList((C(Instance))Instance_newEx(ensureTemplatized <TP_LLT>(_cpp_class, "List").impl, false), ensureTemplatized <TP_LLT>(_cpp_class, "List")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(TList)
   static TCPPClass<TList> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         TList <TP_LLT> * inst = new TList <TP_LLT>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      TList <TP_LLT>* inst = (TList <TP_LLT>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(TList <TP_LLT> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline TList /*<TP_LLT>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TLinkList<TP_LLT>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   TList (std::initializer_list<TP_LLT> list) : TList<TP_LLT> ()
   {
      typename std::initializer_list<TP_LLT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   TList<TP_LLT> & operator =(std::initializer_list<TP_LLT> list)
   {
      typename std::initializer_list<TP_LLT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }
};

class Map
{
public:
   static TCPPClass<Map> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};


#define TREG_Map(c, TP_MT, TP_V) \
      TMap::class_registration(_cpp_class);

#define MAP_VIRTUAL_METHODS_PROTO(c) \

#define MAP_VIRTUAL_METHODS(c) \

template <typename TP_MT, typename TP_V>
class TMap : public TCustomAVLTree<TP_MT, TP_V>, public Map
{
public:
   inline TMap/*<TP_MT, TP_V>*/(TMap<TP_MT, TP_V> && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline TMap<TP_MT, TP_V> & operator= (TMap<TP_MT, TP_V> && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   TMap() : TMap((C(Instance))Instance_newEx(ensureTemplatized <TP_MT, TP_V>(_cpp_class, "Map").impl, false), ensureTemplatized <TP_MT, TP_V>(_cpp_class, "Map")) { }
   INSTANCE_VIRTUAL_METHODS_PROTO(TMap)
   static TCPPClass<TMap> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         TMap <TP_MT, TP_V> * inst = new TMap <TP_MT, TP_V>(i, _cpp_class);
         if(inst)
         {
            /*printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      TMap <TP_MT, TP_V>* inst = (TMap <TP_MT, TP_V>*)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor) ((void (*)(TMap <TP_MT, TP_V> & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree) delete inst;
      }
   }
   explicit inline TMap /*<TP_MT, TP_V>*/(C(Instance) _impl, CPPClass & cl = _cpp_class) : TCustomAVLTree<TP_MT, TP_V>(_impl, cl)
 { }

   static void class_registration(CPPClass & _cpp_class);

   TMap (std::initializer_list<TP_MT> list) : TMap<TP_MT, TP_V> ()
   {
      typename std::initializer_list<TP_MT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
   }

   TMap<TP_MT, TP_V> & operator =(std::initializer_list<TP_MT> list)
   {
      typename std::initializer_list<TP_MT>::iterator it;
      for(it = list.begin(); it != list.end(); ++it)
         this->add(*it);
      return *this;
   }

   struct mapSrc_Prop
   {
      constexpr mapSrc_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0A*/const Map & operator= (/*0A*/const Map & v);
      /*nstSet*/inline /*0C*/const Map * operator= (/*0C*/const Map * v);
   } mapSrc;
};

struct MapIterator : Struct
{
public:
   static TCPPClass<MapIterator> _cpp_class;
   static void class_registration(CPPClass & _cpp_class);
};

template <typename TP_KT, typename TP_V>
class TMapIterator : MapIterator
{
public:
   C(MapIterator) impl;
   constexpr TMapIterator() : impl({}) { }
   constexpr TMapIterator(const C(MapIterator) impl) : impl(impl) { }

   // REVIEW: How to generate this here since MapIterator inherits from Iterator?
   inline C(bool) index(TP_KT index, C(bool) create);

   struct map_Prop
   {
      constexpr map_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0A*/const TMap<TP_KT, TP_V> & operator= (/*0A*/const TMap<TP_KT, TP_V> & v);
      /*regSet*/inline TMapIterator<TP_KT _ARG TP_V>::map_Prop & operator= (TMapIterator<TP_KT _ARG TP_V>::map_Prop & prop);
      /*nstSet*/inline /*0C*/const TMap<TP_KT, TP_V> * operator= (/*0C*/const TMap<TP_KT, TP_V> * v);
      /*regGet*/inline operator /*0B*/TIH<TMap<TP_KT, TP_V>> () const;
      /*regGet*/inline TIH<TMap<TP_KT, TP_V>> operator /*0D*/-> () const;
      /*regGet*/inline operator /*0E*/TMap<TP_KT, TP_V> () const;
      /*regGet*/inline operator /*0F*/TMap<TP_KT, TP_V>* () const;
   } map;
   struct key_Prop
   {
      constexpr key_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0B*/TIH<TP_KT> () const;
      /*regGet*/inline TIH<TP_KT> operator /*0D*/-> () const;
      /*regGet*/inline operator /*0E*/TP_KT () const;
      /*regGet*/inline operator /*0F*/TP_KT* () const;
   } key;
   struct value_Prop
   {
      constexpr value_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0A*/const TP_V & operator= (/*0A*/const TP_V & v);
      /*regSet*/inline TMapIterator<TP_KT _ARG TP_V>::value_Prop & operator= (TMapIterator<TP_KT _ARG TP_V>::value_Prop & prop);
      /*nstSet*/inline /*0C*/const TP_V * operator= (/*0C*/const TP_V * v);
      /*regGet*/inline operator /*0B*/TIH<TP_V> () const;
      /*regGet*/inline TIH<TP_V> operator /*0D*/-> () const;
      /*regGet*/inline operator /*0E*/TP_V () const;
      /*regGet*/inline operator /*0F*/TP_V* () const;
   } value;
};

template <class TC, C(Class) ** TCO>
class TMethod : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TMethod<TC, TCO>::name_Prop & operator= (typename TMethod<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct parent_Prop
   {
      constexpr parent_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Method operator= (/*0H*/Method & v);
      /*regSet*/inline typename TMethod<TC, TCO>::parent_Prop & operator= (typename TMethod<TC, TCO>::parent_Prop & prop);
      /*regGet*/inline operator /*0I*/Method () const;
   } parent;
   struct left_Prop
   {
      constexpr left_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Method operator= (/*0H*/Method & v);
      /*regSet*/inline typename TMethod<TC, TCO>::left_Prop & operator= (typename TMethod<TC, TCO>::left_Prop & prop);
      /*regGet*/inline operator /*0I*/Method () const;
   } left;
   struct right_Prop
   {
      constexpr right_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Method operator= (/*0H*/Method & v);
      /*regSet*/inline typename TMethod<TC, TCO>::right_Prop & operator= (typename TMethod<TC, TCO>::right_Prop & prop);
      /*regGet*/inline operator /*0I*/Method () const;
   } right;
   struct depth_Prop
   {
      constexpr depth_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TMethod<TC, TCO>::depth_Prop & operator= (typename TMethod<TC, TCO>::depth_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } depth;
   struct vid_Prop
   {
      constexpr vid_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TMethod<TC, TCO>::vid_Prop & operator= (typename TMethod<TC, TCO>::vid_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } vid;
   struct type_Prop
   {
      constexpr type_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/MethodType operator= (/*0H*/MethodType v);
      /*regSet*/inline typename TMethod<TC, TCO>::type_Prop & operator= (typename TMethod<TC, TCO>::type_Prop & prop);
      /*regGet*/inline operator /*0I*/MethodType () const;
   } type;
   struct _class_Prop
   {
      constexpr _class_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TMethod<TC, TCO>::_class_Prop & operator= (typename TMethod<TC, TCO>::_class_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } _class;
   struct symbol_Prop
   {
      constexpr symbol_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TMethod<TC, TCO>::symbol_Prop & operator= (typename TMethod<TC, TCO>::symbol_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } symbol;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TMethod<TC, TCO>::dataTypeString_Prop & operator= (typename TMethod<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct memberAccess_Prop
   {
      constexpr memberAccess_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Access_Mode operator= (/*0H*/Access_Mode v);
      /*regSet*/inline typename TMethod<TC, TCO>::memberAccess_Prop & operator= (typename TMethod<TC, TCO>::memberAccess_Prop & prop);
      /*regGet*/inline operator /*0I*/Access_Mode () const;
   } memberAccess;
   ~TMethod()
   {
      this->impl = null; // How to know not to delete?
   }
};

enum class MethodType : int
{
   normalMethod = MethodType_normalMethod,
   virtualMethod = MethodType_virtualMethod
};

struct NameSpace : public TStruct<NameSpace>
{
public:
   C(NameSpace) impl;
   constexpr NameSpace() : impl({}) { }
   constexpr NameSpace(const C(NameSpace) impl) : impl(impl) { }

   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline NameSpace::name_Prop & operator= (NameSpace::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct depth_Prop
   {
      constexpr depth_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline NameSpace::depth_Prop & operator= (NameSpace::depth_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } depth;
   struct nameSpaces_Prop
   {
      constexpr nameSpaces_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline NameSpace::nameSpaces_Prop & operator= (NameSpace::nameSpaces_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } nameSpaces;
   struct classes_Prop
   {
      constexpr classes_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline NameSpace::classes_Prop & operator= (NameSpace::classes_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } classes;
   struct defines_Prop
   {
      constexpr defines_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline NameSpace::defines_Prop & operator= (NameSpace::defines_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } defines;
   struct functions_Prop
   {
      constexpr functions_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BinaryTree operator= (/*0H*/BinaryTree v);
      /*regSet*/inline NameSpace::functions_Prop & operator= (NameSpace::functions_Prop & prop);
      /*regGet*/inline operator /*0I*/BinaryTree () const;
   } functions;
};

template <class TC, C(Class) ** TCO>
class TObjectInfo : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ObjectInfo operator= (/*0H*/ObjectInfo & v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::prev_Prop & operator= (typename TObjectInfo<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/ObjectInfo () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ObjectInfo operator= (/*0H*/ObjectInfo & v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::next_Prop & operator= (typename TObjectInfo<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/ObjectInfo () const;
   } next;
   struct instance_Prop
   {
      constexpr instance_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Instance operator= (/*0H*/Instance v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::instance_Prop & operator= (typename TObjectInfo<TC, TCO>::instance_Prop & prop);
      /*regGet*/inline operator /*0I*/Instance () const;
   } instance;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::name_Prop & operator= (typename TObjectInfo<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } name;
   struct deleted_Prop
   {
      constexpr deleted_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::deleted_Prop & operator= (typename TObjectInfo<TC, TCO>::deleted_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } deleted;
   struct oClass_Prop
   {
      constexpr oClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/ObjectInfo operator= (/*0H*/ObjectInfo & v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::oClass_Prop & operator= (typename TObjectInfo<TC, TCO>::oClass_Prop & prop);
      /*regGet*/inline operator /*0I*/ObjectInfo () const;
   } oClass;
   struct instances_Prop
   {
      constexpr instances_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldList operator= (/*0H*/OldList v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::instances_Prop & operator= (typename TObjectInfo<TC, TCO>::instances_Prop & prop);
      /*regGet*/inline operator /*0I*/OldList () const;
   } instances;
   struct modified_Prop
   {
      constexpr modified_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::modified_Prop & operator= (typename TObjectInfo<TC, TCO>::modified_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } modified;
   struct i18nStrings_Prop
   {
      constexpr i18nStrings_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TObjectInfo<TC, TCO>::i18nStrings_Prop & operator= (typename TObjectInfo<TC, TCO>::i18nStrings_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } i18nStrings;
   ~TObjectInfo()
   {
      this->impl = null; // How to know not to delete?
   }
};

enum class Platform : int
{
   unknown = Platform_unknown,
   win32 = Platform_win32,
   tux = Platform_tux,
   apple = Platform_apple
};

template <class TC, C(Class) ** TCO>
class TProperty : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Property operator= (/*0H*/Property & v);
      /*regSet*/inline typename TProperty<TC, TCO>::prev_Prop & operator= (typename TProperty<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/Property () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Property operator= (/*0H*/Property & v);
      /*regSet*/inline typename TProperty<TC, TCO>::next_Prop & operator= (typename TProperty<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/Property () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TProperty<TC, TCO>::name_Prop & operator= (typename TProperty<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } name;
   struct isProperty_Prop
   {
      constexpr isProperty_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TProperty<TC, TCO>::isProperty_Prop & operator= (typename TProperty<TC, TCO>::isProperty_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isProperty;
   struct memberAccess_Prop
   {
      constexpr memberAccess_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Access_Mode operator= (/*0H*/Access_Mode v);
      /*regSet*/inline typename TProperty<TC, TCO>::memberAccess_Prop & operator= (typename TProperty<TC, TCO>::memberAccess_Prop & prop);
      /*regGet*/inline operator /*0I*/Access_Mode () const;
   } memberAccess;
   struct id_Prop
   {
      constexpr id_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TProperty<TC, TCO>::id_Prop & operator= (typename TProperty<TC, TCO>::id_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } id;
   struct _class_Prop
   {
      constexpr _class_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TProperty<TC, TCO>::_class_Prop & operator= (typename TProperty<TC, TCO>::_class_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } _class;
   struct dataTypeString_Prop
   {
      constexpr dataTypeString_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TProperty<TC, TCO>::dataTypeString_Prop & operator= (typename TProperty<TC, TCO>::dataTypeString_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } dataTypeString;
   struct dataTypeClass_Prop
   {
      constexpr dataTypeClass_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Class operator= (/*0H*/Class & v);
      /*regSet*/inline typename TProperty<TC, TCO>::dataTypeClass_Prop & operator= (typename TProperty<TC, TCO>::dataTypeClass_Prop & prop);
      /*regGet*/inline operator /*0I*/Class () const;
   } dataTypeClass;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TProperty<TC, TCO>::data_Prop & operator= (typename TProperty<TC, TCO>::data_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } data;
   struct symbol_Prop
   {
      constexpr symbol_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TProperty<TC, TCO>::symbol_Prop & operator= (typename TProperty<TC, TCO>::symbol_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } symbol;
   struct vid_Prop
   {
      constexpr vid_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TProperty<TC, TCO>::vid_Prop & operator= (typename TProperty<TC, TCO>::vid_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } vid;
   struct conversion_Prop
   {
      constexpr conversion_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TProperty<TC, TCO>::conversion_Prop & operator= (typename TProperty<TC, TCO>::conversion_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } conversion;
   struct watcherOffset_Prop
   {
      constexpr watcherOffset_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline typename TProperty<TC, TCO>::watcherOffset_Prop & operator= (typename TProperty<TC, TCO>::watcherOffset_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } watcherOffset;
   struct category_Prop
   {
      constexpr category_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline typename TProperty<TC, TCO>::category_Prop & operator= (typename TProperty<TC, TCO>::category_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } category;
   struct compiled_Prop
   {
      constexpr compiled_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TProperty<TC, TCO>::compiled_Prop & operator= (typename TProperty<TC, TCO>::compiled_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } compiled;
   struct selfWatchable_Prop
   {
      constexpr selfWatchable_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TProperty<TC, TCO>::selfWatchable_Prop & operator= (typename TProperty<TC, TCO>::selfWatchable_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } selfWatchable;
   struct isWatchable_Prop
   {
      constexpr isWatchable_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline typename TProperty<TC, TCO>::isWatchable_Prop & operator= (typename TProperty<TC, TCO>::isWatchable_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } isWatchable;
   ~TProperty()
   {
      this->impl = null; // How to know not to delete?
   }
};


#define REG_SerialBuffer(c) \
      SerialBuffer::class_registration(_cpp_class); \
      REG_IOChannel_readData(readData, c); \
      REG_IOChannel_writeData(writeData, c);

#define SERIALBUFFER_VIRTUAL_METHODS_PROTO(c) \

#define SERIALBUFFER_VIRTUAL_METHODS(c) \

class SerialBuffer : public IOChannel
{
public:
   inline SerialBuffer(SerialBuffer && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline SerialBuffer & operator= (SerialBuffer && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   SerialBuffer() : SerialBuffer((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<SerialBuffer> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         SerialBuffer * inst = new SerialBuffer(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      SerialBuffer * inst = (SerialBuffer *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(SerialBuffer & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline SerialBuffer(C(Instance) _impl, CPPClass & cl = _cpp_class) : IOChannel(_impl, cl) { }

   inline void free(); // SerialBuffer_free

   static void class_registration(CPPClass & _cpp_class);

   inline SerialBuffer(unsigned char * _buffer, uintsize count, uintsize _size, uintsize pos, unsigned char * buffer, uint size);

   struct buffer_Prop
   {
      constexpr buffer_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/unsigned char * operator= (/*0H*/unsigned char * v);
      /*regSet*/inline SerialBuffer::buffer_Prop & operator= (SerialBuffer::buffer_Prop & prop);
      /*regGet*/inline operator /*0I*/unsigned char * () const;
   } buffer;
   struct size_Prop
   {
      constexpr size_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline SerialBuffer::size_Prop & operator= (SerialBuffer::size_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } size;

   struct _buffer_Prop
   {
      constexpr _buffer_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/unsigned char * operator= (/*0H*/unsigned char * v);
      /*regSet*/inline SerialBuffer::_buffer_Prop & operator= (SerialBuffer::_buffer_Prop & prop);
      /*regGet*/inline operator /*0I*/unsigned char * () const;
   } _buffer;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uintsize operator= (/*0H*/uintsize v);
      /*regSet*/inline SerialBuffer::count_Prop & operator= (SerialBuffer::count_Prop & prop);
      /*regGet*/inline operator /*0I*/uintsize () const;
   } count;
   struct _size_Prop
   {
      constexpr _size_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uintsize operator= (/*0H*/uintsize v);
      /*regSet*/inline SerialBuffer::_size_Prop & operator= (SerialBuffer::_size_Prop & prop);
      /*regGet*/inline operator /*0I*/uintsize () const;
   } _size;
   struct pos_Prop
   {
      constexpr pos_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uintsize operator= (/*0H*/uintsize v);
      /*regSet*/inline SerialBuffer::pos_Prop & operator= (SerialBuffer::pos_Prop & prop);
      /*regGet*/inline operator /*0I*/uintsize () const;
   } pos;
};

struct StaticString : public TStruct<StaticString>
{
public:
   C(StaticString) impl;
   constexpr StaticString() : impl({}) { }
   constexpr StaticString(const C(StaticString) impl) : impl(impl) { }
};

template <class TC, C(Class) ** TCO>
class TSubModule : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/SubModule operator= (/*0H*/SubModule & v);
      /*regSet*/inline typename TSubModule<TC, TCO>::prev_Prop & operator= (typename TSubModule<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/SubModule () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/SubModule operator= (/*0H*/SubModule & v);
      /*regSet*/inline typename TSubModule<TC, TCO>::next_Prop & operator= (typename TSubModule<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/SubModule () const;
   } next;
   struct module_Prop
   {
      constexpr module_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Module operator= (/*0H*/Module v);
      /*regSet*/inline typename TSubModule<TC, TCO>::module_Prop & operator= (typename TSubModule<TC, TCO>::module_Prop & prop);
      /*regGet*/inline operator /*0I*/Module () const;
   } module;
   struct importMode_Prop
   {
      constexpr importMode_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Access_Mode operator= (/*0H*/Access_Mode v);
      /*regSet*/inline typename TSubModule<TC, TCO>::importMode_Prop & operator= (typename TSubModule<TC, TCO>::importMode_Prop & prop);
      /*regGet*/inline operator /*0I*/Access_Mode () const;
   } importMode;
   ~TSubModule()
   {
      this->impl = null; // How to know not to delete?
   }
};

enum class TemplateMemberType : int
{
   dataMember = TemplateMemberType_dataMember,
   method = TemplateMemberType_method,
   prop = TemplateMemberType_prop
};

enum class TemplateParameterType : int
{
   type = TemplateParameterType_type,
   identifier = TemplateParameterType_identifier,
   expression = TemplateParameterType_expression
};

struct BinaryTree : public TStruct<BinaryTree>
{
public:
   C(BinaryTree) impl;
   constexpr BinaryTree() : impl({}) { }
   constexpr BinaryTree(const C(BinaryTree) impl) : impl(impl) { }

   inline C(bool) add(/*1Ab*/BTNode node); // BinaryTree_add
   inline C(bool) check(); // BinaryTree_check
   inline int compareInt(/*1Ab*/uintptr a, /*1Ab*/uintptr b); // BinaryTree_compareInt
   inline int compareString(/*1Ab*/const char * a, /*1Ab*/const char * b); // BinaryTree_compareString
   inline void _delete(/*1Ab*/BTNode node); // BinaryTree_delete
   inline C(BTNode) * find(/*1Ab*/uintptr key); // BinaryTree_find
   inline C(BTNode) * findAll(/*1Ab*/uintptr key); // BinaryTree_findAll
   inline C(BTNode) * findPrefix(/*1Ab*/const char * key); // BinaryTree_findPrefix
   inline C(BTNode) * findString(/*1Ab*/const char * key); // BinaryTree_findString
   inline void free(); // BinaryTree_free
   static inline void freeString(/*1Ab*/char * string); // BinaryTree_freeString
   inline char * print(/*1Ab*/char * output, /*1Ab*/TreePrintStyle tps); // BinaryTree_print
   inline void remove(/*1Ab*/BTNode node); // BinaryTree_remove

   struct first_Prop
   {
      constexpr first_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } first;
   struct last_Prop
   {
      constexpr last_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } last;

   struct root_Prop
   {
      constexpr root_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNode operator= (/*0H*/BTNode & v);
      /*regSet*/inline BinaryTree::root_Prop & operator= (BinaryTree::root_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNode () const;
   } root;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline BinaryTree::count_Prop & operator= (BinaryTree::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
};

template <class TC, C(Class) ** TCO>
class TBTNode : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   inline C(BTNode) * findPrefix(/*1Ab*/const char * key); // BTNode_findPrefix
   inline C(BTNode) * findString(/*1Ab*/const char * key); // BTNode_findString

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } next;
   struct minimum_Prop
   {
      constexpr minimum_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } minimum;
   struct maximum_Prop
   {
      constexpr maximum_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } maximum;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } count;
   struct depthProp_Prop
   {
      constexpr depthProp_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } depthProp;

   struct key_Prop
   {
      constexpr key_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uintptr operator= (/*0H*/uintptr v);
      /*regSet*/inline typename TBTNode<TC, TCO>::key_Prop & operator= (typename TBTNode<TC, TCO>::key_Prop & prop);
      /*regGet*/inline operator /*0I*/uintptr () const;
   } key;
   struct parent_Prop
   {
      constexpr parent_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNode operator= (/*0H*/BTNode & v);
      /*regSet*/inline typename TBTNode<TC, TCO>::parent_Prop & operator= (typename TBTNode<TC, TCO>::parent_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNode () const;
   } parent;
   struct left_Prop
   {
      constexpr left_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNode operator= (/*0H*/BTNode & v);
      /*regSet*/inline typename TBTNode<TC, TCO>::left_Prop & operator= (typename TBTNode<TC, TCO>::left_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNode () const;
   } left;
   struct right_Prop
   {
      constexpr right_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNode operator= (/*0H*/BTNode & v);
      /*regSet*/inline typename TBTNode<TC, TCO>::right_Prop & operator= (typename TBTNode<TC, TCO>::right_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNode () const;
   } right;
   struct depth_Prop
   {
      constexpr depth_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TBTNode<TC, TCO>::depth_Prop & operator= (typename TBTNode<TC, TCO>::depth_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } depth;
   ~TBTNode()
   {
      this->impl = null; // How to know not to delete?
   }
};

class EscapeCStringOptions
{
public:
   C(EscapeCStringOptions) impl;
   constexpr EscapeCStringOptions() : impl(0) { }
   constexpr EscapeCStringOptions(C(EscapeCStringOptions) impl) : impl(impl) { }
   EscapeCStringOptions(bool escapeSingleQuote, bool escapeDoubleQuotes, bool writeQuotes, bool multiLine, int indent);
   operator C(EscapeCStringOptions)() { return impl; }
   EscapeCStringOptions & operator =(C(EscapeCStringOptions) impl) { impl = impl; return *this; }
   bool operator ==(const EscapeCStringOptions & value) const { return impl == value.impl; }
   bool operator !=(const EscapeCStringOptions & value) const { return impl != value.impl; }

   struct escapeSingleQuote_Prop
   {
      constexpr escapeSingleQuote_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline EscapeCStringOptions::escapeSingleQuote_Prop & operator= (EscapeCStringOptions::escapeSingleQuote_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } escapeSingleQuote;
   struct escapeDoubleQuotes_Prop
   {
      constexpr escapeDoubleQuotes_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline EscapeCStringOptions::escapeDoubleQuotes_Prop & operator= (EscapeCStringOptions::escapeDoubleQuotes_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } escapeDoubleQuotes;
   struct writeQuotes_Prop
   {
      constexpr writeQuotes_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline EscapeCStringOptions::writeQuotes_Prop & operator= (EscapeCStringOptions::writeQuotes_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } writeQuotes;
   struct multiLine_Prop
   {
      constexpr multiLine_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline EscapeCStringOptions::multiLine_Prop & operator= (EscapeCStringOptions::multiLine_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } multiLine;
   struct indent_Prop
   {
      constexpr indent_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline EscapeCStringOptions::indent_Prop & operator= (EscapeCStringOptions::indent_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } indent;
};

template <class TC, C(Class) ** TCO>
class TItem : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   inline void copy(/*1Ab*/TItem<TC, TCO> src, /*1Ab*/int size); // Item_copy

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Item operator= (/*0H*/Item & v);
      /*regSet*/inline typename TItem<TC, TCO>::prev_Prop & operator= (typename TItem<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/Item () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/Item operator= (/*0H*/Item & v);
      /*regSet*/inline typename TItem<TC, TCO>::next_Prop & operator= (typename TItem<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/Item () const;
   } next;
   ~TItem()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TMutex : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   inline void release(); // Mutex_release
   inline void wait(); // Mutex_wait

   struct lockCount_Prop
   {
      constexpr lockCount_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int () const;
   } lockCount;
   struct owningThread_Prop
   {
      constexpr owningThread_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/int64 () const;
   } owningThread;
   ~TMutex()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TNamedItem : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/NamedItem operator= (/*0H*/NamedItem & v);
      /*regSet*/inline typename TNamedItem<TC, TCO>::prev_Prop & operator= (typename TNamedItem<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/NamedItem () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/NamedItem operator= (/*0H*/NamedItem & v);
      /*regSet*/inline typename TNamedItem<TC, TCO>::next_Prop & operator= (typename TNamedItem<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/NamedItem () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline typename TNamedItem<TC, TCO>::name_Prop & operator= (typename TNamedItem<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } name;
   ~TNamedItem()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TNamedLink : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/NamedLink operator= (/*0H*/NamedLink & v);
      /*regSet*/inline typename TNamedLink<TC, TCO>::prev_Prop & operator= (typename TNamedLink<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/NamedLink () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/NamedLink operator= (/*0H*/NamedLink & v);
      /*regSet*/inline typename TNamedLink<TC, TCO>::next_Prop & operator= (typename TNamedLink<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/NamedLink () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline typename TNamedLink<TC, TCO>::name_Prop & operator= (typename TNamedLink<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } name;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TNamedLink<TC, TCO>::data_Prop & operator= (typename TNamedLink<TC, TCO>::data_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } data;
   ~TNamedLink()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TNamedLink64 : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/NamedLink64 operator= (/*0H*/NamedLink64 & v);
      /*regSet*/inline typename TNamedLink64<TC, TCO>::prev_Prop & operator= (typename TNamedLink64<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/NamedLink64 () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/NamedLink64 operator= (/*0H*/NamedLink64 & v);
      /*regSet*/inline typename TNamedLink64<TC, TCO>::next_Prop & operator= (typename TNamedLink64<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/NamedLink64 () const;
   } next;
   struct name_Prop
   {
      constexpr name_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline typename TNamedLink64<TC, TCO>::name_Prop & operator= (typename TNamedLink64<TC, TCO>::name_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } name;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int64 operator= (/*0H*/int64 v);
      /*regSet*/inline typename TNamedLink64<TC, TCO>::data_Prop & operator= (typename TNamedLink64<TC, TCO>::data_Prop & prop);
      /*regGet*/inline operator /*0I*/int64 () const;
   } data;
   ~TNamedLink64()
   {
      this->impl = null; // How to know not to delete?
   }
};

template <class TC, C(Class) ** TCO>
class TOldLink : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   inline void free(); // OldLink_free

   struct prev_Prop
   {
      constexpr prev_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldLink operator= (/*0H*/OldLink & v);
      /*regSet*/inline typename TOldLink<TC, TCO>::prev_Prop & operator= (typename TOldLink<TC, TCO>::prev_Prop & prop);
      /*regGet*/inline operator /*0I*/OldLink () const;
   } prev;
   struct next_Prop
   {
      constexpr next_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/OldLink operator= (/*0H*/OldLink & v);
      /*regSet*/inline typename TOldLink<TC, TCO>::next_Prop & operator= (typename TOldLink<TC, TCO>::next_Prop & prop);
      /*regGet*/inline operator /*0I*/OldLink () const;
   } next;
   struct data_Prop
   {
      constexpr data_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline typename TOldLink<TC, TCO>::data_Prop & operator= (typename TOldLink<TC, TCO>::data_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } data;
   ~TOldLink()
   {
      this->impl = null; // How to know not to delete?
   }
};

struct OldList : public TStruct<OldList>
{
public:
   C(OldList) impl;
   constexpr OldList() : impl({}) { }
   constexpr OldList(const C(OldList) impl) : impl(impl) { }

   inline void add(/*1Ab*/void * item); // OldList_add
   inline C(bool) addName(/*1Ab*/void * item); // OldList_addName
   inline void clear(); // OldList_clear
   inline void copy(/*1Ab*/OldList & src, /*1Ab*/int size, void (* copy)(void * dest, void * src)); // OldList_copy
   inline void copy(/*1Ac*/OldList * src, /*1Ac*/int size, void (* copy)(void * dest, void * src)); // OldList_copy
   inline void _delete(/*1Ab*/void * item); // OldList_delete
   inline C(OldLink) * findLink(/*1Ab*/void * data); // OldList_findLink
   inline void * findName(/*1Ab*/const char * name, /*1Ab*/bool warn); // OldList_findName
   inline void * findNamedLink(/*1Ab*/const char * name, /*1Ab*/bool warn); // OldList_findNamedLink
   inline void free(void (* freeFn)(void *)); // OldList_free
   inline C(bool) insert(/*1Ab*/void * prevItem, /*1Ab*/void * item); // OldList_insert
   inline void move(/*1Ab*/void * item, /*1Ab*/void * prevItem); // OldList_move
   inline C(bool) placeName(/*1Ab*/const char * name, /*1Ab*/void ** place); // OldList_placeName
   inline void remove(/*1Ab*/void * item); // OldList_remove
   inline void removeAll(void (* freeFn)(void *)); // OldList_removeAll
   inline void sort(int (* compare)(void *, void *, void *), /*1Ab*/void * data); // OldList_sort
   inline void swap(/*1Ab*/void * item1, /*1Ab*/void * item2); // OldList_swap

   inline OldList(void * first, void * last, int count, uint offset, bool circ);

   struct first_Prop
   {
      constexpr first_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline OldList::first_Prop & operator= (OldList::first_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } first;
   struct last_Prop
   {
      constexpr last_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/void * operator= (/*0H*/void * v);
      /*regSet*/inline OldList::last_Prop & operator= (OldList::last_Prop & prop);
      /*regGet*/inline operator /*0I*/void * () const;
   } last;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline OldList::count_Prop & operator= (OldList::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
   struct offset_Prop
   {
      constexpr offset_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/uint operator= (/*0H*/uint v);
      /*regSet*/inline OldList::offset_Prop & operator= (OldList::offset_Prop & prop);
      /*regGet*/inline operator /*0I*/uint () const;
   } offset;
   struct circ_Prop
   {
      constexpr circ_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/bool operator= (/*0H*/bool v);
      /*regSet*/inline OldList::circ_Prop & operator= (OldList::circ_Prop & prop);
      /*regGet*/inline operator /*0I*/bool () const;
   } circ;
};

enum class StringAllocType : int
{
   pointer = StringAllocType_pointer,
   stack = StringAllocType_stack,
   heap = StringAllocType_heap
};

template <class TC, C(Class) ** TCO>
class TStringBTNode : public TNHInstance<TC, TCO>
{
public:
   using TNHInstance<TC, TCO>::TNHInstance;

   struct key_Prop
   {
      constexpr key_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline typename TStringBTNode<TC, TCO>::key_Prop & operator= (typename TStringBTNode<TC, TCO>::key_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } key;
   struct parent_Prop
   {
      constexpr parent_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/StringBTNode operator= (/*0H*/StringBTNode & v);
      /*regSet*/inline typename TStringBTNode<TC, TCO>::parent_Prop & operator= (typename TStringBTNode<TC, TCO>::parent_Prop & prop);
      /*regGet*/inline operator /*0I*/StringBTNode () const;
   } parent;
   struct left_Prop
   {
      constexpr left_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/StringBTNode operator= (/*0H*/StringBTNode & v);
      /*regSet*/inline typename TStringBTNode<TC, TCO>::left_Prop & operator= (typename TStringBTNode<TC, TCO>::left_Prop & prop);
      /*regGet*/inline operator /*0I*/StringBTNode () const;
   } left;
   struct right_Prop
   {
      constexpr right_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/StringBTNode operator= (/*0H*/StringBTNode & v);
      /*regSet*/inline typename TStringBTNode<TC, TCO>::right_Prop & operator= (typename TStringBTNode<TC, TCO>::right_Prop & prop);
      /*regGet*/inline operator /*0I*/StringBTNode () const;
   } right;
   struct depth_Prop
   {
      constexpr depth_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline typename TStringBTNode<TC, TCO>::depth_Prop & operator= (typename TStringBTNode<TC, TCO>::depth_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } depth;
   ~TStringBTNode()
   {
      this->impl = null; // How to know not to delete?
   }
};

struct StringBinaryTree : public TStruct<StringBinaryTree>
{
public:
   C(StringBinaryTree) impl;
   constexpr StringBinaryTree() : impl({}) { }
   constexpr StringBinaryTree(const C(StringBinaryTree) impl) : impl(impl) { }

   inline C(bool) add(/*1Ab*/BTNode node); // BinaryTree_add
   inline C(bool) check(); // BinaryTree_check
   inline int compareInt(/*1Ab*/uintptr a, /*1Ab*/uintptr b); // BinaryTree_compareInt
   inline int compareString(/*1Ab*/const char * a, /*1Ab*/const char * b); // BinaryTree_compareString
   inline void _delete(/*1Ab*/BTNode node); // BinaryTree_delete
   inline C(BTNode) * find(/*1Ab*/uintptr key); // BinaryTree_find
   inline C(BTNode) * findAll(/*1Ab*/uintptr key); // BinaryTree_findAll
   inline C(BTNode) * findPrefix(/*1Ab*/const char * key); // BinaryTree_findPrefix
   inline C(BTNode) * findString(/*1Ab*/const char * key); // BinaryTree_findString
   inline void free(); // BinaryTree_free
   static inline void freeString(/*1Ab*/char * string); // BinaryTree_freeString
   inline char * print(/*1Ab*/char * output, /*1Ab*/TreePrintStyle tps); // BinaryTree_print
   inline void remove(/*1Ab*/BTNode node); // BinaryTree_remove

   struct first_Prop
   {
      constexpr first_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } first;
   struct last_Prop
   {
      constexpr last_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*regGet*/inline operator /*0I*/BTNode () const;
   } last;

   struct root_Prop
   {
      constexpr root_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/BTNode operator= (/*0H*/BTNode & v);
      /*regSet*/inline StringBinaryTree::root_Prop & operator= (StringBinaryTree::root_Prop & prop);
      /*regGet*/inline operator /*0I*/BTNode () const;
   } root;
   struct count_Prop
   {
      constexpr count_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline StringBinaryTree::count_Prop & operator= (StringBinaryTree::count_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } count;
};

enum class TreePrintStyle : int
{
   inOrder = TreePrintStyle_inOrder,
   postOrder = TreePrintStyle_postOrder,
   preOrder = TreePrintStyle_preOrder,
   depthOrder = TreePrintStyle_depthOrder
};


#define REG_ZString(c) \
      ZString::class_registration(_cpp_class);

#define ZSTRING_VIRTUAL_METHODS_PROTO(c) \

#define ZSTRING_VIRTUAL_METHODS(c) \

class ZString : public Instance
{
public:
   inline operator constString() const;
   inline ZString(const char * s);
   inline ZString & operator =(constString s);
   inline ZString(ZString && i)
   {
      Instance * self = (Instance *)this;
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
   }
   inline ZString & operator= (ZString && i)
   {
      Instance * self = (Instance *)this;
      if(self->impl)
      {
         C(Instance) impl = self->impl;
         int refCount = impl->_refCount;
         Instance_decRef(impl);
         if(refCount > 1)
         {
            Instance ** inst = (Instance **)&INSTANCEL(impl, impl->_class);
            if(inst && *inst == self)
               *inst = null;
         }
      }
      self->impl = i.impl;
      self->vTbl = i.vTbl;
      self->mustFree = i.mustFree; /* checking: should this be in all these instances? */
      i.impl = null;
      i.vTbl = null;
      return *this;
   }
   ZString() : ZString((C(Instance))Instance_newEx(_cpp_class.impl, false), _cpp_class) { }
   struct Instance_onCompare_Functor
   {
      [[no_unique_address]] int _[0];
      typedef int (* FunctionType)(Instance & , /*6Bj*/Instance & object);
      inline FunctionType operator= (FunctionType func);
      inline int operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object);
   } onCompare;
   // inline static void register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)

   struct Instance_onCopy_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Bj*/Instance & newData);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData);
   } onCopy;
   // inline static void register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)

   struct Instance_onDisplay_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags);
   } onDisplay;
   // inline static void register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)

   struct Instance_onEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef Instance & (* FunctionType)(Instance & , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
      inline FunctionType operator= (FunctionType func);
      inline Instance & operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData);
   } onEdit;
   // inline static void register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)

   struct Instance_onFree_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance &);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ );
   } onFree;
   // inline static void register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)

   struct Instance_onGetDataFromString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Fj*/const char * string);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string);
   } onGetDataFromString;
   // inline static void register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)

   struct Instance_onGetString_Functor
   {
      [[no_unique_address]] int _[0];
      typedef const char * (* FunctionType)(Instance & , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
      inline FunctionType operator= (FunctionType func);
      inline const char * operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType);
   } onGetString;
   // inline static void register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)

   struct Instance_onSaveEdit_Functor
   {
      [[no_unique_address]] int _[0];
      typedef bool (* FunctionType)(Instance & , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
      inline FunctionType operator= (FunctionType func);
      inline bool operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object);
   } onSaveEdit;
   // inline static void register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)

   struct Instance_onSerialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onSerialize;
   // inline static void register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)

   struct Instance_onUnserialize_Functor
   {
      [[no_unique_address]] int _[0];
      typedef void (* FunctionType)(Instance & , /*6Aj*/Instance /*IOChannel*/ & channel);
      inline FunctionType operator= (FunctionType func);
      inline void operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel);
   } onUnserialize;
   // inline static void register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)

   static TCPPClass<ZString> _cpp_class;
   static C(bool) constructor(C(Instance) i, C(bool) alloc)
   {
      if(alloc && !INSTANCEL(i, i->_class))
      {
         ZString * inst = new ZString(i, _cpp_class);
         if(inst)
         {
            /* printf("Must free!\n");*/
            inst->mustFree = true;
         }
         return inst != null;
      }
      return true;
   }
   static void destructor(C(Instance) i)
   {
      ZString * inst = (ZString *)INSTANCEL(i, i->_class);
      if(inst)
      {
         if(_cpp_class.destructor)
            ((void (*)(ZString & self))_cpp_class.destructor)(*inst);
         if(inst->mustFree)
            delete inst;
      }
   }
   explicit inline ZString(C(Instance) _impl, CPPClass & cl = _cpp_class) : Instance(_impl, cl) { }

   inline void concat(/*1Ab*/ZString & s); // ZString_concat
   template<typename... Args> inline void concatf(/*1Ab*/const char * format, /*1Ab*/Args... args); // ZString_concatf
   inline void concatn(/*1Ab*/ZString & s, /*1Ab*/int l); // ZString_concatn
   template <typename ...Ts> inline void concatx(/*1Ab*/const Ts&... ts); // ZString_concatx
   inline void copy(/*1Ab*/ZString & s); // ZString_copy
   inline void copyString(/*1Ab*/const char * value, /*1Ab*/int newLen); // ZString_copyString

   static void class_registration(CPPClass & _cpp_class);

   inline ZString(char * _string, int len, StringAllocType allocType, int size, int minSize, int maxSize, const char * string);

   struct string_Prop
   {
      constexpr string_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/const char * operator= (/*0H*/const char * v);
      /*regSet*/inline ZString::string_Prop & operator= (ZString::string_Prop & prop);
      /*regGet*/inline operator /*0I*/const char * () const;
   } string;

   struct _string_Prop
   {
      constexpr _string_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/char * operator= (/*0H*/char * v);
      /*regSet*/inline ZString::_string_Prop & operator= (ZString::_string_Prop & prop);
      /*regGet*/inline operator /*0I*/char * () const;
   } _string;
   struct len_Prop
   {
      constexpr len_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline ZString::len_Prop & operator= (ZString::len_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } len;
   struct allocType_Prop
   {
      constexpr allocType_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/StringAllocType operator= (/*0H*/StringAllocType v);
      /*regSet*/inline ZString::allocType_Prop & operator= (ZString::allocType_Prop & prop);
      /*regGet*/inline operator /*0I*/StringAllocType () const;
   } allocType;
   struct size_Prop
   {
      constexpr size_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline ZString::size_Prop & operator= (ZString::size_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } size;
   struct minSize_Prop
   {
      constexpr minSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline ZString::minSize_Prop & operator= (ZString::minSize_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } minSize;
   struct maxSize_Prop
   {
      constexpr maxSize_Prop() :_() { };
      [[no_unique_address]] int _[0];
      /*nstSet*/inline /*0H*/int operator= (/*0H*/int v);
      /*regSet*/inline ZString::maxSize_Prop & operator= (ZString::maxSize_Prop & prop);
      /*regGet*/inline operator /*0I*/int () const;
   } maxSize;
};

template<typename TTT> C(Class) * class_of(char v) { return CO(char); };
template<typename TTT> C(Class) * class_of(short v) { return CO(short); };
template<typename TTT> C(Class) * class_of(int v) { return CO(int); };
template<typename TTT> C(Class) * class_of(int64 v) { return CO(int64); };
template<typename TTT> C(Class) * class_of(uint v) { return CO(uint); };
template<typename TTT> C(Class) * class_of(uint64 v) { return CO(uint64); };
template<typename TTT> C(Class) * class_of(float v) { return CO(float); };
template<typename TTT> C(Class) * class_of(double v) { return CO(double); };
template<typename TTT> C(Class) * class_of(const char * v) { return CO(String); };
template<typename TTT> C(Class) * class_of(char * v) { return CO(String); };
template<typename TTT> C(Class) * class_of(const Instance & v) { return v.impl ? v.impl->_class : v._cpp_class.impl; };

template<typename TTT> inline typename std::enable_if<
   std::is_base_of<Struct, typename std::remove_reference<TTT>::type>::value, C(Class) *>::type
   class_of(const TTT & v)
{
    return std::remove_reference_t<TTT>::_class;
};

#define classof(x) class_of<decltype(x)>((x))

template <typename TTT, C(Class) ** TCO> inline const void * vapass(TNHInstance<TTT, TCO> & f) { return f.impl; }

inline const void * vapass(const char * f) { return f; }
inline const void * vapass(const short & f) { return &f; }
inline const void * vapass(const int & f) { return &f; }
inline const void * vapass(const int64 & f) { return &f; }
inline const void * vapass(const uint & f) { return &f; }
inline const void * vapass(const uint64 & f) { return &f; }
inline const void * vapass(const float & f) { return &f; }
inline const void * vapass(const double & f) { return &f; }
inline const void * vapass(const Instance & f) { return f.impl; }
inline const void * vapass(const Struct & f) { return &f; }


template <typename TSC>
inline int TStruct<TSC>::Struct_onCompare_Functor::operator()( /*6Bj*/Instance & object)
{
   return Instance_onCompare(_class, this, /*7Al*/object.impl);
}

template <typename TSC>
inline void TStruct<TSC>::Struct_onCopy_Functor::operator()( /*6Bj*/Instance & newData)
{
   _onCopy(_class, this, /*7Al*/newData.impl);
}

template <typename TSC>
inline void TStruct<TSC>::Struct_onDisplay_Functor::operator()( /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags)
{
   _onDisplay(_class, this, /*7Al*/surface.impl, /*7Al*/x, /*7Al*/y, /*7Al*/width, /*7Al*/fieldData, /*7Al*/(C(Alignment))alignment, /*7Al*/displayFlags);
}

template <typename TSC>
inline Instance & TStruct<TSC>::Struct_onEdit_Functor::operator()( /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData)
{
   C(Instance) retInstance = Instance_onEdit(_class, this, /*7Al*/dataBox.impl, /*7Al*/obsolete.impl, /*7Al*/x, /*7Al*/y, /*7Al*/w, /*7Al*/h, /*7Al*/userData);
         return *(Instance *)INSTANCEL(retInstance, retInstance->_class);
}

template <typename TSC>
inline void TStruct<TSC>::Struct_onFree_Functor::operator()( )
{
   _onFree(_class, this);
}

template <typename TSC>
inline bool TStruct<TSC>::Struct_onGetDataFromString_Functor::operator()( /*6Fj*/const char * string)
{
   return (bool)Instance_onGetDataFromString(_class, this, /*7Al*/string);
}

template <typename TSC>
inline const char * TStruct<TSC>::Struct_onGetString_Functor::operator()( /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType)
{
   return Instance_onGetString(_class, this, /*7Al*/tempString, /*7Al*/reserved, /*7Al*/(C(ObjectNotationType) *)onType);
}

template <typename TSC>
inline bool TStruct<TSC>::Struct_onSaveEdit_Functor::operator()( /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object)
{
   return (bool)Instance_onSaveEdit(_class, this, /*7Al*/window.impl, /*7Al*/object);
}

template <typename TSC>
inline void TStruct<TSC>::Struct_onSerialize_Functor::operator()( /*6Aj*/Instance /*IOChannel*/ & channel)
{
   _onSerialize(_class, this, /*7Al*/channel.impl);
}

template <typename TSC>
inline void TStruct<TSC>::Struct_onUnserialize_Functor::operator()( /*6Aj*/Instance /*IOChannel*/ & channel)
{
   _onUnserialize(_class, this, /*7Al*/channel.impl);
}

//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
////                                                                        //// ////////////////////////
////    split implementations                                               //// ////////////////////////
////                                                                        //// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////


/////////////////////////////////////////////////////////// [ecereCOM]/ //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


////////////////////////////////////////////////////// [ecereCOM]/ecere //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


///////////////////////////////////////////////// [ecereCOM]/ecere::com //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


template <typename TP_AT> TCPPClass<TAVLTree<TP_AT>> TAVLTree<TP_AT>::_cpp_class;

template <typename TP_AT> void TAVLTree<TP_AT>::class_registration(CPPClass & _cpp_class)
{
}

inline Application::Application(int argc, const char ** argv = null, int exitCode = 0, bool isGUIApp = (bool)0, OldList allModules = OldList(), char * parsedCommand = null, NameSpace systemNameSpace = NameSpace()) : Application()
{
   this->argc = argc;
   this->argv = argv;
   this->exitCode = exitCode;
   this->isGUIApp = isGUIApp;
   this->allModules = allModules.impl;
   this->parsedCommand = parsedCommand;
   this->systemNameSpace = systemNameSpace.impl;
}

inline Application::Application_main_Functor::FunctionType Application::Application_main_Functor::operator= (FunctionType func)
{
   SELF(Application, main);
   if(self->vTbl == Application::_cpp_class.vTbl)
   {
      uint size = Application::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Application::Application_main_Functor::FunctionType, size);
      memcpy(self->vTbl, Application::_cpp_class.vTbl, sizeof(Application::Application_main_Functor::FunctionType) * size);
   }
   ((Application::Application_main_Functor::FunctionType *)self->vTbl)[M_VTBLID(Application, main)] = func;
   return func;
}
inline void Application::Application_main_Functor::operator()( )
{
   SELF(Application, main);
   Application_main(self ? self->impl : (C(Application))null);
}
// inline void Application::register_main(CPPClass & cl, Application::Application_main_Functor::FunctionType func)
// {
//    ((Application::Application_main_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Application, main)] = func;
// }



// member accessors: Application::argc
// (normal::int)

/*nstSet*/inline /*0H*/int Application::argc_Prop::operator= (/*0H*/int v)
{
   SELF(Application, argc);
   IPTR(self->impl, Application)->argc = v;
   return v;
}

/*regSet*/inline Application::argc_Prop & Application::argc_Prop::operator= (Application::argc_Prop & prop)
{
   SELF(Application, argc);
   /*0H*/int v = prop;
   IPTR(self->impl, Application)->argc = v;
   return prop;
}
/*regGet*/inline Application::argc_Prop::operator /*0I*/int () const
{
   SELF(Application, argc);
   return self ? IPTR(self->impl, Application)->argc : 0;
}

// member accessors: Application::argv
// (normal::const char **)

/*nstSet*/inline /*0H*/const char ** Application::argv_Prop::operator= (/*0H*/const char ** v)
{
   SELF(Application, argv);
   IPTR(self->impl, Application)->argv = v;
   return v;
}

/*regSet*/inline Application::argv_Prop & Application::argv_Prop::operator= (Application::argv_Prop & prop)
{
   SELF(Application, argv);
   /*0H*/const char ** v = prop;
   IPTR(self->impl, Application)->argv = v;
   return prop;
}
/*regGet*/inline Application::argv_Prop::operator /*0I*/const char ** () const
{
   SELF(Application, argv);
   return self ? IPTR(self->impl, Application)->argv : null;
}

// member accessors: Application::exitCode
// (normal::int)

/*nstSet*/inline /*0H*/int Application::exitCode_Prop::operator= (/*0H*/int v)
{
   SELF(Application, exitCode);
   IPTR(self->impl, Application)->exitCode = v;
   return v;
}

/*regSet*/inline Application::exitCode_Prop & Application::exitCode_Prop::operator= (Application::exitCode_Prop & prop)
{
   SELF(Application, exitCode);
   /*0H*/int v = prop;
   IPTR(self->impl, Application)->exitCode = v;
   return prop;
}
/*regGet*/inline Application::exitCode_Prop::operator /*0I*/int () const
{
   SELF(Application, exitCode);
   return self ? IPTR(self->impl, Application)->exitCode : 0;
}

// member accessors: Application::isGUIApp
// (normal::enum|uint)

/*nstSet*/inline /*0H*/bool Application::isGUIApp_Prop::operator= (/*0H*/bool v)
{
   SELF(Application, isGUIApp);
   IPTR(self->impl, Application)->isGUIApp = (C(bool))v;
   return v;
}

/*regSet*/inline Application::isGUIApp_Prop & Application::isGUIApp_Prop::operator= (Application::isGUIApp_Prop & prop)
{
   SELF(Application, isGUIApp);
   /*0H*/bool v = prop;
   IPTR(self->impl, Application)->isGUIApp = (C(bool))v;
   return prop;
}
/*regGet*/inline Application::isGUIApp_Prop::operator /*0I*/bool () const
{
   SELF(Application, isGUIApp);
   return self ? (bool)IPTR(self->impl, Application)->isGUIApp : (bool)0;
}

// member accessors: Application::allModules
// (normal::struct)

/*nstSet*/inline /*0H*/OldList Application::allModules_Prop::operator= (/*0H*/OldList v)
{
   SELF(Application, allModules);
   IPTR(self->impl, Application)->allModules = v.impl;
   return v;
}

/*regSet*/inline Application::allModules_Prop & Application::allModules_Prop::operator= (Application::allModules_Prop & prop)
{
   SELF(Application, allModules);
   /*0H*/OldList v = prop;
   IPTR(self->impl, Application)->allModules = v.impl;
   return prop;
}
/*regGet*/inline Application::allModules_Prop::operator /*0I*/OldList () const
{
   SELF(Application, allModules);
   OldList value(IPTR(self->impl, Application)->allModules);
   return value;
}

// member accessors: Application::parsedCommand
// (normal::char *)

/*nstSet*/inline /*0H*/char * Application::parsedCommand_Prop::operator= (/*0H*/char * v)
{
   SELF(Application, parsedCommand);
   IPTR(self->impl, Application)->parsedCommand = v;
   return v;
}

/*regSet*/inline Application::parsedCommand_Prop & Application::parsedCommand_Prop::operator= (Application::parsedCommand_Prop & prop)
{
   SELF(Application, parsedCommand);
   /*0H*/char * v = prop;
   IPTR(self->impl, Application)->parsedCommand = v;
   return prop;
}
/*regGet*/inline Application::parsedCommand_Prop::operator /*0I*/char * () const
{
   SELF(Application, parsedCommand);
   return self ? IPTR(self->impl, Application)->parsedCommand : null;
}

// member accessors: Application::systemNameSpace
// (normal::struct)

/*nstSet*/inline /*0H*/NameSpace Application::systemNameSpace_Prop::operator= (/*0H*/NameSpace v)
{
   SELF(Application, systemNameSpace);
   IPTR(self->impl, Application)->systemNameSpace = v.impl;
   return v;
}

/*regSet*/inline Application::systemNameSpace_Prop & Application::systemNameSpace_Prop::operator= (Application::systemNameSpace_Prop & prop)
{
   SELF(Application, systemNameSpace);
   /*0H*/NameSpace v = prop;
   IPTR(self->impl, Application)->systemNameSpace = v.impl;
   return prop;
}
/*regGet*/inline Application::systemNameSpace_Prop::operator /*0I*/NameSpace () const
{
   SELF(Application, systemNameSpace);
   NameSpace value(IPTR(self->impl, Application)->systemNameSpace);
   return value;
}

template <typename TP_T, typename TP_I, typename TP_D> inline TArray<TP_T, TP_I, TP_D>::TArray(TP(Container, T) * array, uint count, uint minAllocSize, uint size) : TArray<TP_T, TP_I, TP_D>()
{
   this->array = array;
   this->count = count;
   this->minAllocSize = minAllocSize;
   this->size = size;
}

template <typename TP_T, typename TP_I, typename TP_D> TCPPClass<TArray<TP_T, TP_I, TP_D>> TArray<TP_T, TP_I, TP_D>::_cpp_class;

template <typename TP_T, typename TP_I, typename TP_D> void TArray<TP_T, TP_I, TP_D>::class_registration(CPPClass & _cpp_class)
{
}


// property callers: Array::size
// (normal::uint)

/*nstSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline /*0H*/uint TArray<TP_T _ARG TP_I _ARG TP_D> ::size_Prop::operator= (/*0H*/uint v)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), size);
   Array_set_size(self ? self->impl : null, v);
   return v;
}

/*regSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline typename TArray<TP_T _ARG TP_I _ARG TP_D>::size_Prop & TArray<TP_T _ARG TP_I _ARG TP_D>::size_Prop::operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::size_Prop & prop)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), size);
   /*0H*/uint v = prop;
   Array_set_size(self ? self->impl : null, v);
   return prop;
}
/*regGet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline TArray<TP_T _ARG TP_I _ARG TP_D> ::size_Prop::operator /*0I*/uint () const
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), size);
   return Array_get_size(self ? self->impl : null);
}

// property callers: Array::minAllocSize
// (normal::uint)

/*nstSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline /*0H*/uint TArray<TP_T _ARG TP_I _ARG TP_D> ::minAllocSize_Prop::operator= (/*0H*/uint v)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), minAllocSize);
   Array_set_minAllocSize(self ? self->impl : null, v);
   return v;
}

/*regSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline typename TArray<TP_T _ARG TP_I _ARG TP_D>::minAllocSize_Prop & TArray<TP_T _ARG TP_I _ARG TP_D>::minAllocSize_Prop::operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::minAllocSize_Prop & prop)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), minAllocSize);
   /*0H*/uint v = prop;
   Array_set_minAllocSize(self ? self->impl : null, v);
   return prop;
}
/*regGet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline TArray<TP_T _ARG TP_I _ARG TP_D> ::minAllocSize_Prop::operator /*0I*/uint () const
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), minAllocSize);
   return self ? IPTR(self->impl, Array)->minAllocSize : 0;
}


// member accessors: Array::array
// (normal::T *)

/*nstSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline /*0H*/TP(Container, T) * TArray<TP_T _ARG TP_I _ARG TP_D> ::array_Prop::operator= (/*0H*/TP(Container, T) * v)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), array);
   IPTR(self->impl, Array)->array = v;
   return v;
}

/*regSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline typename TArray<TP_T _ARG TP_I _ARG TP_D>::array_Prop & TArray<TP_T _ARG TP_I _ARG TP_D>::array_Prop::operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::array_Prop & prop)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), array);
   /*0H*/TP(Container, T) * v = prop;
   IPTR(self->impl, Array)->array = v;
   return prop;
}
/*regGet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline TArray<TP_T _ARG TP_I _ARG TP_D> ::array_Prop::operator /*0I*/TP(Container, T) * () const
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), array);
   return self ? IPTR(self->impl, Array)->array : null;
}

// member accessors: Array::count
// (normal::uint)

/*nstSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline /*0H*/uint TArray<TP_T _ARG TP_I _ARG TP_D> ::count_Prop::operator= (/*0H*/uint v)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), count);
   IPTR(self->impl, Array)->count = v;
   return v;
}

/*regSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline typename TArray<TP_T _ARG TP_I _ARG TP_D>::count_Prop & TArray<TP_T _ARG TP_I _ARG TP_D>::count_Prop::operator= (TArray<TP_T _ARG TP_I _ARG TP_D>::count_Prop & prop)
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), count);
   /*0H*/uint v = prop;
   IPTR(self->impl, Array)->count = v;
   return prop;
}
/*regGet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline TArray<TP_T _ARG TP_I _ARG TP_D> ::count_Prop::operator /*0I*/uint () const
{
   TSELF(Array, (<TP_T _ARG TP_I _ARG TP_D>), count);
   return self ? IPTR(self->impl, Array)->count : 0;
}



// member accessors: BTNamedLink::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TBTNamedLink<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(BTNamedLink, name);
   ((C(BTNamedLink) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNamedLink<TC, TCO>::name_Prop & TBTNamedLink<TC, TCO>::name_Prop::operator= (typename TBTNamedLink<TC, TCO>::name_Prop & prop)
{
   SELF(BTNamedLink, name);
   /*0H*/const char * v = prop;
   ((C(BTNamedLink) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNamedLink<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TBTNamedLink TCTCO * self = CONTAINER_OF(this, TBTNamedLink TCTCO, name);
   return self ? ((C(BTNamedLink) *)self->impl)->name : null;
}

// member accessors: BTNamedLink::parent
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BTNamedLink TBTNamedLink<TC, TCO>::parent_Prop::operator= (/*0H*/BTNamedLink & v)
{
   SELF(BTNamedLink, parent);
   ((C(BTNamedLink) *)self->impl)->parent = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNamedLink<TC, TCO>::parent_Prop & TBTNamedLink<TC, TCO>::parent_Prop::operator= (typename TBTNamedLink<TC, TCO>::parent_Prop & prop)
{
   SELF(BTNamedLink, parent);
   /*0H*/BTNamedLink v = prop;
   ((C(BTNamedLink) *)self->impl)->parent = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNamedLink<TC, TCO>::parent_Prop::operator /*0I*/BTNamedLink () const
{
   __attribute__((unused)) TBTNamedLink TCTCO * self = CONTAINER_OF(this, TBTNamedLink TCTCO, parent);
   BTNamedLink value(((C(BTNamedLink) *)self->impl)->parent);
   return value;
}

// member accessors: BTNamedLink::left
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BTNamedLink TBTNamedLink<TC, TCO>::left_Prop::operator= (/*0H*/BTNamedLink & v)
{
   SELF(BTNamedLink, left);
   ((C(BTNamedLink) *)self->impl)->left = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNamedLink<TC, TCO>::left_Prop & TBTNamedLink<TC, TCO>::left_Prop::operator= (typename TBTNamedLink<TC, TCO>::left_Prop & prop)
{
   SELF(BTNamedLink, left);
   /*0H*/BTNamedLink v = prop;
   ((C(BTNamedLink) *)self->impl)->left = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNamedLink<TC, TCO>::left_Prop::operator /*0I*/BTNamedLink () const
{
   __attribute__((unused)) TBTNamedLink TCTCO * self = CONTAINER_OF(this, TBTNamedLink TCTCO, left);
   BTNamedLink value(((C(BTNamedLink) *)self->impl)->left);
   return value;
}

// member accessors: BTNamedLink::right
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BTNamedLink TBTNamedLink<TC, TCO>::right_Prop::operator= (/*0H*/BTNamedLink & v)
{
   SELF(BTNamedLink, right);
   ((C(BTNamedLink) *)self->impl)->right = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNamedLink<TC, TCO>::right_Prop & TBTNamedLink<TC, TCO>::right_Prop::operator= (typename TBTNamedLink<TC, TCO>::right_Prop & prop)
{
   SELF(BTNamedLink, right);
   /*0H*/BTNamedLink v = prop;
   ((C(BTNamedLink) *)self->impl)->right = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNamedLink<TC, TCO>::right_Prop::operator /*0I*/BTNamedLink () const
{
   __attribute__((unused)) TBTNamedLink TCTCO * self = CONTAINER_OF(this, TBTNamedLink TCTCO, right);
   BTNamedLink value(((C(BTNamedLink) *)self->impl)->right);
   return value;
}

// member accessors: BTNamedLink::depth
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TBTNamedLink<TC, TCO>::depth_Prop::operator= (/*0H*/int v)
{
   SELF(BTNamedLink, depth);
   ((C(BTNamedLink) *)self->impl)->depth = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNamedLink<TC, TCO>::depth_Prop & TBTNamedLink<TC, TCO>::depth_Prop::operator= (typename TBTNamedLink<TC, TCO>::depth_Prop & prop)
{
   SELF(BTNamedLink, depth);
   /*0H*/int v = prop;
   ((C(BTNamedLink) *)self->impl)->depth = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNamedLink<TC, TCO>::depth_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TBTNamedLink TCTCO * self = CONTAINER_OF(this, TBTNamedLink TCTCO, depth);
   return self ? ((C(BTNamedLink) *)self->impl)->depth : 0;
}

// member accessors: BTNamedLink::data
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TBTNamedLink<TC, TCO>::data_Prop::operator= (/*0H*/void * v)
{
   SELF(BTNamedLink, data);
   ((C(BTNamedLink) *)self->impl)->data = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNamedLink<TC, TCO>::data_Prop & TBTNamedLink<TC, TCO>::data_Prop::operator= (typename TBTNamedLink<TC, TCO>::data_Prop & prop)
{
   SELF(BTNamedLink, data);
   /*0H*/void * v = prop;
   ((C(BTNamedLink) *)self->impl)->data = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNamedLink<TC, TCO>::data_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TBTNamedLink TCTCO * self = CONTAINER_OF(this, TBTNamedLink TCTCO, data);
   return self ? ((C(BTNamedLink) *)self->impl)->data : null;
}



// member accessors: BitMember::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BitMember TBitMember<TC, TCO>::prev_Prop::operator= (/*0H*/BitMember & v)
{
   SELF(BitMember, prev);
   ((C(BitMember) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::prev_Prop & TBitMember<TC, TCO>::prev_Prop::operator= (typename TBitMember<TC, TCO>::prev_Prop & prop)
{
   SELF(BitMember, prev);
   /*0H*/BitMember v = prop;
   ((C(BitMember) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::prev_Prop::operator /*0I*/BitMember () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, prev);
   BitMember value(((C(BitMember) *)self->impl)->prev);
   return value;
}

// member accessors: BitMember::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BitMember TBitMember<TC, TCO>::next_Prop::operator= (/*0H*/BitMember & v)
{
   SELF(BitMember, next);
   ((C(BitMember) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::next_Prop & TBitMember<TC, TCO>::next_Prop::operator= (typename TBitMember<TC, TCO>::next_Prop & prop)
{
   SELF(BitMember, next);
   /*0H*/BitMember v = prop;
   ((C(BitMember) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::next_Prop::operator /*0I*/BitMember () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, next);
   BitMember value(((C(BitMember) *)self->impl)->next);
   return value;
}

// member accessors: BitMember::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TBitMember<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(BitMember, name);
   ((C(BitMember) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::name_Prop & TBitMember<TC, TCO>::name_Prop::operator= (typename TBitMember<TC, TCO>::name_Prop & prop)
{
   SELF(BitMember, name);
   /*0H*/const char * v = prop;
   ((C(BitMember) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, name);
   return self ? ((C(BitMember) *)self->impl)->name : null;
}

// member accessors: BitMember::isProperty
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TBitMember<TC, TCO>::isProperty_Prop::operator= (/*0H*/bool v)
{
   SELF(BitMember, isProperty);
   ((C(BitMember) *)self->impl)->isProperty = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::isProperty_Prop & TBitMember<TC, TCO>::isProperty_Prop::operator= (typename TBitMember<TC, TCO>::isProperty_Prop & prop)
{
   SELF(BitMember, isProperty);
   /*0H*/bool v = prop;
   ((C(BitMember) *)self->impl)->isProperty = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::isProperty_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, isProperty);
   return self ? ((C(BitMember) *)self->impl)->isProperty : (bool)0;
}

// member accessors: BitMember::memberAccess
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Access_Mode TBitMember<TC, TCO>::memberAccess_Prop::operator= (/*0H*/Access_Mode v)
{
   SELF(BitMember, memberAccess);
   ((C(BitMember) *)self->impl)->memberAccess = (C(AccessMode))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::memberAccess_Prop & TBitMember<TC, TCO>::memberAccess_Prop::operator= (typename TBitMember<TC, TCO>::memberAccess_Prop & prop)
{
   SELF(BitMember, memberAccess);
   /*0H*/Access_Mode v = prop;
   ((C(BitMember) *)self->impl)->memberAccess = (C(AccessMode))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::memberAccess_Prop::operator /*0I*/Access_Mode () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, memberAccess);
   return self ? ((C(BitMember) *)self->impl)->memberAccess : (Access_Mode)0;
}

// member accessors: BitMember::id
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TBitMember<TC, TCO>::id_Prop::operator= (/*0H*/int v)
{
   SELF(BitMember, id);
   ((C(BitMember) *)self->impl)->id = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::id_Prop & TBitMember<TC, TCO>::id_Prop::operator= (typename TBitMember<TC, TCO>::id_Prop & prop)
{
   SELF(BitMember, id);
   /*0H*/int v = prop;
   ((C(BitMember) *)self->impl)->id = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::id_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, id);
   return self ? ((C(BitMember) *)self->impl)->id : 0;
}

// member accessors: BitMember::_class
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TBitMember<TC, TCO>::_class_Prop::operator= (/*0H*/Class & v)
{
   SELF(BitMember, _class);
   ((C(BitMember) *)self->impl)->_class = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::_class_Prop & TBitMember<TC, TCO>::_class_Prop::operator= (typename TBitMember<TC, TCO>::_class_Prop & prop)
{
   SELF(BitMember, _class);
   /*0H*/Class v = prop;
   ((C(BitMember) *)self->impl)->_class = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::_class_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, _class);
   Class value(((C(BitMember) *)self->impl)->_class);
   return value;
}

// member accessors: BitMember::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TBitMember<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(BitMember, dataTypeString);
   ((C(BitMember) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::dataTypeString_Prop & TBitMember<TC, TCO>::dataTypeString_Prop::operator= (typename TBitMember<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(BitMember, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(BitMember) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, dataTypeString);
   return self ? ((C(BitMember) *)self->impl)->dataTypeString : null;
}

// member accessors: BitMember::dataTypeClass
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TBitMember<TC, TCO>::dataTypeClass_Prop::operator= (/*0H*/Class & v)
{
   SELF(BitMember, dataTypeClass);
   ((C(BitMember) *)self->impl)->dataTypeClass = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::dataTypeClass_Prop & TBitMember<TC, TCO>::dataTypeClass_Prop::operator= (typename TBitMember<TC, TCO>::dataTypeClass_Prop & prop)
{
   SELF(BitMember, dataTypeClass);
   /*0H*/Class v = prop;
   ((C(BitMember) *)self->impl)->dataTypeClass = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::dataTypeClass_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, dataTypeClass);
   Class value(((C(BitMember) *)self->impl)->dataTypeClass);
   return value;
}

// member accessors: BitMember::type
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/DataMemberType TBitMember<TC, TCO>::type_Prop::operator= (/*0H*/DataMemberType v)
{
   SELF(BitMember, type);
   ((C(BitMember) *)self->impl)->type = (C(DataMemberType))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::type_Prop & TBitMember<TC, TCO>::type_Prop::operator= (typename TBitMember<TC, TCO>::type_Prop & prop)
{
   SELF(BitMember, type);
   /*0H*/DataMemberType v = prop;
   ((C(BitMember) *)self->impl)->type = (C(DataMemberType))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::type_Prop::operator /*0I*/DataMemberType () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, type);
   return self ? ((C(BitMember) *)self->impl)->type : (DataMemberType)0;
}

// member accessors: BitMember::size
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TBitMember<TC, TCO>::size_Prop::operator= (/*0H*/int v)
{
   SELF(BitMember, size);
   ((C(BitMember) *)self->impl)->size = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::size_Prop & TBitMember<TC, TCO>::size_Prop::operator= (typename TBitMember<TC, TCO>::size_Prop & prop)
{
   SELF(BitMember, size);
   /*0H*/int v = prop;
   ((C(BitMember) *)self->impl)->size = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::size_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, size);
   return self ? ((C(BitMember) *)self->impl)->size : 0;
}

// member accessors: BitMember::pos
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TBitMember<TC, TCO>::pos_Prop::operator= (/*0H*/int v)
{
   SELF(BitMember, pos);
   ((C(BitMember) *)self->impl)->pos = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::pos_Prop & TBitMember<TC, TCO>::pos_Prop::operator= (typename TBitMember<TC, TCO>::pos_Prop & prop)
{
   SELF(BitMember, pos);
   /*0H*/int v = prop;
   ((C(BitMember) *)self->impl)->pos = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::pos_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, pos);
   return self ? ((C(BitMember) *)self->impl)->pos : 0;
}

// member accessors: BitMember::mask
// (nohead::uint64)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/uint64 TBitMember<TC, TCO>::mask_Prop::operator= (/*0H*/uint64 v)
{
   SELF(BitMember, mask);
   ((C(BitMember) *)self->impl)->mask = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBitMember<TC, TCO>::mask_Prop & TBitMember<TC, TCO>::mask_Prop::operator= (typename TBitMember<TC, TCO>::mask_Prop & prop)
{
   SELF(BitMember, mask);
   /*0H*/uint64 v = prop;
   ((C(BitMember) *)self->impl)->mask = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBitMember<TC, TCO>::mask_Prop::operator /*0I*/uint64 () const
{
   __attribute__((unused)) TBitMember TCTCO * self = CONTAINER_OF(this, TBitMember TCTCO, mask);
   return self ? ((C(BitMember) *)self->impl)->mask : 0;
}

inline BuiltInContainer::BuiltInContainer(void ** _vTbl)
{
   impl._vTbl = _vTbl;
}



// member accessors: BuiltInContainer::_vTbl
// (struct::void * *)

/*nstSet*/inline /*0H*/void ** BuiltInContainer::_vTbl_Prop::operator= (/*0H*/void ** v)
{
   SELF(BuiltInContainer, _vTbl);
   self->impl._vTbl = v;
   return v;
}

/*regSet*/inline BuiltInContainer::_vTbl_Prop & BuiltInContainer::_vTbl_Prop::operator= (BuiltInContainer::_vTbl_Prop & prop)
{
   SELF(BuiltInContainer, _vTbl);
   /*0H*/void ** v = prop;
   self->impl._vTbl = v;
   return prop;
}
/*regGet*/inline BuiltInContainer::_vTbl_Prop::operator /*0I*/void ** () const
{
   SELF(BuiltInContainer, _vTbl);
   return self ? ((C(BuiltInContainer) *)&self->impl)->_vTbl : null;
}

// member accessors: BuiltInContainer::_class
// (struct::nohead)

/*nstSet*/inline /*0H*/Class BuiltInContainer::_class_Prop::operator= (/*0H*/Class & v)
{
   SELF(BuiltInContainer, _class);
   self->impl._class = v.impl;
   return v;
}

/*regSet*/inline BuiltInContainer::_class_Prop & BuiltInContainer::_class_Prop::operator= (BuiltInContainer::_class_Prop & prop)
{
   SELF(BuiltInContainer, _class);
   /*0H*/Class v = prop;
   self->impl._class = v.impl;
   return prop;
}
/*regGet*/inline BuiltInContainer::_class_Prop::operator /*0I*/Class () const
{
   SELF(BuiltInContainer, _class);
   Class value(((C(BuiltInContainer) *)&self->impl)->_class);
   return value;
}

// member accessors: BuiltInContainer::_refCount
// (struct::int)

/*nstSet*/inline /*0H*/int BuiltInContainer::_refCount_Prop::operator= (/*0H*/int v)
{
   SELF(BuiltInContainer, _refCount);
   self->impl._refCount = v;
   return v;
}

/*regSet*/inline BuiltInContainer::_refCount_Prop & BuiltInContainer::_refCount_Prop::operator= (BuiltInContainer::_refCount_Prop & prop)
{
   SELF(BuiltInContainer, _refCount);
   /*0H*/int v = prop;
   self->impl._refCount = v;
   return prop;
}
/*regGet*/inline BuiltInContainer::_refCount_Prop::operator /*0I*/int () const
{
   SELF(BuiltInContainer, _refCount);
   return self ? ((C(BuiltInContainer) *)&self->impl)->_refCount : 0;
}

// member accessors: BuiltInContainer::data
// (struct::void *)

/*nstSet*/inline /*0H*/void * BuiltInContainer::data_Prop::operator= (/*0H*/void * v)
{
   SELF(BuiltInContainer, data);
   self->impl.data = v;
   return v;
}

/*regSet*/inline BuiltInContainer::data_Prop & BuiltInContainer::data_Prop::operator= (BuiltInContainer::data_Prop & prop)
{
   SELF(BuiltInContainer, data);
   /*0H*/void * v = prop;
   self->impl.data = v;
   return prop;
}
/*regGet*/inline BuiltInContainer::data_Prop::operator /*0I*/void * () const
{
   SELF(BuiltInContainer, data);
   return self ? ((C(BuiltInContainer) *)&self->impl)->data : null;
}

// member accessors: BuiltInContainer::count
// (struct::int)

/*nstSet*/inline /*0H*/int BuiltInContainer::count_Prop::operator= (/*0H*/int v)
{
   SELF(BuiltInContainer, count);
   self->impl.count = v;
   return v;
}

/*regSet*/inline BuiltInContainer::count_Prop & BuiltInContainer::count_Prop::operator= (BuiltInContainer::count_Prop & prop)
{
   SELF(BuiltInContainer, count);
   /*0H*/int v = prop;
   self->impl.count = v;
   return prop;
}
/*regGet*/inline BuiltInContainer::count_Prop::operator /*0I*/int () const
{
   SELF(BuiltInContainer, count);
   return self ? ((C(BuiltInContainer) *)&self->impl)->count : 0;
}

// member accessors: BuiltInContainer::type
// (struct::nohead)

/*nstSet*/inline /*0H*/Class BuiltInContainer::type_Prop::operator= (/*0H*/Class & v)
{
   SELF(BuiltInContainer, type);
   self->impl.type = v.impl;
   return v;
}

/*regSet*/inline BuiltInContainer::type_Prop & BuiltInContainer::type_Prop::operator= (BuiltInContainer::type_Prop & prop)
{
   SELF(BuiltInContainer, type);
   /*0H*/Class v = prop;
   self->impl.type = v.impl;
   return prop;
}
/*regGet*/inline BuiltInContainer::type_Prop::operator /*0I*/Class () const
{
   SELF(BuiltInContainer, type);
   Class value(((C(BuiltInContainer) *)&self->impl)->type);
   return value;
}



// member accessors: Class::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TClass<TC, TCO>::prev_Prop::operator= (/*0H*/Class & v)
{
   SELF(Class, prev);
   ((C(Class) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::prev_Prop & TClass<TC, TCO>::prev_Prop::operator= (typename TClass<TC, TCO>::prev_Prop & prop)
{
   SELF(Class, prev);
   /*0H*/Class v = prop;
   ((C(Class) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::prev_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, prev);
   Class value(((C(Class) *)self->impl)->prev);
   return value;
}

// member accessors: Class::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TClass<TC, TCO>::next_Prop::operator= (/*0H*/Class & v)
{
   SELF(Class, next);
   ((C(Class) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::next_Prop & TClass<TC, TCO>::next_Prop::operator= (typename TClass<TC, TCO>::next_Prop & prop)
{
   SELF(Class, next);
   /*0H*/Class v = prop;
   ((C(Class) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::next_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, next);
   Class value(((C(Class) *)self->impl)->next);
   return value;
}

// member accessors: Class::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClass<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(Class, name);
   ((C(Class) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::name_Prop & TClass<TC, TCO>::name_Prop::operator= (typename TClass<TC, TCO>::name_Prop & prop)
{
   SELF(Class, name);
   /*0H*/const char * v = prop;
   ((C(Class) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, name);
   return self ? ((C(Class) *)self->impl)->name : null;
}

// member accessors: Class::offset
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::offset_Prop::operator= (/*0H*/int v)
{
   SELF(Class, offset);
   ((C(Class) *)self->impl)->offset = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::offset_Prop & TClass<TC, TCO>::offset_Prop::operator= (typename TClass<TC, TCO>::offset_Prop & prop)
{
   SELF(Class, offset);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->offset = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::offset_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, offset);
   return self ? ((C(Class) *)self->impl)->offset : 0;
}

// member accessors: Class::structSize
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::structSize_Prop::operator= (/*0H*/int v)
{
   SELF(Class, structSize);
   ((C(Class) *)self->impl)->structSize = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::structSize_Prop & TClass<TC, TCO>::structSize_Prop::operator= (typename TClass<TC, TCO>::structSize_Prop & prop)
{
   SELF(Class, structSize);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->structSize = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::structSize_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, structSize);
   return self ? ((C(Class) *)self->impl)->structSize : 0;
}

// member accessors: Class::_vTbl
// (nohead::void * *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void ** TClass<TC, TCO>::_vTbl_Prop::operator= (/*0H*/void ** v)
{
   SELF(Class, _vTbl);
   ((C(Class) *)self->impl)->_vTbl = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::_vTbl_Prop & TClass<TC, TCO>::_vTbl_Prop::operator= (typename TClass<TC, TCO>::_vTbl_Prop & prop)
{
   SELF(Class, _vTbl);
   /*0H*/void ** v = prop;
   ((C(Class) *)self->impl)->_vTbl = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::_vTbl_Prop::operator /*0I*/void ** () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, _vTbl);
   return self ? ((C(Class) *)self->impl)->_vTbl : null;
}

// member accessors: Class::vTblSize
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::vTblSize_Prop::operator= (/*0H*/int v)
{
   SELF(Class, vTblSize);
   ((C(Class) *)self->impl)->vTblSize = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::vTblSize_Prop & TClass<TC, TCO>::vTblSize_Prop::operator= (typename TClass<TC, TCO>::vTblSize_Prop & prop)
{
   SELF(Class, vTblSize);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->vTblSize = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::vTblSize_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, vTblSize);
   return self ? ((C(Class) *)self->impl)->vTblSize : 0;
}

// member accessors: Class::offsetClass
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::offsetClass_Prop::operator= (/*0H*/int v)
{
   SELF(Class, offsetClass);
   ((C(Class) *)self->impl)->offsetClass = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::offsetClass_Prop & TClass<TC, TCO>::offsetClass_Prop::operator= (typename TClass<TC, TCO>::offsetClass_Prop & prop)
{
   SELF(Class, offsetClass);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->offsetClass = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::offsetClass_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, offsetClass);
   return self ? ((C(Class) *)self->impl)->offsetClass : 0;
}

// member accessors: Class::sizeClass
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::sizeClass_Prop::operator= (/*0H*/int v)
{
   SELF(Class, sizeClass);
   ((C(Class) *)self->impl)->sizeClass = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::sizeClass_Prop & TClass<TC, TCO>::sizeClass_Prop::operator= (typename TClass<TC, TCO>::sizeClass_Prop & prop)
{
   SELF(Class, sizeClass);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->sizeClass = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::sizeClass_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, sizeClass);
   return self ? ((C(Class) *)self->impl)->sizeClass : 0;
}

// member accessors: Class::base
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TClass<TC, TCO>::base_Prop::operator= (/*0H*/Class & v)
{
   SELF(Class, base);
   ((C(Class) *)self->impl)->base = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::base_Prop & TClass<TC, TCO>::base_Prop::operator= (typename TClass<TC, TCO>::base_Prop & prop)
{
   SELF(Class, base);
   /*0H*/Class v = prop;
   ((C(Class) *)self->impl)->base = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::base_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, base);
   Class value(((C(Class) *)self->impl)->base);
   return value;
}

// member accessors: Class::methods
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BinaryTree TClass<TC, TCO>::methods_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(Class, methods);
   ((C(Class) *)self->impl)->methods = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::methods_Prop & TClass<TC, TCO>::methods_Prop::operator= (typename TClass<TC, TCO>::methods_Prop & prop)
{
   SELF(Class, methods);
   /*0H*/BinaryTree v = prop;
   ((C(Class) *)self->impl)->methods = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::methods_Prop::operator /*0I*/BinaryTree () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, methods);
   BinaryTree value(((C(Class) *)self->impl)->methods);
   return value;
}

// member accessors: Class::members
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BinaryTree TClass<TC, TCO>::members_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(Class, members);
   ((C(Class) *)self->impl)->members = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::members_Prop & TClass<TC, TCO>::members_Prop::operator= (typename TClass<TC, TCO>::members_Prop & prop)
{
   SELF(Class, members);
   /*0H*/BinaryTree v = prop;
   ((C(Class) *)self->impl)->members = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::members_Prop::operator /*0I*/BinaryTree () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, members);
   BinaryTree value(((C(Class) *)self->impl)->members);
   return value;
}

// member accessors: Class::prop
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BinaryTree TClass<TC, TCO>::prop_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(Class, prop);
   ((C(Class) *)self->impl)->prop = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::prop_Prop & TClass<TC, TCO>::prop_Prop::operator= (typename TClass<TC, TCO>::prop_Prop & prop)
{
   SELF(Class, prop);
   /*0H*/BinaryTree v = prop;
   ((C(Class) *)self->impl)->prop = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::prop_Prop::operator /*0I*/BinaryTree () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, prop);
   BinaryTree value(((C(Class) *)self->impl)->prop);
   return value;
}

// member accessors: Class::membersAndProperties
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TClass<TC, TCO>::membersAndProperties_Prop::operator= (/*0H*/OldList v)
{
   SELF(Class, membersAndProperties);
   ((C(Class) *)self->impl)->membersAndProperties = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::membersAndProperties_Prop & TClass<TC, TCO>::membersAndProperties_Prop::operator= (typename TClass<TC, TCO>::membersAndProperties_Prop & prop)
{
   SELF(Class, membersAndProperties);
   /*0H*/OldList v = prop;
   ((C(Class) *)self->impl)->membersAndProperties = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::membersAndProperties_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, membersAndProperties);
   OldList value(((C(Class) *)self->impl)->membersAndProperties);
   return value;
}

// member accessors: Class::classProperties
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BinaryTree TClass<TC, TCO>::classProperties_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(Class, classProperties);
   ((C(Class) *)self->impl)->classProperties = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::classProperties_Prop & TClass<TC, TCO>::classProperties_Prop::operator= (typename TClass<TC, TCO>::classProperties_Prop & prop)
{
   SELF(Class, classProperties);
   /*0H*/BinaryTree v = prop;
   ((C(Class) *)self->impl)->classProperties = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::classProperties_Prop::operator /*0I*/BinaryTree () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, classProperties);
   BinaryTree value(((C(Class) *)self->impl)->classProperties);
   return value;
}

// member accessors: Class::derivatives
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TClass<TC, TCO>::derivatives_Prop::operator= (/*0H*/OldList v)
{
   SELF(Class, derivatives);
   ((C(Class) *)self->impl)->derivatives = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::derivatives_Prop & TClass<TC, TCO>::derivatives_Prop::operator= (typename TClass<TC, TCO>::derivatives_Prop & prop)
{
   SELF(Class, derivatives);
   /*0H*/OldList v = prop;
   ((C(Class) *)self->impl)->derivatives = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::derivatives_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, derivatives);
   OldList value(((C(Class) *)self->impl)->derivatives);
   return value;
}

// member accessors: Class::memberID
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::memberID_Prop::operator= (/*0H*/int v)
{
   SELF(Class, memberID);
   ((C(Class) *)self->impl)->memberID = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::memberID_Prop & TClass<TC, TCO>::memberID_Prop::operator= (typename TClass<TC, TCO>::memberID_Prop & prop)
{
   SELF(Class, memberID);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->memberID = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::memberID_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, memberID);
   return self ? ((C(Class) *)self->impl)->memberID : 0;
}

// member accessors: Class::startMemberID
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::startMemberID_Prop::operator= (/*0H*/int v)
{
   SELF(Class, startMemberID);
   ((C(Class) *)self->impl)->startMemberID = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::startMemberID_Prop & TClass<TC, TCO>::startMemberID_Prop::operator= (typename TClass<TC, TCO>::startMemberID_Prop & prop)
{
   SELF(Class, startMemberID);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->startMemberID = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::startMemberID_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, startMemberID);
   return self ? ((C(Class) *)self->impl)->startMemberID : 0;
}

// member accessors: Class::type
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ClassType TClass<TC, TCO>::type_Prop::operator= (/*0H*/ClassType v)
{
   SELF(Class, type);
   ((C(Class) *)self->impl)->type = (C(ClassType))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::type_Prop & TClass<TC, TCO>::type_Prop::operator= (typename TClass<TC, TCO>::type_Prop & prop)
{
   SELF(Class, type);
   /*0H*/ClassType v = prop;
   ((C(Class) *)self->impl)->type = (C(ClassType))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::type_Prop::operator /*0I*/ClassType () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, type);
   return self ? ((C(Class) *)self->impl)->type : (ClassType)0;
}

// member accessors: Class::module
// (nohead::normal)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Module TClass<TC, TCO>::module_Prop::operator= (/*0H*/Module v)
{
   SELF(Class, module);
   ((C(Class) *)self->impl)->module = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::module_Prop & TClass<TC, TCO>::module_Prop::operator= (typename TClass<TC, TCO>::module_Prop & prop)
{
   SELF(Class, module);
   /*0H*/Module v = prop;
   ((C(Class) *)self->impl)->module = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::module_Prop::operator /*0I*/Module () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, module);
   Module value(((C(Class) *)self->impl)->module);
   return value;
}

// member accessors: Class::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClass<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(Class, dataTypeString);
   ((C(Class) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::dataTypeString_Prop & TClass<TC, TCO>::dataTypeString_Prop::operator= (typename TClass<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(Class, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(Class) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, dataTypeString);
   return self ? ((C(Class) *)self->impl)->dataTypeString : null;
}

// member accessors: Class::typeSize
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::typeSize_Prop::operator= (/*0H*/int v)
{
   SELF(Class, typeSize);
   ((C(Class) *)self->impl)->typeSize = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::typeSize_Prop & TClass<TC, TCO>::typeSize_Prop::operator= (typename TClass<TC, TCO>::typeSize_Prop & prop)
{
   SELF(Class, typeSize);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->typeSize = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::typeSize_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, typeSize);
   return self ? ((C(Class) *)self->impl)->typeSize : 0;
}

// member accessors: Class::defaultAlignment
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::defaultAlignment_Prop::operator= (/*0H*/int v)
{
   SELF(Class, defaultAlignment);
   ((C(Class) *)self->impl)->defaultAlignment = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::defaultAlignment_Prop & TClass<TC, TCO>::defaultAlignment_Prop::operator= (typename TClass<TC, TCO>::defaultAlignment_Prop & prop)
{
   SELF(Class, defaultAlignment);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->defaultAlignment = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::defaultAlignment_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, defaultAlignment);
   return self ? ((C(Class) *)self->impl)->defaultAlignment : 0;
}

// member accessors: Class::memberOffset
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::memberOffset_Prop::operator= (/*0H*/int v)
{
   SELF(Class, memberOffset);
   ((C(Class) *)self->impl)->memberOffset = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::memberOffset_Prop & TClass<TC, TCO>::memberOffset_Prop::operator= (typename TClass<TC, TCO>::memberOffset_Prop & prop)
{
   SELF(Class, memberOffset);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->memberOffset = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::memberOffset_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, memberOffset);
   return self ? ((C(Class) *)self->impl)->memberOffset : 0;
}

// member accessors: Class::selfWatchers
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TClass<TC, TCO>::selfWatchers_Prop::operator= (/*0H*/OldList v)
{
   SELF(Class, selfWatchers);
   ((C(Class) *)self->impl)->selfWatchers = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::selfWatchers_Prop & TClass<TC, TCO>::selfWatchers_Prop::operator= (typename TClass<TC, TCO>::selfWatchers_Prop & prop)
{
   SELF(Class, selfWatchers);
   /*0H*/OldList v = prop;
   ((C(Class) *)self->impl)->selfWatchers = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::selfWatchers_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, selfWatchers);
   OldList value(((C(Class) *)self->impl)->selfWatchers);
   return value;
}

// member accessors: Class::designerClass
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClass<TC, TCO>::designerClass_Prop::operator= (/*0H*/const char * v)
{
   SELF(Class, designerClass);
   ((C(Class) *)self->impl)->designerClass = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::designerClass_Prop & TClass<TC, TCO>::designerClass_Prop::operator= (typename TClass<TC, TCO>::designerClass_Prop & prop)
{
   SELF(Class, designerClass);
   /*0H*/const char * v = prop;
   ((C(Class) *)self->impl)->designerClass = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::designerClass_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, designerClass);
   return self ? ((C(Class) *)self->impl)->designerClass : null;
}

// member accessors: Class::noExpansion
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClass<TC, TCO>::noExpansion_Prop::operator= (/*0H*/bool v)
{
   SELF(Class, noExpansion);
   ((C(Class) *)self->impl)->noExpansion = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::noExpansion_Prop & TClass<TC, TCO>::noExpansion_Prop::operator= (typename TClass<TC, TCO>::noExpansion_Prop & prop)
{
   SELF(Class, noExpansion);
   /*0H*/bool v = prop;
   ((C(Class) *)self->impl)->noExpansion = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::noExpansion_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, noExpansion);
   return self ? ((C(Class) *)self->impl)->noExpansion : (bool)0;
}

// member accessors: Class::defaultProperty
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClass<TC, TCO>::defaultProperty_Prop::operator= (/*0H*/const char * v)
{
   SELF(Class, defaultProperty);
   ((C(Class) *)self->impl)->defaultProperty = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::defaultProperty_Prop & TClass<TC, TCO>::defaultProperty_Prop::operator= (typename TClass<TC, TCO>::defaultProperty_Prop & prop)
{
   SELF(Class, defaultProperty);
   /*0H*/const char * v = prop;
   ((C(Class) *)self->impl)->defaultProperty = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::defaultProperty_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, defaultProperty);
   return self ? ((C(Class) *)self->impl)->defaultProperty : null;
}

// member accessors: Class::comRedefinition
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClass<TC, TCO>::comRedefinition_Prop::operator= (/*0H*/bool v)
{
   SELF(Class, comRedefinition);
   ((C(Class) *)self->impl)->comRedefinition = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::comRedefinition_Prop & TClass<TC, TCO>::comRedefinition_Prop::operator= (typename TClass<TC, TCO>::comRedefinition_Prop & prop)
{
   SELF(Class, comRedefinition);
   /*0H*/bool v = prop;
   ((C(Class) *)self->impl)->comRedefinition = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::comRedefinition_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, comRedefinition);
   return self ? ((C(Class) *)self->impl)->comRedefinition : (bool)0;
}

// member accessors: Class::count
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::count_Prop::operator= (/*0H*/int v)
{
   SELF(Class, count);
   ((C(Class) *)self->impl)->count = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::count_Prop & TClass<TC, TCO>::count_Prop::operator= (typename TClass<TC, TCO>::count_Prop & prop)
{
   SELF(Class, count);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->count = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::count_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, count);
   return self ? ((C(Class) *)self->impl)->count : 0;
}

// member accessors: Class::isRemote
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::isRemote_Prop::operator= (/*0H*/int v)
{
   SELF(Class, isRemote);
   ((C(Class) *)self->impl)->isRemote = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::isRemote_Prop & TClass<TC, TCO>::isRemote_Prop::operator= (typename TClass<TC, TCO>::isRemote_Prop & prop)
{
   SELF(Class, isRemote);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->isRemote = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::isRemote_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, isRemote);
   return self ? ((C(Class) *)self->impl)->isRemote : 0;
}

// member accessors: Class::internalDecl
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClass<TC, TCO>::internalDecl_Prop::operator= (/*0H*/bool v)
{
   SELF(Class, internalDecl);
   ((C(Class) *)self->impl)->internalDecl = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::internalDecl_Prop & TClass<TC, TCO>::internalDecl_Prop::operator= (typename TClass<TC, TCO>::internalDecl_Prop & prop)
{
   SELF(Class, internalDecl);
   /*0H*/bool v = prop;
   ((C(Class) *)self->impl)->internalDecl = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::internalDecl_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, internalDecl);
   return self ? ((C(Class) *)self->impl)->internalDecl : (bool)0;
}

// member accessors: Class::data
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TClass<TC, TCO>::data_Prop::operator= (/*0H*/void * v)
{
   SELF(Class, data);
   ((C(Class) *)self->impl)->data = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::data_Prop & TClass<TC, TCO>::data_Prop::operator= (typename TClass<TC, TCO>::data_Prop & prop)
{
   SELF(Class, data);
   /*0H*/void * v = prop;
   ((C(Class) *)self->impl)->data = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::data_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, data);
   return self ? ((C(Class) *)self->impl)->data : null;
}

// member accessors: Class::computeSize
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClass<TC, TCO>::computeSize_Prop::operator= (/*0H*/bool v)
{
   SELF(Class, computeSize);
   ((C(Class) *)self->impl)->computeSize = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::computeSize_Prop & TClass<TC, TCO>::computeSize_Prop::operator= (typename TClass<TC, TCO>::computeSize_Prop & prop)
{
   SELF(Class, computeSize);
   /*0H*/bool v = prop;
   ((C(Class) *)self->impl)->computeSize = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::computeSize_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, computeSize);
   return self ? ((C(Class) *)self->impl)->computeSize : (bool)0;
}

// member accessors: Class::structAlignment
// (nohead::short)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/short TClass<TC, TCO>::structAlignment_Prop::operator= (/*0H*/short v)
{
   SELF(Class, structAlignment);
   ((C(Class) *)self->impl)->structAlignment = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::structAlignment_Prop & TClass<TC, TCO>::structAlignment_Prop::operator= (typename TClass<TC, TCO>::structAlignment_Prop & prop)
{
   SELF(Class, structAlignment);
   /*0H*/short v = prop;
   ((C(Class) *)self->impl)->structAlignment = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::structAlignment_Prop::operator /*0I*/short () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, structAlignment);
   return self ? ((C(Class) *)self->impl)->structAlignment : 0;
}

// member accessors: Class::pointerAlignment
// (nohead::short)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/short TClass<TC, TCO>::pointerAlignment_Prop::operator= (/*0H*/short v)
{
   SELF(Class, pointerAlignment);
   ((C(Class) *)self->impl)->pointerAlignment = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::pointerAlignment_Prop & TClass<TC, TCO>::pointerAlignment_Prop::operator= (typename TClass<TC, TCO>::pointerAlignment_Prop & prop)
{
   SELF(Class, pointerAlignment);
   /*0H*/short v = prop;
   ((C(Class) *)self->impl)->pointerAlignment = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::pointerAlignment_Prop::operator /*0I*/short () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, pointerAlignment);
   return self ? ((C(Class) *)self->impl)->pointerAlignment : 0;
}

// member accessors: Class::destructionWatchOffset
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::destructionWatchOffset_Prop::operator= (/*0H*/int v)
{
   SELF(Class, destructionWatchOffset);
   ((C(Class) *)self->impl)->destructionWatchOffset = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::destructionWatchOffset_Prop & TClass<TC, TCO>::destructionWatchOffset_Prop::operator= (typename TClass<TC, TCO>::destructionWatchOffset_Prop & prop)
{
   SELF(Class, destructionWatchOffset);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->destructionWatchOffset = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::destructionWatchOffset_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, destructionWatchOffset);
   return self ? ((C(Class) *)self->impl)->destructionWatchOffset : 0;
}

// member accessors: Class::fixed
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClass<TC, TCO>::fixed_Prop::operator= (/*0H*/bool v)
{
   SELF(Class, fixed);
   ((C(Class) *)self->impl)->fixed = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::fixed_Prop & TClass<TC, TCO>::fixed_Prop::operator= (typename TClass<TC, TCO>::fixed_Prop & prop)
{
   SELF(Class, fixed);
   /*0H*/bool v = prop;
   ((C(Class) *)self->impl)->fixed = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::fixed_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, fixed);
   return self ? ((C(Class) *)self->impl)->fixed : (bool)0;
}

// member accessors: Class::delayedCPValues
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TClass<TC, TCO>::delayedCPValues_Prop::operator= (/*0H*/OldList v)
{
   SELF(Class, delayedCPValues);
   ((C(Class) *)self->impl)->delayedCPValues = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::delayedCPValues_Prop & TClass<TC, TCO>::delayedCPValues_Prop::operator= (typename TClass<TC, TCO>::delayedCPValues_Prop & prop)
{
   SELF(Class, delayedCPValues);
   /*0H*/OldList v = prop;
   ((C(Class) *)self->impl)->delayedCPValues = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::delayedCPValues_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, delayedCPValues);
   OldList value(((C(Class) *)self->impl)->delayedCPValues);
   return value;
}

// member accessors: Class::inheritanceAccess
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Access_Mode TClass<TC, TCO>::inheritanceAccess_Prop::operator= (/*0H*/Access_Mode v)
{
   SELF(Class, inheritanceAccess);
   ((C(Class) *)self->impl)->inheritanceAccess = (C(AccessMode))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::inheritanceAccess_Prop & TClass<TC, TCO>::inheritanceAccess_Prop::operator= (typename TClass<TC, TCO>::inheritanceAccess_Prop & prop)
{
   SELF(Class, inheritanceAccess);
   /*0H*/Access_Mode v = prop;
   ((C(Class) *)self->impl)->inheritanceAccess = (C(AccessMode))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::inheritanceAccess_Prop::operator /*0I*/Access_Mode () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, inheritanceAccess);
   return self ? ((C(Class) *)self->impl)->inheritanceAccess : (Access_Mode)0;
}

// member accessors: Class::fullName
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClass<TC, TCO>::fullName_Prop::operator= (/*0H*/const char * v)
{
   SELF(Class, fullName);
   ((C(Class) *)self->impl)->fullName = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::fullName_Prop & TClass<TC, TCO>::fullName_Prop::operator= (typename TClass<TC, TCO>::fullName_Prop & prop)
{
   SELF(Class, fullName);
   /*0H*/const char * v = prop;
   ((C(Class) *)self->impl)->fullName = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::fullName_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, fullName);
   return self ? ((C(Class) *)self->impl)->fullName : null;
}

// member accessors: Class::symbol
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TClass<TC, TCO>::symbol_Prop::operator= (/*0H*/void * v)
{
   SELF(Class, symbol);
   ((C(Class) *)self->impl)->symbol = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::symbol_Prop & TClass<TC, TCO>::symbol_Prop::operator= (typename TClass<TC, TCO>::symbol_Prop & prop)
{
   SELF(Class, symbol);
   /*0H*/void * v = prop;
   ((C(Class) *)self->impl)->symbol = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::symbol_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, symbol);
   return self ? ((C(Class) *)self->impl)->symbol : null;
}

// member accessors: Class::conversions
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TClass<TC, TCO>::conversions_Prop::operator= (/*0H*/OldList v)
{
   SELF(Class, conversions);
   ((C(Class) *)self->impl)->conversions = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::conversions_Prop & TClass<TC, TCO>::conversions_Prop::operator= (typename TClass<TC, TCO>::conversions_Prop & prop)
{
   SELF(Class, conversions);
   /*0H*/OldList v = prop;
   ((C(Class) *)self->impl)->conversions = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::conversions_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, conversions);
   OldList value(((C(Class) *)self->impl)->conversions);
   return value;
}

// member accessors: Class::templateParams
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TClass<TC, TCO>::templateParams_Prop::operator= (/*0H*/OldList v)
{
   SELF(Class, templateParams);
   ((C(Class) *)self->impl)->templateParams = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::templateParams_Prop & TClass<TC, TCO>::templateParams_Prop::operator= (typename TClass<TC, TCO>::templateParams_Prop & prop)
{
   SELF(Class, templateParams);
   /*0H*/OldList v = prop;
   ((C(Class) *)self->impl)->templateParams = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::templateParams_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, templateParams);
   OldList value(((C(Class) *)self->impl)->templateParams);
   return value;
}

// member accessors: Class::templateClass
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TClass<TC, TCO>::templateClass_Prop::operator= (/*0H*/Class & v)
{
   SELF(Class, templateClass);
   ((C(Class) *)self->impl)->templateClass = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::templateClass_Prop & TClass<TC, TCO>::templateClass_Prop::operator= (typename TClass<TC, TCO>::templateClass_Prop & prop)
{
   SELF(Class, templateClass);
   /*0H*/Class v = prop;
   ((C(Class) *)self->impl)->templateClass = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::templateClass_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, templateClass);
   Class value(((C(Class) *)self->impl)->templateClass);
   return value;
}

// member accessors: Class::templatized
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TClass<TC, TCO>::templatized_Prop::operator= (/*0H*/OldList v)
{
   SELF(Class, templatized);
   ((C(Class) *)self->impl)->templatized = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::templatized_Prop & TClass<TC, TCO>::templatized_Prop::operator= (typename TClass<TC, TCO>::templatized_Prop & prop)
{
   SELF(Class, templatized);
   /*0H*/OldList v = prop;
   ((C(Class) *)self->impl)->templatized = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::templatized_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, templatized);
   OldList value(((C(Class) *)self->impl)->templatized);
   return value;
}

// member accessors: Class::numParams
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClass<TC, TCO>::numParams_Prop::operator= (/*0H*/int v)
{
   SELF(Class, numParams);
   ((C(Class) *)self->impl)->numParams = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::numParams_Prop & TClass<TC, TCO>::numParams_Prop::operator= (typename TClass<TC, TCO>::numParams_Prop & prop)
{
   SELF(Class, numParams);
   /*0H*/int v = prop;
   ((C(Class) *)self->impl)->numParams = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::numParams_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, numParams);
   return self ? ((C(Class) *)self->impl)->numParams : 0;
}

// member accessors: Class::isInstanceClass
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClass<TC, TCO>::isInstanceClass_Prop::operator= (/*0H*/bool v)
{
   SELF(Class, isInstanceClass);
   ((C(Class) *)self->impl)->isInstanceClass = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::isInstanceClass_Prop & TClass<TC, TCO>::isInstanceClass_Prop::operator= (typename TClass<TC, TCO>::isInstanceClass_Prop & prop)
{
   SELF(Class, isInstanceClass);
   /*0H*/bool v = prop;
   ((C(Class) *)self->impl)->isInstanceClass = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::isInstanceClass_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, isInstanceClass);
   return self ? ((C(Class) *)self->impl)->isInstanceClass : (bool)0;
}

// member accessors: Class::byValueSystemClass
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClass<TC, TCO>::byValueSystemClass_Prop::operator= (/*0H*/bool v)
{
   SELF(Class, byValueSystemClass);
   ((C(Class) *)self->impl)->byValueSystemClass = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::byValueSystemClass_Prop & TClass<TC, TCO>::byValueSystemClass_Prop::operator= (typename TClass<TC, TCO>::byValueSystemClass_Prop & prop)
{
   SELF(Class, byValueSystemClass);
   /*0H*/bool v = prop;
   ((C(Class) *)self->impl)->byValueSystemClass = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::byValueSystemClass_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, byValueSystemClass);
   return self ? ((C(Class) *)self->impl)->byValueSystemClass : (bool)0;
}

// member accessors: Class::bindingsClass
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TClass<TC, TCO>::bindingsClass_Prop::operator= (/*0H*/void * v)
{
   SELF(Class, bindingsClass);
   ((C(Class) *)self->impl)->bindingsClass = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClass<TC, TCO>::bindingsClass_Prop & TClass<TC, TCO>::bindingsClass_Prop::operator= (typename TClass<TC, TCO>::bindingsClass_Prop & prop)
{
   SELF(Class, bindingsClass);
   /*0H*/void * v = prop;
   ((C(Class) *)self->impl)->bindingsClass = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClass<TC, TCO>::bindingsClass_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TClass TCTCO * self = CONTAINER_OF(this, TClass TCTCO, bindingsClass);
   return self ? ((C(Class) *)self->impl)->bindingsClass : null;
}



// member accessors: ClassProperty::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClassProperty<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(ClassProperty, name);
   ((C(ClassProperty) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassProperty<TC, TCO>::name_Prop & TClassProperty<TC, TCO>::name_Prop::operator= (typename TClassProperty<TC, TCO>::name_Prop & prop)
{
   SELF(ClassProperty, name);
   /*0H*/const char * v = prop;
   ((C(ClassProperty) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassProperty<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClassProperty TCTCO * self = CONTAINER_OF(this, TClassProperty TCTCO, name);
   return self ? ((C(ClassProperty) *)self->impl)->name : null;
}

// member accessors: ClassProperty::parent
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ClassProperty TClassProperty<TC, TCO>::parent_Prop::operator= (/*0H*/ClassProperty & v)
{
   SELF(ClassProperty, parent);
   ((C(ClassProperty) *)self->impl)->parent = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassProperty<TC, TCO>::parent_Prop & TClassProperty<TC, TCO>::parent_Prop::operator= (typename TClassProperty<TC, TCO>::parent_Prop & prop)
{
   SELF(ClassProperty, parent);
   /*0H*/ClassProperty v = prop;
   ((C(ClassProperty) *)self->impl)->parent = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassProperty<TC, TCO>::parent_Prop::operator /*0I*/ClassProperty () const
{
   __attribute__((unused)) TClassProperty TCTCO * self = CONTAINER_OF(this, TClassProperty TCTCO, parent);
   ClassProperty value(((C(ClassProperty) *)self->impl)->parent);
   return value;
}

// member accessors: ClassProperty::left
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ClassProperty TClassProperty<TC, TCO>::left_Prop::operator= (/*0H*/ClassProperty & v)
{
   SELF(ClassProperty, left);
   ((C(ClassProperty) *)self->impl)->left = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassProperty<TC, TCO>::left_Prop & TClassProperty<TC, TCO>::left_Prop::operator= (typename TClassProperty<TC, TCO>::left_Prop & prop)
{
   SELF(ClassProperty, left);
   /*0H*/ClassProperty v = prop;
   ((C(ClassProperty) *)self->impl)->left = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassProperty<TC, TCO>::left_Prop::operator /*0I*/ClassProperty () const
{
   __attribute__((unused)) TClassProperty TCTCO * self = CONTAINER_OF(this, TClassProperty TCTCO, left);
   ClassProperty value(((C(ClassProperty) *)self->impl)->left);
   return value;
}

// member accessors: ClassProperty::right
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ClassProperty TClassProperty<TC, TCO>::right_Prop::operator= (/*0H*/ClassProperty & v)
{
   SELF(ClassProperty, right);
   ((C(ClassProperty) *)self->impl)->right = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassProperty<TC, TCO>::right_Prop & TClassProperty<TC, TCO>::right_Prop::operator= (typename TClassProperty<TC, TCO>::right_Prop & prop)
{
   SELF(ClassProperty, right);
   /*0H*/ClassProperty v = prop;
   ((C(ClassProperty) *)self->impl)->right = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassProperty<TC, TCO>::right_Prop::operator /*0I*/ClassProperty () const
{
   __attribute__((unused)) TClassProperty TCTCO * self = CONTAINER_OF(this, TClassProperty TCTCO, right);
   ClassProperty value(((C(ClassProperty) *)self->impl)->right);
   return value;
}

// member accessors: ClassProperty::depth
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TClassProperty<TC, TCO>::depth_Prop::operator= (/*0H*/int v)
{
   SELF(ClassProperty, depth);
   ((C(ClassProperty) *)self->impl)->depth = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassProperty<TC, TCO>::depth_Prop & TClassProperty<TC, TCO>::depth_Prop::operator= (typename TClassProperty<TC, TCO>::depth_Prop & prop)
{
   SELF(ClassProperty, depth);
   /*0H*/int v = prop;
   ((C(ClassProperty) *)self->impl)->depth = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassProperty<TC, TCO>::depth_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TClassProperty TCTCO * self = CONTAINER_OF(this, TClassProperty TCTCO, depth);
   return self ? ((C(ClassProperty) *)self->impl)->depth : 0;
}

// member accessors: ClassProperty::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClassProperty<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(ClassProperty, dataTypeString);
   ((C(ClassProperty) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassProperty<TC, TCO>::dataTypeString_Prop & TClassProperty<TC, TCO>::dataTypeString_Prop::operator= (typename TClassProperty<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(ClassProperty, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(ClassProperty) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassProperty<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClassProperty TCTCO * self = CONTAINER_OF(this, TClassProperty TCTCO, dataTypeString);
   return self ? ((C(ClassProperty) *)self->impl)->dataTypeString : null;
}

// member accessors: ClassProperty::constant
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TClassProperty<TC, TCO>::constant_Prop::operator= (/*0H*/bool v)
{
   SELF(ClassProperty, constant);
   ((C(ClassProperty) *)self->impl)->constant = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassProperty<TC, TCO>::constant_Prop & TClassProperty<TC, TCO>::constant_Prop::operator= (typename TClassProperty<TC, TCO>::constant_Prop & prop)
{
   SELF(ClassProperty, constant);
   /*0H*/bool v = prop;
   ((C(ClassProperty) *)self->impl)->constant = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassProperty<TC, TCO>::constant_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TClassProperty TCTCO * self = CONTAINER_OF(this, TClassProperty TCTCO, constant);
   return self ? ((C(ClassProperty) *)self->impl)->constant : (bool)0;
}

inline ClassTemplateArgument::ClassTemplateArgument(const char * dataTypeString)
{
   impl.dataTypeString = dataTypeString;
}



// member accessors: ClassTemplateArgument::dataTypeString
// (struct::const char *)

/*nstSet*/inline /*0H*/const char * ClassTemplateArgument::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(ClassTemplateArgument, dataTypeString);
   self->impl.dataTypeString = v;
   return v;
}

/*regSet*/inline ClassTemplateArgument::dataTypeString_Prop & ClassTemplateArgument::dataTypeString_Prop::operator= (ClassTemplateArgument::dataTypeString_Prop & prop)
{
   SELF(ClassTemplateArgument, dataTypeString);
   /*0H*/const char * v = prop;
   self->impl.dataTypeString = v;
   return prop;
}
/*regGet*/inline ClassTemplateArgument::dataTypeString_Prop::operator /*0I*/const char * () const
{
   SELF(ClassTemplateArgument, dataTypeString);
   return self ? ((C(ClassTemplateArgument) *)&self->impl)->dataTypeString : null;
}

// member accessors: ClassTemplateArgument::dataTypeClass
// (struct::nohead)

/*nstSet*/inline /*0H*/Class ClassTemplateArgument::dataTypeClass_Prop::operator= (/*0H*/Class & v)
{
   SELF(ClassTemplateArgument, dataTypeClass);
   self->impl.dataTypeClass = v.impl;
   return v;
}

/*regSet*/inline ClassTemplateArgument::dataTypeClass_Prop & ClassTemplateArgument::dataTypeClass_Prop::operator= (ClassTemplateArgument::dataTypeClass_Prop & prop)
{
   SELF(ClassTemplateArgument, dataTypeClass);
   /*0H*/Class v = prop;
   self->impl.dataTypeClass = v.impl;
   return prop;
}
/*regGet*/inline ClassTemplateArgument::dataTypeClass_Prop::operator /*0I*/Class () const
{
   SELF(ClassTemplateArgument, dataTypeClass);
   Class value(((C(ClassTemplateArgument) *)&self->impl)->dataTypeClass);
   return value;
}

// member accessors: ClassTemplateArgument::expression
// (struct::struct)

/*nstSet*/inline /*0H*/DataValue ClassTemplateArgument::expression_Prop::operator= (/*0H*/DataValue v)
{
   SELF(ClassTemplateArgument, expression);
   self->impl.expression = v.impl;
   return v;
}

/*regSet*/inline ClassTemplateArgument::expression_Prop & ClassTemplateArgument::expression_Prop::operator= (ClassTemplateArgument::expression_Prop & prop)
{
   SELF(ClassTemplateArgument, expression);
   /*0H*/DataValue v = prop;
   self->impl.expression = v.impl;
   return prop;
}
/*regGet*/inline ClassTemplateArgument::expression_Prop::operator /*0I*/DataValue () const
{
   SELF(ClassTemplateArgument, expression);
   DataValue value(((C(ClassTemplateArgument) *)&self->impl)->expression);
   return value;
}

// member accessors: ClassTemplateArgument::memberString
// (struct::const char *)

/*nstSet*/inline /*0H*/const char * ClassTemplateArgument::memberString_Prop::operator= (/*0H*/const char * v)
{
   SELF(ClassTemplateArgument, memberString);
   self->impl.memberString = v;
   return v;
}

/*regSet*/inline ClassTemplateArgument::memberString_Prop & ClassTemplateArgument::memberString_Prop::operator= (ClassTemplateArgument::memberString_Prop & prop)
{
   SELF(ClassTemplateArgument, memberString);
   /*0H*/const char * v = prop;
   self->impl.memberString = v;
   return prop;
}
/*regGet*/inline ClassTemplateArgument::memberString_Prop::operator /*0I*/const char * () const
{
   SELF(ClassTemplateArgument, memberString);
   return self ? ((C(ClassTemplateArgument) *)&self->impl)->memberString : null;
}

// member accessors: ClassTemplateArgument::member
// (struct::nohead)

/*nstSet*/inline /*0H*/DataMember ClassTemplateArgument::member_Prop::operator= (/*0H*/DataMember & v)
{
   SELF(ClassTemplateArgument, member);
   self->impl.member = v.impl;
   return v;
}

/*regSet*/inline ClassTemplateArgument::member_Prop & ClassTemplateArgument::member_Prop::operator= (ClassTemplateArgument::member_Prop & prop)
{
   SELF(ClassTemplateArgument, member);
   /*0H*/DataMember v = prop;
   self->impl.member = v.impl;
   return prop;
}
/*regGet*/inline ClassTemplateArgument::member_Prop::operator /*0I*/DataMember () const
{
   SELF(ClassTemplateArgument, member);
   DataMember value(((C(ClassTemplateArgument) *)&self->impl)->member);
   return value;
}

// member accessors: ClassTemplateArgument::prop
// (struct::nohead)

/*nstSet*/inline /*0H*/Property ClassTemplateArgument::prop_Prop::operator= (/*0H*/Property & v)
{
   SELF(ClassTemplateArgument, prop);
   self->impl.prop = v.impl;
   return v;
}

/*regSet*/inline ClassTemplateArgument::prop_Prop & ClassTemplateArgument::prop_Prop::operator= (ClassTemplateArgument::prop_Prop & prop)
{
   SELF(ClassTemplateArgument, prop);
   /*0H*/Property v = prop;
   self->impl.prop = v.impl;
   return prop;
}
/*regGet*/inline ClassTemplateArgument::prop_Prop::operator /*0I*/Property () const
{
   SELF(ClassTemplateArgument, prop);
   Property value(((C(ClassTemplateArgument) *)&self->impl)->prop);
   return value;
}

// member accessors: ClassTemplateArgument::method
// (struct::nohead)

/*nstSet*/inline /*0H*/Method ClassTemplateArgument::method_Prop::operator= (/*0H*/Method & v)
{
   SELF(ClassTemplateArgument, method);
   self->impl.method = v.impl;
   return v;
}

/*regSet*/inline ClassTemplateArgument::method_Prop & ClassTemplateArgument::method_Prop::operator= (ClassTemplateArgument::method_Prop & prop)
{
   SELF(ClassTemplateArgument, method);
   /*0H*/Method v = prop;
   self->impl.method = v.impl;
   return prop;
}
/*regGet*/inline ClassTemplateArgument::method_Prop::operator /*0I*/Method () const
{
   SELF(ClassTemplateArgument, method);
   Method value(((C(ClassTemplateArgument) *)&self->impl)->method);
   return value;
}



// member accessors: ClassTemplateParameter::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ClassTemplateParameter TClassTemplateParameter<TC, TCO>::prev_Prop::operator= (/*0H*/ClassTemplateParameter & v)
{
   SELF(ClassTemplateParameter, prev);
   ((C(ClassTemplateParameter) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::prev_Prop & TClassTemplateParameter<TC, TCO>::prev_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::prev_Prop & prop)
{
   SELF(ClassTemplateParameter, prev);
   /*0H*/ClassTemplateParameter v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::prev_Prop::operator /*0I*/ClassTemplateParameter () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, prev);
   ClassTemplateParameter value(((C(ClassTemplateParameter) *)self->impl)->prev);
   return value;
}

// member accessors: ClassTemplateParameter::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ClassTemplateParameter TClassTemplateParameter<TC, TCO>::next_Prop::operator= (/*0H*/ClassTemplateParameter & v)
{
   SELF(ClassTemplateParameter, next);
   ((C(ClassTemplateParameter) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::next_Prop & TClassTemplateParameter<TC, TCO>::next_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::next_Prop & prop)
{
   SELF(ClassTemplateParameter, next);
   /*0H*/ClassTemplateParameter v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::next_Prop::operator /*0I*/ClassTemplateParameter () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, next);
   ClassTemplateParameter value(((C(ClassTemplateParameter) *)self->impl)->next);
   return value;
}

// member accessors: ClassTemplateParameter::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClassTemplateParameter<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(ClassTemplateParameter, name);
   ((C(ClassTemplateParameter) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::name_Prop & TClassTemplateParameter<TC, TCO>::name_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::name_Prop & prop)
{
   SELF(ClassTemplateParameter, name);
   /*0H*/const char * v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, name);
   return self ? ((C(ClassTemplateParameter) *)self->impl)->name : null;
}

// member accessors: ClassTemplateParameter::type
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/TemplateParameterType TClassTemplateParameter<TC, TCO>::type_Prop::operator= (/*0H*/TemplateParameterType v)
{
   SELF(ClassTemplateParameter, type);
   ((C(ClassTemplateParameter) *)self->impl)->type = (C(TemplateParameterType))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::type_Prop & TClassTemplateParameter<TC, TCO>::type_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::type_Prop & prop)
{
   SELF(ClassTemplateParameter, type);
   /*0H*/TemplateParameterType v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->type = (C(TemplateParameterType))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::type_Prop::operator /*0I*/TemplateParameterType () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, type);
   return self ? ((C(ClassTemplateParameter) *)self->impl)->type : (TemplateParameterType)0;
}

// member accessors: ClassTemplateParameter::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TClassTemplateParameter<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(ClassTemplateParameter, dataTypeString);
   ((C(ClassTemplateParameter) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::dataTypeString_Prop & TClassTemplateParameter<TC, TCO>::dataTypeString_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(ClassTemplateParameter, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, dataTypeString);
   return self ? ((C(ClassTemplateParameter) *)self->impl)->dataTypeString : null;
}

// member accessors: ClassTemplateParameter::memberType
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/TemplateMemberType TClassTemplateParameter<TC, TCO>::memberType_Prop::operator= (/*0H*/TemplateMemberType v)
{
   SELF(ClassTemplateParameter, memberType);
   ((C(ClassTemplateParameter) *)self->impl)->memberType = (C(TemplateMemberType))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::memberType_Prop & TClassTemplateParameter<TC, TCO>::memberType_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::memberType_Prop & prop)
{
   SELF(ClassTemplateParameter, memberType);
   /*0H*/TemplateMemberType v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->memberType = (C(TemplateMemberType))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::memberType_Prop::operator /*0I*/TemplateMemberType () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, memberType);
   return self ? ((C(ClassTemplateParameter) *)self->impl)->memberType : (TemplateMemberType)0;
}

// member accessors: ClassTemplateParameter::defaultArg
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ClassTemplateArgument TClassTemplateParameter<TC, TCO>::defaultArg_Prop::operator= (/*0H*/ClassTemplateArgument v)
{
   SELF(ClassTemplateParameter, defaultArg);
   ((C(ClassTemplateParameter) *)self->impl)->defaultArg = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::defaultArg_Prop & TClassTemplateParameter<TC, TCO>::defaultArg_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::defaultArg_Prop & prop)
{
   SELF(ClassTemplateParameter, defaultArg);
   /*0H*/ClassTemplateArgument v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->defaultArg = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::defaultArg_Prop::operator /*0I*/ClassTemplateArgument () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, defaultArg);
   ClassTemplateArgument value(((C(ClassTemplateParameter) *)self->impl)->defaultArg);
   return value;
}

// member accessors: ClassTemplateParameter::param
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TClassTemplateParameter<TC, TCO>::param_Prop::operator= (/*0H*/void * v)
{
   SELF(ClassTemplateParameter, param);
   ((C(ClassTemplateParameter) *)self->impl)->param = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TClassTemplateParameter<TC, TCO>::param_Prop & TClassTemplateParameter<TC, TCO>::param_Prop::operator= (typename TClassTemplateParameter<TC, TCO>::param_Prop & prop)
{
   SELF(ClassTemplateParameter, param);
   /*0H*/void * v = prop;
   ((C(ClassTemplateParameter) *)self->impl)->param = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TClassTemplateParameter<TC, TCO>::param_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TClassTemplateParameter TCTCO * self = CONTAINER_OF(this, TClassTemplateParameter TCTCO, param);
   return self ? ((C(ClassTemplateParameter) *)self->impl)->param : null;
}

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_add_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_add_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), add);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_add_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_add_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_add_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, add)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_add_Functor::operator()( /*6Ej*/TP_T value)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), add);
   C(IteratorPointer) * retIteratorPointer = Container_add(self ? self->impl : (C(Container))null, /*7Al*/toTA<TP_T>(value));
      return retIteratorPointer;
}
// inline void Container::register_add(CPPClass & cl, Container::Container_add_Functor::FunctionType func)
// {
//    ((Container::Container_add_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, add)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_copy_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_copy_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), copy);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_copy_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_copy_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_copy_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, copy)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_copy_Functor::operator()( /*6Fj*/TContainer & source)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), copy);
   Container_copy(self ? self->impl : (C(Container))null, /*7Al*/((Instance&)source).impl);
}
// inline void Container::register_copy(CPPClass & cl, Container::Container_copy_Functor::FunctionType func)
// {
//    ((Container::Container_copy_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, copy)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_delete_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_delete_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), delete_);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_delete_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_delete_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_delete_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, delete)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_delete_Functor::operator()( /*6Fj*/IteratorPointer it)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), delete_);
   Container_delete(self ? self->impl : (C(Container))null, /*7Al*/it.impl);
}
// inline void Container::register_delete(CPPClass & cl, Container::Container_delete_Functor::FunctionType func)
// {
//    ((Container::Container_delete_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, delete)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_find_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_find_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), find);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_find_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_find_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_find_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, find)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_find_Functor::operator()( /*6Ej*/TP_D value)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), find);
   C(IteratorPointer) * retIteratorPointer = Container_find(self ? self->impl : (C(Container))null, /*7Al*/toTA<TP_D>(value));
      return retIteratorPointer;
}
// inline void Container::register_find(CPPClass & cl, Container::Container_find_Functor::FunctionType func)
// {
//    ((Container::Container_find_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, find)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_free_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_free_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), free);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_free_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_free_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_free_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, free)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_free_Functor::operator()( )
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), free);
   Container_free(self ? self->impl : (C(Container))null);
}
// inline void Container::register_free(CPPClass & cl, Container::Container_free_Functor::FunctionType func)
// {
//    ((Container::Container_free_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, free)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_freeIterator_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_freeIterator_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), freeIterator);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_freeIterator_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_freeIterator_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_freeIterator_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, freeIterator)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_freeIterator_Functor::operator()( /*6Fj*/IteratorPointer it)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), freeIterator);
   Container_freeIterator(self ? self->impl : (C(Container))null, /*7Al*/it.impl);
}
// inline void Container::register_freeIterator(CPPClass & cl, Container::Container_freeIterator_Functor::FunctionType func)
// {
//    ((Container::Container_freeIterator_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, freeIterator)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getAtPosition_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getAtPosition_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getAtPosition);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getAtPosition_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getAtPosition_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getAtPosition_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, getAtPosition)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getAtPosition_Functor::operator()( /*6Ej*/TP_I pos, /*6Fj*/bool create, /*6Cj*/C(bool) * justAdded)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getAtPosition);
   C(IteratorPointer) * retIteratorPointer = Container_getAtPosition(self ? self->impl : (C(Container))null, /*7Al*/toTA<TP_I>(pos), /*7Al*/(C(bool))create, /*7Al*/justAdded);
      return retIteratorPointer;
}
// inline void Container::register_getAtPosition(CPPClass & cl, Container::Container_getAtPosition_Functor::FunctionType func)
// {
//    ((Container::Container_getAtPosition_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, getAtPosition)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getCount_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getCount_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getCount);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getCount_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getCount_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getCount_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, getCount)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline int TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getCount_Functor::operator()( )
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getCount);
   return Container_getCount(self ? self->impl : (C(Container))null);
}
// inline void Container::register_getCount(CPPClass & cl, Container::Container_getCount_Functor::FunctionType func)
// {
//    ((Container::Container_getCount_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, getCount)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getData_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getData_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getData);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getData_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getData_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getData_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, getData)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline TP_D TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getData_Functor::operator()( /*6Fj*/IteratorPointer pointer)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getData);
   return fromTA<TP_D>(Container_getData(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl));
}
// inline void Container::register_getData(CPPClass & cl, Container::Container_getData_Functor::FunctionType func)
// {
//    ((Container::Container_getData_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, getData)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getFirst_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getFirst_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getFirst);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getFirst_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getFirst_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getFirst_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, getFirst)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getFirst_Functor::operator()( )
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getFirst);
   C(IteratorPointer) * retIteratorPointer = Container_getFirst(self ? self->impl : (C(Container))null);
      return retIteratorPointer;
}
// inline void Container::register_getFirst(CPPClass & cl, Container::Container_getFirst_Functor::FunctionType func)
// {
//    ((Container::Container_getFirst_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, getFirst)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getLast_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getLast_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getLast);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getLast_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getLast_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getLast_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, getLast)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getLast_Functor::operator()( )
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getLast);
   C(IteratorPointer) * retIteratorPointer = Container_getLast(self ? self->impl : (C(Container))null);
      return retIteratorPointer;
}
// inline void Container::register_getLast(CPPClass & cl, Container::Container_getLast_Functor::FunctionType func)
// {
//    ((Container::Container_getLast_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, getLast)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getNext_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getNext_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getNext);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getNext_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getNext_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getNext_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, getNext)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getNext_Functor::operator()( /*6Fj*/IteratorPointer pointer)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getNext);
   C(IteratorPointer) * retIteratorPointer = Container_getNext(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl);
      return retIteratorPointer;
}
// inline void Container::register_getNext(CPPClass & cl, Container::Container_getNext_Functor::FunctionType func)
// {
//    ((Container::Container_getNext_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, getNext)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getPrev_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getPrev_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getPrev);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getPrev_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getPrev_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getPrev_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, getPrev)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_getPrev_Functor::operator()( /*6Fj*/IteratorPointer pointer)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), getPrev);
   C(IteratorPointer) * retIteratorPointer = Container_getPrev(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl);
      return retIteratorPointer;
}
// inline void Container::register_getPrev(CPPClass & cl, Container::Container_getPrev_Functor::FunctionType func)
// {
//    ((Container::Container_getPrev_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, getPrev)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_insert_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_insert_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), insert);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_insert_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_insert_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_insert_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, insert)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline IteratorPointer TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_insert_Functor::operator()( /*6Fj*/IteratorPointer after, /*6Ej*/TP_T value)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), insert);
   C(IteratorPointer) * retIteratorPointer = Container_insert(self ? self->impl : (C(Container))null, /*7Al*/after.impl, /*7Al*/toTA<TP_T>(value));
      return retIteratorPointer;
}
// inline void Container::register_insert(CPPClass & cl, Container::Container_insert_Functor::FunctionType func)
// {
//    ((Container::Container_insert_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, insert)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_move_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_move_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), move);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_move_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_move_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_move_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, move)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_move_Functor::operator()( /*6Fj*/IteratorPointer it, /*6Fj*/IteratorPointer after)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), move);
   Container_move(self ? self->impl : (C(Container))null, /*7Al*/it.impl, /*7Al*/after.impl);
}
// inline void Container::register_move(CPPClass & cl, Container::Container_move_Functor::FunctionType func)
// {
//    ((Container::Container_move_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, move)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_remove_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_remove_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), remove);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_remove_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_remove_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_remove_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, remove)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_remove_Functor::operator()( /*6Fj*/IteratorPointer it)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), remove);
   Container_remove(self ? self->impl : (C(Container))null, /*7Al*/it.impl);
}
// inline void Container::register_remove(CPPClass & cl, Container::Container_remove_Functor::FunctionType func)
// {
//    ((Container::Container_remove_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, remove)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_removeAll_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_removeAll_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), removeAll);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_removeAll_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_removeAll_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_removeAll_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, removeAll)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_removeAll_Functor::operator()( )
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), removeAll);
   Container_removeAll(self ? self->impl : (C(Container))null);
}
// inline void Container::register_removeAll(CPPClass & cl, Container::Container_removeAll_Functor::FunctionType func)
// {
//    ((Container::Container_removeAll_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, removeAll)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_setData_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_setData_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), setData);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_setData_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_setData_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_setData_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, setData)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline bool TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_setData_Functor::operator()( /*6Fj*/IteratorPointer pointer, /*6Ej*/TP_D data)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), setData);
   return (bool)Container_setData(self ? self->impl : (C(Container))null, /*7Al*/pointer.impl, /*7Al*/toTA<TP_D>(data));
}
// inline void Container::register_setData(CPPClass & cl, Container::Container_setData_Functor::FunctionType func)
// {
//    ((Container::Container_setData_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, setData)] = func;
// }

template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline typename TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_sort_Functor::FunctionType TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_sort_Functor::operator= (FunctionType func)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), sort);
   if(self->vTbl == TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl)
   {
      uint size = TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())eC_new(sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_sort_Functor::FunctionType) * size);
      memcpy(self->vTbl, TContainer <TP_T _ARG TP_I _ARG TP_D>::_cpp_class.vTbl, sizeof(TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_sort_Functor::FunctionType) * size);
   }
   ((TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_sort_Functor::FunctionType *)self->vTbl)[M_VTBLID(Container, sort)] = func;
   return func;
}
template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T>
inline void TContainer <TP_T _ARG TP_I _ARG TP_D>::Container_sort_Functor::operator()( /*6Fj*/bool ascending)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), sort);
   Container_sort(self ? self->impl : (C(Container))null, /*7Al*/(C(bool))ascending);
}
// inline void Container::register_sort(CPPClass & cl, Container::Container_sort_Functor::FunctionType func)
// {
//    ((Container::Container_sort_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Container, sort)] = func;
// }

template <typename TP_T, typename TP_I, typename TP_D> TCPPClass<TContainer<TP_T, TP_I, TP_D>> TContainer<TP_T, TP_I, TP_D>::_cpp_class;

template <typename TP_T, typename TP_I, typename TP_D> void TContainer<TP_T, TP_I, TP_D>::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "Copy", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(Container) source)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, copy);
         typename Container_copy_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_copy_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<TContainer> source_l(source); fn(*i, /*3Bd*/*source_l);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(Container) source))(CO(Container)->_vTbl)[M_VTBLID(Container, copy)]);
            if(method) return method (o_, source);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "Delete", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, delete);
         typename Container_delete_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_delete_Functor::FunctionType) i->vTbl[vid];
            /*2Cg*/IteratorPointer it_l(it); fn(*i, /*3Cd*/it_l);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it))(CO(Container)->_vTbl)[M_VTBLID(Container, delete)]);
            if(method) return method (o_, it);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "Free", (void *) +[](/*1Aa*/C(Container) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, free);
         typename Container_free_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_free_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_))(CO(Container)->_vTbl)[M_VTBLID(Container, free)]);
            if(method) return method (o_);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "FreeIterator", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, freeIterator);
         typename Container_freeIterator_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_freeIterator_Functor::FunctionType) i->vTbl[vid];
            /*2Cg*/IteratorPointer it_l(it); fn(*i, /*3Cd*/it_l);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it))(CO(Container)->_vTbl)[M_VTBLID(Container, freeIterator)]);
            if(method) return method (o_, it);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "GetCount", (void *) +[](/*1Aa*/C(Container) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, getCount);
         typename Container_getCount_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_getCount_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(Container) o_))(CO(Container)->_vTbl)[M_VTBLID(Container, getCount)]);
            if(method) return method (o_);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "GetData", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * pointer)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, getData);
         typename Container_getData_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_getData_Functor::FunctionType) i->vTbl[vid];
            /*2Cg*/IteratorPointer pointer_l(pointer); TP_D ret = fn(*i, /*3Cd*/pointer_l); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((TP_D (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * pointer))(CO(Container)->_vTbl)[M_VTBLID(Container, getData)]);
            if(method) return method (o_, pointer);
         }
         return (TP_D)1;
      });


      addMethod(_cpp_class.impl, "GetFirst", (void *) +[](/*1Aa*/C(Container) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, getFirst);
         typename Container_getFirst_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_getFirst_Functor::FunctionType) i->vTbl[vid];
            C(IteratorPointer) * ret = fn(*i).impl; return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(IteratorPointer) * (*) (/*1Aa*/C(Container) o_))(CO(Container)->_vTbl)[M_VTBLID(Container, getFirst)]);
            if(method) return method (o_);
         }
         return (C(IteratorPointer) *)null;
      });


      addMethod(_cpp_class.impl, "GetLast", (void *) +[](/*1Aa*/C(Container) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, getLast);
         typename Container_getLast_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_getLast_Functor::FunctionType) i->vTbl[vid];
            C(IteratorPointer) * ret = fn(*i).impl; return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(IteratorPointer) * (*) (/*1Aa*/C(Container) o_))(CO(Container)->_vTbl)[M_VTBLID(Container, getLast)]);
            if(method) return method (o_);
         }
         return (C(IteratorPointer) *)null;
      });


      addMethod(_cpp_class.impl, "GetNext", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * pointer)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, getNext);
         typename Container_getNext_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_getNext_Functor::FunctionType) i->vTbl[vid];
            /*2Cg*/IteratorPointer pointer_l(pointer); C(IteratorPointer) * ret = fn(*i, /*3Cd*/pointer_l).impl; return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(IteratorPointer) * (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * pointer))(CO(Container)->_vTbl)[M_VTBLID(Container, getNext)]);
            if(method) return method (o_, pointer);
         }
         return (C(IteratorPointer) *)null;
      });


      addMethod(_cpp_class.impl, "GetPrev", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * pointer)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, getPrev);
         typename Container_getPrev_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_getPrev_Functor::FunctionType) i->vTbl[vid];
            /*2Cg*/IteratorPointer pointer_l(pointer); C(IteratorPointer) * ret = fn(*i, /*3Cd*/pointer_l).impl; return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(IteratorPointer) * (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * pointer))(CO(Container)->_vTbl)[M_VTBLID(Container, getPrev)]);
            if(method) return method (o_, pointer);
         }
         return (C(IteratorPointer) *)null;
      });


      addMethod(_cpp_class.impl, "Move", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it, /*1Aa*/C(IteratorPointer) * after)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, move);
         typename Container_move_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_move_Functor::FunctionType) i->vTbl[vid];
            /*2Cg*/IteratorPointer it_l(it); /*2Cg*/IteratorPointer after_l(after); fn(*i, /*3Cd*/it_l, /*3Cd*/after_l);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it, /*1Aa*/C(IteratorPointer) * after))(CO(Container)->_vTbl)[M_VTBLID(Container, move)]);
            if(method) return method (o_, it, after);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "Remove", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, remove);
         typename Container_remove_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_remove_Functor::FunctionType) i->vTbl[vid];
            /*2Cg*/IteratorPointer it_l(it); fn(*i, /*3Cd*/it_l);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(IteratorPointer) * it))(CO(Container)->_vTbl)[M_VTBLID(Container, remove)]);
            if(method) return method (o_, it);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "RemoveAll", (void *) +[](/*1Aa*/C(Container) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, removeAll);
         typename Container_removeAll_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_removeAll_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_))(CO(Container)->_vTbl)[M_VTBLID(Container, removeAll)]);
            if(method) return method (o_);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "Sort", (void *) +[](/*1Aa*/C(Container) o_, /*1Aa*/C(bool) ascending)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         TContainer <TP_T _ARG TP_I _ARG TP_D> * i = (o_) ? (TContainer <TP_T _ARG TP_I _ARG TP_D> *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Container, sort);
         typename Container_sort_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (typename Container_sort_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Hd*/(bool)ascending);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Container) o_, /*1Aa*/C(bool) ascending))(CO(Container)->_vTbl)[M_VTBLID(Container, sort)]);
            if(method) return method (o_, ascending);
         }
         return ;
      });


}


// property callers: Container::copySrc   // set only
// (normal::normal)

/*nstSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline /*0A*/const TContainer<TP_T, TP_I, TP_D> & TContainer<TP_T _ARG TP_I _ARG TP_D> ::copySrc_Prop::operator= (/*0A*/const TContainer<TP_T, TP_I, TP_D> & v)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), copySrc);
   Container_set_copySrc(self ? self->impl : null, ((Instance *)&v)->impl);
   return v;
}

/*nstSet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline /*0C*/const TContainer<TP_T, TP_I, TP_D> * TContainer<TP_T _ARG TP_I _ARG TP_D> ::copySrc_Prop::operator= (/*0C*/const TContainer<TP_T, TP_I, TP_D> * v)
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), copySrc);
   Container_set_copySrc(self ? self->impl : null, v ? ((Instance *)v)->impl : null);
   return v;
}


// property callers: Container::firstIterator   // get only
// (normal::struct)

/*regGet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline TContainer<TP_T _ARG TP_I _ARG TP_D> ::firstIterator_Prop::operator /*0I*/TIterator<TP_T, TP_D> () const
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), firstIterator);
   TIterator<TP_T, TP_D> value;Container_get_firstIterator(self->impl, &value.impl);
   return value;
}

// property callers: Container::lastIterator   // get only
// (normal::struct)

/*regGet*/template <typename TP_T _ARG typename TP_I = int _ARG typename TP_D = TP_T> inline TContainer<TP_T _ARG TP_I _ARG TP_D> ::lastIterator_Prop::operator /*0I*/TIterator<TP_T, TP_D> () const
{
   TSELF(Container, (<TP_T _ARG TP_I _ARG TP_D>), lastIterator);
   TIterator<TP_T, TP_D> value;Container_get_lastIterator(self->impl, &value.impl);
   return value;
}

template <typename TP_BT, typename TP_KT> TCPPClass<TCustomAVLTree<TP_BT, TP_KT>> TCustomAVLTree<TP_BT, TP_KT>::_cpp_class;

template <typename TP_BT, typename TP_KT> void TCustomAVLTree<TP_BT, TP_KT>::class_registration(CPPClass & _cpp_class)
{
}


// member accessors: CustomAVLTree::root
// (normal::BT)

/*nstSet*/template <typename TP_BT _ARG typename TP_KT = uint64> inline /*0H*/TP_BT TCustomAVLTree<TP_BT _ARG TP_KT> ::root_Prop::operator= (/*0H*/TP_BT v)
{
   TSELF(CustomAVLTree, (<TP_BT _ARG TP_KT>), root);
   IPTR(self->impl, CustomAVLTree)->root = v;
   return v;
}

/*regSet*/template <typename TP_BT _ARG typename TP_KT = uint64> inline typename TCustomAVLTree<TP_BT _ARG TP_KT>::root_Prop & TCustomAVLTree<TP_BT _ARG TP_KT>::root_Prop::operator= (TCustomAVLTree<TP_BT _ARG TP_KT>::root_Prop & prop)
{
   TSELF(CustomAVLTree, (<TP_BT _ARG TP_KT>), root);
   /*0H*/TP_BT v = prop;
   IPTR(self->impl, CustomAVLTree)->root = v;
   return prop;
}
/*regGet*/template <typename TP_BT _ARG typename TP_KT = uint64> inline TCustomAVLTree<TP_BT _ARG TP_KT> ::root_Prop::operator /*0I*/TP_BT () const
{
   TSELF(CustomAVLTree, (<TP_BT _ARG TP_KT>), root);
   return self ? IPTR(self->impl, CustomAVLTree)->root : 0;
}

// member accessors: CustomAVLTree::count
// (normal::int)

/*nstSet*/template <typename TP_BT _ARG typename TP_KT = uint64> inline /*0H*/int TCustomAVLTree<TP_BT _ARG TP_KT> ::count_Prop::operator= (/*0H*/int v)
{
   TSELF(CustomAVLTree, (<TP_BT _ARG TP_KT>), count);
   IPTR(self->impl, CustomAVLTree)->count = v;
   return v;
}

/*regSet*/template <typename TP_BT _ARG typename TP_KT = uint64> inline typename TCustomAVLTree<TP_BT _ARG TP_KT>::count_Prop & TCustomAVLTree<TP_BT _ARG TP_KT>::count_Prop::operator= (TCustomAVLTree<TP_BT _ARG TP_KT>::count_Prop & prop)
{
   TSELF(CustomAVLTree, (<TP_BT _ARG TP_KT>), count);
   /*0H*/int v = prop;
   IPTR(self->impl, CustomAVLTree)->count = v;
   return prop;
}
/*regGet*/template <typename TP_BT _ARG typename TP_KT = uint64> inline TCustomAVLTree<TP_BT _ARG TP_KT> ::count_Prop::operator /*0I*/int () const
{
   TSELF(CustomAVLTree, (<TP_BT _ARG TP_KT>), count);
   return self ? IPTR(self->impl, CustomAVLTree)->count : 0;
}



// member accessors: DataMember::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/DataMember TDataMember<TC, TCO>::prev_Prop::operator= (/*0H*/DataMember & v)
{
   SELF(DataMember, prev);
   ((C(DataMember) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::prev_Prop & TDataMember<TC, TCO>::prev_Prop::operator= (typename TDataMember<TC, TCO>::prev_Prop & prop)
{
   SELF(DataMember, prev);
   /*0H*/DataMember v = prop;
   ((C(DataMember) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::prev_Prop::operator /*0I*/DataMember () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, prev);
   DataMember value(((C(DataMember) *)self->impl)->prev);
   return value;
}

// member accessors: DataMember::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/DataMember TDataMember<TC, TCO>::next_Prop::operator= (/*0H*/DataMember & v)
{
   SELF(DataMember, next);
   ((C(DataMember) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::next_Prop & TDataMember<TC, TCO>::next_Prop::operator= (typename TDataMember<TC, TCO>::next_Prop & prop)
{
   SELF(DataMember, next);
   /*0H*/DataMember v = prop;
   ((C(DataMember) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::next_Prop::operator /*0I*/DataMember () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, next);
   DataMember value(((C(DataMember) *)self->impl)->next);
   return value;
}

// member accessors: DataMember::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TDataMember<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(DataMember, name);
   ((C(DataMember) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::name_Prop & TDataMember<TC, TCO>::name_Prop::operator= (typename TDataMember<TC, TCO>::name_Prop & prop)
{
   SELF(DataMember, name);
   /*0H*/const char * v = prop;
   ((C(DataMember) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, name);
   return self ? ((C(DataMember) *)self->impl)->name : null;
}

// member accessors: DataMember::isProperty
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TDataMember<TC, TCO>::isProperty_Prop::operator= (/*0H*/bool v)
{
   SELF(DataMember, isProperty);
   ((C(DataMember) *)self->impl)->isProperty = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::isProperty_Prop & TDataMember<TC, TCO>::isProperty_Prop::operator= (typename TDataMember<TC, TCO>::isProperty_Prop & prop)
{
   SELF(DataMember, isProperty);
   /*0H*/bool v = prop;
   ((C(DataMember) *)self->impl)->isProperty = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::isProperty_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, isProperty);
   return self ? ((C(DataMember) *)self->impl)->isProperty : (bool)0;
}

// member accessors: DataMember::memberAccess
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Access_Mode TDataMember<TC, TCO>::memberAccess_Prop::operator= (/*0H*/Access_Mode v)
{
   SELF(DataMember, memberAccess);
   ((C(DataMember) *)self->impl)->memberAccess = (C(AccessMode))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::memberAccess_Prop & TDataMember<TC, TCO>::memberAccess_Prop::operator= (typename TDataMember<TC, TCO>::memberAccess_Prop & prop)
{
   SELF(DataMember, memberAccess);
   /*0H*/Access_Mode v = prop;
   ((C(DataMember) *)self->impl)->memberAccess = (C(AccessMode))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::memberAccess_Prop::operator /*0I*/Access_Mode () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, memberAccess);
   return self ? ((C(DataMember) *)self->impl)->memberAccess : (Access_Mode)0;
}

// member accessors: DataMember::id
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TDataMember<TC, TCO>::id_Prop::operator= (/*0H*/int v)
{
   SELF(DataMember, id);
   ((C(DataMember) *)self->impl)->id = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::id_Prop & TDataMember<TC, TCO>::id_Prop::operator= (typename TDataMember<TC, TCO>::id_Prop & prop)
{
   SELF(DataMember, id);
   /*0H*/int v = prop;
   ((C(DataMember) *)self->impl)->id = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::id_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, id);
   return self ? ((C(DataMember) *)self->impl)->id : 0;
}

// member accessors: DataMember::_class
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TDataMember<TC, TCO>::_class_Prop::operator= (/*0H*/Class & v)
{
   SELF(DataMember, _class);
   ((C(DataMember) *)self->impl)->_class = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::_class_Prop & TDataMember<TC, TCO>::_class_Prop::operator= (typename TDataMember<TC, TCO>::_class_Prop & prop)
{
   SELF(DataMember, _class);
   /*0H*/Class v = prop;
   ((C(DataMember) *)self->impl)->_class = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::_class_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, _class);
   Class value(((C(DataMember) *)self->impl)->_class);
   return value;
}

// member accessors: DataMember::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TDataMember<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(DataMember, dataTypeString);
   ((C(DataMember) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::dataTypeString_Prop & TDataMember<TC, TCO>::dataTypeString_Prop::operator= (typename TDataMember<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(DataMember, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(DataMember) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, dataTypeString);
   return self ? ((C(DataMember) *)self->impl)->dataTypeString : null;
}

// member accessors: DataMember::dataTypeClass
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TDataMember<TC, TCO>::dataTypeClass_Prop::operator= (/*0H*/Class & v)
{
   SELF(DataMember, dataTypeClass);
   ((C(DataMember) *)self->impl)->dataTypeClass = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::dataTypeClass_Prop & TDataMember<TC, TCO>::dataTypeClass_Prop::operator= (typename TDataMember<TC, TCO>::dataTypeClass_Prop & prop)
{
   SELF(DataMember, dataTypeClass);
   /*0H*/Class v = prop;
   ((C(DataMember) *)self->impl)->dataTypeClass = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::dataTypeClass_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, dataTypeClass);
   Class value(((C(DataMember) *)self->impl)->dataTypeClass);
   return value;
}

// member accessors: DataMember::type
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/DataMemberType TDataMember<TC, TCO>::type_Prop::operator= (/*0H*/DataMemberType v)
{
   SELF(DataMember, type);
   ((C(DataMember) *)self->impl)->type = (C(DataMemberType))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::type_Prop & TDataMember<TC, TCO>::type_Prop::operator= (typename TDataMember<TC, TCO>::type_Prop & prop)
{
   SELF(DataMember, type);
   /*0H*/DataMemberType v = prop;
   ((C(DataMember) *)self->impl)->type = (C(DataMemberType))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::type_Prop::operator /*0I*/DataMemberType () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, type);
   return self ? ((C(DataMember) *)self->impl)->type : (DataMemberType)0;
}

// member accessors: DataMember::offset
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TDataMember<TC, TCO>::offset_Prop::operator= (/*0H*/int v)
{
   SELF(DataMember, offset);
   ((C(DataMember) *)self->impl)->offset = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::offset_Prop & TDataMember<TC, TCO>::offset_Prop::operator= (typename TDataMember<TC, TCO>::offset_Prop & prop)
{
   SELF(DataMember, offset);
   /*0H*/int v = prop;
   ((C(DataMember) *)self->impl)->offset = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::offset_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, offset);
   return self ? ((C(DataMember) *)self->impl)->offset : 0;
}

// member accessors: DataMember::memberID
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TDataMember<TC, TCO>::memberID_Prop::operator= (/*0H*/int v)
{
   SELF(DataMember, memberID);
   ((C(DataMember) *)self->impl)->memberID = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::memberID_Prop & TDataMember<TC, TCO>::memberID_Prop::operator= (typename TDataMember<TC, TCO>::memberID_Prop & prop)
{
   SELF(DataMember, memberID);
   /*0H*/int v = prop;
   ((C(DataMember) *)self->impl)->memberID = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::memberID_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, memberID);
   return self ? ((C(DataMember) *)self->impl)->memberID : 0;
}

// member accessors: DataMember::members
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TDataMember<TC, TCO>::members_Prop::operator= (/*0H*/OldList v)
{
   SELF(DataMember, members);
   ((C(DataMember) *)self->impl)->members = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::members_Prop & TDataMember<TC, TCO>::members_Prop::operator= (typename TDataMember<TC, TCO>::members_Prop & prop)
{
   SELF(DataMember, members);
   /*0H*/OldList v = prop;
   ((C(DataMember) *)self->impl)->members = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::members_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, members);
   OldList value(((C(DataMember) *)self->impl)->members);
   return value;
}

// member accessors: DataMember::membersAlpha
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BinaryTree TDataMember<TC, TCO>::membersAlpha_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(DataMember, membersAlpha);
   ((C(DataMember) *)self->impl)->membersAlpha = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::membersAlpha_Prop & TDataMember<TC, TCO>::membersAlpha_Prop::operator= (typename TDataMember<TC, TCO>::membersAlpha_Prop & prop)
{
   SELF(DataMember, membersAlpha);
   /*0H*/BinaryTree v = prop;
   ((C(DataMember) *)self->impl)->membersAlpha = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::membersAlpha_Prop::operator /*0I*/BinaryTree () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, membersAlpha);
   BinaryTree value(((C(DataMember) *)self->impl)->membersAlpha);
   return value;
}

// member accessors: DataMember::memberOffset
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TDataMember<TC, TCO>::memberOffset_Prop::operator= (/*0H*/int v)
{
   SELF(DataMember, memberOffset);
   ((C(DataMember) *)self->impl)->memberOffset = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::memberOffset_Prop & TDataMember<TC, TCO>::memberOffset_Prop::operator= (typename TDataMember<TC, TCO>::memberOffset_Prop & prop)
{
   SELF(DataMember, memberOffset);
   /*0H*/int v = prop;
   ((C(DataMember) *)self->impl)->memberOffset = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::memberOffset_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, memberOffset);
   return self ? ((C(DataMember) *)self->impl)->memberOffset : 0;
}

// member accessors: DataMember::structAlignment
// (nohead::short)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/short TDataMember<TC, TCO>::structAlignment_Prop::operator= (/*0H*/short v)
{
   SELF(DataMember, structAlignment);
   ((C(DataMember) *)self->impl)->structAlignment = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::structAlignment_Prop & TDataMember<TC, TCO>::structAlignment_Prop::operator= (typename TDataMember<TC, TCO>::structAlignment_Prop & prop)
{
   SELF(DataMember, structAlignment);
   /*0H*/short v = prop;
   ((C(DataMember) *)self->impl)->structAlignment = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::structAlignment_Prop::operator /*0I*/short () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, structAlignment);
   return self ? ((C(DataMember) *)self->impl)->structAlignment : 0;
}

// member accessors: DataMember::pointerAlignment
// (nohead::short)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/short TDataMember<TC, TCO>::pointerAlignment_Prop::operator= (/*0H*/short v)
{
   SELF(DataMember, pointerAlignment);
   ((C(DataMember) *)self->impl)->pointerAlignment = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDataMember<TC, TCO>::pointerAlignment_Prop & TDataMember<TC, TCO>::pointerAlignment_Prop::operator= (typename TDataMember<TC, TCO>::pointerAlignment_Prop & prop)
{
   SELF(DataMember, pointerAlignment);
   /*0H*/short v = prop;
   ((C(DataMember) *)self->impl)->pointerAlignment = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDataMember<TC, TCO>::pointerAlignment_Prop::operator /*0I*/short () const
{
   __attribute__((unused)) TDataMember TCTCO * self = CONTAINER_OF(this, TDataMember TCTCO, pointerAlignment);
   return self ? ((C(DataMember) *)self->impl)->pointerAlignment : 0;
}

inline DataValue::DataValue(char c, unsigned char uc = 0, short s = 0, uint16 us = 0, int i = 0, uint ui = 0, void * p = null, float f = 0, double d = 0, int64 i64 = 0, uint64 ui64 = 0)
{
   impl.c = c;
   impl.uc = uc;
   impl.s = s;
   impl.us = us;
   impl.i = i;
   impl.ui = ui;
   impl.p = p;
   impl.f = f;
   impl.d = d;
   impl.i64 = i64;
   impl.ui64 = ui64;
}



// member accessors: DataValue::c
// (struct::char)

/*nstSet*/inline /*0H*/char DataValue::c_Prop::operator= (/*0H*/char v)
{
   SELF(DataValue, c);
   self->impl.c = v;
   return v;
}

/*regSet*/inline DataValue::c_Prop & DataValue::c_Prop::operator= (DataValue::c_Prop & prop)
{
   SELF(DataValue, c);
   /*0H*/char v = prop;
   self->impl.c = v;
   return prop;
}
/*regGet*/inline DataValue::c_Prop::operator /*0I*/char () const
{
   SELF(DataValue, c);
   return self ? ((C(DataValue) *)&self->impl)->c : 0;
}

// member accessors: DataValue::uc
// (struct::byte)

/*nstSet*/inline /*0H*/unsigned char DataValue::uc_Prop::operator= (/*0H*/unsigned char v)
{
   SELF(DataValue, uc);
   self->impl.uc = v;
   return v;
}

/*regSet*/inline DataValue::uc_Prop & DataValue::uc_Prop::operator= (DataValue::uc_Prop & prop)
{
   SELF(DataValue, uc);
   /*0H*/unsigned char v = prop;
   self->impl.uc = v;
   return prop;
}
/*regGet*/inline DataValue::uc_Prop::operator /*0I*/unsigned char () const
{
   SELF(DataValue, uc);
   return self ? ((C(DataValue) *)&self->impl)->uc : 0;
}

// member accessors: DataValue::s
// (struct::short)

/*nstSet*/inline /*0H*/short DataValue::s_Prop::operator= (/*0H*/short v)
{
   SELF(DataValue, s);
   self->impl.s = v;
   return v;
}

/*regSet*/inline DataValue::s_Prop & DataValue::s_Prop::operator= (DataValue::s_Prop & prop)
{
   SELF(DataValue, s);
   /*0H*/short v = prop;
   self->impl.s = v;
   return prop;
}
/*regGet*/inline DataValue::s_Prop::operator /*0I*/short () const
{
   SELF(DataValue, s);
   return self ? ((C(DataValue) *)&self->impl)->s : 0;
}

// member accessors: DataValue::us
// (struct::uint16)

/*nstSet*/inline /*0H*/uint16 DataValue::us_Prop::operator= (/*0H*/uint16 v)
{
   SELF(DataValue, us);
   self->impl.us = v;
   return v;
}

/*regSet*/inline DataValue::us_Prop & DataValue::us_Prop::operator= (DataValue::us_Prop & prop)
{
   SELF(DataValue, us);
   /*0H*/uint16 v = prop;
   self->impl.us = v;
   return prop;
}
/*regGet*/inline DataValue::us_Prop::operator /*0I*/uint16 () const
{
   SELF(DataValue, us);
   return self ? ((C(DataValue) *)&self->impl)->us : 0;
}

// member accessors: DataValue::i
// (struct::int)

/*nstSet*/inline /*0H*/int DataValue::i_Prop::operator= (/*0H*/int v)
{
   SELF(DataValue, i);
   self->impl.i = v;
   return v;
}

/*regSet*/inline DataValue::i_Prop & DataValue::i_Prop::operator= (DataValue::i_Prop & prop)
{
   SELF(DataValue, i);
   /*0H*/int v = prop;
   self->impl.i = v;
   return prop;
}
/*regGet*/inline DataValue::i_Prop::operator /*0I*/int () const
{
   SELF(DataValue, i);
   return self ? ((C(DataValue) *)&self->impl)->i : 0;
}

// member accessors: DataValue::ui
// (struct::uint)

/*nstSet*/inline /*0H*/uint DataValue::ui_Prop::operator= (/*0H*/uint v)
{
   SELF(DataValue, ui);
   self->impl.ui = v;
   return v;
}

/*regSet*/inline DataValue::ui_Prop & DataValue::ui_Prop::operator= (DataValue::ui_Prop & prop)
{
   SELF(DataValue, ui);
   /*0H*/uint v = prop;
   self->impl.ui = v;
   return prop;
}
/*regGet*/inline DataValue::ui_Prop::operator /*0I*/uint () const
{
   SELF(DataValue, ui);
   return self ? ((C(DataValue) *)&self->impl)->ui : 0;
}

// member accessors: DataValue::p
// (struct::void *)

/*nstSet*/inline /*0H*/void * DataValue::p_Prop::operator= (/*0H*/void * v)
{
   SELF(DataValue, p);
   self->impl.p = v;
   return v;
}

/*regSet*/inline DataValue::p_Prop & DataValue::p_Prop::operator= (DataValue::p_Prop & prop)
{
   SELF(DataValue, p);
   /*0H*/void * v = prop;
   self->impl.p = v;
   return prop;
}
/*regGet*/inline DataValue::p_Prop::operator /*0I*/void * () const
{
   SELF(DataValue, p);
   return self ? ((C(DataValue) *)&self->impl)->p : null;
}

// member accessors: DataValue::f
// (struct::float)

/*nstSet*/inline /*0H*/float DataValue::f_Prop::operator= (/*0H*/float v)
{
   SELF(DataValue, f);
   self->impl.f = v;
   return v;
}

/*regSet*/inline DataValue::f_Prop & DataValue::f_Prop::operator= (DataValue::f_Prop & prop)
{
   SELF(DataValue, f);
   /*0H*/float v = prop;
   self->impl.f = v;
   return prop;
}
/*regGet*/inline DataValue::f_Prop::operator /*0I*/float () const
{
   SELF(DataValue, f);
   return self ? ((C(DataValue) *)&self->impl)->f : 0;
}

// member accessors: DataValue::d
// (struct::double)

/*nstSet*/inline /*0H*/double DataValue::d_Prop::operator= (/*0H*/double v)
{
   SELF(DataValue, d);
   self->impl.d = v;
   return v;
}

/*regSet*/inline DataValue::d_Prop & DataValue::d_Prop::operator= (DataValue::d_Prop & prop)
{
   SELF(DataValue, d);
   /*0H*/double v = prop;
   self->impl.d = v;
   return prop;
}
/*regGet*/inline DataValue::d_Prop::operator /*0I*/double () const
{
   SELF(DataValue, d);
   return self ? ((C(DataValue) *)&self->impl)->d : 0;
}

// member accessors: DataValue::i64
// (struct::int64)

/*nstSet*/inline /*0H*/int64 DataValue::i64_Prop::operator= (/*0H*/int64 v)
{
   SELF(DataValue, i64);
   self->impl.i64 = v;
   return v;
}

/*regSet*/inline DataValue::i64_Prop & DataValue::i64_Prop::operator= (DataValue::i64_Prop & prop)
{
   SELF(DataValue, i64);
   /*0H*/int64 v = prop;
   self->impl.i64 = v;
   return prop;
}
/*regGet*/inline DataValue::i64_Prop::operator /*0I*/int64 () const
{
   SELF(DataValue, i64);
   return self ? ((C(DataValue) *)&self->impl)->i64 : 0;
}

// member accessors: DataValue::ui64
// (struct::uint64)

/*nstSet*/inline /*0H*/uint64 DataValue::ui64_Prop::operator= (/*0H*/uint64 v)
{
   SELF(DataValue, ui64);
   self->impl.ui64 = v;
   return v;
}

/*regSet*/inline DataValue::ui64_Prop & DataValue::ui64_Prop::operator= (DataValue::ui64_Prop & prop)
{
   SELF(DataValue, ui64);
   /*0H*/uint64 v = prop;
   self->impl.ui64 = v;
   return prop;
}
/*regGet*/inline DataValue::ui64_Prop::operator /*0I*/uint64 () const
{
   SELF(DataValue, ui64);
   return self ? ((C(DataValue) *)&self->impl)->ui64 : 0;
}



// member accessors: DefinedExpression::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/DefinedExpression TDefinedExpression<TC, TCO>::prev_Prop::operator= (/*0H*/DefinedExpression & v)
{
   SELF(DefinedExpression, prev);
   ((C(DefinedExpression) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDefinedExpression<TC, TCO>::prev_Prop & TDefinedExpression<TC, TCO>::prev_Prop::operator= (typename TDefinedExpression<TC, TCO>::prev_Prop & prop)
{
   SELF(DefinedExpression, prev);
   /*0H*/DefinedExpression v = prop;
   ((C(DefinedExpression) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDefinedExpression<TC, TCO>::prev_Prop::operator /*0I*/DefinedExpression () const
{
   __attribute__((unused)) TDefinedExpression TCTCO * self = CONTAINER_OF(this, TDefinedExpression TCTCO, prev);
   DefinedExpression value(((C(DefinedExpression) *)self->impl)->prev);
   return value;
}

// member accessors: DefinedExpression::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/DefinedExpression TDefinedExpression<TC, TCO>::next_Prop::operator= (/*0H*/DefinedExpression & v)
{
   SELF(DefinedExpression, next);
   ((C(DefinedExpression) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDefinedExpression<TC, TCO>::next_Prop & TDefinedExpression<TC, TCO>::next_Prop::operator= (typename TDefinedExpression<TC, TCO>::next_Prop & prop)
{
   SELF(DefinedExpression, next);
   /*0H*/DefinedExpression v = prop;
   ((C(DefinedExpression) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDefinedExpression<TC, TCO>::next_Prop::operator /*0I*/DefinedExpression () const
{
   __attribute__((unused)) TDefinedExpression TCTCO * self = CONTAINER_OF(this, TDefinedExpression TCTCO, next);
   DefinedExpression value(((C(DefinedExpression) *)self->impl)->next);
   return value;
}

// member accessors: DefinedExpression::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TDefinedExpression<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(DefinedExpression, name);
   ((C(DefinedExpression) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDefinedExpression<TC, TCO>::name_Prop & TDefinedExpression<TC, TCO>::name_Prop::operator= (typename TDefinedExpression<TC, TCO>::name_Prop & prop)
{
   SELF(DefinedExpression, name);
   /*0H*/const char * v = prop;
   ((C(DefinedExpression) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDefinedExpression<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TDefinedExpression TCTCO * self = CONTAINER_OF(this, TDefinedExpression TCTCO, name);
   return self ? ((C(DefinedExpression) *)self->impl)->name : null;
}

// member accessors: DefinedExpression::value
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TDefinedExpression<TC, TCO>::value_Prop::operator= (/*0H*/const char * v)
{
   SELF(DefinedExpression, value);
   ((C(DefinedExpression) *)self->impl)->value = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TDefinedExpression<TC, TCO>::value_Prop & TDefinedExpression<TC, TCO>::value_Prop::operator= (typename TDefinedExpression<TC, TCO>::value_Prop & prop)
{
   SELF(DefinedExpression, value);
   /*0H*/const char * v = prop;
   ((C(DefinedExpression) *)self->impl)->value = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TDefinedExpression<TC, TCO>::value_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TDefinedExpression TCTCO * self = CONTAINER_OF(this, TDefinedExpression TCTCO, value);
   return self ? ((C(DefinedExpression) *)self->impl)->value : null;
}



// member accessors: EnumClassData::values
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TEnumClassData<TC, TCO>::values_Prop::operator= (/*0H*/OldList v)
{
   SELF(EnumClassData, values);
   ((C(EnumClassData) *)self->impl)->values = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TEnumClassData<TC, TCO>::values_Prop & TEnumClassData<TC, TCO>::values_Prop::operator= (typename TEnumClassData<TC, TCO>::values_Prop & prop)
{
   SELF(EnumClassData, values);
   /*0H*/OldList v = prop;
   ((C(EnumClassData) *)self->impl)->values = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TEnumClassData<TC, TCO>::values_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TEnumClassData TCTCO * self = CONTAINER_OF(this, TEnumClassData TCTCO, values);
   OldList value(((C(EnumClassData) *)self->impl)->values);
   return value;
}

// member accessors: EnumClassData::largest
// (nohead::int64)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int64 TEnumClassData<TC, TCO>::largest_Prop::operator= (/*0H*/int64 v)
{
   SELF(EnumClassData, largest);
   ((C(EnumClassData) *)self->impl)->largest = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TEnumClassData<TC, TCO>::largest_Prop & TEnumClassData<TC, TCO>::largest_Prop::operator= (typename TEnumClassData<TC, TCO>::largest_Prop & prop)
{
   SELF(EnumClassData, largest);
   /*0H*/int64 v = prop;
   ((C(EnumClassData) *)self->impl)->largest = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TEnumClassData<TC, TCO>::largest_Prop::operator /*0I*/int64 () const
{
   __attribute__((unused)) TEnumClassData TCTCO * self = CONTAINER_OF(this, TEnumClassData TCTCO, largest);
   return self ? ((C(EnumClassData) *)self->impl)->largest : 0;
}



// member accessors: GlobalFunction::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/GlobalFunction TGlobalFunction<TC, TCO>::prev_Prop::operator= (/*0H*/GlobalFunction & v)
{
   SELF(GlobalFunction, prev);
   ((C(GlobalFunction) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TGlobalFunction<TC, TCO>::prev_Prop & TGlobalFunction<TC, TCO>::prev_Prop::operator= (typename TGlobalFunction<TC, TCO>::prev_Prop & prop)
{
   SELF(GlobalFunction, prev);
   /*0H*/GlobalFunction v = prop;
   ((C(GlobalFunction) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TGlobalFunction<TC, TCO>::prev_Prop::operator /*0I*/GlobalFunction () const
{
   __attribute__((unused)) TGlobalFunction TCTCO * self = CONTAINER_OF(this, TGlobalFunction TCTCO, prev);
   GlobalFunction value(((C(GlobalFunction) *)self->impl)->prev);
   return value;
}

// member accessors: GlobalFunction::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/GlobalFunction TGlobalFunction<TC, TCO>::next_Prop::operator= (/*0H*/GlobalFunction & v)
{
   SELF(GlobalFunction, next);
   ((C(GlobalFunction) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TGlobalFunction<TC, TCO>::next_Prop & TGlobalFunction<TC, TCO>::next_Prop::operator= (typename TGlobalFunction<TC, TCO>::next_Prop & prop)
{
   SELF(GlobalFunction, next);
   /*0H*/GlobalFunction v = prop;
   ((C(GlobalFunction) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TGlobalFunction<TC, TCO>::next_Prop::operator /*0I*/GlobalFunction () const
{
   __attribute__((unused)) TGlobalFunction TCTCO * self = CONTAINER_OF(this, TGlobalFunction TCTCO, next);
   GlobalFunction value(((C(GlobalFunction) *)self->impl)->next);
   return value;
}

// member accessors: GlobalFunction::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TGlobalFunction<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(GlobalFunction, name);
   ((C(GlobalFunction) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TGlobalFunction<TC, TCO>::name_Prop & TGlobalFunction<TC, TCO>::name_Prop::operator= (typename TGlobalFunction<TC, TCO>::name_Prop & prop)
{
   SELF(GlobalFunction, name);
   /*0H*/const char * v = prop;
   ((C(GlobalFunction) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TGlobalFunction<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TGlobalFunction TCTCO * self = CONTAINER_OF(this, TGlobalFunction TCTCO, name);
   return self ? ((C(GlobalFunction) *)self->impl)->name : null;
}

// member accessors: GlobalFunction::module
// (nohead::normal)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Module TGlobalFunction<TC, TCO>::module_Prop::operator= (/*0H*/Module v)
{
   SELF(GlobalFunction, module);
   ((C(GlobalFunction) *)self->impl)->module = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TGlobalFunction<TC, TCO>::module_Prop & TGlobalFunction<TC, TCO>::module_Prop::operator= (typename TGlobalFunction<TC, TCO>::module_Prop & prop)
{
   SELF(GlobalFunction, module);
   /*0H*/Module v = prop;
   ((C(GlobalFunction) *)self->impl)->module = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TGlobalFunction<TC, TCO>::module_Prop::operator /*0I*/Module () const
{
   __attribute__((unused)) TGlobalFunction TCTCO * self = CONTAINER_OF(this, TGlobalFunction TCTCO, module);
   Module value(((C(GlobalFunction) *)self->impl)->module);
   return value;
}

// member accessors: GlobalFunction::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TGlobalFunction<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(GlobalFunction, dataTypeString);
   ((C(GlobalFunction) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TGlobalFunction<TC, TCO>::dataTypeString_Prop & TGlobalFunction<TC, TCO>::dataTypeString_Prop::operator= (typename TGlobalFunction<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(GlobalFunction, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(GlobalFunction) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TGlobalFunction<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TGlobalFunction TCTCO * self = CONTAINER_OF(this, TGlobalFunction TCTCO, dataTypeString);
   return self ? ((C(GlobalFunction) *)self->impl)->dataTypeString : null;
}

// member accessors: GlobalFunction::symbol
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TGlobalFunction<TC, TCO>::symbol_Prop::operator= (/*0H*/void * v)
{
   SELF(GlobalFunction, symbol);
   ((C(GlobalFunction) *)self->impl)->symbol = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TGlobalFunction<TC, TCO>::symbol_Prop & TGlobalFunction<TC, TCO>::symbol_Prop::operator= (typename TGlobalFunction<TC, TCO>::symbol_Prop & prop)
{
   SELF(GlobalFunction, symbol);
   /*0H*/void * v = prop;
   ((C(GlobalFunction) *)self->impl)->symbol = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TGlobalFunction<TC, TCO>::symbol_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TGlobalFunction TCTCO * self = CONTAINER_OF(this, TGlobalFunction TCTCO, symbol);
   return self ? ((C(GlobalFunction) *)self->impl)->symbol : null;
}

template <typename TP_KT, typename TP_VT> TCPPClass<THashMap<TP_KT, TP_VT>> THashMap<TP_KT, TP_VT>::_cpp_class;

template <typename TP_KT, typename TP_VT> void THashMap<TP_KT, TP_VT>::class_registration(CPPClass & _cpp_class)
{
}


// property callers: HashMap::count   // get only
// (normal::int)

/*regGet*/template <typename TP_KT = int64 _ARG typename TP_VT = uintptr> inline THashMap<TP_KT _ARG TP_VT> ::count_Prop::operator /*0I*/int () const
{
   TSELF(HashMap, (<TP_KT _ARG TP_VT>), count);
   return HashMap_get_count(self ? self->impl : null);
}

// property callers: HashMap::initSize   // set only
// (normal::int)

/*nstSet*/template <typename TP_KT = int64 _ARG typename TP_VT = uintptr> inline /*0G*/int THashMap<TP_KT _ARG TP_VT> ::initSize_Prop::operator= (/*0G*/int v)
{
   TSELF(HashMap, (<TP_KT _ARG TP_VT>), initSize);
   HashMap_set_initSize(self->impl, v);
   return v;
}



// member accessors: HashMap::noRemResize
// (normal::enum|uint)

/*nstSet*/template <typename TP_KT = int64 _ARG typename TP_VT = uintptr> inline /*0H*/bool THashMap<TP_KT _ARG TP_VT> ::noRemResize_Prop::operator= (/*0H*/bool v)
{
   TSELF(HashMap, (<TP_KT _ARG TP_VT>), noRemResize);
   IPTR(self->impl, HashMap)->noRemResize = (C(bool))v;
   return v;
}

/*regSet*/template <typename TP_KT = int64 _ARG typename TP_VT = uintptr> inline typename THashMap<TP_KT _ARG TP_VT>::noRemResize_Prop & THashMap<TP_KT _ARG TP_VT>::noRemResize_Prop::operator= (THashMap<TP_KT _ARG TP_VT>::noRemResize_Prop & prop)
{
   TSELF(HashMap, (<TP_KT _ARG TP_VT>), noRemResize);
   /*0H*/bool v = prop;
   IPTR(self->impl, HashMap)->noRemResize = (C(bool))v;
   return prop;
}
/*regGet*/template <typename TP_KT = int64 _ARG typename TP_VT = uintptr> inline THashMap<TP_KT _ARG TP_VT> ::noRemResize_Prop::operator /*0I*/bool () const
{
   TSELF(HashMap, (<TP_KT _ARG TP_VT>), noRemResize);
   return self ? (bool)IPTR(self->impl, HashMap)->noRemResize : (bool)0;
}



// property callers: HashMapIterator::map
// (struct::normal)

/*nstSet*/template <typename TP_KT _ARG typename TP_VT> inline /*0A*/const THashMap<TP_KT, TP_VT> & THashMapIterator<TP_KT _ARG TP_VT> ::map_Prop::operator= (/*0A*/const THashMap<TP_KT, TP_VT> & v)
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), map);
   HashMapIterator_set_map(self ? &self->impl : null, ((Instance *)&v)->impl);
   return v;
}

/*regSet*/template <typename TP_KT _ARG typename TP_VT> inline typename THashMapIterator<TP_KT _ARG TP_VT>::map_Prop & THashMapIterator<TP_KT _ARG TP_VT>::map_Prop::operator= (THashMapIterator<TP_KT _ARG TP_VT>::map_Prop & prop)
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), map);
   /*0A*/const THashMap<TP_KT, TP_VT> & v = prop;
   HashMapIterator_set_map(self ? &self->impl : null, ((Instance *)&v)->impl);
   return prop;
}
/*nstSet*/template <typename TP_KT _ARG typename TP_VT> inline /*0C*/const THashMap<TP_KT, TP_VT> * THashMapIterator<TP_KT _ARG TP_VT> ::map_Prop::operator= (/*0C*/const THashMap<TP_KT, TP_VT> * v)
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), map);
   HashMapIterator_set_map(self ? &self->impl : null, v ? ((Instance *)v)->impl : null);
   return v;
}

/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::map_Prop::operator /*0B*/TIH<THashMap<TP_KT, TP_VT>> () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), map);
   C(Instance) i = HashMapIterator_get_map(self ? &self->impl : null);
   TIH<THashMap<TP_KT, TP_VT>> cppi(i);
   return *cppi;
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline TIH<THashMap<TP_KT, TP_VT>> THashMapIterator<TP_KT _ARG TP_VT> ::map_Prop::operator /*0D*/-> () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), map);
   C(Instance) i = HashMapIterator_get_map(self ? &self->impl : null);
   TIH<THashMap<TP_KT, TP_VT>> holder(i);
   return holder;
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::map_Prop::operator /*0E*/THashMap<TP_KT, TP_VT> () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), map);
   C(Instance) i = HashMapIterator_get_map(self ? &self->impl : null);
   return THashMap<TP_KT, TP_VT>(i);
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::map_Prop::operator /*0F*/THashMap<TP_KT, TP_VT>* () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), map);
   C(Instance) i = HashMapIterator_get_map(self ? &self->impl : null);
   return BINDINGS_CLASS(i) ? (THashMap<TP_KT, TP_VT> *)INSTANCEL(i, i->_class) : (THashMap<TP_KT, TP_VT> *)0;
}

// property callers: HashMapIterator::key   // get only
// (struct::const KT)

/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::key_Prop::operator /*0B*/TIH<TP_KT> () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), key);
   return *TIH<TP_KT>(fromTA<C(Instance)>(HashMapIterator_get_key(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline TIH<TP_KT> THashMapIterator<TP_KT _ARG TP_VT> ::key_Prop::operator /*0D*/-> () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), key);
   return TIH<TP_KT>(fromTA<C(Instance)>(HashMapIterator_get_key(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::key_Prop::operator /*0E*/TP_KT () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), key);
   return fromTA<TP_KT>(HashMapIterator_get_key(self ? &self->impl : null));
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::key_Prop::operator /*0F*/TP_KT* () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), key);
   return fromTA<TP_KT *>(HashMapIterator_get_key(self ? &self->impl : null));
}

// property callers: HashMapIterator::value
// (struct::VT)

/*nstSet*/template <typename TP_KT _ARG typename TP_VT> inline /*0A*/const TP_VT & THashMapIterator<TP_KT _ARG TP_VT> ::value_Prop::operator= (/*0A*/const TP_VT & v)
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), value);
   HashMapIterator_set_value(self ? &self->impl : null, toTA(v));
   return v;
}

/*regSet*/template <typename TP_KT _ARG typename TP_VT> inline typename THashMapIterator<TP_KT _ARG TP_VT>::value_Prop & THashMapIterator<TP_KT _ARG TP_VT>::value_Prop::operator= (THashMapIterator<TP_KT _ARG TP_VT>::value_Prop & prop)
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), value);
   /*0A*/const TP_VT & v = prop;
   HashMapIterator_set_value(self ? &self->impl : null, toTA(v));
   return prop;
}
/*nstSet*/template <typename TP_KT _ARG typename TP_VT> inline /*0C*/const TP_VT * THashMapIterator<TP_KT _ARG TP_VT> ::value_Prop::operator= (/*0C*/const TP_VT * v)
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), value);
   HashMapIterator_set_value(self ? &self->impl : null, toTA(v));
   return v;
}

/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::value_Prop::operator /*0B*/TIH<TP_VT> () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), value);
   return *TIH<TP_VT>(fromTA<C(Instance)>(HashMapIterator_get_value(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline TIH<TP_VT> THashMapIterator<TP_KT _ARG TP_VT> ::value_Prop::operator /*0D*/-> () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), value);
   return TIH<TP_VT>(fromTA<C(Instance)>(HashMapIterator_get_value(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::value_Prop::operator /*0E*/TP_VT () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), value);
   return fromTA<TP_VT>(HashMapIterator_get_value(self ? &self->impl : null));
}
/*regGet*/template <typename TP_KT _ARG typename TP_VT> inline THashMapIterator<TP_KT _ARG TP_VT> ::value_Prop::operator /*0F*/TP_VT* () const
{
   TSELF(HashMapIterator, (<TP_KT _ARG TP_VT>), value);
   return fromTA<TP_VT *>(HashMapIterator_get_value(self ? &self->impl : null));
}

template <typename TP_KT> TCPPClass<THashTable<TP_KT>> THashTable<TP_KT>::_cpp_class;

template <typename TP_KT> void THashTable<TP_KT>::class_registration(CPPClass & _cpp_class)
{
}


// property callers: HashTable::initSize   // set only
// (normal::int)

/*nstSet*/template <typename TP_KT = int64> inline /*0G*/int THashTable<TP_KT> ::initSize_Prop::operator= (/*0G*/int v)
{
   TSELF(HashTable, (<TP_KT>), initSize);
   HashTable_set_initSize(self->impl, v);
   return v;
}


inline IOChannel::IOChannel_readData_Functor::FunctionType IOChannel::IOChannel_readData_Functor::operator= (FunctionType func)
{
   SELF(IOChannel, readData);
   if(self->vTbl == IOChannel::_cpp_class.vTbl)
   {
      uint size = IOChannel::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(IOChannel::IOChannel_readData_Functor::FunctionType, size);
      memcpy(self->vTbl, IOChannel::_cpp_class.vTbl, sizeof(IOChannel::IOChannel_readData_Functor::FunctionType) * size);
   }
   ((IOChannel::IOChannel_readData_Functor::FunctionType *)self->vTbl)[M_VTBLID(IOChannel, readData)] = func;
   return func;
}
inline uintsize IOChannel::IOChannel_readData_Functor::operator()( /*6Fj*/void * data, /*6Fj*/uintsize numBytes)
{
   SELF(IOChannel, readData);
   return IOChannel_readData(self ? self->impl : (C(IOChannel))null, /*7Al*/data, /*7Al*/numBytes);
}
// inline void IOChannel::register_readData(CPPClass & cl, IOChannel::IOChannel_readData_Functor::FunctionType func)
// {
//    ((IOChannel::IOChannel_readData_Functor::FunctionType *)cl.vTbl)[M_VTBLID(IOChannel, readData)] = func;
// }

inline IOChannel::IOChannel_writeData_Functor::FunctionType IOChannel::IOChannel_writeData_Functor::operator= (FunctionType func)
{
   SELF(IOChannel, writeData);
   if(self->vTbl == IOChannel::_cpp_class.vTbl)
   {
      uint size = IOChannel::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(IOChannel::IOChannel_writeData_Functor::FunctionType, size);
      memcpy(self->vTbl, IOChannel::_cpp_class.vTbl, sizeof(IOChannel::IOChannel_writeData_Functor::FunctionType) * size);
   }
   ((IOChannel::IOChannel_writeData_Functor::FunctionType *)self->vTbl)[M_VTBLID(IOChannel, writeData)] = func;
   return func;
}
inline uintsize IOChannel::IOChannel_writeData_Functor::operator()( /*6Fj*/const void * data, /*6Fj*/uintsize numBytes)
{
   SELF(IOChannel, writeData);
   return IOChannel_writeData(self ? self->impl : (C(IOChannel))null, /*7Al*/data, /*7Al*/numBytes);
}
// inline void IOChannel::register_writeData(CPPClass & cl, IOChannel::IOChannel_writeData_Functor::FunctionType func)
// {
//    ((IOChannel::IOChannel_writeData_Functor::FunctionType *)cl.vTbl)[M_VTBLID(IOChannel, writeData)] = func;
// }


inline void IOChannel::get(/*1Bb*/typed_object_class_ptr class_data, void * data)
{
   IOChannel_get(impl, /*5Fe*/class_data, data);
}
inline void IOChannel::put(/*1Bb*/typed_object_class_ptr class_data, void * data)
{
   IOChannel_put(impl, /*5Fe*/class_data, data);
}
inline void IOChannel::serialize(/*1Bb*/typed_object_class_ptr class_data, void * data)
{
   IOChannel_serialize(impl, /*5Fe*/class_data, data);
}
inline void IOChannel::unserialize(/*1Bb*/typed_object_class_ptr class_data, void * data)
{
   IOChannel_unserialize(impl, /*5Fe*/class_data, data);
}
inline Instance::Instance_onCompare_Functor::FunctionType Instance::Instance_onCompare_Functor::operator= (FunctionType func)
{
   SELF(Instance, onCompare);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onCompare_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onCompare_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onCompare_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onCompare)] = func;
   return func;
}
inline int Instance::Instance_onCompare_Functor::operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & object)
{
   SELF(Instance, onCompare);
   return Instance_onCompare(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/object.impl);
}
// inline void Instance::register_onCompare(CPPClass & cl, Instance::Instance_onCompare_Functor::FunctionType func)
// {
//    ((Instance::Instance_onCompare_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onCompare)] = func;
// }

inline Instance::Instance_onCopy_Functor::FunctionType Instance::Instance_onCopy_Functor::operator= (FunctionType func)
{
   SELF(Instance, onCopy);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onCopy_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onCopy_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onCopy_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onCopy)] = func;
   return func;
}
inline void Instance::Instance_onCopy_Functor::operator()(/*6Bk*/Instance & o_ , /*6Bj*/Instance & newData)
{
   SELF(Instance, onCopy);
   Instance_onCopy(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/newData.impl);
}
// inline void Instance::register_onCopy(CPPClass & cl, Instance::Instance_onCopy_Functor::FunctionType func)
// {
//    ((Instance::Instance_onCopy_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onCopy)] = func;
// }

inline Instance::Instance_onDisplay_Functor::FunctionType Instance::Instance_onDisplay_Functor::operator= (FunctionType func)
{
   SELF(Instance, onDisplay);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onDisplay_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onDisplay_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onDisplay_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onDisplay)] = func;
   return func;
}
inline void Instance::Instance_onDisplay_Functor::operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Surface*/ & surface, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int width, /*6Fj*/void * fieldData, /*6Fj*/Alignment alignment, /*6Fj*/DataDisplayFlags displayFlags)
{
   SELF(Instance, onDisplay);
   Instance_onDisplay(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/surface.impl, /*7Al*/x, /*7Al*/y, /*7Al*/width, /*7Al*/fieldData, /*7Al*/(C(Alignment))alignment, /*7Al*/displayFlags);
}
// inline void Instance::register_onDisplay(CPPClass & cl, Instance::Instance_onDisplay_Functor::FunctionType func)
// {
//    ((Instance::Instance_onDisplay_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onDisplay)] = func;
// }

inline Instance::Instance_onEdit_Functor::FunctionType Instance::Instance_onEdit_Functor::operator= (FunctionType func)
{
   SELF(Instance, onEdit);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onEdit_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onEdit_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onEdit_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onEdit)] = func;
   return func;
}
inline Instance & Instance::Instance_onEdit_Functor::operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*DataBox*/ & dataBox, /*6Aj*/Instance /*DataBox*/ & obsolete, /*6Fj*/int x, /*6Fj*/int y, /*6Fj*/int w, /*6Fj*/int h, /*6Fj*/void * userData)
{
   SELF(Instance, onEdit);
   C(Instance) retInstance = Instance_onEdit(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/dataBox.impl, /*7Al*/obsolete.impl, /*7Al*/x, /*7Al*/y, /*7Al*/w, /*7Al*/h, /*7Al*/userData);
      return *(Instance *)INSTANCEL(retInstance, retInstance->_class);
}
// inline void Instance::register_onEdit(CPPClass & cl, Instance::Instance_onEdit_Functor::FunctionType func)
// {
//    ((Instance::Instance_onEdit_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onEdit)] = func;
// }

inline Instance::Instance_onFree_Functor::FunctionType Instance::Instance_onFree_Functor::operator= (FunctionType func)
{
   SELF(Instance, onFree);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onFree_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onFree_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onFree_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onFree)] = func;
   return func;
}
inline void Instance::Instance_onFree_Functor::operator()(/*6Bk*/Instance & o_ )
{
   SELF(Instance, onFree);
   Instance_onFree(_cpp_class.impl, self ? self->impl : (C(Instance))null);
}
// inline void Instance::register_onFree(CPPClass & cl, Instance::Instance_onFree_Functor::FunctionType func)
// {
//    ((Instance::Instance_onFree_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onFree)] = func;
// }

inline Instance::Instance_onGetDataFromString_Functor::FunctionType Instance::Instance_onGetDataFromString_Functor::operator= (FunctionType func)
{
   SELF(Instance, onGetDataFromString);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onGetDataFromString_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onGetDataFromString_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onGetDataFromString_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onGetDataFromString)] = func;
   return func;
}
inline bool Instance::Instance_onGetDataFromString_Functor::operator()(/*6Bk*/Instance & o_ , /*6Fj*/const char * string)
{
   SELF(Instance, onGetDataFromString);
   return (bool)Instance_onGetDataFromString(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/string);
}
// inline void Instance::register_onGetDataFromString(CPPClass & cl, Instance::Instance_onGetDataFromString_Functor::FunctionType func)
// {
//    ((Instance::Instance_onGetDataFromString_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onGetDataFromString)] = func;
// }

inline Instance::Instance_onGetString_Functor::FunctionType Instance::Instance_onGetString_Functor::operator= (FunctionType func)
{
   SELF(Instance, onGetString);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onGetString_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onGetString_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onGetString_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onGetString)] = func;
   return func;
}
inline const char * Instance::Instance_onGetString_Functor::operator()(/*6Bk*/Instance & o_ , /*6Fj*/char * tempString, /*6Fj*/void * reserved, /*6Fj*/ObjectNotationType * onType)
{
   SELF(Instance, onGetString);
   return Instance_onGetString(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/tempString, /*7Al*/reserved, /*7Al*/(C(ObjectNotationType) *)onType);
}
// inline void Instance::register_onGetString(CPPClass & cl, Instance::Instance_onGetString_Functor::FunctionType func)
// {
//    ((Instance::Instance_onGetString_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onGetString)] = func;
// }

inline Instance::Instance_onSaveEdit_Functor::FunctionType Instance::Instance_onSaveEdit_Functor::operator= (FunctionType func)
{
   SELF(Instance, onSaveEdit);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onSaveEdit_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onSaveEdit_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onSaveEdit_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onSaveEdit)] = func;
   return func;
}
inline bool Instance::Instance_onSaveEdit_Functor::operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*Window*/ & window, /*6Fj*/void * object)
{
   SELF(Instance, onSaveEdit);
   return (bool)Instance_onSaveEdit(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/window.impl, /*7Al*/object);
}
// inline void Instance::register_onSaveEdit(CPPClass & cl, Instance::Instance_onSaveEdit_Functor::FunctionType func)
// {
//    ((Instance::Instance_onSaveEdit_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onSaveEdit)] = func;
// }

inline Instance::Instance_onSerialize_Functor::FunctionType Instance::Instance_onSerialize_Functor::operator= (FunctionType func)
{
   SELF(Instance, onSerialize);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onSerialize_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onSerialize_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onSerialize_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onSerialize)] = func;
   return func;
}
inline void Instance::Instance_onSerialize_Functor::operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel)
{
   SELF(Instance, onSerialize);
   Instance_onSerialize(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/channel.impl);
}
// inline void Instance::register_onSerialize(CPPClass & cl, Instance::Instance_onSerialize_Functor::FunctionType func)
// {
//    ((Instance::Instance_onSerialize_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onSerialize)] = func;
// }

inline Instance::Instance_onUnserialize_Functor::FunctionType Instance::Instance_onUnserialize_Functor::operator= (FunctionType func)
{
   SELF(Instance, onUnserialize);
   if(self->vTbl == Instance::_cpp_class.vTbl)
   {
      uint size = Instance::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Instance::Instance_onUnserialize_Functor::FunctionType, size);
      memcpy(self->vTbl, Instance::_cpp_class.vTbl, sizeof(Instance::Instance_onUnserialize_Functor::FunctionType) * size);
   }
   ((Instance::Instance_onUnserialize_Functor::FunctionType *)self->vTbl)[M_VTBLID(Instance, onUnserialize)] = func;
   return func;
}
inline void Instance::Instance_onUnserialize_Functor::operator()(/*6Bk*/Instance & o_ , /*6Aj*/Instance /*IOChannel*/ & channel)
{
   SELF(Instance, onUnserialize);
   Instance_onUnserialize(_cpp_class.impl, self ? self->impl : (C(Instance))null, /*7Al*/channel.impl);
}
// inline void Instance::register_onUnserialize(CPPClass & cl, Instance::Instance_onUnserialize_Functor::FunctionType func)
// {
//    ((Instance::Instance_onUnserialize_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Instance, onUnserialize)] = func;
// }



template <typename TP_T, typename TP_IT> inline void TIterator<TP_T, TP_IT>::free()
{
   Iterator_free(&impl);
}
template <typename TP_T, typename TP_IT> inline TP_T TIterator<TP_T, TP_IT>::getData()
{
   return fromTA<TP_T>(Iterator_getData(&impl));
}
template <typename TP_T, typename TP_IT> inline C(bool) TIterator<TP_T, TP_IT>::next()
{
   return Iterator_next(&impl);
}
template <typename TP_T, typename TP_IT> inline C(bool) TIterator<TP_T, TP_IT>::prev()
{
   return Iterator_prev(&impl);
}
template <typename TP_T, typename TP_IT> inline void TIterator<TP_T, TP_IT>::remove()
{
   Iterator_remove(&impl);
}

template <typename TP_T, typename TP_IT> inline C(bool) TIterator<TP_T, TP_IT>::index(TP_IT index, C(bool) create)
{
   return Iterator_index(&impl, toTA(index), create);
}



// property callers: Iterator::data
// (struct::T)

/*nstSet*/template <typename TP_T _ARG typename TP_IT = int> inline /*0A*/const TP_T & TIterator<TP_T _ARG TP_IT> ::data_Prop::operator= (/*0A*/const TP_T & v)
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), data);
   Iterator_set_data(self ? &self->impl : null, toTA(v));
   return v;
}

/*regSet*/template <typename TP_T _ARG typename TP_IT = int> inline typename TIterator<TP_T _ARG TP_IT>::data_Prop & TIterator<TP_T _ARG TP_IT>::data_Prop::operator= (TIterator<TP_T _ARG TP_IT>::data_Prop & prop)
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), data);
   /*0A*/const TP_T & v = prop;
   Iterator_set_data(self ? &self->impl : null, toTA(v));
   return prop;
}
/*nstSet*/template <typename TP_T _ARG typename TP_IT = int> inline /*0C*/const TP_T * TIterator<TP_T _ARG TP_IT> ::data_Prop::operator= (/*0C*/const TP_T * v)
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), data);
   Iterator_set_data(self ? &self->impl : null, toTA(v));
   return v;
}

/*regGet*/template <typename TP_T _ARG typename TP_IT = int> inline TIterator<TP_T _ARG TP_IT> ::data_Prop::operator /*0B*/TIH<TP_T> () const
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), data);
   return *TIH<TP_T>(fromTA<C(Instance)>(Iterator_get_data(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_T _ARG typename TP_IT = int> inline TIH<TP_T> TIterator<TP_T _ARG TP_IT> ::data_Prop::operator /*0D*/-> () const
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), data);
   return TIH<TP_T>(fromTA<C(Instance)>(Iterator_get_data(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_T _ARG typename TP_IT = int> inline TIterator<TP_T _ARG TP_IT> ::data_Prop::operator /*0E*/TP_T () const
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), data);
   return fromTA<TP_T>(Iterator_get_data(self ? &self->impl : null));
}
/*regGet*/template <typename TP_T _ARG typename TP_IT = int> inline TIterator<TP_T _ARG TP_IT> ::data_Prop::operator /*0F*/TP_T* () const
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), data);
   return fromTA<TP_T *>(Iterator_get_data(self ? &self->impl : null));
}


// member accessors: Iterator::container
// (struct::normal)

/*nstSet*/template <typename TP_T _ARG typename TP_IT = int> inline /*0H*/TContainer<TP_T, TP_IT> TIterator<TP_T _ARG TP_IT> ::container_Prop::operator= (/*0H*/TContainer<TP_T, TP_IT> v)
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), container);
   self->impl.container = ((Instance *)&v)->impl;
   return v;
}

/*regSet*/template <typename TP_T _ARG typename TP_IT = int> inline typename TIterator<TP_T _ARG TP_IT>::container_Prop & TIterator<TP_T _ARG TP_IT>::container_Prop::operator= (TIterator<TP_T _ARG TP_IT>::container_Prop & prop)
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), container);
   /*0H*/TContainer<TP_T, TP_IT> v = prop;
   self->impl.container = ((Instance *)&v)->impl;
   return prop;
}
/*regGet*/template <typename TP_T _ARG typename TP_IT = int> inline TIterator<TP_T _ARG TP_IT> ::container_Prop::operator /*0I*/TContainer<TP_T, TP_IT> () const
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), container);
   TContainer<TP_T, TP_IT> value(((C(Iterator) *)&self->impl)->container);
   return value;
}

// member accessors: Iterator::pointer
// (struct::nohead)

/*nstSet*/template <typename TP_T _ARG typename TP_IT = int> inline /*0H*/IteratorPointer TIterator<TP_T _ARG TP_IT> ::pointer_Prop::operator= (/*0H*/IteratorPointer & v)
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), pointer);
   self->impl.pointer = v.impl;
   return v;
}

/*regSet*/template <typename TP_T _ARG typename TP_IT = int> inline typename TIterator<TP_T _ARG TP_IT>::pointer_Prop & TIterator<TP_T _ARG TP_IT>::pointer_Prop::operator= (TIterator<TP_T _ARG TP_IT>::pointer_Prop & prop)
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), pointer);
   /*0H*/IteratorPointer v = prop;
   self->impl.pointer = v.impl;
   return prop;
}
/*regGet*/template <typename TP_T _ARG typename TP_IT = int> inline TIterator<TP_T _ARG TP_IT> ::pointer_Prop::operator /*0I*/IteratorPointer () const
{
   TSELF(Iterator, (<TP_T _ARG TP_IT>), pointer);
   IteratorPointer value(((C(Iterator) *)&self->impl)->pointer);
   return value;
}




// member accessors: Link::data
// (nohead::uint64)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/uint64 TLink<TC, TCO>::data_Prop::operator= (/*0H*/uint64 v)
{
   SELF(Link, data);
   ((C(Link) *)self->impl)->data = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TLink<TC, TCO>::data_Prop & TLink<TC, TCO>::data_Prop::operator= (typename TLink<TC, TCO>::data_Prop & prop)
{
   SELF(Link, data);
   /*0H*/uint64 v = prop;
   ((C(Link) *)self->impl)->data = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TLink<TC, TCO>::data_Prop::operator /*0I*/uint64 () const
{
   __attribute__((unused)) TLink TCTCO * self = CONTAINER_OF(this, TLink TCTCO, data);
   return self ? ((C(Link) *)self->impl)->data : 0;
}



// member accessors: LinkElement::prev
// (struct::T)

/*nstSet*/template <typename TP_T> inline /*0H*/TP_T TLinkElement<TP_T> ::prev_Prop::operator= (/*0H*/TP_T v)
{
   TSELF(LinkElement, (<TP_T>), prev);
   self->impl.prev = v;
   return v;
}

/*regSet*/template <typename TP_T> inline typename TLinkElement<TP_T>::prev_Prop & TLinkElement<TP_T>::prev_Prop::operator= (TLinkElement<TP_T>::prev_Prop & prop)
{
   TSELF(LinkElement, (<TP_T>), prev);
   /*0H*/TP_T v = prop;
   self->impl.prev = v;
   return prop;
}
/*regGet*/template <typename TP_T> inline TLinkElement<TP_T> ::prev_Prop::operator /*0I*/TP_T () const
{
   TSELF(LinkElement, (<TP_T>), prev);
   return self ? ((C(LinkElement) *)&self->impl)->prev : 0;
}

// member accessors: LinkElement::next
// (struct::T)

/*nstSet*/template <typename TP_T> inline /*0H*/TP_T TLinkElement<TP_T> ::next_Prop::operator= (/*0H*/TP_T v)
{
   TSELF(LinkElement, (<TP_T>), next);
   self->impl.next = v;
   return v;
}

/*regSet*/template <typename TP_T> inline typename TLinkElement<TP_T>::next_Prop & TLinkElement<TP_T>::next_Prop::operator= (TLinkElement<TP_T>::next_Prop & prop)
{
   TSELF(LinkElement, (<TP_T>), next);
   /*0H*/TP_T v = prop;
   self->impl.next = v;
   return prop;
}
/*regGet*/template <typename TP_T> inline TLinkElement<TP_T> ::next_Prop::operator /*0I*/TP_T () const
{
   TSELF(LinkElement, (<TP_T>), next);
   return self ? ((C(LinkElement) *)&self->impl)->next : 0;
}

template <typename TP_LT> TCPPClass<TLinkList<TP_LT>> TLinkList<TP_LT>::_cpp_class;

template <typename TP_LT> void TLinkList<TP_LT>::class_registration(CPPClass & _cpp_class)
{
}


// member accessors: LinkList::first
// (normal::LT)

/*nstSet*/template <typename TP_LT = ListItem> inline /*0H*/TP_LT TLinkList<TP_LT> ::first_Prop::operator= (/*0H*/TP_LT v)
{
   TSELF(LinkList, (<TP_LT>), first);
   IPTR(self->impl, LinkList)->first = v;
   return v;
}

/*regSet*/template <typename TP_LT = ListItem> inline typename TLinkList<TP_LT>::first_Prop & TLinkList<TP_LT>::first_Prop::operator= (TLinkList<TP_LT>::first_Prop & prop)
{
   TSELF(LinkList, (<TP_LT>), first);
   /*0H*/TP_LT v = prop;
   IPTR(self->impl, LinkList)->first = v;
   return prop;
}
/*regGet*/template <typename TP_LT = ListItem> inline TLinkList<TP_LT> ::first_Prop::operator /*0I*/TP_LT () const
{
   TSELF(LinkList, (<TP_LT>), first);
   return self ? IPTR(self->impl, LinkList)->first : 0;
}

// member accessors: LinkList::last
// (normal::LT)

/*nstSet*/template <typename TP_LT = ListItem> inline /*0H*/TP_LT TLinkList<TP_LT> ::last_Prop::operator= (/*0H*/TP_LT v)
{
   TSELF(LinkList, (<TP_LT>), last);
   IPTR(self->impl, LinkList)->last = v;
   return v;
}

/*regSet*/template <typename TP_LT = ListItem> inline typename TLinkList<TP_LT>::last_Prop & TLinkList<TP_LT>::last_Prop::operator= (TLinkList<TP_LT>::last_Prop & prop)
{
   TSELF(LinkList, (<TP_LT>), last);
   /*0H*/TP_LT v = prop;
   IPTR(self->impl, LinkList)->last = v;
   return prop;
}
/*regGet*/template <typename TP_LT = ListItem> inline TLinkList<TP_LT> ::last_Prop::operator /*0I*/TP_LT () const
{
   TSELF(LinkList, (<TP_LT>), last);
   return self ? IPTR(self->impl, LinkList)->last : 0;
}

// member accessors: LinkList::count
// (normal::int)

/*nstSet*/template <typename TP_LT = ListItem> inline /*0H*/int TLinkList<TP_LT> ::count_Prop::operator= (/*0H*/int v)
{
   TSELF(LinkList, (<TP_LT>), count);
   IPTR(self->impl, LinkList)->count = v;
   return v;
}

/*regSet*/template <typename TP_LT = ListItem> inline typename TLinkList<TP_LT>::count_Prop & TLinkList<TP_LT>::count_Prop::operator= (TLinkList<TP_LT>::count_Prop & prop)
{
   TSELF(LinkList, (<TP_LT>), count);
   /*0H*/int v = prop;
   IPTR(self->impl, LinkList)->count = v;
   return prop;
}
/*regGet*/template <typename TP_LT = ListItem> inline TLinkList<TP_LT> ::count_Prop::operator /*0I*/int () const
{
   TSELF(LinkList, (<TP_LT>), count);
   return self ? IPTR(self->impl, LinkList)->count : 0;
}

template <typename TP_LLT> TCPPClass<TList<TP_LLT>> TList<TP_LLT>::_cpp_class;

template <typename TP_LLT> void TList<TP_LLT>::class_registration(CPPClass & _cpp_class)
{
}



// member accessors: ListItem::link
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0Z*/const LinkElement & TListItem<TC, TCO>::link_Prop::operator= (/*0Z*/const LinkElement & v)
{
   SELF(ListItem, link);
   ((C(ListItem) *)self->impl)->link = ((TLinkElement<ListItem> *)&v)->impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TListItem<TC, TCO>::link_Prop & TListItem<TC, TCO>::link_Prop::operator= (typename TListItem<TC, TCO>::link_Prop & prop)
{
   SELF(ListItem, link);
   /*0H*/TLinkElement<ListItem> & v = prop;
   ((C(ListItem) *)self->impl)->link = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TListItem<TC, TCO>::link_Prop::operator /*0I*/LinkElement & () const
{
   __attribute__((unused)) TListItem TCTCO * self = CONTAINER_OF(this, TListItem TCTCO, link);
   LinkElement value(((C(ListItem) *)self->impl)->link);
   return value;
}

// member accessors: ListItem::prev
// (nohead::thisclass)

// REVIEW: These C(Class) do not look right here -- is this the base object?
/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/C(Class) * TListItem<TC, TCO>::prev_Prop::operator= (/*0H*/C(Class) * v)
{
   SELF(ListItem, prev);
   ((C(ListItem) *)self->impl)->prev = (C(ListItem) *)v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TListItem<TC, TCO>::prev_Prop & TListItem<TC, TCO>::prev_Prop::operator= (typename TListItem<TC, TCO>::prev_Prop & prop)
{
   SELF(ListItem, prev);
   /*0H*/C(Class) * v = prop;
   ((C(ListItem) *)self->impl)->prev = (C(ListItem) *)v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TListItem<TC, TCO>::prev_Prop::operator /*0I*/C(Class) * () const
{
   __attribute__((unused)) TListItem TCTCO * self = CONTAINER_OF(this, TListItem TCTCO, prev);
   return self ? ((C(ListItem) *)self->impl)->prev : 0;
}

// member accessors: ListItem::next
// (nohead::thisclass)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/C(Class) * TListItem<TC, TCO>::next_Prop::operator= (/*0H*/C(Class) * v)
{
   SELF(ListItem, next);
   ((C(ListItem) *)self->impl)->next = (C(ListItem) *)v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TListItem<TC, TCO>::next_Prop & TListItem<TC, TCO>::next_Prop::operator= (typename TListItem<TC, TCO>::next_Prop & prop)
{
   SELF(ListItem, next);
   /*0H*/C(Class) * v = prop;
   ((C(ListItem) *)self->impl)->next = (C(ListItem) *)v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TListItem<TC, TCO>::next_Prop::operator /*0I*/C(Class) * () const
{
   __attribute__((unused)) TListItem TCTCO * self = CONTAINER_OF(this, TListItem TCTCO, next);
   return self ? ((C(ListItem) *)self->impl)->next : 0;
}

template <typename TP_MT, typename TP_V> TCPPClass<TMap<TP_MT, TP_V>> TMap<TP_MT, TP_V>::_cpp_class;

template <typename TP_MT, typename TP_V> void TMap<TP_MT, TP_V>::class_registration(CPPClass & _cpp_class)
{
}


// property callers: Map::mapSrc   // set only
// (normal::normal)

/*nstSet*/template <typename TP_MT _ARG typename TP_V> inline /*0A*/const Map & TMap<TP_MT _ARG TP_V> ::mapSrc_Prop::operator= (/*0A*/const Map & v)
{
   TSELF(Map, (<TP_MT _ARG TP_V>), mapSrc);
   Map_set_mapSrc(self ? self->impl : null, ((Instance *)&v)->impl);
   return v;
}

/*nstSet*/template <typename TP_MT _ARG typename TP_V> inline /*0C*/const Map * TMap<TP_MT _ARG TP_V> ::mapSrc_Prop::operator= (/*0C*/const Map * v)
{
   TSELF(Map, (<TP_MT _ARG TP_V>), mapSrc);
   Map_set_mapSrc(self ? self->impl : null, v ? ((Instance *)v)->impl : null);
   return v;
}


template <typename TP_KT, typename TP_KV> inline C(bool) TMapIterator<TP_KT, TP_KV>::index(TP_KT index, C(bool) create)
{
   return Iterator_index((C(Iterator) *)&impl, toTA(index), create);
}

// property callers: MapIterator::map
// (struct::normal)

/*nstSet*/template <typename TP_KT _ARG typename TP_V> inline /*0A*/const TMap<TP_KT, TP_V> & TMapIterator<TP_KT _ARG TP_V> ::map_Prop::operator= (/*0A*/const TMap<TP_KT, TP_V> & v)
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), map);
   MapIterator_set_map(self ? &self->impl : null, ((Instance *)&v)->impl);
   return v;
}

/*regSet*/template <typename TP_KT _ARG typename TP_V> inline typename TMapIterator<TP_KT _ARG TP_V>::map_Prop & TMapIterator<TP_KT _ARG TP_V>::map_Prop::operator= (TMapIterator<TP_KT _ARG TP_V>::map_Prop & prop)
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), map);
   /*0A*/const TMap<TP_KT, TP_V> & v = prop;
   MapIterator_set_map(self ? &self->impl : null, ((Instance *)&v)->impl);
   return prop;
}
/*nstSet*/template <typename TP_KT _ARG typename TP_V> inline /*0C*/const TMap<TP_KT, TP_V> * TMapIterator<TP_KT _ARG TP_V> ::map_Prop::operator= (/*0C*/const TMap<TP_KT, TP_V> * v)
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), map);
   MapIterator_set_map(self ? &self->impl : null, v ? ((Instance *)v)->impl : null);
   return v;
}

/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::map_Prop::operator /*0B*/TIH<TMap<TP_KT, TP_V>> () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), map);
   C(Instance) i = MapIterator_get_map(self ? &self->impl : null);
   TIH<TMap<TP_KT, TP_V>> cppi(i);
   return *cppi;
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TIH<TMap<TP_KT, TP_V>> TMapIterator<TP_KT _ARG TP_V> ::map_Prop::operator /*0D*/-> () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), map);
   C(Instance) i = MapIterator_get_map(self ? &self->impl : null);
   TIH<TMap<TP_KT, TP_V>> holder(i);
   return holder;
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::map_Prop::operator /*0E*/TMap<TP_KT, TP_V> () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), map);
   C(Instance) i = MapIterator_get_map(self ? &self->impl : null);
   return TMap<TP_KT, TP_V>(i);
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::map_Prop::operator /*0F*/TMap<TP_KT, TP_V>* () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), map);
   C(Instance) i = MapIterator_get_map(self ? &self->impl : null);
   return BINDINGS_CLASS(i) ? (TMap<TP_KT, TP_V> *)INSTANCEL(i, i->_class) : (TMap<TP_KT, TP_V> *)0;
}

// property callers: MapIterator::key   // get only
// (struct::const KT)

/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::key_Prop::operator /*0B*/TIH<TP_KT> () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), key);
   return *TIH<TP_KT>(fromTA<C(Instance)>(MapIterator_get_key(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TIH<TP_KT> TMapIterator<TP_KT _ARG TP_V> ::key_Prop::operator /*0D*/-> () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), key);
   return TIH<TP_KT>(fromTA<C(Instance)>(MapIterator_get_key(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::key_Prop::operator /*0E*/TP_KT () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), key);
   return fromTA<TP_KT>(MapIterator_get_key(self ? &self->impl : null));
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::key_Prop::operator /*0F*/TP_KT* () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), key);
   return fromTA<TP_KT *>(MapIterator_get_key(self ? &self->impl : null));
}

// property callers: MapIterator::value
// (struct::V)

/*nstSet*/template <typename TP_KT _ARG typename TP_V> inline /*0A*/const TP_V & TMapIterator<TP_KT _ARG TP_V> ::value_Prop::operator= (/*0A*/const TP_V & v)
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), value);
   MapIterator_set_value(self ? &self->impl : null, toTA(v));
   return v;
}

/*regSet*/template <typename TP_KT _ARG typename TP_V> inline typename TMapIterator<TP_KT _ARG TP_V>::value_Prop & TMapIterator<TP_KT _ARG TP_V>::value_Prop::operator= (TMapIterator<TP_KT _ARG TP_V>::value_Prop & prop)
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), value);
   /*0A*/const TP_V & v = prop;
   MapIterator_set_value(self ? &self->impl : null, toTA(v));
   return prop;
}
/*nstSet*/template <typename TP_KT _ARG typename TP_V> inline /*0C*/const TP_V * TMapIterator<TP_KT _ARG TP_V> ::value_Prop::operator= (/*0C*/const TP_V * v)
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), value);
   MapIterator_set_value(self ? &self->impl : null, toTA(v));
   return v;
}

/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::value_Prop::operator /*0B*/TIH<TP_V> () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), value);
   return *TIH<TP_V>(fromTA<C(Instance)>(MapIterator_get_value(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TIH<TP_V> TMapIterator<TP_KT _ARG TP_V> ::value_Prop::operator /*0D*/-> () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), value);
   return TIH<TP_V>(fromTA<C(Instance)>(MapIterator_get_value(self ? &self->impl : null)));
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::value_Prop::operator /*0E*/TP_V () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), value);
   return fromTA<TP_V>(MapIterator_get_value(self ? &self->impl : null));
}
/*regGet*/template <typename TP_KT _ARG typename TP_V> inline TMapIterator<TP_KT _ARG TP_V> ::value_Prop::operator /*0F*/TP_V* () const
{
   TSELF(MapIterator, (<TP_KT _ARG TP_V>), value);
   return fromTA<TP_V *>(MapIterator_get_value(self ? &self->impl : null));
}



// member accessors: Method::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TMethod<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(Method, name);
   ((C(Method) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::name_Prop & TMethod<TC, TCO>::name_Prop::operator= (typename TMethod<TC, TCO>::name_Prop & prop)
{
   SELF(Method, name);
   /*0H*/const char * v = prop;
   ((C(Method) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, name);
   return self ? ((C(Method) *)self->impl)->name : null;
}

// member accessors: Method::parent
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Method TMethod<TC, TCO>::parent_Prop::operator= (/*0H*/Method & v)
{
   SELF(Method, parent);
   ((C(Method) *)self->impl)->parent = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::parent_Prop & TMethod<TC, TCO>::parent_Prop::operator= (typename TMethod<TC, TCO>::parent_Prop & prop)
{
   SELF(Method, parent);
   /*0H*/Method v = prop;
   ((C(Method) *)self->impl)->parent = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::parent_Prop::operator /*0I*/Method () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, parent);
   Method value(((C(Method) *)self->impl)->parent);
   return value;
}

// member accessors: Method::left
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Method TMethod<TC, TCO>::left_Prop::operator= (/*0H*/Method & v)
{
   SELF(Method, left);
   ((C(Method) *)self->impl)->left = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::left_Prop & TMethod<TC, TCO>::left_Prop::operator= (typename TMethod<TC, TCO>::left_Prop & prop)
{
   SELF(Method, left);
   /*0H*/Method v = prop;
   ((C(Method) *)self->impl)->left = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::left_Prop::operator /*0I*/Method () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, left);
   Method value(((C(Method) *)self->impl)->left);
   return value;
}

// member accessors: Method::right
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Method TMethod<TC, TCO>::right_Prop::operator= (/*0H*/Method & v)
{
   SELF(Method, right);
   ((C(Method) *)self->impl)->right = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::right_Prop & TMethod<TC, TCO>::right_Prop::operator= (typename TMethod<TC, TCO>::right_Prop & prop)
{
   SELF(Method, right);
   /*0H*/Method v = prop;
   ((C(Method) *)self->impl)->right = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::right_Prop::operator /*0I*/Method () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, right);
   Method value(((C(Method) *)self->impl)->right);
   return value;
}

// member accessors: Method::depth
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TMethod<TC, TCO>::depth_Prop::operator= (/*0H*/int v)
{
   SELF(Method, depth);
   ((C(Method) *)self->impl)->depth = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::depth_Prop & TMethod<TC, TCO>::depth_Prop::operator= (typename TMethod<TC, TCO>::depth_Prop & prop)
{
   SELF(Method, depth);
   /*0H*/int v = prop;
   ((C(Method) *)self->impl)->depth = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::depth_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, depth);
   return self ? ((C(Method) *)self->impl)->depth : 0;
}

// member accessors: Method::vid
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TMethod<TC, TCO>::vid_Prop::operator= (/*0H*/int v)
{
   SELF(Method, vid);
   ((C(Method) *)self->impl)->vid = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::vid_Prop & TMethod<TC, TCO>::vid_Prop::operator= (typename TMethod<TC, TCO>::vid_Prop & prop)
{
   SELF(Method, vid);
   /*0H*/int v = prop;
   ((C(Method) *)self->impl)->vid = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::vid_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, vid);
   return self ? ((C(Method) *)self->impl)->vid : 0;
}

// member accessors: Method::type
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/MethodType TMethod<TC, TCO>::type_Prop::operator= (/*0H*/MethodType v)
{
   SELF(Method, type);
   ((C(Method) *)self->impl)->type = (C(MethodType))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::type_Prop & TMethod<TC, TCO>::type_Prop::operator= (typename TMethod<TC, TCO>::type_Prop & prop)
{
   SELF(Method, type);
   /*0H*/MethodType v = prop;
   ((C(Method) *)self->impl)->type = (C(MethodType))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::type_Prop::operator /*0I*/MethodType () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, type);
   return self ? ((C(Method) *)self->impl)->type : (MethodType)0;
}

// member accessors: Method::_class
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TMethod<TC, TCO>::_class_Prop::operator= (/*0H*/Class & v)
{
   SELF(Method, _class);
   ((C(Method) *)self->impl)->_class = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::_class_Prop & TMethod<TC, TCO>::_class_Prop::operator= (typename TMethod<TC, TCO>::_class_Prop & prop)
{
   SELF(Method, _class);
   /*0H*/Class v = prop;
   ((C(Method) *)self->impl)->_class = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::_class_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, _class);
   Class value(((C(Method) *)self->impl)->_class);
   return value;
}

// member accessors: Method::symbol
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TMethod<TC, TCO>::symbol_Prop::operator= (/*0H*/void * v)
{
   SELF(Method, symbol);
   ((C(Method) *)self->impl)->symbol = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::symbol_Prop & TMethod<TC, TCO>::symbol_Prop::operator= (typename TMethod<TC, TCO>::symbol_Prop & prop)
{
   SELF(Method, symbol);
   /*0H*/void * v = prop;
   ((C(Method) *)self->impl)->symbol = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::symbol_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, symbol);
   return self ? ((C(Method) *)self->impl)->symbol : null;
}

// member accessors: Method::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TMethod<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(Method, dataTypeString);
   ((C(Method) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::dataTypeString_Prop & TMethod<TC, TCO>::dataTypeString_Prop::operator= (typename TMethod<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(Method, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(Method) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, dataTypeString);
   return self ? ((C(Method) *)self->impl)->dataTypeString : null;
}

// member accessors: Method::memberAccess
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Access_Mode TMethod<TC, TCO>::memberAccess_Prop::operator= (/*0H*/Access_Mode v)
{
   SELF(Method, memberAccess);
   ((C(Method) *)self->impl)->memberAccess = (C(AccessMode))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TMethod<TC, TCO>::memberAccess_Prop & TMethod<TC, TCO>::memberAccess_Prop::operator= (typename TMethod<TC, TCO>::memberAccess_Prop & prop)
{
   SELF(Method, memberAccess);
   /*0H*/Access_Mode v = prop;
   ((C(Method) *)self->impl)->memberAccess = (C(AccessMode))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TMethod<TC, TCO>::memberAccess_Prop::operator /*0I*/Access_Mode () const
{
   __attribute__((unused)) TMethod TCTCO * self = CONTAINER_OF(this, TMethod TCTCO, memberAccess);
   return self ? ((C(Method) *)self->impl)->memberAccess : (Access_Mode)0;
}

inline Module::Module_onLoad_Functor::FunctionType Module::Module_onLoad_Functor::operator= (FunctionType func)
{
   SELF(Module, onLoad);
   if(self->vTbl == Module::_cpp_class.vTbl)
   {
      uint size = Module::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Module::Module_onLoad_Functor::FunctionType, size);
      memcpy(self->vTbl, Module::_cpp_class.vTbl, sizeof(Module::Module_onLoad_Functor::FunctionType) * size);
   }
   ((Module::Module_onLoad_Functor::FunctionType *)self->vTbl)[M_VTBLID(Module, onLoad)] = func;
   return func;
}
inline bool Module::Module_onLoad_Functor::operator()( )
{
   SELF(Module, onLoad);
   return (bool)Module_onLoad(self ? self->impl : (C(Module))null);
}
// inline void Module::register_onLoad(CPPClass & cl, Module::Module_onLoad_Functor::FunctionType func)
// {
//    ((Module::Module_onLoad_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Module, onLoad)] = func;
// }

inline Module::Module_onUnload_Functor::FunctionType Module::Module_onUnload_Functor::operator= (FunctionType func)
{
   SELF(Module, onUnload);
   if(self->vTbl == Module::_cpp_class.vTbl)
   {
      uint size = Module::_cpp_class.impl->vTblSize;
      self->vTbl = (void (**)())newt(Module::Module_onUnload_Functor::FunctionType, size);
      memcpy(self->vTbl, Module::_cpp_class.vTbl, sizeof(Module::Module_onUnload_Functor::FunctionType) * size);
   }
   ((Module::Module_onUnload_Functor::FunctionType *)self->vTbl)[M_VTBLID(Module, onUnload)] = func;
   return func;
}
inline void Module::Module_onUnload_Functor::operator()( )
{
   SELF(Module, onUnload);
   Module_onUnload(self ? self->impl : (C(Module))null);
}
// inline void Module::register_onUnload(CPPClass & cl, Module::Module_onUnload_Functor::FunctionType func)
// {
//    ((Module::Module_onUnload_Functor::FunctionType *)cl.vTbl)[M_VTBLID(Module, onUnload)] = func;
// }




// member accessors: NameSpace::name
// (struct::const char *)

/*nstSet*/inline /*0H*/const char * NameSpace::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(NameSpace, name);
   self->impl.name = v;
   return v;
}

/*regSet*/inline NameSpace::name_Prop & NameSpace::name_Prop::operator= (NameSpace::name_Prop & prop)
{
   SELF(NameSpace, name);
   /*0H*/const char * v = prop;
   self->impl.name = v;
   return prop;
}
/*regGet*/inline NameSpace::name_Prop::operator /*0I*/const char * () const
{
   SELF(NameSpace, name);
   return self ? ((C(NameSpace) *)&self->impl)->name : null;
}

// member accessors: NameSpace::depth
// (struct::int)

/*nstSet*/inline /*0H*/int NameSpace::depth_Prop::operator= (/*0H*/int v)
{
   SELF(NameSpace, depth);
   self->impl.depth = v;
   return v;
}

/*regSet*/inline NameSpace::depth_Prop & NameSpace::depth_Prop::operator= (NameSpace::depth_Prop & prop)
{
   SELF(NameSpace, depth);
   /*0H*/int v = prop;
   self->impl.depth = v;
   return prop;
}
/*regGet*/inline NameSpace::depth_Prop::operator /*0I*/int () const
{
   SELF(NameSpace, depth);
   return self ? ((C(NameSpace) *)&self->impl)->depth : 0;
}

// member accessors: NameSpace::nameSpaces
// (struct::struct)

/*nstSet*/inline /*0H*/BinaryTree NameSpace::nameSpaces_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(NameSpace, nameSpaces);
   self->impl.nameSpaces = v.impl;
   return v;
}

/*regSet*/inline NameSpace::nameSpaces_Prop & NameSpace::nameSpaces_Prop::operator= (NameSpace::nameSpaces_Prop & prop)
{
   SELF(NameSpace, nameSpaces);
   /*0H*/BinaryTree v = prop;
   self->impl.nameSpaces = v.impl;
   return prop;
}
/*regGet*/inline NameSpace::nameSpaces_Prop::operator /*0I*/BinaryTree () const
{
   SELF(NameSpace, nameSpaces);
   BinaryTree value(((C(NameSpace) *)&self->impl)->nameSpaces);
   return value;
}

// member accessors: NameSpace::classes
// (struct::struct)

/*nstSet*/inline /*0H*/BinaryTree NameSpace::classes_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(NameSpace, classes);
   self->impl.classes = v.impl;
   return v;
}

/*regSet*/inline NameSpace::classes_Prop & NameSpace::classes_Prop::operator= (NameSpace::classes_Prop & prop)
{
   SELF(NameSpace, classes);
   /*0H*/BinaryTree v = prop;
   self->impl.classes = v.impl;
   return prop;
}
/*regGet*/inline NameSpace::classes_Prop::operator /*0I*/BinaryTree () const
{
   SELF(NameSpace, classes);
   BinaryTree value(((C(NameSpace) *)&self->impl)->classes);
   return value;
}

// member accessors: NameSpace::defines
// (struct::struct)

/*nstSet*/inline /*0H*/BinaryTree NameSpace::defines_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(NameSpace, defines);
   self->impl.defines = v.impl;
   return v;
}

/*regSet*/inline NameSpace::defines_Prop & NameSpace::defines_Prop::operator= (NameSpace::defines_Prop & prop)
{
   SELF(NameSpace, defines);
   /*0H*/BinaryTree v = prop;
   self->impl.defines = v.impl;
   return prop;
}
/*regGet*/inline NameSpace::defines_Prop::operator /*0I*/BinaryTree () const
{
   SELF(NameSpace, defines);
   BinaryTree value(((C(NameSpace) *)&self->impl)->defines);
   return value;
}

// member accessors: NameSpace::functions
// (struct::struct)

/*nstSet*/inline /*0H*/BinaryTree NameSpace::functions_Prop::operator= (/*0H*/BinaryTree v)
{
   SELF(NameSpace, functions);
   self->impl.functions = v.impl;
   return v;
}

/*regSet*/inline NameSpace::functions_Prop & NameSpace::functions_Prop::operator= (NameSpace::functions_Prop & prop)
{
   SELF(NameSpace, functions);
   /*0H*/BinaryTree v = prop;
   self->impl.functions = v.impl;
   return prop;
}
/*regGet*/inline NameSpace::functions_Prop::operator /*0I*/BinaryTree () const
{
   SELF(NameSpace, functions);
   BinaryTree value(((C(NameSpace) *)&self->impl)->functions);
   return value;
}



// member accessors: ObjectInfo::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ObjectInfo TObjectInfo<TC, TCO>::prev_Prop::operator= (/*0H*/ObjectInfo & v)
{
   SELF(ObjectInfo, prev);
   ((C(ObjectInfo) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::prev_Prop & TObjectInfo<TC, TCO>::prev_Prop::operator= (typename TObjectInfo<TC, TCO>::prev_Prop & prop)
{
   SELF(ObjectInfo, prev);
   /*0H*/ObjectInfo v = prop;
   ((C(ObjectInfo) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::prev_Prop::operator /*0I*/ObjectInfo () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, prev);
   ObjectInfo value(((C(ObjectInfo) *)self->impl)->prev);
   return value;
}

// member accessors: ObjectInfo::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ObjectInfo TObjectInfo<TC, TCO>::next_Prop::operator= (/*0H*/ObjectInfo & v)
{
   SELF(ObjectInfo, next);
   ((C(ObjectInfo) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::next_Prop & TObjectInfo<TC, TCO>::next_Prop::operator= (typename TObjectInfo<TC, TCO>::next_Prop & prop)
{
   SELF(ObjectInfo, next);
   /*0H*/ObjectInfo v = prop;
   ((C(ObjectInfo) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::next_Prop::operator /*0I*/ObjectInfo () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, next);
   ObjectInfo value(((C(ObjectInfo) *)self->impl)->next);
   return value;
}

// member accessors: ObjectInfo::instance
// (nohead::normal)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Instance TObjectInfo<TC, TCO>::instance_Prop::operator= (/*0H*/Instance v)
{
   SELF(ObjectInfo, instance);
   ((C(ObjectInfo) *)self->impl)->instance = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::instance_Prop & TObjectInfo<TC, TCO>::instance_Prop::operator= (typename TObjectInfo<TC, TCO>::instance_Prop & prop)
{
   SELF(ObjectInfo, instance);
   /*0H*/Instance v = prop;
   ((C(ObjectInfo) *)self->impl)->instance = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::instance_Prop::operator /*0I*/Instance () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, instance);
   Instance value(((C(ObjectInfo) *)self->impl)->instance);
   return value;
}

// member accessors: ObjectInfo::name
// (nohead::char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/char * TObjectInfo<TC, TCO>::name_Prop::operator= (/*0H*/char * v)
{
   SELF(ObjectInfo, name);
   ((C(ObjectInfo) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::name_Prop & TObjectInfo<TC, TCO>::name_Prop::operator= (typename TObjectInfo<TC, TCO>::name_Prop & prop)
{
   SELF(ObjectInfo, name);
   /*0H*/char * v = prop;
   ((C(ObjectInfo) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::name_Prop::operator /*0I*/char * () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, name);
   return self ? ((C(ObjectInfo) *)self->impl)->name : null;
}

// member accessors: ObjectInfo::deleted
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TObjectInfo<TC, TCO>::deleted_Prop::operator= (/*0H*/bool v)
{
   SELF(ObjectInfo, deleted);
   ((C(ObjectInfo) *)self->impl)->deleted = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::deleted_Prop & TObjectInfo<TC, TCO>::deleted_Prop::operator= (typename TObjectInfo<TC, TCO>::deleted_Prop & prop)
{
   SELF(ObjectInfo, deleted);
   /*0H*/bool v = prop;
   ((C(ObjectInfo) *)self->impl)->deleted = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::deleted_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, deleted);
   return self ? ((C(ObjectInfo) *)self->impl)->deleted : (bool)0;
}

// member accessors: ObjectInfo::oClass
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/ObjectInfo TObjectInfo<TC, TCO>::oClass_Prop::operator= (/*0H*/ObjectInfo & v)
{
   SELF(ObjectInfo, oClass);
   ((C(ObjectInfo) *)self->impl)->oClass = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::oClass_Prop & TObjectInfo<TC, TCO>::oClass_Prop::operator= (typename TObjectInfo<TC, TCO>::oClass_Prop & prop)
{
   SELF(ObjectInfo, oClass);
   /*0H*/ObjectInfo v = prop;
   ((C(ObjectInfo) *)self->impl)->oClass = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::oClass_Prop::operator /*0I*/ObjectInfo () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, oClass);
   ObjectInfo value(((C(ObjectInfo) *)self->impl)->oClass);
   return value;
}

// member accessors: ObjectInfo::instances
// (nohead::struct)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldList TObjectInfo<TC, TCO>::instances_Prop::operator= (/*0H*/OldList v)
{
   SELF(ObjectInfo, instances);
   ((C(ObjectInfo) *)self->impl)->instances = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::instances_Prop & TObjectInfo<TC, TCO>::instances_Prop::operator= (typename TObjectInfo<TC, TCO>::instances_Prop & prop)
{
   SELF(ObjectInfo, instances);
   /*0H*/OldList v = prop;
   ((C(ObjectInfo) *)self->impl)->instances = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::instances_Prop::operator /*0I*/OldList () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, instances);
   OldList value(((C(ObjectInfo) *)self->impl)->instances);
   return value;
}

// member accessors: ObjectInfo::modified
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TObjectInfo<TC, TCO>::modified_Prop::operator= (/*0H*/bool v)
{
   SELF(ObjectInfo, modified);
   ((C(ObjectInfo) *)self->impl)->modified = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::modified_Prop & TObjectInfo<TC, TCO>::modified_Prop::operator= (typename TObjectInfo<TC, TCO>::modified_Prop & prop)
{
   SELF(ObjectInfo, modified);
   /*0H*/bool v = prop;
   ((C(ObjectInfo) *)self->impl)->modified = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::modified_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, modified);
   return self ? ((C(ObjectInfo) *)self->impl)->modified : (bool)0;
}

// member accessors: ObjectInfo::i18nStrings
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TObjectInfo<TC, TCO>::i18nStrings_Prop::operator= (/*0H*/void * v)
{
   SELF(ObjectInfo, i18nStrings);
   ((C(ObjectInfo) *)self->impl)->i18nStrings = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TObjectInfo<TC, TCO>::i18nStrings_Prop & TObjectInfo<TC, TCO>::i18nStrings_Prop::operator= (typename TObjectInfo<TC, TCO>::i18nStrings_Prop & prop)
{
   SELF(ObjectInfo, i18nStrings);
   /*0H*/void * v = prop;
   ((C(ObjectInfo) *)self->impl)->i18nStrings = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TObjectInfo<TC, TCO>::i18nStrings_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TObjectInfo TCTCO * self = CONTAINER_OF(this, TObjectInfo TCTCO, i18nStrings);
   return self ? ((C(ObjectInfo) *)self->impl)->i18nStrings : null;
}



// member accessors: Property::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Property TProperty<TC, TCO>::prev_Prop::operator= (/*0H*/Property & v)
{
   SELF(Property, prev);
   ((C(Property) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::prev_Prop & TProperty<TC, TCO>::prev_Prop::operator= (typename TProperty<TC, TCO>::prev_Prop & prop)
{
   SELF(Property, prev);
   /*0H*/Property v = prop;
   ((C(Property) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::prev_Prop::operator /*0I*/Property () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, prev);
   Property value(((C(Property) *)self->impl)->prev);
   return value;
}

// member accessors: Property::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Property TProperty<TC, TCO>::next_Prop::operator= (/*0H*/Property & v)
{
   SELF(Property, next);
   ((C(Property) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::next_Prop & TProperty<TC, TCO>::next_Prop::operator= (typename TProperty<TC, TCO>::next_Prop & prop)
{
   SELF(Property, next);
   /*0H*/Property v = prop;
   ((C(Property) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::next_Prop::operator /*0I*/Property () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, next);
   Property value(((C(Property) *)self->impl)->next);
   return value;
}

// member accessors: Property::name
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TProperty<TC, TCO>::name_Prop::operator= (/*0H*/const char * v)
{
   SELF(Property, name);
   ((C(Property) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::name_Prop & TProperty<TC, TCO>::name_Prop::operator= (typename TProperty<TC, TCO>::name_Prop & prop)
{
   SELF(Property, name);
   /*0H*/const char * v = prop;
   ((C(Property) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::name_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, name);
   return self ? ((C(Property) *)self->impl)->name : null;
}

// member accessors: Property::isProperty
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TProperty<TC, TCO>::isProperty_Prop::operator= (/*0H*/bool v)
{
   SELF(Property, isProperty);
   ((C(Property) *)self->impl)->isProperty = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::isProperty_Prop & TProperty<TC, TCO>::isProperty_Prop::operator= (typename TProperty<TC, TCO>::isProperty_Prop & prop)
{
   SELF(Property, isProperty);
   /*0H*/bool v = prop;
   ((C(Property) *)self->impl)->isProperty = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::isProperty_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, isProperty);
   return self ? ((C(Property) *)self->impl)->isProperty : (bool)0;
}

// member accessors: Property::memberAccess
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Access_Mode TProperty<TC, TCO>::memberAccess_Prop::operator= (/*0H*/Access_Mode v)
{
   SELF(Property, memberAccess);
   ((C(Property) *)self->impl)->memberAccess = (C(AccessMode))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::memberAccess_Prop & TProperty<TC, TCO>::memberAccess_Prop::operator= (typename TProperty<TC, TCO>::memberAccess_Prop & prop)
{
   SELF(Property, memberAccess);
   /*0H*/Access_Mode v = prop;
   ((C(Property) *)self->impl)->memberAccess = (C(AccessMode))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::memberAccess_Prop::operator /*0I*/Access_Mode () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, memberAccess);
   return self ? ((C(Property) *)self->impl)->memberAccess : (Access_Mode)0;
}

// member accessors: Property::id
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TProperty<TC, TCO>::id_Prop::operator= (/*0H*/int v)
{
   SELF(Property, id);
   ((C(Property) *)self->impl)->id = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::id_Prop & TProperty<TC, TCO>::id_Prop::operator= (typename TProperty<TC, TCO>::id_Prop & prop)
{
   SELF(Property, id);
   /*0H*/int v = prop;
   ((C(Property) *)self->impl)->id = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::id_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, id);
   return self ? ((C(Property) *)self->impl)->id : 0;
}

// member accessors: Property::_class
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TProperty<TC, TCO>::_class_Prop::operator= (/*0H*/Class & v)
{
   SELF(Property, _class);
   ((C(Property) *)self->impl)->_class = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::_class_Prop & TProperty<TC, TCO>::_class_Prop::operator= (typename TProperty<TC, TCO>::_class_Prop & prop)
{
   SELF(Property, _class);
   /*0H*/Class v = prop;
   ((C(Property) *)self->impl)->_class = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::_class_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, _class);
   Class value(((C(Property) *)self->impl)->_class);
   return value;
}

// member accessors: Property::dataTypeString
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TProperty<TC, TCO>::dataTypeString_Prop::operator= (/*0H*/const char * v)
{
   SELF(Property, dataTypeString);
   ((C(Property) *)self->impl)->dataTypeString = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::dataTypeString_Prop & TProperty<TC, TCO>::dataTypeString_Prop::operator= (typename TProperty<TC, TCO>::dataTypeString_Prop & prop)
{
   SELF(Property, dataTypeString);
   /*0H*/const char * v = prop;
   ((C(Property) *)self->impl)->dataTypeString = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::dataTypeString_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, dataTypeString);
   return self ? ((C(Property) *)self->impl)->dataTypeString : null;
}

// member accessors: Property::dataTypeClass
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Class TProperty<TC, TCO>::dataTypeClass_Prop::operator= (/*0H*/Class & v)
{
   SELF(Property, dataTypeClass);
   ((C(Property) *)self->impl)->dataTypeClass = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::dataTypeClass_Prop & TProperty<TC, TCO>::dataTypeClass_Prop::operator= (typename TProperty<TC, TCO>::dataTypeClass_Prop & prop)
{
   SELF(Property, dataTypeClass);
   /*0H*/Class v = prop;
   ((C(Property) *)self->impl)->dataTypeClass = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::dataTypeClass_Prop::operator /*0I*/Class () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, dataTypeClass);
   Class value(((C(Property) *)self->impl)->dataTypeClass);
   return value;
}

// member accessors: Property::data
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TProperty<TC, TCO>::data_Prop::operator= (/*0H*/void * v)
{
   SELF(Property, data);
   ((C(Property) *)self->impl)->data = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::data_Prop & TProperty<TC, TCO>::data_Prop::operator= (typename TProperty<TC, TCO>::data_Prop & prop)
{
   SELF(Property, data);
   /*0H*/void * v = prop;
   ((C(Property) *)self->impl)->data = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::data_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, data);
   return self ? ((C(Property) *)self->impl)->data : null;
}

// member accessors: Property::symbol
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TProperty<TC, TCO>::symbol_Prop::operator= (/*0H*/void * v)
{
   SELF(Property, symbol);
   ((C(Property) *)self->impl)->symbol = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::symbol_Prop & TProperty<TC, TCO>::symbol_Prop::operator= (typename TProperty<TC, TCO>::symbol_Prop & prop)
{
   SELF(Property, symbol);
   /*0H*/void * v = prop;
   ((C(Property) *)self->impl)->symbol = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::symbol_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, symbol);
   return self ? ((C(Property) *)self->impl)->symbol : null;
}

// member accessors: Property::vid
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TProperty<TC, TCO>::vid_Prop::operator= (/*0H*/int v)
{
   SELF(Property, vid);
   ((C(Property) *)self->impl)->vid = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::vid_Prop & TProperty<TC, TCO>::vid_Prop::operator= (typename TProperty<TC, TCO>::vid_Prop & prop)
{
   SELF(Property, vid);
   /*0H*/int v = prop;
   ((C(Property) *)self->impl)->vid = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::vid_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, vid);
   return self ? ((C(Property) *)self->impl)->vid : 0;
}

// member accessors: Property::conversion
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TProperty<TC, TCO>::conversion_Prop::operator= (/*0H*/bool v)
{
   SELF(Property, conversion);
   ((C(Property) *)self->impl)->conversion = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::conversion_Prop & TProperty<TC, TCO>::conversion_Prop::operator= (typename TProperty<TC, TCO>::conversion_Prop & prop)
{
   SELF(Property, conversion);
   /*0H*/bool v = prop;
   ((C(Property) *)self->impl)->conversion = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::conversion_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, conversion);
   return self ? ((C(Property) *)self->impl)->conversion : (bool)0;
}

// member accessors: Property::watcherOffset
// (nohead::uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/uint TProperty<TC, TCO>::watcherOffset_Prop::operator= (/*0H*/uint v)
{
   SELF(Property, watcherOffset);
   ((C(Property) *)self->impl)->watcherOffset = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::watcherOffset_Prop & TProperty<TC, TCO>::watcherOffset_Prop::operator= (typename TProperty<TC, TCO>::watcherOffset_Prop & prop)
{
   SELF(Property, watcherOffset);
   /*0H*/uint v = prop;
   ((C(Property) *)self->impl)->watcherOffset = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::watcherOffset_Prop::operator /*0I*/uint () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, watcherOffset);
   return self ? ((C(Property) *)self->impl)->watcherOffset : 0;
}

// member accessors: Property::category
// (nohead::const char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/const char * TProperty<TC, TCO>::category_Prop::operator= (/*0H*/const char * v)
{
   SELF(Property, category);
   ((C(Property) *)self->impl)->category = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::category_Prop & TProperty<TC, TCO>::category_Prop::operator= (typename TProperty<TC, TCO>::category_Prop & prop)
{
   SELF(Property, category);
   /*0H*/const char * v = prop;
   ((C(Property) *)self->impl)->category = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::category_Prop::operator /*0I*/const char * () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, category);
   return self ? ((C(Property) *)self->impl)->category : null;
}

// member accessors: Property::compiled
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TProperty<TC, TCO>::compiled_Prop::operator= (/*0H*/bool v)
{
   SELF(Property, compiled);
   ((C(Property) *)self->impl)->compiled = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::compiled_Prop & TProperty<TC, TCO>::compiled_Prop::operator= (typename TProperty<TC, TCO>::compiled_Prop & prop)
{
   SELF(Property, compiled);
   /*0H*/bool v = prop;
   ((C(Property) *)self->impl)->compiled = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::compiled_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, compiled);
   return self ? ((C(Property) *)self->impl)->compiled : (bool)0;
}

// member accessors: Property::selfWatchable
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TProperty<TC, TCO>::selfWatchable_Prop::operator= (/*0H*/bool v)
{
   SELF(Property, selfWatchable);
   ((C(Property) *)self->impl)->selfWatchable = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::selfWatchable_Prop & TProperty<TC, TCO>::selfWatchable_Prop::operator= (typename TProperty<TC, TCO>::selfWatchable_Prop & prop)
{
   SELF(Property, selfWatchable);
   /*0H*/bool v = prop;
   ((C(Property) *)self->impl)->selfWatchable = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::selfWatchable_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, selfWatchable);
   return self ? ((C(Property) *)self->impl)->selfWatchable : (bool)0;
}

// member accessors: Property::isWatchable
// (nohead::enum|uint)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/bool TProperty<TC, TCO>::isWatchable_Prop::operator= (/*0H*/bool v)
{
   SELF(Property, isWatchable);
   ((C(Property) *)self->impl)->isWatchable = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TProperty<TC, TCO>::isWatchable_Prop & TProperty<TC, TCO>::isWatchable_Prop::operator= (typename TProperty<TC, TCO>::isWatchable_Prop & prop)
{
   SELF(Property, isWatchable);
   /*0H*/bool v = prop;
   ((C(Property) *)self->impl)->isWatchable = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TProperty<TC, TCO>::isWatchable_Prop::operator /*0I*/bool () const
{
   __attribute__((unused)) TProperty TCTCO * self = CONTAINER_OF(this, TProperty TCTCO, isWatchable);
   return self ? ((C(Property) *)self->impl)->isWatchable : (bool)0;
}

inline SerialBuffer::SerialBuffer(unsigned char * _buffer, uintsize count = 0, uintsize _size = 0, uintsize pos = 0, unsigned char * buffer = null, uint size = 0) : SerialBuffer()
{
   this->_buffer = _buffer;
   this->count = count;
   this->_size = _size;
   this->pos = pos;
   this->buffer = buffer;
   this->size = size;
}


inline void SerialBuffer::free()
{
   SerialBuffer_free(impl);
}


// property callers: SerialBuffer::buffer
// (normal::byte *)

/*nstSet*/inline /*0H*/unsigned char * SerialBuffer::buffer_Prop::operator= (/*0H*/unsigned char * v)
{
   SELF(SerialBuffer, buffer);
   SerialBuffer_set_buffer(self ? self->impl : null, v);
   return v;
}

/*regSet*/inline SerialBuffer::buffer_Prop & SerialBuffer::buffer_Prop::operator= (SerialBuffer::buffer_Prop & prop)
{
   SELF(SerialBuffer, buffer);
   /*0H*/unsigned char * v = prop;
   SerialBuffer_set_buffer(self ? self->impl : null, v);
   return prop;
}
/*regGet*/inline SerialBuffer::buffer_Prop::operator /*0I*/unsigned char * () const
{
   SELF(SerialBuffer, buffer);
   return SerialBuffer_get_buffer(self ? self->impl : null);
}

// property callers: SerialBuffer::size
// (normal::uint)

/*nstSet*/inline /*0H*/uint SerialBuffer::size_Prop::operator= (/*0H*/uint v)
{
   SELF(SerialBuffer, size);
   SerialBuffer_set_size(self ? self->impl : null, v);
   return v;
}

/*regSet*/inline SerialBuffer::size_Prop & SerialBuffer::size_Prop::operator= (SerialBuffer::size_Prop & prop)
{
   SELF(SerialBuffer, size);
   /*0H*/uint v = prop;
   SerialBuffer_set_size(self ? self->impl : null, v);
   return prop;
}
/*regGet*/inline SerialBuffer::size_Prop::operator /*0I*/uint () const
{
   SELF(SerialBuffer, size);
   return SerialBuffer_get_size(self ? self->impl : null);
}


// member accessors: SerialBuffer::_buffer
// (normal::byte *)

/*nstSet*/inline /*0H*/unsigned char * SerialBuffer::_buffer_Prop::operator= (/*0H*/unsigned char * v)
{
   SELF(SerialBuffer, _buffer);
   IPTR(self->impl, SerialBuffer)->_buffer = v;
   return v;
}

/*regSet*/inline SerialBuffer::_buffer_Prop & SerialBuffer::_buffer_Prop::operator= (SerialBuffer::_buffer_Prop & prop)
{
   SELF(SerialBuffer, _buffer);
   /*0H*/unsigned char * v = prop;
   IPTR(self->impl, SerialBuffer)->_buffer = v;
   return prop;
}
/*regGet*/inline SerialBuffer::_buffer_Prop::operator /*0I*/unsigned char * () const
{
   SELF(SerialBuffer, _buffer);
   return self ? IPTR(self->impl, SerialBuffer)->_buffer : null;
}

// member accessors: SerialBuffer::count
// (normal::uintsize)

/*nstSet*/inline /*0H*/uintsize SerialBuffer::count_Prop::operator= (/*0H*/uintsize v)
{
   SELF(SerialBuffer, count);
   IPTR(self->impl, SerialBuffer)->count = v;
   return v;
}

/*regSet*/inline SerialBuffer::count_Prop & SerialBuffer::count_Prop::operator= (SerialBuffer::count_Prop & prop)
{
   SELF(SerialBuffer, count);
   /*0H*/uintsize v = prop;
   IPTR(self->impl, SerialBuffer)->count = v;
   return prop;
}
/*regGet*/inline SerialBuffer::count_Prop::operator /*0I*/uintsize () const
{
   SELF(SerialBuffer, count);
   return self ? IPTR(self->impl, SerialBuffer)->count : 0;
}

// member accessors: SerialBuffer::_size
// (normal::uintsize)

/*nstSet*/inline /*0H*/uintsize SerialBuffer::_size_Prop::operator= (/*0H*/uintsize v)
{
   SELF(SerialBuffer, _size);
   IPTR(self->impl, SerialBuffer)->_size = v;
   return v;
}

/*regSet*/inline SerialBuffer::_size_Prop & SerialBuffer::_size_Prop::operator= (SerialBuffer::_size_Prop & prop)
{
   SELF(SerialBuffer, _size);
   /*0H*/uintsize v = prop;
   IPTR(self->impl, SerialBuffer)->_size = v;
   return prop;
}
/*regGet*/inline SerialBuffer::_size_Prop::operator /*0I*/uintsize () const
{
   SELF(SerialBuffer, _size);
   return self ? IPTR(self->impl, SerialBuffer)->_size : 0;
}

// member accessors: SerialBuffer::pos
// (normal::uintsize)

/*nstSet*/inline /*0H*/uintsize SerialBuffer::pos_Prop::operator= (/*0H*/uintsize v)
{
   SELF(SerialBuffer, pos);
   IPTR(self->impl, SerialBuffer)->pos = v;
   return v;
}

/*regSet*/inline SerialBuffer::pos_Prop & SerialBuffer::pos_Prop::operator= (SerialBuffer::pos_Prop & prop)
{
   SELF(SerialBuffer, pos);
   /*0H*/uintsize v = prop;
   IPTR(self->impl, SerialBuffer)->pos = v;
   return prop;
}
/*regGet*/inline SerialBuffer::pos_Prop::operator /*0I*/uintsize () const
{
   SELF(SerialBuffer, pos);
   return self ? IPTR(self->impl, SerialBuffer)->pos : 0;
}




// member accessors: SubModule::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/SubModule TSubModule<TC, TCO>::prev_Prop::operator= (/*0H*/SubModule & v)
{
   SELF(SubModule, prev);
   ((C(SubModule) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TSubModule<TC, TCO>::prev_Prop & TSubModule<TC, TCO>::prev_Prop::operator= (typename TSubModule<TC, TCO>::prev_Prop & prop)
{
   SELF(SubModule, prev);
   /*0H*/SubModule v = prop;
   ((C(SubModule) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TSubModule<TC, TCO>::prev_Prop::operator /*0I*/SubModule () const
{
   __attribute__((unused)) TSubModule TCTCO * self = CONTAINER_OF(this, TSubModule TCTCO, prev);
   SubModule value(((C(SubModule) *)self->impl)->prev);
   return value;
}

// member accessors: SubModule::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/SubModule TSubModule<TC, TCO>::next_Prop::operator= (/*0H*/SubModule & v)
{
   SELF(SubModule, next);
   ((C(SubModule) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TSubModule<TC, TCO>::next_Prop & TSubModule<TC, TCO>::next_Prop::operator= (typename TSubModule<TC, TCO>::next_Prop & prop)
{
   SELF(SubModule, next);
   /*0H*/SubModule v = prop;
   ((C(SubModule) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TSubModule<TC, TCO>::next_Prop::operator /*0I*/SubModule () const
{
   __attribute__((unused)) TSubModule TCTCO * self = CONTAINER_OF(this, TSubModule TCTCO, next);
   SubModule value(((C(SubModule) *)self->impl)->next);
   return value;
}

// member accessors: SubModule::module
// (nohead::normal)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Module TSubModule<TC, TCO>::module_Prop::operator= (/*0H*/Module v)
{
   SELF(SubModule, module);
   ((C(SubModule) *)self->impl)->module = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TSubModule<TC, TCO>::module_Prop & TSubModule<TC, TCO>::module_Prop::operator= (typename TSubModule<TC, TCO>::module_Prop & prop)
{
   SELF(SubModule, module);
   /*0H*/Module v = prop;
   ((C(SubModule) *)self->impl)->module = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TSubModule<TC, TCO>::module_Prop::operator /*0I*/Module () const
{
   __attribute__((unused)) TSubModule TCTCO * self = CONTAINER_OF(this, TSubModule TCTCO, module);
   Module value(((C(SubModule) *)self->impl)->module);
   return value;
}

// member accessors: SubModule::importMode
// (nohead::enum|int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Access_Mode TSubModule<TC, TCO>::importMode_Prop::operator= (/*0H*/Access_Mode v)
{
   SELF(SubModule, importMode);
   ((C(SubModule) *)self->impl)->importMode = (C(AccessMode))v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TSubModule<TC, TCO>::importMode_Prop & TSubModule<TC, TCO>::importMode_Prop::operator= (typename TSubModule<TC, TCO>::importMode_Prop & prop)
{
   SELF(SubModule, importMode);
   /*0H*/Access_Mode v = prop;
   ((C(SubModule) *)self->impl)->importMode = (C(AccessMode))v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TSubModule<TC, TCO>::importMode_Prop::operator /*0I*/Access_Mode () const
{
   __attribute__((unused)) TSubModule TCTCO * self = CONTAINER_OF(this, TSubModule TCTCO, importMode);
   return self ? ((C(SubModule) *)self->impl)->importMode : (Access_Mode)0;
}

///////////////////////////////////////////////// [ecereCOM]/ecere::gui //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


///////////////////////////////////////////////// [ecereCOM]/ecere::sys //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////



template <class TC, C(Class) ** TCO> inline C(BTNode) * TBTNode<TC, TCO>::findPrefix(/*1Ab*/const char * key)
{
   return BTNode_findPrefix((C(BTNode)*)this->impl, /*5Ge*/key);
}
template <class TC, C(Class) ** TCO> inline C(BTNode) * TBTNode<TC, TCO>::findString(/*1Ab*/const char * key)
{
   return BTNode_findString((C(BTNode)*)this->impl, /*5Ge*/key);
}


// property callers: BTNode::prev   // get only
// (nohead::nohead)

/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::prev_Prop::operator /*0I*/BTNode () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, prev);
   BTNode value(BTNode_get_prev(self->impl));
   return value;
}

// property callers: BTNode::next   // get only
// (nohead::nohead)

/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::next_Prop::operator /*0I*/BTNode () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, next);
   BTNode value(BTNode_get_next(self->impl));
   return value;
}

// property callers: BTNode::minimum   // get only
// (nohead::nohead)

/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::minimum_Prop::operator /*0I*/BTNode () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, minimum);
   BTNode value(BTNode_get_minimum(self->impl));
   return value;
}

// property callers: BTNode::maximum   // get only
// (nohead::nohead)

/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::maximum_Prop::operator /*0I*/BTNode () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, maximum);
   BTNode value(BTNode_get_maximum(self->impl));
   return value;
}

// property callers: BTNode::count   // get only
// (nohead::int)

/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::count_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, count);
   return BTNode_get_count(self ? self->impl : null);
}

// property callers: BTNode::depthProp   // get only
// (nohead::int)

/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::depthProp_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, depthProp);
   return BTNode_get_depthProp(self ? self->impl : null);
}


// member accessors: BTNode::key
// (nohead::uintptr)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/uintptr TBTNode<TC, TCO>::key_Prop::operator= (/*0H*/uintptr v)
{
   SELF(BTNode, key);
   ((C(BTNode) *)self->impl)->key = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNode<TC, TCO>::key_Prop & TBTNode<TC, TCO>::key_Prop::operator= (typename TBTNode<TC, TCO>::key_Prop & prop)
{
   SELF(BTNode, key);
   /*0H*/uintptr v = prop;
   ((C(BTNode) *)self->impl)->key = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::key_Prop::operator /*0I*/uintptr () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, key);
   return self ? ((C(BTNode) *)self->impl)->key : 0;
}

// member accessors: BTNode::parent
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BTNode TBTNode<TC, TCO>::parent_Prop::operator= (/*0H*/BTNode & v)
{
   SELF(BTNode, parent);
   ((C(BTNode) *)self->impl)->parent = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNode<TC, TCO>::parent_Prop & TBTNode<TC, TCO>::parent_Prop::operator= (typename TBTNode<TC, TCO>::parent_Prop & prop)
{
   SELF(BTNode, parent);
   /*0H*/BTNode v = prop;
   ((C(BTNode) *)self->impl)->parent = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::parent_Prop::operator /*0I*/BTNode () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, parent);
   BTNode value(((C(BTNode) *)self->impl)->parent);
   return value;
}

// member accessors: BTNode::left
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BTNode TBTNode<TC, TCO>::left_Prop::operator= (/*0H*/BTNode & v)
{
   SELF(BTNode, left);
   ((C(BTNode) *)self->impl)->left = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNode<TC, TCO>::left_Prop & TBTNode<TC, TCO>::left_Prop::operator= (typename TBTNode<TC, TCO>::left_Prop & prop)
{
   SELF(BTNode, left);
   /*0H*/BTNode v = prop;
   ((C(BTNode) *)self->impl)->left = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::left_Prop::operator /*0I*/BTNode () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, left);
   BTNode value(((C(BTNode) *)self->impl)->left);
   return value;
}

// member accessors: BTNode::right
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/BTNode TBTNode<TC, TCO>::right_Prop::operator= (/*0H*/BTNode & v)
{
   SELF(BTNode, right);
   ((C(BTNode) *)self->impl)->right = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNode<TC, TCO>::right_Prop & TBTNode<TC, TCO>::right_Prop::operator= (typename TBTNode<TC, TCO>::right_Prop & prop)
{
   SELF(BTNode, right);
   /*0H*/BTNode v = prop;
   ((C(BTNode) *)self->impl)->right = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::right_Prop::operator /*0I*/BTNode () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, right);
   BTNode value(((C(BTNode) *)self->impl)->right);
   return value;
}

// member accessors: BTNode::depth
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TBTNode<TC, TCO>::depth_Prop::operator= (/*0H*/int v)
{
   SELF(BTNode, depth);
   ((C(BTNode) *)self->impl)->depth = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TBTNode<TC, TCO>::depth_Prop & TBTNode<TC, TCO>::depth_Prop::operator= (typename TBTNode<TC, TCO>::depth_Prop & prop)
{
   SELF(BTNode, depth);
   /*0H*/int v = prop;
   ((C(BTNode) *)self->impl)->depth = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TBTNode<TC, TCO>::depth_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TBTNode TCTCO * self = CONTAINER_OF(this, TBTNode TCTCO, depth);
   return self ? ((C(BTNode) *)self->impl)->depth : 0;
}


inline C(bool) BinaryTree::add(/*1Ab*/BTNode node)
{
   return BinaryTree_add(&impl, /*5De*/node.impl);
}
inline C(bool) BinaryTree::check()
{
   return BinaryTree_check(&impl);
}
inline int BinaryTree::compareInt(/*1Ab*/uintptr a, /*1Ab*/uintptr b)
{
   return BinaryTree_compareInt(&impl, /*5Ge*/a, /*5Ge*/b);
}
inline int BinaryTree::compareString(/*1Ab*/const char * a, /*1Ab*/const char * b)
{
   return BinaryTree_compareString(&impl, /*5Ge*/a, /*5Ge*/b);
}
inline void BinaryTree::_delete(/*1Ab*/BTNode node)
{
   BinaryTree_delete(&impl, /*5De*/node.impl);
}
inline C(BTNode) * BinaryTree::find(/*1Ab*/uintptr key)
{
   return BinaryTree_find(&impl, /*5Ge*/key);
}
inline C(BTNode) * BinaryTree::findAll(/*1Ab*/uintptr key)
{
   return BinaryTree_findAll(&impl, /*5Ge*/key);
}
inline C(BTNode) * BinaryTree::findPrefix(/*1Ab*/const char * key)
{
   return BinaryTree_findPrefix(&impl, /*5Ge*/key);
}
inline C(BTNode) * BinaryTree::findString(/*1Ab*/const char * key)
{
   return BinaryTree_findString(&impl, /*5Ge*/key);
}
inline void BinaryTree::free()
{
   BinaryTree_free(&impl);
}
inline void BinaryTree::freeString(/*1Ab*/char * string)
{
   BinaryTree_freeString(/*5Ge*/string);
}
inline char * BinaryTree::print(/*1Ab*/char * output, /*1Ab*/TreePrintStyle tps)
{
   return BinaryTree_print(&impl, /*5Ge*/output, /*5Ee*/(C(TreePrintStyle))tps);
}
inline void BinaryTree::remove(/*1Ab*/BTNode node)
{
   BinaryTree_remove(&impl, /*5De*/node.impl);
}


// property callers: BinaryTree::first   // get only
// (struct::nohead)

/*regGet*/inline BinaryTree::first_Prop::operator /*0I*/BTNode () const
{
   SELF(BinaryTree, first);
   BTNode value(BinaryTree_get_first(&self->impl));
   return value;
}

// property callers: BinaryTree::last   // get only
// (struct::nohead)

/*regGet*/inline BinaryTree::last_Prop::operator /*0I*/BTNode () const
{
   SELF(BinaryTree, last);
   BTNode value(BinaryTree_get_last(&self->impl));
   return value;
}


// member accessors: BinaryTree::root
// (struct::nohead)

/*nstSet*/inline /*0H*/BTNode BinaryTree::root_Prop::operator= (/*0H*/BTNode & v)
{
   SELF(BinaryTree, root);
   self->impl.root = v.impl;
   return v;
}

/*regSet*/inline BinaryTree::root_Prop & BinaryTree::root_Prop::operator= (BinaryTree::root_Prop & prop)
{
   SELF(BinaryTree, root);
   /*0H*/BTNode v = prop;
   self->impl.root = v.impl;
   return prop;
}
/*regGet*/inline BinaryTree::root_Prop::operator /*0I*/BTNode () const
{
   SELF(BinaryTree, root);
   BTNode value(((C(BinaryTree) *)&self->impl)->root);
   return value;
}

// member accessors: BinaryTree::count
// (struct::int)

/*nstSet*/inline /*0H*/int BinaryTree::count_Prop::operator= (/*0H*/int v)
{
   SELF(BinaryTree, count);
   self->impl.count = v;
   return v;
}

/*regSet*/inline BinaryTree::count_Prop & BinaryTree::count_Prop::operator= (BinaryTree::count_Prop & prop)
{
   SELF(BinaryTree, count);
   /*0H*/int v = prop;
   self->impl.count = v;
   return prop;
}
/*regGet*/inline BinaryTree::count_Prop::operator /*0I*/int () const
{
   SELF(BinaryTree, count);
   return self ? ((C(BinaryTree) *)&self->impl)->count : 0;
}



// member accessors: EscapeCStringOptions::escapeSingleQuote
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool EscapeCStringOptions::escapeSingleQuote_Prop::operator= (/*0H*/bool v)
{
   SELF(EscapeCStringOptions, escapeSingleQuote);
   ESCAPECSTRINGOPTIONS_SET_escapeSingleQuote(self->impl, v);
   return v;
}

/*regSet*/inline EscapeCStringOptions::escapeSingleQuote_Prop & EscapeCStringOptions::escapeSingleQuote_Prop::operator= (EscapeCStringOptions::escapeSingleQuote_Prop & prop)
{
   SELF(EscapeCStringOptions, escapeSingleQuote);
   /*0H*/bool v = prop;
   ESCAPECSTRINGOPTIONS_SET_escapeSingleQuote(self->impl, v);
   return prop;
}
/*regGet*/inline EscapeCStringOptions::escapeSingleQuote_Prop::operator /*0I*/bool () const
{
   SELF(EscapeCStringOptions, escapeSingleQuote);
   return (bool)ESCAPECSTRINGOPTIONS_escapeSingleQuote(self->impl);
}

// member accessors: EscapeCStringOptions::escapeDoubleQuotes
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool EscapeCStringOptions::escapeDoubleQuotes_Prop::operator= (/*0H*/bool v)
{
   SELF(EscapeCStringOptions, escapeDoubleQuotes);
   ESCAPECSTRINGOPTIONS_SET_escapeDoubleQuotes(self->impl, v);
   return v;
}

/*regSet*/inline EscapeCStringOptions::escapeDoubleQuotes_Prop & EscapeCStringOptions::escapeDoubleQuotes_Prop::operator= (EscapeCStringOptions::escapeDoubleQuotes_Prop & prop)
{
   SELF(EscapeCStringOptions, escapeDoubleQuotes);
   /*0H*/bool v = prop;
   ESCAPECSTRINGOPTIONS_SET_escapeDoubleQuotes(self->impl, v);
   return prop;
}
/*regGet*/inline EscapeCStringOptions::escapeDoubleQuotes_Prop::operator /*0I*/bool () const
{
   SELF(EscapeCStringOptions, escapeDoubleQuotes);
   return (bool)ESCAPECSTRINGOPTIONS_escapeDoubleQuotes(self->impl);
}

// member accessors: EscapeCStringOptions::writeQuotes
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool EscapeCStringOptions::writeQuotes_Prop::operator= (/*0H*/bool v)
{
   SELF(EscapeCStringOptions, writeQuotes);
   ESCAPECSTRINGOPTIONS_SET_writeQuotes(self->impl, v);
   return v;
}

/*regSet*/inline EscapeCStringOptions::writeQuotes_Prop & EscapeCStringOptions::writeQuotes_Prop::operator= (EscapeCStringOptions::writeQuotes_Prop & prop)
{
   SELF(EscapeCStringOptions, writeQuotes);
   /*0H*/bool v = prop;
   ESCAPECSTRINGOPTIONS_SET_writeQuotes(self->impl, v);
   return prop;
}
/*regGet*/inline EscapeCStringOptions::writeQuotes_Prop::operator /*0I*/bool () const
{
   SELF(EscapeCStringOptions, writeQuotes);
   return (bool)ESCAPECSTRINGOPTIONS_writeQuotes(self->impl);
}

// member accessors: EscapeCStringOptions::multiLine
// (bits::enum|uint)

/*nstSet*/inline /*0H*/bool EscapeCStringOptions::multiLine_Prop::operator= (/*0H*/bool v)
{
   SELF(EscapeCStringOptions, multiLine);
   ESCAPECSTRINGOPTIONS_SET_multiLine(self->impl, v);
   return v;
}

/*regSet*/inline EscapeCStringOptions::multiLine_Prop & EscapeCStringOptions::multiLine_Prop::operator= (EscapeCStringOptions::multiLine_Prop & prop)
{
   SELF(EscapeCStringOptions, multiLine);
   /*0H*/bool v = prop;
   ESCAPECSTRINGOPTIONS_SET_multiLine(self->impl, v);
   return prop;
}
/*regGet*/inline EscapeCStringOptions::multiLine_Prop::operator /*0I*/bool () const
{
   SELF(EscapeCStringOptions, multiLine);
   return (bool)ESCAPECSTRINGOPTIONS_multiLine(self->impl);
}

// member accessors: EscapeCStringOptions::indent
// (bits::int)

/*nstSet*/inline /*0H*/int EscapeCStringOptions::indent_Prop::operator= (/*0H*/int v)
{
   SELF(EscapeCStringOptions, indent);
   ESCAPECSTRINGOPTIONS_SET_indent(self->impl, v);
   return v;
}

/*regSet*/inline EscapeCStringOptions::indent_Prop & EscapeCStringOptions::indent_Prop::operator= (EscapeCStringOptions::indent_Prop & prop)
{
   SELF(EscapeCStringOptions, indent);
   /*0H*/int v = prop;
   ESCAPECSTRINGOPTIONS_SET_indent(self->impl, v);
   return prop;
}
/*regGet*/inline EscapeCStringOptions::indent_Prop::operator /*0I*/int () const
{
   SELF(EscapeCStringOptions, indent);
   return ESCAPECSTRINGOPTIONS_indent(self->impl);
}


template <class TC, C(Class) ** TCO> inline void TItem<TC, TCO>::copy(/*1Ab*/TItem<TC, TCO> src, /*1Ab*/int size)
{
   Item_copy((C(Item)*)this->impl, /*5De*/src.impl, /*5Ge*/size);
}


// member accessors: Item::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Item TItem<TC, TCO>::prev_Prop::operator= (/*0H*/Item & v)
{
   SELF(Item, prev);
   ((C(Item) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TItem<TC, TCO>::prev_Prop & TItem<TC, TCO>::prev_Prop::operator= (typename TItem<TC, TCO>::prev_Prop & prop)
{
   SELF(Item, prev);
   /*0H*/Item v = prop;
   ((C(Item) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TItem<TC, TCO>::prev_Prop::operator /*0I*/Item () const
{
   __attribute__((unused)) TItem TCTCO * self = CONTAINER_OF(this, TItem TCTCO, prev);
   Item value(((C(Item) *)self->impl)->prev);
   return value;
}

// member accessors: Item::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/Item TItem<TC, TCO>::next_Prop::operator= (/*0H*/Item & v)
{
   SELF(Item, next);
   ((C(Item) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TItem<TC, TCO>::next_Prop & TItem<TC, TCO>::next_Prop::operator= (typename TItem<TC, TCO>::next_Prop & prop)
{
   SELF(Item, next);
   /*0H*/Item v = prop;
   ((C(Item) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TItem<TC, TCO>::next_Prop::operator /*0I*/Item () const
{
   __attribute__((unused)) TItem TCTCO * self = CONTAINER_OF(this, TItem TCTCO, next);
   Item value(((C(Item) *)self->impl)->next);
   return value;
}


template <class TC, C(Class) ** TCO> inline void TMutex<TC, TCO>::release()
{
   Mutex_release((C(Mutex)*)this->impl);
}
template <class TC, C(Class) ** TCO> inline void TMutex<TC, TCO>::wait()
{
   Mutex_wait((C(Mutex)*)this->impl);
}


// property callers: Mutex::lockCount   // get only
// (nohead::int)

/*regGet*/template <class TC, C(Class) ** TCO> inline TMutex<TC, TCO>::lockCount_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TMutex TCTCO * self = CONTAINER_OF(this, TMutex TCTCO, lockCount);
   return Mutex_get_lockCount(self ? self->impl : null);
}

// property callers: Mutex::owningThread   // get only
// (nohead::int64)

/*regGet*/template <class TC, C(Class) ** TCO> inline TMutex<TC, TCO>::owningThread_Prop::operator /*0I*/int64 () const
{
   __attribute__((unused)) TMutex TCTCO * self = CONTAINER_OF(this, TMutex TCTCO, owningThread);
   return Mutex_get_owningThread(self ? self->impl : null);
}



// member accessors: NamedItem::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/NamedItem TNamedItem<TC, TCO>::prev_Prop::operator= (/*0H*/NamedItem & v)
{
   SELF(NamedItem, prev);
   ((C(NamedItem) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedItem<TC, TCO>::prev_Prop & TNamedItem<TC, TCO>::prev_Prop::operator= (typename TNamedItem<TC, TCO>::prev_Prop & prop)
{
   SELF(NamedItem, prev);
   /*0H*/NamedItem v = prop;
   ((C(NamedItem) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedItem<TC, TCO>::prev_Prop::operator /*0I*/NamedItem () const
{
   __attribute__((unused)) TNamedItem TCTCO * self = CONTAINER_OF(this, TNamedItem TCTCO, prev);
   NamedItem value(((C(NamedItem) *)self->impl)->prev);
   return value;
}

// member accessors: NamedItem::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/NamedItem TNamedItem<TC, TCO>::next_Prop::operator= (/*0H*/NamedItem & v)
{
   SELF(NamedItem, next);
   ((C(NamedItem) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedItem<TC, TCO>::next_Prop & TNamedItem<TC, TCO>::next_Prop::operator= (typename TNamedItem<TC, TCO>::next_Prop & prop)
{
   SELF(NamedItem, next);
   /*0H*/NamedItem v = prop;
   ((C(NamedItem) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedItem<TC, TCO>::next_Prop::operator /*0I*/NamedItem () const
{
   __attribute__((unused)) TNamedItem TCTCO * self = CONTAINER_OF(this, TNamedItem TCTCO, next);
   NamedItem value(((C(NamedItem) *)self->impl)->next);
   return value;
}

// member accessors: NamedItem::name
// (nohead::char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/char * TNamedItem<TC, TCO>::name_Prop::operator= (/*0H*/char * v)
{
   SELF(NamedItem, name);
   ((C(NamedItem) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedItem<TC, TCO>::name_Prop & TNamedItem<TC, TCO>::name_Prop::operator= (typename TNamedItem<TC, TCO>::name_Prop & prop)
{
   SELF(NamedItem, name);
   /*0H*/char * v = prop;
   ((C(NamedItem) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedItem<TC, TCO>::name_Prop::operator /*0I*/char * () const
{
   __attribute__((unused)) TNamedItem TCTCO * self = CONTAINER_OF(this, TNamedItem TCTCO, name);
   return self ? ((C(NamedItem) *)self->impl)->name : null;
}



// member accessors: NamedLink::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/NamedLink TNamedLink<TC, TCO>::prev_Prop::operator= (/*0H*/NamedLink & v)
{
   SELF(NamedLink, prev);
   ((C(NamedLink) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink<TC, TCO>::prev_Prop & TNamedLink<TC, TCO>::prev_Prop::operator= (typename TNamedLink<TC, TCO>::prev_Prop & prop)
{
   SELF(NamedLink, prev);
   /*0H*/NamedLink v = prop;
   ((C(NamedLink) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink<TC, TCO>::prev_Prop::operator /*0I*/NamedLink () const
{
   __attribute__((unused)) TNamedLink TCTCO * self = CONTAINER_OF(this, TNamedLink TCTCO, prev);
   NamedLink value(((C(NamedLink) *)self->impl)->prev);
   return value;
}

// member accessors: NamedLink::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/NamedLink TNamedLink<TC, TCO>::next_Prop::operator= (/*0H*/NamedLink & v)
{
   SELF(NamedLink, next);
   ((C(NamedLink) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink<TC, TCO>::next_Prop & TNamedLink<TC, TCO>::next_Prop::operator= (typename TNamedLink<TC, TCO>::next_Prop & prop)
{
   SELF(NamedLink, next);
   /*0H*/NamedLink v = prop;
   ((C(NamedLink) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink<TC, TCO>::next_Prop::operator /*0I*/NamedLink () const
{
   __attribute__((unused)) TNamedLink TCTCO * self = CONTAINER_OF(this, TNamedLink TCTCO, next);
   NamedLink value(((C(NamedLink) *)self->impl)->next);
   return value;
}

// member accessors: NamedLink::name
// (nohead::char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/char * TNamedLink<TC, TCO>::name_Prop::operator= (/*0H*/char * v)
{
   SELF(NamedLink, name);
   ((C(NamedLink) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink<TC, TCO>::name_Prop & TNamedLink<TC, TCO>::name_Prop::operator= (typename TNamedLink<TC, TCO>::name_Prop & prop)
{
   SELF(NamedLink, name);
   /*0H*/char * v = prop;
   ((C(NamedLink) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink<TC, TCO>::name_Prop::operator /*0I*/char * () const
{
   __attribute__((unused)) TNamedLink TCTCO * self = CONTAINER_OF(this, TNamedLink TCTCO, name);
   return self ? ((C(NamedLink) *)self->impl)->name : null;
}

// member accessors: NamedLink::data
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TNamedLink<TC, TCO>::data_Prop::operator= (/*0H*/void * v)
{
   SELF(NamedLink, data);
   ((C(NamedLink) *)self->impl)->data = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink<TC, TCO>::data_Prop & TNamedLink<TC, TCO>::data_Prop::operator= (typename TNamedLink<TC, TCO>::data_Prop & prop)
{
   SELF(NamedLink, data);
   /*0H*/void * v = prop;
   ((C(NamedLink) *)self->impl)->data = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink<TC, TCO>::data_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TNamedLink TCTCO * self = CONTAINER_OF(this, TNamedLink TCTCO, data);
   return self ? ((C(NamedLink) *)self->impl)->data : null;
}



// member accessors: NamedLink64::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/NamedLink64 TNamedLink64<TC, TCO>::prev_Prop::operator= (/*0H*/NamedLink64 & v)
{
   SELF(NamedLink64, prev);
   ((C(NamedLink64) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink64<TC, TCO>::prev_Prop & TNamedLink64<TC, TCO>::prev_Prop::operator= (typename TNamedLink64<TC, TCO>::prev_Prop & prop)
{
   SELF(NamedLink64, prev);
   /*0H*/NamedLink64 v = prop;
   ((C(NamedLink64) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink64<TC, TCO>::prev_Prop::operator /*0I*/NamedLink64 () const
{
   __attribute__((unused)) TNamedLink64 TCTCO * self = CONTAINER_OF(this, TNamedLink64 TCTCO, prev);
   NamedLink64 value(((C(NamedLink64) *)self->impl)->prev);
   return value;
}

// member accessors: NamedLink64::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/NamedLink64 TNamedLink64<TC, TCO>::next_Prop::operator= (/*0H*/NamedLink64 & v)
{
   SELF(NamedLink64, next);
   ((C(NamedLink64) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink64<TC, TCO>::next_Prop & TNamedLink64<TC, TCO>::next_Prop::operator= (typename TNamedLink64<TC, TCO>::next_Prop & prop)
{
   SELF(NamedLink64, next);
   /*0H*/NamedLink64 v = prop;
   ((C(NamedLink64) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink64<TC, TCO>::next_Prop::operator /*0I*/NamedLink64 () const
{
   __attribute__((unused)) TNamedLink64 TCTCO * self = CONTAINER_OF(this, TNamedLink64 TCTCO, next);
   NamedLink64 value(((C(NamedLink64) *)self->impl)->next);
   return value;
}

// member accessors: NamedLink64::name
// (nohead::char *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/char * TNamedLink64<TC, TCO>::name_Prop::operator= (/*0H*/char * v)
{
   SELF(NamedLink64, name);
   ((C(NamedLink64) *)self->impl)->name = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink64<TC, TCO>::name_Prop & TNamedLink64<TC, TCO>::name_Prop::operator= (typename TNamedLink64<TC, TCO>::name_Prop & prop)
{
   SELF(NamedLink64, name);
   /*0H*/char * v = prop;
   ((C(NamedLink64) *)self->impl)->name = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink64<TC, TCO>::name_Prop::operator /*0I*/char * () const
{
   __attribute__((unused)) TNamedLink64 TCTCO * self = CONTAINER_OF(this, TNamedLink64 TCTCO, name);
   return self ? ((C(NamedLink64) *)self->impl)->name : null;
}

// member accessors: NamedLink64::data
// (nohead::int64)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int64 TNamedLink64<TC, TCO>::data_Prop::operator= (/*0H*/int64 v)
{
   SELF(NamedLink64, data);
   ((C(NamedLink64) *)self->impl)->data = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TNamedLink64<TC, TCO>::data_Prop & TNamedLink64<TC, TCO>::data_Prop::operator= (typename TNamedLink64<TC, TCO>::data_Prop & prop)
{
   SELF(NamedLink64, data);
   /*0H*/int64 v = prop;
   ((C(NamedLink64) *)self->impl)->data = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TNamedLink64<TC, TCO>::data_Prop::operator /*0I*/int64 () const
{
   __attribute__((unused)) TNamedLink64 TCTCO * self = CONTAINER_OF(this, TNamedLink64 TCTCO, data);
   return self ? ((C(NamedLink64) *)self->impl)->data : 0;
}


template <class TC, C(Class) ** TCO> inline void TOldLink<TC, TCO>::free()
{
   OldLink_free((C(OldLink)*)this->impl);
}


// member accessors: OldLink::prev
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldLink TOldLink<TC, TCO>::prev_Prop::operator= (/*0H*/OldLink & v)
{
   SELF(OldLink, prev);
   ((C(OldLink) *)self->impl)->prev = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TOldLink<TC, TCO>::prev_Prop & TOldLink<TC, TCO>::prev_Prop::operator= (typename TOldLink<TC, TCO>::prev_Prop & prop)
{
   SELF(OldLink, prev);
   /*0H*/OldLink v = prop;
   ((C(OldLink) *)self->impl)->prev = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TOldLink<TC, TCO>::prev_Prop::operator /*0I*/OldLink () const
{
   __attribute__((unused)) TOldLink TCTCO * self = CONTAINER_OF(this, TOldLink TCTCO, prev);
   OldLink value(((C(OldLink) *)self->impl)->prev);
   return value;
}

// member accessors: OldLink::next
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/OldLink TOldLink<TC, TCO>::next_Prop::operator= (/*0H*/OldLink & v)
{
   SELF(OldLink, next);
   ((C(OldLink) *)self->impl)->next = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TOldLink<TC, TCO>::next_Prop & TOldLink<TC, TCO>::next_Prop::operator= (typename TOldLink<TC, TCO>::next_Prop & prop)
{
   SELF(OldLink, next);
   /*0H*/OldLink v = prop;
   ((C(OldLink) *)self->impl)->next = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TOldLink<TC, TCO>::next_Prop::operator /*0I*/OldLink () const
{
   __attribute__((unused)) TOldLink TCTCO * self = CONTAINER_OF(this, TOldLink TCTCO, next);
   OldLink value(((C(OldLink) *)self->impl)->next);
   return value;
}

// member accessors: OldLink::data
// (nohead::void *)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/void * TOldLink<TC, TCO>::data_Prop::operator= (/*0H*/void * v)
{
   SELF(OldLink, data);
   ((C(OldLink) *)self->impl)->data = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TOldLink<TC, TCO>::data_Prop & TOldLink<TC, TCO>::data_Prop::operator= (typename TOldLink<TC, TCO>::data_Prop & prop)
{
   SELF(OldLink, data);
   /*0H*/void * v = prop;
   ((C(OldLink) *)self->impl)->data = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TOldLink<TC, TCO>::data_Prop::operator /*0I*/void * () const
{
   __attribute__((unused)) TOldLink TCTCO * self = CONTAINER_OF(this, TOldLink TCTCO, data);
   return self ? ((C(OldLink) *)self->impl)->data : null;
}

inline OldList::OldList(void * first, void * last = null, int count = 0, uint offset = 0, bool circ = (bool)0)
{
   impl.first = first;
   impl.last = last;
   impl.count = count;
   impl.offset = offset;
   impl.circ = (C(bool))circ;
}


inline void OldList::add(/*1Ab*/void * item)
{
   OldList_add(&impl, /*5Ge*/item);
}
inline C(bool) OldList::addName(/*1Ab*/void * item)
{
   return OldList_addName(&impl, /*5Ge*/item);
}
inline void OldList::clear()
{
   OldList_clear(&impl);
}
inline void OldList::copy(/*1Ab*/OldList & src, /*1Ab*/int size, void (* copy)(void * dest, void * src))
{
   OldList_copy(&impl, /*5De*/&src.impl, /*5Ge*/size, /*5Ge*/copy);
}
inline void OldList::copy(/*1Ac*/OldList * src, /*1Ac*/int size, void (* copy)(void * dest, void * src))
{
   OldList_copy(&impl, /*5Cf*/(C(OldList) *)src, /*5Gf*/size, /*5Gf*/copy);
}
inline void OldList::_delete(/*1Ab*/void * item)
{
   OldList_delete(&impl, /*5Ge*/item);
}
inline C(OldLink) * OldList::findLink(/*1Ab*/void * data)
{
   return OldList_findLink(&impl, /*5Ge*/data);
}
inline void * OldList::findName(/*1Ab*/const char * name, /*1Ab*/bool warn)
{
   return OldList_findName(&impl, /*5Ge*/name, /*5Ee*/(C(bool))warn);
}
inline void * OldList::findNamedLink(/*1Ab*/const char * name, /*1Ab*/bool warn)
{
   return OldList_findNamedLink(&impl, /*5Ge*/name, /*5Ee*/(C(bool))warn);
}
inline void OldList::free(void (* freeFn)(void *))
{
   OldList_free(&impl, /*5Ge*/freeFn);
}
inline C(bool) OldList::insert(/*1Ab*/void * prevItem, /*1Ab*/void * item)
{
   return OldList_insert(&impl, /*5Ge*/prevItem, /*5Ge*/item);
}
inline void OldList::move(/*1Ab*/void * item, /*1Ab*/void * prevItem)
{
   OldList_move(&impl, /*5Ge*/item, /*5Ge*/prevItem);
}
inline C(bool) OldList::placeName(/*1Ab*/const char * name, /*1Ab*/void ** place)
{
   return OldList_placeName(&impl, /*5Ge*/name, /*5Ge*/place);
}
inline void OldList::remove(/*1Ab*/void * item)
{
   OldList_remove(&impl, /*5Ge*/item);
}
inline void OldList::removeAll(void (* freeFn)(void *))
{
   OldList_removeAll(&impl, /*5Ge*/freeFn);
}
inline void OldList::sort(int (* compare)(void *, void *, void *), /*1Ab*/void * data)
{
   OldList_sort(&impl, /*5Ge*/compare, /*5Ge*/data);
}
inline void OldList::swap(/*1Ab*/void * item1, /*1Ab*/void * item2)
{
   OldList_swap(&impl, /*5Ge*/item1, /*5Ge*/item2);
}


// member accessors: OldList::first
// (struct::void *)

/*nstSet*/inline /*0H*/void * OldList::first_Prop::operator= (/*0H*/void * v)
{
   SELF(OldList, first);
   self->impl.first = v;
   return v;
}

/*regSet*/inline OldList::first_Prop & OldList::first_Prop::operator= (OldList::first_Prop & prop)
{
   SELF(OldList, first);
   /*0H*/void * v = prop;
   self->impl.first = v;
   return prop;
}
/*regGet*/inline OldList::first_Prop::operator /*0I*/void * () const
{
   SELF(OldList, first);
   return self ? ((C(OldList) *)&self->impl)->first : null;
}

// member accessors: OldList::last
// (struct::void *)

/*nstSet*/inline /*0H*/void * OldList::last_Prop::operator= (/*0H*/void * v)
{
   SELF(OldList, last);
   self->impl.last = v;
   return v;
}

/*regSet*/inline OldList::last_Prop & OldList::last_Prop::operator= (OldList::last_Prop & prop)
{
   SELF(OldList, last);
   /*0H*/void * v = prop;
   self->impl.last = v;
   return prop;
}
/*regGet*/inline OldList::last_Prop::operator /*0I*/void * () const
{
   SELF(OldList, last);
   return self ? ((C(OldList) *)&self->impl)->last : null;
}

// member accessors: OldList::count
// (struct::int)

/*nstSet*/inline /*0H*/int OldList::count_Prop::operator= (/*0H*/int v)
{
   SELF(OldList, count);
   self->impl.count = v;
   return v;
}

/*regSet*/inline OldList::count_Prop & OldList::count_Prop::operator= (OldList::count_Prop & prop)
{
   SELF(OldList, count);
   /*0H*/int v = prop;
   self->impl.count = v;
   return prop;
}
/*regGet*/inline OldList::count_Prop::operator /*0I*/int () const
{
   SELF(OldList, count);
   return self ? ((C(OldList) *)&self->impl)->count : 0;
}

// member accessors: OldList::offset
// (struct::uint)

/*nstSet*/inline /*0H*/uint OldList::offset_Prop::operator= (/*0H*/uint v)
{
   SELF(OldList, offset);
   self->impl.offset = v;
   return v;
}

/*regSet*/inline OldList::offset_Prop & OldList::offset_Prop::operator= (OldList::offset_Prop & prop)
{
   SELF(OldList, offset);
   /*0H*/uint v = prop;
   self->impl.offset = v;
   return prop;
}
/*regGet*/inline OldList::offset_Prop::operator /*0I*/uint () const
{
   SELF(OldList, offset);
   return self ? ((C(OldList) *)&self->impl)->offset : 0;
}

// member accessors: OldList::circ
// (struct::enum|uint)

/*nstSet*/inline /*0H*/bool OldList::circ_Prop::operator= (/*0H*/bool v)
{
   SELF(OldList, circ);
   self->impl.circ = (C(bool))v;
   return v;
}

/*regSet*/inline OldList::circ_Prop & OldList::circ_Prop::operator= (OldList::circ_Prop & prop)
{
   SELF(OldList, circ);
   /*0H*/bool v = prop;
   self->impl.circ = (C(bool))v;
   return prop;
}
/*regGet*/inline OldList::circ_Prop::operator /*0I*/bool () const
{
   SELF(OldList, circ);
   return self ? (bool)((C(OldList) *)&self->impl)->circ : (bool)0;
}



// member accessors: StringBTNode::key
// (nohead::string)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/char * TStringBTNode<TC, TCO>::key_Prop::operator= (/*0H*/char * v)
{
   SELF(StringBTNode, key);
   ((C(StringBTNode) *)self->impl)->key = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TStringBTNode<TC, TCO>::key_Prop & TStringBTNode<TC, TCO>::key_Prop::operator= (typename TStringBTNode<TC, TCO>::key_Prop & prop)
{
   SELF(StringBTNode, key);
   /*0H*/char * v = prop;
   ((C(StringBTNode) *)self->impl)->key = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TStringBTNode<TC, TCO>::key_Prop::operator /*0I*/char * () const
{
   __attribute__((unused)) TStringBTNode TCTCO * self = CONTAINER_OF(this, TStringBTNode TCTCO, key);
   return self ? ((C(StringBTNode) *)self->impl)->key : 0;
}

// member accessors: StringBTNode::parent
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/StringBTNode TStringBTNode<TC, TCO>::parent_Prop::operator= (/*0H*/StringBTNode & v)
{
   SELF(StringBTNode, parent);
   ((C(StringBTNode) *)self->impl)->parent = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TStringBTNode<TC, TCO>::parent_Prop & TStringBTNode<TC, TCO>::parent_Prop::operator= (typename TStringBTNode<TC, TCO>::parent_Prop & prop)
{
   SELF(StringBTNode, parent);
   /*0H*/StringBTNode v = prop;
   ((C(StringBTNode) *)self->impl)->parent = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TStringBTNode<TC, TCO>::parent_Prop::operator /*0I*/StringBTNode () const
{
   __attribute__((unused)) TStringBTNode TCTCO * self = CONTAINER_OF(this, TStringBTNode TCTCO, parent);
   StringBTNode value(((C(StringBTNode) *)self->impl)->parent);
   return value;
}

// member accessors: StringBTNode::left
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/StringBTNode TStringBTNode<TC, TCO>::left_Prop::operator= (/*0H*/StringBTNode & v)
{
   SELF(StringBTNode, left);
   ((C(StringBTNode) *)self->impl)->left = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TStringBTNode<TC, TCO>::left_Prop & TStringBTNode<TC, TCO>::left_Prop::operator= (typename TStringBTNode<TC, TCO>::left_Prop & prop)
{
   SELF(StringBTNode, left);
   /*0H*/StringBTNode v = prop;
   ((C(StringBTNode) *)self->impl)->left = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TStringBTNode<TC, TCO>::left_Prop::operator /*0I*/StringBTNode () const
{
   __attribute__((unused)) TStringBTNode TCTCO * self = CONTAINER_OF(this, TStringBTNode TCTCO, left);
   StringBTNode value(((C(StringBTNode) *)self->impl)->left);
   return value;
}

// member accessors: StringBTNode::right
// (nohead::nohead)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/StringBTNode TStringBTNode<TC, TCO>::right_Prop::operator= (/*0H*/StringBTNode & v)
{
   SELF(StringBTNode, right);
   ((C(StringBTNode) *)self->impl)->right = v.impl;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TStringBTNode<TC, TCO>::right_Prop & TStringBTNode<TC, TCO>::right_Prop::operator= (typename TStringBTNode<TC, TCO>::right_Prop & prop)
{
   SELF(StringBTNode, right);
   /*0H*/StringBTNode v = prop;
   ((C(StringBTNode) *)self->impl)->right = v.impl;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TStringBTNode<TC, TCO>::right_Prop::operator /*0I*/StringBTNode () const
{
   __attribute__((unused)) TStringBTNode TCTCO * self = CONTAINER_OF(this, TStringBTNode TCTCO, right);
   StringBTNode value(((C(StringBTNode) *)self->impl)->right);
   return value;
}

// member accessors: StringBTNode::depth
// (nohead::int)

/*nstSet*/template <class TC, C(Class) ** TCO> inline /*0H*/int TStringBTNode<TC, TCO>::depth_Prop::operator= (/*0H*/int v)
{
   SELF(StringBTNode, depth);
   ((C(StringBTNode) *)self->impl)->depth = v;
   return v;
}

/*regSet*/template <class TC, C(Class) ** TCO> inline typename TStringBTNode<TC, TCO>::depth_Prop & TStringBTNode<TC, TCO>::depth_Prop::operator= (typename TStringBTNode<TC, TCO>::depth_Prop & prop)
{
   SELF(StringBTNode, depth);
   /*0H*/int v = prop;
   ((C(StringBTNode) *)self->impl)->depth = v;
   return prop;
}
/*regGet*/template <class TC, C(Class) ** TCO> inline TStringBTNode<TC, TCO>::depth_Prop::operator /*0I*/int () const
{
   __attribute__((unused)) TStringBTNode TCTCO * self = CONTAINER_OF(this, TStringBTNode TCTCO, depth);
   return self ? ((C(StringBTNode) *)self->impl)->depth : 0;
}


inline C(bool) StringBinaryTree::add(/*1Ab*/BTNode node)
{
   return BinaryTree_add((C(BinaryTree)*)&impl, /*5De*/node.impl);
}
inline C(bool) StringBinaryTree::check()
{
   return BinaryTree_check((C(BinaryTree)*)&impl);
}
inline int StringBinaryTree::compareInt(/*1Ab*/uintptr a, /*1Ab*/uintptr b)
{
   return BinaryTree_compareInt((C(BinaryTree)*)&impl, /*5Ge*/a, /*5Ge*/b);
}
inline int StringBinaryTree::compareString(/*1Ab*/const char * a, /*1Ab*/const char * b)
{
   return BinaryTree_compareString((C(BinaryTree)*)&impl, /*5Ge*/a, /*5Ge*/b);
}
inline void StringBinaryTree::_delete(/*1Ab*/BTNode node)
{
   BinaryTree_delete((C(BinaryTree)*)&impl, /*5De*/node.impl);
}
inline C(BTNode) * StringBinaryTree::find(/*1Ab*/uintptr key)
{
   return BinaryTree_find((C(BinaryTree)*)&impl, /*5Ge*/key);
}
inline C(BTNode) * StringBinaryTree::findAll(/*1Ab*/uintptr key)
{
   return BinaryTree_findAll((C(BinaryTree)*)&impl, /*5Ge*/key);
}
inline C(BTNode) * StringBinaryTree::findPrefix(/*1Ab*/const char * key)
{
   return BinaryTree_findPrefix((C(BinaryTree)*)&impl, /*5Ge*/key);
}
inline C(BTNode) * StringBinaryTree::findString(/*1Ab*/const char * key)
{
   return BinaryTree_findString((C(BinaryTree)*)&impl, /*5Ge*/key);
}
inline void StringBinaryTree::free()
{
   BinaryTree_free((C(BinaryTree)*)&impl);
}
inline void StringBinaryTree::freeString(/*1Ab*/char * string)
{
   BinaryTree_freeString(/*5Ge*/string);
}
inline char * StringBinaryTree::print(/*1Ab*/char * output, /*1Ab*/TreePrintStyle tps)
{
   return BinaryTree_print((C(BinaryTree)*)&impl, /*5Ge*/output, /*5Ee*/(C(TreePrintStyle))tps);
}
inline void StringBinaryTree::remove(/*1Ab*/BTNode node)
{
   BinaryTree_remove((C(BinaryTree)*)&impl, /*5De*/node.impl);
}


// property callers: StringBinaryTree::first   // get only
// (struct::nohead)

/*regGet*/inline StringBinaryTree::first_Prop::operator /*0I*/BTNode () const
{
   SELF(StringBinaryTree, first);
   BTNode value(BinaryTree_get_first((C(BinaryTree)*)&self->impl));
   return value;
}

// property callers: StringBinaryTree::last   // get only
// (struct::nohead)

/*regGet*/inline StringBinaryTree::last_Prop::operator /*0I*/BTNode () const
{
   SELF(StringBinaryTree, last);
   BTNode value(BinaryTree_get_last((C(BinaryTree)*)&self->impl));
   return value;
}


// member accessors: StringBinaryTree::root
// (struct::nohead)

/*nstSet*/inline /*0H*/BTNode StringBinaryTree::root_Prop::operator= (/*0H*/BTNode & v)
{
   SELF(StringBinaryTree, root);
   self->impl.root = v.impl;
   return v;
}

/*regSet*/inline StringBinaryTree::root_Prop & StringBinaryTree::root_Prop::operator= (StringBinaryTree::root_Prop & prop)
{
   SELF(StringBinaryTree, root);
   /*0H*/BTNode v = prop;
   self->impl.root = v.impl;
   return prop;
}
/*regGet*/inline StringBinaryTree::root_Prop::operator /*0I*/BTNode () const
{
   SELF(StringBinaryTree, root);
   BTNode value(((C(StringBinaryTree) *)&self->impl)->root);
   return value;
}

// member accessors: StringBinaryTree::count
// (struct::int)

/*nstSet*/inline /*0H*/int StringBinaryTree::count_Prop::operator= (/*0H*/int v)
{
   SELF(StringBinaryTree, count);
   self->impl.count = v;
   return v;
}

/*regSet*/inline StringBinaryTree::count_Prop & StringBinaryTree::count_Prop::operator= (StringBinaryTree::count_Prop & prop)
{
   SELF(StringBinaryTree, count);
   /*0H*/int v = prop;
   self->impl.count = v;
   return prop;
}
/*regGet*/inline StringBinaryTree::count_Prop::operator /*0I*/int () const
{
   SELF(StringBinaryTree, count);
   return self ? ((C(StringBinaryTree) *)&self->impl)->count : 0;
}
// A (normal:string)
inline ZString::operator constString() const { return (constString)ZString_to_String(impl); }
inline ZString::ZString(const char * s) : ZString() { impl = ZString_from_String((const C(String))s); }
inline ZString & ZString::operator =(constString s) { ZString_from_String((const C(String))s); return *this; }

inline ZString::ZString(char * _string, int len = 0, StringAllocType allocType = (StringAllocType)0, int size = 0, int minSize = 0, int maxSize = 0, const char * string = null) : ZString()
{
   this->_string = _string;
   this->len = len;
   this->allocType = allocType;
   this->size = size;
   this->minSize = minSize;
   this->maxSize = maxSize;
   this->string = string;
}


inline void ZString::concat(/*1Ab*/ZString & s)
{
   ZString_concat(impl, /*5De*/s.impl);
}
template<typename... Args> inline void ZString::concatf(/*1Ab*/const char * format, /*1Ab*/Args... args)
{
   ZString_concatf(impl, /*5Ge*/format, /*5Ae*/args...);
}
inline void ZString::concatn(/*1Ab*/ZString & s, /*1Ab*/int l)
{
   ZString_concatn(impl, /*5De*/s.impl, /*5Ge*/l);
}
template <typename ...Ts> inline void ZString::concatx(/*1Ab*/const Ts&... ts)
{
   std::apply(
      [this](/*1Ab*/auto ...args) { ZString_concatx(impl, /*5Ae*/args...); },
      std::tuple_cat(std::make_tuple(classof(ts), vapass(ts))..., std::make_tuple(null))
   );
}
inline void ZString::copy(/*1Ab*/ZString & s)
{
   ZString_copy(impl, /*5De*/s.impl);
}
inline void ZString::copyString(/*1Ab*/const char * value, /*1Ab*/int newLen)
{
   ZString_copyString(impl, /*5Ge*/value, /*5Ge*/newLen);
}


// property callers: ZString::string
// (normal::const char *)

/*nstSet*/inline /*0H*/const char * ZString::string_Prop::operator= (/*0H*/const char * v)
{
   SELF(ZString, string);
   ZString_set_string(self ? self->impl : null, v);
   return v;
}

/*regSet*/inline ZString::string_Prop & ZString::string_Prop::operator= (ZString::string_Prop & prop)
{
   SELF(ZString, string);
   /*0H*/const char * v = prop;
   ZString_set_string(self ? self->impl : null, v);
   return prop;
}
/*regGet*/inline ZString::string_Prop::operator /*0I*/const char * () const
{
   SELF(ZString, string);
   return ZString_get_string(self ? self->impl : null);
}


// member accessors: ZString::_string
// (normal::char *)

/*nstSet*/inline /*0H*/char * ZString::_string_Prop::operator= (/*0H*/char * v)
{
   SELF(ZString, _string);
   IPTR(self->impl, ZString)->_string = v;
   return v;
}

/*regSet*/inline ZString::_string_Prop & ZString::_string_Prop::operator= (ZString::_string_Prop & prop)
{
   SELF(ZString, _string);
   /*0H*/char * v = prop;
   IPTR(self->impl, ZString)->_string = v;
   return prop;
}
/*regGet*/inline ZString::_string_Prop::operator /*0I*/char * () const
{
   SELF(ZString, _string);
   return self ? IPTR(self->impl, ZString)->_string : null;
}

// member accessors: ZString::len
// (normal::int)

/*nstSet*/inline /*0H*/int ZString::len_Prop::operator= (/*0H*/int v)
{
   SELF(ZString, len);
   IPTR(self->impl, ZString)->len = v;
   return v;
}

/*regSet*/inline ZString::len_Prop & ZString::len_Prop::operator= (ZString::len_Prop & prop)
{
   SELF(ZString, len);
   /*0H*/int v = prop;
   IPTR(self->impl, ZString)->len = v;
   return prop;
}
/*regGet*/inline ZString::len_Prop::operator /*0I*/int () const
{
   SELF(ZString, len);
   return self ? IPTR(self->impl, ZString)->len : 0;
}

// member accessors: ZString::allocType
// (normal::enum|int)

/*nstSet*/inline /*0H*/StringAllocType ZString::allocType_Prop::operator= (/*0H*/StringAllocType v)
{
   SELF(ZString, allocType);
   IPTR(self->impl, ZString)->allocType = (C(StringAllocType))v;
   return v;
}

/*regSet*/inline ZString::allocType_Prop & ZString::allocType_Prop::operator= (ZString::allocType_Prop & prop)
{
   SELF(ZString, allocType);
   /*0H*/StringAllocType v = prop;
   IPTR(self->impl, ZString)->allocType = (C(StringAllocType))v;
   return prop;
}
/*regGet*/inline ZString::allocType_Prop::operator /*0I*/StringAllocType () const
{
   SELF(ZString, allocType);
   return self ? (StringAllocType)IPTR(self->impl, ZString)->allocType : (StringAllocType)0;
}

// member accessors: ZString::size
// (normal::int)

/*nstSet*/inline /*0H*/int ZString::size_Prop::operator= (/*0H*/int v)
{
   SELF(ZString, size);
   IPTR(self->impl, ZString)->size = v;
   return v;
}

/*regSet*/inline ZString::size_Prop & ZString::size_Prop::operator= (ZString::size_Prop & prop)
{
   SELF(ZString, size);
   /*0H*/int v = prop;
   IPTR(self->impl, ZString)->size = v;
   return prop;
}
/*regGet*/inline ZString::size_Prop::operator /*0I*/int () const
{
   SELF(ZString, size);
   return self ? IPTR(self->impl, ZString)->size : 0;
}

// member accessors: ZString::minSize
// (normal::int)

/*nstSet*/inline /*0H*/int ZString::minSize_Prop::operator= (/*0H*/int v)
{
   SELF(ZString, minSize);
   IPTR(self->impl, ZString)->minSize = v;
   return v;
}

/*regSet*/inline ZString::minSize_Prop & ZString::minSize_Prop::operator= (ZString::minSize_Prop & prop)
{
   SELF(ZString, minSize);
   /*0H*/int v = prop;
   IPTR(self->impl, ZString)->minSize = v;
   return prop;
}
/*regGet*/inline ZString::minSize_Prop::operator /*0I*/int () const
{
   SELF(ZString, minSize);
   return self ? IPTR(self->impl, ZString)->minSize : 0;
}

// member accessors: ZString::maxSize
// (normal::int)

/*nstSet*/inline /*0H*/int ZString::maxSize_Prop::operator= (/*0H*/int v)
{
   SELF(ZString, maxSize);
   IPTR(self->impl, ZString)->maxSize = v;
   return v;
}

/*regSet*/inline ZString::maxSize_Prop & ZString::maxSize_Prop::operator= (ZString::maxSize_Prop & prop)
{
   SELF(ZString, maxSize);
   /*0H*/int v = prop;
   IPTR(self->impl, ZString)->maxSize = v;
   return prop;
}
/*regGet*/inline ZString::maxSize_Prop::operator /*0I*/int () const
{
   SELF(ZString, maxSize);
   return self ? IPTR(self->impl, ZString)->maxSize : 0;
}
// function: checkConsistency
inline void checkConsistency()
{
   F(checkConsistency)();
}
// function: checkMemory
inline void checkMemory()
{
   F(checkMemory)();
}
// function: getActiveDesigner
// function: getRuntimePlatform
inline C(Platform) getRuntimePlatform()
{
   return F(getRuntimePlatform)();
}
// function: locateModule
inline C(bool) locateModule(/*1Ab*/const char * name, /*1Ab*/const char * fileName)
{
   return F(locateModule)(/*5Ge*/name, /*5Ge*/fileName);
}
// function: printx
template <typename ...Ts> inline void printx(/*1Ab*/const Ts&... ts)
{
   std::apply(
      [](/*1Ab*/auto ...args) { F(printx)(/*5Ae*/args...); },
      std::tuple_cat(std::make_tuple(classof(ts), vapass(ts))..., std::make_tuple(null))
   );
}
// function: printBuf
template <typename ...Ts> inline int printBuf(/*1Ab*/char * buffer, /*1Ab*/int maxLen, /*1Ab*/const Ts&... ts)
{
   return std::apply(
      [](/*1Ab*/char * buffer, /*1Ab*/int maxLen, /*1Ab*/auto ...args) { F(printBuf)(/*5Ge*/buffer, /*5Ge*/maxLen, /*5Ae*/args...); },
      std::tuple_cat(std::make_tuple(classof(ts), vapass(ts))..., std::make_tuple(null))
   );
}
// function: printLn
template <typename ...Ts> inline void printLn(/*1Ab*/const Ts&... ts)
{
   std::apply(
      [](/*1Ab*/auto ...args) { F(printLn)(/*5Ae*/args...); },
      std::tuple_cat(std::make_tuple(classof(ts), vapass(ts))..., std::make_tuple(null))
   );
}
// function: printLnBuf
template <typename ...Ts> inline int printLnBuf(/*1Ab*/char * buffer, /*1Ab*/int maxLen, /*1Ab*/const Ts&... ts)
{
   return std::apply(
      [](/*1Ab*/char * buffer, /*1Ab*/int maxLen, /*1Ab*/auto ...args) { F(printLnBuf)(/*5Ge*/buffer, /*5Ge*/maxLen, /*5Ae*/args...); },
      std::tuple_cat(std::make_tuple(classof(ts), vapass(ts))..., std::make_tuple(null))
   );
}
// function: printLnString
template <typename ...Ts> inline char * printLnString(/*1Ab*/const Ts&... ts)
{
   return std::apply(
      [](/*1Ab*/auto ...args) { F(printLnString)(/*5Ae*/args...); },
      std::tuple_cat(std::make_tuple(classof(ts), vapass(ts))..., std::make_tuple(null))
   );
}
// function: printStdArgsToBuffer
inline int printStdArgsToBuffer(/*1Ab*/char * buffer, /*1Ab*/int maxLen, /*1Bb*/typed_object_class_ptr class_object, void * object, /*1Ab*/__builtin_va_list args)
{
   return F(printStdArgsToBuffer)(/*5Ge*/buffer, /*5Ge*/maxLen, /*5Fe*/class_object, object, /*5Ge*/args);
}
// function: printString
template <typename ...Ts> inline char * printString(/*1Ab*/const Ts&... ts)
{
   return std::apply(
      [](/*1Ab*/auto ...args) { F(printString)(/*5Ae*/args...); },
      std::tuple_cat(std::make_tuple(classof(ts), vapass(ts))..., std::make_tuple(null))
   );
}
// function: setActiveDesigner
// function: eSystem_LockMem
inline void eSystem_LockMem()
{
   F(eSystem_LockMem)();
}
// function: eSystem_UnlockMem
inline void eSystem_UnlockMem()
{
   F(eSystem_UnlockMem)();
}
// function: log2i
inline uint log2i(/*1Ab*/uint number)
{
   return F(log2i)(/*5Ge*/number);
}
// function: memswap
inline void memswap(/*1Ab*/byte * a, /*1Ab*/byte * b, /*1Ab*/uint size)
{
   F(memswap)(/*5Ge*/a, /*5Ge*/b, /*5Ge*/size);
}
// function: pow2i
inline uint pow2i(/*1Ab*/uint number)
{
   return F(pow2i)(/*5Ge*/number);
}
// function: qsortr
inline void qsortr(/*1Ab*/void * base, /*1Ab*/uintsize nel, /*1Ab*/uintsize width, int (* compare)(void * arg, const void * a, const void * b), /*1Ab*/void * arg)
{
   F(qsortr)(/*5Ge*/base, /*5Ge*/nel, /*5Ge*/width, /*5Ge*/compare, /*5Ge*/arg);
}
// function: qsortrx
inline void qsortrx(/*1Ab*/void * base, /*1Ab*/uintsize nel, /*1Ab*/uintsize width, int (* compare)(void * arg, const void * a, const void * b), int (* optCompareArgLast)(const void * a, const void * b, void * arg), /*1Ab*/void * arg, /*1Ab*/bool deref, /*1Ab*/bool ascending)
{
   F(qsortrx)(/*5Ge*/base, /*5Ge*/nel, /*5Ge*/width, /*5Ge*/compare, /*5Ge*/optCompareArgLast, /*5Ge*/arg, /*5Ee*/(C(bool))deref, /*5Ee*/(C(bool))ascending);
}
// function: queryMemInfo
inline void queryMemInfo(/*1Ab*/char * string)
{
   F(queryMemInfo)(/*5Ge*/string);
}
// function: changeCh
inline void changeCh(/*1Ab*/char * string, /*1Ab*/char ch1, /*1Ab*/char ch2)
{
   F(changeCh)(/*5Ge*/string, /*5Ge*/ch1, /*5Ge*/ch2);
}
// function: changeChars
inline void changeChars(/*1Ab*/char * string, /*1Ab*/const char * chars, /*1Ab*/char alt)
{
   F(changeChars)(/*5Ge*/string, /*5Ge*/chars, /*5Ge*/alt);
}
// function: changeExtension
inline char * changeExtension(/*1Ab*/const char * string, /*1Ab*/const char * ext, /*1Ab*/char * output)
{
   return F(changeExtension)(/*5Ge*/string, /*5Ge*/ext, /*5Ge*/output);
}
// function: copyString
inline char * copyString(/*1Ab*/const char * string)
{
   return F(copyString)(/*5Ge*/string);
}
// function: escapeCString
inline int escapeCString(C(String) outString, /*1Ab*/int bufferLen, constString s, /*1Ab*/EscapeCStringOptions options)
{
   return F(escapeCString)(/*5Ge*/outString, /*5Ge*/bufferLen, /*5Ge*/s, /*5Ee*/(C(EscapeCStringOptions))options.impl);
}
// function: floatFromString
inline double floatFromString(/*1Ab*/const char * string)
{
   return F(floatFromString)(/*5Ge*/string);
}
// function: getCurrentThreadID
inline int64 getCurrentThreadID()
{
   return F(getCurrentThreadID)();
}
// function: getExtension
inline char * getExtension(/*1Ab*/const char * string, /*1Ab*/char * output)
{
   return F(getExtension)(/*5Ge*/string, /*5Ge*/output);
}
// function: getHexValue
inline uint getHexValue(/*1Ab*/const char ** buffer)
{
   return F(getHexValue)(/*5Ge*/buffer);
}
// function: getLastDirectory
inline char * getLastDirectory(/*1Ab*/const char * string, /*1Ab*/char * output)
{
   return F(getLastDirectory)(/*5Ge*/string, /*5Ge*/output);
}
// function: getString
inline C(bool) getString(/*1Ab*/const char ** buffer, /*1Ab*/char * string, /*1Ab*/int max)
{
   return F(getString)(/*5Ge*/buffer, /*5Ge*/string, /*5Ge*/max);
}
// function: getValue
inline int getValue(/*1Ab*/const char ** buffer)
{
   return F(getValue)(/*5Ge*/buffer);
}
// function: iSO8859_1toUTF8
inline int iSO8859_1toUTF8(/*1Ab*/const char * source, /*1Ab*/char * dest, /*1Ab*/int max)
{
   return F(iSO8859_1toUTF8)(/*5Ge*/source, /*5Ge*/dest, /*5Ge*/max);
}
// function: isPathInsideOf
inline C(bool) isPathInsideOf(/*1Ab*/const char * path, /*1Ab*/const char * of)
{
   return F(isPathInsideOf)(/*5Ge*/path, /*5Ge*/of);
}
// function: makePathRelative
inline char * makePathRelative(/*1Ab*/const char * path, /*1Ab*/const char * to, /*1Ab*/char * destination)
{
   return F(makePathRelative)(/*5Ge*/path, /*5Ge*/to, /*5Ge*/destination);
}
// function: pathCat
inline char * pathCat(/*1Ab*/char * string, /*1Ab*/const char * addedPath)
{
   return F(pathCat)(/*5Ge*/string, /*5Ge*/addedPath);
}
// function: pathCatSlash
inline char * pathCatSlash(/*1Ab*/char * string, /*1Ab*/const char * addedPath)
{
   return F(pathCatSlash)(/*5Ge*/string, /*5Ge*/addedPath);
}
// function: printBigSize
inline void printBigSize(/*1Ab*/char * string, /*1Ab*/double size, /*1Ab*/int prec)
{
   F(printBigSize)(/*5Ge*/string, /*5Ge*/size, /*5Ge*/prec);
}
// function: printSize
inline void printSize(/*1Ab*/char * string, /*1Ab*/uint64 size, /*1Ab*/int prec)
{
   F(printSize)(/*5Ge*/string, /*5Ge*/size, /*5Ge*/prec);
}
// function: rSearchString
inline char * rSearchString(/*1Ab*/const char * buffer, /*1Ab*/const char * subStr, /*1Ab*/int maxLen, /*1Ab*/bool matchCase, /*1Ab*/bool matchWord)
{
   return F(rSearchString)(/*5Ge*/buffer, /*5Ge*/subStr, /*5Ge*/maxLen, /*5Ee*/(C(bool))matchCase, /*5Ee*/(C(bool))matchWord);
}
// function: repeatCh
inline void repeatCh(/*1Ab*/char * string, /*1Ab*/int count, /*1Ab*/char ch)
{
   F(repeatCh)(/*5Ge*/string, /*5Ge*/count, /*5Ge*/ch);
}
// function: searchString
inline char * searchString(/*1Ab*/const char * buffer, /*1Ab*/int start, /*1Ab*/const char * subStr, /*1Ab*/bool matchCase, /*1Ab*/bool matchWord)
{
   return F(searchString)(/*5Ge*/buffer, /*5Ge*/start, /*5Ge*/subStr, /*5Ee*/(C(bool))matchCase, /*5Ee*/(C(bool))matchWord);
}
// function: splitArchivePath
inline C(bool) splitArchivePath(/*1Ab*/const char * fileName, /*1Ab*/char * archiveName, /*1Ab*/const char ** archiveFile)
{
   return F(splitArchivePath)(/*5Ge*/fileName, /*5Ge*/archiveName, /*5Ge*/archiveFile);
}
// function: splitDirectory
inline char * splitDirectory(/*1Ab*/const char * string, /*1Ab*/char * part, /*1Ab*/char * rest)
{
   return F(splitDirectory)(/*5Ge*/string, /*5Ge*/part, /*5Ge*/rest);
}
// function: stringLikePattern
inline C(bool) stringLikePattern(constString string, constString pattern)
{
   return F(stringLikePattern)(/*5Ge*/string, /*5Ge*/pattern);
}
// function: stripChars
inline char * stripChars(C(String) string, constString chars)
{
   return F(stripChars)(/*5Ge*/string, /*5Ge*/chars);
}
// function: stripExtension
inline C(bool) stripExtension(/*1Ab*/char * string)
{
   return F(stripExtension)(/*5Ge*/string);
}
// function: stripLastDirectory
inline char * stripLastDirectory(/*1Ab*/const char * string, /*1Ab*/char * output)
{
   return F(stripLastDirectory)(/*5Ge*/string, /*5Ge*/output);
}
// function: stripQuotes
inline char * stripQuotes(/*1Ab*/const char * string, /*1Ab*/char * output)
{
   return F(stripQuotes)(/*5Ge*/string, /*5Ge*/output);
}
// function: tokenize
inline int tokenize(/*1Ab*/char * string, /*1Ab*/int maxTokens, /*1Ab*/char * tokens[], /*1Ab*/BackSlashEscaping esc)
{
   return F(tokenize)(/*5Ge*/string, /*5Ge*/maxTokens, /*5Ge*/tokens, /*5Ee*/(C(BackSlashEscaping))esc);
}
// function: tokenizeWith
inline int tokenizeWith(/*1Ab*/char * string, /*1Ab*/int maxTokens, /*1Ab*/char * tokens[], /*1Ab*/const char * tokenizers, /*1Ab*/bool escapeBackSlashes)
{
   return F(tokenizeWith)(/*5Ge*/string, /*5Ge*/maxTokens, /*5Ge*/tokens, /*5Ge*/tokenizers, /*5Ee*/(C(bool))escapeBackSlashes);
}
// function: trimLSpaces
inline char * trimLSpaces(/*1Ab*/const char * string, /*1Ab*/char * output)
{
   return F(trimLSpaces)(/*5Ge*/string, /*5Ge*/output);
}
// function: trimRSpaces
inline char * trimRSpaces(/*1Ab*/const char * string, /*1Ab*/char * output)
{
   return F(trimRSpaces)(/*5Ge*/string, /*5Ge*/output);
}
// function: uTF16toUTF8
inline char * uTF16toUTF8(/*1Ab*/const uint16 * source)
{
   return F(uTF16toUTF8)(/*5Ge*/source);
}
// function: uTF16toUTF8Buffer
inline int uTF16toUTF8Buffer(/*1Ab*/const uint16 * source, /*1Ab*/char * dest, /*1Ab*/int max)
{
   return F(uTF16toUTF8Buffer)(/*5Ge*/source, /*5Ge*/dest, /*5Ge*/max);
}
// function: uTF32toUTF8Len
inline int uTF32toUTF8Len(/*1Ab*/const unichar * source, /*1Ab*/int count, /*1Ab*/char * dest, /*1Ab*/int max)
{
   return F(uTF32toUTF8Len)(/*5Ge*/source, /*5Ge*/count, /*5Ge*/dest, /*5Ge*/max);
}
// function: uTF8GetChar
inline unichar uTF8GetChar(/*1Ab*/const char * string, /*1Ab*/int * numBytes)
{
   return F(uTF8GetChar)(/*5Ge*/string, /*5Ge*/numBytes);
}
// function: uTF8Validate
inline C(bool) uTF8Validate(/*1Ab*/const char * source)
{
   return F(uTF8Validate)(/*5Ge*/source);
}
// function: uTF8toUTF16
inline uint16 * uTF8toUTF16(/*1Ab*/const char * source, /*1Ab*/int * wordCount)
{
   return F(uTF8toUTF16)(/*5Ge*/source, /*5Ge*/wordCount);
}
// function: uTF8toUTF16Buffer
inline int uTF8toUTF16Buffer(/*1Ab*/const char * source, /*1Ab*/uint16 * dest, /*1Ab*/int max)
{
   return F(uTF8toUTF16Buffer)(/*5Ge*/source, /*5Ge*/dest, /*5Ge*/max);
}
// function: unescapeCString
inline int unescapeCString(/*1Ab*/char * d, /*1Ab*/const char * s, /*1Ab*/int len)
{
   return F(unescapeCString)(/*5Ge*/d, /*5Ge*/s, /*5Ge*/len);
}
// function: unescapeCStringLoose
inline int unescapeCStringLoose(/*1Ab*/char * d, /*1Ab*/const char * s, /*1Ab*/int len)
{
   return F(unescapeCStringLoose)(/*5Ge*/d, /*5Ge*/s, /*5Ge*/len);
}
// function: ishexdigit
inline C(bool) ishexdigit(/*1Ab*/char x)
{
   return F(ishexdigit)(/*5Ge*/x);
}
// function: strchrmax
inline char * strchrmax(/*1Ab*/const char * s, /*1Ab*/int c, /*1Ab*/int max)
{
   return F(strchrmax)(/*5Ge*/s, /*5Ge*/c, /*5Ge*/max);
}


#endif // !defined(__EC_HPP__)
