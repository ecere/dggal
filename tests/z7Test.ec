public import IMPORT_STATIC "ecrt"
import "testingFramework"
import IMPORT_STATIC "dggal"
import IMPORT_STATIC "SFCollections"

define testMaxLevel = 8;

Array<const String> pointsFiles
{ [
   "points7H-L0.geojson",
   "points7H-L1.geojson",
   "points7H-L2.geojson",
   "points7H-L3.geojson",
   "points7H-L4.geojson",
   "points7H-L5.geojson",
   "points7H-L6.geojson",
   "points7H-L7-nz.geojson",
   "points7H-L8-nz.geojson"
] };

double authCP[2][AUTH_ORDER];

public class DGGSUnitTest : eTest
{
   void testZ7Indices(int level, const String z7PointsGeoJSON)
   {
      DGGRS dggrs = ISEA7H_Z7 { };
      bool passed = true;
      File f = FileOpen(z7PointsGeoJSON, read);
      if(f)
      {
         HashMap<int64, Map<String, FieldValue>> attribs { };
         FeatureCollection<PointFeature> fc;

         PrintLn("Loading input file ", z7PointsGeoJSON);
         fc = (FeatureCollection<PointFeature>)loadGeoJSON(f, attribs, false);
         if(fc)
         {
            int count = ((Array)fc).GetCount(), i;

            PrintLn("Testing ", count, " ISEA7H-Z7 zones for level ", level);

            for(i = 0; i < count; i++)
            {
               PointFeature * pf = &((Array<PointFeature>)fc)[i];
               uint64 id = pf->id;
               Map<String, FieldValue> attr = attribs[id];
               if(attr)
               {
                  HashMapIterator<String, FieldValue> it { map = (void *)attr };
                  if(it.Index("Name", false) || it.Index("name", false))
                  {
                     FieldValue * fv = (FieldValue *)(uintptr)it.GetData();
                     if(fv)
                     {
                        const String z7ID = fv->type.type == text ? fv->s : null;
                        if(z7ID)
                        {
                           Array<GeoPoint> geom = (Array<GeoPoint>)pf->geometry;
                           if(geom && geom.count == 1)
                           {
                              DGGRSZone zone;
                              GeoPoint geodetic = geom[0];

                              geodetic.lat = latAuthalicToGeodetic(authCP, geodetic.lat);

                              zone = dggrs.getZoneFromWGS84Centroid(level, geodetic);
                              if(zone != nullZone)
                              {
                                 char dggalZ7[256];
                                 dggrs.getZoneTextID(zone, dggalZ7);
                                 if(strcmp(z7ID, dggalZ7))
                                 {
                                    PrintLn("DGGRID Z7 ID: ", z7ID, "; DGGAL Z7 ID: ", dggalZ7);
                                    fail("Z7", z7ID, "of Z7 identifier mismatch"), passed = false;
                                    passed = false;
                                 }
                              }
                              else
                                 fail("Z7", z7PointsGeoJSON, "of failure to resolve zone from centroid"), passed = false;
                           }
                           else
                              fail("Z7", z7PointsGeoJSON, "of invalid Point geometry in input GeoJSON"), passed = false;
                        }
                        else
                           fail("Z7", z7PointsGeoJSON, "of null zone identifier in input GeoJSON"), passed = false;
                     }
                     else
                        fail("Z7", z7PointsGeoJSON, "of null zone identifier in input GeoJSON"), passed = false;
                  }
                  else
                     fail("Z7", z7PointsGeoJSON, "of failure to look up zone identifier \"Name\" attribute in input GeoJSON"), passed = false;
               }
            }
            if(passed)
               pass("Z7 Identifier checks", z7PointsGeoJSON);
            else
               fail("Z7", z7PointsGeoJSON, "of Z7 identifier mismatches");

            delete fc;
         }
         else
            fail("Z7", z7PointsGeoJSON, "of failure to load GeoJSON input points");

         attribs.Free();
         delete attribs;
         delete f;
      }
      else
         fail("Z7", z7PointsGeoJSON, "of failure to open input points file");

      delete dggrs;
   }

   bool prepareTests()
   {
      int i;

      for(i = 0; i <= testMaxLevel; i++)
      {
         char fn[MAX_LOCATION];

         strcpy(fn, inputPath);
         PathCat(fn, pointsFiles[i]);
         if(!FileExists(fn).isFile)
         {
            PrintLn("Failure to open input points file: ", fn);
            return false;
         }
      }
      return true;
   }

   void executeTests()
   {
      int i;

      authalicSetup(wgs84Major, wgs84Minor, authCP);

      for(i = 0; i <= testMaxLevel; i++)
      {
         char fn[MAX_LOCATION];

         strcpy(fn, inputPath);
         PathCat(fn, pointsFiles[i]);
         testZ7Indices(i, fn);
      }

      Sleep(0.1);
   }
}
