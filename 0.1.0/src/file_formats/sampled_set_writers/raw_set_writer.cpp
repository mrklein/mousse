// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "raw_set_writer.hpp"
#include "coord_set.hpp"
#include "file_name.hpp"
#include "ofstream.hpp"
// Constructors 
template<class Type>
mousse::rawSetWriter<Type>::rawSetWriter()
:
  writer<Type>()
{}
// Destructor 
template<class Type>
mousse::rawSetWriter<Type>::~rawSetWriter()
{}
// Member Functions 
template<class Type>
mousse::fileName mousse::rawSetWriter<Type>::getFileName
(
  const coordSet& points,
  const wordList& valueSetNames
) const
{
  return this->getBaseName(points, valueSetNames) + ".xy";
}
template<class Type>
void mousse::rawSetWriter<Type>::write
(
  const coordSet& points,
  const wordList& valueSetNames,
  const List<const Field<Type>*>& valueSets,
  Ostream& os
) const
{
  // Collect sets into columns
  List<const List<Type>*> columns(valueSets.size());
  forAll(valueSets, i)
  {
    columns[i] = valueSets[i];
  }
  this->writeTable(points, columns, os);
}
template<class Type>
void mousse::rawSetWriter<Type>::write
(
  const bool writeTracks,
  const PtrList<coordSet>& points,
  const wordList& valueSetNames,
  const List<List<Field<Type> > >& valueSets,
  Ostream& os
) const
{
  if (valueSets.size() != valueSetNames.size())
  {
    FatalErrorIn("rawSetWriter<Type>::write(..)")
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
