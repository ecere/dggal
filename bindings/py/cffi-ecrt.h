typedef const void * any_object;
typedef const char * constString;

typedef int eC_Alignment;
enum
{
   Alignment_left = 0x0,
   Alignment_right = 0x1,
   Alignment_center = 0x2
};
typedef uint32_t eC_DataDisplayFlags;

typedef void eC_Type;
typedef void eC_Instantiation;
typedef void eC_ClassDefinition;

typedef struct class_members_Instance * eC_Instance;
typedef eC_Instance eC_Surface;
typedef eC_Instance eC_Window;
typedef eC_Window eC_CommonControl;
typedef eC_CommonControl eC_DataBox;
typedef eC_CommonControl eC_EditBox;

typedef int eC_MinMaxValue;
struct eC_Size
{
   eC_MinMaxValue w;
   eC_MinMaxValue h;
};
typedef struct eC_Size eC_Size;
typedef struct eC_BTNode eC_BTNode;
typedef uintptr_t uintptr;
struct eC_BTNode
{
   uintptr key;
   eC_BTNode * parent;
   eC_BTNode * left;
   eC_BTNode * right;
   int depth;
};
typedef struct eC_BinaryTree eC_BinaryTree;
struct eC_BinaryTree
{
   eC_BTNode * root;
   int count;
   int (* CompareKey)(eC_BinaryTree * tree, uintptr a, uintptr b);
   void (* FreeKey)(void * key);
};
typedef struct eC_NameSpace eC_NameSpace;
struct eC_NameSpace
{
   const char * name;
   eC_NameSpace * btParent;
   eC_NameSpace * left;
   eC_NameSpace * right;
   int depth;
   eC_NameSpace * parent;
   eC_BinaryTree nameSpaces;
   eC_BinaryTree classes;
   eC_BinaryTree defines;
   eC_BinaryTree functions;
};
typedef uint32_t uint;
typedef uint eC_bool;
#define false 0
#define true 1

struct eC_OldList
{
   void * first;
   void * last;
   int count;
   uint offset;
   eC_bool circ;
};
typedef eC_Instance eC_Module;
typedef eC_Module eC_Application;
typedef int eC_ImportType;
enum
{
   ImportType_normalImport = 0x0,
   ImportType_staticImport = 0x1,
   ImportType_remoteImport = 0x2,
   ImportType_preDeclImport = 0x3,
   ImportType_comCheckImport = 0x4
};

typedef struct eC_OldList eC_OldList;
struct class_members_Module
{
   eC_Application application;
   eC_OldList classes;
   eC_OldList defines;
   eC_OldList functions;
   eC_OldList modules;
   eC_Module prev;
   eC_Module next;
   const char * name;
   void * library;
   void * Unload;
   eC_ImportType importType;
   eC_ImportType origImportType;
   eC_NameSpace privateNameSpace;
   eC_NameSpace publicNameSpace;
};
typedef int eC_AccessMode;
enum
{
   AccessMode_defaultAccess = 0x0,
   AccessMode_publicAccess = 0x1,
   AccessMode_privateAccess = 0x2,
   AccessMode_staticAccess = 0x3,
   AccessMode_baseSystemAccess = 0x4
};

typedef struct eC_Class eC_Class;
typedef struct eC_ClassTemplateArgument eC_ClassTemplateArgument;
typedef int eC_ClassType;
enum
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

struct eC_Class
{
   eC_Class * prev;
   eC_Class * next;
   const char * name;
   int offset;
   int structSize;
   void ** _vTbl;
   int vTblSize;
   eC_bool (* Constructor)(void *);
   void (* Destructor)(void *);
   int offsetClass;
   int sizeClass;
   eC_Class * base;
   eC_BinaryTree methods;
   eC_BinaryTree members;
   eC_BinaryTree prop;
   eC_OldList membersAndProperties;
   eC_BinaryTree classProperties;
   eC_OldList derivatives;
   int memberID;
   int startMemberID;
   eC_ClassType type;
   eC_Module module;
   eC_NameSpace * nameSpace;
   const char * dataTypeString;
   eC_Type * dataType;
   int typeSize;
   int defaultAlignment;
   void (* Initialize)(void);
   int memberOffset;
   eC_OldList selfWatchers;
   const char * designerClass;
   eC_bool noExpansion;
   const char * defaultProperty;
   eC_bool comRedefinition;
   int count;
   int isRemote;
   eC_bool internalDecl;
   void * data;
   eC_bool computeSize;
   short structAlignment;
   short pointerAlignment;
   int destructionWatchOffset;
   eC_bool fixed;
   eC_OldList delayedCPValues;
   eC_AccessMode inheritanceAccess;
   const char * fullName;
   void * symbol;
   eC_OldList conversions;
   eC_OldList templateParams;
   eC_ClassTemplateArgument * templateArgs;
   eC_Class * templateClass;
   eC_OldList templatized;
   int numParams;
   eC_bool isInstanceClass;
   eC_bool byValueSystemClass;
   void * bindingsClass;
};
typedef struct eC_Method eC_Method;
struct class_members_Application
{
   int argc;
   const char ** argv;
   int exitCode;
   eC_bool isGUIApp;
   eC_OldList allModules;
   char * parsedCommand;
   eC_NameSpace systemNameSpace;
};
extern int Application_main_vTblID;
void Application_main(eC_Application __i);
extern eC_Method * method_Application_main;

struct class_members_Instance
{
   void ** _vTbl;
   eC_Class * _class;
   int _refCount;
};

extern int Module_onLoad_vTblID;
eC_bool Module_onLoad(eC_Module __i);
extern eC_Method * method_Module_onLoad;

extern int Module_onUnload_vTblID;
void Module_onUnload(eC_Module __i);
extern eC_Method * method_Module_onUnload;


typedef struct eC_BTNamedLink eC_BTNamedLink;
struct eC_BTNamedLink
{
   const char * name;
   eC_BTNamedLink * parent;
   eC_BTNamedLink * left;
   eC_BTNamedLink * right;
   int depth;
   void * data;
};
typedef struct eC_BitMember eC_BitMember;
typedef int eC_DataMemberType;
enum
{
   DataMemberType_normalMember = 0x0,
   DataMemberType_unionMember = 0x1,
   DataMemberType_structMember = 0x2
};

typedef uint64_t uint64;
struct eC_BitMember
{
   eC_BitMember * prev;
   eC_BitMember * next;
   const char * name;
   eC_bool isProperty;
   eC_AccessMode memberAccess;
   int id;
   eC_Class * _class;
   const char * dataTypeString;
   eC_Class * dataTypeClass;
   eC_Type * dataType;
   eC_DataMemberType type;
   int size;
   int pos;
   uint64 mask;
};
typedef struct eC_ClassProperty eC_ClassProperty;
typedef int64_t int64;
struct eC_ClassProperty
{
   const char * name;
   eC_ClassProperty * parent;
   eC_ClassProperty * left;
   eC_ClassProperty * right;
   int depth;
   void (* Set)(eC_Class *, int64);
   int64 (* Get)(eC_Class *);
   const char * dataTypeString;
   eC_Type * dataType;
   eC_bool constant;
};
typedef struct eC_DataMember eC_DataMember;
struct eC_DataMember
{
   eC_DataMember * prev;
   eC_DataMember * next;
   const char * name;
   eC_bool isProperty;
   eC_AccessMode memberAccess;
   int id;
   eC_Class * _class;
   const char * dataTypeString;
   eC_Class * dataTypeClass;
   eC_Type * dataType;
   eC_DataMemberType type;
   int offset;
   int memberID;
   eC_OldList members;
   eC_BinaryTree membersAlpha;
   int memberOffset;
   short structAlignment;
   short pointerAlignment;
};
typedef uint8_t byte;
typedef uint16_t uint16;
struct eC_DataValue
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
typedef int eC_MethodType;
enum
{
   MethodType_normalMethod = 0x0,
   MethodType_virtualMethod = 0x1
};

struct eC_Method
{
   const char * name;
   eC_Method * parent;
   eC_Method * left;
   eC_Method * right;
   int depth;
   int (* function)(void);
   int vid;
   eC_MethodType type;
   eC_Class * _class;
   void * symbol;
   const char * dataTypeString;
   eC_Type * dataType;
   eC_AccessMode memberAccess;
};
typedef struct eC_Property eC_Property;
struct eC_Property
{
   eC_Property * prev;
   eC_Property * next;
   const char * name;
   eC_bool isProperty;
   eC_AccessMode memberAccess;
   int id;
   eC_Class * _class;
   const char * dataTypeString;
   eC_Class * dataTypeClass;
   eC_Type * dataType;
   void (* Set)(void *, int);
   int (* Get)(void *);
   eC_bool (* IsSet)(void *);
   void * data;
   void * symbol;
   int vid;
   eC_bool conversion;
   uint watcherOffset;
   const char * category;
   eC_bool compiled;
   eC_bool selfWatchable;
   eC_bool isWatchable;
};
typedef struct eC_DataValue eC_DataValue;
struct eC_ClassTemplateArgument
{
   union
   {
      struct
      {
         const char * dataTypeString;
         eC_Class * dataTypeClass;
      };
      eC_DataValue expression;
      struct
      {
         const char * memberString;
         union
         {
            eC_DataMember * member;
            eC_Property * prop;
            eC_Method * method;
         };
      };
   };
};
typedef struct eC_ClassTemplateParameter eC_ClassTemplateParameter;
typedef int eC_TemplateMemberType;
enum
{
   TemplateMemberType_dataMember = 0x0,
   TemplateMemberType_method = 0x1,
   TemplateMemberType_prop = 0x2
};

typedef int eC_TemplateParameterType;
enum
{
   TemplateParameterType_type = 0x0,
   TemplateParameterType_identifier = 0x1,
   TemplateParameterType_expression = 0x2
};

struct eC_ClassTemplateParameter
{
   eC_ClassTemplateParameter * prev;
   eC_ClassTemplateParameter * next;
   const char * name;
   eC_TemplateParameterType type;
   union
   {
      const char * dataTypeString;
      eC_TemplateMemberType memberType;
   };
   eC_ClassTemplateArgument defaultArg;
   void * param;
};
typedef struct eC_DefinedExpression eC_DefinedExpression;
struct eC_DefinedExpression
{
   eC_DefinedExpression * prev;
   eC_DefinedExpression * next;
   const char * name;
   const char * value;
   eC_NameSpace * nameSpace;
};
typedef struct eC_GlobalFunction eC_GlobalFunction;
struct eC_GlobalFunction
{
   eC_GlobalFunction * prev;
   eC_GlobalFunction * next;
   const char * name;
   int (* function)(void);
   eC_Module module;
   eC_NameSpace * nameSpace;
   const char * dataTypeString;
   eC_Type * dataType;
   void * symbol;
};
typedef struct eC_ObjectInfo eC_ObjectInfo;
struct eC_ObjectInfo
{
   eC_ObjectInfo * prev;
   eC_ObjectInfo * next;
   eC_Instance instance;
   char * name;
   eC_Instantiation * instCode;
   eC_bool deleted;
   eC_ObjectInfo * oClass;
   eC_OldList instances;
   eC_ClassDefinition * classDefinition;
   eC_bool modified;
   void * i18nStrings;
};
typedef struct eC_SubModule eC_SubModule;
struct eC_SubModule
{
   eC_SubModule * prev;
   eC_SubModule * next;
   eC_Module module;
   eC_AccessMode importMode;
};
typedef eC_bool eC_BackSlashEscaping;
enum
{
   BackSlashEscaping_forArgsPassing = 0x2
};

typedef struct eC_Box eC_Box;
typedef eC_Instance eC_ClassDesignerBase;
typedef eC_Instance eC_DesignerBase;
typedef uint32_t uint32;
typedef uint32 eC_EscapeCStringOptions;
typedef eC_Instance eC_IOChannel;
typedef int eC_MinMaxValue;
typedef int eC_Platform;
enum
{
   Platform_unknown = 0x0,
   Platform_win32 = 0x1,
   Platform_tux = 0x2,
   Platform_apple = 0x3
};

typedef struct eC_Point eC_Point;
typedef eC_IOChannel eC_SerialBuffer;
typedef char * eC_String;
typedef int eC_StringAllocType;
enum
{
   StringAllocType_pointer = 0x0,
   StringAllocType_stack = 0x1,
   StringAllocType_heap = 0x2
};

typedef eC_Instance eC_ZString;
typedef size_t uintsize;
static const char DIR_SEP;

static const char * const DIR_SEPS;

static const char * const FORMAT64D;

static const char * const FORMAT64DLL;

static const char * const FORMAT64HEX;

static const char * const FORMAT64HEXLL;

static const char * const FORMAT64U;

#define MAXBYTE 0xff

static const double MAXDOUBLE;

#define MAXDWORD 0xffffffff

static const float MAXFLOAT;

#define MAXINT 2147483647

static const int64 MAXINT64;

static const uint64 MAXQWORD;

#define MAXWORD 0xffff

#define MAX_DIRECTORY 534

#define MAX_EXTENSION 17

#define MAX_FILENAME 274

#define MAX_F_STRING 1025

#define MAX_LOCATION 797

static const double MINDOUBLE;

static const float MINFLOAT;

#define MININT -2147483648

static const int64 MININT64;

typedef double eC_Angle;
typedef char * eC_CIString;
typedef double eC_Distance;
typedef struct eC_EnumClassData eC_EnumClassData;
typedef eC_bool eC_ObjectNotationType;
enum
{
   ObjectNotationType_none = 0x0,
   ObjectNotationType_econ = 0x1,
   ObjectNotationType_json = 0x2
};

typedef struct eC_Pointd eC_Pointd;
typedef struct eC_Pointf eC_Pointf;
typedef struct eC_StaticString eC_StaticString;
struct eC_Box
{
   int left;
   int top;
   int right;
   int bottom;
};
extern void (* Box_clip)(eC_Box * __this, eC_Box * against);

extern void (* Box_clipOffset)(eC_Box * __this, eC_Box * against, int x, int y);

extern eC_bool (* Box_isPointInside)(eC_Box * __this, eC_Point * point);

extern eC_bool (* Box_overlap)(eC_Box * __this, eC_Box * box);

extern eC_Property * property_Box_width;
extern void (* Box_set_width)(const eC_Box * b, int value);
extern int (* Box_get_width)(const eC_Box * b);

extern eC_Property * property_Box_height;
extern void (* Box_set_height)(const eC_Box * b, int value);
extern int (* Box_get_height)(const eC_Box * b);

extern eC_Property * property_Centimeters_Meters;
extern double (* Centimeters_from_Meters)(const eC_Distance meters);
extern eC_Distance (* Centimeters_to_Meters)(const double centimeters);

extern eC_Property * property_Class_char_ptr;
extern void (* Class_from_char_ptr)(const eC_Class * c, const char * value);
extern const char * (* Class_to_char_ptr)(const eC_Class * c);

extern int ClassDesignerBase_addObject_vTblID;
void ClassDesignerBase_addObject(eC_ClassDesignerBase __i);
extern eC_Method * method_ClassDesignerBase_addObject;

