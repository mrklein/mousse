# Difference from original wmake

Initially `_make` instead of `Make` was selected as it was always in the
beginning of the folder listing. At that time I had rather vague idea of
OpenFOAM's love to put file with the same snake\_case\_names in the same
folder (like codedSource.H and CodedSource.H).

Since limiting case of snake case is `_a` instead of `A`, the following changes
were made to original `wmake`:

- In case of executables snake-case is formed with `-` instead of `_`.
- `file` and `options` files should be in `_make` folder.
- `wmake-lninclude` creates `_lninclude` folder with linked files (maybe to
  avoid too many open files I should also change `xargs` call to `for` cycle,
  though it could be slower).
- Rules and MPI library types are reduced to the list of OSes I can perform
  testing on.
