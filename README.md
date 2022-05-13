# C-Stack-Recursion

Small program whose aim is to test handling usual recursive function calls, without using language features, but instead by constructing a program stack, as would happen in assembly.

Practical applications would be in languages which don't natively support recursion, such as OpenCL, though handling of jumping to addresses could be done better, as currently the `&&` operator is a GCC specific extension.

## Compilation

Barebones project, only requires GCC, the C standard library and Make.

```
make
```
