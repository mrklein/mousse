// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_body_motion_displacement_point_patch_vector_field.hpp"
#include "transform_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "point_patch_fields.hpp"
namespace mousse
{
// Constructors
solidBodyMotionDisplacementPointPatchVectorField::
solidBodyMotionDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchVectorField{p, iF},
  SBMFPtr_{}
{}
solidBodyMotionDisplacementPointPatchVectorField::
solidBodyMotionDisplacementPointPatchVectorField
(
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const dictionary& dict
)
:
  fixedValuePointPatchVectorField{p, iF, dict, false},
  SBMFPtr_{solidBodyMotionFunction::New(dict, this->db().time())}
{
  if (!dict.found("value"))
  {
    // Determine current local points and offset
    fixedValuePointPatchVectorField::operator==
    (
      transform(SBMFPtr_().transformation(), localPoints0())
     -localPoints0()
    );
  }
}
solidBodyMotionDisplacementPointPatchVectorField::
solidBodyMotionDisplacementPointPatchVectorField
(
  const solidBodyMotionDisplacementPointPatchVectorField& ptf,
  const pointPatch& p,
  const DimensionedField<vector, pointMesh>& iF,
  const pointPatchFieldMapper& mapper
)
:
  fixedValuePointPatchVectorField{ptf, p, iF, mapper},
  SBMFPtr_{ptf.SBMFPtr_().clone().ptr()}
{
  // For safety re-evaluate
  fixedValuePointPatchVectorField::operator==
  (
    transform(SBMFPtr_().transformation(), localPoints0())
   -localPoints0()
  );
}
solidBodyMotionDisplacementPointPatchVectorField::
solidBodyMotionDisplacementPointPatchVectorField
(
  const solidBodyMotionDisplacementPointPatchVectorField& ptf
)
:
  fixedValuePointPatchVectorField{ptf},
  SBMFPtr_{ptf.SBMFPtr_().clone().ptr()}
{}
solidBodyMotionDisplacementPointPatchVectorField::
solidBodyMotionDisplacementPointPatchVectorField
(
  const solidBodyMotionDisplacementPointPatchVectorField& ptf,
  const DimensionedField<vector, pointMesh>& iF
)
:
  fixedValuePointPatchVectorField{ptf, iF},
  SBMFPtr_{ptf.SBMFPtr_().clone().ptr()}
{
  // For safety re-evaluate
  fixedValuePointPatchVectorField::operator==
  (
    transform(SBMFPtr_().transformation(), localPoints0())
   -localPoints0()
  );
}
// Member Functions 
const pointField&
solidBodyMotionDisplacementPointPatchVectorField::localPoints0() const
{
  if (!localPoints0Ptr_.valid())
  {
    pointIOField points0
    {
      // IOobject
      {
        "points",
        this->db().time().constant(),
        polyMesh::meshSubDir,
        this->db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE,
        false
      }
    };
    localPoints0Ptr_.reset(new pointField(points0, patch().meshPoints()));
  }
  return localPoints0Ptr_();
}
void solidBodyMotionDisplacementPointPatchVectorField::updateCoeffs()
{
  if (this->updated())
  {
    return;
  }
  // Determine current local points and offset
  fixedValuePointPatchVectorField::operator==
  (
    transform(SBMFPtr_().transformation(), localPoints0())
   -localPoints0()
  );
  fixedValuePointPatchVectorField::updateCoeffs();
}
void solidBodyMotionDisplacementPointPatchVectorField::
write(Ostream& os) const
{
  // Note: write value
  fixedValuePointPatchVectorField::write(os);
  os.writeKeyword(solidBodyMotionFunction::typeName) << SBMFPtr_->type()
    << token::END_STATEMENT << nl;
  os  << indent << word(SBMFPtr_->type() + "Coeffs");
  SBMFPtr_->writeData(os);
}
MAKE_POINT_PATCH_TYPE_FIELD
(
  pointPatchVectorField,
  solidBodyMotionDisplacementPointPatchVectorField
);
}  // namespace mousse
