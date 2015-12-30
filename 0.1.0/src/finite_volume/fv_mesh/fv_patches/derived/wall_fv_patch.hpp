// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallFvPatch
// Description
//   mousse::wallFvPatch
// SourceFiles
//   wall_fv_patch.cpp
#ifndef wall_fv_patch_hpp_
#define wall_fv_patch_hpp_
#include "fv_patch.hpp"
#include "wall_poly_patch.hpp"
namespace mousse
{
class wallFvPatch
:
  public fvPatch
{
public:
  //- Runtime type information
  TypeName(wallPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    wallFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif
