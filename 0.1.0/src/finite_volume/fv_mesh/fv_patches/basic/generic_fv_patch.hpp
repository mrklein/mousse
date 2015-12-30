// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::genericFvPatch
// Description
//   FV variant of the genericPolyPatch.
// SourceFiles
//   generic_fv_patch.cpp
#ifndef generic_fv_patch_hpp_
#define generic_fv_patch_hpp_
#include "fv_patch.hpp"
#include "generic_poly_patch.hpp"
namespace mousse
{
class genericFvPatch
:
  public fvPatch
{
public:
  //- Runtime type information
  TypeName(genericPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    genericFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
    :
      fvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif
