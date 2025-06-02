#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
//#![allow(unused_variables)]

extern crate ecrt_sys;

use ecrt_sys::tparam_MapIterator_KT;
use ecrt_sys::tparam_MapIterator_V;
use ecrt_sys::MapIterator_get_key;
use ecrt_sys::MapIterator_get_value;
use ecrt_sys::Iterator_next;

use std::ffi::CString;
use std::ffi::CStr;
use std::ffi::c_void;
use std::os::raw::c_char;
use std::ops::Deref;
use std::ops::DerefMut;
use std::marker::PhantomData;
use std::mem::transmute;

#[macro_export] macro_rules! define_bitclass {
   (
      $name:ident, $base_type:ty,
      $(
         $field:ident => {
            set: $set:ident,
            is_bool: $bool_token:tt,
            type: $field_type:ty,
            prim_type: $prim_type:ty,
            mask: $mask:expr,
            shift: $shift:expr
         }
      ),* $(,)?
    ) => {
      #[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
      pub struct $name(pub $base_type);
      impl PartialEq<$base_type> for $name {
          fn eq(&self, other: &$base_type) -> bool {
              self.0 == *other
          }
      }
      impl From<$name> for $base_type {
         fn from(t: $name) -> Self { t.0 }
      }
      impl Deref for $name {
          type Target = $base_type;

          fn deref(&self) -> &Self::Target {
              &self.0
          }
      }

      impl $name {
         $(
            pub fn $set(&mut self, value: $field_type) {
               self.0 = (self.0 & !($mask as $base_type)) | ((value as $base_type) << $shift) & ($mask as $base_type);
            }
            pub fn $field(&self) -> $field_type {
               let prim: $prim_type = ((self.0 & ($mask as $base_type)) >> $shift) as $prim_type;
               define_bitclass!(@get $bool_token, prim, $field_type, $prim_type)
            }
         )*
      }
   };
   (@get true, $value:expr, $field_type:ty, $prim_type:ty) => {
      $value != 0
   };
   (@get false, $value:expr, $field_type:ty, $prim_type:ty) => {
      unsafe { std::mem::transmute::<$prim_type, $field_type>($value) }
   };
}

pub const nullInst : ecrt_sys::Instance = 0 as ecrt_sys::Instance;
pub const nullVTbl : *mut *mut c_void = 0 as *mut *mut c_void;
pub const nullPtr : *mut c_void = 0 as *mut c_void;

#[repr(transparent)]
#[derive(Clone, Copy)]
pub struct Instance(pub ecrt_sys::Instance);

impl Default for Instance
{
   fn default() -> Self { Instance(nullInst) }
}

impl DerefMut for Instance {
   fn deref_mut(&mut self) -> &mut Self::Target {
      &mut self.0
   }
}
impl Deref for Instance {
   type Target = ecrt_sys::Instance;
   fn deref(&self) -> &Self::Target {
      &self.0
   }
}


// eC runtime singleton
pub struct Application {
   pub app: ecrt_sys::Application
}
impl Drop for Application {
   fn drop(&mut self) {
      unsafe {
         ecrt_sys::__eCNameSpace__eC__types__eInstance_DecRef(self.app);
      }
   }
}

impl Application {
   pub fn new(_args: &Vec<std::string::String>) -> Application
   {
      unsafe {
         let app = ecrt_sys::ecrt_init(nullInst, true as u32, false as u32, 0,
               0 /*ptr::null_mut::<* mut * mut i8>()*/ as * mut * mut i8); // TODO: argc, args);
         Application { app: app }
      }
   }
}

// Trait for Template Types Arguments (conversion to 64-bit integer)
pub trait TTAU64 {
   fn from_u64(value: u64) -> Self;
   fn to_u64(&self) -> u64;
}

