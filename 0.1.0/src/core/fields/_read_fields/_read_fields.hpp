// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   _read_fields.cpp
#ifndef _read_fields_hpp_
#define _read_fields_hpp_
#include "ptr_list.hpp"
#include "word_list.hpp"
namespace mousse
{
class IOobjectList;
//- Helper routine to read fields
template<class GeoField, class Mesh>
wordList ReadFields
(
  const Mesh& mesh,
  const IOobjectList& objects,
  PtrList<GeoField>& fields,
  const bool syncPar = true
);
//- Helper routine to read GeometricFields. The fieldsCache is per time
//  an objectRegistry of all stored fields
template<class GeoField>
static void ReadFields
(
  const word& fieldName,
  const typename GeoField::Mesh& mesh,
  const wordList& timeNames,
  objectRegistry& fieldsCache
);
//- Helper routine to read GeometricFields. The fieldsCache is per time
//  an objectRegistry of all stored fields
template<class GeoField>
static void ReadFields
(
  const word& fieldName,
  const typename GeoField::Mesh& mesh,
  const wordList& timeNames,
  const word& registryName = "fieldsCache"
);
}  // namespace mousse
#ifdef NoRepository
#   include "_read_fields.cpp"
#endif
#endif
