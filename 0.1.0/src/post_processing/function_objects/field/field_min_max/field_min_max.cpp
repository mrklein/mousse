// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_min_max.hpp"
#include "field_types.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(fieldMinMax, 0);
  template<>
  const char* NamedEnum
  <
    fieldMinMax::modeType,
    2
  >::names[] =
  {
    "magnitude",
    "component"
  };
}
const mousse::NamedEnum<mousse::fieldMinMax::modeType, 2>
mousse::fieldMinMax::modeTypeNames_;
// Constructors 
mousse::fieldMinMax::fieldMinMax
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
  active_(true),
  log_(true),
  location_(true),
  mode_(mdMag),
  fieldSet_()
{
  // Check if the available mesh is an fvMesh otherise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WarningIn
    (
      "fieldMinMax::fieldMinMax"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )   << "No fvMesh available, deactivating " << name_
      << endl;
  }
  read(dict);
}
// Destructor 
mousse::fieldMinMax::~fieldMinMax()
{}
// Member Functions 
void mousse::fieldMinMax::read(const dictionary& dict)
{
  if (active_)
  {
    log_ = dict.lookupOrDefault<Switch>("log", true);
    location_ = dict.lookupOrDefault<Switch>("location", true);
    mode_ = modeTypeNames_[dict.lookupOrDefault<word>("mode", "magnitude")];
    dict.lookup("fields") >> fieldSet_;
  }
}
void mousse::fieldMinMax::writeFileHeader(const label i)
{
  OFstream& file = this->file();
  writeHeader(file, "Field minima and maxima");
  writeCommented(file, "Time");
  if (location_)
  {
    writeTabbed(file, "field");
    writeTabbed(file, "min");
    writeTabbed(file, "location(min)");
    if (Pstream::parRun())
    {
      writeTabbed(file, "processor");
    }
    writeTabbed(file, "max");
    writeTabbed(file, "location(max)");
    if (Pstream::parRun())
    {
      writeTabbed(file, "processor");
    }
  }
  else
  {
    forAll(fieldSet_, fieldI)
    {
      writeTabbed(file, "min(" + fieldSet_[fieldI] + ')');
      writeTabbed(file, "max(" + fieldSet_[fieldI] + ')');
    }
  }
  file<< endl;
}
void mousse::fieldMinMax::execute()
{
  // Do nothing - only valid on write
}
void mousse::fieldMinMax::end()
{
  // Do nothing - only valid on write
}
void mousse::fieldMinMax::timeSet()
{
  // Do nothing - only valid on write
}
void mousse::fieldMinMax::write()
{
  if (active_)
  {
    functionObjectFile::write();
    if (!location_) file()<< obr_.time().value();
    if (log_) Info<< type() << " " << name_ <<  " output:" << nl;
    forAll(fieldSet_, fieldI)
    {
      calcMinMaxFields<scalar>(fieldSet_[fieldI], mdCmpt);
      calcMinMaxFields<vector>(fieldSet_[fieldI], mode_);
      calcMinMaxFields<sphericalTensor>(fieldSet_[fieldI], mode_);
      calcMinMaxFields<symmTensor>(fieldSet_[fieldI], mode_);
      calcMinMaxFields<tensor>(fieldSet_[fieldI], mode_);
    }
    if (!location_) file()<< endl;
    if (log_) Info<< endl;
  }
}
