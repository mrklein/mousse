// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xmgrace_set_writer.hpp"
#include "coord_set.hpp"
#include "file_name.hpp"
#include "ofstream.hpp"
// Constructors 
template<class Type>
mousse::xmgraceSetWriter<Type>::xmgraceSetWriter()
:
  writer<Type>()
{}
// Destructor 
template<class Type>
mousse::xmgraceSetWriter<Type>::~xmgraceSetWriter()
{}
// Member Functions 
template<class Type>
mousse::fileName mousse::xmgraceSetWriter<Type>::getFileName
(
  const coordSet& points,
  const wordList& valueSetNames
) const
{
  return this->getBaseName(points, valueSetNames) + ".agr";
}
template<class Type>
void mousse::xmgraceSetWriter<Type>::write
(
  const coordSet& points,
  const wordList& valueSetNames,
  const List<const Field<Type>*>& valueSets,
  Ostream& os
) const
{
  os  << "@g0 on" << nl
    << "@with g0" << nl
    << "@    title \"" << points.name() << '"' << nl
    << "@    xaxis label " << '"' << points.axis() << '"' << nl;
  forAll(valueSets, i)
  {
    os  << "@    s" << i << " legend " << '"'
      << valueSetNames[i] << '"' << nl
      << "@target G0.S" << i << nl;
    this->writeTable(points, *valueSets[i], os);
    os  << '&' << nl;
  }
}
template<class Type>
void mousse::xmgraceSetWriter<Type>::write
(
  const bool writeTracks,
  const PtrList<coordSet>& trackPoints,
  const wordList& valueSetNames,
  const List<List<Field<Type> > >& valueSets,
  Ostream& os
) const
{
  if (valueSets.size() != valueSetNames.size())
  {
    FatalErrorIn("gnuplotSetWriter<Type>::write(..)")
      << "Number of variables:" << valueSetNames.size() << endl
      << "Number of valueSets:" << valueSets.size()
      << exit(FatalError);
  }
  if (trackPoints.size() > 0)
  {
    os  << "@g0 on" << nl
      << "@with g0" << nl
      << "@    title \"" << trackPoints[0].name() << '"' << nl
      << "@    xaxis label " << '"' << trackPoints[0].axis() << '"' << nl;
    // Data index.
    label sI = 0;
    forAll(trackPoints, trackI)
    {
      forAll(valueSets, i)
      {
        os  << "@    s" << sI << " legend " << '"'
          << valueSetNames[i] << "_track" << i << '"' << nl
          << "@target G0.S" << sI << nl;
        this->writeTable(trackPoints[trackI], valueSets[i][trackI], os);
        os  << '&' << nl;
        sI++;
      }
    }
  }
}
