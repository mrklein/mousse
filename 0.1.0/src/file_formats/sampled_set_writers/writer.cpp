// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "writer.hpp"
#include "coord_set.hpp"
#include "ofstream.hpp"
#include "os_specific.hpp"
// Static Data Members
template<class Type>
mousse::autoPtr< mousse::writer<Type> > mousse::writer<Type>::New
(
  const word& writeType
)
{
  typename wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(writeType);
  if (cstrIter == wordConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "writer::New(const word&)"
    )   << "Unknown write type "
      << writeType << nl << nl
      << "Valid write types : " << endl
      << wordConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<writer<Type> >(cstrIter()());
}
// Private Member Functions 
template<class Type>
mousse::fileName mousse::writer<Type>::getBaseName
(
  const coordSet& points,
  const wordList& valueSets
) const
{
  fileName fName(points.name());
  forAll(valueSets, i)
  {
    fName += '_' + valueSets[i];
  }
  return fName;
}
template<class Type>
void mousse::writer<Type>::writeCoord
(
  const coordSet& points,
  const label pointI,
  Ostream& os
) const
{
  if (points.hasVectorAxis())
  {
    write(points.vectorCoord(pointI), os);
  }
  else
  {
    write(points.scalarCoord(pointI), os);
  }
}
template<class Type>
void mousse::writer<Type>::writeTable
(
  const coordSet& points,
  const List<Type>& values,
  Ostream& os
) const
{
  forAll(points, pointI)
  {
    writeCoord(points, pointI, os);
    writeSeparator(os);
    write(values[pointI], os);
    os << nl;
  }
}
template<class Type>
void mousse::writer<Type>::writeTable
(
  const coordSet& points,
  const List<const List<Type>*>& valuesPtrList,
  Ostream& os
) const
{
  forAll(points, pointI)
  {
    writeCoord(points, pointI, os);
    forAll(valuesPtrList, i)
    {
      writeSeparator(os);
      const List<Type>& values = *valuesPtrList[i];
      write(values[pointI], os);
    }
    os << nl;
  }
}
// Constructors 
template<class Type>
mousse::writer<Type>::writer()
{}
// Destructor 
template<class Type>
mousse::writer<Type>::~writer()
{}
// Member Functions 
template<class Type>
void mousse::writer<Type>::write
(
  const coordSet& points,
  const wordList& valueSetNames,
  const List<Field<Type> >& valueSets,
  Ostream& os
) const
{
  List<const Field<Type>*> valueSetPtrs(valueSets.size());
  forAll(valueSetPtrs, i)
  {
    valueSetPtrs[i] = &valueSets[i];
  }
  write(points, valueSetNames, valueSetPtrs, os);
}
template<class Type>
mousse::Ostream& mousse::writer<Type>::write
(
  const scalar value,
  Ostream& os
) const
{
  return os << value;
}
template<class Type>
template<class VSType>
mousse::Ostream& mousse::writer<Type>::writeVS
(
  const VSType& value,
  Ostream& os
) const
{
  for (direction d=0; d<VSType::nComponents; d++)
  {
    if (d > 0)
    {
      writeSeparator(os);
    }
    os << value.component(d);
  }
  return os;
}
template<class Type>
void mousse::writer<Type>::writeSeparator
(
  Ostream& os
) const
{
  os << token::SPACE << token::TAB;
}
template<class Type>
mousse::Ostream& mousse::writer<Type>::write
(
  const vector& value,
  Ostream& os
) const
{
  return writeVS(value, os);
}
template<class Type>
mousse::Ostream& mousse::writer<Type>::write
(
  const sphericalTensor& value,
  Ostream& os
) const
{
  return writeVS(value, os);
}
template<class Type>
mousse::Ostream& mousse::writer<Type>::write
(
  const symmTensor& value,
  Ostream& os
) const
{
  return writeVS(value, os);
}
template<class Type>
mousse::Ostream& mousse::writer<Type>::write
(
  const tensor& value,
  Ostream& os
) const
{
  return writeVS(value, os);
}
