#####################################################################
#
#  Name:         Makefile
#  Created by:   Stefan Ritt
#
#  Contents:     Makefile for MIDAS example frontend and analyzer
#
#  $Id$
#
#####################################################################
#
#--------------------------------------------------------------------
# The MIDASSYS should be defined prior the use of this Makefile
ifndef MIDASSYS
missmidas::
	@echo "...";
	@echo "Missing definition of environment variable 'MIDASSYS' !";
	@echo "...";
endif

# get OS type from shell
OSTYPE = $(shell uname)

#--------------------------------------------------------------------
# The following lines contain specific switches for different UNIX
# systems. Find the one which matches your OS and outcomment the 
# lines below.

#-----------------------------------------
# This is for Linux
ifeq ($(OSTYPE),Linux)
OSTYPE = linux
endif

ifeq ($(OSTYPE),linux)
OS_DIR = linux
OSFLAGS = -DOS_LINUX -Dextname -DLINUX
CFLAGS = -g -O2 -Wall
# add to compile in 32-bit mode
# OSFLAGS += -m32
LIBS = -lm -lz -lutil -lnsl -lpthread -lrt
endif

#-----------------------
# MacOSX/Darwin is just a funny Linux
#
ifeq ($(OSTYPE),Darwin)
OSTYPE = darwin
endif

ifeq ($(OSTYPE),darwin)
OS_DIR = darwin
FF = cc
OSFLAGS = -DOS_LINUX -DOS_DARWIN -DHAVE_STRLCPY -fPIC -Wno-unused-function
LIBS = -lpthread -lz
NEED_STRLCPY=
NEED_RANLIB=1
NEED_SHLIB=
NEED_RPATH=

endif

#-----------------------------------------
# ROOT flags and libs
#
ifdef ROOTSYS
#ROOTCFLAGS := $(shell root-config --version 2> /dev/null) 
ROOTCFLAGS := $(shell  $(ROOTSYS)/bin/root-config --cflags)
ROOTCFLAGS += -DHAVE_ROOT -DUSE_ROOT
#ROOTLIBS   := $(shell root-config --libs) 
ROOTLIBS   := $(shell  $(ROOTSYS)/bin/root-config --libs) -Wl,-rpath,$(ROOTSYS)/lib
ROOTLIBS   += -lThread
else
missroot:
	@echo "...";
	@echo "Missing definition of environment variable 'ROOTSYS' !";
	@echo "...";
endif
#-------------------------------------------------------------------
# The following lines define directories. Adjust if necessary
#                 
INC_DIR   = $(MIDASSYS)/include
#LIB_DIR   = $(MIDASSYS)/$(OS_DIR)/lib
#else
LIB_DIR	  = $(MIDASSYS)/lib
#endif

LIB	  = $(LIB_DIR)/libmidas.a

 #$(OS_DIR)/lib
SRC_DIR   = $(MIDASSYS)/src
USR_INC   = $(HOME)/online/midas_daq/include
USR_SRC   = $(HOME)/online/midas_daq/src
ROOTANA_INC = -I/home/daqcontrol/packages/rootana/libMidasInterface


###############################################################
#equipment select 
#EQU_FLAG = -DUSE_PPAC_F1 -DUSE_PPAC_F2 #F2 trigger 
#EQU_FLAG =  -DUSE_PLASTIC_F1 #F1 plastic_trigger
#EQU_FLAG = -DUSE_PPAC_F1 -DUSE_PPAC_F2 #F1 trigger #F2  
#EQU_FLAG = -DUSE_PPAC_F2 #F2 trigger, ignore F1

#EQU_FLAG = -DUSE_PPAC_F3 #F1 trigger #F2  
#EQU_FLAG = -DUSE_PPAC_F1 -DUSE_PPAC_F2 -DUSE_PPAC_F3
#EQU_FLAG = -DUSE_SILICON_F3 #F3 silicon_trigger 
#EQU_FLAG= -DUSE_PLASTIC_F3
#EQU_FLAG= -DUSE_PPAC_F3 -DUSE_SILICON_F3
EQU_FLAG= -DUSE_PPAC_F3 
##############################################################


#-------------------------------------------------------------------
# List of analyzer modules
#
MODULES = f1_ppac_anal.o f1_plastic_anal.o f2_ppac_anal.o f3_silicon_anal.o f3_plastic_anal.o f3_ppac_anal.o
MFE_MODULES = mfe_common.o mfe_f1_plastic.o mfe_f1_ppac.o mfe_f2_ppac.o mfe_f3_silicon.o mfe_f3_plastic.o mfe_f3_ppac.o 

#-------------------------------------------------------------------
# Frontend code name defaulted to frontend in this example.
# comment out the line and run your own frontend as follow:
# gmake UFE=my_frontend
#
UFE = frontend
UANA = analyzer
MIDAS2ROOT = midas2root

####################################################################
# Lines below here should not be edited
####################################################################

# MIDAS library
#LIB = $(LIB_DIR)/libmidas.a


