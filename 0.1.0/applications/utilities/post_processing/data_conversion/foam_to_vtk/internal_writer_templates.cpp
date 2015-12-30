// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "internal_writer.hpp"
#include "write_funs.hpp"
template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::internalWriter::write
(
  const PtrList<GeometricField<Type, PatchField, GeoMesh> >& flds
)
{
  forAll(flds, i)
  {
    writeFuns::write(os_, binary_, flds[i], vMesh_);
  }
}
template<class Type>
void mousse::internalWriter::write
(
  const volPointInterpolation& pInterp,
  const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds
)
{
  forAll(flds, i)
  {
    writeFuns::write
    (
      os_,
      binary_,
      flds[i],
      pInterp.interpolate(flds[i])(),
      vMesh_
    );
  }
}
