// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gnuplot_set_writer.hpp"

#include "coord_set.hpp"
#include "file_name.hpp"
#include "ofstream.hpp"
#include "add_to_run_time_selection_table.hpp"

// Constructors 
template<class Type>
mousse::gnuplotSetWriter<Type>::gnuplotSetWriter()
:
  writer<Type>()
{}


// Destructor 
template<class Type>
mousse::gnuplotSetWriter<Type>::~gnuplotSetWriter()
{}


// Member Functions 
template<class Type>
mousse::fileName mousse::gnuplotSetWriter<Type>::getFileName
(
  const coordSet& points,
  const wordList& valueSetNames
) const
{
  return this->getBaseName(points, valueSetNames) + ".gplt";
}


template<class Type>
void mousse::gnuplotSetWriter<Type>::write
(
  const coordSet& points,
  const wordList& valueSetNames,
  const List<const Field<Type>*>& valueSets,
  Ostream& os
) const
{
  os<< "set term postscript color" << nl
    << "set output \"" << points.name() << ".ps\"" << nl
    << "plot";
  FOR_ALL(valueSets, i)
  {
    if (i != 0)
    {
      os << ',';
    }
    os << " \"-\" title \"" << valueSetNames[i] << "\" with lines";
  }
  os << nl;
  FOR_ALL(valueSets, i)
  {
    this->writeTable(points, *valueSets[i], os);
    os << "e" << nl;
  }
}


template<class Type>
void mousse::gnuplotSetWriter<Type>::write
(
  const bool /*writeTracks*/,
  const PtrList<coordSet>& trackPoints,
  const wordList& valueSetNames,
  const List<List<Field<Type> > >& valueSets,
  Ostream& os
) const
{
  if (valueSets.size() != valueSetNames.size())
  {
    FATAL_ERROR_IN("gnuplotSetWriter<Type>::write(..)")
      << "Number of variables:" << valueSetNames.size() << endl
      << "Number of valueSets:" << valueSets.size()
      << exit(FatalError);
  }
  if (trackPoints.size() > 0)
  {
    os<< "set term postscript color" << nl
      << "set output \"" << trackPoints[0].name() << ".ps\"" << nl;
    FOR_ALL(trackPoints, trackI)
    {
      os << "plot";
      FOR_ALL(valueSets, i)
      {
        if (i != 0)
        {
          os << ',';
        }
        os << " \"-\" title \"" << valueSetNames[i] << "\" with lines";
      }
      os << nl;
      FOR_ALL(valueSets, i)
      {
        this->writeTable(trackPoints[trackI], valueSets[i][trackI], os);
        os  << "e" << nl;
      }
    }
  }
}
