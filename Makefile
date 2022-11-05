CC = clang
CXX = clang++
CXXFLAGS = `llvm-config --cxxflags` `llvm-config --ldflags` `llvm-config --libs` -fno-rtti -fPIC -std=c++20 -shared

SRC = Pass.cc
BIN = Pass.dylib

all:
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC)
	$(CC) `llvm-config --ldflags` -flegacy-pass-manager -Xclang -load -Xclang $(BIN) -o target target.c
