// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pyrolysis_model.hpp"
#include "fv_mesh.hpp"
#include "mapped_field_fv_patch_field.hpp"
namespace mousse
{
namespace regionModels
{
namespace pyrolysisModels
{
// Static Data Members
defineTypeNameAndDebug(pyrolysisModel, 0);
defineRunTimeSelectionTable(pyrolysisModel, mesh);
defineRunTimeSelectionTable(pyrolysisModel, dictionary);
// Protected Member Functions 
void pyrolysisModel::readPyrolysisControls()
{
  // do nothing
}
bool pyrolysisModel::read()
{
  if (regionModel1D::read())
  {
    readPyrolysisControls();
    return true;
  }
  else
  {
    return false;
  }
}
bool pyrolysisModel::read(const dictionary& dict)
{
  if (regionModel1D::read(dict))
  {
    readPyrolysisControls();
    return true;
  }
  else
  {
    return false;
  }
}
// Constructors 
pyrolysisModel::pyrolysisModel(const fvMesh& mesh, const word& regionType)
:
  regionModel1D(mesh, regionType)
{}
pyrolysisModel::pyrolysisModel
(
  const word& modelType,
  const fvMesh& mesh,
  const word& regionType
)
:
  regionModel1D(mesh, regionType, modelType)
{
  if (active_)
  {
    read();
  }
}
pyrolysisModel::pyrolysisModel
(
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& regionType
)
:
  regionModel1D(mesh, regionType, modelType, dict)
{
  if (active_)
  {
    read(dict);
  }
}
// Destructor 
pyrolysisModel::~pyrolysisModel()
{}
// Member Functions 
scalar pyrolysisModel::addMassSources
(
  const label patchI,
  const label faceI
)
{
  return 0.0;
}
scalar pyrolysisModel::solidRegionDiffNo() const
{
  return -GREAT;
}
scalar pyrolysisModel::maxDiff() const
{
  return GREAT;
}
}  // namespace pyrolysisModels
}  // namespace regionModels
}  // namespace mousse
