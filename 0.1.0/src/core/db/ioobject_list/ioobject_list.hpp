#ifndef CORE_DB_IOOBJECT_LIST_IOOBJECT_LIST_HPP_
#define CORE_DB_IOOBJECT_LIST_IOOBJECT_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOobjectList
// Description
//   List of IOobjects with searching and retrieving facilities.
// SourceFiles
//   ioobject_list.cpp


#include "hash_ptr_table.hpp"
#include "ioobject.hpp"
#include "word_re_list.hpp"

namespace mousse
{

class IOobjectList
:
  public HashPtrTable<IOobject>
{
public:

  // Constructors

    //- Construct given an initial estimate for the number of entries
    explicit IOobjectList(const label nIoObjects = 128);

    //- Construct from objectRegistry and instance path
    IOobjectList
    (
      const objectRegistry& db,
      const fileName& instance,
      const fileName& local = "",
      IOobject::readOption r = IOobject::MUST_READ,
      IOobject::writeOption w = IOobject::NO_WRITE,
      bool registerObject = true
    );

    //- Construct as copy
    IOobjectList(const IOobjectList&);

    //- Disallow default bitwise assignment
    IOobjectList& operator=(const IOobjectList&) = delete;

  //- Destructor
  ~IOobjectList();

  // Member functions

    //- Add an IOobject to the list
    bool add(IOobject&);

    //- Remove an IOobject from the list
    bool remove(IOobject&);

    //- Lookup a given name and return IOobject ptr if found else NULL
    IOobject* lookup(const word& name) const;

    //- Return the list for all IOobects whose name matches name
    IOobjectList lookup(const wordRe& name) const;

    //- Return the list for all IOobects whose name matches name
    IOobjectList lookup(const wordReList& patterns) const;

    //- Return the list for all IOobjects of a given class
    IOobjectList lookupClass(const word& className) const;

    //- Return the list of names of the IOobjects
    wordList names() const;

    //- Return the sorted list of names of the IOobjects
    wordList sortedNames() const;

    //- Return the list of names of the IOobjects of given class
    wordList names(const word& className) const;

    //- Return the sorted list of names of the IOobjects of given class
    wordList sortedNames(const word& className) const;

};

}  // namespace mousse

#endif
