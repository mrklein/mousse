// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolate_point_to_cell.hpp"

// Global Functions 
template<class Type>
Type mousse::interpolatePointToCell
(
  const GeometricField<Type, pointPatchField, pointMesh>& ptf,
  const label celli
)
{
  const primitiveMesh& mesh = ptf.mesh()();
  const cell& cFaces = mesh.cells()[celli];
  labelHashSet pointHad{10*cFaces.size()};
  Type sum{pTraits<Type>::zero};
  FOR_ALL(cFaces, i)
  {
    const face& f = mesh.faces()[cFaces[i]];
    FOR_ALL(f, fp)
    {
      label v = f[fp];
      if (pointHad.insert(v))
      {
        sum += ptf[v];
      }
    }
  }
  return sum/pointHad.size();
}
