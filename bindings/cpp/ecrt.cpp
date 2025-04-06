#include "ecrt.hpp"

TCPPClass<Application> Application::_cpp_class;
TCPPClass<Instance> Instance::_cpp_class;
TCPPClass<Module> Module::_cpp_class;
TCPPClass<AVLTree> AVLTree::_cpp_class;
TCPPClass<Array> Array::_cpp_class;
TCPPClass<Container> Container::_cpp_class;
TCPPClass<CustomAVLTree> CustomAVLTree::_cpp_class;
TCPPClass<HashMap> HashMap::_cpp_class;
TCPPClass<HashTable> HashTable::_cpp_class;
TCPPClass<LinkList> LinkList::_cpp_class;
TCPPClass<List> List::_cpp_class;
TCPPClass<Map> Map::_cpp_class;
TCPPClass<Archive> Archive::_cpp_class;
TCPPClass<ArchiveDir> ArchiveDir::_cpp_class;
TCPPClass<BufferedFile> BufferedFile::_cpp_class;
TCPPClass<ConsoleFile> ConsoleFile::_cpp_class;
TCPPClass<DualPipe> DualPipe::_cpp_class;
TCPPClass<File> File::_cpp_class;
TCPPClass<FileMonitor> FileMonitor::_cpp_class;
TCPPClass<TempFile> TempFile::_cpp_class;
TCPPClass<ECONGlobalSettings> ECONGlobalSettings::_cpp_class;
TCPPClass<ECONParser> ECONParser::_cpp_class;
TCPPClass<GlobalAppSettings> GlobalAppSettings::_cpp_class;
TCPPClass<GlobalSettings> GlobalSettings::_cpp_class;
TCPPClass<GlobalSettingsData> GlobalSettingsData::_cpp_class;
TCPPClass<GlobalSettingsDriver> GlobalSettingsDriver::_cpp_class;
TCPPClass<JSONGlobalSettings> JSONGlobalSettings::_cpp_class;
TCPPClass<JSONParser> JSONParser::_cpp_class;
TCPPClass<OptionsMap> OptionsMap::_cpp_class;
TCPPClass<Thread> Thread::_cpp_class;
TCPPClass<IOChannel> IOChannel::_cpp_class;
TCPPClass<SerialBuffer> SerialBuffer::_cpp_class;
TCPPClass<ZString> ZString::_cpp_class;

