// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_set_writer.hpp"

#include "coord_set.hpp"
#include "file_name.hpp"
#include "ofstream.hpp"
#include "add_to_run_time_selection_table.hpp"

// Constructors 
template<class Type>
mousse::vtkSetWriter<Type>::vtkSetWriter()
:
  writer<Type>()
{}


// Destructor 
template<class Type>
mousse::vtkSetWriter<Type>::~vtkSetWriter()
{}


// Member Functions 
template<class Type>
mousse::fileName mousse::vtkSetWriter<Type>::getFileName
(
  const coordSet& points,
  const wordList& valueSetNames
) const
{
  return this->getBaseName(points, valueSetNames) + ".vtk";
}


template<class Type>
void mousse::vtkSetWriter<Type>::write
(
  const coordSet& points,
  const wordList& valueSetNames,
  const List<const Field<Type>*>& valueSets,
  Ostream& os
) const
{
  os<< "# vtk DataFile Version 2.0" << nl
    << points.name() << nl
    << "ASCII" << nl
    << "DATASET POLYDATA" << nl
    << "POINTS " << points.size() << " float" << nl;
  FOR_ALL(points, i)
  {
    const vector& pt = points[i];
    os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
  }
  os<< "POINT_DATA " << points.size() << nl
    << " FIELD attributes " << valueSetNames.size() << nl;
  FOR_ALL(valueSetNames, setI)
  {
    os<< valueSetNames[setI] << ' ' << pTraits<Type>::nComponents << ' '
      << points.size() << " float" << nl;
    const Field<Type>& fld = *valueSets[setI];
    FOR_ALL(fld, pointI)
    {
      if (pointI != 0)
      {
        os  << ' ';
      }
      writer<Type>::write(fld[pointI], os);
    }
    os  << nl;
  }
}


template<class Type>
void mousse::vtkSetWriter<Type>::write
(
  const bool writeTracks,
  const PtrList<coordSet>& tracks,
  const wordList& valueSetNames,
  const List<List<Field<Type>>>& valueSets,
  Ostream& os
) const
{
  if (valueSets.size() != valueSetNames.size())
  {
    FATAL_ERROR_IN("vtkSetWriter<Type>::write(..)")
      << "Number of variables:" << valueSetNames.size() << endl
      << "Number of valueSets:" << valueSets.size()
      << exit(FatalError);
  }
  label nTracks = tracks.size();
  label nPoints = 0;
  FOR_ALL(tracks, i)
  {
    nPoints += tracks[i].size();
  }
  os<< "# vtk DataFile Version 2.0" << nl
    << tracks[0].name() << nl
    << "ASCII" << nl
    << "DATASET POLYDATA" << nl
    << "POINTS " << nPoints << " float" << nl;
  FOR_ALL(tracks, trackI)
  {
    const coordSet& points = tracks[trackI];
    FOR_ALL(points, i)
    {
      const vector& pt = points[i];
      os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
    }
  }
  if (writeTracks)
  {
    os << "LINES " << nTracks << ' ' << nPoints+nTracks << nl;
    // Write ids of track points to file
    label globalPtI = 0;
    FOR_ALL(tracks, trackI)
    {
      const coordSet& points = tracks[trackI];
      os << points.size();
      FOR_ALL(points, i)
      {
        os << ' ' << globalPtI;
        globalPtI++;
      }
      os << nl;
    }
  }
  os << "POINT_DATA " << nPoints << nl << " FIELD attributes "
    << valueSetNames.size() << nl;
  FOR_ALL(valueSetNames, setI)
  {
    os << valueSetNames[setI] << ' ' << pTraits<Type>::nComponents << ' '
      << nPoints << " float" << nl;
    const List<Field<Type>>& fieldVals = valueSets[setI];
    FOR_ALL(fieldVals, i)
    {
      const Field<Type>& vals = fieldVals[i];
      FOR_ALL(vals, j)
      {
        if (j != 0)
        {
          os  << ' ';
        }
        writer<Type>::write(vals[j], os);
      }
      os  << nl;
    }
  }
}
