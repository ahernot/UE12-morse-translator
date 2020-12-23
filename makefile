CXX=g++
CPP=C++11
CXXFLAGS=-Wall -ggdb -Iinclude
AR=/usr/bin/ar -r 

src := $(wildcard src/*.cpp)
obj := $(subst src, build, $(src:.cpp=.o))


.PHONY: directories

all: directories libmorse.a morse
	@echo $(obj)

morse: main.cpp libmorse.a
	$(CXX) $(CXXFLAGS) $^ -o $@

libmorse.a: ${obj}
	${AR} $@ $^

build/%.o: src/%.cpp include/%.h include/config.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<


directories: build
build:
	mkdir -p $@

clean: 
	rm morse
	rm libmorse.a
	rm -r build