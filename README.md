# TODO: Project's Name

## Pre-requisites

There are three main steps to the algorithm.

* Generation of Task Graphs
* Tabu Search
* Results analysis

### Generation of Task Graph
For generating the Task Graphs,
Task Graphs For Free (TGFF) was used.
The source code can be found on
[https://robertdick.org/projects/tgff/index.html](https://robertdick.org/projects/tgff/index.html).
The version used was **3.6**.
More information about TGFF can be found in
task-graphs' directory.

## MAKE

If problems occur during the `make`,
run the command again after converting the file lists
(`_DEPS` and `_OBJS`)
from multiple to single line.
This can be done by deleting the backslashes (\\) and the newlines.

Standard:
```
make
./gftinoc
```

Debugging with GDB:
```
make debug
gdb debug
```
