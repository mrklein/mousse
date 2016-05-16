// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "partial_write.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"


template<class Type>
void mousse::partialWrite::loadField
(
  const word& fieldName,
  UPtrList<GeometricField<Type, fvPatchField, volMesh>>& vflds,
  UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh>>& sflds
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;
  if (obr_.foundObject<vfType>(fieldName)) {
    if (debug) {
      Info << "partialWrite : Field "
        << fieldName << " found in database" << endl;
    }
    vfType& vField =
      const_cast<vfType&>
      (
        obr_.lookupObject<vfType>(fieldName)
      );
    const unsigned int sz = vflds.size();
    vflds.setSize(sz + 1);
    vflds.set(sz, &vField);
  } else if (obr_.foundObject<sfType>(fieldName)) {
    if (debug) {
      Info << "partialWrite : Field " << fieldName
        << " found in database" << endl;
    }
    sfType& sField =
      const_cast<sfType&>
      (
        obr_.lookupObject<sfType>(fieldName)
      );
    const unsigned int sz = sflds.size();
    sflds.setSize(sz + 1);
    sflds.set(sz, &sField);
  }
}


template<class Type>
void mousse::partialWrite::changeWriteOptions
(
  UPtrList<GeometricField<Type, fvPatchField, volMesh>>& vflds,
  UPtrList<GeometricField<Type, fvsPatchField, surfaceMesh>>& sflds,
  const IOobject::writeOption wOption
) const
{
  FOR_ALL(vflds , i) {
    vflds[i].writeOpt() = wOption;
  }
  FOR_ALL(sflds , i) {
    sflds[i].writeOpt() = wOption;
  }
}

