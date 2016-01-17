// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(fieldValue, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(fieldValue, dictionary);
}
// Protected Member Functions 
void mousse::fieldValue::read(const dictionary& dict)
{
  if (active_)
  {
    dict_ = dict;
    log_ = dict.lookupOrDefault<Switch>("log", true);
    dict.lookup("fields") >> fields_;
    dict.lookup("valueOutput") >> valueOutput_;
  }
}
void mousse::fieldValue::write()
{
  if (active_)
  {
    functionObjectFile::write();
    if (log_) Info<< type() << " " << name_ << " output:" << nl;
  }
}
// Constructors 
mousse::fieldValue::fieldValue
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const word& valueType,
  const bool /*loadFromFiles*/
)
:
  functionObjectFile(obr, name, valueType),
  name_(name),
  obr_(obr),
  dict_(dict),
  active_(true),
  log_(true),
  sourceName_(word::null),
  fields_(dict.lookup("fields")),
  valueOutput_(dict.lookup("valueOutput")),
  resultDict_(fileName("name"), dictionary::null)
{
  // Only active if obr is an fvMesh
  if (isA<fvMesh>(obr_))
  {
    read(dict);
  }
  else
  {
    WARNING_IN
    (
      "fieldValue::fieldValue"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name << nl
      << endl;
    active_ = false;
  }
}
// Destructor 
mousse::fieldValue::~fieldValue()
{}
// Member Functions 
void mousse::fieldValue::execute()
{
  // Do nothing
}
void mousse::fieldValue::end()
{
  // Do nothing
}
void mousse::fieldValue::timeSet()
{
  // Do nothing
}
void mousse::fieldValue::updateMesh(const mapPolyMesh&)
{
  // Do nothing
}
void mousse::fieldValue::movePoints(const polyMesh&)
{
  // Do nothing
}