extern int ClassDesignerBase_createNew_vTblID;
void ClassDesignerBase_createNew(eC_ClassDesignerBase __i, eC_Instance editBox, eC_Size * clientSize, const char * name, const char * inherit);
extern eC_Method * method_ClassDesignerBase_createNew;

extern int ClassDesignerBase_createObject_vTblID;
void ClassDesignerBase_createObject(eC_ClassDesignerBase __i, eC_DesignerBase designer, eC_Instance instance, eC_ObjectInfo * object, eC_bool isClass, eC_Instance _class);
extern eC_Method * method_ClassDesignerBase_createObject;

extern int ClassDesignerBase_destroyObject_vTblID;
void ClassDesignerBase_destroyObject(eC_ClassDesignerBase __i, eC_Instance object);
extern eC_Method * method_ClassDesignerBase_destroyObject;

extern int ClassDesignerBase_droppedObject_vTblID;
void ClassDesignerBase_droppedObject(eC_ClassDesignerBase __i, eC_Instance instance, eC_ObjectInfo * object, eC_bool isClass, eC_Instance _class);
extern eC_Method * method_ClassDesignerBase_droppedObject;

extern int ClassDesignerBase_fixProperty_vTblID;
void ClassDesignerBase_fixProperty(eC_ClassDesignerBase __i, eC_Property * prop, eC_Instance object);
extern eC_Method * method_ClassDesignerBase_fixProperty;

extern int ClassDesignerBase_listToolBoxClasses_vTblID;
void ClassDesignerBase_listToolBoxClasses(eC_ClassDesignerBase __i, eC_DesignerBase designer);
extern eC_Method * method_ClassDesignerBase_listToolBoxClasses;

extern int ClassDesignerBase_postCreateObject_vTblID;
void ClassDesignerBase_postCreateObject(eC_ClassDesignerBase __i, eC_Instance instance, eC_ObjectInfo * object, eC_bool isClass, eC_Instance _class);
extern eC_Method * method_ClassDesignerBase_postCreateObject;

extern int ClassDesignerBase_prepareTestObject_vTblID;
void ClassDesignerBase_prepareTestObject(eC_ClassDesignerBase __i, eC_DesignerBase designer, eC_Instance test);
extern eC_Method * method_ClassDesignerBase_prepareTestObject;

extern int ClassDesignerBase_reset_vTblID;
void ClassDesignerBase_reset(eC_ClassDesignerBase __i);
extern eC_Method * method_ClassDesignerBase_reset;

extern int ClassDesignerBase_selectObject_vTblID;
void ClassDesignerBase_selectObject(eC_ClassDesignerBase __i, eC_ObjectInfo * object, eC_Instance control);
extern eC_Method * method_ClassDesignerBase_selectObject;

extern eC_Property * property_Degrees_Radians;
extern double (* Degrees_from_Radians)(const eC_Angle radians);
extern eC_Angle (* Degrees_to_Radians)(const double degrees);

struct class_members_DesignerBase
{
   eC_ClassDesignerBase classDesigner;
   const char * objectClass;
   eC_bool isDragging;
};
extern int DesignerBase_addDefaultMethod_vTblID;
void DesignerBase_addDefaultMethod(eC_DesignerBase __i, eC_Instance instance, eC_Instance classInstance);
extern eC_Method * method_DesignerBase_addDefaultMethod;

extern int DesignerBase_addToolBoxClass_vTblID;
void DesignerBase_addToolBoxClass(eC_DesignerBase __i, eC_Class * _class);
extern eC_Method * method_DesignerBase_addToolBoxClass;

extern int DesignerBase_codeAddObject_vTblID;
void DesignerBase_codeAddObject(eC_DesignerBase __i, eC_Instance instance, eC_ObjectInfo * object);
extern eC_Method * method_DesignerBase_codeAddObject;

extern int DesignerBase_deleteObject_vTblID;
void DesignerBase_deleteObject(eC_DesignerBase __i, eC_ObjectInfo * object);
extern eC_Method * method_DesignerBase_deleteObject;

extern int DesignerBase_findObject_vTblID;
eC_bool DesignerBase_findObject(eC_DesignerBase __i, eC_Instance * instance, const char * string);
extern eC_Method * method_DesignerBase_findObject;

extern int DesignerBase_modifyCode_vTblID;
void DesignerBase_modifyCode(eC_DesignerBase __i);
extern eC_Method * method_DesignerBase_modifyCode;

extern int DesignerBase_objectContainsCode_vTblID;
eC_bool DesignerBase_objectContainsCode(eC_DesignerBase __i, eC_ObjectInfo * object);
extern eC_Method * method_DesignerBase_objectContainsCode;

extern int DesignerBase_renameObject_vTblID;
void DesignerBase_renameObject(eC_DesignerBase __i, eC_ObjectInfo * object, const char * name);
extern eC_Method * method_DesignerBase_renameObject;

extern int DesignerBase_selectObjectFromDesigner_vTblID;
void DesignerBase_selectObjectFromDesigner(eC_DesignerBase __i, eC_ObjectInfo * object);
extern eC_Method * method_DesignerBase_selectObjectFromDesigner;

extern int DesignerBase_sheetAddObject_vTblID;
void DesignerBase_sheetAddObject(eC_DesignerBase __i, eC_ObjectInfo * object);
extern eC_Method * method_DesignerBase_sheetAddObject;

extern int DesignerBase_updateProperties_vTblID;
void DesignerBase_updateProperties(eC_DesignerBase __i);
extern eC_Method * method_DesignerBase_updateProperties;

extern eC_Property * property_DesignerBase_classDesigner;
extern void (* DesignerBase_set_classDesigner)(const eC_DesignerBase d, eC_ClassDesignerBase value);
extern eC_ClassDesignerBase (* DesignerBase_get_classDesigner)(const eC_DesignerBase d);

extern eC_Property * property_DesignerBase_objectClass;
extern void (* DesignerBase_set_objectClass)(const eC_DesignerBase d, const char * value);
extern const char * (* DesignerBase_get_objectClass)(const eC_DesignerBase d);

extern eC_Property * property_DesignerBase_isDragging;
extern void (* DesignerBase_set_isDragging)(const eC_DesignerBase d, eC_bool value);
extern eC_bool (* DesignerBase_get_isDragging)(const eC_DesignerBase d);

struct eC_EnumClassData
{
   eC_OldList values;
   int64 largest;
};
#define ESCAPECSTRINGOPTIONS_escapeSingleQuote_SHIFT     0
#define ESCAPECSTRINGOPTIONS_escapeSingleQuote_MASK      0x1
#define ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_SHIFT    1
#define ESCAPECSTRINGOPTIONS_escapeDoubleQuotes_MASK     0x2
#define ESCAPECSTRINGOPTIONS_writeQuotes_SHIFT           2
#define ESCAPECSTRINGOPTIONS_writeQuotes_MASK            0x4
#define ESCAPECSTRINGOPTIONS_multiLine_SHIFT             3
#define ESCAPECSTRINGOPTIONS_multiLine_MASK              0x8
#define ESCAPECSTRINGOPTIONS_indent_SHIFT                4
#define ESCAPECSTRINGOPTIONS_indent_MASK                 0xFFFF0


extern eC_Property * property_Feet_Meters;
extern double (* Feet_from_Meters)(const eC_Distance meters);
extern eC_Distance (* Feet_to_Meters)(const double feet);

extern void (* IOChannel_get)(eC_IOChannel __this, eC_Class * class_data, void * data);

extern void (* IOChannel_put)(eC_IOChannel __this, eC_Class * class_data, void * data);

extern int IOChannel_readData_vTblID;
uintsize IOChannel_readData(eC_IOChannel __i, void * data, uintsize numBytes);
extern eC_Method * method_IOChannel_readData;

extern void (* IOChannel_serialize)(eC_IOChannel __this, eC_Class * class_data, void * data);

extern void (* IOChannel_unserialize)(eC_IOChannel __this, eC_Class * class_data, void * data);

extern int IOChannel_writeData_vTblID;
uintsize IOChannel_writeData(eC_IOChannel __i, const void * data, uintsize numBytes);
extern eC_Method * method_IOChannel_writeData;


extern eC_Property * property_Platform_char_ptr;
extern eC_Platform (* Platform_from_char_ptr)(const char * c);
extern const char * (* Platform_to_char_ptr)(const eC_Platform platform);

struct eC_Point
{
   int x;
   int y;
};
struct eC_Pointd
{
   double x;
   double y;
};
struct eC_Pointf
{
   float x;
   float y;
};

struct class_members_SerialBuffer
{
   byte * _buffer;
   uintsize count;
   uintsize _size;
   uintsize pos;
};
extern void (* SerialBuffer_free)(eC_SerialBuffer __this);

extern eC_Property * property_SerialBuffer_buffer;
extern void (* SerialBuffer_set_buffer)(const eC_SerialBuffer s, byte * value);
extern byte * (* SerialBuffer_get_buffer)(const eC_SerialBuffer s);

extern eC_Property * property_SerialBuffer_size;
extern void (* SerialBuffer_set_size)(const eC_SerialBuffer s, uint value);
extern uint (* SerialBuffer_get_size)(const eC_SerialBuffer s);

/*struct eC_Size
{
   eC_MinMaxValue w;
   eC_MinMaxValue h;
};*/
struct eC_StaticString
{
   char string[1];
};
struct class_members_ZString
{
   char * _string;
   int len;
   eC_StringAllocType allocType;
   int size;
   int minSize;
   int maxSize;
};
extern void (* ZString_concat)(eC_ZString __this, eC_ZString s);

extern void (* ZString_concatf)(eC_ZString __this, const char * format, ...);

extern void (* ZString_concatn)(eC_ZString __this, eC_ZString s, int l);

extern void (* ZString_concatx)(eC_ZString __this, eC_Class * class_object, const void * object, ...);

extern void (* ZString_copy)(eC_ZString __this, eC_ZString s);

extern void (* ZString_copyString)(eC_ZString __this, const char * value, int newLen);

extern eC_Property * property_ZString_string;
extern void (* ZString_set_string)(const eC_ZString z, const char * value);
extern const char * (* ZString_get_string)(const eC_ZString z);

extern eC_Property * property_ZString_char_ptr;
extern eC_ZString (* ZString_from_char_ptr)(const char * c);
extern const char * (* ZString_to_char_ptr)(const eC_ZString z);

extern eC_Property * property_ZString_String;
extern eC_ZString (* ZString_from_String)(const eC_String string);
extern constString (* ZString_to_String)(const eC_ZString z);

extern void (* eC_changeCh)(char * string, char ch1, char ch2);
extern void (* eC_changeChars)(char * string, const char * chars, char alt);
extern char * (* eC_changeExtension)(const char * string, const char * ext, char * output);
extern void (* eC_checkConsistency)(void);
extern void (* eC_checkMemory)(void);
extern void (* eC_copyBytes)(void * dest, const void * source, uint64 count);
extern void (* eC_copyBytesBy2)(void * dest, const void * source, uint count);
extern void (* eC_copyBytesBy4)(void * dest, const void * source, uint count);
extern char * (* eC_copyString)(const char * string);
extern int (* eC_escapeCString)(eC_String outString, int bufferLen, constString s, eC_EscapeCStringOptions options);
extern void (* eC_fillBytes)(void * area, byte value, uint count);
extern void (* eC_fillBytesBy2)(void * area, uint16 value, uint count);
extern void (* eC_fillBytesBy4)(void * area, uint value, uint count);
extern double (* eC_floatFromString)(const char * string);
extern eC_DesignerBase (* eC_getActiveDesigner)(void);
extern char * (* eC_getExtension)(const char * string, char * output);
extern uint (* eC_getHexValue)(const char ** buffer);
extern char * (* eC_getLastDirectory)(const char * string, char * output);
extern eC_Platform (* eC_getRuntimePlatform)(void);
extern eC_bool (* eC_getString)(const char ** buffer, char * string, int max);
extern int (* eC_getValue)(const char ** buffer);
extern eC_bool (* eC_isPathInsideOf)(const char * path, const char * of);
extern eC_bool (* eC_locateModule)(const char * name, const char * fileName);
extern char * (* eC_makePathRelative)(const char * path, const char * to, char * destination);
extern void (* eC_moveBytes)(void * dest, const void * source, uint count);
extern char * (* eC_pathCat)(char * string, const char * addedPath);
extern char * (* eC_pathCatSlash)(char * string, const char * addedPath);
extern void (* eC_printx)(eC_Class * class_object, const void * object, ...);
extern void (* eC_printBigSize)(char * string, double size, int prec);
extern int (* eC_printBuf)(char * buffer, int maxLen, eC_Class * class_object, const void * object, ...);
extern void (* eC_printLn)(eC_Class * class_object, const void * object, ...);
extern int (* eC_printLnBuf)(char * buffer, int maxLen, eC_Class * class_object, const void * object, ...);
extern char * (* eC_printLnString)(eC_Class * class_object, const void * object, ...);
extern void (* eC_printSize)(char * string, uint64 size, int prec);
extern char * (* eC_printString)(eC_Class * class_object, const void * object, ...);
extern char * (* eC_rSearchString)(const char * buffer, const char * subStr, int maxLen, eC_bool matchCase, eC_bool matchWord);
extern void (* eC_repeatCh)(char * string, int count, char ch);
extern char * (* eC_searchString)(const char * buffer, int start, const char * subStr, eC_bool matchCase, eC_bool matchWord);
extern void (* eC_setActiveDesigner)(eC_DesignerBase designer);
extern eC_bool (* eC_splitArchivePath)(const char * fileName, char * archiveName, const char ** archiveFile);
extern char * (* eC_splitDirectory)(const char * string, char * part, char * rest);
extern eC_bool (* eC_stringLikePattern)(constString string, constString pattern);
extern char * (* eC_stripChars)(eC_String string, constString chars);
extern eC_bool (* eC_stripExtension)(char * string);
extern char * (* eC_stripLastDirectory)(const char * string, char * output);
extern char * (* eC_stripQuotes)(const char * string, char * output);
extern int (* eC_tokenize)(char * string, int maxTokens, char * tokens[], eC_BackSlashEscaping esc);
extern int (* eC_tokenizeWith)(char * string, int maxTokens, char * tokens[], const char * tokenizers, eC_bool escapeBackSlashes);
extern char * (* eC_trimLSpaces)(const char * string, char * output);
extern char * (* eC_trimRSpaces)(const char * string, char * output);
extern int (* eC_unescapeCString)(char * d, const char * s, int len);
extern int (* eC_unescapeCStringLoose)(char * d, const char * s, int len);
extern void (* eC_eSystem_LockMem)(void);
extern void (* eC_eSystem_UnlockMem)(void);
extern eC_bool (* eC_ishexdigit)(char x);
extern uint (* eC_log2i)(uint number);
extern void (* eC_memswap)(byte * a, byte * b, uint size);
extern uint (* eC_pow2i)(uint number);
extern void (* eC_queryMemInfo)(char * string);
extern char * (* eC_strchrmax)(const char * s, int c, int max);
typedef eC_Instance eC_Container;
typedef eC_Container eC_Array;
typedef eC_Container eC_CustomAVLTree;
typedef eC_CustomAVLTree eC_Map;
int fstrcmp(const char *, const char *);

