// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "circle_set.hpp"
#include "sampled_set.hpp"
#include "mesh_search.hpp"
#include "dynamic_list.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "word.hpp"
#include "mathematical_constants.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(circleSet, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(sampledSet, circleSet, word);
}
// Private Member Functions 
void mousse::circleSet::calcSamples
(
  DynamicList<point>& samplingPts,
  DynamicList<label>& samplingCells,
  DynamicList<label>& samplingFaces,
  DynamicList<label>& samplingSegments,
  DynamicList<scalar>& samplingCurveDist
) const
{
  static const string funcName =
  (
    "void circleSet::calcSamples"
    "("
      "DynamicList<point>&, "
      "DynamicList<label>&, "
      "DynamicList<label>&, "
      "DynamicList<label>&, "
      "DynamicList<scalar>&"
    ") const"
  );
  // set start point
  label cellI = searchEngine().findCell(startPoint_);
  if (cellI != -1)
  {
    samplingPts.append(startPoint_);
    samplingCells.append(cellI);
    samplingFaces.append(-1);
    samplingSegments.append(0);
    samplingCurveDist.append(0.0);
  }
  else
  {
    WARNING_IN_FUNCTION
      << "Unable to find cell at point id " << 0
      << " at location " << startPoint_ << endl;
  }
  // add remaining points
  const scalar alpha = constant::mathematical::pi/180.0*dTheta_;
  const scalar sinAlpha = sin(alpha);
  const scalar cosAlpha = cos(alpha);
  // first axis
  vector axis1 = startPoint_ - origin_;
  const scalar radius = mag(axis1);
  if (mag(axis1 & circleAxis_) > SMALL)
  {
    WARNING_IN_FUNCTION
      << "Vector defined by (startPoint - origin) not orthogonal to "
      << "circleAxis:" << nl
      << "    startPoint - origin = " << axis1 << nl
      << "    circleAxis          = " << circleAxis_ << nl
      << endl;
  }
  axis1 /= mag(axis1);
  scalar theta = dTheta_;
  label nPoint = 1;
  while (theta < 360)
  {
    axis1 = axis1*cosAlpha + (axis1^circleAxis_)*sinAlpha;
    axis1 /= mag(axis1);
    point pt = origin_ + radius*axis1;
    label cellI = searchEngine().findCell(pt);
    if (cellI != -1)
    {
      samplingPts.append(pt);
      samplingCells.append(cellI);
      samplingFaces.append(-1);
      samplingSegments.append(nPoint);
      samplingCurveDist.append
      (
        radius*constant::mathematical::pi/180.0*theta
      );
      nPoint++;
    }
    else
    {
      WARNING_IN_FUNCTION
        << "Unable to find cell at point id " << nPoint
        << " at location " << pt << endl;
    }
    theta += dTheta_;
  }
}
void mousse::circleSet::genSamples()
{
  // Storage for sample points
  DynamicList<point> samplingPts;
  DynamicList<label> samplingCells;
  DynamicList<label> samplingFaces;
  DynamicList<label> samplingSegments;
  DynamicList<scalar> samplingCurveDist;
  calcSamples
  (
    samplingPts,
    samplingCells,
    samplingFaces,
    samplingSegments,
    samplingCurveDist
  );
  samplingPts.shrink();
  samplingCells.shrink();
  samplingFaces.shrink();
  samplingSegments.shrink();
  samplingCurveDist.shrink();
  setSamples
  (
    samplingPts,
    samplingCells,
    samplingFaces,
    samplingSegments,
    samplingCurveDist
  );
}
// Constructors 
mousse::circleSet::circleSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const word& axis,
  const point& origin,
  const vector& circleAxis,
  const point& startPoint,
  const scalar dTheta
)
:
  sampledSet{name, mesh, searchEngine, axis},
  origin_{origin},
  circleAxis_{circleAxis},
  startPoint_{startPoint},
  dTheta_{dTheta}
{
  genSamples();
  if (debug)
  {
    write(Info);
  }
}
mousse::circleSet::circleSet
(
  const word& name,
  const polyMesh& mesh,
  const meshSearch& searchEngine,
  const dictionary& dict
)
:
  sampledSet{name, mesh, searchEngine, dict},
  origin_{dict.lookup("origin")},
  circleAxis_{dict.lookup("circleAxis")},
  startPoint_{dict.lookup("startPoint")},
  dTheta_{readScalar(dict.lookup("dTheta"))}
{
  // normalise circleAxis
  circleAxis_ /= mag(circleAxis_);
  genSamples();
  if (debug)
  {
    write(Info);
  }
}
// Destructor 
mousse::circleSet::~circleSet()
{}
// Member Functions 
mousse::point mousse::circleSet::getRefPoint(const List<point>& /*pts*/) const
{
  return startPoint_;
}
