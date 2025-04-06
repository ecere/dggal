public import IMPORT_STATIC "ecrt"

Map<String, const String> dashMapJSONSchema
{ [
   { "id", "$id" },
   { "schema", "$schema" },
   { "xogcrole", "x-ogc-role" },
   { "xogcpropertySeq", "x-ogc-propertySeq" }
] };

Map<String, const String> dashMapJSONSchemaNoId
{ [
   // { "id", "$id" },
   { "schema", "$schema" },
   { "xogcrole", "x-ogc-role" },
   { "xogcpropertySeq", "x-ogc-propertySeq" }
] };

public class JSONSchema
{
public:
   String schema;
   String id;
   String title;
   String comment;
   String description;
   FieldValue Default;
   bool readOnly;
   bool writeOnly;
   Array<String> examples;
   Array<double> multipleOf;
   JSONSchemaType type;
   Array<FieldValue> Enum;
   String format; // geometry-multipolygon
   String contentMediaType;
   double maximum;
   property double maximum { get { return maximum; } isset { return maximum != MAXINT; } }
   double exclusiveMaximum;
   property double exclusiveMaximum { get { return exclusiveMaximum; } isset { return exclusiveMaximum != MAXINT; } }
   double minimum;
   property double minimum { get { return minimum; } isset { return minimum != MININT; } }
   double exclusiveMinimum;
   property double exclusiveMinimum { get { return exclusiveMinimum; } isset { return exclusiveMinimum != MININT; } }
   String pattern;
   JSONSchema items;
   int maxItems;
   property int maxItems { get { return maxItems; } isset { return maxItems != MAXINT; } }
   int minItems;
   property int minItems { get { return minItems; } isset { return minItems != 0; } }
   bool uniqueItems;
   String contains;
   int maxProperties;
   property int maxProperties { get { return maxProperties; } isset { return maxProperties != MAXINT; } }
   int minProperties;
   property int minProperties { get { return minProperties; } isset { return minProperties != 0; } }
   Array<String> required;
   JSONSchema additionalProperties;
   Map<String, JSONSchema> definitions;
   Map<String, JSONSchema> properties;
   Map<String, JSONSchema> patternProperties;
   Map<String, JSONSchema> dependencies;
   String propertyNames;
   String contentEncoding;
   JSONSchema If;
   JSONSchema Then;
   JSONSchema Else;
   Array<JSONSchema> allOf;
   Array<JSONSchema> anyOf;
   Array<JSONSchema> oneOf;
   JSONSchema Not;
   String xogcrole;  // id, primary-geometry, primary-instant
   int xogcpropertySeq;

   property int xogcpropertySeq
   {
      isset { return xogcpropertySeq != 0; }
   }

   property FieldValue Default
   {
      isset { return Default.type != 0; }
   }
   JSONSchema()
   {
      maximum = MAXINT;
      minimum = MININT;
      exclusiveMaximum = MAXINT;
      exclusiveMinimum = MININT;
      maxItems = MAXINT;
      maxProperties = MAXINT;
   }
private:
   ~JSONSchema()
   {
      delete title;
      delete id;
      delete schema;
      delete comment;
      delete description;
      if(examples) examples.Free(), delete examples;
      if(multipleOf) multipleOf.Free(), delete multipleOf;
      if(Enum) Enum.Free(), delete Enum;
      delete format;
      delete contentMediaType;
      delete pattern;
      delete items;
      if(required) required.Free(), delete required;
      delete additionalProperties;
      if(definitions) definitions.Free(), delete definitions;
      if(properties) properties.Free(), delete properties;
      if(patternProperties) patternProperties.Free(), delete patternProperties;
      if(dependencies) dependencies.Free(), delete dependencies;
      delete propertyNames;
      delete contentEncoding;
      delete If;
      delete Then;
      delete Else;
      if(allOf) allOf.Free(), delete allOf;
      if(anyOf) anyOf.Free(), delete anyOf;
      if(oneOf) oneOf.Free(), delete oneOf;
      delete Not;
      delete xogcrole;
      Default.OnFree();
   }
}

public enum JSONSchemaType
{
   unset, array, boolean, integer, null, number, object, string;

   const char * OnGetString(char * tempString, void * fieldData, ObjectNotationType * onType)
   {
      class::OnGetString(tempString, fieldData, onType);
      tempString[0] = (char)tolower(tempString[0]);
      return tempString;
   }
};
