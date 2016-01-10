// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ensightSetWriter
// Description
// SourceFiles
//   ensight_set_writer.cpp

#ifndef ensight_set_writer_hpp_
#define ensight_set_writer_hpp_

#include "writer.hpp"

namespace mousse
{

template<class Type>
class ensightSetWriter
:
  public writer<Type>
{
public:

  //- Runtime type information
  TYPE_NAME("ensight");

  // Constructors

    //- Construct null
    ensightSetWriter();

  //- Destructor
  virtual ~ensightSetWriter();

  // Member Functions
    virtual fileName getFileName
    (
      const coordSet&,
      const wordList&
    ) const;

    virtual void write
    (
      const coordSet&,
      const wordList&,
      const List<const Field<Type>*>&,
      Ostream&
    ) const;

    virtual void write
    (
      const bool writeTracks,
      const PtrList<coordSet>&,
      const wordList& valueSetNames,
      const List<List<Field<Type> > >&,
      Ostream&
    ) const;
};

}  // namespace mousse

#ifdef NoRepository
#   include "ensight_set_writer.cpp"
#endif

#endif
