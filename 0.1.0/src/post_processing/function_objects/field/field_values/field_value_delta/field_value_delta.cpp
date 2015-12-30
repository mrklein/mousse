// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value_delta.hpp"
#include "list_ops.hpp"
#include "time.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
// Static Data Members
namespace mousse
{
  namespace fieldValues
  {
    defineTypeNameAndDebug(fieldValueDelta, 0);
  }
  template<>
  const char*
  NamedEnum<fieldValues::fieldValueDelta::operationType, 5>::names[] =
  {
    "add",
    "subtract",
    "min",
    "max",
    "average"
  };
  const NamedEnum<fieldValues::fieldValueDelta::operationType, 5>
    fieldValues::fieldValueDelta::operationTypeNames_;
}
// Constructors 
mousse::fieldValues::fieldValueDelta::fieldValueDelta
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  functionObjectFile(obr, name, typeName),
  name_(name),
  obr_(obr),
  loadFromFiles_(loadFromFiles),
  log_(true),
  operation_(opSubtract),
  source1Ptr_(NULL),
  source2Ptr_(NULL)
{
  read(dict);
}
void mousse::fieldValues::fieldValueDelta::writeFileHeader(const label i)
{
  const wordList& fields1 = source1Ptr_->fields();
  const wordList& fields2 = source2Ptr_->fields();
  DynamicList<word> commonFields(fields1.size());
  forAll(fields1, i)
  {
    label index = findIndex(fields2, fields1[i]);
    if (index != -1)
    {
      commonFields.append(fields1[i]);
    }
  }
  Ostream& os = file();
  writeHeaderValue(os, "Source1", source1Ptr_->name());
  writeHeaderValue(os, "Source2", source2Ptr_->name());
  writeHeaderValue(os, "Operation", operationTypeNames_[operation_]);
  writeCommented(os, "Time");
  forAll(commonFields, i)
  {
    os  << tab << commonFields[i];
  }
  os  << endl;
}
// Destructor 
mousse::fieldValues::fieldValueDelta::~fieldValueDelta()
{}
// Member Functions 
void mousse::fieldValues::fieldValueDelta::read(const dictionary& dict)
{
  log_ = dict.lookupOrDefault<Switch>("log", true);
  source1Ptr_.reset
  (
    fieldValue::New
    (
      name_ + ".source1",
      obr_,
      dict.subDict("source1"),
      loadFromFiles_,
      false
    ).ptr()
  );
  source2Ptr_.reset
  (
    fieldValue::New
    (
      name_ + ".source2",
      obr_,
      dict.subDict("source2"),
      loadFromFiles_,
      false
    ).ptr()
  );
  operation_ = operationTypeNames_.read(dict.lookup("operation"));
}
void mousse::fieldValues::fieldValueDelta::write()
{
  functionObjectFile::write();
  source1Ptr_->write();
  source2Ptr_->write();
  if (Pstream::master())
  {
    file()<< obr_.time().value();
  }
  if (log_) Info<< type() << " " << name_ << " output:" << endl;
  bool found = false;
  processFields<scalar>(found);
  processFields<vector>(found);
  processFields<sphericalTensor>(found);
  processFields<symmTensor>(found);
  processFields<tensor>(found);
  if (Pstream::master())
  {
    file()<< endl;
  }
  if (log_)
  {
    if (!found)
    {
      Info<< "    none" << endl;
    }
    else
    {
      Info<< endl;
    }
  }
}
void mousse::fieldValues::fieldValueDelta::execute()
{
  // Do nothing
}
void mousse::fieldValues::fieldValueDelta::end()
{
  // Do nothing
}
void mousse::fieldValues::fieldValueDelta::timeSet()
{
  // Do nothing
}
void mousse::fieldValues::fieldValueDelta::updateMesh(const mapPolyMesh&)
{
  // Do nothing
}
void mousse::fieldValues::fieldValueDelta::movePoints(const polyMesh&)
{
  // Do nothing
}
