// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "automatic.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tri_surface_mesh.hpp"
#include "vtk_surface_writer.hpp"
#include "primitive_patch_interpolation.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(automatic, 0);
  addToRunTimeSelectionTable(cellSizeCalculationType, automatic, dictionary);
}
// Private Member Functions 
void mousse::automatic::smoothField(triSurfaceScalarField& surf)
{
  label nSmoothingIterations = 10;
  for (label iter = 0; iter < nSmoothingIterations; ++iter)
  {
    const pointField& faceCentres = surface_.faceCentres();
    forAll(surf, sI)
    {
      const labelList& faceFaces = surface_.faceFaces()[sI];
      const point& fC = faceCentres[sI];
      const scalar value = surf[sI];
      scalar newValue = 0;
      scalar totalDist = 0;
      label nFaces = 0;
      forAll(faceFaces, fI)
      {
        const label faceLabel = faceFaces[fI];
        const point& faceCentre = faceCentres[faceLabel];
        const scalar faceValue = surf[faceLabel];
        const scalar distance = mag(faceCentre - fC);
        newValue += faceValue/(distance + SMALL);
        totalDist += 1.0/(distance + SMALL);
        if (value < faceValue)
        {
          nFaces++;
        }
      }
      // Do not smooth out the peak values
      if (nFaces == faceFaces.size())
      {
        continue;
      }
      surf[sI] = newValue/totalDist;
    }
  }
}
// Constructors 
mousse::automatic::automatic
(
  const dictionary& cellSizeCalcTypeDict,
  const triSurfaceMesh& surface,
  const scalar& defaultCellSize
)
:
  cellSizeCalculationType
  (
    typeName,
    cellSizeCalcTypeDict,
    surface,
    defaultCellSize
  ),
  coeffsDict_(cellSizeCalcTypeDict.subDict(typeName + "Coeffs")),
  surfaceName_(surface.searchableSurface::name()),
  readCurvature_(Switch(coeffsDict_.lookup("curvature"))),
  curvatureFile_(coeffsDict_.lookup("curvatureFile")),
  readFeatureProximity_(Switch(coeffsDict_.lookup("featureProximity"))),
  featureProximityFile_(coeffsDict_.lookup("featureProximityFile")),
  readInternalCloseness_(Switch(coeffsDict_.lookup("internalCloseness"))),
  internalClosenessFile_(coeffsDict_.lookup("internalClosenessFile")),
  curvatureCellSizeCoeff_
  (
    readScalar(coeffsDict_.lookup("curvatureCellSizeCoeff"))
  ),
  maximumCellSize_
  (
    readScalar(coeffsDict_.lookup("maximumCellSizeCoeff"))*defaultCellSize
  )
{}
// Member Functions 
mousse::tmp<mousse::triSurfacePointScalarField> mousse::automatic::load()
{
  Info<< indent
    << "Calculating cell size on surface: " << surfaceName_ << endl;
  tmp<triSurfacePointScalarField> tPointCellSize
  (
    new triSurfacePointScalarField
    (
      IOobject
      (
        surfaceName_ + ".cellSize",
        surface_.searchableSurface::time().constant(),
        "triSurface",
        surface_.searchableSurface::time(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      surface_,
      dimLength,
      scalarField(surface_.nPoints(), maximumCellSize_)
    )
  );
  triSurfacePointScalarField& pointCellSize = tPointCellSize();
  if (readCurvature_)
  {
    Info<< indent
      << "Reading curvature         : " << curvatureFile_ << endl;
    triSurfacePointScalarField curvature
    (
      IOobject
      (
        curvatureFile_,
        surface_.searchableSurface::time().constant(),
        "triSurface",
        surface_.searchableSurface::time(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      surface_,
      dimLength,
      true
    );
    forAll(pointCellSize, pI)
    {
      pointCellSize[pI] =
        min
        (
          1.0
         /max
          (
            (1.0/curvatureCellSizeCoeff_)*mag(curvature[pI]),
            1.0/maximumCellSize_
          ),
          pointCellSize[pI]
        );
    }
  }
  PrimitivePatchInterpolation
  <
    PrimitivePatch<labelledTri, ::mousse::List, pointField, point>
  > patchInterpolate(surface_);
  const Map<label>& meshPointMap = surface_.meshPointMap();
  if (readInternalCloseness_)
  {
    Info<< indent
      << "Reading internal closeness: " << internalClosenessFile_ << endl;
    triSurfaceScalarField internalCloseness
    (
      IOobject
      (
        internalClosenessFile_,
        surface_.searchableSurface::time().constant(),
        "triSurface",
        surface_.searchableSurface::time(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      surface_,
      dimLength,
      true
    );
    scalarField internalClosenessPointField
    (
      patchInterpolate.faceToPointInterpolate(internalCloseness)
    );
    forAll(pointCellSize, pI)
    {
      pointCellSize[pI] =
        min
        (
          internalClosenessPointField[meshPointMap[pI]],
          pointCellSize[pI]
        );
    }
  }
  if (readFeatureProximity_)
  {
    Info<< indent
      << "Reading feature proximity : " << featureProximityFile_ << endl;
    triSurfaceScalarField featureProximity
    (
      IOobject
      (
        featureProximityFile_,
        surface_.searchableSurface::time().constant(),
        "triSurface",
        surface_.searchableSurface::time(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      surface_,
      dimLength,
      true
    );
    scalarField featureProximityPointField
    (
      patchInterpolate.faceToPointInterpolate(featureProximity)
    );
    forAll(pointCellSize, pI)
    {
      pointCellSize[pI] =
        min
        (
          featureProximityPointField[meshPointMap[pI]],
          pointCellSize[pI]
        );
    }
  }
  //smoothField(surfaceCellSize);
  pointCellSize.write();
  if (debug)
  {
    faceList faces(surface_.size());
    forAll(surface_, fI)
    {
      faces[fI] = surface_.triSurface::operator[](fI).triFaceFace();
    }
    vtkSurfaceWriter().write
    (
      surface_.searchableSurface::time().constant()/"triSurface",
      surfaceName_.lessExt().name(),
      surface_.points(),
      faces,
      "cellSize",
      pointCellSize,
      true,
      true
    );
  }
  return tPointCellSize;
}
