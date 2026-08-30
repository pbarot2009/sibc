# sibC

sibc is a C compiler written in pure C. It checks standard C code for memory bugs and undefined behavior at compile time, runs constant folding and dead code elimination, and outputs clean C17 source code ready for GCC or Clang.

## Build from source

Requires `clang` or `gcc`.

```bash
git clone https://github.com/pbarot2009/sibc.git
cd sibc
chmod +x build.sh

# Build debug binary
./build.sh

# Build release binary
./build.sh release

# Clean build files
./build.sh clean
```

## author

- **Github**: [pbarot2009](https://github.com/pbarot2009)
- **Youtube**: [pbarot2009](https://youtube.com/@pbarot2009)

## license

Licensed under the [Apache 2.0 License](LICENSE). 

## version

`0.0.1-dev`