int ecrt_cpp_init(const Module & module)
{
   if(!ZString::_cpp_class.impl)
   {
#ifdef _DEBUG
      // printf("%s_cpp_init\n", "ecrt");
#endif

   TStruct<FieldValue>::_class = CO(FieldValue);
   TStruct<BinaryTree>::_class = CO(BinaryTree);
   TStruct<BuiltInContainer>::_class = CO(BuiltInContainer);
   TStruct<HashMapIterator>::_class = CO(HashMapIterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<Iterator>::_class = CO(Iterator);
   TStruct<LinkElement>::_class = CO(LinkElement);
   TStruct<LinkElement>::_class = CO(LinkElement);
   TStruct<MapIterator>::_class = CO(MapIterator);
   TStruct<OldList>::_class = CO(OldList);
   TStruct<StringBinaryTree>::_class = CO(StringBinaryTree);
   TStruct<FileListing>::_class = CO(FileListing);
   TStruct<FileStats>::_class = CO(FileStats);
   TStruct<Date>::_class = CO(Date);
   TStruct<DateTime>::_class = CO(DateTime);
   TStruct<Box>::_class = CO(Box);
   TStruct<ClassTemplateArgument>::_class = CO(ClassTemplateArgument);
   TStruct<DataValue>::_class = CO(DataValue);
   TStruct<NameSpace>::_class = CO(NameSpace);
   TStruct<Point>::_class = CO(Point);
   TStruct<Pointd>::_class = CO(Pointd);
   TStruct<Pointf>::_class = CO(Pointf);
   TStruct<Size>::_class = CO(Size);
   TStruct<StaticString>::_class = CO(StaticString);

   Application::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Application", "Application",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) Application::constructor,
               (void(*)(void *)) Application::destructor,
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
   Module::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Module", "Module",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) Module::constructor,
               (void(*)(void *)) Module::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   AVLTree::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "AVLTree", "AVLTree",
               sizeof(Instance *), 0,
               null,
               null,
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
   Archive::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Archive", "Archive",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) Archive::constructor,
               (void(*)(void *)) Archive::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   ArchiveDir::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "ArchiveDir", "ArchiveDir",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) ArchiveDir::constructor,
               (void(*)(void *)) ArchiveDir::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   BufferedFile::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "BufferedFile", "BufferedFile",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) BufferedFile::constructor,
               (void(*)(void *)) BufferedFile::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   ConsoleFile::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "ConsoleFile", "ConsoleFile",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) ConsoleFile::constructor,
               (void(*)(void *)) ConsoleFile::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   DualPipe::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "DualPipe", "DualPipe",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) DualPipe::constructor,
               (void(*)(void *)) DualPipe::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   File::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "File", "File",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) File::constructor,
               (void(*)(void *)) File::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   FileMonitor::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "FileMonitor", "FileMonitor",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) FileMonitor::constructor,
               (void(*)(void *)) FileMonitor::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   TempFile::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "TempFile", "TempFile",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) TempFile::constructor,
               (void(*)(void *)) TempFile::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   ECONGlobalSettings::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "ECONGlobalSettings", "ECONGlobalSettings",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) ECONGlobalSettings::constructor,
               (void(*)(void *)) ECONGlobalSettings::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   ECONParser::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "ECONParser", "ECONParser",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) ECONParser::constructor,
               (void(*)(void *)) ECONParser::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   GlobalAppSettings::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "GlobalAppSettings", "GlobalAppSettings",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) GlobalAppSettings::constructor,
               (void(*)(void *)) GlobalAppSettings::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   GlobalSettings::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "GlobalSettings", "GlobalSettings",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) GlobalSettings::constructor,
               (void(*)(void *)) GlobalSettings::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   GlobalSettingsData::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "GlobalSettingsData", "GlobalSettingsData",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) GlobalSettingsData::constructor,
               (void(*)(void *)) GlobalSettingsData::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   GlobalSettingsDriver::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "GlobalSettingsDriver", "GlobalSettingsDriver",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) GlobalSettingsDriver::constructor,
               (void(*)(void *)) GlobalSettingsDriver::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   JSONGlobalSettings::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "JSONGlobalSettings", "JSONGlobalSettings",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) JSONGlobalSettings::constructor,
               (void(*)(void *)) JSONGlobalSettings::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   JSONParser::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "JSONParser", "JSONParser",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) JSONParser::constructor,
               (void(*)(void *)) JSONParser::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   OptionsMap::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "OptionsMap", "OptionsMap",
               sizeof(Instance *), 0,
               null,
               null,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   Thread::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "Thread", "Thread",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) Thread::constructor,
               (void(*)(void *)) Thread::destructor,
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

