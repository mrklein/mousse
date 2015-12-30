// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uncoupled_six_dof_rigid_body_displacement_point_patch_vector_field.hpp"
#include "point_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "uniform_dimensioned_fields.hpp"
namespace mousse
{
// Constructors 
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>(p, iF),
  motion_(),
  initialPoints_(p.localPoints()),
  curTimeIndex_(-1)
{}
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchField<vector>(p, iF, dict),
  motion_(dict, dict),
  curTimeIndex_(-1)
{
  if (!dict.found("value"))
  {
    updateCoeffs();
  }
  if (dict.found("initialPoints"))
  {
    initialPoints_ = vectorField("initialPoints", dict , p.size());
  }
  else
  {
    initialPoints_ = p.localPoints();
  }
}
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
(
  const uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchField<vector>(ptf, p, iF, mapper),
  motion_(ptf.motion_),
  initialPoints_(ptf.initialPoints_, mapper),
  curTimeIndex_(-1)
{}
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::
uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
(
  const uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchField<vector>(ptf, iF),
  motion_(ptf.motion_),
  initialPoints_(ptf.initialPoints_),
  curTimeIndex_(-1)
{}
// Member Functions 
void uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::autoMap
(
  const pointPatchFieldMapper& m
)
{
  fixedValuePointPatchField<vector>::autoMap(m);
  initialPoints_.autoMap(m);
}
void uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::rmap
(
  const pointPatchField<vector>& ptf,
  const labelList& addr
)
{
  const uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField& uSDoFptf =
  refCast
  <
    const uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
  >(ptf);
  fixedValuePointPatchField<vector>::rmap(uSDoFptf, addr);
  initialPoints_.rmap(uSDoFptf.initialPoints_, addr);
}
void uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  const polyMesh& mesh = this->dimensionedInternalField().mesh()();
  const Time& t = mesh.time();
  // Store the motion state at the beginning of the time-step
  bool firstIter = false;
  if (curTimeIndex_ != t.timeIndex())
  {
    motion_.newTime();
    curTimeIndex_ = t.timeIndex();
    firstIter = true;
  }
  vector gravity = vector::zero;
  if (db().foundObject<uniformDimensionedVectorField>("g"))
  {
    uniformDimensionedVectorField g =
    db().lookupObject<uniformDimensionedVectorField>("g");
    gravity = g.value();
  }
  // Do not modify the accelerations, except with gravity, where available
  motion_.update
  (
    firstIter,
    gravity*motion_.mass(),
    vector::zero,
    t.deltaTValue(),
    t.deltaT0Value()
  );
  Field<vector>::operator=
  (
    motion_.transform(initialPoints_) - initialPoints_
  );
  fixedValuePointPatchField<vector>::updateCoeffs();
}
void uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField::write
(
  Ostream& os
) const
{
  pointPatchField<vector>::write(os);
  motion_.write(os);
  initialPoints_.writeEntry("initialPoints", os);
  writeEntry("value", os);
}
makePointPatchTypeField
(
  pointPatchVectorField,
  uncoupledSixDoFRigidBodyDisplacementPointPatchVectorField
);
}  // namespace mousse
