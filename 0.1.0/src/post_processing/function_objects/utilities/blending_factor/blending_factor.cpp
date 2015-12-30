// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "blending_factor.hpp"
#include "dictionary.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(blendingFactor, 0);
}
// Constructors 
mousse::blendingFactor::blendingFactor
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
  phiName_("unknown-phiName"),
  fieldName_("unknown-fieldName")
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WarningIn
    (
      "blendingFactor::blendingFactor"
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
}
// Destructor 
mousse::blendingFactor::~blendingFactor()
{}
// Member Functions 
void mousse::blendingFactor::read(const dictionary& dict)
{
  if (active_)
  {
    phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
    dict.lookup("fieldName") >> fieldName_;
  }
}
void mousse::blendingFactor::execute()
{
  if (active_)
  {
    calc<scalar>();
    calc<vector>();
  }
}
void mousse::blendingFactor::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::blendingFactor::timeSet()
{
  // Do nothing
}
void mousse::blendingFactor::write()
{
  if (active_)
  {
    const word fieldName = "blendingFactor:" + fieldName_;
    const volScalarField& blendingFactor =
      obr_.lookupObject<volScalarField>(fieldName);
    Info<< type() << " " << name_ << " output:" << nl
      << "    writing field " << blendingFactor.name() << nl
      << endl;
    blendingFactor.write();
  }
}
