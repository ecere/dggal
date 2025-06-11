public import IMPORT_STATIC "ecrt"
import IMPORT_STATIC "dggal"

import "info"
import "zone"
import "grid"
import "geom"
import "level"
import "list"
import "rel"
import "sub"
import "index"
import "compact"
import "decompact"
import "togeo"

static void showSyntax()
{
   PrintLn(
     $"DGGAL, the Discrete Global Grid Abstraction Library\n"
      "Copyright (c) 2025 Ecere Corporation\n"
      "Syntax:\n"
      "   dgg <dggrs> <command> [options] <arguments>\n"
      "\n"
      "Supported DGGRSs:\n"
      "   GNOSIS (Global Grid), ISEA3H, ISEA9R, IVEA3H, IVEA9R, RTEA3H, RTEA9R, rHEALPix (A9)\n"
      "\n"
      "Commands:\n"
      "   info       [zone]\n"
      "      Display information about a DGGRS or about a zone of a DGGRS\n"
      "   zone       <coord1,coord2> [level]\n"
      "      Return DGGRS zone at position -- specified in EPSG:4236 (lat,lon)\n"
      "   level      [level]\n"
      "      Display information about a DGGRS refinement level\n"
      "   grid       [level]\n"
      "      Generate DGGRS grid at specified refinement level (default: 0)\n"
      "   geom       <zone>\n"
      "      Generate geometry for a particular zone\n"
      "   list       [level]\n"
      "      List DGGRS zones (as JSON string array)\n"
      "   rel        <zone 1> <zone 2>\n"
      "      Display information about the relationships between two zones of a DGGRS\n"
      "   sub        <zone> [index]\n"
      "      List subzones of a DGGRS zone or resolve a sub-zone by index\n"
      "   index      <parent zone> <sub-zone>\n"
      "      Display index of sub-zone within parent\n"
      "   compact    <JSON input zone file (zone ID strings array)> \n"
      "      Compact input zone list\n"
      "   decompact  <JSON input zone file (zone ID strings array)> [level]\n"
      "      Decompact zone list\n"
      "   togeo      <DGGS-(UB)JSON(-FG) input file>\n"
      "      Convert DGGS-JSON (DGGS-quantized raster data) or DGGS-JSON-FG (DGGS-quantized vector data) to GeoJSON\n"
      "\n"
      "Options:\n"
      "   -o <filename>\n"
      "     Output to file instead of standard output\n"
      "   -crs <crs>\n"
      "     Select an output coordinate reference system, one of:\n"
      "        EPSG:4326, OGC:CRS84, 5x6, ico (icosahedron net), rhp (rHEALPix)\n"
      "   -depth <relative depth>\n"
      "      For sub, specify relative depth\n"
      "      Also to change depth considered for calculating optional [level] from -scale, -mpp and -pixels\n"
      "        default: depth corresponding to ~64K sub-zones (IS/VEA9R: 5, IS/VEA3H: 10, GNOSIS: 8, rHEALPix: 5)\n"
      "   -bbox <llLat,llLon,urLat,urLon>\n"
      "      Specify extent for which to list zones, generate grid, or reference extent for -pixels\n"
      "      example: -bbox 60,-120,62,-118 -- specified in EPSG:4326 (lat,lon)\n"
      "   -centroids\n"
      "      For sub, list centroids instead of sub-zone identifiers\n"
      "      For togeo, use centroid points for geometry instead of polygons\n"
      "   -compact\n"
      "      For list and grid, return compact list of zones\n"
      "   -mpp <physical meters per sub-zone>\n"
      "      Specify physical meters per sub-zone as substitute for optional [level] arguments\n"
      "   -scale-denom <scale denominator>\n"
      "      Specify scale-denominator as substitute for optional [level] arguments (based on -depth)\n"
      "   -pixels <with,height>\n"
      "      Specify display pixels as a substitute for optional [level] argument (in combination with -bbox)\n"
      "   -display-res <mm-per-pixels>\n"
      "      Specify display resolution in millimeters/pixel in combination with -scale and -pixels (default: 0.28)\n"
      );
}

