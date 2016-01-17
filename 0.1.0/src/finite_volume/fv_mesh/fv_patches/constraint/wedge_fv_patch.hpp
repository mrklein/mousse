// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wedgeFvPatch
// Description
//   Wedge front and back plane patch.
// SourceFiles
//   wedge_fv_patch.cpp
#ifndef wedge_fv_patch_hpp_
#define wedge_fv_patch_hpp_
#include "fv_patch.hpp"
#include "wedge_poly_patch.hpp"
namespace mousse
{
class wedgeFvPatch
:
  public fvPatch
{
  // Private data
    const wedgePolyPatch& wedgePolyPatch_;
public:
  //- Runtime type information
  TYPE_NAME(wedgePolyPatch::typeName_());
  // Constructors
    //- Construct from polyPatch
    wedgeFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm);
  // Member functions
    // Access
      //- Return face transformation tensor
      const tensor& faceT() const
      {
        return wedgePolyPatch_.faceT();
      }
      //- Return neighbour-cell transformation tensor
      const tensor& cellT() const
      {
        return wedgePolyPatch_.cellT();
      }
};
}  // namespace mousse
#endif
