# qsim

## Introduction

qsim is a quantum circuit simulator written in C.

The user can

1. define a quantum circuit via application of gates
2. compute the state vector after applying the circuit
3. calculate the probability of output states (across all qubits, or a subrange)
4. visualize the circuit in the terminal

### To use
1. clone this repository
3. edit `main.c` to create your circuit
2. run `make`
4. run `./qsim`

## Example: Quantum Phase Estimation

qsim is able to run the canonical [Quantum Phase Estimation](https://en.wikipedia.org/wiki/Quantum_phase_estimation_algorithm) algorithm. It does so by computing a forward [Quantum Fourier Transform](https://en.wikipedia.org/wiki/Quantum_Fourier_transform) and appending its adjoint to the rest of the circuit. See `qpe()` within `src/main.c` to see how to build this circuit.

The 3-qubit quantum phase estimation circuit looks like this:

```
qubit 0 |0>-H-|CPS|---|---|SWP|---|---|---|---|---|SWP|CPS|SWP|SWP|CPS|SWP|-H-|
qubit 1 |0>-H-|CPS|CPS|---|SWP|---|SWP|CPS|SWP|-H-|SWP|CPS|SWP|SWP|CPS|SWP|---|
qubit 2 |0>-H-|CPS|CPS|CPS|SWP|-H-|SWP|CPS|SWP|---|SWP|CPS|SWP|---|---|---|---|
qubit 3 |0>-X-|CPS|CPS|CPS|---|---|---|---|---|---|---|---|---|---|---|---|---|
layer #     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  
```

Here, qubit 3 is the qubit representing the eigenvector of the unitary U, and qubits 0 - 2 are the estimation qubits. In this example, U is the phase shift operation by `+pi / 4`.

Layers 1-3 are the `C`ontrolled `P`hase `S`hift of U^4, U^2, and U, respectively (my formatter doesn't distinguish between control and target). Layers 4-16 are the inverse QFT. Measuring only qubits 0 - 2, we get the following results:

```
Measuring qubits 0 to 2: |q0q1q2>
|0> = |000> = probability 0.000000 of being 1
|1> = |001> = probability 1.000000 of being 1
|2> = |010> = probability 0.000000 of being 1
|3> = |011> = probability 0.000000 of being 1
|4> = |100> = probability 0.000000 of being 1
|5> = |101> = probability 0.000000 of being 1
|6> = |110> = probability 0.000000 of being 1
|7> = |111> = probability 0.000000 of being 1
```

We see that the state `|001>` was measured with probability 1. The binary numeral `0b0.001` is equal to 1/8 in decimal, and corresponds with `theta = 1 / 8`. Note that QPE finds the value of `theta` where `2 * pi * theta` equals the phase. So our QPE execution gave a phase of `2 * pi * (1 / 8)`, or `pi / 4`, as expected.


## Hierarchy

![architecture of qsim](/public/architecture.png)

The type hierarchy of qsim begins at the top level of a quantum circuit, which consists of layers of operations and maintains its own statevector.

Each operation is a gate acting upon one or more qubits. For some gates, such as a controlled gate (e.g. CX), their matrix representation is dependent on the qubits they act upon. qsim builds the matrices for a controlled-single-qubit-gate on the fly.

## Layering

![layering in qsim](/public/layers.png)

As operations are added to the circuit, they are grouped into layers, which represent a logical parallelization of the application of the gates. The grouping mechanism searches for an "empty slot" to place the new gate into, and creates a new one if one doesn't exist.

## Running the circuit

The statevector is initialized to all 0's as per the typical convention, and for each layer, a matrix is calculated and multiplied onto the amplitudes. The matrix is calculated using the Kronecker product of the gate matrices.

## Notes

### File Structure

```
qsim
├── include
│   ├── gate.h              // gate_t object
│   ├── layer.h             // layer_t object
│   ├── operation.h         // operation_t object
│   ├── qc.h                // high level quantum circuit object qc_t
│   ├── qmath.h             // math with complex numbers, matrices
│   └── utils.h             // utility functions
├── Makefile
├── public                  // README dependencies
│   ├── architecture.png
│   └── layers.png
├── README.md
└── src
    ├── gate.c
    ├── layer.c
    ├── main.c
    ├── operation.c
    ├── qc.c
    ├── qmath.c
    └── utils.c
```

### Learning points

1. `make` for building and then cleaning the repository
2. `leaks` for testing for memory leaks
3. `lldb` for tracing double-frees
3. building dynamic structures in C
4. devising and implementing a greedy layering algorithm
5. endianness and how it relates to building the quantum circuits (qsim uses little-endian)

## References

- [Kronecker Product](https://en.wikipedia.org/wiki/Kronecker_product)
- [Matrix Representation of controlled-U gate](https://quantumcomputing.stackexchange.com/questions/9614/how-to-interpret-a-4-qubit-quantum-circuit-as-a-matrix)
- [QFT](https://en.wikipedia.org/wiki/Quantum_Fourier_transform)
- [QPE](https://pennylane.ai/qml/demos/tutorial_qpe)
- [Quirk Quantum Circuit Simulator](https://algassert.com/quirk#circuit=%7B%22cols%22:%5B%5D%7D)