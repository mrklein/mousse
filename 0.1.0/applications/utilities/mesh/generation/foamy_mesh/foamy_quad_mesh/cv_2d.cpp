// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cv_2d.hpp"
#include "random.hpp"
#include "transform.hpp"
#include "ifstream.hpp"
#include "uint.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(CV2D, 0);
}
// Private Member Functions 
void mousse::CV2D::insertBoundingBox()
{
  Info<< "insertBoundingBox: creating bounding mesh" << endl;
  scalar bigSpan = 10*meshControls().span();
  insertPoint(point2D(-bigSpan, -bigSpan), Vb::FAR_POINT);
  insertPoint(point2D(-bigSpan, bigSpan), Vb::FAR_POINT);
  insertPoint(point2D(bigSpan, -bigSpan), Vb::FAR_POINT);
  insertPoint(point2D(bigSpan, bigSpan), Vb::FAR_POINT);
}
void mousse::CV2D::fast_restore_Delaunay(Vertex_handle vh)
{
  int i;
  Face_handle f = vh->face(), next, start(f);
  do
  {
    i=f->index(vh);
    if (!is_infinite(f))
    {
      if (!internal_flip(f, cw(i))) external_flip(f, i);
      if (f->neighbor(i) == start) start = f;
    }
    f = f->neighbor(cw(i));
  } while (f != start);
}
void mousse::CV2D::external_flip(Face_handle& f, int i)
{
  Face_handle n = f->neighbor(i);
  if (CGAL::ON_POSITIVE_SIDE
      != side_of_oriented_circle(n, f->vertex(i)->point()))
  {
    return;
  }
  flip(f, i);
  i = n->index(f->vertex(i));
  external_flip(n, i);
}
bool mousse::CV2D::internal_flip(Face_handle& f, int i)
{
  Face_handle n = f->neighbor(i);
  if (CGAL::ON_POSITIVE_SIDE
      != side_of_oriented_circle(n, f->vertex(i)->point()))
  {
    return false;
  }
  flip(f, i);
  return true;
}
// Constructors 
mousse::CV2D::CV2D
(
  const Time& runTime,
  const dictionary& cvMeshDict
)
:
  Delaunay{},
  runTime_{runTime},
  rndGen_{64293*Pstream::myProcNo()},
  allGeometry_
  {
    {
      "cvSearchableSurfaces",
      runTime_.constant(),
      "triSurface",
      runTime_,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    cvMeshDict.subDict("geometry"),
    cvMeshDict.lookupOrDefault("singleRegionName", true)
  },
  qSurf_
  {
    runTime_,
    // rndGen_,
    allGeometry_,
    cvMeshDict.subDict("surfaceConformation")
  },
  controls_{cvMeshDict, qSurf_.globalBounds()},
  cellSizeControl_
  {
    runTime,
    cvMeshDict.subDict("motionControl").subDict("shapeControlFunctions"),
    qSurf_,
    controls_.minCellSize()
  },
  relaxationModel_
  {
    relaxationModel::New
    (
      cvMeshDict.subDict("motionControl"),
      runTime
    )
  },
  z_
  {
    point
    (
      cvMeshDict.subDict("surfaceConformation").lookup("locationInMesh")
    ).z()
  },
  startOfInternalPoints_{0},
  startOfSurfacePointPairs_{0},
  startOfBoundaryConformPointPairs_{0},
  featurePoints_{}
{
  Info<< meshControls() << endl;
  insertBoundingBox();
  insertFeaturePoints();
}
// Destructor 
mousse::CV2D::~CV2D()
{}
// Member Functions 
void mousse::CV2D::insertPoints
(
  const point2DField& points,
  const scalar nearness
)
{
  Info<< "insertInitialPoints(const point2DField& points): ";
  startOfInternalPoints_ = number_of_vertices();
  label nVert = startOfInternalPoints_;
  // Add the points and index them
  FOR_ALL(points, i)
  {
    const point2D& p = points[i];
    if (qSurf_.wellInside(toPoint3D(p), nearness))
    {
      insert(toPoint(p))->index() = nVert++;
    }
    else
    {
      Warning
        << "Rejecting point " << p << " outside surface" << endl;
    }
  }
  Info<< nVert << " vertices inserted" << endl;
  if (meshControls().objOutput())
  {
    // Checking validity of triangulation
    assert(is_valid());
    writeTriangles("initial_triangles.obj", true);
    writeFaces("initial_faces.obj", true);
  }
}
void mousse::CV2D::insertPoints(const fileName& pointFileName)
{
  IFstream pointsFile{pointFileName};
  if (pointsFile.good())
  {
    insertPoints
    (
      point2DField(pointsFile),
      0.5*meshControls().minCellSize2()
    );
  }
  else
  {
    FATAL_ERROR_IN("insertInitialPoints")
      << "Could not open pointsFile " << pointFileName
      << exit(FatalError);
  }
}
void mousse::CV2D::insertGrid()
{
  Info<< "insertInitialGrid: ";
  startOfInternalPoints_ = number_of_vertices();
  label nVert = startOfInternalPoints_;
  scalar x0 = qSurf_.globalBounds().min().x();
  scalar xR = qSurf_.globalBounds().max().x() - x0;
  int ni = int(xR/meshControls().minCellSize()) + 1;
  scalar deltax = xR/ni;
  scalar y0 = qSurf_.globalBounds().min().y();
  scalar yR = qSurf_.globalBounds().max().y() - y0;
  int nj = int(yR/meshControls().minCellSize()) + 1;
  scalar deltay = yR/nj;
  Random rndGen(1321);
  scalar pert = meshControls().randomPerturbation()*min(deltax, deltay);
  for (int i=0; i<ni; i++)
  {
    for (int j=0; j<nj; j++)
    {
      point p(x0 + i*deltax, y0 + j*deltay, 0);
      if (meshControls().randomiseInitialGrid())
      {
        p.x() += pert*(rndGen.scalar01() - 0.5);
        p.y() += pert*(rndGen.scalar01() - 0.5);
      }
      if (qSurf_.wellInside(p, 0.5*meshControls().minCellSize2()))
      {
        insert(Point(p.x(), p.y()))->index() = nVert++;
      }
    }
  }
  Info<< nVert << " vertices inserted" << endl;
  if (meshControls().objOutput())
  {
    // Checking validity of triangulation
    assert(is_valid());
    writeTriangles("initial_triangles.obj", true);
    writeFaces("initial_faces.obj", true);
  }
}
void mousse::CV2D::insertSurfacePointPairs()
{
  startOfSurfacePointPairs_ = number_of_vertices();
  if (meshControls().insertSurfaceNearestPointPairs())
  {
    insertSurfaceNearestPointPairs();
  }
  write("nearest");
  // Insertion of point-pairs for near-points may cause protrusions
  // so insertBoundaryConformPointPairs must be executed last
  if (meshControls().insertSurfaceNearPointPairs())
  {
    insertSurfaceNearPointPairs();
  }
  startOfBoundaryConformPointPairs_ = number_of_vertices();
}
void mousse::CV2D::boundaryConform()
{
  if (!meshControls().insertSurfaceNearestPointPairs())
  {
    markNearBoundaryPoints();
  }
  // Mark all the faces as SAVE_CHANGED
  for
  (
    Triangulation::Finite_faces_iterator fit = finite_faces_begin();
    fit != finite_faces_end();
    fit++
  )
  {
    fit->faceIndex() = Fb::SAVE_CHANGED;
  }
  for (label iter=1; iter<=meshControls().maxBoundaryConformingIter(); iter++)
  {
    label nIntersections = insertBoundaryConformPointPairs
    (
      "surfaceIntersections_" + mousse::name(iter) + ".obj"
    );
    if (nIntersections == 0)
    {
      break;
    }
    else
    {
      Info<< "BC iteration " << iter << ": "
        << nIntersections << " point-pairs inserted" << endl;
    }
    // Any faces changed by insertBoundaryConformPointPairs will now
    // be marked CHANGED, mark those as SAVE_CHANGED and those that
    // remained SAVE_CHANGED as UNCHANGED
    for (auto fit = finite_faces_begin(); fit != finite_faces_end(); fit++)
    {
      if (fit->faceIndex() == Fb::SAVE_CHANGED)
      {
        fit->faceIndex() = Fb::UNCHANGED;
      }
      else if (fit->faceIndex() == Fb::CHANGED)
      {
        fit->faceIndex() = Fb::SAVE_CHANGED;
      }
    }
  }
  Info<< nl;
  write("boundary");
}
void mousse::CV2D::removeSurfacePointPairs()
{
  for (auto vit = finite_vertices_begin(); vit != finite_vertices_end(); ++vit)
  {
    if (vit->index() >= startOfSurfacePointPairs_)
    {
      remove(vit);
    }
  }
}
void mousse::CV2D::newPoints()
{
  const scalar relaxation = relaxationModel_->relaxation();
  Info<< "Relaxation = " << relaxation << endl;
  Field<point2D> dualVertices{static_cast<label>(number_of_faces())};
  label dualVerti = 0;
  // Find the dual point of each tetrahedron and assign it an index.
  for (auto fit = finite_faces_begin(); fit != finite_faces_end(); ++fit)
  {
    fit->faceIndex() = -1;
    if (fit->vertex(0)->internalOrBoundaryPoint()
        || fit->vertex(1)->internalOrBoundaryPoint()
        || fit->vertex(2)->internalOrBoundaryPoint())
    {
      fit->faceIndex() = dualVerti;
      dualVertices[dualVerti] = toPoint2D(circumcenter(fit));
      dualVerti++;
    }
  }
  dualVertices.setSize(dualVerti);
  Field<vector2D> displacementAccumulator
  {
    startOfSurfacePointPairs_,
    vector2D::zero
  };
  // Calculate target size and alignment for vertices
  scalarField sizes
  {
    static_cast<label>(number_of_vertices()),
    meshControls().minCellSize()
  };
  Field<vector2D> alignments
  {
    static_cast<label>(number_of_vertices()),
    vector2D(1, 0)
  };
  for (auto vit = finite_vertices_begin(); vit != finite_vertices_end(); ++vit)
  {
    if (vit->internalOrBoundaryPoint())
    {
      point2D vert = toPoint2D(vit->point());
      // alignment and size determination
      pointIndexHit pHit;
      label hitSurface = -1;
      qSurf_.findSurfaceNearest
      (
        toPoint3D(vert),
        meshControls().span2(),
        pHit,
        hitSurface
      );
      if (pHit.hit())
      {
        vectorField norm{1};
        allGeometry_[hitSurface].getNormal
        (
          List<pointIndexHit>(1, pHit),
          norm
        );
        alignments[vit->index()] = toPoint2D(norm[0]);
        sizes[vit->index()] =
          cellSizeControl_.cellSize
          (
            toPoint3D(vit->point())
          );
      }
    }
  }
  // Info<< "Calculated alignments" << endl;
  scalar cosAlignmentAcceptanceAngle = 0.68;
  // Upper and lower edge length ratios for weight
  scalar u = 1.0;
  scalar l = 0.7;
  PackedBoolList pointToBeRetained{startOfSurfacePointPairs_, true};
  std::list<Point> pointsToInsert;
  for (auto eit = finite_edges_begin(); eit != finite_edges_end(); eit++)
  {
    Vertex_handle vA = eit->first->vertex(cw(eit->second));
    Vertex_handle vB = eit->first->vertex(ccw(eit->second));
    if (!vA->internalOrBoundaryPoint() || !vB->internalOrBoundaryPoint())
    {
      continue;
    }
    const point2D& dualV1 = dualVertices[eit->first->faceIndex()];
    const point2D& dualV2 =
      dualVertices[eit->first->neighbor(eit->second)->faceIndex()];
    scalar dualEdgeLength = mag(dualV1 - dualV2);
    point2D dVA = toPoint2D(vA->point());
    point2D dVB = toPoint2D(vB->point());
    Field<vector2D> alignmentDirsA{2};
    alignmentDirsA[0] = alignments[vA->index()];
    alignmentDirsA[1] = vector2D
    {
     -alignmentDirsA[0].y(),
      alignmentDirsA[0].x()
    };
    Field<vector2D> alignmentDirsB(2);
    alignmentDirsB[0] = alignments[vB->index()];
    alignmentDirsB[1] = vector2D
    {
     -alignmentDirsB[0].y(),
      alignmentDirsB[0].x()
    };
    Field<vector2D> alignmentDirs{alignmentDirsA};
    FOR_ALL(alignmentDirsA, aA)
    {
      const vector2D& a = alignmentDirsA[aA];
      scalar maxDotProduct = 0.0;
      FOR_ALL(alignmentDirsB, aB)
      {
        const vector2D& b = alignmentDirsB[aB];
        scalar dotProduct = a & b;
        if (mag(dotProduct) > maxDotProduct)
        {
          maxDotProduct = mag(dotProduct);
          alignmentDirs[aA] = a + sign(dotProduct)*b;
          alignmentDirs[aA] /= mag(alignmentDirs[aA]);
        }
      }
    }
    vector2D rAB = dVA - dVB;
    scalar rABMag = mag(rAB);
    FOR_ALL(alignmentDirs, aD)
    {
      vector2D& alignmentDir = alignmentDirs[aD];
      if ((rAB & alignmentDir) < 0)
      {
        // swap the direction of the alignment so that has the
        // same sense as rAB
        alignmentDir *= -1;
      }
      scalar alignmentDotProd = ((rAB/rABMag) & alignmentDir);
      if (alignmentDotProd > cosAlignmentAcceptanceAngle)
      {
        scalar targetFaceSize =
          0.5*(sizes[vA->index()] + sizes[vB->index()]);
        // Test for changing aspect ratio on second alignment (first
        // alignment is neartest surface normal)
        // if (aD == 1)
        // {
        //     targetFaceSize *= 2.0;
        // }
        alignmentDir *= 0.5*targetFaceSize;
        vector2D delta = alignmentDir - 0.5*rAB;
        if (dualEdgeLength < 0.7*targetFaceSize)
        {
          delta *= 0;
        }
        else if (dualEdgeLength < targetFaceSize)
        {
          delta *= (dualEdgeLength/(targetFaceSize*(u - l)) - 1/((u/l) - 1));
        }
        if (vA->internalPoint()
            && vB->internalPoint()
            && rABMag > 1.75*targetFaceSize
            && dualEdgeLength > 0.05*targetFaceSize
            && alignmentDotProd > 0.93)
        {
          // Point insertion
          pointsToInsert.push_back(toPoint(0.5*(dVA + dVB)));
        }
        else if
        (
          (vA->internalPoint() || vB->internalPoint())
          && rABMag < 0.65*targetFaceSize
        )
        {
          // Point removal
          // Only insert a point at the midpoint of the short edge
          // if neither attached point has already been identified
          // to be removed.
          if (pointToBeRetained[vA->index()] == true
              && pointToBeRetained[vB->index()] == true)
          {
            pointsToInsert.push_back(toPoint(0.5*(dVA + dVB)));
          }
          if (vA->internalPoint())
          {
            pointToBeRetained[vA->index()] = false;
          }
          if (vB->internalPoint())
          {
            pointToBeRetained[vB->index()] = false;
          }
        }
        else
        {
          if (vA->internalPoint())
          {
            displacementAccumulator[vA->index()] += delta;
          }
          if (vB->internalPoint())
          {
            displacementAccumulator[vB->index()] += -delta;
          }
        }
      }
    }
  }
  vector2D totalDisp = sum(displacementAccumulator);
  scalar totalDist = sum(mag(displacementAccumulator));
  // Relax the calculated displacement
  displacementAccumulator *= relaxation;
  label numberOfNewPoints = pointsToInsert.size();
  for (auto vit = finite_vertices_begin(); vit != finite_vertices_end(); ++vit)
  {
    if (vit->internalPoint())
    {
      if (pointToBeRetained[vit->index()])
      {
        pointsToInsert.push_front
        (
          toPoint
          (
            toPoint2D(vit->point()) + displacementAccumulator[vit->index()]
          )
        );
      }
    }
  }
  // Clear the triangulation and reinsert the bounding box and feature points.
  // This is faster than removing and moving points.
  this->clear();
  insertBoundingBox();
  reinsertFeaturePoints();
  startOfInternalPoints_ = number_of_vertices();
  label nVert = startOfInternalPoints_;
  Info<< "Inserting " << numberOfNewPoints << " new points" << endl;
  // Use the range insert as it is faster than individually inserting points.
  insert(pointsToInsert.begin(), pointsToInsert.end());
  for (auto vit = finite_vertices_begin(); vit != finite_vertices_end(); ++vit)
  {
    if (vit->type() == Vb::INTERNAL_POINT
        && vit->index() == Vb::INTERNAL_POINT)
    {
      vit->index() = nVert++;
    }
  }
  Info<< "    Total displacement = " << totalDisp << nl
    << "    Total distance = " << totalDist << nl
    << "    Points added = " << pointsToInsert.size()
    << endl;
  write("internal");
  insertSurfacePointPairs();
  boundaryConform();
}
void mousse::CV2D::write() const
{
  if (meshControls().objOutput())
  {
    writeFaces("allFaces.obj", false);
    writeFaces("faces.obj", true);
    writeTriangles("allTriangles.obj", false);
    writeTriangles("triangles.obj", true);
    writePatch("patch.pch");
  }
}
void mousse::CV2D::write(const word& stage) const
{
  if (meshControls().objOutput())
  {
    mousse::mkDir(stage + "Faces");
    mousse::mkDir(stage + "Triangles");
    writeFaces
    (
      stage + "Faces/allFaces_" + runTime_.timeName() + ".obj",
      false
    );
    writeTriangles
    (
      stage + "Triangles/allTriangles_" + runTime_.timeName() + ".obj",
      false
    );
  }
}
