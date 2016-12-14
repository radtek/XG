CC    = gcc
CPLUS = g++

PRJ_HOME = /mnt/hgfs/AztServer

BOOST    = $(PRJ_HOME)/XDepend/stl_boost_wstl/
PYTHON   = $(PRJ_HOME)/XDepend/Python279Linux/Include/python2.7/
MONGODB	 = $(PRJ_HOME)/XDepend/mongodbcxx104/include/

MAKE=make
INC	= -I ./header/ -I ./include/ -I ./source/

LIBS += -lpthread -ldl -lutil -liconv

CFLAG   = -c -Wall -DOS_LINUX -std=c++11 -fPIC -Wno-deprecated 
ARFLAGS = -rv
AR	    = ar

Custom =
UVAL = $(shell uname -a)
ifeq (BPI, $(Custom))
	ifeq (BPI,$(findstring BPI,$(UVAL)))
		OUTPUT = $(PRJ_HOME)/XDepend/BPI_ARMV7_Raspbian_Lib/lib_others
		INCLUDE = -I $(BOOST)
		CFLAG += -D_BPI_ARMV7
	
		SRC = $(wildcard ./Common/*.cpp)
		SRC+= $(wildcard ./Frame/allocmem/*.cpp)
		SRC+= $(wildcard ./Frame/control/*.cpp)
		SRC+= $(wildcard ./Frame/log/*.cpp)
		SRC+= $(wildcard ./Frame/timer/*.cpp)
		SRC+= $(wildcard ./Frame/tinxml/*.cpp)
		SRC+= $(wildcard ./Frame/*.cpp)
		SRC+= $(wildcard ./Frame/net/*.cpp)
		SRC+= $(wildcard ./Frame/netlog/*.cpp)
		SRC+= $(wildcard ./Frame/config/*.cpp)
		SRC+= $(wildcard ./Frame/base64/*.cpp)
		SRC+= $(wildcard ./Frame/json/*.cpp)
		SRC+= $(wildcard ./Frame/sqlite3/*.cpp)
		SRC+= $(wildcard ./Frame/TimeBufMg/*.cpp)
		SRC+= $(wildcard ./Frame/httpclient/*.cpp)
		SRC+= $(wildcard ./Frame/ComboxEvent/*.cpp)
		SRC+= $(wildcard ./Frame/pyscript/*.cpp)
		SRC+= $(wildcard ./ServerLib/Thread.cpp)

		CSRC = $(wildcard ./Frame/zip/*.c)
		CSRC+= $(wildcard ./Common/*.c)
		CSRC+= $(wildcard ./Frame/sqlite3/*.c)
	else
		$(shell exit)
	endif
else
	OUTPUT = $(PRJ_HOME)/Vc9Bin/centos_lib/
	INCLUDE = -I $(BOOST)
	INCLUDE+= -I $(MONGODB)
	INCLUDE+= -I $(PYTHON)
	
	SRC = $(wildcard ./Common/*.cpp)
	SRC+= $(wildcard ./ServerLib/*.cpp)
	SRC+= $(wildcard ./ServerLib/SQLDB/MySQLDB.cpp)
	SRC+= $(wildcard ./Frame/allocmem/*.cpp)
	SRC+= $(wildcard ./Frame/control/*.cpp)
	SRC+= $(wildcard ./Frame/log/*.cpp)
	SRC+= $(wildcard ./Frame/timer/*.cpp)
	SRC+= $(wildcard ./Frame/tinxml/*.cpp)
	SRC+= $(wildcard ./Frame/*.cpp)
	SRC+= $(wildcard ./Frame/net/*.cpp)
	SRC+= $(wildcard ./Frame/netlog/*.cpp)
	SRC+= $(wildcard ./Frame/mysqlbase/*.cpp)
	SRC+= $(wildcard ./Frame/config/*.cpp)
	SRC+= $(wildcard ./Frame/base64/*.cpp)
	SRC+= $(wildcard ./Frame/json/*.cpp)
	SRC+= $(wildcard ./Frame/hiredis/*.cpp)
	SRC+= $(wildcard ./Frame/mongdb/*.cpp)
	SRC+= $(wildcard ./Frame/httpclient/*.cpp)
	SRC+= $(wildcard ./Frame/HttpSs/*.cpp)
	SRC+= $(wildcard ./Frame/mongdb_hiredis_mg/*.cpp)
	#SRC+= $(wildcard ./Frame/pyscript/*.cpp)
	SRC+= $(wildcard ./Frame/sqlite3/*.cpp)
	SRC+= $(wildcard ./Frame/TimeBufMg/*.cpp)
	SRC+= $(wildcard ./Frame/ComboxEvent/*.cpp)
	SRC+= $(wildcard ./Frame/pyscript/*.cpp)
	SRC+= $(wildcard ./Frame/DistrFramePub/*.cpp)


	CSRC = $(wildcard ./Frame/zip/*.c)
	CSRC+= $(wildcard ./Common/*.c)
	CSRC+= $(wildcard ./Frame/sqlite3/*.c)
endif
 
.c.o:
	$(CC) $(CFLAGS) $(CFLAG) -o $*.o $(INC) $(INCLUDE) $*.c 

.cpp.o:
	$(CPLUS) $(CFLAGS) $(CFLAG) -o $*.o $(INC) $(INCLUDE) $*.cpp 


OBJ = $(SRC:.cpp=.o)
COBJ= $(CSRC:.c=.o)

TARGET = libXFrame_wstld.a

lib:$(OBJ) $(COBJ)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJ) $(COBJ)
	cp $(TARGET) $(OUTPUT)

all:lib

clean:
	find . -name "*.o" -exec rm {} \;
	rm $(TARGET) -fr
