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
- [x] multiqubit quantum circuits
    - [ ] 2^n statevector
- [x] implement a quantum gate
    - [x] application of quantum gate on qubit
    - [x] application of a quantum gate on subset of circuits' qubits
        - achieved by building 2^n x 2^n matrix on a layer-by-layer basis
- [x] multi-qubit state
- [ ] building quantum circuits
    - [ ] visualizing quantum circuits in stdout
- [ ] running quantum circuits
    - [ ] final statevector

## future goals (priority descending)

* comparison of various simulation methods (sparsity based, tensor networks (MPS), decision diagrams)
* implement a bloch sphere state visualizer in C
* image visualization of quantum circuit

## references

- https://en.wikipedia.org/wiki/Kronecker_product
- https://quantumcomputing.stackexchange.com/questions/9614/how-to-interpret-a-4-qubit-quantum-circuit-as-a-matrix (matrix representation of controlled-U gate)