int strcmpi(const char *, const char *);

int strnicmp(const char *, const char *, uintsize n);

typedef intptr_t intptr;
typedef ssize_t intsize;
typedef eC_Map template_Map_String_FieldValue;
typedef eC_Array template_Array_FieldValue;
typedef eC_Map template_Map_String_JSONTypeOptions;

extern int class_onCompare_vTblID;
int _onCompare(eC_Class * __c, any_object __i, any_object object);
extern eC_Method * method_class_onCompare;

extern int class_onCopy_vTblID;
void _onCopy(eC_Class * __c, any_object __i, any_object newData);
extern eC_Method * method_class_onCopy;

extern int class_onDisplay_vTblID;
void _onDisplay(eC_Class * __c, any_object __i, eC_Instance surface, int x, int y, int width, void * fieldData, int alignment, uint displayFlags);
extern eC_Method * method_class_onDisplay;

extern int class_onEdit_vTblID;
eC_Instance _onEdit(eC_Class * __c, any_object __i, eC_Instance dataBox, eC_Instance obsolete, int x, int y, int w, int h, void * userData);
extern eC_Method * method_class_onEdit;

extern int class_onFree_vTblID;
void _onFree(eC_Class * __c, any_object __i);
extern eC_Method * method_class_onFree;

extern int class_onGetDataFromString_vTblID;
eC_bool _onGetDataFromString(eC_Class * __c, any_object __i, const char * string);
extern eC_Method * method_class_onGetDataFromString;

extern int class_onGetString_vTblID;
const char * _onGetString(eC_Class * __c, any_object __i, char * tempString, void * reserved, eC_ObjectNotationType * onType);
extern eC_Method * method_class_onGetString;

extern int class_onSaveEdit_vTblID;
eC_bool _onSaveEdit(eC_Class * __c, any_object __i, eC_Instance window, void * object);
extern eC_Method * method_class_onSaveEdit;

extern int class_onSerialize_vTblID;
void _onSerialize(eC_Class * __c, any_object __i, eC_IOChannel channel);
extern eC_Method * method_class_onSerialize;

extern int class_onUnserialize_vTblID;
void _onUnserialize(eC_Class * __c, any_object __i, eC_IOChannel channel);
extern eC_Method * method_class_onUnserialize;

extern int class_onCompare_vTblID;
int Instance_onCompare(eC_Class * __c, eC_Instance __i, any_object object);

extern int class_onCopy_vTblID;
void Instance_onCopy(eC_Class * __c, eC_Instance __i, any_object newData);

extern int class_onDisplay_vTblID;
void Instance_onDisplay(eC_Class * __c, eC_Instance __i, eC_Instance surface, int x, int y, int width, void * fieldData, int alignment, uint displayFlags);

extern int class_onEdit_vTblID;
eC_Instance Instance_onEdit(eC_Class * __c, eC_Instance __i, eC_Instance dataBox, eC_Instance obsolete, int x, int y, int w, int h, void * userData);

extern int class_onFree_vTblID;
void Instance_onFree(eC_Class * __c, eC_Instance __i);

extern int class_onGetDataFromString_vTblID;
eC_bool Instance_onGetDataFromString(eC_Class * __c, eC_Instance __i, const char * string);

extern int class_onGetString_vTblID;
const char * Instance_onGetString(eC_Class * __c, eC_Instance __i, char * tempString, void * reserved, eC_ObjectNotationType * onType);

extern int class_onSaveEdit_vTblID;
eC_bool Instance_onSaveEdit(eC_Class * __c, eC_Instance __i, eC_Instance window, void * object);

extern int class_onSerialize_vTblID;
void Instance_onSerialize(eC_Class * __c, eC_Instance __i, eC_IOChannel channel);

extern int class_onUnserialize_vTblID;
void Instance_onUnserialize(eC_Class * __c, eC_Instance __i, eC_IOChannel channel);

extern double (* double_inf)(void);

extern double (* double_nan)(void);

extern eC_Property * property_double_isNan;
extern eC_bool (* double_get_isNan)(const double d);

extern eC_Property * property_double_isInf;
extern eC_bool (* double_get_isInf)(const double d);

extern eC_Property * property_double_signBit;
extern int (* double_get_signBit)(const double d);

extern float (* float_inf)(void);

extern float (* float_nan)(void);

extern eC_Property * property_float_isNan;
extern eC_bool (* float_get_isNan)(const float f);

extern eC_Property * property_float_isInf;
extern eC_bool (* float_get_isInf)(const float f);

extern eC_Property * property_float_signBit;
extern int (* float_get_signBit)(const float f);

#define jsonIndentWidth 3

typedef uint32_t unichar;
typedef uint64_t tparam_Container_T;
struct class_members_Array
{
   tparam_Container_T * array;
   uint count;
   uint minAllocSize;
};
typedef int eC_FieldTypeEx;
typedef struct eC_FieldValue eC_FieldValue;
typedef int eC_FieldType;
enum
{
   FieldType_integer = 0x1,
   FieldType_real = 0x2,
   FieldType_text = 0x3,
   FieldType_blob = 0x4,
   FieldType_nil = 0x5,
   FieldType_array = 0x6,
   FieldType_map = 0x7
};

typedef int eC_FieldValueFormat;
enum
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
#define FIELDTYPEEX_mustFree_SHIFT                       3
#define FIELDTYPEEX_mustFree_MASK                        0x8
#define FIELDTYPEEX_format_SHIFT                         4
#define FIELDTYPEEX_format_MASK                          0xF0
#define FIELDTYPEEX_isUnsigned_SHIFT                     8
#define FIELDTYPEEX_isUnsigned_MASK                      0x100
#define FIELDTYPEEX_isDateTime_SHIFT                     9
#define FIELDTYPEEX_isDateTime_MASK                      0x200


struct eC_FieldValue
{
   eC_FieldTypeEx type;
   union
   {
      int64 i;
      double r;
      eC_String s;
      void * b;
      eC_Array a;
      eC_Map m;
   };
};
extern int (* FieldValue_compareInt)(eC_FieldValue * __this, eC_FieldValue * other);

extern int (* FieldValue_compareReal)(eC_FieldValue * __this, eC_FieldValue * other);

extern int (* FieldValue_compareText)(eC_FieldValue * __this, eC_FieldValue * other);

extern eC_String (* FieldValue_formatArray)(eC_FieldValue * __this, char * tempString, void * fieldData, eC_ObjectNotationType * onType);

extern eC_String (* FieldValue_formatFloat)(eC_FieldValue * __this, char * stringOutput, eC_bool fixDot);

extern eC_String (* FieldValue_formatInteger)(eC_FieldValue * __this, char * stringOutput);

extern eC_String (* FieldValue_formatMap)(eC_FieldValue * __this, char * tempString, void * fieldData, eC_ObjectNotationType * onType);

extern eC_bool (* FieldValue_getArrayOrMap)(const char * string, eC_Class * destClass, void ** destination);

extern eC_String (* FieldValue_stringify)(eC_FieldValue * __this);

typedef uint64_t tparam_AVLNode_T;
typedef uint64_t tparam_CustomAVLTree_BT;
typedef uint64_t tparam_LinkElement_T;
typedef uint64_t tparam_LinkList_LT;
typedef uint64_t tparam_MapNode_V;
struct class_members_HashMap
{
   byte __eCPrivateData0[8];
   eC_bool noRemResize;
   byte __ecere_padding[4];
};
typedef struct eC_Item eC_Item;
struct eC_Item
{
   eC_Item * prev;
   eC_Item * next;
};
struct eC_IteratorPointer
{
};
typedef struct eC_IteratorPointer eC_IteratorPointer;
struct eC_Iterator
{
   eC_Container container;
   eC_IteratorPointer * pointer;
};
struct eC_LinkElement
{
   tparam_LinkElement_T prev;
   tparam_LinkElement_T next;
};
typedef struct eC_NamedItem eC_NamedItem;
struct eC_NamedItem
{
   eC_NamedItem * prev;
   eC_NamedItem * next;
   char * name;
};
typedef struct eC_NamedLink eC_NamedLink;
struct eC_NamedLink
{
   eC_NamedLink * prev;
   eC_NamedLink * next;
   char * name;
   void * data;
};
typedef struct eC_NamedLink64 eC_NamedLink64;
struct eC_NamedLink64
{
   eC_NamedLink64 * prev;
   eC_NamedLink64 * next;
   char * name;
   int64 data;
};
typedef struct eC_OldLink eC_OldLink;
struct eC_OldLink
{
   eC_OldLink * prev;
   eC_OldLink * next;
   void * data;
};
typedef struct eC_StringBTNode eC_StringBTNode;
struct eC_StringBTNode
{
   eC_String key;
   eC_StringBTNode * parent;
   eC_StringBTNode * left;
   eC_StringBTNode * right;
   int depth;
};
typedef struct eC_AVLNode eC_AVLNode;
typedef struct eC_BuiltInContainer eC_BuiltInContainer;
typedef eC_Container eC_HashMap;
typedef struct eC_Iterator eC_Iterator;
typedef struct eC_LinkElement eC_LinkElement;
typedef eC_Container eC_LinkList;
typedef int eC_TreePrintStyle;
enum
{
   TreePrintStyle_inOrder = 0x0,
   TreePrintStyle_postOrder = 0x1,
   TreePrintStyle_preOrder = 0x2,
   TreePrintStyle_depthOrder = 0x3
};

typedef eC_CustomAVLTree eC_AVLTree;
typedef struct eC_HashMapIterator eC_HashMapIterator;
typedef eC_Container eC_HashTable;
typedef struct eC_Link eC_Link;
typedef eC_LinkList eC_List;
typedef struct eC_ListItem eC_ListItem;
typedef struct eC_MapIterator eC_MapIterator;
typedef struct eC_MapNode eC_MapNode;
typedef struct eC_StringBinaryTree eC_StringBinaryTree;
typedef uint64_t tparam_HashMapIterator_KT;
typedef uint64_t tparam_Container_D;
typedef uint64_t tparam_Iterator_T;
typedef uint64_t tparam_MapIterator_KT;
typedef uint64_t tparam_MapNode_KT;
typedef uint64_t tparam_HashMapIterator_VT;
typedef uint64_t tparam_MapIterator_V;
typedef uint64_t tparam_Iterator_IT;
typedef uint64_t tparam_Container_I;
struct eC_AVLNode
{
   byte __eCPrivateData0[32];
   tparam_AVLNode_T key;
};
extern eC_AVLNode * (* AVLNode_find)(eC_AVLNode * __this, eC_Class * Tclass, tparam_AVLNode_T key);

extern eC_Property * property_AVLNode_prev;
extern eC_AVLNode * (* AVLNode_get_prev)(const eC_AVLNode * a);

extern eC_Property * property_AVLNode_next;
extern eC_AVLNode * (* AVLNode_get_next)(const eC_AVLNode * a);

extern eC_Property * property_AVLNode_minimum;
extern eC_AVLNode * (* AVLNode_get_minimum)(const eC_AVLNode * a);

extern eC_Property * property_AVLNode_maximum;
extern eC_AVLNode * (* AVLNode_get_maximum)(const eC_AVLNode * a);

extern eC_Property * property_AVLNode_count;
extern int (* AVLNode_get_count)(const eC_AVLNode * a);

extern eC_Property * property_AVLNode_depthProp;
extern int (* AVLNode_get_depthProp)(const eC_AVLNode * a);

extern eC_Property * property_Array_size;
extern void (* Array_set_size)(const eC_Array a, uint value);
extern uint (* Array_get_size)(const eC_Array a);

extern eC_Property * property_Array_minAllocSize;
extern void (* Array_set_minAllocSize)(const eC_Array a, uint value);
extern uint (* Array_get_minAllocSize)(const eC_Array a);

extern eC_BTNode * (* BTNode_findPrefix)(eC_BTNode * __this, const char * key);

extern eC_BTNode * (* BTNode_findString)(eC_BTNode * __this, const char * key);

extern eC_Property * property_BTNode_prev;
extern eC_BTNode * (* BTNode_get_prev)(const eC_BTNode * b);

extern eC_Property * property_BTNode_next;
extern eC_BTNode * (* BTNode_get_next)(const eC_BTNode * b);

extern eC_Property * property_BTNode_minimum;
extern eC_BTNode * (* BTNode_get_minimum)(const eC_BTNode * b);

extern eC_Property * property_BTNode_maximum;
extern eC_BTNode * (* BTNode_get_maximum)(const eC_BTNode * b);

extern eC_Property * property_BTNode_count;
extern int (* BTNode_get_count)(const eC_BTNode * b);

extern eC_Property * property_BTNode_depthProp;
extern int (* BTNode_get_depthProp)(const eC_BTNode * b);

extern eC_bool (* BinaryTree_add)(eC_BinaryTree * __this, eC_BTNode * node);

extern eC_bool (* BinaryTree_check)(eC_BinaryTree * __this);

extern int (* BinaryTree_compareInt)(eC_BinaryTree * __this, uintptr a, uintptr b);

extern int (* BinaryTree_compareString)(eC_BinaryTree * __this, const char * a, const char * b);

extern void (* BinaryTree_delete)(eC_BinaryTree * __this, eC_BTNode * node);

extern eC_BTNode * (* BinaryTree_find)(eC_BinaryTree * __this, uintptr key);

extern eC_BTNode * (* BinaryTree_findAll)(eC_BinaryTree * __this, uintptr key);

extern eC_BTNode * (* BinaryTree_findPrefix)(eC_BinaryTree * __this, const char * key);

extern eC_BTNode * (* BinaryTree_findString)(eC_BinaryTree * __this, const char * key);

extern void (* BinaryTree_free)(eC_BinaryTree * __this);

extern void (* BinaryTree_freeString)(char * string);

extern char * (* BinaryTree_print)(eC_BinaryTree * __this, char * output, eC_TreePrintStyle tps);

extern void (* BinaryTree_remove)(eC_BinaryTree * __this, eC_BTNode * node);

extern eC_Property * property_BinaryTree_first;
extern eC_BTNode * (* BinaryTree_get_first)(const eC_BinaryTree * b);

extern eC_Property * property_BinaryTree_last;
extern eC_BTNode * (* BinaryTree_get_last)(const eC_BinaryTree * b);

struct eC_BuiltInContainer
{
   void ** _vTbl;
   eC_Class * _class;
   int _refCount;
   void * data;
   int count;
   eC_Class * type;
};
extern int BuiltInContainer_add_vTblID;
eC_IteratorPointer * BuiltInContainer_add(eC_BuiltInContainer * __i, uint64 value);
extern eC_Method * method_BuiltInContainer_add;

extern int BuiltInContainer_copy_vTblID;
void BuiltInContainer_copy(eC_BuiltInContainer * __i, eC_Container source);
extern eC_Method * method_BuiltInContainer_copy;

extern int BuiltInContainer_delete_vTblID;
void BuiltInContainer_delete(eC_BuiltInContainer * __i, eC_IteratorPointer * it);
extern eC_Method * method_BuiltInContainer_delete;

