// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_properties.hpp"
#include "iodictionary.hpp"


// Constructors 
mousse::regionProperties::regionProperties(const Time& runTime)
:
  HashTable<wordList>
  {
    IOdictionary
    {
      IOobject
      {
        "regionProperties",
        runTime.time().constant(),
        runTime.db(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
      }
    }.lookup("regions")
  }
{}


// Destructor 
mousse::regionProperties::~regionProperties()
{}

