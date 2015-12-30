// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "srf_model.hpp"
#include "srf_velocity_fv_patch_vector_field.hpp"
// Static Data Members
namespace mousse
{
  namespace SRF
  {
    defineTypeNameAndDebug(SRFModel, 0);
    defineRunTimeSelectionTable(SRFModel, dictionary);
  }
}
// Constructors
mousse::SRF::SRFModel::SRFModel
(
  const word& type,
  const volVectorField& Urel
)
:
  IOdictionary
  (
    IOobject
    (
      "SRFProperties",
      Urel.time().constant(),
      Urel.db(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE
    )
  ),
  Urel_(Urel),
  mesh_(Urel_.mesh()),
  origin_("origin", dimLength, lookup("origin")),
  axis_(lookup("axis")),
  SRFModelCoeffs_(subDict(type + "Coeffs")),
  omega_(dimensionedVector("omega", dimless/dimTime, vector::zero))
{
  // Normalise the axis
  axis_ /= mag(axis_);
}
// Destructor
mousse::SRF::SRFModel::~SRFModel()
{}
// Member Functions
bool mousse::SRF::SRFModel::read()
{
  if (regIOobject::read())
  {
    // Re-read origin
    lookup("origin") >> origin_;
    // Re-read axis
    lookup("axis") >> axis_;
    axis_ /= mag(axis_);
    // Re-read sub-model coeffs
    SRFModelCoeffs_ = subDict(type() + "Coeffs");
    return true;
  }
  else
  {
    return false;
  }
}
const mousse::dimensionedVector& mousse::SRF::SRFModel::origin() const
{
  return origin_;
}
const mousse::vector& mousse::SRF::SRFModel::axis() const
{
  return axis_;
}
const mousse::dimensionedVector& mousse::SRF::SRFModel::omega() const
{
  return omega_;
}
mousse::tmp<mousse::DimensionedField<mousse::vector, mousse::volMesh> >
mousse::SRF::SRFModel::Fcoriolis() const
{
  return tmp<DimensionedField<vector, volMesh> >
  (
    new DimensionedField<vector, volMesh>
    (
      IOobject
      (
        "Fcoriolis",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      2.0*omega_ ^ Urel_
    )
  );
}
mousse::tmp<mousse::DimensionedField<mousse::vector, mousse::volMesh> >
mousse::SRF::SRFModel::Fcentrifugal() const
{
  return tmp<DimensionedField<vector, volMesh> >
  (
    new DimensionedField<vector, volMesh>
    (
      IOobject
      (
        "Fcentrifugal",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      omega_ ^ (omega_ ^ (mesh_.C() - origin_))
    )
  );
}
mousse::tmp<mousse::DimensionedField<mousse::vector, mousse::volMesh> >
mousse::SRF::SRFModel::Su() const
{
  return Fcoriolis() + Fcentrifugal();
}
mousse::vectorField mousse::SRF::SRFModel::velocity
(
  const vectorField& positions
) const
{
  tmp<vectorField> tfld =
    omega_.value()
   ^ (
      (positions - origin_.value())
     - axis_*(axis_ & (positions - origin_.value()))
    );
  return tfld();
}
mousse::tmp<mousse::volVectorField> mousse::SRF::SRFModel::U() const
{
  return tmp<volVectorField>
  (
    new volVectorField
    (
      IOobject
      (
        "Usrf",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      omega_
     ^ ((mesh_.C() - origin_) - axis_*(axis_ & (mesh_.C() - origin_)))
    )
  );
}
mousse::tmp<mousse::volVectorField> mousse::SRF::SRFModel::Uabs() const
{
  tmp<volVectorField> Usrf = U();
  tmp<volVectorField> tUabs
  (
    new volVectorField
    (
      IOobject
      (
        "Uabs",
        mesh_.time().timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      Usrf
    )
  );
  // Add SRF contribution to internal field
  tUabs().internalField() += Urel_.internalField();
  // Add Urel boundary contributions
  const volVectorField::GeometricBoundaryField& bvf = Urel_.boundaryField();
  forAll(bvf, i)
  {
    if (isA<SRFVelocityFvPatchVectorField>(bvf[i]))
    {
      // Only include relative contributions from
      // SRFVelocityFvPatchVectorField's
      const SRFVelocityFvPatchVectorField& UrelPatch =
        refCast<const SRFVelocityFvPatchVectorField>(bvf[i]);
      if (UrelPatch.relative())
      {
        tUabs().boundaryField()[i] += Urel_.boundaryField()[i];
      }
    }
    else
    {
      tUabs().boundaryField()[i] += Urel_.boundaryField()[i];
    }
  }
  return tUabs;
}
