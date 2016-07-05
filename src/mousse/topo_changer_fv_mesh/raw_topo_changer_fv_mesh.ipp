// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "raw_topo_changer_fv_mesh.hpp"
#include "time.hpp"


// Member Functions 
template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::rawTopoChangerFvMesh::setUnmappedValues
(
  GeometricField<Type, PatchField, GeoMesh>& fld,
  const PackedBoolList& mappedFace,
  const GeometricField<Type, PatchField, GeoMesh>& baseFld
)
{
  FOR_ALL(fld.boundaryField(), patchI) {
    PatchField<Type>& fvp =
      const_cast<PatchField<Type>&>(fld.boundaryField()[patchI]);
    const label start = fvp.patch().start();
    FOR_ALL(fvp, i) {
      if (!mappedFace[start+i]) {
        fvp[i] = baseFld.boundaryField()[patchI][i];
      }
    }
  }
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::rawTopoChangerFvMesh::zeroUnmappedValues
(
  const PackedBoolList& mappedFace
) const
{
  typedef GeometricField<Type, PatchField, GeoMesh> FieldType;
  const wordList fldNames{names(FieldType::typeName)};
  FOR_ALL(fldNames, i) {
    FieldType& fld =
      const_cast<FieldType&>(lookupObject<FieldType>(fldNames[i]));
    setUnmappedValues
    (
      fld,
      mappedFace,
      FieldType
      {
        {
          "zero",
          time().timeName(),
          *this,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        *this,
        {"0", fld.dimensions(), pTraits<Type>::zero}
      }
    );
  }
}

