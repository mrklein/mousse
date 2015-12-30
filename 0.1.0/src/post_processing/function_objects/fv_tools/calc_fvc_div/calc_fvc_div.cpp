// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_fvc_div.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "calc_fvc_div.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(calcFvcDiv, 0);
}
// Private Member Functions 
mousse::volScalarField& mousse::calcFvcDiv::divField
(
  const word& divName,
  const dimensionSet& dims
)
{
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  if (!mesh.foundObject<volScalarField>(divName))
  {
    volScalarField* divFieldPtr
    (
      new volScalarField
      (
        IOobject
        (
          divName,
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dims/dimLength, 0.0)
      )
    );
    mesh.objectRegistry::store(divFieldPtr);
  }
  const volScalarField& field = mesh.lookupObject<volScalarField>(divName);
  return const_cast<volScalarField&>(field);
}
// Constructors 
mousse::calcFvcDiv::calcFvcDiv
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
  fieldName_("undefined-fieldName"),
  resultName_("undefined-resultName")
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WarningIn
    (
      "calcFvcDiv::calcFvcDiv"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating." << nl
      << endl;
  }
  read(dict);
}
// Destructor 
mousse::calcFvcDiv::~calcFvcDiv()
{}
// Member Functions 
void mousse::calcFvcDiv::read(const dictionary& dict)
{
  if (active_)
  {
    dict.lookup("fieldName") >> fieldName_;
    dict.lookup("resultName") >> resultName_;
    if (resultName_ == "none")
    {
      resultName_ = "fvc::div(" + fieldName_ + ")";
    }
  }
}
void mousse::calcFvcDiv::execute()
{
  if (active_)
  {
    bool processed = false;
    calcDiv<surfaceScalarField>(fieldName_, resultName_, processed);
    calcDiv<volVectorField>(fieldName_, resultName_, processed);
    if (!processed)
    {
      WarningIn("void mousse::calcFvcDiv::write()")
        << "Unprocessed field " << fieldName_ << endl;
    }
  }
}
void mousse::calcFvcDiv::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::calcFvcDiv::timeSet()
{
  // Do nothing
}
void mousse::calcFvcDiv::write()
{
  if (active_)
  {
    if (obr_.foundObject<regIOobject>(resultName_))
    {
      const regIOobject& field =
        obr_.lookupObject<regIOobject>(resultName_);
      Info<< type() << " " << name_ << " output:" << nl
        << "    writing field " << field.name() << nl << endl;
      field.write();
    }
  }
}
