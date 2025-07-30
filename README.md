# RON: One-Way Circular Shortest Routing to Achieve Efficient and Bounded-waiting Spinlocks

## Paper
- [RON: One-Way Circular Shortest Routing to Achieve Efficient and Bounded-waiting Spinlocks (OSDI 2023)](https://www.usenix.org/conference/osdi23/presentation/lo)

## Overview

Routing on Network-on-chip (RON) is a method that minimizes the communication cost between cores by using a routing table and pre-calculating an optimized locking-unlocking order. RON delivers locks and data in a one-way circular manner among cores to minimize global data movement cost and achieve bounded waiting time.

## Kernel space

To test locks in the kernel space, use a patch in the `klocks` folder. The patch is written on linux kernel v5.12.1.

1. Download Linux kernel v5.12.1 vai the URL https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.12.1.tar.xz or by running the following Git command:

```bash
$ git clone --branch v5.12 https://github.com/torvalds/linux
$ cd linux
```

2. Use `ron.patch` to patch `/locking/qspinlock.c`

```bash
$ patch -p1 < <path_to_patch>
```

## User space

We extend the `Litl` framework (`ulocks`) ad use the `LD_PRELOAD` for userspace benchmarks.

### Supported algorithms

| Name     | Name in the Paper |
|----------|-------------------|
| ron      | RON               |
| ronplock | RON-plock         |
| rontick  | RON-ticket        |

### How to run

Compile the `Litl` framework as follows:
```
$ make -C ulocks
```

If you want to use `ronplock`, do the following:

```
$ ./libronplock_original.sh my_program
```

You cna find more details in `ulocks/README.md`.

## Customize TSP_ORDER based on your own processor

1. Obtain the cost of communication between cores from the following source:
(https://github.com/nviennot/core-to-core-latency)

2. Generate the shortest path using Google OR Tools. Refer to the documentation available at
(https://developers.google.com/optimization)

3. For these four files (ron.c, ronplock.c, rontick.c, qspinlock.c), write the shortest path into an array. Taking AMD Ryzen Threadripper 2990WX as an example, the array format should be:

```
int idCov[64] = { 17, 48, 22,  6, 29, 50, 58, 55,
                  35,  4, 26, 10, 28, 40, 12, 15,
                  57, 46, 54, 63, 25, 61, 37,  0,
                  14, 44, 30, 49, 52,  1,  9, 38,
                   7, 34, 60, 33, 24, 20, 59, 41,
                   5,  8, 45, 13, 43, 31, 36, 21,
                  19, 23, 51,  3, 11, 32, 27, 39,
                  62, 18, 42, 47, 53, 16, 56,  2 };
```