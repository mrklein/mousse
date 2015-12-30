Initial idea of case insensitive change was to just go with snake case. Though
easy (see `utils` folder) at certain point two similar folder names of two
similar file names appear in the same folder (or later they are linked in the
same `_lninclude` folder).

Since it turned out there is no adapted naming policy for files and classes in
OpenFOAM (though it seems like there is a policy to start template classes from
capital and concrete classes from small letter, there is concrete Time class;
and in fact OpenFOAM is rather inhomogeneous in terms of naming policy),
initially all classes were transformed form `camelCase` to `snake_case`. Later
`TitleCase` classes were renamed to `_title_case` (adapting Mercurial's way of
dealing with capital letters).

There is certain ambiguity with cases like `iofield` and `scalar_io_field`.
Currently underline position is somewhat arbitrary, later more consistent
scheme should be elaborated.
