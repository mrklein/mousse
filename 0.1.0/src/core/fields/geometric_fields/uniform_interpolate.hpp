// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_field.hpp"
#include "hash_ptr_table.hpp"
namespace mousse
{
// Global functions 
//- Interpolate selected fields (given by indices and corresponding weights)
//  (boundary type becomes calculated). Fields stored per index. Field gets name
//  "uniformInterpolate(" + fld.name() + ')'.
template<class GeoField>
tmp<GeoField> uniformInterpolate
(
  const HashPtrTable<GeoField, label, Hash<label> >& fields,
  const labelList& indices,
  const scalarField& weights
);
//- Interpolate fields. fieldsCache contains per timeName all loaded fields.
//  Resulting field gets properties according to fieldIO
template<class GeoField>
tmp<GeoField> uniformInterpolate
(
  const IOobject& fieldIO,
  const word& fieldName,
  const wordList& times,
  const scalarField& weights,
  const objectRegistry& fieldsCache
);
//- Interpolate fields. fieldsCache contains per timeName all loaded fields.
//  Resulting field gets properties according to fieldIO
template<class GeoField>
tmp<GeoField> uniformInterpolate
(
  const IOobject& fieldIO,
  const word& fieldName,
  const wordList& times,
  const scalarField& weights,
  const word& registryName = "fieldsCache"
);
}  // namespace mousse
#ifdef NoRepository
#   include "uniform_interpolate.cpp"
#endif
