// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "sortable_list.hpp"
#include "pstream_reduce_ops.hpp"

bool mousse::primitiveMesh::checkPointNearness
(
  const bool report,
  const scalar reportDistSqr,
  labelHashSet* setPtr
) const
{
  const pointField& points = this->points();
  // Sort points
  SortableList<scalar> sortedMag(magSqr(points));
  label nClose = 0;
  for (label i = 1; i < sortedMag.size(); i++)
  {
    label pti = sortedMag.indices()[i];
    // Compare pti to any previous points with similar sortedMag
    for
    (
      label j = i-1;
      j >= 0 && (sortedMag[j] > sortedMag[i]-reportDistSqr);
      --j
    )
    {
      label prevPtI = sortedMag.indices()[j];
      if (magSqr(points[pti] - points[prevPtI]) < reportDistSqr)
      {
        nClose++;
        if (setPtr)
        {
          setPtr->insert(pti);
          setPtr->insert(prevPtI);
        }
      }
    }
  }
  reduce(nClose, sumOp<label>());
  if (nClose > 0)
  {
    if (report)
    {
      Info<< "  <<Points closer than " << mousse::sqrt(reportDistSqr)
        << " together found, number: " << nClose
        << endl;
    }
    return true;
  }
  else
  {
    return false;
  }
}
