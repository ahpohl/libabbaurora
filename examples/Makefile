# define the C compiler to use
CPP = g++

# define any compile-time flags
CPPFLAGS = -Wall -g -std=c++11 -pthread

# define any directories containing header files other than /usr/include
INCLUDES = -I../include

# define examples build directory
BUILD = build

# define library paths in addition to /usr/lib
LFLAGS = -L../build

# define any libraries to link into executable:
LIBS = -Wl,-Bstatic -labbaurora -Wl,-Bdynamic

# define source files
SRCS = $(wildcard *.cpp)

# define object files
OBJS = $(patsubst %.cpp,$(BUILD)/%.o,$(SRCS))

# define apps
APPS = $(patsubst $(BUILD)/%.o,$(BUILD)/%,$(OBJS))

.PHONY: all clean build

all: build $(OBJS) $(APPS)

$(BUILD)/%: $(BUILD)/%.o
	$(CPP) $< $(LFLAGS) $(LIBS) -o $@

$(BUILD)/%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

build:
	-@ mkdir -p $(BUILD)

clean:
	-@ $(RM) $(OBJS) $(APPS)
