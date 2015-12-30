// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "atm_boundary_layer_inlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
// Constructors 
atmBoundaryLayerInletVelocityFvPatchVectorField::
atmBoundaryLayerInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(p, iF),
  atmBoundaryLayer()
{}
atmBoundaryLayerInletVelocityFvPatchVectorField::
atmBoundaryLayerInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField(p, iF),
  atmBoundaryLayer(patch().Cf(), dict)
{
  vectorField::operator=(U(patch().Cf()));
}
atmBoundaryLayerInletVelocityFvPatchVectorField::
atmBoundaryLayerInletVelocityFvPatchVectorField
(
  const atmBoundaryLayerInletVelocityFvPatchVectorField& pvf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField(pvf, p, iF, mapper),
  atmBoundaryLayer(pvf, mapper)
{}
atmBoundaryLayerInletVelocityFvPatchVectorField::
atmBoundaryLayerInletVelocityFvPatchVectorField
(
  const atmBoundaryLayerInletVelocityFvPatchVectorField& pvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(pvf, iF),
  atmBoundaryLayer(pvf)
{}
// Member Functions 
void atmBoundaryLayerInletVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchVectorField::autoMap(m);
  atmBoundaryLayer::autoMap(m);
}
void atmBoundaryLayerInletVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& pvf,
  const labelList& addr
)
{
  fixedValueFvPatchVectorField::rmap(pvf, addr);
  const atmBoundaryLayerInletVelocityFvPatchVectorField& blpvf =
    refCast<const atmBoundaryLayerInletVelocityFvPatchVectorField>(pvf);
  atmBoundaryLayer::rmap(blpvf, addr);
}
void atmBoundaryLayerInletVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  atmBoundaryLayer::write(os);
  writeEntry("value", os);
}
makePatchTypeField
(
  fvPatchVectorField,
  atmBoundaryLayerInletVelocityFvPatchVectorField
);
}  // namespace mousse