extern int BuiltInContainer_find_vTblID;
eC_IteratorPointer * BuiltInContainer_find(eC_BuiltInContainer * __i, uint64 value);
extern eC_Method * method_BuiltInContainer_find;

extern int BuiltInContainer_free_vTblID;
void BuiltInContainer_free(eC_BuiltInContainer * __i);
extern eC_Method * method_BuiltInContainer_free;

extern int BuiltInContainer_freeIterator_vTblID;
void BuiltInContainer_freeIterator(eC_BuiltInContainer * __i, eC_IteratorPointer * it);
extern eC_Method * method_BuiltInContainer_freeIterator;

extern int BuiltInContainer_getAtPosition_vTblID;
eC_IteratorPointer * BuiltInContainer_getAtPosition(eC_BuiltInContainer * __i, const uint64 pos, eC_bool create);
extern eC_Method * method_BuiltInContainer_getAtPosition;

extern int BuiltInContainer_getCount_vTblID;
int BuiltInContainer_getCount(eC_BuiltInContainer * __i);
extern eC_Method * method_BuiltInContainer_getCount;

extern int BuiltInContainer_getData_vTblID;
uint64 BuiltInContainer_getData(eC_BuiltInContainer * __i, eC_IteratorPointer * pointer);
extern eC_Method * method_BuiltInContainer_getData;

extern int BuiltInContainer_getFirst_vTblID;
eC_IteratorPointer * BuiltInContainer_getFirst(eC_BuiltInContainer * __i);
extern eC_Method * method_BuiltInContainer_getFirst;

extern int BuiltInContainer_getLast_vTblID;
eC_IteratorPointer * BuiltInContainer_getLast(eC_BuiltInContainer * __i);
extern eC_Method * method_BuiltInContainer_getLast;

extern int BuiltInContainer_getNext_vTblID;
eC_IteratorPointer * BuiltInContainer_getNext(eC_BuiltInContainer * __i, eC_IteratorPointer * pointer);
extern eC_Method * method_BuiltInContainer_getNext;

extern int BuiltInContainer_getPrev_vTblID;
eC_IteratorPointer * BuiltInContainer_getPrev(eC_BuiltInContainer * __i, eC_IteratorPointer * pointer);
extern eC_Method * method_BuiltInContainer_getPrev;

extern int BuiltInContainer_insert_vTblID;
eC_IteratorPointer * BuiltInContainer_insert(eC_BuiltInContainer * __i, eC_IteratorPointer * after, uint64 value);
extern eC_Method * method_BuiltInContainer_insert;

extern int BuiltInContainer_move_vTblID;
void BuiltInContainer_move(eC_BuiltInContainer * __i, eC_IteratorPointer * it, eC_IteratorPointer * after);
extern eC_Method * method_BuiltInContainer_move;

extern int BuiltInContainer_remove_vTblID;
void BuiltInContainer_remove(eC_BuiltInContainer * __i, eC_IteratorPointer * it);
extern eC_Method * method_BuiltInContainer_remove;

extern int BuiltInContainer_removeAll_vTblID;
void BuiltInContainer_removeAll(eC_BuiltInContainer * __i);
extern eC_Method * method_BuiltInContainer_removeAll;

extern int BuiltInContainer_setData_vTblID;
eC_bool BuiltInContainer_setData(eC_BuiltInContainer * __i, eC_IteratorPointer * pointer, uint64 data);
extern eC_Method * method_BuiltInContainer_setData;

extern int BuiltInContainer_sort_vTblID;
void BuiltInContainer_sort(eC_BuiltInContainer * __i, eC_bool ascending);
extern eC_Method * method_BuiltInContainer_sort;

extern eC_Property * property_BuiltInContainer_Container;
extern eC_Container (* BuiltInContainer_to_Container)(const eC_BuiltInContainer * b);

extern int Container_add_vTblID;
eC_IteratorPointer * Container_add(eC_Container __i, tparam_Container_T value);
extern eC_Method * method_Container_add;

extern int Container_copy_vTblID;
void Container_copy(eC_Container __i, eC_Container source);
extern eC_Method * method_Container_copy;

extern int Container_delete_vTblID;
void Container_delete(eC_Container __i, eC_IteratorPointer * i);
extern eC_Method * method_Container_delete;

extern int Container_find_vTblID;
eC_IteratorPointer * Container_find(eC_Container __i, tparam_Container_D value);
extern eC_Method * method_Container_find;

extern int Container_free_vTblID;
void Container_free(eC_Container __i);
extern eC_Method * method_Container_free;

extern int Container_freeIterator_vTblID;
void Container_freeIterator(eC_Container __i, eC_IteratorPointer * it);
extern eC_Method * method_Container_freeIterator;

extern int Container_getAtPosition_vTblID;
eC_IteratorPointer * Container_getAtPosition(eC_Container __i, tparam_Container_I pos, eC_bool create, eC_bool * justAdded);
extern eC_Method * method_Container_getAtPosition;

extern int Container_getCount_vTblID;
int Container_getCount(eC_Container __i);
extern eC_Method * method_Container_getCount;

extern int Container_getData_vTblID;
tparam_Container_D Container_getData(eC_Container __i, eC_IteratorPointer * pointer);
extern eC_Method * method_Container_getData;

extern int Container_getFirst_vTblID;
eC_IteratorPointer * Container_getFirst(eC_Container __i);
extern eC_Method * method_Container_getFirst;

extern int Container_getLast_vTblID;
eC_IteratorPointer * Container_getLast(eC_Container __i);
extern eC_Method * method_Container_getLast;

extern int Container_getNext_vTblID;
eC_IteratorPointer * Container_getNext(eC_Container __i, eC_IteratorPointer * pointer);
extern eC_Method * method_Container_getNext;

extern int Container_getPrev_vTblID;
eC_IteratorPointer * Container_getPrev(eC_Container __i, eC_IteratorPointer * pointer);
extern eC_Method * method_Container_getPrev;

extern int Container_insert_vTblID;
eC_IteratorPointer * Container_insert(eC_Container __i, eC_IteratorPointer * after, tparam_Container_T value);
extern eC_Method * method_Container_insert;

extern int Container_move_vTblID;
void Container_move(eC_Container __i, eC_IteratorPointer * it, eC_IteratorPointer * after);
extern eC_Method * method_Container_move;

extern int Container_remove_vTblID;
void Container_remove(eC_Container __i, eC_IteratorPointer * it);
extern eC_Method * method_Container_remove;

extern int Container_removeAll_vTblID;
void Container_removeAll(eC_Container __i);
extern eC_Method * method_Container_removeAll;

extern int Container_setData_vTblID;
eC_bool Container_setData(eC_Container __i, eC_IteratorPointer * pointer, tparam_Container_D data);
extern eC_Method * method_Container_setData;

extern int Container_sort_vTblID;
void Container_sort(eC_Container __i, eC_bool ascending);
extern eC_Method * method_Container_sort;

extern eC_bool (* Container_takeOut)(eC_Container __this, tparam_Container_D d);

extern eC_Property * property_Container_copySrc;
extern void (* Container_set_copySrc)(const eC_Container c, eC_Container value);

extern eC_Property * property_Container_firstIterator;
extern void (* Container_get_firstIterator)(const eC_Container c, eC_Iterator * value);

extern eC_Property * property_Container_lastIterator;
extern void (* Container_get_lastIterator)(const eC_Container c, eC_Iterator * value);

struct class_members_CustomAVLTree
{
   tparam_CustomAVLTree_BT root;
   int count;
};
extern eC_bool (* CustomAVLTree_check)(eC_CustomAVLTree __this);

extern void (* CustomAVLTree_freeKey)(eC_CustomAVLTree __this, eC_AVLNode * item);

extern void (* HashMap_removeIterating)(eC_HashMap __this, eC_IteratorPointer * it);

extern void (* HashMap_resize)(eC_HashMap __this, eC_IteratorPointer * movedEntry);

extern eC_Property * property_HashMap_count;
extern int (* HashMap_get_count)(const eC_HashMap h);

extern eC_Property * property_HashMap_initSize;
extern void (* HashMap_set_initSize)(const eC_HashMap h, int value);

struct eC_HashMapIterator
{
   eC_Container container;
   eC_IteratorPointer * pointer;
};
extern eC_Property * property_HashMapIterator_map;
extern void (* HashMapIterator_set_map)(const eC_HashMapIterator * h, eC_HashMap value);
extern eC_HashMap (* HashMapIterator_get_map)(const eC_HashMapIterator * h);

extern eC_Property * property_HashMapIterator_key;
extern tparam_HashMapIterator_KT (* HashMapIterator_get_key)(const eC_HashMapIterator * h);

extern eC_Property * property_HashMapIterator_value;
extern void (* HashMapIterator_set_value)(const eC_HashMapIterator * h, tparam_HashMapIterator_VT value);
extern tparam_HashMapIterator_VT (* HashMapIterator_get_value)(const eC_HashMapIterator * h);

extern eC_Property * property_HashTable_initSize;
extern void (* HashTable_set_initSize)(const eC_HashTable h, int value);

extern void (* Item_copy)(eC_Item * __this, eC_Item * src, int size);

extern eC_bool (* Iterator_find)(eC_Iterator * __this, tparam_Iterator_T value);

extern void (* Iterator_free)(eC_Iterator * __this);

extern tparam_Iterator_T (* Iterator_getData)(eC_Iterator * __this);

extern eC_bool (* Iterator_index)(eC_Iterator * __this, tparam_Iterator_IT index, eC_bool create);

extern eC_bool (* Iterator_next)(eC_Iterator * __this);

extern eC_bool (* Iterator_prev)(eC_Iterator * __this);

extern void (* Iterator_remove)(eC_Iterator * __this);

extern eC_bool (* Iterator_setData)(eC_Iterator * __this, tparam_Iterator_T value);

extern eC_Property * property_Iterator_data;
extern void (* Iterator_set_data)(const eC_Iterator * i, tparam_Iterator_T value);
extern tparam_Iterator_T (* Iterator_get_data)(const eC_Iterator * i);

struct eC_Link
{
   union
   {
      eC_LinkElement link;
      struct
      {
         eC_ListItem * prev;
         eC_ListItem * next;
      };
   };
   uint64 data;
};
struct class_members_LinkList
{
   tparam_LinkList_LT first;
   tparam_LinkList_LT last;
   int count;
};
extern void (* LinkList__Sort)(eC_LinkList __this, eC_bool ascending, eC_LinkList * lists);

struct eC_ListItem
{
   union
   {
      eC_LinkElement link;
      struct
      {
         eC_ListItem * prev;
         eC_ListItem * next;
      };
   };
};
extern eC_Property * property_Map_mapSrc;
extern void (* Map_set_mapSrc)(const eC_Map m, eC_Map value);

struct eC_MapIterator
{
   eC_Container container;
   eC_IteratorPointer * pointer;
};
extern eC_Property * property_MapIterator_map;
extern void (* MapIterator_set_map)(const eC_MapIterator * m, eC_Map value);
extern eC_Map (* MapIterator_get_map)(const eC_MapIterator * m);

extern eC_Property * property_MapIterator_key;
extern tparam_MapIterator_KT (* MapIterator_get_key)(const eC_MapIterator * m);

extern eC_Property * property_MapIterator_value;
extern void (* MapIterator_set_value)(const eC_MapIterator * m, tparam_MapIterator_V value);
extern tparam_MapIterator_V (* MapIterator_get_value)(const eC_MapIterator * m);

struct eC_MapNode
{
   byte __eCPrivateData0[32];
   tparam_AVLNode_T key;
   tparam_MapNode_V value;
};
extern eC_Property * property_MapNode_key;
extern void (* MapNode_set_key)(const eC_MapNode * m, tparam_MapNode_KT value);
extern tparam_MapNode_KT (* MapNode_get_key)(const eC_MapNode * m);

extern eC_Property * property_MapNode_value;
extern void (* MapNode_set_value)(const eC_MapNode * m, tparam_MapNode_V value);
extern tparam_MapNode_V (* MapNode_get_value)(const eC_MapNode * m);

extern eC_Property * property_MapNode_prev;
extern eC_MapNode * (* MapNode_get_prev)(const eC_MapNode * m);

extern eC_Property * property_MapNode_next;
extern eC_MapNode * (* MapNode_get_next)(const eC_MapNode * m);

extern eC_Property * property_MapNode_minimum;
extern eC_MapNode * (* MapNode_get_minimum)(const eC_MapNode * m);

extern eC_Property * property_MapNode_maximum;
extern eC_MapNode * (* MapNode_get_maximum)(const eC_MapNode * m);

extern void (* OldLink_free)(eC_OldLink * __this);

extern void (* OldList_add)(eC_OldList * __this, void * item);

extern eC_bool (* OldList_addName)(eC_OldList * __this, void * item);

extern void (* OldList_clear)(eC_OldList * __this);

extern void (* OldList_copy)(eC_OldList * __this, eC_OldList * src, int size, void (* copy)(void * dest, void * src));

extern void (* OldList_delete)(eC_OldList * __this, void * item);

extern eC_OldLink * (* OldList_findLink)(eC_OldList * __this, void * data);

extern void * (* OldList_findName)(eC_OldList * __this, const char * name, eC_bool warn);

extern void * (* OldList_findNamedLink)(eC_OldList * __this, const char * name, eC_bool warn);

extern void (* OldList_free)(eC_OldList * __this, void (* freeFn)(void *));

extern eC_bool (* OldList_insert)(eC_OldList * __this, void * prevItem, void * item);

extern void (* OldList_move)(eC_OldList * __this, void * item, void * prevItem);

extern eC_bool (* OldList_placeName)(eC_OldList * __this, const char * name, void ** place);

extern void (* OldList_remove)(eC_OldList * __this, void * item);

extern void (* OldList_removeAll)(eC_OldList * __this, void (* freeFn)(void *));

extern void (* OldList_sort)(eC_OldList * __this, int (* compare)(void *, void *, void *), void * data);

extern void (* OldList_swap)(eC_OldList * __this, void * item1, void * item2);

struct eC_StringBinaryTree
{
   eC_BTNode * root;
   int count;
   int (* CompareKey)(eC_BinaryTree * tree, uintptr a, uintptr b);
   void (* FreeKey)(void * key);
};
extern void (* eC_qsortr)(void * base, uintsize nel, uintsize width, int (* compare)(void * arg, const void * a, const void * b), void * arg);
extern void (* eC_qsortrx)(void * base, uintsize nel, uintsize width, int (* compare)(void * arg, const void * a, const void * b), int (* optCompareArgLast)(const void * a, const void * b, void * arg), void * arg, eC_bool deref, eC_bool ascending);
typedef struct eC_Date eC_Date;
typedef struct eC_DateTime eC_DateTime;
typedef int eC_DayOfTheWeek;
enum
{
   DayOfTheWeek_sunday = 0x0,
   DayOfTheWeek_monday = 0x1,
   DayOfTheWeek_tuesday = 0x2,
   DayOfTheWeek_wednesday = 0x3,
   DayOfTheWeek_thursday = 0x4,
   DayOfTheWeek_friday = 0x5,
   DayOfTheWeek_saturday = 0x6
};

