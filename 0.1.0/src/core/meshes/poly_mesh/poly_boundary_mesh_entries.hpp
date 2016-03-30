#ifndef CORE_MESHES_POLY_MESH_POLY_BOUNDARY_MESH_ENTRIES_HPP_
#define CORE_MESHES_POLY_MESH_POLY_BOUNDARY_MESH_ENTRIES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyBoundaryMeshEntries
// Description
//   mousse::polyBoundaryMeshEntries

#include "reg_ioobject.hpp"
#include "ptr_list.hpp"
#include "entry.hpp"


namespace mousse {

class polyBoundaryMeshEntries
:
  public regIOobject,
  public PtrList<entry>
{
public:
  //- Runtime type information
  TYPE_NAME("polyBoundaryMesh");
  // Constructors
    explicit polyBoundaryMeshEntries(const IOobject& io)
    :
      regIOobject{io},
      PtrList<entry>{readStream(typeName)}
    {
      close();
    }
 // Member functions
    bool writeData(Ostream&) const
    {
      NOT_IMPLEMENTED("writeData(Ostream&) const");
      return false;
    }
};

}  // namespace mousse

#endif

