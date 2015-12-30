// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_registered_object.hpp"
#include "dictionary.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{
defineTypeNameAndDebug(writeRegisteredObject, 0);
}
// Constructors 
mousse::writeRegisteredObject::writeRegisteredObject
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  name_(name),
  exclusiveWriting_(false),
  obr_(obr),
  objectNames_()
{
  read(dict);
}
// Destructor 
mousse::writeRegisteredObject::~writeRegisteredObject()
{}
// Member Functions 
void mousse::writeRegisteredObject::read(const dictionary& dict)
{
  dict.lookup("objectNames") >> objectNames_;
  dict.readIfPresent("exclusiveWriting", exclusiveWriting_);
}
void mousse::writeRegisteredObject::execute()
{
  // Do nothing - only valid on write
}
void mousse::writeRegisteredObject::end()
{
  // Do nothing - only valid on write
}
void mousse::writeRegisteredObject::timeSet()
{
  // Do nothing - only valid on write
}
void mousse::writeRegisteredObject::write()
{
  Info<< type() << " " << name_ << " output:" << nl;
  DynamicList<word> allNames(obr_.toc().size());
  forAll(objectNames_, i)
  {
    wordList names(obr_.names<regIOobject>(objectNames_[i]));
    if (names.size())
    {
      allNames.append(names);
    }
    else
    {
      WarningIn("mousse::writeRegisteredObject::write()")
        << "Object " << objectNames_[i] << " not found in "
        << "database. Available objects:" << nl << obr_.sortedToc()
        << endl;
    }
  }
  forAll(allNames, i)
  {
    regIOobject& obj =
      const_cast<regIOobject&>
      (
        obr_.lookupObject<regIOobject>(allNames[i])
      );
    if (exclusiveWriting_)
    {
      // Switch off automatic writing to prevent double write
      obj.writeOpt() = IOobject::NO_WRITE;
    }
    Info<< "    writing object " << obj.name() << nl << endl;
    obj.write();
  }
}
