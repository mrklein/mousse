// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iodictionary.hpp"
#include "pstream.hpp"
#include "iostreams.hpp"
#include "ipstream.hpp"
#include "opstream.hpp"

// Private Member Functions
// Parallel aware reading, using non-virtual type information (typeName instead
// of type()) because of use in constructor.
void mousse::IOdictionary::readFile(const bool masterOnly)
{
  if (Pstream::master() || !masterOnly)
  {
    if (debug)
    {
      Pout<< "IOdictionary : Reading " << objectPath()
        << " from file " << endl;
    }
    // Set flag for e.g. codeStream
    bool oldFlag = regIOobject::masterOnlyReading;
    regIOobject::masterOnlyReading = masterOnly;
    // Read file
    readStream(typeName) >> *this;
    close();
    regIOobject::masterOnlyReading = oldFlag;
    if (writeDictionaries && Pstream::master())
    {
      Sout<< nl
        << "--- IOdictionary " << name()
        << ' ' << objectPath() << ":" << nl;
      writeHeader(Sout);
      writeData(Sout);
      Sout<< "--- End of IOdictionary " << name() << nl << endl;
    }
  }
  if (masterOnly && Pstream::parRun())
  {
    // Scatter master data using communication scheme
    const List<Pstream::commsStruct>& comms =
    (
      (Pstream::nProcs() < Pstream::nProcsSimpleSum)
     ? Pstream::linearCommunication()
     : Pstream::treeCommunication()
    );
    // Master reads headerclassname from file. Make sure this gets
    // transfered as well as contents.
    Pstream::scatter
    (
      comms,
      const_cast<word&>(headerClassName()),
      Pstream::msgType(),
      Pstream::worldComm
    );
    Pstream::scatter(comms, note(), Pstream::msgType(), Pstream::worldComm);
    // Get my communication order
    const Pstream::commsStruct& myComm = comms[Pstream::myProcNo()];
    // Reveive from up
    if (myComm.above() != -1)
    {
      if (debug)
      {
        Pout<< "IOdictionary : Reading " << objectPath()
          << " from processor " << myComm.above() << endl;
      }
      // Note: use ASCII for now - binary IO of dictionaries is
      // not currently supported or rather the primitiveEntries of
      // the dictionary think they are in binary form whereas they are
      // not. Could reset all the ITstreams to ascii?
      IPstream fromAbove
      (
        Pstream::scheduled,
        myComm.above(),
        0,
        Pstream::msgType(),
        Pstream::worldComm,
        IOstream::ASCII
      );
      IOdictionary::readData(fromAbove);
    }
    // Send to my downstairs neighbours
    FOR_ALL(myComm.below(), belowI)
    {
      if (debug)
      {
        Pout<< "IOdictionary : Sending " << objectPath()
          << " to processor " << myComm.below()[belowI] << endl;
      }
      OPstream toBelow
      (
        Pstream::scheduled,
        myComm.below()[belowI],
        0,
        Pstream::msgType(),
        Pstream::worldComm,
        IOstream::ASCII
      );
      IOdictionary::writeData(toBelow);
    }
  }
}

// Members Functions
bool mousse::IOdictionary::readData(Istream& is)
{
  is >> *this;
  if (writeDictionaries && Pstream::master() && !is.bad())
  {
    Sout<< nl
      << "--- IOdictionary " << name()
      << ' ' << objectPath() << ":" << nl;
    writeHeader(Sout);
    writeData(Sout);
    Sout<< "--- End of IOdictionary " << name() << nl << endl;
  }
  return !is.bad();
}
bool mousse::IOdictionary::writeData(Ostream& os) const
{
  dictionary::write(os, false);
  return os.good();
}
