# pre-existing IDL files
TOPIC_IDL := dds_dcps_builtintopics.idl
DCPS_API_IDL := dds_dcps.idl

# Only valid for Unix/Linux type systems.
OBJ_POSTFIX := .o 

# Identifier for the StandAlone CORBA libraries.
SPLICE_SA_ORB := DDS_Eorb_3_0_SA

.PRECIOUS: %SplDcps.cpp %Dcps_impl.cpp

include ./orbdeps_sa.mak

# This determines what will be processed 

# OpenSplice Preprocessor (idlpp) output
IDLPP_OBJ        = $(TOPIC_IDL:%.idl=%Dcps_impl.o) $(TOPIC_IDL:%.idl=%SplDcps.o)
IDLPP_SRC        = $(IDLPP_OBJ:%.o=%.cpp)
IDLPP_IDL        = $(TOPIC_IDL:%.idl=%Dcps.idl)
IDLPP_HDR        = $(IDLPP_OBJ:%.o=%.h) ccpp_$(TOPIC_IDL:%.idl=%.h)

# API Classes.
API_SRC          = $(wildcard ccpp_*.cpp)
API_OBJ          = $(API_SRC:%.cpp=%.o)

# EORB implementation classes.
EORB_OBJ         = common_cobject.o localobject.o

# All objects
OBJS = $(IDLPP_OBJ) $(ORB_TOP_OBJ) $(ORB_API_OBJ) $(API_OBJ) $(IDLPP_ORB_OBJ) $(EORB_OBJ)

# library target name
TARGET_DLIB := dcpssacpp
DLIB_PREFIX := lib
DLIB_POSTFIX := .so

TARGET = $(DLIB_PREFIX)$(TARGET_DLIB)$(DLIB_POSTFIX)

INCLUDE += -I./
INCLUDE += -I$(OSPL_HOME)/include
INCLUDE += -I$(OSPL_HOME)/include/sys
INCLUDE += -I$(OSPL_HOME)/include/dcps/C++/SACPP

# compiler and compiler flags (Only valid for gcc-compilers)
CXX := CC
CXXFLAGS := -g -KPIC -mt -xO4
CPPFLAGS = $(ORB_SA_SELECT_FLAGS) $(INCLUDE) $(ORB_SA_CPP_FLAGS)

# linker and linker flags (Only valid for gcc-linkers)
LD_SO := $(CXX)
SPLICE_LIBRARY_PATH := $(OSPL_HOME)/lib
LD_FLAGS := -G -mt -R -xildoff
LD_LIBS  := -lrt -ldl -lpthread -lnsl -ldcpsgapi -lgen -lposix4 -lX11 -lXt -lXm $(ORB_LDLIBS)

# SPLICE IDL preprocessor and preprocessor flags
IDLPP := idlpp
IDLPPFLAGS := -P SACPP_API -S -l cpp

#Dependencies

all : $(TARGET)

$(IDLPP_OBJ): $(IDLPP_ORB_HDR) $(ORB_TOP_HDR) $(ORB_API_HDR)

#generic rules for IDL preprocessing

$(ORB_TOP_SRC) $(ORB_TOP_HDR) $(IDLPP_IDL) $(IDLPP_SRC) $(IDLPP_HDR) $(IDLPP_ORB_SRC) $(IDLPP_ORB_HDR) : $(TOPIC_IDL)
	$(IDLPP) $(IDLPPFLAGS) $<

$(ORB_API_SRC) $(ORB_API_HDR): $(DCPS_API_IDL)
	$(ORB_SA_IDL_COMPILER) $(ORB_SA_IDL_FLAGS) $<

$(TARGET): $(OBJS) 
	$(LD_SO) -L$(SPLICE_LIBRARY_PATH) $(LD_FLAGS) $(OBJS) $(LD_LIBS) -o $(TARGET)
	-mkdir -p SACPP/include
	-mkdir -p SACPP/lib
	cp $(TOPIC_IDL) $(DCPS_API_IDL) $(ORB_TOP_HDR) $(ORB_API_HDR) $(IDLPP_ORB_HDR) $(IDLPP_HDR) SACPP/include
	cp $(TARGET) SACPP/lib
	

clean:
	-rm $(TARGET) $(OBJS) $(ORB_TOP_SRC) $(ORB_API_SRC) $(IDLPP_IDL) $(IDLPP_SRC) $(IDLPP_ORB_SRC) $(ORB_TOP_HDR) $(ORB_API_HDR) $(IDLPP_ORB_HDR) $(IDLPP_HDR)
	