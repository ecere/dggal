#include "eC.hpp"

TCPPClass<AVLTree> AVLTree::_cpp_class;
TCPPClass<Application> Application::_cpp_class;
TCPPClass<Array> Array::_cpp_class;
TCPPClass<Container> Container::_cpp_class;
TCPPClass<CustomAVLTree> CustomAVLTree::_cpp_class;
TCPPClass<HashMap> HashMap::_cpp_class;
TCPPClass<HashTable> HashTable::_cpp_class;
TCPPClass<IOChannel> IOChannel::_cpp_class;
TCPPClass<Instance> Instance::_cpp_class;
TCPPClass<LinkList> LinkList::_cpp_class;
TCPPClass<List> List::_cpp_class;
TCPPClass<Map> Map::_cpp_class;
TCPPClass<Module> Module::_cpp_class;
TCPPClass<SerialBuffer> SerialBuffer::_cpp_class;
TCPPClass<ZString> ZString::_cpp_class;

int eC_cpp_init(const Module & module)
{
   if(!ZString::_cpp_class.impl)
   {
#ifdef _DEBUG
      // printf("%s_cpp_init\n", "eC");
#endif

   TStruct<BuiltInContainer>::_class = CO(BuiltInContainer);
   TStruct<ClassTemplateArgument>::_class = CO(ClassTemplateArgument);
   TStruct<DataValue>::_class = CO(DataValue);
   TStruct<HashMapIterator>::_class = CO(HashMapIterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<LinkElement>::_class = CO(LinkElement);
   TStruct<LinkElement>::_class = CO(LinkElement);
   TStruct<MapIterator>::_class = CO(MapIterator);
   TStruct<NameSpace>::_class = CO(NameSpace);
   TStruct<StaticString>::_class = CO(StaticString);
   TStruct<BinaryTree>::_class = CO(BinaryTree);
   TStruct<OldList>::_class = CO(OldList);
   TStruct<StringBinaryTree>::_class = CO(StringBinaryTree);

   AVLTree::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "AVLTree", "AVLTree",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   Application::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Application", "Application",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) Application::constructor,
               (void(*)(void *)) Application::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   Array::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Array", "Array",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   Container::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Container", "Container",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   CustomAVLTree::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "CustomAVLTree", "CustomAVLTree",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   HashMap::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "HashMap", "HashMap",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   HashTable::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "HashTable", "HashTable",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   IOChannel::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "IOChannel", "IOChannel",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) IOChannel::constructor,
               (void(*)(void *)) IOChannel::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   Instance::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Instance", "Instance",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) Instance::constructor,
               (void(*)(void *)) Instance::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   LinkList::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "LinkList", "LinkList",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   List::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "List", "List",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   Map::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Map", "Map",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   Module::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Module", "Module",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) Module::constructor,
               (void(*)(void *)) Module::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   SerialBuffer::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "SerialBuffer", "SerialBuffer",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) SerialBuffer::constructor,
               (void(*)(void *)) SerialBuffer::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   ZString::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "ZString", "ZString",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) ZString::constructor,
               (void(*)(void *)) ZString::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   }
   return 0;
}

// Instance methods depending on libecere
void Instance::class_registration(CPPClass & _cpp_class)

{

      addMethod(_cpp_class.impl, "OnCompare", (void *) +[](XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) object)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_) ? (Instance *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Instance, onCompare);
         Instance_onCompare_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onCompare_Functor::FunctionType) i->vTbl[vid];
            /*2Ag*/TIH<Instance> object_l(object); int ret = fn(*i, /*3Ad*/*object_l); return ret;
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onCompare_Functor::FunctionType) cppcl->vTbl[vid];
/*2Ag*/TIH<Instance> object_l(object); int ret = fn(*i, /*3Ad*/*object_l); return ret;
         }
         else
         {
            auto method = ((int (*) (XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) object))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onCompare)]);
            if(method) return method (_class, o_, object);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "OnCopy", (void *) +[](XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(Instance) newData)
      {
         XClass * cl = (o_ ? *o_ : null) ? (XClass *)(o_ ? *o_ : null)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_ ? *o_ : null) ? (Instance *)INSTANCEL(o_ ? *o_ : null, cl) : null;
         int vid = M_VTBLID(Instance, onCopy);
         Instance_onCopy_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onCopy_Functor::FunctionType) i->vTbl[vid];
            /*2Ag*/TIH<Instance> newData_l(newData); fn(*i, /*3Ad*/*newData_l);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onCopy_Functor::FunctionType) cppcl->vTbl[vid];