// Instance methods depending on libecrt
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


      addMethod(_cpp_class.impl, "OnDisplay", (void *) +[](XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) surface, /*1Aa*/int x, /*1Aa*/int y, /*1Aa*/int width, /*1Aa*/void * fieldData, /*1Aa*/int alignment, /*1Aa*/uint displayFlags)
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
            /*2Bg*/TIH<Instance> surface_l(surface); fn(*i, /*3Bd*/*surface_l, /*3Kd*/x, /*3Kd*/y, /*3Kd*/width, /*3Kd*/fieldData, /*3Kd*/alignment, /*3Kd*/displayFlags);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onDisplay_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<Instance> surface_l(surface); fn(*i, /*3Bd*/*surface_l, /*3Kd*/x, /*3Kd*/y, /*3Kd*/width, /*3Kd*/fieldData, /*3Kd*/alignment, /*3Kd*/displayFlags);
         }
         else
         {
            auto method = ((void (*) (XClass * _class, /*1Aa*/C(Instance) o_, /*1Aa*/C(Instance) surface, /*1Aa*/int x, /*1Aa*/int y, /*1Aa*/int width, /*1Aa*/void * fieldData, /*1Aa*/int alignment, /*1Aa*/uint displayFlags))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onDisplay)]);
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


      addMethod(_cpp_class.impl, "OnSaveEdit", (void *) +[](XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(Instance) window, /*1Aa*/void * object)
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
            auto method = ((C(bool) (*) (XClass * _class, /*1Aa*/C(Instance) * o_, /*1Aa*/C(Instance) window, /*1Aa*/void * object))(CO(Instance)->_vTbl)[M_VTBLID(Instance, onSaveEdit)]);
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
            /*2Bg*/TIH<IOChannel> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onSerialize_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<IOChannel> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
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
            /*2Bg*/TIH<IOChannel> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
         }
         // 'cp2' is present
         else if(cppcl && cppcl->vTbl && cppcl->vTbl[vid])
         {
            fn = (Instance_onUnserialize_Functor::FunctionType) cppcl->vTbl[vid];
/*2Bg*/TIH<IOChannel> channel_l(channel); fn(*i, /*3Bd*/*channel_l);
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


/////////////////////////////////////////////////////////////// [ecrt]/ //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


///////////////////////////////////////////////////////////// [ecrt]/eC //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


////////////////////////////////////////////////////// [ecrt]/eC::types //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

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

/////////////////////////////////////////////////////////////// [ecrt]/ //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


///////////////////////////////////////////////////////////// [ecrt]/eC //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

   FieldTypeEx::FieldTypeEx(FieldType type, bool mustFree, FieldValueFormat format, bool isUnsigned, bool isDateTime)
   {
      impl = FIELDTYPEEX(type, mustFree, format, isUnsigned, isDateTime);
   }

////////////////////////////////////////////////////// [ecrt]/eC::types //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

   EscapeCStringOptions::EscapeCStringOptions(bool escapeSingleQuote, bool escapeDoubleQuotes, bool writeQuotes, bool multiLine, int indent)
   {
      impl = ESCAPECSTRINGOPTIONS(escapeSingleQuote, escapeDoubleQuotes, writeQuotes, multiLine, indent);
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
void SerialBuffer::class_registration(CPPClass & _cpp_class)
{
}
void ZString::class_registration(CPPClass & _cpp_class)
{
}

///////////////////////////////////////////////// [ecrt]/eC::containers //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

void AVLTree::class_registration(CPPClass & _cpp_class)
{
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
void LinkList::class_registration(CPPClass & _cpp_class)
{
}
void List::class_registration(CPPClass & _cpp_class)
{
}
void Map::class_registration(CPPClass & _cpp_class)
{
}

////////////////////////////////////////////////////// [ecrt]/eC::files //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


#undef AnyFileChange
FileChange AnyFileChange = FileChange { true, true, true, true, true };

void Archive::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "Clear", (void *) +[](/*1Aa*/C(Archive) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, clear);
         Archive_clear_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_clear_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(Archive) o_))(CO(Archive)->_vTbl)[M_VTBLID(Archive, clear)]);
            if(method) return method (o_);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "FileExists", (void *) +[](/*1Aa*/C(Archive) o_, /*1Aa*/const char * fileName)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, fileExists);
         Archive_fileExists_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_fileExists_Functor::FunctionType) i->vTbl[vid];
            C(FileAttribs) ret = fn(*i, /*3Kd*/fileName); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(FileAttribs) (*) (/*1Aa*/C(Archive) o_, /*1Aa*/const char * fileName))(CO(Archive)->_vTbl)[M_VTBLID(Archive, fileExists)]);
            if(method) return method (o_, fileName);
         }
         return (C(FileAttribs))1;
      });


      addMethod(_cpp_class.impl, "FileOpen", (void *) +[](/*1Aa*/C(Archive) o_, /*1Aa*/const char * fileName)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, fileOpen);
         Archive_fileOpen_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_fileOpen_Functor::FunctionType) i->vTbl[vid];
            File * ret = fn(*i, /*3Kd*/fileName); return ret->impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(File) (*) (/*1Aa*/C(Archive) o_, /*1Aa*/const char * fileName))(CO(Archive)->_vTbl)[M_VTBLID(Archive, fileOpen)]);
            if(method) return method (o_, fileName);
         }
         return (C(File))null;
      });


      addMethod(_cpp_class.impl, "FileOpenAtPosition", (void *) +[](/*1Aa*/C(Archive) o_, /*1Aa*/uint position)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, fileOpenAtPosition);
         Archive_fileOpenAtPosition_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_fileOpenAtPosition_Functor::FunctionType) i->vTbl[vid];
            File * ret = fn(*i, /*3Kd*/position); return ret->impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(File) (*) (/*1Aa*/C(Archive) o_, /*1Aa*/uint position))(CO(Archive)->_vTbl)[M_VTBLID(Archive, fileOpenAtPosition)]);
            if(method) return method (o_, position);
         }
         return (C(File))null;
      });


      addMethod(_cpp_class.impl, "FileOpenCompressed", (void *) +[](/*1Aa*/C(Archive) o_, /*1Aa*/const char * fileName, /*1Aa*/C(bool) * isCompressed, /*1Aa*/uint64 * ucSize)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, fileOpenCompressed);
         Archive_fileOpenCompressed_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_fileOpenCompressed_Functor::FunctionType) i->vTbl[vid];
            File * ret = fn(*i, /*3Kd*/fileName, /*3Fd*/isCompressed, /*3Kd*/ucSize); return ret->impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(File) (*) (/*1Aa*/C(Archive) o_, /*1Aa*/const char * fileName, /*1Aa*/C(bool) * isCompressed, /*1Aa*/uint64 * ucSize))(CO(Archive)->_vTbl)[M_VTBLID(Archive, fileOpenCompressed)]);
            if(method) return method (o_, fileName, isCompressed, ucSize);
         }
         return (C(File))null;
      });


      addMethod(_cpp_class.impl, "OpenDirectory", (void *) +[](/*1Aa*/C(Archive) o_, /*1Aa*/const char * name, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, openDirectory);
         Archive_openDirectory_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_openDirectory_Functor::FunctionType) i->vTbl[vid];
            ArchiveDir * ret = fn(*i, /*3Kd*/name, /*3Id*/*(FileStats *)stats, /*3Hd*/(ArchiveAddMode)addMode); return ret->impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(ArchiveDir) (*) (/*1Aa*/C(Archive) o_, /*1Aa*/const char * name, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode))(CO(Archive)->_vTbl)[M_VTBLID(Archive, openDirectory)]);
            if(method) return method (o_, name, stats, addMode);
         }
         return (C(ArchiveDir))null;
      });


      addMethod(_cpp_class.impl, "SetBufferRead", (void *) +[](/*1Aa*/C(Archive) o_, /*1Aa*/uint bufferRead)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, setBufferRead);
         Archive_setBufferRead_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_setBufferRead_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Kd*/bufferRead);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Archive) o_, /*1Aa*/uint bufferRead))(CO(Archive)->_vTbl)[M_VTBLID(Archive, setBufferRead)]);
            if(method) return method (o_, bufferRead);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "SetBufferSize", (void *) +[](/*1Aa*/C(Archive) o_, /*1Aa*/uint bufferSize)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Archive * i = (o_) ? (Archive *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Archive, setBufferSize);
         Archive_setBufferSize_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Archive_setBufferSize_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Kd*/bufferSize);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(Archive) o_, /*1Aa*/uint bufferSize))(CO(Archive)->_vTbl)[M_VTBLID(Archive, setBufferSize)]);
            if(method) return method (o_, bufferSize);
         }
         return ;
      });


}
void ArchiveDir::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "AddFromFile", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/C(File) input, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode, /*1Aa*/int compression, /*1Aa*/int * ratio, /*1Aa*/uint * newPosition)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, addFromFile);
         ArchiveDir_addFromFile_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_addFromFile_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<File> input_l(input); C(bool) ret = (C(bool))fn(*i, /*3Kd*/name, /*3Bd*/*input_l, /*3Id*/*(FileStats *)stats, /*3Hd*/(ArchiveAddMode)addMode, /*3Kd*/compression, /*3Kd*/ratio, /*3Kd*/newPosition); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/C(File) input, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode, /*1Aa*/int compression, /*1Aa*/int * ratio, /*1Aa*/uint * newPosition))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, addFromFile)]);
            if(method) return method (o_, name, input, stats, addMode, compression, ratio, newPosition);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "AddFromFileAtPosition", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/uint position, /*1Aa*/const char * name, /*1Aa*/C(File) input, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode, /*1Aa*/int compression, /*1Aa*/int * ratio, /*1Aa*/uint * newPosition)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, addFromFileAtPosition);
         ArchiveDir_addFromFileAtPosition_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_addFromFileAtPosition_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<File> input_l(input); C(bool) ret = (C(bool))fn(*i, /*3Kd*/position, /*3Kd*/name, /*3Bd*/*input_l, /*3Id*/*(FileStats *)stats, /*3Hd*/(ArchiveAddMode)addMode, /*3Kd*/compression, /*3Kd*/ratio, /*3Kd*/newPosition); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/uint position, /*1Aa*/const char * name, /*1Aa*/C(File) input, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode, /*1Aa*/int compression, /*1Aa*/int * ratio, /*1Aa*/uint * newPosition))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, addFromFileAtPosition)]);
            if(method) return method (o_, position, name, input, stats, addMode, compression, ratio, newPosition);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Delete", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * fileName)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, delete);
         ArchiveDir_delete_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_delete_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/fileName); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * fileName))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, delete)]);
            if(method) return method (o_, fileName);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "FileExists", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * fileName)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, fileExists);
         ArchiveDir_fileExists_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_fileExists_Functor::FunctionType) i->vTbl[vid];
            C(FileAttribs) ret = fn(*i, /*3Kd*/fileName); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(FileAttribs) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * fileName))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, fileExists)]);
            if(method) return method (o_, fileName);
         }
         return (C(FileAttribs))1;
      });


      addMethod(_cpp_class.impl, "FileOpen", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * fileName)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, fileOpen);
         ArchiveDir_fileOpen_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_fileOpen_Functor::FunctionType) i->vTbl[vid];
            File * ret = fn(*i, /*3Kd*/fileName); return ret->impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(File) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * fileName))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, fileOpen)]);
            if(method) return method (o_, fileName);
         }
         return (C(File))null;
      });


      addMethod(_cpp_class.impl, "Move", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/C(ArchiveDir) to)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, move);
         ArchiveDir_move_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_move_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<ArchiveDir> to_l(to); C(bool) ret = (C(bool))fn(*i, /*3Kd*/name, /*3Bd*/*to_l); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/C(ArchiveDir) to))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, move)]);
            if(method) return method (o_, name, to);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "OpenDirectory", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, openDirectory);
         ArchiveDir_openDirectory_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_openDirectory_Functor::FunctionType) i->vTbl[vid];
            ArchiveDir * ret = fn(*i, /*3Kd*/name, /*3Id*/*(FileStats *)stats, /*3Hd*/(ArchiveAddMode)addMode); return ret->impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(ArchiveDir) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/C(FileStats) * stats, /*1Aa*/C(ArchiveAddMode) addMode))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, openDirectory)]);
            if(method) return method (o_, name, stats, addMode);
         }
         return (C(ArchiveDir))null;
      });


      addMethod(_cpp_class.impl, "Rename", (void *) +[](/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/const char * newName)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         ArchiveDir * i = (o_) ? (ArchiveDir *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(ArchiveDir, rename);
         ArchiveDir_rename_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (ArchiveDir_rename_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/name, /*3Kd*/newName); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(ArchiveDir) o_, /*1Aa*/const char * name, /*1Aa*/const char * newName))(CO(ArchiveDir)->_vTbl)[M_VTBLID(ArchiveDir, rename)]);
            if(method) return method (o_, name, newName);
         }
         return (C(bool))1;
      });


}
   ArchiveOpenFlags::ArchiveOpenFlags(bool writeAccess, bool buffered, bool exclusive, bool waitLock)
   {
      impl = ARCHIVEOPENFLAGS(writeAccess, buffered, exclusive, waitLock);
   }
