#ifndef FILE_FORMATS_SAMPLED_SET_WRITERS_CSV_SET_WRITER_HPP_
#define FILE_FORMATS_SAMPLED_SET_WRITERS_CSV_SET_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::csvSetWriter
// Description
//   Write set in csv format

#include "writer.hpp"


namespace mousse {

template<class Type>
class csvSetWriter
:
  public writer<Type>
{
  // Private Member Functions
    void writeCoordHeader(const coordSet&, Ostream&) const;
    void writeHeader(const coordSet&, const wordList&, Ostream&) const;

protected:
  virtual void writeSeparator(Ostream&) const;

public:
  //- Runtime type information
  TYPE_NAME("csv");

  // Constructors

    //- Construct null
    csvSetWriter();

  //- Destructor
  virtual ~csvSetWriter();

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

#include "csv_set_writer.ipp"

#endif
