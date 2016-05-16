// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "residuals.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(residuals, 0);

}


// Constructors 
mousse::residuals::residuals
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  functionObjectFile{obr, name, typeName},
  name_{name},
  obr_{obr},
  active_{true},
  fieldSet_{}
{
  // Check if the available mesh is an fvMesh otherwise deactivate
  if (!isA<fvMesh>(obr_)) {
    active_ = false;
    WARNING_IN
    (
      "residuals::residuals"
      "("
      "  const word&, "
      "  const objectRegistry&, "
      "  const dictionary&, "
      "  const bool"
      ")"
    )
    << "No fvMesh available, deactivating " << name_
    << endl;
  }
  read(dict);
}


// Destructor 
mousse::residuals::~residuals()
{}


// Member Functions 
void mousse::residuals::read(const dictionary& dict)
{
  if (active_) {
    dict.lookup("fields") >> fieldSet_;
  }
}


void mousse::residuals::writeFileHeader(const label /*i*/)
{
  if (!Pstream::master())
    return;
  writeHeader(file(), "Residuals");
  writeCommented(file(), "Time");
  FOR_ALL(fieldSet_, fieldI) {
    writeTabbed(file(), fieldSet_[fieldI]);
  }
  file() << endl;
}


void mousse::residuals::execute()
{
  // Do nothing - only valid on write
}


void mousse::residuals::end()
{
  // Do nothing - only valid on write
}


void mousse::residuals::timeSet()
{
  // Do nothing - only valid on write
}


void mousse::residuals::write()
{
  if (!active_)
    return;
  functionObjectFile::write();
  if (!Pstream::master())
    return;
  file() << obr_.time().value();
  FOR_ALL(fieldSet_, fieldI) {
    const word& fieldName = fieldSet_[fieldI];
    writeResidual<scalar>(fieldName);
    writeResidual<vector>(fieldName);
    writeResidual<sphericalTensor>(fieldName);
    writeResidual<symmTensor>(fieldName);
    writeResidual<tensor>(fieldName);
  }
  file() << endl;
}

