#ifndef FILE_FORMATS_SAMPLED_SET_WRITERS_XMGRACE_SET_WRITER_HPP_
#define FILE_FORMATS_SAMPLED_SET_WRITERS_XMGRACE_SET_WRITER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::xmgraceSetWriter
// Description
// SourceFiles
//   xmgrace_set_writer.cpp


#include "writer.hpp"

namespace mousse
{

template<class Type>
class xmgraceSetWriter
:
  public writer<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("xmgr");

  // Constructors

    //- Construct null
    xmgraceSetWriter();

  //- Destructor
  virtual ~xmgraceSetWriter();

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
      const List<List<Field<Type>>>&,
      Ostream&
    ) const;
};

}  // namespace mousse

#ifdef NoRepository
#   include "xmgrace_set_writer.cpp"
#endif
#endif