void BufferedFile::class_registration(CPPClass & _cpp_class)
{
}
void ConsoleFile::class_registration(CPPClass & _cpp_class)
{
}
void DualPipe::class_registration(CPPClass & _cpp_class)
{
}
   ErrorCode::ErrorCode(ErrorLevel level, uint code)
   {
      impl = ERRORCODE(level, code);
   }
void File::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "Close", (void *) +[](/*1Aa*/C(File) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, close);
         File_close_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_close_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(File) o_))(CO(File)->_vTbl)[M_VTBLID(File, close)]);
            if(method) return method (o_);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "CloseInput", (void *) +[](/*1Aa*/C(File) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, closeInput);
         File_closeInput_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_closeInput_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(File) o_))(CO(File)->_vTbl)[M_VTBLID(File, closeInput)]);
            if(method) return method (o_);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "CloseOutput", (void *) +[](/*1Aa*/C(File) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, closeOutput);
         File_closeOutput_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_closeOutput_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(File) o_))(CO(File)->_vTbl)[M_VTBLID(File, closeOutput)]);
            if(method) return method (o_);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "Eof", (void *) +[](/*1Aa*/C(File) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, eof);
         File_eof_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_eof_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_))(CO(File)->_vTbl)[M_VTBLID(File, eof)]);
            if(method) return method (o_);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "GetSize", (void *) +[](/*1Aa*/C(File) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, getSize);
         File_getSize_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_getSize_Functor::FunctionType) i->vTbl[vid];
            uint64 ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uint64 (*) (/*1Aa*/C(File) o_))(CO(File)->_vTbl)[M_VTBLID(File, getSize)]);
            if(method) return method (o_);
         }
         return (uint64)1;
      });


      addMethod(_cpp_class.impl, "Getc", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/char * ch)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, getc);
         File_getc_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_getc_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/ch); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_, /*1Aa*/char * ch))(CO(File)->_vTbl)[M_VTBLID(File, getc)]);
            if(method) return method (o_, ch);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Lock", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/C(FileLock) type, /*1Aa*/uint64 start, /*1Aa*/uint64 length, /*1Aa*/C(bool) wait)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, lock);
         File_lock_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_lock_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Hd*/(FileLock)type, /*3Kd*/start, /*3Kd*/length, /*3Hd*/(bool)wait); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_, /*1Aa*/C(FileLock) type, /*1Aa*/uint64 start, /*1Aa*/uint64 length, /*1Aa*/C(bool) wait))(CO(File)->_vTbl)[M_VTBLID(File, lock)]);
            if(method) return method (o_, type, start, length, wait);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Putc", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/char ch)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, putc);
         File_putc_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_putc_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/ch); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_, /*1Aa*/char ch))(CO(File)->_vTbl)[M_VTBLID(File, putc)]);
            if(method) return method (o_, ch);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Puts", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/const char * string)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, puts);
         File_puts_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_puts_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/string); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_, /*1Aa*/const char * string))(CO(File)->_vTbl)[M_VTBLID(File, puts)]);
            if(method) return method (o_, string);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Read", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/void * buffer, /*1Aa*/uintsize size, /*1Aa*/uintsize count)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, read);
         File_read_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_read_Functor::FunctionType) i->vTbl[vid];
            uintsize ret = fn(*i, /*3Kd*/buffer, /*3Kd*/size, /*3Kd*/count); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uintsize (*) (/*1Aa*/C(File) o_, /*1Aa*/void * buffer, /*1Aa*/uintsize size, /*1Aa*/uintsize count))(CO(File)->_vTbl)[M_VTBLID(File, read)]);
            if(method) return method (o_, buffer, size, count);
         }
         return (uintsize)1;
      });


      addMethod(_cpp_class.impl, "Seek", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/int64 pos, /*1Aa*/C(FileSeekMode) mode)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, seek);
         File_seek_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_seek_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/pos, /*3Hd*/(FileSeekMode)mode); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_, /*1Aa*/int64 pos, /*1Aa*/C(FileSeekMode) mode))(CO(File)->_vTbl)[M_VTBLID(File, seek)]);
            if(method) return method (o_, pos, mode);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Tell", (void *) +[](/*1Aa*/C(File) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, tell);
         File_tell_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_tell_Functor::FunctionType) i->vTbl[vid];
            uint64 ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uint64 (*) (/*1Aa*/C(File) o_))(CO(File)->_vTbl)[M_VTBLID(File, tell)]);
            if(method) return method (o_);
         }
         return (uint64)1;
      });


      addMethod(_cpp_class.impl, "Truncate", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/uint64 size)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, truncate);
         File_truncate_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_truncate_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/size); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_, /*1Aa*/uint64 size))(CO(File)->_vTbl)[M_VTBLID(File, truncate)]);
            if(method) return method (o_, size);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Unlock", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/uint64 start, /*1Aa*/uint64 length, /*1Aa*/C(bool) wait)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, unlock);
         File_unlock_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_unlock_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Kd*/start, /*3Kd*/length, /*3Hd*/(bool)wait); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(File) o_, /*1Aa*/uint64 start, /*1Aa*/uint64 length, /*1Aa*/C(bool) wait))(CO(File)->_vTbl)[M_VTBLID(File, unlock)]);
            if(method) return method (o_, start, length, wait);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "Write", (void *) +[](/*1Aa*/C(File) o_, /*1Aa*/const void * buffer, /*1Aa*/uintsize size, /*1Aa*/uintsize count)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         File * i = (o_) ? (File *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(File, write);
         File_write_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (File_write_Functor::FunctionType) i->vTbl[vid];
            uintsize ret = fn(*i, /*3Kd*/buffer, /*3Kd*/size, /*3Kd*/count); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uintsize (*) (/*1Aa*/C(File) o_, /*1Aa*/const void * buffer, /*1Aa*/uintsize size, /*1Aa*/uintsize count))(CO(File)->_vTbl)[M_VTBLID(File, write)]);
            if(method) return method (o_, buffer, size, count);
         }
         return (uintsize)1;
      });


}
   FileChange::FileChange(bool created, bool renamed, bool modified, bool deleted, bool attribs)
   {
      impl = FILECHANGE(created, renamed, modified, deleted, attribs);
   }
