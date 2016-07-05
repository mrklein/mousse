#ifndef REGION_MODELS_REGION_MODEL_DERIVED_FV_PATCHES_MAPPED_VARIABLE_THICKNESS_WALL_FV_PATCH_HPP_
#define REGION_MODELS_REGION_MODEL_DERIVED_FV_PATCHES_MAPPED_VARIABLE_THICKNESS_WALL_FV_PATCH_HPP_

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

#include "wall_fv_patch.hpp"
#include "mapped_variable_thickness_wall_poly_patch.hpp"


namespace mousse {

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
  TYPE_NAME(mappedVariableThicknessWallPolyPatch::typeName_());
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

