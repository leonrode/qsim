# qsim

## introduction

qsim is a small quantum circuit simulator written in C. it is a toy project whose scope is limited to

* defining quantum circuits
* computing final statevector
* simulating probability outcomes of measurement

this list may grow as more features are included.

all fundamental code is self-written. no external libraries for linear algebra or quantum computing are used.

## todo

- [x] a lot of linear algebra
- [x] implement a qubit
- [x] implement a quantum gate
    - [x] application of quantum gate on qubit
- [x] multi-qubit state
    - complete in the form of a quantum circuit
- [ ] multi-qubit gate (cx, generic control gate)
- [x] building quantum circuits
    - [ ] visualizing quantum circuits in stdout

## future goals

* implement a bloch sphere state visualizer in C
* image visualization of quantum circuit