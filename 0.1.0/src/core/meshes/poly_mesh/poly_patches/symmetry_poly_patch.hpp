#ifndef CORE_MESHES_POLY_MESH_POLY_PATCHES_SYMMETRY_POLY_PATCH_HPP_
#define CORE_MESHES_POLY_MESH_POLY_PATCHES_SYMMETRY_POLY_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryPolyPatch
// Description
//   Symmetry patch for non-planar or multi-plane patches.
// SourceFiles
//   symmetry_poly_patch.cpp
//   see_also
//   symmetry_plane_poly_patch


#include "poly_patch.hpp"

namespace mousse
{
class symmetryPolyPatch
:
  public polyPatch
{
public:
  //- Runtime type information
  TYPE_NAME("symmetry");
  // Constructors
    //- Construct from components
    symmetryPolyPatch
    (
      const word& name,
      const label size,
      const label start,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct from dictionary
    symmetryPolyPatch
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyBoundaryMesh& bm,
      const word& patchType
    );
    //- Construct as copy, resetting the boundary mesh
    symmetryPolyPatch(const symmetryPolyPatch&, const polyBoundaryMesh&);
    //- Construct given the original patch and resetting the
    //  face list and boundary mesh information
    symmetryPolyPatch
    (
      const symmetryPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const label newSize,
      const label newStart
    );
    //- Construct given the original patch and a map
    symmetryPolyPatch
    (
      const symmetryPolyPatch& pp,
      const polyBoundaryMesh& bm,
      const label index,
      const labelUList& mapAddressing,
      const label newStart
    );
    //- Construct and return a clone, resetting the boundary mesh
    virtual autoPtr<polyPatch> clone(const polyBoundaryMesh& bm) const
    {
      return autoPtr<polyPatch>(new symmetryPolyPatch(*this, bm));
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
        new symmetryPolyPatch(*this, bm, index, newSize, newStart)
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
        new symmetryPolyPatch(*this, bm, index, mapAddressing, newStart)
      );
    }
};
}  // namespace mousse
#endif
