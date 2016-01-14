// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "near_wall_fields.hpp"
template<class Type>
void mousse::nearWallFields::createFields
(
  PtrList<GeometricField<Type, fvPatchField, volMesh> >& sflds
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  HashTable<const vfType*> flds(obr_.lookupClass<vfType>());
  FOR_ALL_CONST_ITER(typename HashTable<const vfType*>, flds, iter)
  {
    const vfType& fld = *iter();
    if (fieldMap_.found(fld.name()))
    {
      const word& sampleFldName = fieldMap_[fld.name()];
      if (obr_.found(sampleFldName))
      {
        Info<< "    a field " << sampleFldName
          << " already exists on the mesh."
          << endl;
      }
      else
      {
        label sz = sflds.size();
        sflds.setSize(sz+1);
        IOobject io(fld);
        io.readOpt() = IOobject::NO_READ;
        io.writeOpt() = IOobject::NO_WRITE;
        io.rename(sampleFldName);
        sflds.set(sz, new vfType(io, fld));
        Info<< "    created " << sflds[sz].name() << " to sample "
          << fld.name() << endl;
      }
    }
  }
}
template<class Type>
void mousse::nearWallFields::sampleBoundaryField
(
  const interpolationCellPoint<Type>& interpolator,
  GeometricField<Type, fvPatchField, volMesh>& fld
) const
{
  // Construct flat fields for all patch faces to be sampled
  Field<Type> sampledValues(getPatchDataMapPtr_().constructSize());
  FOR_ALL(cellToWalls_, cellI)
  {
    const labelList& cData = cellToWalls_[cellI];
    FOR_ALL(cData, i)
    {
      const point& samplePt = cellToSamples_[cellI][i];
      sampledValues[cData[i]] = interpolator.interpolate(samplePt, cellI);
    }
  }
  // Send back sampled values to patch faces
  getPatchDataMapPtr_().reverseDistribute
  (
    getPatchDataMapPtr_().constructSize(),
    sampledValues
  );
  // Pick up data
  label nPatchFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchSet_, iter)
  {
    label patchI = iter.key();
    fvPatchField<Type>& pfld = fld.boundaryField()[patchI];
    Field<Type> newFld(pfld.size());
    FOR_ALL(pfld, i)
    {
      newFld[i] = sampledValues[nPatchFaces++];
    }
    pfld == newFld;
  }
}
template<class Type>
void mousse::nearWallFields::sampleFields
(
  PtrList<GeometricField<Type, fvPatchField, volMesh> >& sflds
) const
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  FOR_ALL(sflds, i)
  {
    const word& fldName = reverseFieldMap_[sflds[i].name()];
    const vfType& fld = obr_.lookupObject<vfType>(fldName);
    // Take over internal and boundary values
    sflds[i] == fld;
    // Construct interpolation method
    interpolationCellPoint<Type> interpolator(fld);
    // Override sampled values
    sampleBoundaryField(interpolator, sflds[i]);
  }
}
