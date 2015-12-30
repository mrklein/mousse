// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::gnuplotSetWriter
// Description
// SourceFiles
//   gnuplot_set_writer.cpp
#ifndef gnuplot_set_writer_hpp_
#define gnuplot_set_writer_hpp_
#include "writer.hpp"
namespace mousse
{
template<class Type>
class gnuplotSetWriter
:
  public writer<Type>
{
public:
  //- Runtime type information
  TypeName("gnuplot");
  // Constructors
    //- Construct null
    gnuplotSetWriter();
  //- Destructor
  virtual ~gnuplotSetWriter();
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
#   include "gnuplot_set_writer.cpp"
#endif
#endif
