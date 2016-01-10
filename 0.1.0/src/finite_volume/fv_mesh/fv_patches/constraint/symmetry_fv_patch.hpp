// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::symmetryFvPatch
// Description
//   Symmetry patch for non-planar or multi-plane patches.
// SourceFiles
//   symmetry_fv_patch.cpp
#ifndef symmetry_fv_patch_hpp_
#define symmetry_fv_patch_hpp_
#include "fv_patch.hpp"
#include "symmetry_poly_patch.hpp"
namespace mousse
{
class symmetryFvPatch
:
  public fvPatch
{
public:
  //- Runtime type information
  TYPE_NAME(symmetryPolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    symmetryFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif
