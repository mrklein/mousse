// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "remove_registered_object.hpp"
#include "dictionary.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(removeRegisteredObject, 0);
}
// Constructors 
mousse::removeRegisteredObject::removeRegisteredObject
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  objectNames_{}
{
  read(dict);
}
// Destructor 
mousse::removeRegisteredObject::~removeRegisteredObject()
{}
// Member Functions 
void mousse::removeRegisteredObject::read(const dictionary& dict)
{
  dict.lookup("objectNames") >> objectNames_;
}
void mousse::removeRegisteredObject::execute()
{
  FOR_ALL(objectNames_, i)
  {
    if (obr_.foundObject<regIOobject>(objectNames_[i]))
    {
      const regIOobject& obj =
        obr_.lookupObject<regIOobject>(objectNames_[i]);
      if (obj.ownedByRegistry())
      {
        Info<< type() << " " << name_ << " output:" << nl
          << "    removing object " << obj.name() << nl
          << endl;
        const_cast<regIOobject&>(obj).release();
        delete &obj;
      }
    }
  }
}
void mousse::removeRegisteredObject::end()
{
  execute();
}
void mousse::removeRegisteredObject::timeSet()
{
  // Do nothing - only valid on execute
}
void mousse::removeRegisteredObject::write()
{
  // Do nothing - only valid on execute
}
