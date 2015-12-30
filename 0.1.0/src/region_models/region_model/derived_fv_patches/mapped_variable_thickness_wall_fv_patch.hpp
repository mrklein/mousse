// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mappedVariableThicknessWallFvPatch
// Group
//   grpRegionBoundaryConditions
// Description
//   Take thickness field and number of layers and returns deltaCoeffs
//   as 2.0/thickness/nLayers.
//   To be used with 1D thermo baffle.
// SourceFiles
//   mapped_variable_thickness_wall_fv_patch.cpp
#ifndef mapped_variable_thickness_wall_fv_patch_hpp_
#define mapped_variable_thickness_wall_fv_patch_hpp_
#include "wall_fv_patch.hpp"
#include "mapped_variable_thickness_wall_poly_patch.hpp"
namespace mousse
{
class mappedVariableThicknessWallFvPatch
:
  public wallFvPatch
{
protected:
  // Protected Member Functions
    //- Read neighbour cell distances from dictionary
    void makeDeltaCoeffs(scalarField& dc) const;
public:
  //- Runtime type information
  TypeName(mappedVariableThicknessWallPolyPatch::typeName_());
  // Constructors
    //- Construct from components
    mappedVariableThicknessWallFvPatch
    (
      const polyPatch& patch,
      const fvBoundaryMesh& bm
    )
    :
      wallFvPatch(patch, bm)
    {}
};
}  // namespace mousse
#endif
