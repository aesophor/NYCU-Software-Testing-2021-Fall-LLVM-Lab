# NYCU Software Testing 2021 Fall: Lab 5

Spec: https://docs.google.com/presentation/d/1el6sU4BIT3MGymi-CHaDVdE1zgJ5EpA9SKI2AesycLk/edit?usp=sharing

```
> make
clang++ `llvm-config --cxxflags` `llvm-config --ldflags` `llvm-config --libs` -fno-rtti -fPIC -std=c++20 -shared -o Pass.dylib src/Pass.cc
clang `llvm-config --ldflags` -flegacy-pass-manager -Xclang -load -Xclang Pass.dylib -o target target.c
runOnModule
main
```

```
> ./target 1
debug mode
argc: 9487
argv[1]: aesophor is ghost !!!
Omg! Why your argc is so large ?
Yes, aesophor is ghost !
```
