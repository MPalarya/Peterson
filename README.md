# Peterson's Algorithm

As part of "Techniques for Improving Software Productivity' course we have analyzed Peterson's Algorithm using CBMC and Dafny.

Peterson's Algorithm is a concurrent programming algorithm for mutual exclusion that allows two processes to safely share a single resource.

Based on a pseudo code found in Wikipedia, and a C implementation from Github, we used Bounded Model Checker for C (CBMC) to check for correctness of the implementation and in order to find pontetial bugs and correct them.

Using an existing Dafny-based proof of correctness for the simplified version of the algorithm, as was presented at Washington University on January 9, 2017, we tried to generalize the algorithm and prove its mutual exclusion for an n-Processes scenario.
We were looking to find an inductive invariant which proves that any two threads cannot enter the critical section simultaneously in an N-Processes problem.
