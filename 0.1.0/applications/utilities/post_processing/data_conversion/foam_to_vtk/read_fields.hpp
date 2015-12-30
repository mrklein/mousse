// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   read_fields.cpp
#ifndef read_fields_hpp_
#define read_fields_hpp_
#include "fv_mesh.hpp"
#include "ptr_list.hpp"
#include "ioobject_list.hpp"
#include "hash_set.hpp"
namespace mousse
{
// Read the fields and optionally subset and put on the pointer list
template<class GeoField>
void readFields
(
  const vtkMesh& vMesh,
  const typename GeoField::Mesh& mesh,
  const IOobjectList& objects,
  const HashSet<word>& selectedFields,
  PtrList<GeoField>& fields
);
}  // namespace mousse
#ifdef NoRepository
  #include "read_fields.cpp"
#endif
#endif
