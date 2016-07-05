// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"


// Protected Member Functions 
template<class Type>
void mousse::turbulenceFields::processField
(
  const word& fieldName,
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvalue
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> FieldType;
  const word scopedName = modelName + ':' + fieldName;
  if (obr_.foundObject<FieldType>(scopedName)) {
    FieldType& fld =
      const_cast<FieldType&>(obr_.lookupObject<FieldType>(scopedName));
    fld == tvalue();
  } else if (obr_.found(scopedName)) {
    WARNING_IN
    (
      "void mousse::turbulenceFields::processField"
      "("
      "  const word&, "
      "  const tmp<GeometricField<Type, fvPatchField, volMesh> >&"
      ")"
    )
    << "Cannot store turbulence field " << scopedName
    << " since an object with that name already exists"
    << nl << endl;
  } else {
    obr_.store
    (
      new FieldType
      {
        {
          scopedName,
          obr_.time().timeName(),
          obr_,
          IOobject::READ_IF_PRESENT,
          IOobject::NO_WRITE
        },
        tvalue
      }
    );
  }
}

