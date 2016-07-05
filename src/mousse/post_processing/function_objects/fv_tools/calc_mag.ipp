// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


template<class FieldType>
FieldType& mousse::calcMag::magField
(
  const word& magName,
  const dimensionSet& dims
)
{
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  if (!mesh.foundObject<FieldType>(magName)) {
    FieldType* magFieldPtr
    {
      new FieldType
      {
        {
          magName,
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh,
        {"zero", dims, 0.0}
      }
    };
    mesh.objectRegistry::store(magFieldPtr);
  }
  const FieldType& f = mesh.lookupObject<FieldType>(magName);
  return const_cast<FieldType&>(f);
}


// Member Functions 
template<class Type>
void mousse::calcMag::calc
(
  const word& fieldName,
  const word& /*resultName*/,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  if (mesh.foundObject<vfType>(fieldName)) {
    const vfType& vf = mesh.lookupObject<vfType>(fieldName);
    volScalarField& field =
      magField<volScalarField>(resultName_, vf.dimensions());
    field = mag(vf);
    processed = true;
  } else if (mesh.foundObject<sfType>(fieldName)) {
    const sfType& sf = mesh.lookupObject<sfType>(fieldName);
    surfaceScalarField& field =
      magField<surfaceScalarField>(resultName_, sf.dimensions());
    field = mag(sf);
    processed = true;
  }
}

