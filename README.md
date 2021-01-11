# calc-cpp

My first C++ project.

An implementation of http://jorendorff.github.io/calc/docs/calculator-parser.html written in C++.

## Usage

```
$ make
$ ./calc-cpp "1 + 1"
type: +
  left: { type: number, value: 1 }
  right: { type: number, value: 1 }
```

## Todo

- [x] Implement `display` function.
- [ ] Add tests.
- [x] Use a build tool.