void FileMonitor::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "OnDirNotify", (void *) +[](/*1Aa*/any_object o_, /*1Aa*/C(FileChange) action, /*1Aa*/const char * fileName, /*1Aa*/const char * param)
      {
         FileMonitor * i = (FileMonitor *)o_;
         // 'cp1' is empty
         int vid = M_VTBLID(FileMonitor, onDirNotify);
         FileMonitor_onDirNotify_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (FileMonitor_onDirNotify_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(i->_userData, /*3Hd*/(FileChange)action, /*3Kd*/fileName, /*3Kd*/param); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/any_object o_, /*1Aa*/C(FileChange) action, /*1Aa*/const char * fileName, /*1Aa*/const char * param))(CO(FileMonitor)->_vTbl)[M_VTBLID(FileMonitor, onDirNotify)]);
            if(method) return method (o_, action, fileName, param);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "OnFileNotify", (void *) +[](/*1Aa*/any_object o_, /*1Aa*/C(FileChange) action, /*1Aa*/const char * param)
      {
         FileMonitor * i = (FileMonitor *)o_;
         // 'cp1' is empty
         int vid = M_VTBLID(FileMonitor, onFileNotify);
         FileMonitor_onFileNotify_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (FileMonitor_onFileNotify_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(i->_userData, /*3Hd*/(FileChange)action, /*3Kd*/param); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/any_object o_, /*1Aa*/C(FileChange) action, /*1Aa*/const char * param))(CO(FileMonitor)->_vTbl)[M_VTBLID(FileMonitor, onFileNotify)]);
            if(method) return method (o_, action, param);
         }
         return (C(bool))1;
      });


}
   MoveFileOptions::MoveFileOptions(bool overwrite, bool sync)
   {
      impl = MOVEFILEOPTIONS(overwrite, sync);
   }
   PipeOpenMode::PipeOpenMode(bool output, bool error, bool input, bool showWindow)
   {
      impl = PIPEOPENMODE(output, error, input, showWindow);
   }
