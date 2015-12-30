// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   ensight_field.cpp
#ifndef ensight_field_hpp_
#define ensight_field_hpp_
#include "ensight_mesh.hpp"
#include "fv_mesh_subset.hpp"
//- Wrapper to get hold of the field or the subsetted field
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
volField
(
  const mousse::fvMeshSubset&,
  const mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh>& vf
);
template<class Type>
void ensightField
(
  const mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh>& vf,
  const mousse::ensightMesh& eMesh,
  const mousse::fileName& postProcPath,
  const mousse::word& prepend,
  const mousse::label timeIndex,
  const bool binary,
  const bool nodeValues,
  mousse::Ostream& ensightCaseFile
);
template<class Type>
void writePatchField
(
  const mousse::word& fieldName,
  const mousse::Field<Type>& pf,
  const mousse::word& patchName,
  const mousse::ensightMesh& eMesh,
  const mousse::fileName& postProcPath,
  const mousse::word& prepend,
  const mousse::label timeIndex,
  mousse::Ostream& ensightCaseFile
);
#ifdef NoRepository
  #include "ensight_field.cpp"
#endif
#endif
