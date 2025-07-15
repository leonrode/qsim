# qsim

## Introduction

qsim is a small quantum circuit simulator written in C.

The user can

1. define a quantum circuit via application of gates
2. compute the state vector after applying the circuit
3. calculate the probabilty of the state of a qubit after measurement
4. visualize the circuit in the terminal

### To use
1. clone this repository
3. edit `main.c` to create your circuit
2. run `make`
4. run `./qsim`

## Hierarchy

![architecture of qsim](/public/architecture.png)

The type hierarchy of qsim begins at the top level of a quantum circuit, which consists of layers of operations and maintains its own statevector.

Each operation is a gate acting upon one or more qubits. For some gates, such as a controlled gate (e.g. CX), their matrix representation is dependent on the qubits they act upon. qsim builds the matrices for a controlled-single-qubit-gate on the fly.

## Layering

![layering in qsim](/public/layers.png)

As operations are added to the circuit, they are grouped into layers, which reprsent a logical parallelization of the application of the gates. The grouping mechanism searches for an "empty slot" to place the new gate into, and creates a new one if one doesn't exist.

## Running the circuit

The statevector is initialized to all 0's as per the typical convention, and for each layer, a matrix is calculated and multiplied onto the amplitudes. The matrix is calculated using the Kronecker product of the gate matrices.

## Notes

### Learning points

1. `make` for building and then cleaning the repository
2. `leaks` for testing for memory leaks
3. building dynamic structures in C
4. devising and implementing a greedy layering algorithm

### Progress Tracking

- [x] a lot of linear algebra
- [x] implement a qubit
- [x] multiqubit quantum circuits
    - [x] 2^n statevector
- [x] implement a quantum gate
    - [x] application of quantum gate on qubit
    - [x] application of a quantum gate on subset of circuits' qubits
        - achieved by building 2^n x 2^n matrix on a layer-by-layer basis
- [x] multi-qubit state
- [x] building quantum circuits
    - [x] circuit layering
        - involves a greedy algorithm to group the operations by layer
    - [ ] visualizing quantum circuits in stdout
        - [ ] visualize control/target qubit
- [x] running quantum circuits
    - [x] final statevector
    - [x] qubit probabilities
- [x] run Deutsch-Josza 
    - 2-qubit case with CX for the balanced function unitary and I for constant function unitary

## References

- https://en.wikipedia.org/wiki/Kronecker_product
- https://quantumcomputing.stackexchange.com/questions/9614/how-to-interpret-a-4-qubit-quantum-circuit-as-a-matrix (matrix representation of controlled-U gate)