#ifndef CORE_MESHES_POLY_MESH_POLY_PATCHES_WALL_POLY_PATCH_HPP_
#define CORE_MESHES_POLY_MESH_POLY_PATCHES_WALL_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallPolyPatch
// Description
//   mousse::wallPolyPatch
// SourceFiles
//   wall_poly_patch.cpp
#include "poly_patch.hpp"
namespace mousse
{
class wallPolyPatch
:
  public polyPatch
{
public:
  //- Runtime type information
  TYPE_NAME("wall");
  // Constructors
    //- Construct from components
    wallPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from dictionary
    wallPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    wallPolyPatch(const wallPolyPatch&, const polyBoundaryMesh&);
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    wallPolyPatch
    (
      const wallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    wallPolyPatch
    (
      const wallPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new wallPolyPatch(*this, bm));
    }
    //- Construct and return a clone, resetting the face list
    //  and boundary mesh
    virtual autoPtr<polyPatch> clone
    (
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    ) const
    {
      return autoPtr<polyPatch>
      (
        new wallPolyPatch(*this, bm, index, newSize, newStart)
      );
    }
    //- Construct and return a clone, resetting the face list
    //  and boundary mesh
    virtual autoPtr<polyPatch> clone
    (
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    ) const
    {
      return autoPtr<polyPatch>
      (
        new wallPolyPatch(*this, bm, index, mapAddressing, newStart)
      );
    }
};
}  // namespace mousse
#endif
