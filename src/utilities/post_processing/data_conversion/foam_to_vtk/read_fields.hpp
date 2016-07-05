#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_READ_FIELDS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_READ_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "ptr_list.hpp"
#include "ioobject_list.hpp"
#include "hash_set.hpp"


namespace mousse {

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

#include "read_fields.ipp"

#endif