/*2Ag*/TIH<Instance> newData_l(newData); fn(*i, /*3Ad*/*newData_l);
         }
         else
         {
            auto method = ((void (*) (XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(Instance) newData))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onCopy)]);
            if(method) return method (_class, o_, newData);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "OnDisplay", (void *) +[](XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) surface, /*1Aa*/int x, /*1Aa*/int y, /*1Aa*/int width, /*1Aa*/void * fieldData, /*1Aa*/C(Alignment) alignment, /*1Aa*/C(DataDisplayFlags) displayFlags)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_) ? (Instance *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Instance, onDisplay);
         Instance_onDisplay_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onDisplay_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<Instance> surface_l(surface); fn(*i, /*3Bd*/*surface_l, /*3Kd*/x, /*3Kd*/y, /*3Kd*/width, /*3Kd*/fieldData, /*3Hd*/(Alignment)alignment, /*3Kd*/displayFlags);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onDisplay_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<Instance> surface_l(surface); fn(*i, /*3Bd*/*surface_l, /*3Kd*/x, /*3Kd*/y, /*3Kd*/width, /*3Kd*/fieldData, /*3Hd*/(Alignment)alignment, /*3Kd*/displayFlags);
         }
         else
         {
            auto method = ((void (*) (XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) surface, /*1Aa*/int x, /*1Aa*/int y, /*1Aa*/int width, /*1Aa*/void * fieldData, /*1Aa*/C(Alignment) alignment, /*1Aa*/C(DataDisplayFlags) displayFlags))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onDisplay)]);
            if(method) return method (_class, o_, surface, x, y, width, fieldData, alignment, displayFlags);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "OnEdit", (void *) +[](XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) dataBox, /*1Aa*/C(Instance) obsolete, /*1Aa*/int x, /*1Aa*/int y, /*1Aa*/int w, /*1Aa*/int h, /*1Aa*/void * userData)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_) ? (Instance *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Instance, onEdit);
         Instance_onEdit_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onEdit_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<Instance> dataBox_l(dataBox); /*2Bg*/TIH<Instance> obsolete_l(obsolete); Instance * ret =  &fn(*i, /*3Bd*/*dataBox_l, /*3Bd*/*obsolete_l, /*3Kd*/x, /*3Kd*/y, /*3Kd*/w, /*3Kd*/h, /*3Kd*/userData); return ret ? ret->impl : null;
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onEdit_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<Instance> dataBox_l(dataBox); /*2Bg*/TIH<Instance> obsolete_l(obsolete); Instance * ret =  &fn(*i, /*3Bd*/*dataBox_l, /*3Bd*/*obsolete_l, /*3Kd*/x, /*3Kd*/y, /*3Kd*/w, /*3Kd*/h, /*3Kd*/userData); return ret ? ret->impl : null;
         }
         else
         {
            auto method = ((C(Instance) (*) (XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) dataBox, /*1Aa*/C(Instance) obsolete, /*1Aa*/int x, /*1Aa*/int y, /*1Aa*/int w, /*1Aa*/int h, /*1Aa*/void * userData))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onEdit)]);
            if(method) return method (_class, o_, dataBox, obsolete, x, y, w, h, userData);
         }
         return (C(Instance))null;
      });


      addMethod(_cpp_class.impl, "OnFree", (void *) +[](XClass * _class, /*1Aa*/C(Instance) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_) ? (Instance *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Instance, onFree);
         Instance_onFree_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onFree_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onFree_Functor::FunctionType) cppcl->vTbl[vid];
