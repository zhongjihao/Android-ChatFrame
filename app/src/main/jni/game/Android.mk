LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
 
LOCAL_MODULE    	:= gamecore

LOCAL_SRC_FILES 	:= \
	./jni_wrapper.cpp \
	./network/src/CRecvPacketHelper.cpp \
	./network/src/CSendPacketHelper.cpp \
	./network/src/TCPSocketEnging.cpp \
	./plazaviewItem/src/PlazaJni_help.cpp \
	./plazaviewItem/src/CLogonPlaza.cpp \
	./thread/Thread.cpp   \
	./usermanager/src/CClientUserItem.cpp \
	./usermanager/src/CClientUserManager.cpp \
	./roomviewItem/src/CTableFrame.cpp \
	./roomviewItem/src/CTableView.cpp \
	./roomviewItem/src/RoomJni_help.cpp \
	./roomviewItem/src/CRoomViewItem.cpp


LOCAL_LDLIBS := -llog

LOCAL_CFLAGS += -frtti -fexceptions -std=c++11

include $(BUILD_SHARED_LIBRARY)