enum DGGALCommand
{
   info = 1, zone, grid, geom, level, list, rel, sub, index, compact, decompact, togeo
};

class DGGAL : Application
{
   void Main()
   {
      DGGALCommand command = 0;
      Map<String, const String> options = null;
      int a;
      const String currentOption = null;
      bool syntaxError = false;
      int cmdArg = 0;
      subclass(DGGRS) dggrsClass = null;
      // Command arguments
      int gLevel = -1;
      int64 subIndex = -1;
      const String zone1ID = null, zone2ID = null, input = null, coordinates = null;

           if(!strcmpi(argv[0], "i3h") || !strcmpi(argv[0], "isea3h")) dggrsClass = class(ISEA3H), cmdArg = 1;
      else if(!strcmpi(argv[0], "r3h") || !strcmpi(argv[0], "rtea3h")) dggrsClass = class(RTEA3H), cmdArg = 1;
      else if(!strcmpi(argv[0], "ggg") || !strcmpi(argv[0], "gnosis")) dggrsClass = class(GNOSISGlobalGrid), cmdArg = 1;
      else if(!strcmpi(argv[0], "g3h") || !strcmpi(argv[0], "gpp3h"))  dggrsClass = class(GPP3H), cmdArg = 1;
      else if(!strcmpi(argv[0], "b3h") || !strcmpi(argv[0], "bcta3h")) dggrsClass = class(BCTA3H), cmdArg = 1;
      else if(!strcmpi(argv[0], "v3h") || !strcmpi(argv[0], "ivea3h")) dggrsClass = class(IVEA3H), cmdArg = 1;
      else if(!strcmpi(argv[0], "v9r") || !strcmpi(argv[0], "ivea9r")) dggrsClass = class(IVEA9R), cmdArg = 1;
      else if(!strcmpi(argv[0], "i9r") || !strcmpi(argv[0], "isea9r")) dggrsClass = class(ISEA9R), cmdArg = 1;
      else if(!strcmpi(argv[0], "r9r") || !strcmpi(argv[0], "rtea9r")) dggrsClass = class(RTEA9R), cmdArg = 1;
      else if(!strcmpi(argv[0], "rhp") || !strcmpi(argv[0], "rHEALPix")) dggrsClass = class(rHEALPix), cmdArg = 1;

      for(a = 1; !syntaxError && a < argc; a++)
      {
         const char * arg = argv[a];
         if(arg[0] == '-' && !strchr(arg, ',')) // Avoid confusion with negative coordinates
         {
            if(!options) options = {};
            currentOption = arg + 1;

            // Boolean options
            if(!strcmpi(currentOption, "centroids") ||
               !strcmpi(currentOption, "compact"))
               options[currentOption] = "true", currentOption = null;
         }
         else if(currentOption)
         {
            options[currentOption] = arg;
            currentOption = null;
         }
         else
         {
            switch(cmdArg)
            {
               case 0:
                  // DGGRS
                  if(!strncasecmp(arg, "GNOSIS", 6)) dggrsClass = class(GNOSISGlobalGrid);
                  else if(!strcmpi(arg, "ISEA3H"))   dggrsClass = class(ISEA3H);
                  else if(!strcmpi(arg, "ISEA9R"))   dggrsClass = class(ISEA9R);
                  else if(!strcmpi(arg, "GPP3H"))    dggrsClass = class(GPP3H);
                  else if(!strcmpi(arg, "BCTA3H"))   dggrsClass = class(BCTA3H);
                  else if(!strcmpi(arg, "IVEA3H"))   dggrsClass = class(IVEA3H);
                  else if(!strcmpi(arg, "IVEA9R"))   dggrsClass = class(IVEA9R);
                  else if(!strcmpi(arg, "RTEA3H"))   dggrsClass = class(RTEA3H);
                  else if(!strcmpi(arg, "RTEA9R"))   dggrsClass = class(RTEA9R);
                  else if(!strcmpi(arg, "rHEALPix")) dggrsClass = class(rHEALPix);
                  else if(!strcmpi(arg, "togeo"))
                     command = togeo;
                  else
                     syntaxError = true;
                  break;
               case 1:
                  if(command == togeo)
                     input = arg;
                  else
                     // Command
                     syntaxError = !command.OnGetDataFromString(arg);
                  break;
               case 2:
                  // First command argument
                  switch(command)
                  {
                     case grid: case level: case list:
                        if(!gLevel.OnGetDataFromString(arg))
                           syntaxError = true;
                        break;
                     case zone:
                        coordinates = arg;
                        break;
                     case info: case geom: case rel: case sub: case index:
                        zone1ID = arg;
                        break;
                     case togeo: case compact: case decompact:
                        input = arg;
                        break;
                     default: syntaxError = true; break;
                  }
                  break;
               case 3:
                  // Second command argument
                  switch(command)
                  {
                     case rel: case index:
                        zone2ID = arg;
                        break;
                     case sub:
                        if(!subIndex.OnGetDataFromString(arg))
                           syntaxError = true;
                        break;
                     case decompact: case zone:
                        if(!gLevel.OnGetDataFromString(arg))
                           syntaxError = true;
                        break;
                     default: syntaxError = true; break;
                  }
                  break;
               default: syntaxError = true; break;
            }
            cmdArg++;
         }
      }
      if((!dggrsClass && command != togeo) || !command) syntaxError = true;

      if(syntaxError)
      {
         showSyntax();
         exitCode = 1;
      }
      else
      {
         DGGRSZone zone1 = nullZone, zone2 = nullZone;
         DGGRS dggrs = eInstance_New(dggrsClass);
         bool jsonOutput = (command == grid || command == geom || command == compact || command == decompact ||
            command == togeo || command == list || command == sub);

         if(!jsonOutput)
            PrintLn($"DGGRS: https://maps.gnosis.earth/ogcapi/dggrs/", dggrs._class.name);

         if(zone1ID)
         {
            zone1 = dggrs.getZoneFromTextID(zone1ID);
            if(zone1 == nullZone)
            {
               PrintLn($"Invalid ", ((Class)dggrsClass).name, $" zone identifier: ", zone1ID);
               exitCode = 1;
            }
         }
         if(zone2ID)
         {
            zone2 = dggrs.getZoneFromTextID(zone2ID);
            if(zone2 == nullZone)
            {
               PrintLn($"Invalid ", ((Class)dggrsClass).name, $" zone identifier: ", zone2ID);
               exitCode = 1;
            }
         }

         if(gLevel != -1)
         {
            int maxZoneLevel = dggrs.getMaxDGGRSZoneLevel();
            int maxLevel = maxZoneLevel + dggrs.get64KDepth() * 2;
            if(gLevel < 0 || gLevel > maxLevel)
            {
               PrintLn($"Invalid refinement level ", gLevel, $" (max zone level: ", maxZoneLevel, $", max zone + depth: ", maxLevel, ")");
               exitCode = 1;
            }
         }

         if(!exitCode)
            switch(command)
            {
               case info: exitCode = displayInfo(dggrs, zone1, options); break;
               case zone: exitCode = queryZone(dggrs, coordinates, gLevel, options); break;
               case level: exitCode = levelInfo(dggrs, gLevel, options, false); break;
               case rel: exitCode = relationInfo(dggrs, zone1, zone2, options); break;
               case index: exitCode = subZoneIndex(dggrs, zone1, zone2, options); break;

               case grid: exitCode = generateGrid(dggrs, gLevel, options); break;
               case geom: exitCode = generateGeometry(dggrs, zone1, options); break;
               case list: exitCode = listZones(dggrs, gLevel, options); break;
               case sub: exitCode = subZones(dggrs, zone1, subIndex, options); break;
               case compact: exitCode = compactZones(dggrs, input, options); break;
               case decompact: exitCode = decompactZones(dggrs, input, options); break;
               case togeo: exitCode = convertToGeoJSON(input, options); break;
            }
         delete dggrs;
      }
      delete options;
   }
}
