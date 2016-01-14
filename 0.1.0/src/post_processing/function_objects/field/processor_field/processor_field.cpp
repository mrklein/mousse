// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_field.hpp"
#include "dictionary.hpp"
#include "pstream.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(processorField, 0);
}
// Constructors 
mousse::processorField::processorField
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  active_{true}
{
  // Check if the available mesh is an fvMesh otherise deactivate
  if (isA<fvMesh>(obr_))
  {
    read(dict);
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volScalarField* procFieldPtr
    (
      new volScalarField
      (
        IOobject
        (
          "processorID",
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("0", dimless, 0.0)
      )
    );
    mesh.objectRegistry::store(procFieldPtr);
  }
  else
  {
    active_ = false;
    WARNING_IN
    (
      "processorField::processorField"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_
      << endl;
  }
}
// Destructor 
mousse::processorField::~processorField()
{}
// Member Functions 
void mousse::processorField::read(const dictionary&)
{
  // do nothing
}
void mousse::processorField::execute()
{
  if (active_)
  {
    const volScalarField& procField =
      obr_.lookupObject<volScalarField>("processorID");
    const_cast<volScalarField&>(procField) ==
      dimensionedScalar("procI", dimless, Pstream::myProcNo());
  }
}
void mousse::processorField::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::processorField::timeSet()
{
  // Do nothing
}
void mousse::processorField::write()
{
  if (active_)
  {
    const volScalarField& procField =
      obr_.lookupObject<volScalarField>("processorID");
    procField.write();
  }
}
