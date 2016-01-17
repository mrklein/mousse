// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_mesh_writer.hpp"
#include "write_funs.hpp"
template<class Type>
mousse::tmp<Field<Type> > mousse::surfaceMeshWriter::getFaceField
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& sfld
) const
{
  const polyBoundaryMesh& patches = sfld.mesh().boundaryMesh();
  tmp<Field<Type> > tfld(new Field<Type>(pp_.size()));
  Field<Type>& fld = tfld();
  FOR_ALL(pp_.addressing(), i)
  {
    label faceI = pp_.addressing()[i];
    label patchI = patches.whichPatch(faceI);
    if (patchI == -1)
    {
      fld[i] = sfld[faceI];
    }
    else
    {
      label localFaceI = faceI - patches[patchI].start();
      fld[i] = sfld.boundaryField()[patchI][localFaceI];
    }
  }
  return tfld;
}
template<class Type>
void mousse::surfaceMeshWriter::write
(
  const PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >& sflds
)
{
  FOR_ALL(sflds, fieldI)
  {
    const GeometricField<Type, fvsPatchField, surfaceMesh>& fld =
      sflds[fieldI];
    os_ << fld.name() << ' ' << pTraits<Type>::nComponents << ' '
      << pp_.size() << " float" << std::endl;
    DynamicList<floatScalar> fField{pTraits<Type>::nComponents*pp_.size()};
    writeFuns::insert(getFaceField(fld)(), fField);
    writeFuns::write(os_, binary_, fField);
  }
}
