.PHONY: all objdir cleantarget clean realclean distclean test

# CORE VARIABLES

MODULE := dggalTest
VERSION :=
CONFIG := release
ifndef COMPILER
COMPILER := default
endif

TARGET_TYPE = executable

# FLAGS

ECFLAGS =
ifndef DEBIAN_PACKAGE
CFLAGS =
LDFLAGS =
endif
PRJ_CFLAGS =
CECFLAGS =
OFLAGS =
LIBS =

ifdef DEBUG
NOSTRIP := y
endif

CONSOLE = -mconsole

# INCLUDES

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

RUNNER =

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk
include $(_CF_DIR)default.cf
ifndef WINDOWS_HOST
export LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(DGGAL_ABSPATH)obj/$(CONFIG).$(HOST_PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/
ifdef WINDOWS_TARGET
export WINEPATH := $(WINEPATH);$(DGGAL_ABSPATH)obj/$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/;$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)/bin/;$(EC_SDK_SRC)/deps/curl-8.5.0/obj/release.$(PLATFORM)$(COMPILER_SUFFIX);$(OPENSSL_BIN_DIR)
RUNNER = wine
endif
else
export PATH := $(PATH);$(EC_SDK_SRC)/obj/$(HOST_PLATFORM)$(COMPILER_SUFFIX)/bin/;$(DGGAL_ABSPATH)obj/$(CONFIG).$(HOST_PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/
endif

# POST-INCLUDES VARIABLES

OBJ = obj/$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/

RES =

TARGET_NAME := dggalTest

TARGET = obj/$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/$(TARGET_NAME)$(OUT)

_ECSOURCES = \
	dggrs/isea3HTests.ec \
	$(EC_SDK_SRC)/extras/testing/testingFramework.ec \
	dggalTest.ec \
	dggrsTestCase.ec

ECSOURCES = $(call shwspace,$(_ECSOURCES))

_COBJECTS = $(addprefix $(OBJ),$(patsubst %.ec,%$(C),$(notdir $(_ECSOURCES))))

_SYMBOLS = $(addprefix $(OBJ),$(patsubst %.ec,%$(S),$(notdir $(_ECSOURCES))))

_IMPORTS = $(addprefix $(OBJ),$(patsubst %.ec,%$(I),$(notdir $(_ECSOURCES))))

_ECOBJECTS = $(addprefix $(OBJ),$(patsubst %.ec,%$(O),$(notdir $(_ECSOURCES))))

_BOWLS = $(addprefix $(OBJ),$(patsubst %.ec,%$(B),$(notdir $(_ECSOURCES))))

COBJECTS = $(call shwspace,$(_COBJECTS))

SYMBOLS = $(call shwspace,$(_SYMBOLS))

IMPORTS = $(call shwspace,$(_IMPORTS))

ECOBJECTS = $(call shwspace,$(_ECOBJECTS))

BOWLS = $(call shwspace,$(_BOWLS))

OBJECTS = $(ECOBJECTS) $(OBJ)$(MODULE).main$(O)

SOURCES = $(ECSOURCES)

RESOURCES =

ifdef USE_RESOURCES_EAR
RESOURCES_EAR =
else
RESOURCES_EAR = $(RESOURCES)
endif

LIBS += $(SHAREDLIB) $(EXECUTABLE) $(LINKOPT)

ifndef STATIC_LIBRARY_TARGET
LIBS += \
	$(call _L,ecrt)
endif

PRJ_CFLAGS += \
	 $(if $(DEBUG), -g, -O2 -ffast-math) $(FPIC) -Wall -DREPOSITORY_VERSION="\"$(REPOSITORY_VER)\"" \
			 -DIMPORT_STATIC=\"\"

ECFLAGS += -module $(MODULE)
# PLATFORM-SPECIFIC OPTIONS

ifdef WINDOWS_TARGET
OFLAGS += \
	$(if $(EC_SDK_SRC)/deps/zlib-1.2.8/obj/release.$(PLATFORM)$(COMPILER_SUFFIX),-L$(call quote_path,$(EC_SDK_SRC)/deps/zlib-1.2.8/obj/release.$(PLATFORM)$(COMPILER_SUFFIX)),) \
	$(if $(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)/bin,-L$(call quote_path,$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)/bin),) \
	-static-libgcc
else
OFLAGS += $(if $(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)/lib,-L$(call quote_path,$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)/lib),)
endif

ifdef LINUX_TARGET
LIBS += \
	$(call _L,m)
endif

CECFLAGS += -cpp $(_CPP)

# TARGETS

all: objdir $(TARGET)

objdir:
	$(if $(wildcard $(OBJ)),,$(call mkdir,$(OBJ)))
	$(if $(EC_SDK_SRC),$(if $(wildcard $(call escspace,$(EC_SDK_SRC)/crossplatform.mk)),,@$(call echo,eC SDK Source Warning: The value of EC_SDK_SRC is pointing to an incorrect ($(EC_SDK_SRC)) location.)),)
	$(if $(EC_SDK_SRC),,$(if $(ECP_DEBUG)$(ECC_DEBUG)$(ECS_DEBUG),@$(call echo,ECC Debug Warning: Please define EC_SDK_SRC before using ECP_DEBUG, ECC_DEBUG or ECS_DEBUG),))

