// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lambda2.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "fvc_grad.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(Lambda2, 0);
}
// Constructors 
mousse::Lambda2::Lambda2
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
  UName_{"U"}
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WARNING_IN
    (
      "Lambda2::Lambda2"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )
    << "No fvMesh available, deactivating " << name_ << nl
    << endl;
  }
  read(dict);
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volScalarField* Lambda2Ptr
    {
      new volScalarField
      {
        // IOobject
        {
          type(),
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh,
        {"0", dimless/sqr(dimTime), 0.0}
      }
    };
    mesh.objectRegistry::store(Lambda2Ptr);
  }
}
// Destructor 
mousse::Lambda2::~Lambda2()
{}
// Member Functions 
void mousse::Lambda2::read(const dictionary& dict)
{
  if (active_)
  {
    UName_ = dict.lookupOrDefault<word>("UName", "U");
  }
}
void mousse::Lambda2::execute()
{
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    const volVectorField& U =
      mesh.lookupObject<volVectorField>(UName_);
    const volTensorField gradU{fvc::grad(U)};
    const volTensorField SSplusWW
    {
      (symm(gradU) & symm(gradU)) + (skew(gradU) & skew(gradU))
    };
    volScalarField& Lambda2 =
      const_cast<volScalarField&>
      (
        mesh.lookupObject<volScalarField>(type())
      );
    Lambda2 = -eigenValues(SSplusWW)().component(vector::Y);
  }
}
void mousse::Lambda2::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::Lambda2::timeSet()
{
  // Do nothing
}
void mousse::Lambda2::write()
{
  if (active_)
  {
    const volScalarField& Lambda2 =
      obr_.lookupObject<volScalarField>(type());
    Info<< type() << " " << name_ << " output:" << nl
      << "    writing field " << Lambda2.name() << nl
      << endl;
    Lambda2.write();
  }
}
