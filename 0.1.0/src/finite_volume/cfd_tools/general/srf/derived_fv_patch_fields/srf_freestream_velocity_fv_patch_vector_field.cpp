// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "srf_freestream_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "srf_model.hpp"
#include "steady_state_ddt_scheme.hpp"
// Constructors
mousse::SRFFreestreamVelocityFvPatchVectorField::
SRFFreestreamVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  inletOutletFvPatchVectorField{p, iF},
  relative_{false},
  UInf_{vector::zero}
{}
mousse::SRFFreestreamVelocityFvPatchVectorField::
SRFFreestreamVelocityFvPatchVectorField
(
  const SRFFreestreamVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  inletOutletFvPatchVectorField{ptf, p, iF, mapper},
  relative_{ptf.relative_},
  UInf_{ptf.UInf_}
{}
mousse::SRFFreestreamVelocityFvPatchVectorField::
SRFFreestreamVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  inletOutletFvPatchVectorField{p, iF},
  relative_{dict.lookupOrDefault("relative", false)},
  UInf_{dict.lookup("UInf")}
{
  this->phiName_ = dict.lookupOrDefault<word>("phi","phi");
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}
mousse::SRFFreestreamVelocityFvPatchVectorField::
SRFFreestreamVelocityFvPatchVectorField
(
  const SRFFreestreamVelocityFvPatchVectorField& srfvpvf
)
:
  inletOutletFvPatchVectorField{srfvpvf},
  relative_{srfvpvf.relative_},
  UInf_{srfvpvf.UInf_}
{}
mousse::SRFFreestreamVelocityFvPatchVectorField::
SRFFreestreamVelocityFvPatchVectorField
(
  const SRFFreestreamVelocityFvPatchVectorField& srfvpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  inletOutletFvPatchVectorField{srfvpvf, iF},
  relative_{srfvpvf.relative_},
  UInf_{srfvpvf.UInf_}
{}
// Member Functions
void mousse::SRFFreestreamVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // Get reference to the SRF model
  const SRF::SRFModel& srf =
    db().lookupObject<SRF::SRFModel>("SRFProperties");
  word ddtScheme
  (
    this->dimensionedInternalField().mesh()
   .ddtScheme(this->dimensionedInternalField().name())
  );
  if (ddtScheme == fv::steadyStateDdtScheme<scalar>::typeName)
  {
    // If not relative to the SRF include the effect of the SRF
    if (!relative_)
    {
      refValue() = UInf_ - srf.velocity(patch().Cf());
    }
    // If already relative to the SRF simply supply the inlet value
    // as a fixed value
    else
    {
      refValue() = UInf_;
    }
  }
  else
  {
    scalar time = this->db().time().value();
    scalar theta = time*mag(srf.omega().value());
    refValue() =
      cos(theta)*UInf_ + sin(theta)*(srf.axis() ^ UInf_)
     - srf.velocity(patch().Cf());
  }
  // Set the inlet-outlet choice based on the direction of the freestream
  valueFraction() = 1.0 - pos(refValue() & patch().Sf());
  mixedFvPatchField<vector>::updateCoeffs();
}
void mousse::SRFFreestreamVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  os.writeKeyword("relative") << relative_ << token::END_STATEMENT << nl;
  os.writeKeyword("UInf") << UInf_ << token::END_STATEMENT << nl;
  os.writeKeyword("phi") << this->phiName_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  SRFFreestreamVelocityFvPatchVectorField
);
}
