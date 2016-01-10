// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_patch_internal_field.hpp"
#include "dictionary.hpp"
#include "poly_mesh.hpp"
#include "poly_patch.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(sampledPatchInternalField, 0);
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  sampledSurface,
  sampledPatchInternalField,
  word,
  patchInternalField
);
}
// Constructors 
mousse::sampledPatchInternalField::sampledPatchInternalField
(
  const word& name,
  const polyMesh& mesh,
  const dictionary& dict
)
:
  sampledPatch{name, mesh, dict},
  mappers_{patchIDs().size()}
{
  mappedPatchBase::offsetMode mode = mappedPatchBase::NORMAL;
  if (dict.found("offsetMode"))
  {
    mode = mappedPatchBase::offsetModeNames_.read
    (
      dict.lookup("offsetMode")
    );
  }
  switch (mode)
  {
    case mappedPatchBase::NORMAL:
    {
      const scalar distance = readScalar(dict.lookup("distance"));
      FOR_ALL(patchIDs(), i)
      {
        mappers_.set
        (
          i,
          new mappedPatchBase
          {
            mesh.boundaryMesh()[patchIDs()[i]],
            mesh.name(),                        // sampleRegion
            mappedPatchBase::NEARESTCELL,       // sampleMode
            word::null,                         // samplePatch
            -distance                  // sample inside my domain
          }
        );
      }
    }
    break;
    case mappedPatchBase::UNIFORM:
    {
      const point offset(dict.lookup("offset"));
      FOR_ALL(patchIDs(), i)
      {
        mappers_.set
        (
          i,
          new mappedPatchBase
          {
            mesh.boundaryMesh()[patchIDs()[i]],
            mesh.name(),                        // sampleRegion
            mappedPatchBase::NEARESTCELL,       // sampleMode
            word::null,                         // samplePatch
            offset                  // sample inside my domain
          }
        );
      }
    }
    break;
    case mappedPatchBase::NONUNIFORM:
    {
      const pointField offsets(dict.lookup("offsets"));
      FOR_ALL(patchIDs(), i)
      {
        mappers_.set
        (
          i,
          new mappedPatchBase
          {
            mesh.boundaryMesh()[patchIDs()[i]],
            mesh.name(),                        // sampleRegion
            mappedPatchBase::NEARESTCELL,       // sampleMode
            word::null,                         // samplePatch
            offsets                  // sample inside my domain
          }
        );
      }
    }
    break;
  }
}
// Destructor 
mousse::sampledPatchInternalField::~sampledPatchInternalField()
{}
// Member Functions 
mousse::tmp<mousse::scalarField> mousse::sampledPatchInternalField::sample
(
  const volScalarField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::vectorField> mousse::sampledPatchInternalField::sample
(
  const volVectorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::sphericalTensorField> mousse::sampledPatchInternalField::sample
(
  const volSphericalTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledPatchInternalField::sample
(
  const volSymmTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::tensorField> mousse::sampledPatchInternalField::sample
(
  const volTensorField& vField
) const
{
  return sampleField(vField);
}
mousse::tmp<mousse::scalarField> mousse::sampledPatchInternalField::interpolate
(
  const interpolation<scalar>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::vectorField> mousse::sampledPatchInternalField::interpolate
(
  const interpolation<vector>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::sphericalTensorField>
mousse::sampledPatchInternalField::interpolate
(
  const interpolation<sphericalTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::symmTensorField> mousse::sampledPatchInternalField::interpolate
(
  const interpolation<symmTensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
mousse::tmp<mousse::tensorField> mousse::sampledPatchInternalField::interpolate
(
  const interpolation<tensor>& interpolator
) const
{
  return interpolateField(interpolator);
}
void mousse::sampledPatchInternalField::print(Ostream& os) const
{
  os  << "sampledPatchInternalField: " << name() << " :"
    << "  patches:" << patchNames()
    << "  faces:" << faces().size()
    << "  points:" << points().size();
}
