# Concurrent-List
Concurrent ordered singly linked list implementation with C++. 

This project contains two concurrent list implementation using template class with C++ that provide `contain(T) add(T) remove(T)` interface, which only these operations support concurrent operating. The implementations provide multi-threaded concurrent access using `pthread_mutex` and adopt fine-grained synchronization (per node mutex) in `ConcurrentList.hpp` and optimistic synchronization in `ConcurrentListOpt.hpp`.

Notice the concurrent list only hold one copy for repeat elements, the optimistic implementation suffers heavily starvation in the attached test.

## Usage
You need clone the whole project, build it and run it. The parameter adjustment is only available in source code level at this time.
```
$ make
$ make run
```
The output is list elements and list length

## TODO
- use more sophisticated rand generator
- control each operation frequency
- count each operation numbers
- implement lock-free concurrent list
