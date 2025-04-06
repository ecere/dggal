#include "dggal.hpp"

TCPPClass<BCTA3H> BCTA3H::_cpp_class;
TCPPClass<DGGRS> DGGRS::_cpp_class;
TCPPClass<DGGSJSON> DGGSJSON::_cpp_class;
TCPPClass<DGGSJSONDepth> DGGSJSONDepth::_cpp_class;
TCPPClass<DGGSJSONGrid> DGGSJSONGrid::_cpp_class;
TCPPClass<DGGSJSONShape> DGGSJSONShape::_cpp_class;
TCPPClass<GNOSISGlobalGrid> GNOSISGlobalGrid::_cpp_class;
TCPPClass<GPP3H> GPP3H::_cpp_class;
TCPPClass<ISEA3H> ISEA3H::_cpp_class;
TCPPClass<ISEA9R> ISEA9R::_cpp_class;
TCPPClass<IVEA3H> IVEA3H::_cpp_class;
TCPPClass<IVEA9R> IVEA9R::_cpp_class;
TCPPClass<JSONSchema> JSONSchema::_cpp_class;
TCPPClass<RhombicIcosahedral3H> RhombicIcosahedral3H::_cpp_class;
TCPPClass<RhombicIcosahedral9R> RhombicIcosahedral9R::_cpp_class;

int dggal_cpp_init(const Module & module)
{
   if(!RhombicIcosahedral9R::_cpp_class.impl)
   {
#ifdef _DEBUG
      // printf("%s_cpp_init\n", "dggal");
#endif

   TStruct<CRSExtent>::_class = CO(CRSExtent);
   TStruct<GeoExtent>::_class = CO(GeoExtent);
   TStruct<GeoPoint>::_class = CO(GeoPoint);
   TStruct<Plane>::_class = CO(Plane);
   TStruct<Vector3D>::_class = CO(Vector3D);

   BCTA3H::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "BCTA3H", "BCTA3H",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) BCTA3H::constructor,
               (void(*)(void *)) BCTA3H::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   DGGRS::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "DGGRS", "DGGRS",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) DGGRS::constructor,
               (void(*)(void *)) DGGRS::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   DGGSJSON::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "DGGSJSON", "DGGSJSON",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) DGGSJSON::constructor,
               (void(*)(void *)) DGGSJSON::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   DGGSJSONDepth::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "DGGSJSONDepth", "DGGSJSONDepth",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) DGGSJSONDepth::constructor,
               (void(*)(void *)) DGGSJSONDepth::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   DGGSJSONGrid::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "DGGSJSONGrid", "DGGSJSONGrid",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) DGGSJSONGrid::constructor,
               (void(*)(void *)) DGGSJSONGrid::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   DGGSJSONShape::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "DGGSJSONShape", "DGGSJSONShape",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) DGGSJSONShape::constructor,
               (void(*)(void *)) DGGSJSONShape::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   GNOSISGlobalGrid::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "GNOSISGlobalGrid", "GNOSISGlobalGrid",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) GNOSISGlobalGrid::constructor,
               (void(*)(void *)) GNOSISGlobalGrid::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   GPP3H::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "GPP3H", "GPP3H",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) GPP3H::constructor,
               (void(*)(void *)) GPP3H::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   ISEA3H::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "ISEA3H", "ISEA3H",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) ISEA3H::constructor,
               (void(*)(void *)) ISEA3H::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   ISEA9R::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "ISEA9R", "ISEA9R",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) ISEA9R::constructor,
               (void(*)(void *)) ISEA9R::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   IVEA3H::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "IVEA3H", "IVEA3H",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) IVEA3H::constructor,
               (void(*)(void *)) IVEA3H::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   IVEA9R::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "IVEA9R", "IVEA9R",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) IVEA9R::constructor,
               (void(*)(void *)) IVEA9R::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   JSONSchema::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "JSONSchema", "JSONSchema",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) JSONSchema::constructor,
               (void(*)(void *)) JSONSchema::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   RhombicIcosahedral3H::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "RhombicIcosahedral3H", "RhombicIcosahedral3H",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) RhombicIcosahedral3H::constructor,
               (void(*)(void *)) RhombicIcosahedral3H::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   RhombicIcosahedral9R::_cpp_class.setup(
         (XClass *)eC_registerClass(
               ClassType_normalClass,
               "CPP" "RhombicIcosahedral9R", "RhombicIcosahedral9R",
               sizeof(Instance *), 0,
               (C(bool) (*)(void *)) RhombicIcosahedral9R::constructor,
               (void(*)(void *)) RhombicIcosahedral9R::destructor,
               (module).impl,
               AccessMode_privateAccess, AccessMode_publicAccess));
   }
   return 0;
}