# compiler
CC = gcc
CXX = g++ -std=c++11
#CFLAGS += -O2 -g -Wall -Wformat=2 -Wno-format-nonliteral -Wno-strict-aliasing -Wuninitialized -Wno-unused-function
CFLAGS += -I$(INC_DIR) -I$(USR_INC) -I/usr/include -I /home/daqcontrol/packages/rootana/include -I/home/daqcontrol/packages/midas/include
LDFLAGS += -L$(ROOTANASYS)/lib/librootana.a -L/home/daqcontrol/packages/rootana/lib -lrootana
COPTS	=	-fPIC -DLINUX -Wall 
DEPLIBS = -lCAENVME -lcurses -lCAENV1x90SDK -lCAENqtpSDK


#TEST:=
#TEST+= frontend
#TEST+= frontend.o
#ifdef HAVE_ROOT


all: $(UFE) $(UANA) $(MIDAS2ROOT) 


mfe_common.o: $(USR_SRC)/mfe_common.c
		$(CXX) $(COPTS) $(CFLAGS) $(OSFLAGS) -o $@ -c $<
		
mfe_f1_plastic.o: $(USR_SRC)/mfe_f1_plastic.c
		$(CXX) $(COPTS) $(CFLAGS) -DUSE_PLASTIC_F1 $(OSFLAGS) -o $@ -c $<		
		
mfe_f1_ppac.o: $(USR_SRC)/mfe_f1_ppac.cxx
		$(CXX) $(COPTS) $(CFLAGS) -DUSE_PPAC_F1 $(OSFLAGS) -o $@ -c $<		

mfe_f2_ppac.o: $(USR_SRC)/mfe_f2_ppac.cxx
		$(CXX) $(COPTS) $(CFLAGS) -DUSE_PPAC_F2 $(OSFLAGS) -o $@ -c $<
 
mfe_f3_silicon.o: $(USR_SRC)/mfe_f3_silicon.c
		$(CXX) $(COPTS) $(CFLAGS) -DUSE_SILICON_F3 $(OSFLAGS) -o $@ -c $<
		
mfe_f3_plastic.o: $(USR_SRC)/mfe_f3_plastic.c
		$(CXX) $(COPTS) $(CFLAGS) -DUSE_PLASTIC_F3 $(OSFLAGS) -o $@ -c $<

mfe_f3_ppac.o: $(USR_SRC)/mfe_f3_ppac.cxx
		$(CXX) $(COPTS) $(CFLAGS) -DUSE_PPAC_F3 $(OSFLAGS) -o $@ -c $<



#newfrontend
#frontend: $(LIB_DIR)/mfe.o frontend.o $(LIB)
#	$(CXX) -o $@ $(CFLAGS) $(OSFLAGS) $(EQI_FLAGS) $^ $(LDEFLAGS) $(LIBS)

#frontend
$(UFE).o: $(USR_SRC)/$(UFE).cxx
	$(CXX) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<

$(UFE): $(UFE).o $(MFE_MODULES) \
	$(LIB) $(LIB_DIR)/mfe.o
	$(CXX) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $(UFE) \
	$(UFE).o $(MFE_MODULES) \
	$(LIB) $(LIB_DIR)/mfe.o \
	$(LDFEFLAGS) $(LIBS) $(DEPLIBS)
##



#new o
#frontend.o: %.cxx
#	$(CXX) -o $@ -c $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $<



#analyze
f1_ppac_anal.o: $(USR_SRC)/f1_ppac_anal.cxx 
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<

f1_plastic_anal.o: $(USR_SRC)/f1_plastic_anal.cxx 
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<

f2_ppac_anal.o: $(USR_SRC)/f2_ppac_anal.cxx 
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<	

f3_silicon_anal.o: $(USR_SRC)/f3_silicon_anal.cxx 
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<

f3_plastic_anal.o: $(USR_SRC)/f3_plastic_anal.cxx 
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<

f3_ppac_anal.o: $(USR_SRC)/f3_ppac_anal.cxx 
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<

$(UANA).o: $(USR_SRC)/$(UANA).cxx 
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<


$(UANA): $(LIB) $(LIB_DIR)/rmana.o $(UANA).o $(MODULES)
	$(CXX) $(CFLAGS) $(EQU_FLAG) -o $@ $(LIB_DIR)/rmana.o $(UANA).o $(MODULES) \
	$(LIB) $(LDFLAGS) $(ROOTLIBS) $(LIBS)



$(MIDAS2ROOT).o: $(USR_SRC)/$(MIDAS2ROOT).cxx   
	$(CXX) $(ROOTCFLAGS) $(CFLAGS) $(OSFLAGS) $(EQU_FLAG) -o $@ -c $<

$(MIDAS2ROOT): $(LIB) $(MIDAS2ROOT).o 
	$(CXX) $(CFLAGS) $(EQU_FLAG) -o $@ $(MIDAS2ROOT).o \
	$(LIB) $(LDFLAGS) $(ROOTLIBS) $(LIBS)
	
	mv *.o ./obj/
			
clean::
	-rm -f $(UANA) $(UFE) $(MIDAS2ROOT) ./obj/*.o *~ \#* 
	-rm -f *.o *~ \#*
#end file