$(OBJ)$(MODULE).main.ec: $(SYMBOLS) $(COBJECTS)
	@$(call rm,$(OBJ)symbols.lst)
	@$(call touch,$(OBJ)symbols.lst)
	$(call addtolistfile,$(SYMBOLS),$(OBJ)symbols.lst)
	$(call addtolistfile,$(IMPORTS),$(OBJ)symbols.lst)
	$(ECS) -console $(ARCH_FLAGS) $(ECSLIBOPT) @$(OBJ)symbols.lst -symbols obj/$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX) -o $(call quote_path,$@)

$(OBJ)$(MODULE).main.c: $(OBJ)$(MODULE).main.ec
	$(ECP) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) -c $(OBJ)$(MODULE).main.ec -o $(OBJ)$(MODULE).main.sym -symbols $(OBJ)
	$(ECC) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(OBJ)$(MODULE).main.ec -o $(call quote_path,$@) -symbols $(OBJ)

$(SYMBOLS): | objdir
$(OBJECTS): | objdir
$(TARGET): $(SOURCES) $(RESOURCES_EAR) $(SYMBOLS) $(OBJECTS) | objdir
	@$(call rm,$(OBJ)objects.lst)
	@$(call touch,$(OBJ)objects.lst)
	$(call addtolistfile,$(OBJ)$(MODULE).main$(O),$(OBJ)objects.lst)
	$(call addtolistfile,$(ECOBJECTS),$(OBJ)objects.lst)
ifndef STATIC_LIBRARY_TARGET
	$(LD) $(OFLAGS) @$(OBJ)objects.lst $(LIBS) -o $(TARGET) $(INSTALLNAME) $(SONAME)
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

# SYMBOL RULES

$(OBJ)isea3HTests.sym: dggrs/isea3HTests.ec
	$(ECP) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,dggrs/isea3HTests.ec) -o $(call quote_path,$@)

$(OBJ)testingFramework.sym: $(EC_SDK_SRC)/extras/testing/testingFramework.ec
	$(ECP) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,$(EC_SDK_SRC)/extras/testing/testingFramework.ec) -o $(call quote_path,$@)

$(OBJ)dggalTest.sym: dggalTest.ec
	$(ECP) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,dggalTest.ec) -o $(call quote_path,$@)

$(OBJ)dggrsTestCase.sym: dggrsTestCase.ec
	$(ECP) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,dggrsTestCase.ec) -o $(call quote_path,$@)

# C OBJECT RULES

$(OBJ)isea3HTests.c: dggrs/isea3HTests.ec $(OBJ)isea3HTests.sym | $(SYMBOLS)
	$(ECC) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,dggrs/isea3HTests.ec) -o $(call quote_path,$@) -symbols $(OBJ)

$(OBJ)testingFramework.c: $(EC_SDK_SRC)/extras/testing/testingFramework.ec $(OBJ)testingFramework.sym | $(SYMBOLS)
	$(ECC) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,$(EC_SDK_SRC)/extras/testing/testingFramework.ec) -o $(call quote_path,$@) -symbols $(OBJ)

$(OBJ)dggalTest.c: dggalTest.ec $(OBJ)dggalTest.sym | $(SYMBOLS)
	$(ECC) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,dggalTest.ec) -o $(call quote_path,$@) -symbols $(OBJ)

$(OBJ)dggrsTestCase.c: dggrsTestCase.ec $(OBJ)dggrsTestCase.sym | $(SYMBOLS)
	$(ECC) $(CFLAGS) $(CECFLAGS) $(ECFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,dggrsTestCase.ec) -o $(call quote_path,$@) -symbols $(OBJ)

# OBJECT RULES

$(OBJ)isea3HTests$(O): $(OBJ)isea3HTests.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,$(OBJ)isea3HTests.c) -o $(call quote_path,$@)

$(OBJ)testingFramework$(O): $(OBJ)testingFramework.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,$(OBJ)testingFramework.c) -o $(call quote_path,$@)

$(OBJ)dggalTest$(O): $(OBJ)dggalTest.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,$(OBJ)dggalTest.c) -o $(call quote_path,$@)

$(OBJ)dggrsTestCase$(O): $(OBJ)dggrsTestCase.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(call quote_path,$(OBJ)dggrsTestCase.c) -o $(call quote_path,$@)

$(OBJ)$(MODULE).main$(O): $(OBJ)$(MODULE).main.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) $(FVISIBILITY) -c $(OBJ)$(MODULE).main.c -o $(call quote_path,$@)

cleantarget:
	$(call rm,$(OBJ)$(MODULE).main$(O) $(OBJ)$(MODULE).main.c $(OBJ)$(MODULE).main.ec $(OBJ)$(MODULE).main$(I) $(OBJ)$(MODULE).main$(S))
	$(call rm,$(OBJ)symbols.lst)
	$(call rm,$(OBJ)objects.lst)
	$(call rm,$(TARGET))
ifdef SHARED_LIBRARY_TARGET
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)))
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO))
endif
endif
endif

clean: cleantarget
	$(call rm,$(_OBJECTS))
	$(call rm,$(_ECOBJECTS))
	$(call rm,$(_COBJECTS))
	$(call rm,$(_BOWLS))
	$(call rm,$(_IMPORTS))
	$(call rm,$(_SYMBOLS))

realclean: cleantarget
	$(call rmr,$(OBJ))

distclean: cleantarget
	$(call rmr,obj/)
	$(call rmr,.configs/)
	$(call rm,*.ews)
	$(call rm,*.Makefile)

test: $(TARGET)
	$(RUNNER) $(TARGET)
