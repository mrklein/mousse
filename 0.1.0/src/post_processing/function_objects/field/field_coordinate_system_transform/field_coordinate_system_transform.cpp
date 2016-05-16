// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_coordinate_system_transform.hpp"
#include "dictionary.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(fieldCoordinateSystemTransform, 0);

}


// Constructors 
mousse::fieldCoordinateSystemTransform::fieldCoordinateSystemTransform
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
  fieldSet_{},
  coordSys_{obr, dict}
{
  // Check if the available mesh is an fvMesh otherise deactivate
  if (isA<fvMesh>(obr_)) {
    read(dict);
    Info << type() << " " << name_ << ":" << nl
      << "   Applying transformation from global Cartesian to local "
      << coordSys_ << nl << endl;
  } else {
    active_ = false;
    WARNING_IN
    (
      "fieldCoordinateSystemTransform::fieldCoordinateSystemTransform"
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
}


// Destructor 
mousse::fieldCoordinateSystemTransform::~fieldCoordinateSystemTransform()
{}


// Member Functions 
void mousse::fieldCoordinateSystemTransform::read(const dictionary& dict)
{
  if (active_) {
    dict.lookup("fields") >> fieldSet_;
  }
}


void mousse::fieldCoordinateSystemTransform::execute()
{
  if (!active_)
    return;
  Info << type() << " " << name_ << " output:" << nl;
  FOR_ALL(fieldSet_, fieldI) {
    // If necessary load field
    transform<scalar>(fieldSet_[fieldI]);
    transform<vector>(fieldSet_[fieldI]);
    transform<sphericalTensor>(fieldSet_[fieldI]);
    transform<symmTensor>(fieldSet_[fieldI]);
    transform<tensor>(fieldSet_[fieldI]);
  }
}


void mousse::fieldCoordinateSystemTransform::end()
{
  if (!active_)
    return;
  execute();
}


void mousse::fieldCoordinateSystemTransform::timeSet()
{
  // Do nothing
}


void mousse::fieldCoordinateSystemTransform::write()
{
  if (!active_)
    return;
  Info << type() << " " << name_ << " output:" << nl;
  FOR_ALL(fieldSet_, fieldI) {
    const word fieldName = fieldSet_[fieldI] + ":Transformed";
    const regIOobject& field =
      obr_.lookupObject<regIOobject>(fieldName);
    Info << "    writing field " << field.name() << nl;
    field.write();
  }
  Info << endl;
}