fn(*i);
         }
         else
         {
            auto method = ((void (*) (XClass * _class, /*1Aa*/C(Instance) o_))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onFree)]);
            if(method) return method (_class, o_);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "OnGetDataFromString", (void *) +[](XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/const char * string)
      {
         XClass * cl = (o_ ? *o_ : null) ? (XClass *)(o_ ? *o_ : null)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_ ? *o_ : null) ? (Instance *)INSTANCEL(o_ ? *o_ : null, cl) : null;
         int vid = M_VTBLID(Instance, onGetDataFromString);
         Instance_onGetDataFromString_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onGetDataFromString_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/string); return ret;
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onGetDataFromString_Functor::FunctionType) cppcl->vTbl[vid];
C(bool) ret = (C(bool))fn(*i, /*3Kd*/string); return ret;
         }
         else
         {
            auto method = ((C(bool) (*) (XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/const char * string))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onGetDataFromString)]);
            if(method) return method (_class, o_, string);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "OnGetString", (void *) +[](XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/char * tempString, /*1Aa*/void * reserved, /*1Aa*/C(ObjectNotationType) * onType)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_) ? (Instance *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Instance, onGetString);
         Instance_onGetString_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onGetString_Functor::FunctionType) i->vTbl[vid];
            const char * ret = fn(*i, /*3Kd*/tempString, /*3Kd*/reserved, /*3Hd*/(ObjectNotationType *)onType); return ret;
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onGetString_Functor::FunctionType) cppcl->vTbl[vid];
const char * ret = fn(*i, /*3Kd*/tempString, /*3Kd*/reserved, /*3Hd*/(ObjectNotationType *)onType); return ret;
         }
         else
         {
            auto method = ((const char * (*) (XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/char * tempString, /*1Aa*/void * reserved, /*1Aa*/C(ObjectNotationType) * onType))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onGetString)]);
            if(method) return method (_class, o_, tempString, reserved, onType);
         }
         return (const char *)null;
      });


      addMethod(_cpp_class.impl, "OnSaveEdit", (void *) +[](XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(Window) window, /*1Aa*/void * object)
      {
         XClass * cl = (o_ ? *o_ : null) ? (XClass *)(o_ ? *o_ : null)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_ ? *o_ : null) ? (Instance *)INSTANCEL(o_ ? *o_ : null, cl) : null;
         int vid = M_VTBLID(Instance, onSaveEdit);
         Instance_onSaveEdit_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onSaveEdit_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<Instance> window_l(window); C(bool) ret = (C(bool))fn(*i, /*3Bd*/*window_l, /*3Kd*/object); return ret;
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onSaveEdit_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<Instance> window_l(window); C(bool) ret = (C(bool))fn(*i, /*3Bd*/*window_l, /*3Kd*/object); return ret;
         }
         else
         {
            auto method = ((C(bool) (*) (XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(Window) window, /*1Aa*/void * object))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onSaveEdit)]);
            if(method) return method (_class, o_, window, object);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "OnSerialize", (void *) +[](XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(IOChannel) channel)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_) ? (Instance *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Instance, onSerialize);
         Instance_onSerialize_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onSerialize_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<Instance> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onSerialize_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<Instance> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
         }
         else
         {
            auto method = ((void (*) (XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(IOChannel) channel))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onSerialize)]);
            if(method) return method (_class, o_, channel);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "OnUnserialize", (void *) +[](XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(IOChannel) channel)
      {
         XClass * cl = (o_ ? *o_ : null) ? (XClass *)(o_ ? *o_ : null)->_class : null;
         // 'cp1' is present
         CPPClass * cppcl = _class ? (CPPClass *)_class->bindingsClass : null;
         Instance * i = (o_ ? *o_ : null) ? (Instance *)INSTANCEL(o_ ? *o_ : null, cl) : null;
         int vid = M_VTBLID(Instance, onUnserialize);
         Instance_onUnserialize_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Instance_onUnserialize_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<Instance> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onUnserialize_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<Instance> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
         }
         else
         {
            auto method = ((void (*) (XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(IOChannel) channel))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onUnserialize)]);
            if(method) return method (_class, o_, channel);
         }
         return ;
      });


}


