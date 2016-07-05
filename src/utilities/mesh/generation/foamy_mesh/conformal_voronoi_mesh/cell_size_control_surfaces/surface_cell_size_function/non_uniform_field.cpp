// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "non_uniform_field.hpp"
#include "tri_surface_mesh.hpp"
#include "searchable_surface.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(nonUniformField, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  surfaceCellSizeFunction,
  nonUniformField,
  dictionary
);

}


// Constructors 
mousse::nonUniformField::nonUniformField
(
  const dictionary& cellSizeFunctionDict,
  const searchableSurface& surface,
  const scalar& defaultCellSize
)
:
  surfaceCellSizeFunction
  {
    typeName,
    cellSizeFunctionDict,
    surface,
    defaultCellSize
  },
  surfaceTriMesh_{refCast<const triSurfaceMesh>(surface)},
  cellSizeCalculationType_
  {
    cellSizeCalculationType::New
    (
      coeffsDict(),
      surfaceTriMesh_,
      defaultCellSize
    )
  },
  pointCellSize_
  {
    IOobject
    {
      "pointCellSize.cellSize",
      surfaceTriMesh_.searchableSurface::time().constant(),
      "triSurface",
      surfaceTriMesh_.searchableSurface::time(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    surfaceTriMesh_,
    dimLength,
    false
  }
{
  Info << incrIndent;
  pointCellSize_ = cellSizeCalculationType_().load();
  Info << indent << "Cell size field statistics:" << nl
    << indent << "    Minimum: " << min(pointCellSize_).value() << nl
    << indent << "    Average: " << average(pointCellSize_).value() << nl
    << indent << "    Maximum: " << max(pointCellSize_).value() << endl;
  Info << decrIndent;
}


// Member Functions 
mousse::scalar mousse::nonUniformField::interpolate
(
  const point& pt,
  const label index
) const
{
  const face& faceHitByPt = surfaceTriMesh_.triSurface::operator[](index);
  const pointField& pts = surfaceTriMesh_.points();
  triPointRef tri
  {
    pts[faceHitByPt[0]],
    pts[faceHitByPt[1]],
    pts[faceHitByPt[2]]
  };
  scalarList bary(3, 0.0);
  tri.barycentric(pt, bary);
  return pointCellSize_[faceHitByPt[0]]*bary[0]
    + pointCellSize_[faceHitByPt[1]]*bary[1]
    + pointCellSize_[faceHitByPt[2]]*bary[2];
}

