// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "read_fields.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"


template<class Type>
void mousse::readFields::loadField
(
  const word& fieldName,
  PtrList<GeometricField<Type, fvPatchField, volMesh>>& vflds,
  PtrList<GeometricField<Type, fvsPatchField, surfaceMesh>>& sflds
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;
  if (obr_.foundObject<vfType>(fieldName)) {
    if (debug) {
      Info << "readFields : Field " << fieldName << " already in database"
        << endl;
    }
  } else if (obr_.foundObject<sfType>(fieldName)) {
    if (debug) {
      Info << "readFields : Field " << fieldName << " already in database"
        << endl;
    }
  } else {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    IOobject fieldHeader
    {
      fieldName,
      mesh.time().timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    };
    if (fieldHeader.headerOk()
        && fieldHeader.headerClassName() == vfType::typeName) {
      // store field locally
      Info << "    Reading " << fieldName << endl;
      label sz = vflds.size();
      vflds.setSize(sz+1);
      vflds.set(sz, new vfType{fieldHeader, mesh});
    } else if (fieldHeader.headerOk()
               && fieldHeader.headerClassName() == sfType::typeName) {
      // store field locally
      Info << "    Reading " << fieldName << endl;
      label sz = sflds.size();
      sflds.setSize(sz+1);
      sflds.set(sz, new sfType{fieldHeader, mesh});
    }
  }
}

