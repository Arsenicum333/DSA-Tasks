# Data Structures and Algorithms (DSA)

## Introduction
This repository contains implementations of various Data Structures and Algorithms (DSA) tasks in C. Each task solves a specific problem to demonstrate key algorithmic concepts. Below is the description of the tasks, what they do, and the Algorithms or Data Structures used.

## Task 1: Greatest Common Divisor
- **Description**: The program reads a number `N` followed by `N` pairs of integers. For each pair, it calculates the Greatest Common Divisor (GCD) and outputs the results, with each GCD on a new line.
- **Algorithms**:
  - **Euclidean Algorithm**: Repeatedly divides the larger number by the smaller one, using the remainder to update the numbers until one becomes zero. The non-zero number is the GCD.

## Task 2: Generating Prime Numbers
- **Description**: The program reads two integers, `min` and `max`, and outputs all prime numbers in the range `[min, max]`, separated by commas.
- **Algorithms**:
  - **Sieve of Eratosthenes**: Marks multiples of each prime number starting from 2 as non-prime in a boolean array, efficiently identifying all prime numbers up to `max`.

## Task 3: Searching Rational Numbers
- **Description**: The program reads `N` rational numbers (integers or decimals) and `M` query numbers. For each query number, it searches for its position in the sorted array of `N` numbers and outputs the 1-based index (or 0 if not found).
- **Algorithms**:
  - **Binary Search**: Divides the search space in half repeatedly to locate the target rational number in a sorted array by comparing combined integer and fractional parts.

## Task 4: Brackets
- **Description**: The program reads `N` strings of brackets (parentheses, square, curly, or angle). For each string, it counts the number of bracket errors, where an error is an unmatched or incorrectly matched bracket, and outputs the results, with each count on a new line.
- **Algorithms**:
  - **Stack-Based Bracket Matching**: Uses a stack to track opening brackets and checks if each closing bracket matches the most recent opening bracket, counting mismatches and unpaired brackets as errors.

## Task 5: Search Trees and Hash Tables
- **Description**: The program implements three Data Structures to manage records with an ID, first name, and last name. It processes insertion, search, and deletion commands from a file and outputs performance metrics for these operations.
- **Data Structures**:
  - **Weight-Balanced Tree**: A binary search tree that maintains balance based on subtree sizes.
  - **Treap Tree**: A hybrid of a binary search tree and a heap with random priorities for nodes.
  - **Hash Table**: An array-based structure using double hashing for collision resolution.

## Task 6: Binary Decision Diagrams
- **Description**: The program constructs Binary Decision Diagrams (BDDs) from Disjunctive Normal Form (DNF) expressions, evaluates them against all possible input combinations, and optimizes variable ordering to minimize BDD size. It outputs performance metrics for these operations.
- **Data Structures**:
  - **Binary Decision Diagram (BDD)**: A directed acyclic graph representing a boolean function with nodes for variables and leaves for truth values.
  - **Hash Table**: Used to cache BDD nodes for efficient node reuse during construction.
- **Algorithms**:
  - **Fisher-Yates Shuffle**: Randomly shuffles variable orderings to test different configurations and select the one yielding the smallest BDD.