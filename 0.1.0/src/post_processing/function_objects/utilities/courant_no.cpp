// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "courant_no.hpp"
#include "surface_fields.hpp"
#include "fvc_surface_integrate.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(CourantNo, 0);

}


// Private Member Functions 
mousse::tmp<mousse::volScalarField::DimensionedInternalField>
mousse::CourantNo::byRho
(
  const tmp<volScalarField::DimensionedInternalField>& Co
) const
{
  if (Co().dimensions() == dimDensity) {
    return Co/obr_.lookupObject<volScalarField>(rhoName_);
  } else {
    return Co;
  }
}


// Constructors 
mousse::CourantNo::CourantNo
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  active_{true},
  phiName_{"phi"},
  rhoName_{"rho"}
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_)) {
    active_ = false;
    WARNING_IN
    (
      "CourantNo::CourantNo"
      "("
      "  const word&, "
      "  const objectRegistry&, "
      "  const dictionary&, "
      "  const bool"
      ")"
    )
    << "No fvMesh available, deactivating " << name_ << nl
    << endl;
  }
  read(dict);
  if (active_) {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volScalarField* CourantNoPtr
    {
      new volScalarField
      {
        {
          type(),
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh,
        {"0", dimless, 0.0},
        zeroGradientFvPatchScalarField::typeName
      }
    };
    mesh.objectRegistry::store(CourantNoPtr);
  }
}


// Destructor 
mousse::CourantNo::~CourantNo()
{}


// Member Functions 
void mousse::CourantNo::read(const dictionary& dict)
{
  if (active_) {
    phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
    rhoName_ = dict.lookupOrDefault<word>("rhoName", "rho");
  }
}


void mousse::CourantNo::execute()
{
  if (!active_)
    return;
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  const surfaceScalarField& phi =
    mesh.lookupObject<surfaceScalarField>(phiName_);
  volScalarField& Co =
    const_cast<volScalarField&>
    (
      mesh.lookupObject<volScalarField>(type())
    );
  Co.dimensionedInternalField() =
    byRho
    (
      (0.5*mesh.time().deltaT())
      *fvc::surfaceSum(mag(phi))().dimensionedInternalField()/mesh.V()
    );
  Co.correctBoundaryConditions();
}


void mousse::CourantNo::end()
{
  if (active_) {
    execute();
  }
}


void mousse::CourantNo::timeSet()
{}


void mousse::CourantNo::write()
{
  if (!active_)
    return;
  const volScalarField& CourantNo =
    obr_.lookupObject<volScalarField>(type());
  Info << type() << " " << name_ << " output:" << nl
    << "    writing field " << CourantNo.name() << nl
    << endl;
  CourantNo.write();
}

