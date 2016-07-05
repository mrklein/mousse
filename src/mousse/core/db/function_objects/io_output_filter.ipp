// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "io_output_filter.hpp"
#include "time.hpp"


// Constructors 
template<class OutputFilter>
mousse::IOOutputFilter<OutputFilter>::IOOutputFilter
(
  const word& outputFilterName,
  const IOobject& ioDict,
  const bool readFromFiles
)
:
  IOdictionary{ioDict},
  OutputFilter{outputFilterName, ioDict.db(), *this, readFromFiles}
{}


template<class OutputFilter>
mousse::IOOutputFilter<OutputFilter>::IOOutputFilter
(
  const word& outputFilterName,
  const objectRegistry& obr,
  const word& dictName,
  const IOobject::readOption rOpt,
  const bool readFromFiles
)
:
  IOdictionary
  {
    {
      dictName,
      obr.time().system(),
      obr,
      rOpt,
      IOobject::NO_WRITE
    }
  },
  OutputFilter{outputFilterName, obr, *this, readFromFiles}
{}


template<class OutputFilter>
mousse::IOOutputFilter<OutputFilter>::IOOutputFilter
(
  const word& outputFilterName,
  const objectRegistry& obr,
  const fileName& dictName,
  const IOobject::readOption rOpt,
  const bool readFromFiles
)
:
  IOdictionary
  {
    {
      dictName,
      obr,
      rOpt,
      IOobject::NO_WRITE
    }
  },
  OutputFilter{outputFilterName, obr, *this, readFromFiles}
{}


// Destructor 
template<class OutputFilter>
mousse::IOOutputFilter<OutputFilter>::~IOOutputFilter()
{}


// Member Functions 
template<class OutputFilter>
bool mousse::IOOutputFilter<OutputFilter>::read()
{
  if (regIOobject::read()) {
    OutputFilter::read(*this);
    return true;
  } else {
    return false;
  }
}


template<class OutputFilter>
void mousse::IOOutputFilter<OutputFilter>::write()
{
  OutputFilter::write();
}

