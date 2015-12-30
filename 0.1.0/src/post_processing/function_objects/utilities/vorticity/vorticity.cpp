// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vorticity.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "fvc_curl.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(vorticity, 0);
}
// Constructors 
mousse::vorticity::vorticity
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  name_(name),
  obr_(obr),
  active_(true),
  UName_("U"),
  outputName_(typeName)
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WarningIn
    (
      "vorticity::vorticity"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_ << nl
      << endl;
  }
  read(dict);
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volVectorField* vorticityPtr
    (
      new volVectorField
      (
        IOobject
        (
          outputName_,
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensionedVector("0", dimless/dimTime, vector::zero)
      )
    );
    mesh.objectRegistry::store(vorticityPtr);
  }
}
// Destructor 
mousse::vorticity::~vorticity()
{}
// Member Functions 
void mousse::vorticity::read(const dictionary& dict)
{
  if (active_)
  {
    UName_ = dict.lookupOrDefault<word>("UName", "U");
    if (UName_ != "U")
    {
      outputName_ = typeName + "(" + UName_ + ")";
    }
  }
}
void mousse::vorticity::execute()
{
  if (active_)
  {
    const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);
    volVectorField& vorticity =
      const_cast<volVectorField&>
      (
        obr_.lookupObject<volVectorField>(outputName_)
      );
    vorticity = fvc::curl(U);
  }
}
void mousse::vorticity::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::vorticity::timeSet()
{
  // Do nothing
}
void mousse::vorticity::write()
{
  if (active_)
  {
    const volVectorField& vorticity =
      obr_.lookupObject<volVectorField>(outputName_);
    Info<< type() << " " << name_ << " output:" << nl
      << "    writing field " << vorticity.name() << nl
      << endl;
    vorticity.write();
  }
}