impl TTAU64 for u64
{
   fn from_u64(value: u64) -> Self { unsafe { transmute(value) } }
   fn to_u64(&self) -> u64 { unsafe { transmute(*self) } }
}
impl TTAU64 for i64
{
   fn from_u64(value: u64) -> Self { unsafe { transmute(value) } }
   fn to_u64(&self) -> u64 { unsafe { transmute(*self) } }
}
impl TTAU64 for f64
{
   fn from_u64(value: u64) -> Self { unsafe { transmute(value) } }
   fn to_u64(&self) -> u64 { unsafe { transmute(*self) } }
}
impl TTAU64 for f32
{
   fn from_u64(value: u64) -> Self { unsafe { transmute(value as u32) } }
   fn to_u64(&self) -> u64 { unsafe { transmute::<f32, u32>(*self) as u64 } }
}
impl TTAU64 for u32
{
   fn from_u64(value: u64) -> Self { value as u32 }
   fn to_u64(&self) -> u64 { *self as u64 }
}
impl TTAU64 for i32
{
   fn from_u64(value: u64) -> Self { value as i32 }
   fn to_u64(&self) -> u64 { *self as u64 }
}
impl TTAU64 for u16
{
   fn from_u64(value: u64) -> Self { value as u16 }
   fn to_u64(&self) -> u64 { *self as u64 }
}
impl TTAU64 for i16
{
   fn from_u64(value: u64) -> Self { value as i16 }
   fn to_u64(&self) -> u64 { *self as u64 }
}
impl TTAU64 for u8
{
   fn from_u64(value: u64) -> Self { value as u8 }
   fn to_u64(&self) -> u64 { *self as u64 }
}
impl TTAU64 for i8
{
   fn from_u64(value: u64) -> Self { value as i8 }
   fn to_u64(&self) -> u64 { *self as u64 }
}
impl TTAU64 for String
{
   fn from_u64(value: u64) -> Self { String(value as ecrt_sys::String) }
   fn to_u64(&self) -> u64 { (*self).0 as *mut c_void as u64 }
}
impl TTAU64 for Instance
{
   fn from_u64(value: u64) -> Self { Instance(value as ecrt_sys::Instance) }
   fn to_u64(&self) -> u64 { self.0 as *mut c_void as u64 }
}
impl TTAU64 for FieldValue
{
   fn from_u64(value: u64) -> Self
   {
      let result: &mut FieldValue;
      unsafe {
         result = &mut *(value as *mut c_void as *mut FieldValue);
      }
      *result
   }
   fn to_u64(&self) -> u64 {
      let result: u64 = (self as *const _ as *mut c_void) as u64;
      result
   }
}

#[macro_export] macro_rules! delegate_ttau64_and_default { ($wrapper:ty) => {
   impl TTAU64 for $wrapper {
      fn from_u64(value: u64) -> Self { Self(TTAU64::from_u64(value)) }
      fn to_u64(&self) -> u64 { self.0.to_u64() }
   }
   impl Default for $wrapper {
      fn default() -> Self {
         Self(Default::default())
      }
   }
}; }

pub type Pointd = ecrt_sys::Pointd;

// Generic field values
pub type FieldValue = ecrt_sys::FieldValue;

#[repr(i32)]
pub enum FieldType {
   Integer = ecrt_sys::FieldType_FieldType_integer,
   Real = ecrt_sys::FieldType_FieldType_real,
   Text = ecrt_sys::FieldType_FieldType_text,
   Blob = ecrt_sys::FieldType_FieldType_blob,
   Nil = ecrt_sys::FieldType_FieldType_nil,
   Array = ecrt_sys::FieldType_FieldType_array,
   Map = ecrt_sys::FieldType_FieldType_map
}

#[repr(i32)]
pub enum FieldValueFormat {
   // Decimal = ecrt_sys::FieldValueFormat_FieldValueFormat_decimal, // Same as Unset
   Unset = ecrt_sys::FieldValueFormat_FieldValueFormat_unset,
   Hex = ecrt_sys::FieldValueFormat_FieldValueFormat_hex,
   Octal = ecrt_sys::FieldValueFormat_FieldValueFormat_octal,
   Binary = ecrt_sys::FieldValueFormat_FieldValueFormat_binary,
   Exponential = ecrt_sys::FieldValueFormat_FieldValueFormat_exponential,
   Boolean = ecrt_sys::FieldValueFormat_FieldValueFormat_boolean,
   TextObj = ecrt_sys::FieldValueFormat_FieldValueFormat_textObj,
   Color = ecrt_sys::FieldValueFormat_FieldValueFormat_color,
}

