// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "csv_set_writer.hpp"
#include "coord_set.hpp"
#include "file_name.hpp"
#include "ofstream.hpp"
// Constructors 
template<class Type>
mousse::csvSetWriter<Type>::csvSetWriter()
:
  writer<Type>()
{}
// Destructor 
template<class Type>
mousse::csvSetWriter<Type>::~csvSetWriter()
{}
// Member Functions 
template<class Type>
mousse::fileName mousse::csvSetWriter<Type>::getFileName
(
  const coordSet& points,
  const wordList& valueSetNames
) const
{
  return this->getBaseName(points, valueSetNames) + ".csv";
}
template<class Type>
void mousse::csvSetWriter<Type>::write
(
  const coordSet& points,
  const wordList& valueSetNames,
  const List<const Field<Type>*>& valueSets,
  Ostream& os
) const
{
  writeHeader(points,valueSetNames,os);
  // Collect sets into columns
  List<const List<Type>*> columns(valueSets.size());
  forAll(valueSets, i)
  {
    columns[i] = valueSets[i];
  }
  this->writeTable(points, columns, os);
}
template<class Type>
void mousse::csvSetWriter<Type>::write
(
  const bool writeTracks,
  const PtrList<coordSet>& points,
  const wordList& valueSetNames,
  const List<List<Field<Type> > >& valueSets,
  Ostream& os
) const
{
  writeHeader(points[0],valueSetNames,os);
  if (valueSets.size() != valueSetNames.size())
  {
    FatalErrorIn("csvSetWriter<Type>::write(..)")
      << "Number of variables:" << valueSetNames.size() << endl
      << "Number of valueSets:" << valueSets.size()
      << exit(FatalError);
  }
  List<const List<Type>*> columns(valueSets.size());
  forAll(points, trackI)
  {
    // Collect sets into columns
    forAll(valueSets, i)
    {
      columns[i] = &valueSets[i][trackI];
    }
    this->writeTable(points[trackI], columns, os);
    os  << nl << nl;
  }
}
template<class Type>
void mousse::csvSetWriter<Type>::writeSeparator(Ostream& os) const
{
  os << token::COMMA;
}
namespace mousse
{
  // otherwise compiler complains about specialization
  template<>
  void csvSetWriter<scalar>::writeHeader
  (
    const coordSet& points,
    const wordList& valueSetNames,
    Ostream& os
  ) const
  {
    writeCoordHeader(points, os);
    forAll(valueSetNames, i)
    {
      if (i > 0)
      {
        writeSeparator(os);
      }
      os << valueSetNames[i];
    }
    os << nl;
  }
} // end namespace
template<class Type>
void mousse::csvSetWriter<Type>::writeHeader
(
  const coordSet& points,
  const wordList& valueSetNames,
  Ostream& os
) const
{
  writeCoordHeader(points, os);
  forAll(valueSetNames, i)
  {
    for (label j=0; j<Type::nComponents; j++)
    {
      if (i>0 || j>0)
      {
        writeSeparator(os);
      }
      os << valueSetNames[i] << "_" << j;
    }
  }
  os << nl;
}
template<class Type>
void mousse::csvSetWriter<Type>::writeCoordHeader
(
  const coordSet& points,
  Ostream& os
) const
{
  if (points.hasVectorAxis())
  {
    forAll(points, i)
    {
      os << points.axis()[i];
      writeSeparator(os);
    }
  }
  else
  {
    os << points.axis();
    writeSeparator(os);
  }
}
