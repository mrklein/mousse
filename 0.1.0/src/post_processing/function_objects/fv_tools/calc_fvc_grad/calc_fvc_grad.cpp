// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "calc_fvc_grad.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "calc_fvc_grad.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(calcFvcGrad, 0);
}
// Constructors 
mousse::calcFvcGrad::calcFvcGrad
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
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WARNING_IN
    (
      "calcFvcGrad::calcFvcGrad"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )
    << "No fvMesh available, deactivating." << nl
    << endl;
  }
  read(dict);
}
// Destructor 
mousse::calcFvcGrad::~calcFvcGrad()
{}
// Member Functions 
void mousse::calcFvcGrad::read(const dictionary& dict)
{
  if (active_)
  {
    dict.lookup("fieldName") >> fieldName_;
    dict.lookup("resultName") >> resultName_;
    if (resultName_ == "none")
    {
      resultName_ = "fvc::grad(" + fieldName_ + ")";
    }
  }
}
void mousse::calcFvcGrad::execute()
{
  if (active_)
  {
    bool processed = false;
    calcGrad<scalar>(fieldName_, resultName_, processed);
    calcGrad<vector>(fieldName_, resultName_, processed);
    if (!processed)
    {
      WARNING_IN("void mousse::calcFvcGrad::write()")
        << "Unprocessed field " << fieldName_ << endl;
    }
  }
}
void mousse::calcFvcGrad::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::calcFvcGrad::timeSet()
{
  // Do nothing
}
void mousse::calcFvcGrad::write()
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
