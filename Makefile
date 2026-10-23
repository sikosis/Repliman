## Haiku Generic Makefile v2.6 ##

NAME = Repliman
TYPE = APP
APP_MIME_SIG = application/x-vnd.rxc-repliman

SRCS = \
	src/xHeaderListItem.cpp \
	src/xListItem.cpp \
	src/replimanWindow.cpp \
	src/replimanApp.cpp \
	src/main.cpp

RDEFS =
RSRCS = resources/repliman.rsrc resources/data.rsrc

LIBS = be $(STDCPPLIBS)
LIBPATHS =
SYSTEM_INCLUDE_PATHS = /boot/system/develop/headers/private/app
LOCAL_INCLUDE_PATHS = src

OPTIMIZE := FULL
LOCALES =
DEFINES =
WARNINGS = ALL
SYMBOLS := FALSE
DEBUGGER =
COMPILER_FLAGS = -std=c++17
LINKER_FLAGS = -Xlinker -s -Xlinker -x
APP_VERSION =
DRIVER_PATH =

DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
