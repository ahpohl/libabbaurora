# get version info from git and compile into the program
# git tags have format vMAJOR.MINOR.RELEASE
# https://embeddedartistry.com/blog/2016/10/27/giving-you-build-a-version

version := $(subst -, ,$(shell git describe --long --dirty --tags))
COMMIT := $(strip $(word 3, $(version)))
COMMITS_PAST := $(strip $(word 2, $(version)))
DIRTY := $(strip $(word 4, $(version)))
ifneq ($(COMMITS_PAST), 0)
    BUILD_INFO_COMMITS := "."$(COMMITS_PAST)
endif
ifneq ($(DIRTY),)
    BUILD_INFO_DIRTY :="+"
endif

export BUILD_TAG := $(strip $(word 1, $(version)))
export BUILD_INFO := $(COMMIT)$(BUILD_INFO_COMMITS)$(BUILD_INFO_DIRTY)
FULL_VERSION := $(BUILD_TAG:v%=%)
MAJOR_VERSION := $(word 1, $(subst ., ,$(FULL_VERSION)))
MINOR_VERSION := $(word 2, $(subst ., ,$(FULL_VERSION)))
RELEASE_VERSION := $(word 3, $(subst ., ,$(FULL_VERSION)))

# define the library file names
PROJECT := libabbaurora
STATIC_LIB := $(PROJECT).a
SHARED_LIB := $(PROJECT).so

# define examples directory
EXAMPLE := examples

#############
### build ###
#############

# define the C compiler to use
CPP := g++

# define cross compiler for aarch64 target
ifeq ($(CROSS_COMPILE),aarch64-unknown-linux-gnu)
CPP := aarch64-unknown-linux-gnu-g++
endif

# define any compile-time flags
CPPFLAGS = -Wall -Wextra -g -std=c++11 -pthread -fPIC -shared

CPPFLAGS += -DVERSION_BUILD_DATE=\""$(shell date "+%F %T")"\" \
            -DVERSION_BUILD_MACHINE=\""$(shell echo `whoami`@`hostname`)"\" \
            -DVERSION_TAG=\"$(BUILD_TAG)\" \
            -DVERSION_BUILD=\"$(BUILD_INFO)\"

# define any directories containing header files other than /usr/include
INCLUDES = -I./include

# define library paths in addition to /usr/lib
LFLAGS = -Wl,-soname,$(SHARED_LIB).$(MAJOR_VERSION)

# define any libraries to link into executable:
LIBS =

# define src directory
SRC_DIR = src

# define the output directory
OBJ_DIR = build

# define the C source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# define the objects
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

###############
### targets ###
###############

.PHONY: build clean install docs examples

all: shared static

build:
	-@ mkdir -p $(OBJ_DIR)

shared: build $(OBJS)
	$(CPP) $(CPPFLAGS) $(INCLUDES) -o $(OBJ_DIR)/$(SHARED_LIB) $(OBJS) $(LFLAGS) $(LIBS)

static: build $(OBJS)
	$(AR) rcs $(OBJ_DIR)/$(STATIC_LIB) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	-@ $(RM) $(OBJS) $(OBJ_DIR)/$(SHARED_LIB) $(OBJ_DIR)/$(STATIC_LIB) *~
	-@ $(MAKE) -C $(EXAMPLE) clean

# define install directories
ifeq ($(PREFIX),)
  PREFIX = /usr/local
endif

install: all
	install -d $(DESTDIR)$(PREFIX)/lib
	install -m 644 $(OBJ_DIR)/$(STATIC_LIB) $(DESTDIR)$(PREFIX)/lib/$(STATIC_LIB)
	install -m 755 $(OBJ_DIR)/$(SHARED_LIB) $(DESTDIR)$(PREFIX)/lib/$(SHARED_LIB).$(FULL_VERSION)
	ln -sr $(DESTDIR)$(PREFIX)/lib/$(SHARED_LIB).$(FULL_VERSION) $(DESTDIR)$(PREFIX)/lib/$(SHARED_LIB)
	ln -sr $(DESTDIR)$(PREFIX)/lib/$(SHARED_LIB).$(FULL_VERSION) $(DESTDIR)$(PREFIX)/lib/$(SHARED_LIB).$(MAJOR_VERSION)
	install -d $(DESTDIR)$(PREFIX)/include/$(PROJECT)
	install -D -m 644 ./include/*.h $(DESTDIR)$(PREFIX)/include/$(PROJECT)

docs:
	doxygen Doxyfile

examples: all
	$(MAKE) -C $(EXAMPLE)
