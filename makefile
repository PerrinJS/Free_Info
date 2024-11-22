CC = gcc
CPP = g++
CFLAGS = -Wall -Werror -g
CPPFLAGS = $(CFLAGS) -std=c++23
LUAFLAGS = -llua -L../lua -I../lua -lm -ldl
EXE = freeinfo
HEADERS = FreeInfo.h InfoGetters.hpp InfoStrParser.hpp argparse/include/argparse/argparse.hpp

$(EXE) : InfoGetters.o FreeInfo.o LuaScriptRunner.o
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LUAFLAGS)

FreeInfo.o : FreeInfo.cpp InfoGetters.o LuaScriptRunner.o $(HEADERS)
	$(CPP) $(CPPFLAGS) $< -c -o $@

InfoGetters.o : InfoGetters.c LuaScriptRunner.o FreeInfo.h LuaScriptRunner.h
	$(CC) $(CFLAGS) $< -c -o $@

LuaScriptRunner.o : LuaScriptRunner.c LuaScriptRunner.h
	$(CC) $(CFLAGS) $< -c -o $@

install : $(EXE)
	mv $< ~/.logingreeting

clean : 
	rm -rf *.o freeinfo
