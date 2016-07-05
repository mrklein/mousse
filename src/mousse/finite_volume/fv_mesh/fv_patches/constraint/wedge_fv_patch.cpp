// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wedge_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(wedgeFvPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, wedgeFvPatch, polyPatch);
// Constructors 
wedgeFvPatch::wedgeFvPatch(const polyPatch& patch, const fvBoundaryMesh& bm)
:
  fvPatch(patch, bm),
  wedgePolyPatch_(refCast<const wedgePolyPatch>(patch))
{}
}  // namespace mousse