typedef int eC_Month;
enum
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

typedef double eC_Time;
typedef int64 eC_SecSince1970;
typedef uint eC_TimeStamp32;
struct eC_Date
{
   int year;
   eC_Month month;
   int day;
};
extern const char * (* Date_onGetStringEn)(eC_Date * __this, char * stringOutput, void * fieldData, eC_ObjectNotationType * onType);

extern eC_Property * property_Date_dayOfTheWeek;
extern eC_DayOfTheWeek (* Date_get_dayOfTheWeek)(const eC_Date * d);

struct eC_DateTime
{
   int year;
   eC_Month month;
   int day;
   int hour;
   int minute;
   int second;
   eC_DayOfTheWeek dayOfTheWeek;
   int dayInTheYear;
};
extern eC_bool (* DateTime_fixDayOfYear)(eC_DateTime * __this);

extern eC_bool (* DateTime_getLocalTime)(eC_DateTime * __this);

extern eC_Property * property_DateTime_global;
extern void (* DateTime_set_global)(const eC_DateTime * d, const eC_DateTime * value);
extern void (* DateTime_get_global)(const eC_DateTime * d, eC_DateTime * value);

extern eC_Property * property_DateTime_local;
extern void (* DateTime_set_local)(const eC_DateTime * d, const eC_DateTime * value);
extern void (* DateTime_get_local)(const eC_DateTime * d, eC_DateTime * value);

extern eC_Property * property_DateTime_daysSince1970;
extern int64 (* DateTime_get_daysSince1970)(const eC_DateTime * d);

extern eC_Property * property_DateTime_SecSince1970;
extern void (* DateTime_from_SecSince1970)(const eC_DateTime * d, eC_SecSince1970 value);
extern eC_SecSince1970 (* DateTime_to_SecSince1970)(const eC_DateTime * d);

extern eC_Property * property_DateTime_Date;
extern void (* DateTime_from_Date)(const eC_DateTime * d, const eC_Date * value);
extern void (* DateTime_to_Date)(const eC_DateTime * d, eC_Date * value);

extern int (* Month_getNumDays)(eC_Month __this, int year);

extern eC_Property * property_SecSince1970_global;
extern eC_SecSince1970 (* SecSince1970_get_global)(const int64 s);

extern eC_Property * property_SecSince1970_local;
extern eC_SecSince1970 (* SecSince1970_get_local)(const int64 s);



extern int (* eC_getRandom)(int lo, int hi);
extern eC_Time (* eC_getTime)(void);
extern void (* eC_randomSeed)(uint seed);
extern void (* eC___sleep)(eC_Time seconds);
typedef uint32 eC_FileAttribs;
struct eC_FileStats
{
   eC_FileAttribs attribs;
   uint64 size;
   eC_SecSince1970 accessed;
   eC_SecSince1970 modified;
   eC_SecSince1970 created;
};
typedef eC_Instance eC_Archive;
typedef int eC_ArchiveAddMode;
enum
{
   ArchiveAddMode_replace = 0x0,
   ArchiveAddMode_refresh = 0x1,
   ArchiveAddMode_update = 0x2,
   ArchiveAddMode_readOnlyDir = 0x3
};

typedef eC_Instance eC_ArchiveDir;
typedef uint32 eC_ArchiveOpenFlags;
typedef eC_IOChannel eC_File;
typedef eC_File eC_BufferedFile;
typedef eC_File eC_DualPipe;
typedef uint32 eC_ErrorCode;
typedef int eC_ErrorLevel;
enum
{
   ErrorLevel_veryFatal = 0x0,
   ErrorLevel_fatal = 0x1,
   ErrorLevel_major = 0x2,
   ErrorLevel_minor = 0x3
};

typedef uint32 eC_FileChange;
typedef struct eC_FileListing eC_FileListing;
typedef int eC_FileLock;
enum
{
   FileLock_unlocked = 0x0,
   FileLock_shared = 0x1,
   FileLock_exclusive = 0x2
};

typedef eC_Instance eC_FileMonitor;
typedef int eC_FileOpenMode;
enum
{
   FileOpenMode_read = 0x1,
   FileOpenMode_write = 0x2,
   FileOpenMode_append = 0x3,
   FileOpenMode_readWrite = 0x4,
   FileOpenMode_writeRead = 0x5,
   FileOpenMode_appendRead = 0x6
};

typedef int eC_FileSeekMode;
enum
{
   FileSeekMode_start = 0x0,
   FileSeekMode_current = 0x1,
   FileSeekMode_end = 0x2
};

typedef uint eC_FileSize;
typedef uint64 eC_FileSize64;
typedef struct eC_FileStats eC_FileStats;
typedef int eC_LoggingMode;
enum
{
   LoggingMode_noLogging = 0x0,
   LoggingMode_stdOut = 0x1,
   LoggingMode_stdErr = 0x2,
   LoggingMode_debug = 0x3,
   LoggingMode_logFile = 0x4,
   LoggingMode_msgBox = 0x5,
   LoggingMode_buffer = 0x6
};

typedef uint32 eC_MoveFileOptions;
typedef uint32 eC_PipeOpenMode;
typedef eC_File eC_TempFile;
static const eC_ErrorLevel AllErrors;

typedef eC_File eC_ConsoleFile;
typedef eC_ErrorCode eC_GuiErrorCode;
enum
{
   GuiErrorCode_driverNotSupported = 0x101,
   GuiErrorCode_windowCreationFailed = 0x102,
   GuiErrorCode_graphicsLoadingFailed = 0x103,
   GuiErrorCode_modeSwitchFailed = 0x104
};

typedef eC_ErrorCode eC_SysErrorCode;
enum
{
   SysErrorCode_allocationFailed = 0x1001,
   SysErrorCode_nameInexistant = 0x1002,
   SysErrorCode_nameExists = 0x1003,
   SysErrorCode_missingLibrary = 0x1004,
   SysErrorCode_fileNotFound = 0x3005,
   SysErrorCode_writeFailed = 0x2006
};

extern int Archive_clear_vTblID;
eC_bool Archive_clear(eC_Archive __i);
extern eC_Method * method_Archive_clear;

extern int Archive_fileExists_vTblID;
eC_FileAttribs Archive_fileExists(eC_Archive __i, const char * fileName);
extern eC_Method * method_Archive_fileExists;

extern int Archive_fileOpen_vTblID;
eC_File Archive_fileOpen(eC_Archive __i, const char * fileName);
extern eC_Method * method_Archive_fileOpen;

extern int Archive_fileOpenAtPosition_vTblID;
eC_File Archive_fileOpenAtPosition(eC_Archive __i, uint position);
extern eC_Method * method_Archive_fileOpenAtPosition;

extern int Archive_fileOpenCompressed_vTblID;
eC_File Archive_fileOpenCompressed(eC_Archive __i, const char * fileName, eC_bool * isCompressed, uint64 * ucSize);
extern eC_Method * method_Archive_fileOpenCompressed;

extern int Archive_openDirectory_vTblID;
eC_ArchiveDir Archive_openDirectory(eC_Archive __i, const char * name, eC_FileStats * stats, eC_ArchiveAddMode addMode);
extern eC_Method * method_Archive_openDirectory;

extern int Archive_setBufferRead_vTblID;
void Archive_setBufferRead(eC_Archive __i, uint bufferRead);
extern eC_Method * method_Archive_setBufferRead;

extern int Archive_setBufferSize_vTblID;
void Archive_setBufferSize(eC_Archive __i, uint bufferSize);
extern eC_Method * method_Archive_setBufferSize;

extern eC_Property * property_Archive_totalSize;
extern void (* Archive_set_totalSize)(const eC_Archive a, eC_FileSize value);
extern eC_FileSize (* Archive_get_totalSize)(const eC_Archive a);

extern eC_Property * property_Archive_bufferSize;
extern void (* Archive_set_bufferSize)(const eC_Archive a, uint value);

extern eC_Property * property_Archive_bufferRead;
extern void (* Archive_set_bufferRead)(const eC_Archive a, uint value);

extern eC_bool (* ArchiveDir_add)(eC_ArchiveDir __this, const char * name, const char * path, eC_ArchiveAddMode addMode, int compression, int * ratio, uint * newPosition);

extern int ArchiveDir_addFromFile_vTblID;
eC_bool ArchiveDir_addFromFile(eC_ArchiveDir __i, const char * name, eC_File input, eC_FileStats * stats, eC_ArchiveAddMode addMode, int compression, int * ratio, uint * newPosition);
extern eC_Method * method_ArchiveDir_addFromFile;

extern int ArchiveDir_addFromFileAtPosition_vTblID;
eC_bool ArchiveDir_addFromFileAtPosition(eC_ArchiveDir __i, uint position, const char * name, eC_File input, eC_FileStats * stats, eC_ArchiveAddMode addMode, int compression, int * ratio, uint * newPosition);
extern eC_Method * method_ArchiveDir_addFromFileAtPosition;

extern int ArchiveDir_delete_vTblID;
eC_bool ArchiveDir_delete(eC_ArchiveDir __i, const char * fileName);
extern eC_Method * method_ArchiveDir_delete;

extern int ArchiveDir_fileExists_vTblID;
eC_FileAttribs ArchiveDir_fileExists(eC_ArchiveDir __i, const char * fileName);
extern eC_Method * method_ArchiveDir_fileExists;

extern int ArchiveDir_fileOpen_vTblID;
eC_File ArchiveDir_fileOpen(eC_ArchiveDir __i, const char * fileName);
extern eC_Method * method_ArchiveDir_fileOpen;

extern int ArchiveDir_move_vTblID;
eC_bool ArchiveDir_move(eC_ArchiveDir __i, const char * name, eC_ArchiveDir to);
extern eC_Method * method_ArchiveDir_move;

extern int ArchiveDir_openDirectory_vTblID;
eC_ArchiveDir ArchiveDir_openDirectory(eC_ArchiveDir __i, const char * name, eC_FileStats * stats, eC_ArchiveAddMode addMode);
extern eC_Method * method_ArchiveDir_openDirectory;

extern int ArchiveDir_rename_vTblID;
eC_bool ArchiveDir_rename(eC_ArchiveDir __i, const char * name, const char * newName);
extern eC_Method * method_ArchiveDir_rename;

#define ARCHIVEOPENFLAGS_writeAccess_SHIFT               0
#define ARCHIVEOPENFLAGS_writeAccess_MASK                0x1
#define ARCHIVEOPENFLAGS_buffered_SHIFT                  1
#define ARCHIVEOPENFLAGS_buffered_MASK                   0x2
#define ARCHIVEOPENFLAGS_exclusive_SHIFT                 2
#define ARCHIVEOPENFLAGS_exclusive_MASK                  0x4
#define ARCHIVEOPENFLAGS_waitLock_SHIFT                  3
#define ARCHIVEOPENFLAGS_waitLock_MASK                   0x8


extern eC_Property * property_BufferedFile_handle;
extern void (* BufferedFile_set_handle)(const eC_BufferedFile b, eC_File value);
extern eC_File (* BufferedFile_get_handle)(const eC_BufferedFile b);

extern eC_Property * property_BufferedFile_bufferSize;
extern void (* BufferedFile_set_bufferSize)(const eC_BufferedFile b, uintsize value);
extern uintsize (* BufferedFile_get_bufferSize)(const eC_BufferedFile b);

extern eC_Property * property_BufferedFile_bufferRead;
extern void (* BufferedFile_set_bufferRead)(const eC_BufferedFile b, uintsize value);
extern uintsize (* BufferedFile_get_bufferRead)(const eC_BufferedFile b);

extern int (* DualPipe_getExitCode)(eC_DualPipe __this);

extern eC_bool (* DualPipe_getLinePeek)(eC_DualPipe __this, char * s, int max, int * charsRead);

extern int (* DualPipe_getProcessID)(eC_DualPipe __this);

extern eC_bool (* DualPipe_peek)(eC_DualPipe __this);

extern void (* DualPipe_terminate)(eC_DualPipe __this);

extern void (* DualPipe_wait)(eC_DualPipe __this);

#define ERRORCODE_level_SHIFT                            12
#define ERRORCODE_level_MASK                             0x3000
#define ERRORCODE_code_SHIFT                             0
#define ERRORCODE_code_MASK                              0xFFF


extern int File_close_vTblID;
void File_close(eC_File __i);
extern eC_Method * method_File_close;

extern int File_closeInput_vTblID;
void File_closeInput(eC_File __i);
extern eC_Method * method_File_closeInput;

extern int File_closeOutput_vTblID;
void File_closeOutput(eC_File __i);
extern eC_Method * method_File_closeOutput;

extern eC_bool (* File_copyTo)(eC_File __this, const char * outputFileName);

extern eC_bool (* File_copyToFile)(eC_File __this, eC_File f);

extern int File_eof_vTblID;
eC_bool File_eof(eC_File __i);
extern eC_Method * method_File_eof;

extern eC_bool (* File_flush)(eC_File __this);

extern double (* File_getDouble)(eC_File __this);

extern float (* File_getFloat)(eC_File __this);

extern uint (* File_getHexValue)(eC_File __this);

extern eC_bool (* File_getLine)(eC_File __this, char * s, int max);

extern int (* File_getLineEx)(eC_File __this, char * s, int max, eC_bool * hasNewLineChar);

extern int File_getSize_vTblID;
uint64 File_getSize(eC_File __i);
extern eC_Method * method_File_getSize;

extern eC_bool (* File_getString)(eC_File __this, char * string, int max);

extern int (* File_getValue)(eC_File __this);

extern int File_getc_vTblID;
eC_bool File_getc(eC_File __i, char * ch);
extern eC_Method * method_File_getc;

extern int File_lock_vTblID;
eC_bool File_lock(eC_File __i, eC_FileLock type, uint64 start, uint64 length, eC_bool wait);
extern eC_Method * method_File_lock;

extern void (* File_print)(eC_File __this, eC_Class * class_object, const void * object, ...);

extern void (* File_printLn)(eC_File __this, eC_Class * class_object, const void * object, ...);

extern int (* File_printf)(eC_File __this, const char * format, ...);

extern int File_putc_vTblID;
eC_bool File_putc(eC_File __i, char ch);
extern eC_Method * method_File_putc;

extern int File_puts_vTblID;
eC_bool File_puts(eC_File __i, const char * string);
extern eC_Method * method_File_puts;

extern int File_read_vTblID;
uintsize File_read(eC_File __i, void * buffer, uintsize size, uintsize count);
extern eC_Method * method_File_read;

extern int File_seek_vTblID;
eC_bool File_seek(eC_File __i, int64 pos, eC_FileSeekMode mode);
extern eC_Method * method_File_seek;

extern int File_tell_vTblID;
uint64 File_tell(eC_File __i);
extern eC_Method * method_File_tell;

extern int File_truncate_vTblID;
eC_bool File_truncate(eC_File __i, uint64 size);
extern eC_Method * method_File_truncate;