define_bitclass! { FieldTypeEx, ecrt_sys::FieldTypeEx,
   type_ =>      { set: set_type,        is_bool: false,  type: FieldType,        prim_type: u32, mask: ecrt_sys::FIELDTYPEEX_type_MASK,       shift: ecrt_sys::FIELDTYPEEX_type_SHIFT },
   mustFree =>   { set: set_mustFree,    is_bool: true,   type: bool,             prim_type: u32, mask: ecrt_sys::FIELDTYPEEX_mustFree_MASK,   shift: ecrt_sys::FIELDTYPEEX_mustFree_SHIFT },
   format =>     { set: set_format,      is_bool: false,  type: FieldValueFormat, prim_type: u32, mask: ecrt_sys::FIELDTYPEEX_format_MASK,     shift: ecrt_sys::FIELDTYPEEX_format_SHIFT },
   isUnsigned => { set: set_isUnsigned,  is_bool: true,   type: bool,             prim_type: u32, mask: ecrt_sys::FIELDTYPEEX_isUnsigned_MASK, shift: ecrt_sys::FIELDTYPEEX_isUnsigned_SHIFT },
   isDateTime => { set: set_isDateTime,  is_bool: true,   type: bool,             prim_type: u32, mask: ecrt_sys::FIELDTYPEEX_isDateTime_MASK, shift: ecrt_sys::FIELDTYPEEX_isDateTime_SHIFT },
}

// Containers
#[derive(Clone, Copy)]
pub struct Map<K, V> {
   pub map: ecrt_sys::Map,
   _marker: PhantomData<(K, V)>,
}

impl<K, V> Map<K, V>
where K: TTAU64 + Copy, V: TTAU64 + Copy
{
   pub fn new(eC_map: ecrt_sys::Map) -> Self
   {
      Map { map: eC_map, _marker: PhantomData }
   }
}

#[derive(Clone, Copy)]
pub struct MapIterator<K, V>(pub ecrt_sys::MapIterator, PhantomData<(K, V)>);

impl<K, V> DerefMut for MapIterator<K, V> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.0
    }
}
impl<K, V> Deref for MapIterator<K, V> {
    type Target = ecrt_sys::MapIterator;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl<K, V> MapIterator<K, V>
where K: TTAU64 + Copy, V: TTAU64 + Copy
{
   pub fn new() -> Self
   {
      Self(ecrt_sys::MapIterator{ container: nullInst, pointer: nullPtr as IteratorPointer }, PhantomData)
   }

   pub fn set_map(&mut self, map: Map<K, V>) {
      self.container = map.map;
   }

   pub fn map(&self) -> Map<K, V>
   {
      // NOTE: This currently creates a new map object
      Map::<K, V> { map: self.container, _marker: PhantomData }
   }

   pub fn key(&self) -> K
   {
      let k: tparam_MapIterator_KT;
      unsafe {
         k = MapIterator_get_key.unwrap()(&**self);
      }
      K::from_u64(k)
   }

   pub fn value(&self) -> V
   {
      let v: tparam_MapIterator_V;
      unsafe {
         v = MapIterator_get_value.unwrap()(&**self);
      }
      V::from_u64(v)
   }

   pub fn next(&mut self) -> bool
   {
      let r: bool;
      unsafe {
         r = Iterator_next.unwrap()(transmute(&mut **self)) != 0;
      }
      r
   }
}

#[macro_export] macro_rules! MapIterator {
   (<$K:ty, $V:ty> $map:expr) => {{
      let mut instance: MapIterator<$K, $V> = MapIterator::<$K, $V>::new();
      instance.set_map($map);
      instance
   }};
}

pub type IteratorPointer = *mut ecrt_sys::IteratorPointer;

#[derive(Clone, Copy)]
pub struct Array<T> {
   pub array: ecrt_sys::Array,
   _marker: PhantomData<T>,
}

