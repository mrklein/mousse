// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   reconstruct_lagrangian_positions.cpp
//   reconstruct_lagrangian_fields.cpp
#ifndef reconstruct_lagrangian_hpp_
#define reconstruct_lagrangian_hpp_
#include "cloud.hpp"
#include "poly_mesh.hpp"
#include "ioobject_list.hpp"
#include "compact_io_field.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
void reconstructLagrangianPositions
(
  const polyMesh& mesh,
  const word& cloudName,
  const PtrList<fvMesh>& meshes,
  const PtrList<labelIOList>& faceProcAddressing,
  const PtrList<labelIOList>& cellProcAddressing
);
template<class Type>
tmp<IOField<Type> > reconstructLagrangianField
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const word& fieldName
);
template<class Type>
tmp<CompactIOField<Field<Type>, Type> > reconstructLagrangianFieldField
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const word& fieldName
);
template<class Type>
void reconstructLagrangianFields
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
);
template<class Type>
void reconstructLagrangianFieldFields
(
  const word& cloudName,
  const polyMesh& mesh,
  const PtrList<fvMesh>& meshes,
  const IOobjectList& objects,
  const HashSet<word>& selectedFields
);
}  // namespace mousse
#ifdef NoRepository
#   include "reconstruct_lagrangian_fields.cpp"
#endif
#endif