extern int File_unlock_vTblID;
eC_bool File_unlock(eC_File __i, uint64 start, uint64 length, eC_bool wait);
extern eC_Method * method_File_unlock;

extern int File_write_vTblID;
uintsize File_write(eC_File __i, const void * buffer, uintsize size, uintsize count);
extern eC_Method * method_File_write;

extern eC_Property * property_File_input;
extern void (* File_set_input)(const eC_File f, void * value);
extern void * (* File_get_input)(const eC_File f);

extern eC_Property * property_File_output;
extern void (* File_set_output)(const eC_File f, void * value);
extern void * (* File_get_output)(const eC_File f);

extern eC_Property * property_File_buffered;
extern void (* File_set_buffered)(const eC_File f, eC_bool value);

extern eC_Property * property_File_eof;
extern eC_bool (* File_get_eof)(const eC_File f);

#define FILEATTRIBS_isFile_SHIFT                         0
#define FILEATTRIBS_isFile_MASK                          0x1
#define FILEATTRIBS_isArchive_SHIFT                      1
#define FILEATTRIBS_isArchive_MASK                       0x2
#define FILEATTRIBS_isHidden_SHIFT                       2
#define FILEATTRIBS_isHidden_MASK                        0x4
#define FILEATTRIBS_isReadOnly_SHIFT                     3
#define FILEATTRIBS_isReadOnly_MASK                      0x8
#define FILEATTRIBS_isSystem_SHIFT                       4
#define FILEATTRIBS_isSystem_MASK                        0x10
#define FILEATTRIBS_isTemporary_SHIFT                    5
#define FILEATTRIBS_isTemporary_MASK                     0x20
#define FILEATTRIBS_isDirectory_SHIFT                    6
#define FILEATTRIBS_isDirectory_MASK                     0x40
#define FILEATTRIBS_isDrive_SHIFT                        7
#define FILEATTRIBS_isDrive_MASK                         0x80
#define FILEATTRIBS_isCDROM_SHIFT                        8
#define FILEATTRIBS_isCDROM_MASK                         0x100
#define FILEATTRIBS_isRemote_SHIFT                       9
#define FILEATTRIBS_isRemote_MASK                        0x200
#define FILEATTRIBS_isRemovable_SHIFT                    10
#define FILEATTRIBS_isRemovable_MASK                     0x400
#define FILEATTRIBS_isServer_SHIFT                       11
#define FILEATTRIBS_isServer_MASK                        0x800
#define FILEATTRIBS_isShare_SHIFT                        12
#define FILEATTRIBS_isShare_MASK                         0x1000


#define FILECHANGE_created_SHIFT                         0
#define FILECHANGE_created_MASK                          0x1
#define FILECHANGE_renamed_SHIFT                         1
#define FILECHANGE_renamed_MASK                          0x2
#define FILECHANGE_modified_SHIFT                        2
#define FILECHANGE_modified_MASK                         0x4
#define FILECHANGE_deleted_SHIFT                         3
#define FILECHANGE_deleted_MASK                          0x8
#define FILECHANGE_attribs_SHIFT                         4
#define FILECHANGE_attribs_MASK                          0x10


struct eC_FileListing
{
   const char * directory;
   const char * extensions;
   byte __ecere_padding[8];
};
extern eC_bool (* FileListing_find)(eC_FileListing * __this);

extern void (* FileListing_stop)(eC_FileListing * __this);

extern eC_Property * property_FileListing_name;
extern const char * (* FileListing_get_name)(const eC_FileListing * f);

extern eC_Property * property_FileListing_path;
extern const char * (* FileListing_get_path)(const eC_FileListing * f);

extern eC_Property * property_FileListing_stats;
extern void (* FileListing_get_stats)(const eC_FileListing * f, eC_FileStats * value);

extern int FileMonitor_onDirNotify_vTblID;
eC_bool FileMonitor_onDirNotify(eC_FileMonitor __i, any_object __t, eC_FileChange action, const char * fileName, const char * param);
extern eC_Method * method_FileMonitor_onDirNotify;

extern int FileMonitor_onFileNotify_vTblID;
eC_bool FileMonitor_onFileNotify(eC_FileMonitor __i, any_object __t, eC_FileChange action, const char * param);
extern eC_Method * method_FileMonitor_onFileNotify;

extern void (* FileMonitor_startMonitoring)(eC_FileMonitor __this);

extern void (* FileMonitor_stopMonitoring)(eC_FileMonitor __this);

extern eC_Property * property_FileMonitor_userData;
extern void (* FileMonitor_set_userData)(const eC_FileMonitor f, void * value);

extern eC_Property * property_FileMonitor_fileChange;
extern void (* FileMonitor_set_fileChange)(const eC_FileMonitor f, eC_FileChange value);

extern eC_Property * property_FileMonitor_fileName;
extern void (* FileMonitor_set_fileName)(const eC_FileMonitor f, const char * value);
extern const char * (* FileMonitor_get_fileName)(const eC_FileMonitor f);

extern eC_Property * property_FileMonitor_directoryName;
extern void (* FileMonitor_set_directoryName)(const eC_FileMonitor f, const char * value);
extern const char * (* FileMonitor_get_directoryName)(const eC_FileMonitor f);

#define MOVEFILEOPTIONS_overwrite_SHIFT                  0
#define MOVEFILEOPTIONS_overwrite_MASK                   0x1
#define MOVEFILEOPTIONS_sync_SHIFT                       1
#define MOVEFILEOPTIONS_sync_MASK                        0x2


#define PIPEOPENMODE_output_SHIFT                        0
#define PIPEOPENMODE_output_MASK                         0x1
#define PIPEOPENMODE_error_SHIFT                         1
#define PIPEOPENMODE_error_MASK                          0x2
#define PIPEOPENMODE_input_SHIFT                         2
#define PIPEOPENMODE_input_MASK                          0x4
#define PIPEOPENMODE_showWindow_SHIFT                    3
#define PIPEOPENMODE_showWindow_MASK                     0x8


extern byte * (* TempFile_stealBuffer)(eC_TempFile __this);

extern eC_Property * property_TempFile_openMode;
extern void (* TempFile_set_openMode)(const eC_TempFile t, eC_FileOpenMode value);
extern eC_FileOpenMode (* TempFile_get_openMode)(const eC_TempFile t);

extern eC_Property * property_TempFile_buffer;
extern void (* TempFile_set_buffer)(const eC_TempFile t, byte * value);
extern byte * (* TempFile_get_buffer)(const eC_TempFile t);

extern eC_Property * property_TempFile_size;
extern void (* TempFile_set_size)(const eC_TempFile t, uintsize value);
extern uintsize (* TempFile_get_size)(const eC_TempFile t);

extern eC_Property * property_TempFile_allocated;
extern void (* TempFile_set_allocated)(const eC_TempFile t, uintsize value);
extern uintsize (* TempFile_get_allocated)(const eC_TempFile t);

extern eC_Archive (* eC_archiveOpen)(const char * fileName, eC_ArchiveOpenFlags flags);
extern eC_bool (* eC_archiveQuerySize)(const char * fileName, eC_FileSize * size);
extern eC_bool (* eC_changeWorkingDir)(const char * buf);
extern char * (* eC_copySystemPath)(const char * p);
extern char * (* eC_copyUnixPath)(const char * p);
extern void (* eC_createTemporaryDir)(char * tempFileName, const char * _template);
extern eC_File (* eC_createTemporaryFile)(char * tempFileName, const char * _template);
extern eC_bool (* eC_deleteFile)(const char * fileName);
extern eC_DualPipe (* eC_dualPipeOpen)(eC_PipeOpenMode mode, const char * commandLine);
extern eC_DualPipe (* eC_dualPipeOpenEnv)(eC_PipeOpenMode mode, const char * env, const char * commandLine);
extern eC_DualPipe (* eC_dualPipeOpenEnvf)(eC_PipeOpenMode mode, const char * env, const char * command, ...);
extern eC_DualPipe (* eC_dualPipeOpenf)(eC_PipeOpenMode mode, const char * command, ...);
extern void (* eC_dumpErrors)(eC_bool display);
extern eC_bool (* eC_execute)(const char * command, ...);
extern eC_bool (* eC_executeEnv)(const char * env, const char * command, ...);
extern eC_bool (* eC_executeWait)(const char * command, ...);
extern eC_FileAttribs (* eC_fileExists)(const char * fileName);
extern void (* eC_fileFixCase)(char * file);
extern eC_bool (* eC_fileGetSize)(const char * fileName, eC_FileSize * size);
extern eC_bool (* eC_fileGetStats)(const char * fileName, eC_FileStats * stats);
extern eC_File (* eC_fileOpen)(const char * fileName, eC_FileOpenMode mode);
extern eC_BufferedFile (* eC_fileOpenBuffered)(const char * fileName, eC_FileOpenMode mode);
extern eC_bool (* eC_fileSetAttribs)(const char * fileName, eC_FileAttribs attribs);
extern eC_bool (* eC_fileSetTime)(const char * fileName, eC_SecSince1970 created, eC_SecSince1970 accessed, eC_SecSince1970 modified);
extern eC_bool (* eC_fileTruncate)(const char * fileName, uint64 size);
extern char * (* eC_getEnvironment)(const char * envName, char * envValue, int max);
extern void (* eC_getFreeSpace)(const char * path, eC_FileSize64 * size);
extern uint (* eC_getLastErrorCode)(void);
extern char * (* eC_getSlashPathBuffer)(char * d, const char * p);
extern char * (* eC_getSystemPathBuffer)(char * d, const char * p);
extern char * (* eC_getWorkingDir)(char * buf, int size);
extern void (* eC___e_log)(const char * text);
extern void (* eC_logErrorCode)(eC_ErrorCode errorCode, const char * details);
extern void (* eC___e_logf)(const char * format, ...);
extern eC_bool (* eC_makeDir)(const char * path);
extern void (* eC_makeSlashPath)(char * p);
extern void (* eC_makeSystemPath)(char * p);
extern eC_bool (* eC_moveFile)(const char * source, const char * dest);
extern eC_bool (* eC_moveFileEx)(const char * source, const char * dest, eC_MoveFileOptions options);
extern eC_bool (* eC_removeDir)(const char * path);
extern eC_bool (* eC_renameFile)(const char * oldName, const char * newName);
extern void (* eC_resetError)(void);
extern void (* eC_setEnvironment)(const char * envName, const char * envValue);
extern void (* eC_setErrorLevel)(eC_ErrorLevel level);
extern void (* eC_setLoggingMode)(eC_LoggingMode mode, void * where);
extern eC_bool (* eC_shellOpen)(const char * fileName, ...);
extern void (* eC_unsetEnvironment)(const char * envName);
extern void (* eC_debugBreakpoint)(void);
typedef uint32 eC_CharCategories;
typedef int eC_CharCategory;
enum
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

typedef uint32 eC_UnicodeDecomposition;
#define unicodeCompatibilityMappings 0xffffffff

typedef eC_CharCategories eC_PredefinedCharCategories;
enum
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
#define CHARCATEGORIES_markNonSpacing_SHIFT              1
#define CHARCATEGORIES_markNonSpacing_MASK               0x2
#define CHARCATEGORIES_markSpacing_SHIFT                 2
#define CHARCATEGORIES_markSpacing_MASK                  0x4
#define CHARCATEGORIES_markEnclosing_SHIFT               3
#define CHARCATEGORIES_markEnclosing_MASK                0x8
#define CHARCATEGORIES_numberDecimalDigit_SHIFT          4
#define CHARCATEGORIES_numberDecimalDigit_MASK           0x10
#define CHARCATEGORIES_numberLetter_SHIFT                5
#define CHARCATEGORIES_numberLetter_MASK                 0x20
#define CHARCATEGORIES_numberOther_SHIFT                 6
#define CHARCATEGORIES_numberOther_MASK                  0x40
#define CHARCATEGORIES_separatorSpace_SHIFT              7
#define CHARCATEGORIES_separatorSpace_MASK               0x80
#define CHARCATEGORIES_separatorLine_SHIFT               8
#define CHARCATEGORIES_separatorLine_MASK                0x100
#define CHARCATEGORIES_separatorParagraph_SHIFT          9
#define CHARCATEGORIES_separatorParagraph_MASK           0x200
#define CHARCATEGORIES_otherControl_SHIFT                10
#define CHARCATEGORIES_otherControl_MASK                 0x400
#define CHARCATEGORIES_otherFormat_SHIFT                 11
#define CHARCATEGORIES_otherFormat_MASK                  0x800
#define CHARCATEGORIES_otherSurrogate_SHIFT              12
#define CHARCATEGORIES_otherSurrogate_MASK               0x1000
#define CHARCATEGORIES_otherPrivateUse_SHIFT             13
#define CHARCATEGORIES_otherPrivateUse_MASK              0x2000
#define CHARCATEGORIES_otherNotAssigned_SHIFT            14
#define CHARCATEGORIES_otherNotAssigned_MASK             0x4000
#define CHARCATEGORIES_letterUpperCase_SHIFT             15
#define CHARCATEGORIES_letterUpperCase_MASK              0x8000
#define CHARCATEGORIES_letterLowerCase_SHIFT             16
#define CHARCATEGORIES_letterLowerCase_MASK              0x10000
#define CHARCATEGORIES_letterTitleCase_SHIFT             17
#define CHARCATEGORIES_letterTitleCase_MASK              0x20000
#define CHARCATEGORIES_letterModifier_SHIFT              18
#define CHARCATEGORIES_letterModifier_MASK               0x40000
#define CHARCATEGORIES_letterOther_SHIFT                 19
#define CHARCATEGORIES_letterOther_MASK                  0x80000
#define CHARCATEGORIES_punctuationConnector_SHIFT        20
#define CHARCATEGORIES_punctuationConnector_MASK         0x100000
#define CHARCATEGORIES_punctuationDash_SHIFT             21
#define CHARCATEGORIES_punctuationDash_MASK              0x200000
#define CHARCATEGORIES_punctuationOpen_SHIFT             22
#define CHARCATEGORIES_punctuationOpen_MASK              0x400000
#define CHARCATEGORIES_punctuationClose_SHIFT            23
#define CHARCATEGORIES_punctuationClose_MASK             0x800000
#define CHARCATEGORIES_punctuationInitial_SHIFT          24
#define CHARCATEGORIES_punctuationInitial_MASK           0x1000000
#define CHARCATEGORIES_punctuationFinal_SHIFT            25
#define CHARCATEGORIES_punctuationFinal_MASK             0x2000000
#define CHARCATEGORIES_punctuationOther_SHIFT            26
#define CHARCATEGORIES_punctuationOther_MASK             0x4000000
#define CHARCATEGORIES_symbolMath_SHIFT                  27
#define CHARCATEGORIES_symbolMath_MASK                   0x8000000
#define CHARCATEGORIES_symbolCurrency_SHIFT              28
#define CHARCATEGORIES_symbolCurrency_MASK               0x10000000
#define CHARCATEGORIES_symbolModifier_SHIFT              29
#define CHARCATEGORIES_symbolModifier_MASK               0x20000000
#define CHARCATEGORIES_symbolOther_SHIFT                 30
#define CHARCATEGORIES_symbolOther_MASK                  0x40000000


