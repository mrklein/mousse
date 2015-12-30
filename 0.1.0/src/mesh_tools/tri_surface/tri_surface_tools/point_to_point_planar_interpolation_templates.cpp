// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_to_point_planar_interpolation.hpp"
// Member Functions 
template<class Type>
mousse::tmp<mousse::Field<Type> > mousse::pointToPointPlanarInterpolation::interpolate
(
  const Field<Type>& sourceFld
) const
{
  if (nPoints_ != sourceFld.size())
  {
    FatalErrorIn
    (
      "pointToPointPlanarInterpolation::interpolate"
      "(const Field<Type>&) const"
    )   << "Number of source points = " << nPoints_
      << " number of values = " << sourceFld.size()
      << exit(FatalError);
  }
  tmp<Field<Type> > tfld(new Field<Type>(nearestVertex_.size()));
  Field<Type>& fld = tfld();
  forAll(fld, i)
  {
    const FixedList<label, 3>& verts = nearestVertex_[i];
    const FixedList<scalar, 3>& w = nearestVertexWeight_[i];
    if (verts[2] == -1)
    {
      if (verts[1] == -1)
      {
        // Use vertex0 only
        fld[i] = sourceFld[verts[0]];
      }
      else
      {
        // Use vertex 0,1
        fld[i] =
          w[0]*sourceFld[verts[0]]
         + w[1]*sourceFld[verts[1]];
      }
    }
    else
    {
      fld[i] =
        w[0]*sourceFld[verts[0]]
       + w[1]*sourceFld[verts[1]]
       + w[2]*sourceFld[verts[2]];
    }
  }
  return tfld;
}
