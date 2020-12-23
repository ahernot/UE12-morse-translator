CXX=g++
CPP=C++11
CXXFLAGS=-Wall -ggdb -Iinclude
AR=/usr/bin/ar -r 

src := $(wildcard src/*.cpp)
obj := $(subst src, build, $(src:.cpp=.o)) # dans src, replace .cpp with .o


.PHONY: directories

all: directories libmorse.a morse # target par défaut
	@echo $(obj)

morse: main.cpp libmorse.a
	#$(CXX) $(CXXFLAGS) -std=$(CPP) $^ -o $@
	$(CXX) $(CXXFLAGS) $^ -o $@

libmorse.a: ${obj}
	${AR} $@ $^
# $^ = toutes les dépendances passées en entrée

# règle pour traiter les build/{}.o : do nothing if target more recent than dépendances; else re-compile
build/%.o: src/%.cpp include/%.h include/config.h
	#$(CXX) $(CXXFLAGS) -std=$(CPP) -o $@ -c $<
	$(CXX) $(CXXFLAGS) -o $@ -c $<


directories: build
build:
	mkdir -p $@
# mkdir -p build

clean: 
	rm morse
	rm libmorse.a
	rm -r build