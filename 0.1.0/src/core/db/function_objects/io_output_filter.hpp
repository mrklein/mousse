#ifndef CORE_DB_FUNCTION_OBJECTS_IO_OUTPUT_FILTER_HPP_
#define CORE_DB_FUNCTION_OBJECTS_IO_OUTPUT_FILTER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//
// Class
//   mousse::IOOutputFilter
// Description
//   IOdictionary wrapper around OutputFilter to allow them to read from
//   their associated dictionaries.
// Note
//   The IOobject or the objectRegistry will normally have to be
//   derived from a fvMesh for a subsequent cast (within OutputFilter)
//   to work correctly.

#include "iodictionary.hpp"
#include "point_field_fwd.hpp"


namespace mousse {

// Forward declaration of classes
class mapPolyMesh;

template<class OutputFilter>
class IOOutputFilter
:
  public IOdictionary,
  public OutputFilter
{
public:

  // Constructors

    //- Construct from an IOobject for IOdictionary
    //  Allow the possibility to load fields from files
    IOOutputFilter
    (
      const word& outputFilterName,
      const IOobject& ioDict,
      const bool loadFromFile = false
    );

    //- Construct for given objectRegistry and dictionary
    //  Allow dictionary to be optional
    //  Allow the possibility to load fields from files
    IOOutputFilter
    (
      const word& outputFilterName,
      const objectRegistry&,
      const word& dictName = OutputFilter::typeName() + "Dict",
      const IOobject::readOption rOpt = IOobject::MUST_READ_IF_MODIFIED,
      const bool loadFromFile = false
    );

    //- Construct for given objectRegistry and dictionary
    //  Dictionary read from full path.
    //  Allow the possibility to load fields from files
    IOOutputFilter
    (
      const word& outputFilterName,
      const objectRegistry&,
      const fileName& dictName,
      const IOobject::readOption rOpt = IOobject::MUST_READ_IF_MODIFIED,
      const bool loadFromFile = false
    );

    // Disallow default bitwise copy construct and assignment
    IOOutputFilter(const IOOutputFilter&) = delete;

    IOOutputFilter& operator=(const IOOutputFilter&) = delete;

  //- Destructor
  virtual ~IOOutputFilter();

  // Member Functions

    //- Return name
    virtual const word& name() const
    {
      return IOdictionary::name();
    }

    //- Inherit read from OutputFilter
    using OutputFilter::read;

    //- Read output filter properties
    virtual bool read();

    //- Inherit write from regIOobject
    using regIOobject::write;

    //- Sample and write
    virtual void write();

    //- Update for changes of mesh
    virtual void updateMesh(const mapPolyMesh& mpm)
    {
      read();
      OutputFilter::updateMesh(mpm);
    }

    //- Update for changes of mesh
    virtual void movePoints(const polyMesh& mesh)
    {
      read();
      OutputFilter::movePoints(mesh);
    }

};

}  // namespace mousse

#include "io_output_filter.ipp"

#endif
