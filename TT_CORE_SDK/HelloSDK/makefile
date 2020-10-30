#
#  WARNING: This file and all related programs (including any computer 
#  programs, example programs, and all source code) are the exclusive 
#  property of Trading Technologies International, Inc. (“TT”), are 
#  protected by copyright law and international treaties, and are for use 
#  only by those with the express written permission from TT.  Unauthorized 
#  possession, reproduction, distribution, use or disclosure of this file 
#  and any related program (or document) derived from it is prohibited by 
#  State and Federal law, and by local law outside of the U.S. and may 
#  result in severe civil and criminal penalties.
#
# ****************************************************************************

SHELL = /bin/sh
CC = /opt/rh/devtoolset-7/root/usr/bin/c++
CFLAGS = -std=c++14 -fPIC -g 
DLIBSFLAGS = -Wl,--no-whole-archive 
INCLUDES = -I./../../include
RLIBPATH = -Wl,--rpath,../lib -Wl,--rpath,../lib64
LIBPATH = -L/usr/local/lib \
  -L../../lib \
  -L../../lib64  


DLIBS = -lpthread \
	-ltt_core_sdk \
	-lcurl \
	-lssl \
	-lcrypto \
	-ldl \
	-lstdc++fs\
	-lboost_system \
	-lboost_thread \
	-lboost_chrono \
	-lboost_date_time \
	-lboost_atomic \
	-lprice_handler \
	-lprice_unifier \
	-lprice_client \
	-lprice_client_mds \
	-lconnector \
	-lpulse \
	-lonload_ext \
	-ldarwin \
	-lall_messages \
	-lprotobuf \
	-lzookeeper_mt \
	-lttmetrics \
	-lmsg_converter \
	-lcommon \
	-lttlogger \
	-lboost_system \
	-lboost_thread \
	-lboost_chrono \
	-lboost_date_time \
	-lboost_atomic \
	-lboost_filesystem \
	-lboost_serialization \
	-losabstraction \
	-lsnappy \
	-llbm \
	-lonload_ext \
	-lwebsockets7 \
	-lz \
	-lrt


BIN_PATH = ./../../bin/
TARGET = $(BIN_PATH)HelloSDK

.PHONY: all
all: $(TARGET)

OBJ_PATH = .


CPP_FILES:= $(wildcard ./*.cpp)
OBJ_FILES:= $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_FILES))

$(TARGET) : $(OBJ_FILES)
	$(CC) -o $@ $(OBJ_FILES)  $(RLIBPATH) $(LIBPATH) $(DLIBS) $(DLIBSFLAGS) 

$(OBJ_FILES) : $(OBJ_PATH)/%.o:./%.cpp
	$(CC)  -pthread $(CFLAGS) $(INCLUDES) -c $< -o $@ 

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ_FILES)

