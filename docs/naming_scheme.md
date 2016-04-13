# Files

## snake\_case

Initial idea of case insensitive change was to just go with snake case. Though
easy (see `utils` folder), at certain point two similar folder names or two
similar file names appear in the same folder (or later they are linked in the
same `_lninclude` folder).

Since it turned out there is no uniform naming policy for files and classes in
OpenFOAM (it seems like there is a policy to start template classes from
capital and concrete classes with small letter, yet there is concrete Time
class), initially all classes were transformed form `camelCase` to
`snake_case`. Later `TitleCase` classes were renamed to `_title_case` (adapting
Mercurial's way of dealing with capital letters).

There is certain ambiguity with cases like `iofield` and `scalar_io_field`.
Currently underline position is somewhat arbitrary, later more consistent
scheme should be elaborated.

## Binary file namespaces

All libraries should be placed into `libmousse_` namespace.

All executables should be placed into `mousse-` namespace. Currently they are
placed into `$MOUSSE_APPBIN` folder. After introduction of facade application
base executables will be migrated into `libexec` folder.

## Source file extensions

File extensions:

* hpp: header file with definitions (and maybe inline implementations), which
  is meant to be included in the beginning of the implementation file.
* cpp: something meant to be compiled, either into object file or into
  executable.
* ipp: template implementations meant to be included into headers.
* inc: files just for textual expansion, can be included anywhere in source
  files.

Unfortunately OpenFOAM uses only two extensions (C and H), so to distinguish
files it uses quite eclectic file name prefixes: `Templates` for template
implementations if .C file is used for template specialisation, or sometimes
`Name` for specialisation, when .C contains template implementations. In
addition to case-insensitive extensions we get a way to easily deduce aim of
a file, and to avoid novice errors, ex. including .H file with declarations
inside `main`.

## Scripts

Preferred language for scripts is Python (using version 2.6 features). Scripts
have `mousse-` prefix and py or no extension.

# Tutorials

Tutorials have tree-like structure similar to `applications/solvers` folder.
Every tutorial case should contain: configuration/IC/BC files, allrun
& allclean scripts, (ideally) document describing the case.

