// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_interpolate_fields.hpp"
#include "vol_fields.hpp"
#include "linear.hpp"
template<class Type>
void mousse::surfaceInterpolateFields::interpolateFields
(
  PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >& sflds
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;
  // Convert field to map
  HashTable<word> fieldMap(2*fieldSet_.size());
  FOR_ALL(fieldSet_, i)
  {
    fieldMap.insert(fieldSet_[i].first(), fieldSet_[i].second());
  }
  HashTable<const vfType*> flds(obr_.lookupClass<vfType>());
  FOR_ALL_CONST_ITER(typename HashTable<const vfType*>, flds, iter)
  {
    const vfType& fld = *iter();
    if (fieldMap.found(fld.name()))
    {
      //const word sName = "interpolate(" + fld.name() + ')';
      const word& sName = fieldMap[fld.name()];
      if (obr_.found(sName))
      {
        Info<< "        surface field " << sName << " already exists"
          << endl;
      }
      else
      {
        label sz = sflds.size();
        sflds.setSize(sz+1);
        sflds.set(sz, new sfType(sName, linearInterpolate(fld)));
        Info<< "        interpolated " << fld.name() << " to create "
          << sflds[sz].name() << endl;
      }
    }
  }
}
