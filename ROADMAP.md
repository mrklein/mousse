# 0.1.0: PoC: sources

  - [X] Fork OpenFOAM 3.0.x (commit: 195caf7)
  - [X] Case insensitive naming scheme for
      - [X] source files
      - [X] wmake
  - [X] Build on case insensitive file system
  - [X] Use deleted methods instead of private undefined default bitwise copy
  constructors and assignment operators
  - [X] Use UPPER\_CASE for preprocessor macros
  - [X] Get rid of \_i header files (merge with including header)
  - [ ] Change extension of the headers used only for preprocessor expansion to
  \*.inc

Since the branch is just a proof of concept, solver, utilities, and tutorial
cases are imported just for `basic` type. More solvers could be imported in
later releases, yet for certain solvers I have no validation cases except
tutorial ones.

# 0.2.0: another PoC: build and facade

  - [ ] Change build software to cmake
  - [ ] Get rid of NoRepository definition
  - [ ] Respect FSH
      - [ ] move libraries to lib
      - [ ] move binaries to libexec
      - [ ] API (so headers could be installed to include) (?)
      - [ ] write facade application mousse
  - [ ] Port scripts to Python 2.6.x (2.4.x would be nice since it is in LSB
  but no thanks)

# 0.3.0: XML

  - [ ] Move case description from free-form dictionaries to XML with schema

# 0.4.0: HDF5

  - [ ] Move data storage from OpenFOAM dictionaries to HDF5 (Xdmf?)

# 0.5.0: Validation

  - [ ] Develop validation/verification schema
  - [ ] Import NASA verification/validation cases
  - [ ] Create common/obvious verification/validation cases
