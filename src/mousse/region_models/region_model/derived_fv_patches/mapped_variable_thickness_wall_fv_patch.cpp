// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_variable_thickness_wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "region_model_1d.hpp"


namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(mappedVariableThicknessWallFvPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  fvPatch,
  mappedVariableThicknessWallFvPatch,
  polyPatch
);

}


// Member Functions 
void mousse::mappedVariableThicknessWallFvPatch::
makeDeltaCoeffs(scalarField& dc) const
{
  const mappedVariableThicknessWallPolyPatch& pp =
    refCast<const mappedVariableThicknessWallPolyPatch>(patch());
  typedef regionModels::regionModel1D modelType;
  const modelType& region1D =
    patch().boundaryMesh().mesh().time().lookupObject<modelType>
    (
      "thermalBaffleProperties"
    );
  dc = 2.0/(pp.thickness()/region1D.nLayers());
}
