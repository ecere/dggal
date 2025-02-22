ifneq ($(V),1)
.SILENT:
endif

# CORE VARIABLES

BINDMODLOC := obj
BINDING_MODULE := dggal
BINDING_NAME := dggal
MODULE := $(BINDING_NAME)_cpp
VERSION := 0.44
CONFIG := release
# CONTAINS_CXX := defined
ifndef COMPILER
COMPILER := default
endif

TARGET_TYPE = sharedlib

# FLAGS

ifndef DEBIAN_PACKAGE
CFLAGS = -std=c++17 -fno-exceptions -fno-rtti -Wcomment
LDFLAGS =
endif
PRJ_CFLAGS =
CECFLAGS =
OFLAGS = -Wl,-nostdlib
LIBS =

ifdef DEBUG
NOSTRIP := y
endif

CONSOLE = -mwindows

# INCLUDES

include ../../$(_CF_DIR)crossplatform.mk
include ../../$(_CF_DIR)default.cf

# POST-INCLUDES VARIABLES

OBJ = obj/$(BINDING_NAME).$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/

RES =

ifdef LINUX_TARGET
TARGET = $(OBJ)$(LP)$(MODULE)$(SO).0.44
SONAME = -Wl,-soname,$(LP)$(MODULE)$(SO).0
else
TARGET = $(OBJ)$(LP)$(MODULE)$(SO)
SONAME =
endif

ifndef CPP_BINDINGS_EMBEDDED_C_DISABLE
_embedded_c := 1
endif

_OBJECTS = $(_DEP_OBJECTS) \
	$(if $(_embedded_c),$(OBJ)$(BINDING_NAME).c$(O),) \
	$(OBJ)$(BINDING_NAME)$(O)

OBJECTS = $(_OBJECTS)

SOURCES = $(_DEP_SOURCES) \
	$(if $(_embedded_c),../c/$(BINDING_NAME).c,) \
	$(BINDING_NAME).cpp

LIBS += $(SHAREDLIB) $(EXECUTABLE) $(LINKOPT)

ifndef STATIC_LIBRARY_TARGET
LIBS += \
	$(call _L,ecere) \
	$(if $(_embedded_c),,$(call _L,ecere_c)) \
	$(call _L,ecere_cpp) \
   $(if $(_embedded_c),,$(call _L,$(BINDING_NAME)_c)) \
       $(call _L,$(BINDING_MODULE))
endif

PRJ_CFLAGS += \
	 $(if $(DEBUG), -g, -O2 -ffast-math) $(FPIC) -w -DECPRFX=eC_ $(if $(_CF_DIR),-I../../$(_CF_DIR)bindings/c -I../../$(_CF_DIR)bindings/cpp,)



PRJ_CFLAGS += \
      -I../c \

ifndef STATIC_LIBRARY_TARGET
OFLAGS += \
	 -L../../$(_CF_DIR)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/bin \
	 -L../../$(_CF_DIR)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/lib \
    -L$(BINDMODLOC)
endif

# TARGETS

.PHONY: all
all: objdir $(TARGET);

.PHONY: objdir
objdir:
	$(call mkdir,$(OBJ))

$(SYMBOLS): | objdir
$(OBJECTS): | objdir
$(TARGET): $(SOURCES) $(OBJECTS) | objdir
ifndef STATIC_LIBRARY_TARGET
	$(LD) $(OFLAGS) $(_OBJECTS) $(LIBS) -o $(TARGET) $(INSTALLNAME)
ifndef NOSTRIP
	$(STRIP) $(STRIPOPT) $(TARGET)
endif
else
ifdef WINDOWS_HOST
	$(AR) rcs $(TARGET) @$(OBJ)objects.lst $(LIBS)
else
	$(AR) rcs $(TARGET) $(OBJECTS) $(LIBS)
endif
endif
ifdef SHARED_LIBRARY_TARGET
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(if $(basename $(VER)),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)),)
	$(if $(VER),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO),)
endif
endif
endif
	$(call cp,$(TARGET),../../$(_CF_DIR)$(HOST_SODESTDIR))
ifdef SHARED_LIBRARY_TARGET
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(if $(basename $(VER)),ln -sf $(LP)$(MODULE)$(SO)$(VER) ../../$(_CF_DIR)$(HOST_SODESTDIR)$(LP)$(MODULE)$(SO)$(basename $(VER)),)
	$(if $(VER),ln -sf $(LP)$(MODULE)$(SO)$(VER) ../../$(_CF_DIR)$(HOST_SODESTDIR)$(LP)$(MODULE)$(SO),)
endif
endif
endif

# OBJECT RULES


ifdef _embedded_c
$(OBJ)$(BINDING_NAME).c$(O): ../c/$(BINDING_NAME).c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,$<) -o $(call quote_path,$@)
endif

$(OBJ)$(BINDING_NAME)$(O): $(BINDING_NAME).cpp
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,$<) -o $(call quote_path,$@)

.PHONY: cleantarget
cleantarget:
	$(call rm,$(TARGET))
ifdef SHARED_LIBRARY_TARGET
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)))
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO))
endif
endif
endif

.PHONY: clean
clean: cleantarget
	$(call rm,$(_OBJECTS))

.PHONY: realclean
realclean: cleantarget
	$(call rmr,$(OBJ))

.PHONY: wipeclean
wipeclean:
	$(call rmr,obj/)

.PHONY: distclean
distclean:
	$(_MAKE) -f ../../$(_CF_DIR)Cleanfile distclean distclean_all_subdirs

$(MAKEFILE_LIST): ;
$(SOURCES): ;
$(RESOURCES): ;