//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
////                                                                        //// ////////////////////////
////    moved to cpp implementations                                        //// ////////////////////////
////                                                                        //// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////////////


////////////////////////////////////////////////////////////// [dggal]/ //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


#undef wholeWorld
GeoExtent wholeWorld = GeoExtent {  { -90, -180 },  { 90, 180 } };

void BCTA3H::class_registration(CPPClass & _cpp_class)
{
}
   CRS_::CRS_(CRSRegistry registry, int crsID, bool h)
   {
      impl = CRS_(registry, crsID, h);
   }
void DGGRS::class_registration(CPPClass & _cpp_class)
{

      addMethod(_cpp_class.impl, "compactZones", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, compactZones);
         DGGRS_compactZones_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_compactZones_Functor::FunctionType) i->vTbl[vid];
            /*2Bg*/TIH<TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)>> zones_l(zones); fn(*i, /*3Bd*/*zones_l);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> & zones))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, compactZones)]);
            if(method) return method (o_, zones);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "countSubZones", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/int depth)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, countSubZones);
         DGGRS_countSubZones_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_countSubZones_Functor::FunctionType) i->vTbl[vid];
            uint64 ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Kd*/depth); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uint64 (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/int depth))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, countSubZones)]);
            if(method) return method (o_, zone, depth);
         }
         return (uint64)1;
      });


      addMethod(_cpp_class.impl, "countZoneEdges", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, countZoneEdges);
         DGGRS_countZoneEdges_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_countZoneEdges_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i, /*3Hd*/(DGGRSZone)zone); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, countZoneEdges)]);
            if(method) return method (o_, zone);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "countZones", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/int level)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, countZones);
         DGGRS_countZones_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_countZones_Functor::FunctionType) i->vTbl[vid];
            uint64 ret = fn(*i, /*3Kd*/level); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((uint64 (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/int level))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, countZones)]);
            if(method) return method (o_, level);
         }
         return (uint64)1;
      });


      addMethod(_cpp_class.impl, "getFirstSubZone", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/int relativeDepth)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getFirstSubZone);
         DGGRS_getFirstSubZone_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getFirstSubZone_Functor::FunctionType) i->vTbl[vid];
            C(DGGRSZone) ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Kd*/relativeDepth); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(DGGRSZone) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/int relativeDepth))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getFirstSubZone)]);
            if(method) return method (o_, zone, relativeDepth);
         }
         return (C(DGGRSZone))1;
      });


      addMethod(_cpp_class.impl, "getIndexMaxDepth", (void *) +[](/*1Aa*/C(DGGRS) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getIndexMaxDepth);
         DGGRS_getIndexMaxDepth_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getIndexMaxDepth_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getIndexMaxDepth)]);
            if(method) return method (o_);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getMaxChildren", (void *) +[](/*1Aa*/C(DGGRS) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getMaxChildren);
         DGGRS_getMaxChildren_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getMaxChildren_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getMaxChildren)]);
            if(method) return method (o_);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getMaxDGGRSZoneLevel", (void *) +[](/*1Aa*/C(DGGRS) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getMaxDGGRSZoneLevel);
         DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getMaxDGGRSZoneLevel_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getMaxDGGRSZoneLevel)]);
            if(method) return method (o_);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getMaxNeighbors", (void *) +[](/*1Aa*/C(DGGRS) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getMaxNeighbors);
         DGGRS_getMaxNeighbors_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getMaxNeighbors_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getMaxNeighbors)]);
            if(method) return method (o_);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getMaxParents", (void *) +[](/*1Aa*/C(DGGRS) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getMaxParents);
         DGGRS_getMaxParents_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getMaxParents_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getMaxParents)]);
            if(method) return method (o_);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getRefinementRatio", (void *) +[](/*1Aa*/C(DGGRS) o_)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getRefinementRatio);
         DGGRS_getRefinementRatio_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getRefinementRatio_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getRefinementRatio)]);
            if(method) return method (o_);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getSubZoneAtIndex", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/int relativeDepth, /*1Aa*/int64 index)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getSubZoneAtIndex);
         DGGRS_getSubZoneAtIndex_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getSubZoneAtIndex_Functor::FunctionType) i->vTbl[vid];
            C(DGGRSZone) ret = fn(*i, /*3Hd*/(DGGRSZone)parent, /*3Kd*/relativeDepth, /*3Kd*/index); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(DGGRSZone) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/int relativeDepth, /*1Aa*/int64 index))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getSubZoneAtIndex)]);
            if(method) return method (o_, parent, relativeDepth, index);
         }
         return (C(DGGRSZone))1;
      });


      addMethod(_cpp_class.impl, "getSubZoneCRSCentroids", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/C(CRS) crs, /*1Aa*/int relativeDepth)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getSubZoneCRSCentroids);
         DGGRS_getSubZoneCRSCentroids_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getSubZoneCRSCentroids_Functor::FunctionType) i->vTbl[vid];
            TArray<Pointd _ARG int _ARG Pointd> ret = fn(*i, /*3Hd*/(DGGRSZone)parent, /*3Hd*/(CRS_)crs, /*3Kd*/relativeDepth); return ret.impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(Array) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/C(CRS) crs, /*1Aa*/int relativeDepth))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getSubZoneCRSCentroids)]);
            if(method) return method (o_, parent, crs, relativeDepth);
         }
         return (C(Array))null;
      });


      addMethod(_cpp_class.impl, "getSubZoneIndex", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/C(DGGRSZone) subZone)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getSubZoneIndex);
         DGGRS_getSubZoneIndex_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getSubZoneIndex_Functor::FunctionType) i->vTbl[vid];
            int64 ret = fn(*i, /*3Hd*/(DGGRSZone)parent, /*3Hd*/(DGGRSZone)subZone); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int64 (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/C(DGGRSZone) subZone))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getSubZoneIndex)]);
            if(method) return method (o_, parent, subZone);
         }
         return (int64)1;
      });


      addMethod(_cpp_class.impl, "getSubZoneWGS84Centroids", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/int relativeDepth)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getSubZoneWGS84Centroids);
         DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getSubZoneWGS84Centroids_Functor::FunctionType) i->vTbl[vid];
            TArray<GeoPoint _ARG int _ARG GeoPoint> ret = fn(*i, /*3Hd*/(DGGRSZone)parent, /*3Kd*/relativeDepth); return ret.impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(Array) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/int relativeDepth))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getSubZoneWGS84Centroids)]);
            if(method) return method (o_, parent, relativeDepth);
         }
         return (C(Array))null;
      });


      addMethod(_cpp_class.impl, "getSubZones", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/int relativeDepth)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getSubZones);
         DGGRS_getSubZones_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getSubZones_Functor::FunctionType) i->vTbl[vid];
            TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> ret = fn(*i, /*3Hd*/(DGGRSZone)parent, /*3Kd*/relativeDepth); return ret.impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(Array) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) parent, /*1Aa*/int relativeDepth))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getSubZones)]);
            if(method) return method (o_, parent, relativeDepth);
         }
         return (C(Array))null;
      });


      addMethod(_cpp_class.impl, "getZoneArea", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneArea);
         DGGRS_getZoneArea_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneArea_Functor::FunctionType) i->vTbl[vid];
            double ret = fn(*i, /*3Hd*/(DGGRSZone)zone); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((double (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneArea)]);
            if(method) return method (o_, zone);
         }
         return (double)1;
      });


      addMethod(_cpp_class.impl, "getZoneCRSCentroid", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/C(Pointd) * centroid)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneCRSCentroid);
         DGGRS_getZoneCRSCentroid_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneCRSCentroid_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(CRS_)crs, /*3Id*/*(Pointd *)centroid);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/C(Pointd) * centroid))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneCRSCentroid)]);
            if(method) return method (o_, zone, crs, centroid);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "getZoneCRSExtent", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/C(CRSExtent) * extent)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneCRSExtent);
         DGGRS_getZoneCRSExtent_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneCRSExtent_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(CRS_)crs, /*3Id*/*(CRSExtent *)extent);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/C(CRSExtent) * extent))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneCRSExtent)]);
            if(method) return method (o_, zone, crs, extent);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "getZoneCRSVertices", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/C(Pointd) * vertices)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneCRSVertices);
         DGGRS_getZoneCRSVertices_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneCRSVertices_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(CRS_)crs, /*3Hd*/(Pointd *)vertices); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/C(Pointd) * vertices))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneCRSVertices)]);
            if(method) return method (o_, zone, crs, vertices);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getZoneCentroidChild", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneCentroidChild);
         DGGRS_getZoneCentroidChild_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneCentroidChild_Functor::FunctionType) i->vTbl[vid];
            C(DGGRSZone) ret = fn(*i, /*3Hd*/(DGGRSZone)zone); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(DGGRSZone) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneCentroidChild)]);
            if(method) return method (o_, zone);
         }
         return (C(DGGRSZone))1;
      });


      addMethod(_cpp_class.impl, "getZoneCentroidParent", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneCentroidParent);
         DGGRS_getZoneCentroidParent_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneCentroidParent_Functor::FunctionType) i->vTbl[vid];
            C(DGGRSZone) ret = fn(*i, /*3Hd*/(DGGRSZone)zone); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(DGGRSZone) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneCentroidParent)]);
            if(method) return method (o_, zone);
         }
         return (C(DGGRSZone))1;
      });


      addMethod(_cpp_class.impl, "getZoneChildren", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(DGGRSZone) * children)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneChildren);
         DGGRS_getZoneChildren_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneChildren_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(DGGRSZone *)children); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(DGGRSZone) * children))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneChildren)]);
            if(method) return method (o_, zone, children);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getZoneFromCRSCentroid", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/int level, /*1Aa*/C(CRS) crs, /*1Aa*/const C(Pointd) * centroid)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneFromCRSCentroid);
         DGGRS_getZoneFromCRSCentroid_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneFromCRSCentroid_Functor::FunctionType) i->vTbl[vid];
            C(DGGRSZone) ret = fn(*i, /*3Kd*/level, /*3Hd*/(CRS_)crs, /*3Id*/*(Pointd *)centroid); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(DGGRSZone) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/int level, /*1Aa*/C(CRS) crs, /*1Aa*/const C(Pointd) * centroid))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneFromCRSCentroid)]);
            if(method) return method (o_, level, crs, centroid);
         }
         return (C(DGGRSZone))1;
      });


      addMethod(_cpp_class.impl, "getZoneFromTextID", (void *) +[](/*1Aa*/C(DGGRS) o_, constString zoneID)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneFromTextID);
         DGGRS_getZoneFromTextID_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneFromTextID_Functor::FunctionType) i->vTbl[vid];
            C(DGGRSZone) ret = fn(*i, /*3Kd*/zoneID); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(DGGRSZone) (*) (/*1Aa*/C(DGGRS) o_, constString zoneID))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneFromTextID)]);
            if(method) return method (o_, zoneID);
         }
         return (C(DGGRSZone))1;
      });


      addMethod(_cpp_class.impl, "getZoneFromWGS84Centroid", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/int level, /*1Aa*/const C(GeoPoint) * centroid)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneFromWGS84Centroid);
         DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneFromWGS84Centroid_Functor::FunctionType) i->vTbl[vid];
            C(DGGRSZone) ret = fn(*i, /*3Kd*/level, /*3Id*/*(GeoPoint *)centroid); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(DGGRSZone) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/int level, /*1Aa*/const C(GeoPoint) * centroid))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneFromWGS84Centroid)]);
            if(method) return method (o_, level, centroid);
         }
         return (C(DGGRSZone))1;
      });


      addMethod(_cpp_class.impl, "getZoneLevel", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneLevel);
         DGGRS_getZoneLevel_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneLevel_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i, /*3Hd*/(DGGRSZone)zone); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneLevel)]);
            if(method) return method (o_, zone);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getZoneNeighbors", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(DGGRSZone) * neighbors, /*1Aa*/int * nbType)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneNeighbors);
         DGGRS_getZoneNeighbors_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneNeighbors_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(DGGRSZone *)neighbors, /*3Kd*/nbType); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(DGGRSZone) * neighbors, /*1Aa*/int * nbType))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneNeighbors)]);
            if(method) return method (o_, zone, neighbors, nbType);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getZoneParents", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(DGGRSZone) * parents)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneParents);
         DGGRS_getZoneParents_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneParents_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(DGGRSZone *)parents); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(DGGRSZone) * parents))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneParents)]);
            if(method) return method (o_, zone, parents);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "getZoneRefinedCRSVertices", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/int edgeRefinement)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneRefinedCRSVertices);
         DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneRefinedCRSVertices_Functor::FunctionType) i->vTbl[vid];
            TArray<Pointd _ARG int _ARG Pointd> ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(CRS_)crs, /*3Kd*/edgeRefinement); return ret.impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(Array) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(CRS) crs, /*1Aa*/int edgeRefinement))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneRefinedCRSVertices)]);
            if(method) return method (o_, zone, crs, edgeRefinement);
         }
         return (C(Array))null;
      });


      addMethod(_cpp_class.impl, "getZoneRefinedWGS84Vertices", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/int edgeRefinement)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneRefinedWGS84Vertices);
         DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneRefinedWGS84Vertices_Functor::FunctionType) i->vTbl[vid];
            TArray<GeoPoint _ARG int _ARG GeoPoint> ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Kd*/edgeRefinement); return ret.impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(Array) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/int edgeRefinement))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneRefinedWGS84Vertices)]);
            if(method) return method (o_, zone, edgeRefinement);
         }
         return (C(Array))null;
      });


      addMethod(_cpp_class.impl, "getZoneTextID", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, C(String) zoneID)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneTextID);
         DGGRS_getZoneTextID_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneTextID_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Kd*/zoneID);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, C(String) zoneID))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneTextID)]);
            if(method) return method (o_, zone, zoneID);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "getZoneWGS84Centroid", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(GeoPoint) * centroid)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneWGS84Centroid);
         DGGRS_getZoneWGS84Centroid_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneWGS84Centroid_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Id*/*(GeoPoint *)centroid);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(GeoPoint) * centroid))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneWGS84Centroid)]);
            if(method) return method (o_, zone, centroid);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "getZoneWGS84Extent", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(GeoExtent) * extent)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneWGS84Extent);
         DGGRS_getZoneWGS84Extent_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneWGS84Extent_Functor::FunctionType) i->vTbl[vid];
            fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Id*/*(GeoExtent *)extent);
         }
         // 'cp2' is empty
         else
         {
            auto method = ((void (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(GeoExtent) * extent))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneWGS84Extent)]);
            if(method) return method (o_, zone, extent);
         }
         return ;
      });


      addMethod(_cpp_class.impl, "getZoneWGS84Vertices", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(GeoPoint) * vertices)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, getZoneWGS84Vertices);
         DGGRS_getZoneWGS84Vertices_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_getZoneWGS84Vertices_Functor::FunctionType) i->vTbl[vid];
            int ret = fn(*i, /*3Hd*/(DGGRSZone)zone, /*3Hd*/(GeoPoint *)vertices); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((int (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone, /*1Aa*/C(GeoPoint) * vertices))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, getZoneWGS84Vertices)]);
            if(method) return method (o_, zone, vertices);
         }
         return (int)1;
      });


      addMethod(_cpp_class.impl, "isZoneCentroidChild", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, isZoneCentroidChild);
         DGGRS_isZoneCentroidChild_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_isZoneCentroidChild_Functor::FunctionType) i->vTbl[vid];
            C(bool) ret = (C(bool))fn(*i, /*3Hd*/(DGGRSZone)zone); return ret;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(bool) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/C(DGGRSZone) zone))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, isZoneCentroidChild)]);
            if(method) return method (o_, zone);
         }
         return (C(bool))1;
      });


      addMethod(_cpp_class.impl, "listZones", (void *) +[](/*1Aa*/C(DGGRS) o_, /*1Aa*/int level, /*1Aa*/const C(GeoExtent) * bbox)
      {
         XClass * cl = (o_) ? (XClass *)(o_)->_class : null;
         // 'cp1' is empty
         DGGRS * i = (o_) ? (DGGRS *)INSTANCEL(o_, cl) : null;
         int vid = M_VTBLID(DGGRS, listZones);
         DGGRS_listZones_Functor::FunctionType fn;
         if(i && i->vTbl && i->vTbl[vid])
         {
            fn = (DGGRS_listZones_Functor::FunctionType) i->vTbl[vid];
            TArray<C(DGGRSZone) _ARG int _ARG C(DGGRSZone)> ret = fn(*i, /*3Kd*/level, /*3Id*/*(GeoExtent *)bbox); return ret.impl;
         }
         // 'cp2' is empty
         else
         {
            auto method = ((C(Array) (*) (/*1Aa*/C(DGGRS) o_, /*1Aa*/int level, /*1Aa*/const C(GeoExtent) * bbox))(CO(DGGRS)->_vTbl)[M_VTBLID(DGGRS, listZones)]);
            if(method) return method (o_, level, bbox);
         }
         return (C(Array))null;
      });


}
void DGGSJSON::class_registration(CPPClass & _cpp_class)
{
}
void DGGSJSONDepth::class_registration(CPPClass & _cpp_class)
{
}
void DGGSJSONGrid::class_registration(CPPClass & _cpp_class)
{
}
void DGGSJSONShape::class_registration(CPPClass & _cpp_class)
{
}
void GNOSISGlobalGrid::class_registration(CPPClass & _cpp_class)
{
}
void GPP3H::class_registration(CPPClass & _cpp_class)
{
}
void ISEA3H::class_registration(CPPClass & _cpp_class)
{
}
void ISEA9R::class_registration(CPPClass & _cpp_class)
{
}
void IVEA3H::class_registration(CPPClass & _cpp_class)
{
}
void IVEA9R::class_registration(CPPClass & _cpp_class)
{
}
void JSONSchema::class_registration(CPPClass & _cpp_class)
{
}
void RhombicIcosahedral3H::class_registration(CPPClass & _cpp_class)
{
}
void RhombicIcosahedral9R::class_registration(CPPClass & _cpp_class)
{
}