#define UNICODEDECOMPOSITION_canonical_SHIFT             0
#define UNICODEDECOMPOSITION_canonical_MASK              0x1
#define UNICODEDECOMPOSITION_compat_SHIFT                1
#define UNICODEDECOMPOSITION_compat_MASK                 0x2
#define UNICODEDECOMPOSITION_fraction_SHIFT              2
#define UNICODEDECOMPOSITION_fraction_MASK               0x4
#define UNICODEDECOMPOSITION_font_SHIFT                  3
#define UNICODEDECOMPOSITION_font_MASK                   0x8
#define UNICODEDECOMPOSITION_noBreak_SHIFT               4
#define UNICODEDECOMPOSITION_noBreak_MASK                0x10
#define UNICODEDECOMPOSITION_initial_SHIFT               5
#define UNICODEDECOMPOSITION_initial_MASK                0x20
#define UNICODEDECOMPOSITION_final_SHIFT                 6
#define UNICODEDECOMPOSITION_final_MASK                  0x40
#define UNICODEDECOMPOSITION_medial_SHIFT                7
#define UNICODEDECOMPOSITION_medial_MASK                 0x80
#define UNICODEDECOMPOSITION_isolated_SHIFT              8
#define UNICODEDECOMPOSITION_isolated_MASK               0x100
#define UNICODEDECOMPOSITION_circle_SHIFT                9
#define UNICODEDECOMPOSITION_circle_MASK                 0x200
#define UNICODEDECOMPOSITION_square_SHIFT                10
#define UNICODEDECOMPOSITION_square_MASK                 0x400
#define UNICODEDECOMPOSITION_sub_SHIFT                   11
#define UNICODEDECOMPOSITION_sub_MASK                    0x800
#define UNICODEDECOMPOSITION_super_SHIFT                 12
#define UNICODEDECOMPOSITION_super_MASK                  0x1000
#define UNICODEDECOMPOSITION_small_SHIFT                 13
#define UNICODEDECOMPOSITION_small_MASK                  0x2000
#define UNICODEDECOMPOSITION_vertical_SHIFT              14
#define UNICODEDECOMPOSITION_vertical_MASK               0x4000
#define UNICODEDECOMPOSITION_wide_SHIFT                  15
#define UNICODEDECOMPOSITION_wide_MASK                   0x8000
#define UNICODEDECOMPOSITION_narrow_SHIFT                16
#define UNICODEDECOMPOSITION_narrow_MASK                 0x10000


extern eC_bool (* eC_charMatchCategories)(unichar ch, eC_CharCategories categories);
extern eC_bool (* eC_getAlNum)(const char ** input, char * string, int max);
extern eC_CharCategory (* eC_getCharCategory)(unichar ch);
extern uint (* eC_getCombiningClass)(unichar ch);
extern int (* eC_iSO8859_1toUTF8)(const char * source, char * dest, int max);
extern int (* eC_uTF16BEtoUTF8Buffer)(const uint16 * source, byte * dest, int max);
extern char * (* eC_uTF16toUTF8)(const uint16 * source);
extern int (* eC_uTF16toUTF8Buffer)(const uint16 * source, char * dest, int max);
extern int (* eC_uTF32toUTF8Len)(const unichar * source, int count, char * dest, int max);
extern unichar (* eC_uTF8GetChar)(const char * string, int * numBytes);
extern eC_bool (* eC_uTF8Validate)(const char * source);
extern int (* eC_uTF8toISO8859_1)(const char * source, char * dest, int max);
extern uint16 * (* eC_uTF8toUTF16)(const char * source, int * wordCount);
extern int (* eC_uTF8toUTF16Buffer)(const char * source, uint16 * dest, int max);
extern int (* eC_uTF8toUTF16BufferLen)(const char * source, uint16 * dest, int max, int len);
extern uint16 * (* eC_uTF8toUTF16Len)(const char * source, int byteCount, int * wordCount);
extern eC_String (* eC_accenti)(constString string);
extern eC_String (* eC_casei)(constString string);
extern eC_String (* eC_encodeArrayToString)(eC_Array array);
extern eC_String (* eC_normalizeNFC)(constString string);
extern eC_String (* eC_normalizeNFD)(constString string);
extern eC_String (* eC_normalizeNFKC)(constString string);
extern eC_String (* eC_normalizeNFKD)(constString string);
extern eC_Array (* eC_normalizeNFKDArray)(constString string);
extern eC_String (* eC_normalizeUnicode)(constString string, eC_UnicodeDecomposition type, eC_bool compose);
extern eC_Array (* eC_normalizeUnicodeArray)(constString string, eC_UnicodeDecomposition type, eC_bool compose);
extern eC_String (* eC_stripUnicodeCategory)(constString string, eC_CharCategory c);
typedef eC_Instance eC_GlobalSettings;
typedef eC_GlobalSettings eC_GlobalAppSettings;
typedef int eC_GlobalSettingType;
enum
{
   GlobalSettingType_integer = 0x0,
   GlobalSettingType_singleString = 0x1,
   GlobalSettingType_stringList = 0x2
};

typedef eC_Instance eC_GlobalSettingsData;
typedef eC_Instance eC_GlobalSettingsDriver;
typedef int eC_JSONFirstLetterCapitalization;
enum
{
   JSONFirstLetterCapitalization_keepCase = 0x0,
   JSONFirstLetterCapitalization_upperCase = 0x1,
   JSONFirstLetterCapitalization_lowerCase = 0x2
};

typedef eC_Instance eC_JSONParser;
typedef int eC_JSONResult;
enum
{
   JSONResult_syntaxError = 0x0,
   JSONResult_success = 0x1,
   JSONResult_typeMismatch = 0x2,
   JSONResult_noItem = 0x3
};

typedef template_Map_String_JSONTypeOptions eC_OptionsMap;
typedef int eC_SettingsIOResult;
enum
{
   SettingsIOResult_error = 0x0,
   SettingsIOResult_success = 0x1,
   SettingsIOResult_fileNotFound = 0x2,
   SettingsIOResult_fileNotCompatibleWithDriver = 0x3
};

typedef eC_GlobalSettingsDriver eC_ECONGlobalSettings;
typedef eC_JSONParser eC_ECONParser;
typedef eC_GlobalSettingsDriver eC_JSONGlobalSettings;
typedef uint32 eC_JSONTypeOptions;
typedef uint eC_SetBool;
enum
{
   SetBool_unset = 0x0,
   SetBool_false = 0x1,
   SetBool_true = 0x2
};

extern eC_bool (* GlobalAppSettings_getGlobalValue)(eC_GlobalAppSettings __this, const char * section, const char * name, eC_GlobalSettingType type, void * value);

extern eC_bool (* GlobalAppSettings_putGlobalValue)(eC_GlobalAppSettings __this, const char * section, const char * name, eC_GlobalSettingType type, const void * value);

struct class_members_GlobalSettings
{
   eC_GlobalSettingsData data;
   eC_GlobalSettingsData * dataOwner;
   eC_Class * dataClass;
   byte __ecere_padding[96];
};
extern void (* GlobalSettings_close)(eC_GlobalSettings __this);

extern void (* GlobalSettings_closeAndMonitor)(eC_GlobalSettings __this);

extern int GlobalSettings_load_vTblID;
eC_SettingsIOResult GlobalSettings_load(eC_GlobalSettings __i);
extern eC_Method * method_GlobalSettings_load;

extern int GlobalSettings_onAskReloadSettings_vTblID;
void GlobalSettings_onAskReloadSettings(eC_GlobalSettings __i);
extern eC_Method * method_GlobalSettings_onAskReloadSettings;

extern eC_bool (* GlobalSettings_openAndLock)(eC_GlobalSettings __this, eC_FileSize * fileSize);

extern int GlobalSettings_save_vTblID;
eC_SettingsIOResult GlobalSettings_save(eC_GlobalSettings __i);
extern eC_Method * method_GlobalSettings_save;

extern eC_Property * property_GlobalSettings_settingsName;
extern void (* GlobalSettings_set_settingsName)(const eC_GlobalSettings g, const char * value);
extern const char * (* GlobalSettings_get_settingsName)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_settingsExtension;
extern void (* GlobalSettings_set_settingsExtension)(const eC_GlobalSettings g, const char * value);
extern const char * (* GlobalSettings_get_settingsExtension)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_settingsDirectory;
extern void (* GlobalSettings_set_settingsDirectory)(const eC_GlobalSettings g, const char * value);
extern const char * (* GlobalSettings_get_settingsDirectory)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_settingsLocation;
extern void (* GlobalSettings_set_settingsLocation)(const eC_GlobalSettings g, const char * value);
extern const char * (* GlobalSettings_get_settingsLocation)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_settingsFilePath;
extern void (* GlobalSettings_set_settingsFilePath)(const eC_GlobalSettings g, const char * value);
extern const char * (* GlobalSettings_get_settingsFilePath)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_allowDefaultLocations;
extern void (* GlobalSettings_set_allowDefaultLocations)(const eC_GlobalSettings g, eC_bool value);
extern eC_bool (* GlobalSettings_get_allowDefaultLocations)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_allUsers;
extern void (* GlobalSettings_set_allUsers)(const eC_GlobalSettings g, eC_bool value);
extern eC_bool (* GlobalSettings_get_allUsers)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_portable;
extern void (* GlobalSettings_set_portable)(const eC_GlobalSettings g, eC_bool value);
extern eC_bool (* GlobalSettings_get_portable)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_driver;
extern void (* GlobalSettings_set_driver)(const eC_GlobalSettings g, constString value);
extern constString (* GlobalSettings_get_driver)(const eC_GlobalSettings g);

extern eC_Property * property_GlobalSettings_isGlobalPath;
extern eC_bool (* GlobalSettings_get_isGlobalPath)(const eC_GlobalSettings g);

extern int GlobalSettingsDriver_load_vTblID;
eC_SettingsIOResult GlobalSettingsDriver_load(eC_GlobalSettingsDriver __i, eC_File f, eC_GlobalSettings globalSettings);
extern eC_Method * method_GlobalSettingsDriver_load;

extern int GlobalSettingsDriver_save_vTblID;
eC_SettingsIOResult GlobalSettingsDriver_save(eC_GlobalSettingsDriver __i, eC_File f, eC_GlobalSettings globalSettings);
extern eC_Method * method_GlobalSettingsDriver_save;

struct class_members_JSONParser
{
   eC_File f;
   eC_OptionsMap customJsonOptions;
   byte __ecere_padding[32];
};
extern eC_JSONResult (* JSONParser_getObject)(eC_JSONParser __this, eC_Class * objectType, void ** object);

extern eC_Property * property_JSONParser_debug;
extern void (* JSONParser_set_debug)(const eC_JSONParser j, eC_bool value);
extern eC_bool (* JSONParser_get_debug)(const eC_JSONParser j);

extern eC_Property * property_JSONParser_warnings;
extern void (* JSONParser_set_warnings)(const eC_JSONParser j, eC_bool value);
extern eC_bool (* JSONParser_get_warnings)(const eC_JSONParser j);

#define JSONTYPEOPTIONS_numbersUseOGDFS_SHIFT            0
#define JSONTYPEOPTIONS_numbersUseOGDFS_MASK             0x1
#define JSONTYPEOPTIONS_boolUseOGDFS_SHIFT               1
#define JSONTYPEOPTIONS_boolUseOGDFS_MASK                0x2
#define JSONTYPEOPTIONS_nullUseOGDFS_SHIFT               2
#define JSONTYPEOPTIONS_nullUseOGDFS_MASK                0x4
#define JSONTYPEOPTIONS_stringUseOGDFS_SHIFT             3
#define JSONTYPEOPTIONS_stringUseOGDFS_MASK              0x8
#define JSONTYPEOPTIONS_arrayUseOGDFS_SHIFT              4
#define JSONTYPEOPTIONS_arrayUseOGDFS_MASK               0x10
#define JSONTYPEOPTIONS_objectUseOGDFS_SHIFT             5
#define JSONTYPEOPTIONS_objectUseOGDFS_MASK              0x20
#define JSONTYPEOPTIONS_stripQuotesForOGDFS_SHIFT        6
#define JSONTYPEOPTIONS_stripQuotesForOGDFS_MASK         0x40
#define JSONTYPEOPTIONS_strictOGDFS_SHIFT                7
#define JSONTYPEOPTIONS_strictOGDFS_MASK                 0x80


extern eC_String (* eC_printECONObject)(eC_Class * objectType, void * object, int indent);
extern eC_String (* eC_printObjectNotationString)(eC_Class * objectType, void * object, eC_ObjectNotationType onType, int indent, eC_bool indentFirst, eC_JSONFirstLetterCapitalization capitalize);
extern eC_String (* eC_stringIndent)(constString base, int nSpaces, eC_bool indentFirst);
extern eC_bool (* eC_writeECONObject)(eC_File f, eC_Class * objectType, void * object, int indent);
extern eC_bool (* eC_writeJSONObject)(eC_File f, eC_Class * objectType, void * object, int indent);
extern eC_bool (* eC_writeJSONObject2)(eC_File f, eC_Class * objectType, void * object, int indent, eC_JSONFirstLetterCapitalization capitalize);
extern eC_bool (* eC_writeJSONObjectMapped)(eC_File f, eC_Class * objectType, void * object, int indent, eC_Map stringMap);
extern eC_bool (* eC_writeONString)(eC_File f, constString s, eC_bool eCON, int indent);
typedef struct eC_Condition eC_Condition;
typedef struct eC_Mutex eC_Mutex;
typedef struct eC_Semaphore eC_Semaphore;
typedef eC_Instance eC_Thread;
typedef int eC_ThreadPriority;
enum
{
   ThreadPriority_normal = 0x0,
   ThreadPriority_aboveNormal = 0x1,
   ThreadPriority_belowNormal = -1,
   ThreadPriority_highest = 0x2,
   ThreadPriority_lowest = -2,
   ThreadPriority_idle = -15,
   ThreadPriority_timeCritical = 0xF
};

struct eC_Condition
{
   byte __ecere_padding[40];
};
extern void (* Condition_signal)(eC_Condition * __this);

extern void (* Condition_wait)(eC_Condition * __this, eC_Mutex * mutex);

extern eC_Property * property_Condition_name;
extern void (* Condition_set_name)(const eC_Condition * c, const char * value);
extern const char * (* Condition_get_name)(const eC_Condition * c);

struct eC_Mutex
{
   byte __ecere_padding[56];
};
extern void (* Mutex_release)(eC_Mutex * __this);

extern void (* Mutex_wait)(eC_Mutex * __this);

extern eC_Property * property_Mutex_lockCount;
extern int (* Mutex_get_lockCount)(const eC_Mutex * m);

extern eC_Property * property_Mutex_owningThread;
extern int64 (* Mutex_get_owningThread)(const eC_Mutex * m);

struct eC_Semaphore
{
   byte __ecere_padding[40];
};
extern void (* Semaphore_release)(eC_Semaphore * __this);

extern eC_bool (* Semaphore_tryWait)(eC_Semaphore * __this);

