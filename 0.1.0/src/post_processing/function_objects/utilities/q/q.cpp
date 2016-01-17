// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "q.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "fvc_grad.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(Q, 0);
}
// Constructors 
mousse::Q::Q
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
      "Q::Q"
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
    volScalarField* QPtr
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
    mesh.objectRegistry::store(QPtr);
  }
}
// Destructor 
mousse::Q::~Q()
{}
// Member Functions 
void mousse::Q::read(const dictionary& dict)
{
  if (active_)
  {
    UName_ = dict.lookupOrDefault<word>("UName", "U");
  }
}
void mousse::Q::execute()
{
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    const volVectorField& U =
      mesh.lookupObject<volVectorField>(UName_);
    const volTensorField gradU(fvc::grad(U));
    volScalarField& Q =
      const_cast<volScalarField&>
      (
        mesh.lookupObject<volScalarField>(type())
      );
    Q = 0.5*(sqr(tr(gradU)) - tr(((gradU) & (gradU))));
  }
}
void mousse::Q::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::Q::timeSet()
{
  // Do nothing
}
void mousse::Q::write()
{
  if (active_)
  {
    const volScalarField& Q =
      obr_.lookupObject<volScalarField>(type());
    Info<< type() << " " << name_ << " output:" << nl
      << "    writing field " << Q.name() << nl
      << endl;
    Q.write();
  }
}