impl<T: Default + TTAU64> Array<T>
{
   pub fn new(eC_array: ecrt_sys::Array) -> Self
   {
      Self { array: eC_array, _marker: PhantomData }
   }

   pub fn count(&self) -> i32
   {
      let mut count: i32 = 0;
      if self.array != nullInst {
         unsafe {
            let members: *const ecrt_sys::class_members_Array = ((self.array as *const i8).wrapping_add((*ecrt_sys::class_Array).offset as usize)) as *const ecrt_sys::class_members_Array;
            count = (*members).count as i32;
         }
      }
      count
   }

   pub fn getAtPosition(&self, index: i32, create: bool, justCreated: Option<&mut bool>) -> IteratorPointer
   {
      let mut pointer = nullPtr as IteratorPointer;
      unsafe
      {
         let c = ecrt_sys::class_Array;
         let vTbl = if self.array != nullInst && (*self.array)._vTbl != nullVTbl { (*self.array)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(ecrt_sys::Container_getAtPosition_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(container: ecrt_sys::Array, pos: i32, create: u32, justAdded: *mut u32) -> IteratorPointer = std::mem::transmute(cMethod);
            let mut justCreatedValue: u32 = 0;
            let justCreatedPtr = if justCreated.is_some() { &mut justCreatedValue } else { nullPtr as *mut u32 };
            pointer = method(self.array, index, create as u32, justCreatedPtr);
            if let Some(jc) = justCreated { *jc = justCreatedValue != 0; }
         }
      }
      pointer
   }

   pub fn getData(&self, pointer: IteratorPointer) -> T
   {
      let data: T;
      unsafe
      {
         let c = ecrt_sys::class_Array;
         let vTbl = if self.array != nullInst && (*self.array)._vTbl != nullVTbl { (*self.array)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(ecrt_sys::Container_getData_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(container: ecrt_sys::Array, pointer: IteratorPointer) -> u64 = std::mem::transmute(cMethod);
            let dataU64 = method(self.array, pointer);
            data = T::from_u64(dataU64);
         } else {
            data = Default::default();
         }
      }
      data
   }

   pub fn setData(&mut self, pointer: IteratorPointer, value: &T) -> bool
   {
      let mut result: bool = false;
      unsafe
      {
         let c = ecrt_sys::class_Array;
         let vTbl = if self.array != nullInst && (*self.array)._vTbl != nullVTbl { (*self.array)._vTbl } else { (*c)._vTbl };
         let cMethod: usize = std::mem::transmute(*vTbl.add(ecrt_sys::Container_setData_vTblID as usize));
         if cMethod != 0usize {
            let method : unsafe extern "C" fn(container: ecrt_sys::Array, pointer: IteratorPointer, value: u64) -> u64 = std::mem::transmute(cMethod);
            let valueU64: u64 = T::to_u64(&value);
            result = method(self.array, pointer, valueU64) != 0;
         }
      }
      result
   }

   pub fn getElement(&self, index: i32) -> T
   {
      self.getData(self.getAtPosition(index, false, None))
   }

   pub fn setElement(&mut self, index: i32, value: &T) -> bool
   {
      self.setData(self.getAtPosition(index, false, None), value)
   }
}

#[repr(transparent)]
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct String(pub ecrt_sys::String);

impl String
{
   pub fn is_null(&self) -> bool
   {
      self.0 == nullPtr as ecrt_sys::String
   }

   pub fn new(string: &str, v: &mut Vec<u8>) -> String
   {
      *v = string.as_bytes().to_vec();
      v.push(0);
      String(v.as_mut_ptr() as ecrt_sys::String)
   }

   pub fn string(&self) -> std::string::String
   {
      if !self.is_null() {
         let r;
         unsafe { r = CStr::from_ptr((*self).0).to_str(); }
         r.unwrap().to_string()
      } else {
         "".to_string()
      }
   }
}

impl Default for String
{
   fn default() -> Self { String(nullPtr as ecrt_sys::String) }
}

#[repr(transparent)]
#[derive(Clone, Copy)]
pub struct ConstString(pub *const c_char);

impl ConstString
{
   pub fn default() -> Self { ConstString(nullPtr as *const c_char) }

   pub fn is_null(&self) -> bool
   {
      self.0 == nullPtr as *const c_char
   }

   pub fn new(string: &str, cs: &mut CString) -> ConstString
   {
      let rcs = CString::new(string);
      if rcs.is_ok() {
         *cs = rcs.unwrap();
         ConstString(cs.as_ptr())
      } else {
         ConstString(nullPtr as *const c_char)
      }
   }

   pub fn string(&self) -> std::string::String
   {
      if !self.is_null() {
         let r;
         unsafe { r = CStr::from_ptr((*self).0).to_str(); }
         r.unwrap().to_string()
      } else {
         "".to_string()
      }
   }
}

impl Default for ConstString
{
   fn default() -> Self { ConstString(nullPtr as *const c_char) }
}

// String manipulation functions
pub fn getLastDirectory(string: &str) -> std::string::String {
   let result: std::string::String;
   let mut len = string.len();
   let csString = CString::new(string).unwrap();
   let mut buffer = Vec::<u8>::new();
   buffer.reserve(len+1);
   unsafe {
      ecrt_sys::getLastDirectory.unwrap()(csString.as_ptr(), buffer.as_mut_ptr() as *mut i8);
      len = ecrt_sys::strlen(buffer.as_ptr() as *mut i8) as usize;
      buffer.set_len(len);
   }
   result = std::string::String::from_utf8(buffer[..len].to_vec()).unwrap();
   result
}

pub fn tokenizeWith<const MAX_TOKENS: usize>(string: &str, tokenizers: &str, escapeBackSlashes: bool) -> Vec<std::string::String>
{
   let mut tokens : Vec<std::string::String>;
   unsafe
   {
      let mut buffer = Vec::from(string.as_bytes());
      buffer.push(0);
      let cString: *mut i8 = buffer.as_mut_ptr() as *mut i8;
      let cTokenizers = CString::new(tokenizers).unwrap();
      let mut tokensArray: [*mut i8; MAX_TOKENS] = [nullPtr as *mut i8; MAX_TOKENS]; // REVIEW: Any way to avoid this initialization?

      let nTokens: i32 = ecrt_sys::tokenizeWith.unwrap()(cString, MAX_TOKENS as i32, tokensArray.as_mut_ptr(), cTokenizers.as_ptr(), escapeBackSlashes as u32);

      tokens = Vec::new();
      tokens.reserve(nTokens as usize);
      tokens.set_len(nTokens as usize);
      for i in 0..nTokens {
         tokens[i as usize] = CStr::from_ptr(tokensArray[i as usize]).to_str().unwrap().to_string();
      }
   }
   tokens
}

// File system
pub struct File {
   pub file: ecrt_sys::File
}
impl Drop for File {
   fn drop(&mut self) {
      unsafe {
         ecrt_sys::__eCNameSpace__eC__types__eInstance_DecRef(self.file);
      }
   }
}

#[repr(i32)]
pub enum FileOpenMode {
   Read        = ecrt_sys::FileOpenMode_FileOpenMode_read,
   Write       = ecrt_sys::FileOpenMode_FileOpenMode_write,
   Append      = ecrt_sys::FileOpenMode_FileOpenMode_append,
   ReadWrite   = ecrt_sys::FileOpenMode_FileOpenMode_readWrite,
   WriteRead   = ecrt_sys::FileOpenMode_FileOpenMode_writeRead,
   AppendRead  = ecrt_sys::FileOpenMode_FileOpenMode_appendRead
}

impl File {
   pub fn open(fileName: &str, mode: FileOpenMode) -> Result<Self, std::string::String> {
      let f: ecrt_sys::File;
      let csFileName = CString::new(fileName).unwrap();
      unsafe { f = ecrt_sys::fileOpen.unwrap()(csFileName.as_ptr(), mode as ecrt_sys::FileOpenMode); }
      if f != nullInst {
         Ok(File { file: f })
      } else {
         Err(format!("Failed to open {fileName}"))
      }
   }
}
