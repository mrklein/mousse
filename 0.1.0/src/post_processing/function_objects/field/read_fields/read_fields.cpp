// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "read_fields.hpp"
#include "dictionary.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(readFields, 0);
}
// Constructors 
mousse::readFields::readFields
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
  fieldSet_{}
{
  // Check if the available mesh is an fvMesh otherise deactivate
  if (isA<fvMesh>(obr_))
  {
    read(dict);
  }
  else
  {
    active_ = false;
    WARNING_IN
    (
      "readFields::readFields"
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
mousse::readFields::~readFields()
{}
// Member Functions 
void mousse::readFields::read(const dictionary& dict)
{
  if (active_)
  {
    dict.lookup("fields") >> fieldSet_;
  }
}
void mousse::readFields::execute()
{
  if (active_)
  {
    // Clear out any previously loaded fields
    vsf_.clear();
    vvf_.clear();
    vSpheretf_.clear();
    vSymmtf_.clear();
    vtf_.clear();
    ssf_.clear();
    svf_.clear();
    sSpheretf_.clear();
    sSymmtf_.clear();
    stf_.clear();
    FOR_ALL(fieldSet_, fieldI)
    {
      const word& fieldName = fieldSet_[fieldI];
      // If necessary load field
      loadField<scalar>(fieldName, vsf_, ssf_);
      loadField<vector>(fieldName, vvf_, svf_);
      loadField<sphericalTensor>(fieldName, vSpheretf_, sSpheretf_);
      loadField<symmTensor>(fieldName, vSymmtf_, sSymmtf_);
      loadField<tensor>(fieldName, vtf_, stf_);
    }
  }
}
void mousse::readFields::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::readFields::timeSet()
{
  // Do nothing
}
void mousse::readFields::write()
{
  // Do nothing
}