//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
////                                                                        //// ////////////////////////
////    moved to cpp implementations                                        //// ////////////////////////
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

void AVLTree::class_registration(CPPClass & _cpp_class)
{
}
void Application::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "Main", (void *) +[](/*1Aa*/C(Application) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Application * i = (o_) ? (Application *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Application, main);
         Application_main_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Application_main_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Application) o_))(CO(Application)->_vTbl)[M_VTBLID(Application, main)]);
            if(method) return method (o_);
         }
         return ;
      });


}
void Array::class_registration(CPPClass & _cpp_class)
{
}
void Container::class_registration(CPPClass & _cpp_class)
{
}
void CustomAVLTree::class_registration(CPPClass & _cpp_class)
{
}
void HashMap::class_registration(CPPClass & _cpp_class)
{
}
void HashTable::class_registration(CPPClass & _cpp_class)
{
}
void IOChannel::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "ReadData", (void *) +[](/*1Aa*/C(IOChannel) o_, /*1Aa*/void * data, /*1Aa*/uintsize numBytes)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         IOChannel * i = (o_) ? (IOChannel *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(IOChannel, readData);
         IOChannel_readData_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (IOChannel_readData_Functor::FunctionType) i->vTbl[vid];
            uintsize ret = fn(*i, /*3Kd*/data, /*3Kd*/numBytes); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uintsize (*) (/*1Aa*/C(IOChannel) o_, /*1Aa*/void * data, /*1Aa*/uintsize numBytes))(CO(IOChannel)->_vTbl)[M_VTBLID(IOChannel, readData)]);
            if(method) return method (o_, data, numBytes);
         }
         return (uintsize)1;
      });


      addMethod(_cpp_class.impl, "WriteData", (void *) +[](/*1Aa*/C(IOChannel) o_, /*1Aa*/const void * data, /*1Aa*/uintsize numBytes)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         IOChannel * i = (o_) ? (IOChannel *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(IOChannel, writeData);
         IOChannel_writeData_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (IOChannel_writeData_Functor::FunctionType) i->vTbl[vid];
            uintsize ret = fn(*i, /*3Kd*/data, /*3Kd*/numBytes); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uintsize (*) (/*1Aa*/C(IOChannel) o_, /*1Aa*/const void * data, /*1Aa*/uintsize numBytes))(CO(IOChannel)->_vTbl)[M_VTBLID(IOChannel, writeData)]);
            if(method) return method (o_, data, numBytes);
         }
         return (uintsize)1;
      });


}
void LinkList::class_registration(CPPClass & _cpp_class)
{
}
void List::class_registration(CPPClass & _cpp_class)
{
}
void Map::class_registration(CPPClass & _cpp_class)
{
}
void SerialBuffer::class_registration(CPPClass & _cpp_class)
{
}

///////////////////////////////////////////////// [ecereCOM]/ecere::gui //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


///////////////////////////////////////////////// [ecereCOM]/ecere::sys //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

   EscapeCStringOptions::EscapeCStringOptions(bool escapeSingleQuote, bool escapeDoubleQuotes, bool writeQuotes, bool multiLine, int indent)
   {
      impl = ESCAPECSTRINGOPTIONS(escapeSingleQuote, escapeDoubleQuotes, writeQuotes, multiLine, indent);
   }
void ZString::class_registration(CPPClass & _cpp_class)
{
}