void TempFile::class_registration(CPPClass & _cpp_class)
{
}

/////////////////////////////////////////////////////// [ecrt]/eC::i18n //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


/////////////////////////////////////////////////////// [ecrt]/eC::json //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

void ECONGlobalSettings::class_registration(CPPClass & _cpp_class)
{
}
void ECONParser::class_registration(CPPClass & _cpp_class)
{
}
void GlobalAppSettings::class_registration(CPPClass & _cpp_class)
{
}
void GlobalSettings::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "Load", (void *) +[](/*1Aa*/C(GlobalSettings) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         GlobalSettings * i = (o_) ? (GlobalSettings *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(GlobalSettings, load);
         GlobalSettings_load_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (GlobalSettings_load_Functor::FunctionType) i->vTbl[vid];
            C(SettingsIOResult) ret = (C(SettingsIOResult))fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(SettingsIOResult) (*) (/*1Aa*/C(GlobalSettings) o_))(CO(GlobalSettings)->_vTbl)[M_VTBLID(GlobalSettings, load)]);
            if(method) return method (o_);
         }
         return (C(SettingsIOResult))1;
      });


      addMethod(_cpp_class.impl, "OnAskReloadSettings", (void *) +[](/*1Aa*/C(GlobalSettings) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         GlobalSettings * i = (o_) ? (GlobalSettings *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(GlobalSettings, onAskReloadSettings);
         GlobalSettings_onAskReloadSettings_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (GlobalSettings_onAskReloadSettings_Functor::FunctionType) i->vTbl[vid];
            fn(*i);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(GlobalSettings) o_))(CO(GlobalSettings)->_vTbl)[M_VTBLID(GlobalSettings, onAskReloadSettings)]);
            if(method) return method (o_);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "Save", (void *) +[](/*1Aa*/C(GlobalSettings) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         GlobalSettings * i = (o_) ? (GlobalSettings *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(GlobalSettings, save);
         GlobalSettings_save_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (GlobalSettings_save_Functor::FunctionType) i->vTbl[vid];
            C(SettingsIOResult) ret = (C(SettingsIOResult))fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(SettingsIOResult) (*) (/*1Aa*/C(GlobalSettings) o_))(CO(GlobalSettings)->_vTbl)[M_VTBLID(GlobalSettings, save)]);
            if(method) return method (o_);
         }
         return (C(SettingsIOResult))1;
      });


}
void GlobalSettingsData::class_registration(CPPClass & _cpp_class)
{
}
void GlobalSettingsDriver::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "Load", (void *) +[](/*1Aa*/C(File) f, /*1Aa*/C(GlobalSettings) globalSettings)
      {
         XClass * cl = ((C(Instance))null) ? (XClass *)((C(Instance))null)->_class : null;
         // 'cp1' is empty
         GlobalSettingsDriver * i = ((C(Instance))null) ? (GlobalSettingsDriver *)INSTANCEL((C(Instance))null, cl) : null;
         int vid = M_VTBLID(GlobalSettingsDriver, load);
         GlobalSettingsDriver_load_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (GlobalSettingsDriver_load_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<File> f_l(f); /*2Bg*/TIH<GlobalSettings> globalSettings_l(globalSettings); C(SettingsIOResult) ret = (C(SettingsIOResult))fn(*i, /*3Bd*/*f_l, /*3Bd*/*globalSettings_l); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(SettingsIOResult) (*) (/*1Aa*/C(File) f, /*1Aa*/C(GlobalSettings) globalSettings))(CO(GlobalSettingsDriver)->_vTbl)[M_VTBLID(GlobalSettingsDriver, load)]);
            if(method) return method (f, globalSettings);
         }
         return (C(SettingsIOResult))1;
      });


      addMethod(_cpp_class.impl, "Save", (void *) +[](/*1Aa*/C(File) f, /*1Aa*/C(GlobalSettings) globalSettings)
      {
         XClass * cl = ((C(Instance))null) ? (XClass *)((C(Instance))null)->_class : null;
         // 'cp1' is empty
         GlobalSettingsDriver * i = ((C(Instance))null) ? (GlobalSettingsDriver *)INSTANCEL((C(Instance))null, cl) : null;
         int vid = M_VTBLID(GlobalSettingsDriver, save);
         GlobalSettingsDriver_save_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (GlobalSettingsDriver_save_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<File> f_l(f); /*2Bg*/TIH<GlobalSettings> globalSettings_l(globalSettings); C(SettingsIOResult) ret = (C(SettingsIOResult))fn(*i, /*3Bd*/*f_l, /*3Bd*/*globalSettings_l); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(SettingsIOResult) (*) (/*1Aa*/C(File) f, /*1Aa*/C(GlobalSettings) globalSettings))(CO(GlobalSettingsDriver)->_vTbl)[M_VTBLID(GlobalSettingsDriver, save)]);
            if(method) return method (f, globalSettings);
         }
         return (C(SettingsIOResult))1;
      });


}
void JSONGlobalSettings::class_registration(CPPClass & _cpp_class)
{
}
void JSONParser::class_registration(CPPClass & _cpp_class)
{
}
void OptionsMap::class_registration(CPPClass & _cpp_class)
{
}

///////////////////////////////////////////////////////// [ecrt]/eC::mt //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

void Thread::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "Main", (void *) +[](/*1Aa*/C(Thread) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         Thread * i = (o_) ? (Thread *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(Thread, main);
         Thread_main_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (Thread_main_Functor::FunctionType) i->vTbl[vid];
            uint ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uint (*) (/*1Aa*/C(Thread) o_))(CO(Thread)->_vTbl)[M_VTBLID(Thread, main)]);
            if(method) return method (o_);
         }
         return (uint)1;
      });


}

/////////////////////////////////////////////////////// [ecrt]/eC::time //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////

