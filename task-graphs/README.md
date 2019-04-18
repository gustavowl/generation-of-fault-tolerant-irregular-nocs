# Task Graphs

- [x] Generate .tgffopt files;
- [x] Create script to convert .tgff files to adjacency **list**;
- [ ] Create script to convert adjacency **list** to adjacency **matrix**.

## Jonathan Graphs
This directory contains Task Graphs used in
a previous work developed by Jonathan Wanderley.

**TODO: Add link to work and his personal website.**

## Generation of Task Graph
For generating the Task Graphs,
Task Graphs For Free (TGFF) was used.
The source code can be found on
[https://robertdick.org/projects/tgff/index.html](https://robertdick.org/projects/tgff/index.html).
The version used was **3.6**.

Download the source code and save it in
`tgff-3.6` directory.
Follow TGFF's documentation to properly install and `make` it.

## Running the script
Assert that the current directory is `task-graphs`.
```
cd path/to/repository/task-graphs
```
Then run the `generate-tgs.sh` script.
```
./generate-tgs.sh
```

### Script Output
After executing the commands from the previous section,
three directories will be created:
`descriptions/`, `adj-lists/`, and `eps/`.

The `descriptions/` directory contains all generated files,
separated in other directories labeled according to
the parameters used by `generate-tgs.sh`.

Gathering files of the same type in a single directory may help in future analyses.
Namely, `adj-lists/`, and `eps/` will contain `.adjl`, and `.eps` files, respectively.
