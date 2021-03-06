// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "line_divide.hpp"
#include "curved_edge.hpp"


namespace mousse {

//- Calculate the geometric expension factor from the expansion ratio
inline scalar calcGexp(const scalar expRatio, const label nDiv)
{
  return nDiv > 1 ? pow(expRatio, 1.0/(nDiv - 1)) : 0.0;
}

}


// Constructors 
mousse::lineDivide::lineDivide
(
  const curvedEdge& cedge,
  const label nDiv,
  const gradingDescriptors& gd
)
:
  points_{nDiv + 1},
  divisions_{nDiv + 1}
{
  divisions_[0]    = 0.0;
  divisions_[nDiv] = 1.0;
  scalar secStart = divisions_[0];
  label secnStart = 1;
  // Check that there are more divisions than sections
  if (nDiv >= gd.size()) {
    // Calculate distribution of divisions to be independent
    // of the order of the sections
    labelList secnDivs{gd.size()};
    label sumSecnDivs = 0;
    label secnMaxDivs = 0;
    FOR_ALL(gd, sectioni) {
      scalar nDivFrac = gd[sectioni].nDivFraction();
      secnDivs[sectioni] = static_cast<label>(nDivFrac*nDiv + 0.5);
      sumSecnDivs += secnDivs[sectioni];
      // Find the section with the largest number of divisions
      if (nDivFrac > gd[secnMaxDivs].nDivFraction()) {
        secnMaxDivs = sectioni;
      }
    }
    // Adjust the number of divisions on the section with the largest
    // number so that the total is nDiv
    if (sumSecnDivs != nDiv) {
      secnDivs[secnMaxDivs] += (nDiv - sumSecnDivs);
    }
    FOR_ALL(gd, sectioni) {
      scalar blockFrac = gd[sectioni].blockFraction();
      scalar expRatio = gd[sectioni].expansionRatio();
      label secnDiv = secnDivs[sectioni];
      label secnEnd = secnStart + secnDiv;
      // Calculate the spacing
      if (expRatio == 1.0) {
        for (label i = secnStart; i < secnEnd; i++) {
          divisions_[i] =
            secStart + blockFrac*scalar(i - secnStart + 1)/secnDiv;
        }
      } else {
        // Calculate geometric expansion factor from the expansion ratio
        const scalar expFact = calcGexp(expRatio, secnDiv);
        for (label i = secnStart; i < secnEnd; i++) {
          divisions_[i] =
            secStart + blockFrac*(1.0 - pow(expFact, i - secnStart + 1))
            /(1.0 - pow(expFact, secnDiv));
        }
      }
      secStart = divisions_[secnEnd - 1];
      secnStart = secnEnd;
    }
  } else {
    // Otherwise mesh uniformly
    for (label i=1; i < nDiv; i++) {
      divisions_[i] = scalar(i)/nDiv;
    }
  }
  // Calculate the points
  for (label i = 0; i <= nDiv; i++) {
    points_[i] = cedge.position(divisions_[i]);
  }
}


// Member Functions 
const mousse::pointField& mousse::lineDivide::points() const
{
  return points_;
}


const mousse::scalarList& mousse::lineDivide::lambdaDivisions() const
{
  return divisions_;
}

