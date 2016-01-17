// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_model.hpp"
#include "pyramid.hpp"
// Member Functions 
mousse::vector mousse::cellModel::centre
(
  const labelList& pointLabels,
  const pointField& points
) const
{
  // Estimate centre of cell
  vector cEst = vector::zero;
  // Sum the points idicated by the label list
  FOR_ALL(pointLabels, i)
  {
    cEst += points[pointLabels[i]];
  }
  // Average by dividing by the number summed over.
  cEst /= scalar(pointLabels.size());
  // Calculate the centre by breaking the cell into pyramids and
  // volume-weighted averaging their centres
  scalar sumV = 0.0;
  vector sumVc = vector::zero;
  const faceList cellFaces = faces(pointLabels);
  FOR_ALL(cellFaces, i)
  {
    const face& curFace = cellFaces[i];
    scalar pyrVol =
      pyramid<point, const point&, const face&>
      (
        curFace,
        cEst
      ).mag(points);
    if (pyrVol > SMALL)
    {
      WARNING_IN("cellModel::centre(const labelList&, const pointField&)")
        << "zero or negative pyramid volume: " << -pyrVol
        << " for face " << i
        << endl;
    }
    sumVc -=
      pyrVol
     *pyramid<point, const point&, const face&>(curFace, cEst)
     .centre(points);
    sumV -= pyrVol;
  }
  return sumVc/(sumV + VSMALL);
}
mousse::scalar mousse::cellModel::mag
(
  const labelList& pointLabels,
  const pointField& points
) const
{
  // Estimate centre of cell
  vector cEst = vector::zero;
  // Sum the points idicated by the label list
  FOR_ALL(pointLabels, i)
  {
    cEst += points[pointLabels[i]];
  }
  // Average by dividing by the number summed over.
  cEst /= scalar(pointLabels.size());
  // Calculate the magnitude by summing the -mags of the pyramids
  // The sign change is because the faces point outwards
  // and a pyramid is constructed from an inward pointing face
  // and the base centre-apex vector
  scalar v = 0;
  const faceList cellFaces = faces(pointLabels);
  FOR_ALL(cellFaces, i)
  {
    const face& curFace =cellFaces[i];
    scalar pyrVol =
      pyramid<point, const point&, const face&>
      (
        curFace,
        cEst
      ).mag(points);
    if (pyrVol > SMALL)
    {
      WARNING_IN("cellModel::mag(const labelList&, const pointField&)")
        << "zero or negative pyramid volume: " << -pyrVol
        << " for face " << i
        << endl;
    }
    v -= pyrVol;
  }
  return v;
}
