#ifndef FILE_FORMATS_SAMPLED_SET_WRITERS_JPLOT_SET_WRITER_HPP_
#define FILE_FORMATS_SAMPLED_SET_WRITERS_JPLOT_SET_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::jplotSetWriter
// SourceFiles
//   jplot_set_writer.cpp


#include "writer.hpp"
#include "vector.hpp"

namespace mousse
{

template<class Type>
class jplotSetWriter
:
  public writer<Type>
{
  // Private Member Functions

    //- Write header
    Ostream& writeHeader(Ostream&) const;

public:

  //- Runtime type information
  TYPE_NAME("jplot");

  // Constructors

    //- Construct null
    jplotSetWriter();

  //- Destructor
  virtual ~jplotSetWriter();

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
      const bool /*writeTracks*/,
      const PtrList<coordSet>&,
      const wordList& /*valueSetNames*/,
      const List<List<Field<Type> > >&,
      Ostream&
    ) const
    {
      NOT_IMPLEMENTED
      (
        "jplotSetWriter<Type>::write\n"
        "(\n"
        "    const bool,\n"
        "    const PtrList<coordSet>&,\n"
        "    const wordList&,\n"
        "    const List<List<Field<Type> > >&,\n"
        "    Ostream&\n"
        ") const"
      );
    }
};

}  // namespace mousse

#ifdef NoRepository
#   include "jplot_set_writer.cpp"
#endif

#endif
