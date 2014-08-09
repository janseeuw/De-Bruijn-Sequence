#De Bruijn Sequence"

In combinatorial mathematics, a __De Bruijn sequence__ _B(m, n)_ of order _n_, named after the Dutch mathematician Nicolaas Govert de Bruijn, is a cyclic sequence of a given alphabet _A_ with size _m_ for which every possible subsequence of length _n_ in _A_ appears as a sequence of consecutive characters exactly once.
Each _B(m, n)_ has length $m^n$.


The De Bruijn sequences can be constructed by taking a Hamiltonian path of an _n_-dimensional De Bruijn graph over _m_ symbols (or equivalently, a Eulerian cycle of a _(n − 1)_-dimensional De Bruijn graph), or via finite fields.

Here will we construct a _(n-1)_-dimensional De Bruijn graph and find the Euler circuit to construct the De Bruijn sequence.

##Construct the De Bruijn graph

_Example_:

Goal: to construct a _B(2, 4)_ De Bruijn sequence of length 2^4 = 16 using Eulerian (_n_ − 1 = 4 − 1 = 3) 3-dimensional De Bruijn graph cycle.

Each edge in this 3-dimensional De Bruijn graph corresponds to a sequence of four digits: the three digits that label the vertex that the edge is leaving followed by the one that labels the edge. If one traverses the edge labeled 1 from 000, one arrives at 001, thereby indicating the presence of the subsequence 0001 in the De Bruijn sequence. 

##Traverse using Euler circuit

To traverse each edge exactly once is to use each of the 16 four-digit sequences exactly once.

For example, suppose we follow the following Eulerian path through these nodes:

	000, 000, 001, 011, 111, 111, 110, 101, 011,
	110, 100, 001, 010, 101, 010, 100, 000.

These are the output sequences of length m:

0 0 0 0

_ 0 0 0 1

_ _ 0 0 1 1

This corresponds to the following De Bruijn sequence:

	0 0 0 0 1 1 1 1 0 1 1 0 0 1 0 1

