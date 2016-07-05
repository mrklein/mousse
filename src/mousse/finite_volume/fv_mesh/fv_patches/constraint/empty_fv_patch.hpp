#ifndef FINITE_VOLUME_FV_MESH_FV_PATCHES_CONSTRAINT_EMPTY_FV_PATCH_HPP_
#define FINITE_VOLUME_FV_MESH_FV_PATCHES_CONSTRAINT_EMPTY_FV_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::emptyFvPatch
// Description
//   A patch which will not exist in the fvMesh. Typical example is a front and
//   back plane of a 2-D geometry
// SourceFiles
//   empty_fv_patch.cpp
#include "fv_patch.hpp"
#include "empty_poly_patch.hpp"
namespace mousse
{
class emptyFvPatch
:
  public fvPatch
{
  // Private data
    //- face-cell addressing
    const labelList::subList faceCells_;
public:
  //- Runtime type information
  TYPE_NAME(emptyPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    emptyFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm);
  // Member Functions
    // Access
      virtual label size() const
      {
        return 0;
      }
      //- Return faceCells
      virtual const labelUList& faceCells() const;
};
}  // namespace mousse
#endif