extern void (* Semaphore_wait)(eC_Semaphore * __this);

extern eC_Property * property_Semaphore_initCount;
extern void (* Semaphore_set_initCount)(const eC_Semaphore * s, int value);
extern int (* Semaphore_get_initCount)(const eC_Semaphore * s);

extern eC_Property * property_Semaphore_maxCount;
extern void (* Semaphore_set_maxCount)(const eC_Semaphore * s, int value);
extern int (* Semaphore_get_maxCount)(const eC_Semaphore * s);

extern void (* Thread_create)(eC_Thread __this);

extern void (* Thread_kill)(eC_Thread __this);

extern int Thread_main_vTblID;
uint Thread_main(eC_Thread __i);
extern eC_Method * method_Thread_main;

extern void (* Thread_setPriority)(eC_Thread __this, eC_ThreadPriority priority);

extern void (* Thread_wait)(eC_Thread __this);

extern eC_Property * property_Thread_created;
extern eC_bool (* Thread_get_created)(const eC_Thread t);

extern int64 (* eC_getCurrentThreadID)(void);
extern eC_Class * class_Application;
extern eC_Class * class_Instance;
extern eC_Class * class_Module;
extern eC_Class * class_AccessMode;
extern eC_Class * class_Angle;
extern eC_Class * class_BTNamedLink;
extern eC_Class * class_BackSlashEscaping;
extern eC_Class * class_BitMember;
extern eC_Class * class_Box;
extern eC_Class * class_CIString;
extern eC_Class * class_Centimeters;
extern eC_Class * class_Class;
extern eC_Class * class_ClassDesignerBase;
extern eC_Class * class_ClassProperty;
extern eC_Class * class_ClassTemplateArgument;
extern eC_Class * class_ClassTemplateParameter;
extern eC_Class * class_ClassType;
extern eC_Class * class_DataMember;
extern eC_Class * class_DataMemberType;
extern eC_Class * class_DataValue;
extern eC_Class * class_DefinedExpression;
extern eC_Class * class_Degrees;
extern eC_Class * class_DesignerBase;
extern eC_Class * class_Distance;
extern eC_Class * class_EnumClassData;
extern eC_Class * class_EscapeCStringOptions;
extern eC_Class * class_Feet;
extern eC_Class * class_GlobalFunction;
extern eC_Class * class_IOChannel;
extern eC_Class * class_ImportType;
extern eC_Class * class_Meters;
extern eC_Class * class_Method;
extern eC_Class * class_MethodType;
extern eC_Class * class_MinMaxValue;
extern eC_Class * class_NameSpace;
extern eC_Class * class_ObjectInfo;
extern eC_Class * class_ObjectNotationType;
extern eC_Class * class_Platform;
extern eC_Class * class_Point;
extern eC_Class * class_Pointd;
extern eC_Class * class_Pointf;
extern eC_Class * class_Property;
extern eC_Class * class_Radians;
extern eC_Class * class_SerialBuffer;
extern eC_Class * class_Size;
extern eC_Class * class_StaticString;
extern eC_Class * class_StringAllocType;
extern eC_Class * class_SubModule;
extern eC_Class * class_TemplateMemberType;
extern eC_Class * class_TemplateParameterType;
extern eC_Class * class_ZString;
extern eC_Class * class_String;
extern eC_Class * class_byte;
extern eC_Class * class_char;
extern eC_Class * class_class;
extern eC_Class * class_double;
extern eC_Class * class_enum;
extern eC_Class * class_float;
extern eC_Class * class_int;
extern eC_Class * class_int64;
extern eC_Class * class_intptr;
extern eC_Class * class_intsize;
extern eC_Class * class_short;
extern eC_Class * class_struct;
extern eC_Class * class_uint;
extern eC_Class * class_uint16;
extern eC_Class * class_uint32;
extern eC_Class * class_uint64;
extern eC_Class * class_uintptr;
extern eC_Class * class_uintsize;
extern eC_Class * class_FieldType;
extern eC_Class * class_FieldTypeEx;
extern eC_Class * class_FieldValue;
extern eC_Class * class_FieldValueFormat;
extern eC_Class * class_AVLNode;
extern eC_Class * class_AVLTree;
extern eC_Class * class_Array;
extern eC_Class * class_BTNode;
extern eC_Class * class_BinaryTree;
extern eC_Class * class_BuiltInContainer;
extern eC_Class * class_Container;
extern eC_Class * class_CustomAVLTree;
extern eC_Class * class_HashMap;
extern eC_Class * class_HashMapIterator;
extern eC_Class * class_HashTable;
extern eC_Class * class_Item;
extern eC_Class * class_Iterator;
extern eC_Class * class_IteratorPointer;
extern eC_Class * class_Link;
extern eC_Class * class_LinkElement;
extern eC_Class * class_LinkList;
extern eC_Class * class_List;
extern eC_Class * class_ListItem;
extern eC_Class * class_Map;
extern eC_Class * class_MapIterator;
extern eC_Class * class_MapNode;
extern eC_Class * class_NamedItem;
extern eC_Class * class_NamedLink;
extern eC_Class * class_NamedLink64;
extern eC_Class * class_OldLink;
extern eC_Class * class_OldList;
extern eC_Class * class_StringBTNode;
extern eC_Class * class_StringBinaryTree;
extern eC_Class * class_TreePrintStyle;
extern eC_Class * class_Date;
extern eC_Class * class_DateTime;
extern eC_Class * class_DayOfTheWeek;
extern eC_Class * class_Month;
extern eC_Class * class_SecSince1970;
extern eC_Class * class_Seconds;
extern eC_Class * class_Time;
extern eC_Class * class_TimeStamp;
extern eC_Class * class_TimeStamp32;
extern eC_Class * class_Archive;
extern eC_Class * class_ArchiveAddMode;
extern eC_Class * class_ArchiveDir;
extern eC_Class * class_ArchiveOpenFlags;
extern eC_Class * class_BufferedFile;
extern eC_Class * class_ConsoleFile;
extern eC_Class * class_DualPipe;
extern eC_Class * class_ErrorCode;
extern eC_Class * class_ErrorLevel;
extern eC_Class * class_File;
extern eC_Class * class_FileAttribs;
extern eC_Class * class_FileChange;
extern eC_Class * class_FileListing;
extern eC_Class * class_FileLock;
extern eC_Class * class_FileMonitor;
extern eC_Class * class_FileOpenMode;
extern eC_Class * class_FileSeekMode;
extern eC_Class * class_FileSize;
extern eC_Class * class_FileSize64;
extern eC_Class * class_FileStats;
extern eC_Class * class_GuiErrorCode;
extern eC_Class * class_LoggingMode;
extern eC_Class * class_MoveFileOptions;
extern eC_Class * class_PipeOpenMode;
extern eC_Class * class_SysErrorCode;
extern eC_Class * class_TempFile;
extern eC_Class * class_CharCategories;
extern eC_Class * class_CharCategory;
extern eC_Class * class_PredefinedCharCategories;
extern eC_Class * class_UnicodeDecomposition;
extern eC_Class * class_ECONGlobalSettings;
extern eC_Class * class_ECONParser;
extern eC_Class * class_GlobalAppSettings;
extern eC_Class * class_GlobalSettingType;
extern eC_Class * class_GlobalSettings;
extern eC_Class * class_GlobalSettingsData;
extern eC_Class * class_GlobalSettingsDriver;
extern eC_Class * class_JSONFirstLetterCapitalization;
extern eC_Class * class_JSONGlobalSettings;
extern eC_Class * class_JSONParser;
extern eC_Class * class_JSONResult;
extern eC_Class * class_JSONTypeOptions;
extern eC_Class * class_OptionsMap;
extern eC_Class * class_SetBool;
extern eC_Class * class_SettingsIOResult;
extern eC_Class * class_Condition;
extern eC_Class * class_Mutex;
extern eC_Class * class_Semaphore;
extern eC_Class * class_Thread;
extern eC_Class * class_ThreadPriority;


////////////////////////////////////////////////// dll function imports //////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////
//////////////////////////////////////////////////////////////////////////////// ////////////////


extern const char * getTranslatedString(constString name, const char * string, const char * stringAndContext);
extern void loadTranslatedStrings(constString moduleName, const char * name);
extern void unloadTranslatedStrings(constString name);
extern void memoryGuard_popLoc(void);
extern void memoryGuard_pushLoc(const char * loc);
extern eC_BitMember * Class_addBitMember(eC_Class * _class, const char * name, const char * type, int bitSize, int bitPos, eC_AccessMode declMode);
extern eC_ClassProperty * Class_addClassProperty(eC_Class * _class, const char * name, const char * dataType, void * setStmt, void * getStmt);
extern eC_DataMember * Class_addDataMember(eC_Class * _class, const char * name, const char * type, uint size, uint alignment, eC_AccessMode declMode);
extern eC_bool Class_addMember(eC_Class * _class, eC_DataMember * dataMember);
extern eC_Method * Class_addMethod(eC_Class * _class, const char * name, const char * type, void * function, eC_AccessMode declMode);
extern eC_Property * Class_addProperty(eC_Class * _class, const char * name, const char * dataType, void * setStmt, void * getStmt, eC_AccessMode declMode);
extern eC_ClassTemplateParameter * Class_addTemplateParameter(eC_Class * _class, const char * name, eC_TemplateParameterType type, const void * info, eC_ClassTemplateArgument * defaultArg);
extern eC_Method * Class_addVirtualMethod(eC_Class * _class, const char * name, const char * type, void * function, eC_AccessMode declMode);
extern void Class_destructionWatchable(eC_Class * _class);
extern void Class_doneAddingTemplateParameters(eC_Class * base);
extern eC_ClassProperty * Class_findClassProperty(eC_Class * _class, const char * name);
extern eC_DataMember * Class_findDataMember(eC_Class * _class, const char * name, eC_Module module, eC_DataMember * subMemberStack, int * subMemberStackPos);
extern eC_DataMember * Class_findDataMemberAndId(eC_Class * _class, const char * name, int * id, eC_Module module, eC_DataMember * subMemberStack, int * subMemberStackPos);
extern eC_DataMember * Class_findDataMemberAndOffset(eC_Class * _class, const char * name, uint * offset, eC_Module module, eC_DataMember * subMemberStack, int * subMemberStackPos);
extern eC_Method * Class_findMethod(eC_Class * _class, const char * name, eC_Module module);
extern void Class_findNextMember(eC_Class * _class, eC_Class * curClass, eC_DataMember * curMember, eC_DataMember * subMemberStack, int * subMemberStackPos);
extern eC_Property * Class_findProperty(eC_Class * _class, const char * name, eC_Module module);
extern eC_Class * Class_getDesigner(eC_Class * _class);
extern int64 Class_getProperty(eC_Class * _class, const char * name);
extern eC_bool Class_isDerived(eC_Class * _class, eC_Class * from);
extern void Class_resize(eC_Class * _class, int newSize);
extern void Class_setProperty(eC_Class * _class, const char * name, int64 value);
extern void Class_unregister(eC_Class * _class);
extern eC_Application eC_initApp(eC_bool guiApp, int argc, char * argv[]);
extern void Enum_addFixedValue(eC_Class * _class, const char * string, int64 value);
extern int64 Enum_addValue(eC_Class * _class, const char * string);
extern void Instance_decRef(eC_Instance instance);
extern void Instance_delete(eC_Instance instance);
extern void Instance_evolve(eC_Instance * instancePtr, eC_Class * _class);
extern void Instance_fireSelfWatchers(eC_Instance instance, eC_Property * _property);
extern void Instance_fireWatchers(eC_Instance instance, eC_Property * _property);
extern eC_Class * Instance_getDesigner(eC_Instance instance);
extern void Instance_incRef(eC_Instance instance);
extern eC_bool Instance_isDerived(eC_Instance instance, eC_Class * from);
extern void * Instance_new(eC_Class * _class);
extern void * Instance_newEx(eC_Class * _class, eC_bool bindingsAlloc);
extern void Instance_setMethod(eC_Instance instance, const char * name, void * function);
extern void Instance_stopWatching(eC_Instance instance, eC_Property * _property, eC_Instance object);
extern void Instance_watch(eC_Instance instance, eC_Property * _property, void * object, void (* callback)(void *, void *));
extern void Instance_watchDestruction(eC_Instance instance, eC_Instance object, void (* callback)(void *, void *));
extern eC_DataMember * Member_addDataMember(eC_DataMember * member, const char * name, const char * type, uint size, uint alignment, eC_AccessMode declMode);
extern eC_bool Member_addMember(eC_DataMember * addTo, eC_DataMember * dataMember);
extern eC_DataMember * Member_new(eC_DataMemberType type, eC_AccessMode declMode);
extern eC_Module Module_load(eC_Module fromModule, const char * name, eC_AccessMode importAccess);
extern eC_Module Module_loadStatic(eC_Module fromModule, const char * name, eC_AccessMode importAccess, eC_bool (* Load)(eC_Module module), eC_bool (* Unload)(eC_Module module));
extern eC_Module Module_loadStrict(eC_Module fromModule, const char * name, eC_AccessMode importAccess);
extern void Module_unload(eC_Module fromModule, eC_Module module);
extern void Property_selfWatch(eC_Class * _class, const char * name, void (* callback)(void *));
extern void Property_watchable(eC_Property * _property);
extern void eC_delete(void * memory);
extern eC_Class * eC_findClass(eC_Module module, const char * name);
extern eC_DefinedExpression * eC_findDefine(eC_Module module, const char * name);
extern eC_GlobalFunction * eC_findFunction(eC_Module module, const char * name);
extern void * eC_new(uint size);
extern void * eC_new0(uint size);
extern eC_Class * eC_registerClass(eC_ClassType type, const char * name, const char * baseName, int size, int sizeClass, eC_bool (* Constructor)(void *), void (* Destructor)(void *), eC_Module module, eC_AccessMode declMode, eC_AccessMode inheritanceAccess);
extern eC_DefinedExpression * eC_registerDefine(const char * name, const char * value, eC_Module module, eC_AccessMode declMode);
extern eC_GlobalFunction * eC_registerFunction(const char * name, const char * type, void * func, eC_Module module, eC_AccessMode declMode);
extern void * eC_renew(void * memory, uint size);
extern void * eC_renew0(void * memory, uint size);
extern void eC_setArgs(eC_Application app, int argc, char * argv[]);
extern void eC_setPoolingDisabled(eC_bool disabled);

extern eC_Module __thisModule;

extern eC_Application ecrt_init(eC_Module fromModule, eC_bool loadEcere, eC_bool guiApp, int argc, char * argv[]);

/*
uint64 TAc(char x);
uint64 TAb(byte x);
uint64 TAs(short x);
uint64 TAus(uint16 x);
uint64 TAi(int x);
uint64 TAui(uint x);
uint64 TAi64(int64 x);
uint64 TAui64(uint64 x);
uint64 TAf(float x);
uint64 TAd(double x);
uint64 TAp(void * x);
uint64 TAo(Instance x);
*/

void * pTAvoid(uint64 x);
eC_Instance oTAInstance(uint64 x);
