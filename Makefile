
INSTDIR := /usr/opt/spectcl/7.0-pre1
include $(INSTDIR)/etc/SpecTcl_Makefile.include

OBJDIR := obj
SRCDIR := src


USERCXXFLAGS := \
	-I./include \
	-I$(INSTDIR)/ddasformat/include \
	-I$(INSTDIR)/unifiedformat/include \
	-std=c++17 \
	-g -O3 -Wall

USERLDFLAGS := 	\
	-L$(INSTDIR)/ddasformat/lib \
	-lDDASUnpacker -lDDASFormat -Wl,-rpath=$(INSTDIR)/ddasformat/lib 
		

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SOURCES))


SpecTcl: $(OBJECTS) 
	$(CXXLD)  -o SpecTcl $(OBJECTS) $(USERLDFLAGS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(USERCXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)


clean:
	rm -f $(OBJDIR)/*.o SpecTcl

depend:
	makedepend $(USERCXXFLAGS) $(SRCDIR)/*.cpp

help:
	echo "make                 - Build customized SpecTcl"
	echo "make clean           - Remove objects from previous builds"
	echo "make depend          - Add dependencies to the Makefile. "
