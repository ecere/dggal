public import IMPORT_STATIC "ecrt"

import "JSONSchema"

public class DGGSJSONShape
{
public:
   int count;
   int subZones;
   Map<String, int> dimensions;

   ~DGGSJSONShape()
   {
      delete dimensions;
   }
}

public class DGGSJSONGrid
{
public:
   int cellsCount;
   double resolution;
   Array<FieldValue> coordinates;
   Array<FieldValue> boundsCoordinates;
   Array<FieldValue> relativeBounds;
   FieldValue firstCoordinate;

   ~DGGSJSONGrid()
   {
      if(coordinates) coordinates.Free(), delete coordinates;
      if(relativeBounds) relativeBounds.Free(), delete relativeBounds;
      firstCoordinate.OnFree();
   }
}

public class DGGSJSONDimension
{
public:
   String name;
   Array<FieldValue> interval;
   DGGSJSONGrid grid;
   String definition;
   String unit;
   String unitLang;

   ~DGGSJSONDimension()
   {
      delete unit;
      delete unitLang;
      delete definition;
      delete name;
      if(interval) interval.Free(), delete interval;
      delete grid;
   }
}

public class DGGSJSONDepth
{
public:
   int depth;
   DGGSJSONShape shape;
   Array<FieldValue> data;

   ~DGGSJSONDepth()
   {
      delete shape;
      delete data;
   }
}

public class DGGSJSON
{
public:
   // $schema
   String dggrs;
   String zoneId;
   Array<int> depths;
   String representedValue;
   JSONSchema schema;
   Array<DGGSJSONDimension> dimensions;
   Map<String, Array<DGGSJSONDepth>> values;

   ~DGGSJSON()
   {
      delete dggrs;
      delete zoneId;
      delete depths;
      delete representedValue;
      delete schema;
      if(dimensions) dimensions.Free(), delete dimensions;
      if(values) values.Free(), delete values;
   };

}

public DGGSJSON readDGGSJSON(File f)
{
   DGGSJSON dggsJSON = null;
   JSONParser parser { f  };
   if(parser.GetObject(class(DGGSJSON), &dggsJSON) != success)
      delete dggsJSON;
   delete parser;
   return dggsJSON;
}
