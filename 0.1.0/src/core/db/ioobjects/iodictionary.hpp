#ifndef CORE_DB_IOOBJECTS_IODICTIONARY_HPP_
#define CORE_DB_IOOBJECTS_IODICTIONARY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOdictionary
// Description
//   IOdictionary is derived from dictionary and IOobject to give the dictionary
//   automatic IO functionality via the objectRegistry.  To facilitate IO,
//   IOdictioanry is provided with a constructor from IOobject and writeData and
//   write functions.

#include "dictionary.hpp"
#include "reg_ioobject.hpp"


namespace mousse {

class IOdictionary
:
  public regIOobject,
  public dictionary
{
  // Private data
    static bool writeDictionaries;
  // Private Member Functions
    //- Read dictionary from file
    void readFile(const bool);
public:
  TYPE_NAME("dictionary");
  // Constructors
    //- Construct given an IOobject
    IOdictionary(const IOobject&);
    //- Construct given an IOobject and dictionary
    IOdictionary(const IOobject&, const dictionary&);
    //- Construct given an IOobject and Istream
    IOdictionary(const IOobject&, Istream&);
  //- Destructor
  virtual ~IOdictionary();
  // Member functions
    //- Name function is needed to disambiguate those inherited
    //  from regIOobject and dictionary
    const word& name() const;
    //- ReadData function required for regIOobject read operation
    bool readData(Istream&);
    //- WriteData function required for regIOobject write operation
    bool writeData(Ostream&) const;
  // Member operators
    //- Assignment of other IOdictionary's entries to this IOdictionary
    void operator=(const IOdictionary&);
};
}  // namespace mousse
#endif
