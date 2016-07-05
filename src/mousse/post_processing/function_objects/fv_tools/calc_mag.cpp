// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_mag.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "calc_mag.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(calcMag, 0);

}


// Constructors 
mousse::calcMag::calcMag
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
  fieldName_{"undefined-fieldName"},
  resultName_{"undefined-resultName"}
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_)) {
    active_ = false;
    WARNING_IN
    (
      "calcMag::calcMag"
      "("
      "  const word&, "
      "  const objectRegistry&, "
      "  const dictionary&, "
      "  const bool"
      ")"
    )
    << "No fvMesh available, deactivating." << nl
    << endl;
  }
  read(dict);
}


// Destructor 
mousse::calcMag::~calcMag()
{}


// Member Functions 
void mousse::calcMag::read(const dictionary& dict)
{
  if (!active_)
    return;
  dict.lookup("fieldName") >> fieldName_;
  dict.lookup("resultName") >> resultName_;
  if (resultName_ == "none") {
    resultName_ = "mag(" + fieldName_ + ")";
  }
}


void mousse::calcMag::execute()
{
  if (!active_)
    return;
  bool processed = false;
  calc<scalar>(fieldName_, resultName_, processed);
  calc<vector>(fieldName_, resultName_, processed);
  calc<sphericalTensor>(fieldName_, resultName_, processed);
  calc<symmTensor>(fieldName_, resultName_, processed);
  calc<tensor>(fieldName_, resultName_, processed);
  if (!processed) {
    WARNING_IN("void mousse::calcMag::write()")
      << "Unprocessed field " << fieldName_ << endl;
  }
}


void mousse::calcMag::end()
{
  if (active_) {
    execute();
  }
}


void mousse::calcMag::timeSet()
{
  // Do nothing
}


void mousse::calcMag::write()
{
  if (!active_)
    return;
  if (!obr_.foundObject<regIOobject>(resultName_))
    return;
  const regIOobject& field = obr_.lookupObject<regIOobject>(resultName_);
  Info << type() << " " << name_ << " output:" << nl
    << "    writing field " << field.name() << nl << endl;
  field.write();
}

