// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_writer.hpp"
#include "write_funs.hpp"
template<class Type>
void mousse::patchWriter::write
(
  const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds
)
{
  FOR_ALL(flds, fieldI)
  {
    const GeometricField<Type, fvPatchField, volMesh>& fld = flds[fieldI];
    os_ << fld.name() << ' ' << pTraits<Type>::nComponents << ' '
      << nFaces_ << " float" << std::endl;
    DynamicList<floatScalar> fField{pTraits<Type>::nComponents*nFaces_};
    FOR_ALL(patchIDs_, j)
    {
      label patchI = patchIDs_[j];
      const fvPatchField<Type>& pfld = fld.boundaryField()[patchI];
      if (nearCellValue_)
      {
        writeFuns::insert(pfld.patchInternalField()(), fField);
      }
      else
      {
        writeFuns::insert(pfld, fField);
      }
    }
    writeFuns::write(os_, binary_, fField);
  }
}
template<class Type>
void mousse::patchWriter::write
(
  const PtrList<GeometricField<Type, pointPatchField, pointMesh> >& flds
)
{
  FOR_ALL(flds, fieldI)
  {
    const GeometricField<Type, pointPatchField, pointMesh>& fld =
      flds[fieldI];
    os_ << fld.name() << ' ' << pTraits<Type>::nComponents << ' '
      << nPoints_ << " float" << std::endl;
    DynamicList<floatScalar> fField(pTraits<Type>::nComponents*nPoints_);
    FOR_ALL(patchIDs_, j)
    {
      label patchI = patchIDs_[j];
      const pointPatchField<Type>& pfld = fld.boundaryField()[patchI];
      writeFuns::insert(pfld.patchInternalField()(), fField);
    }
    writeFuns::write(os_, binary_, fField);
  }
}
template<class Type>
void mousse::patchWriter::write
(
  const PrimitivePatchInterpolation<primitivePatch>& pInter,
  const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds
)
{
  FOR_ALL(flds, fieldI)
  {
    const GeometricField<Type, fvPatchField, volMesh>& fld = flds[fieldI];
    os_ << fld.name() << ' ' << pTraits<Type>::nComponents << ' '
      << nPoints_ << " float" << std::endl;
    DynamicList<floatScalar> fField{pTraits<Type>::nComponents*nPoints_};
    FOR_ALL(patchIDs_, j)
    {
      label patchI = patchIDs_[j];
      const fvPatchField<Type>& pfld = fld.boundaryField()[patchI];
      if (nearCellValue_)
      {
        writeFuns::insert
        (
          pInter.faceToPointInterpolate
          (
            pfld.patchInternalField()()
          )(),
          fField
        );
      }
      else
      {
        writeFuns::insert
        (
          pInter.faceToPointInterpolate(pfld)(),
          fField
        );
      }
    }
    writeFuns::write(os_, binary_, fField);
  }
}
