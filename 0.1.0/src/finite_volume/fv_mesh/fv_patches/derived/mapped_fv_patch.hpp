// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedFvPatch
// Description
//   mousse::mappedFvPatch
// SourceFiles
//   mapped_fv_patch.cpp

#ifndef mapped_fv_patch_hpp_
#define mapped_fv_patch_hpp_

#include "fv_patch.hpp"
#include "mapped_poly_patch.hpp"

namespace mousse
{
class mappedFvPatch
:
  public fvPatch
{
public:
  //- Runtime type information
  TYPE_NAME(mappedPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    mappedFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm)
    {}
};
}  // namespace mousse

#endif
