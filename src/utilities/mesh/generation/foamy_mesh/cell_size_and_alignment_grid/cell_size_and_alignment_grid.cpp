// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cgal_triangulation_3d_kernel.hpp"
#include "indexed_vertex.hpp"
#include "indexed_cell.hpp"
#include "arg_list.hpp"
#include "time.hpp"
#include "distributed_delaunay_mesh.hpp"
#include "background_mesh_decomposition.hpp"
#include "searchable_surfaces.hpp"
#include "conformation_surfaces.hpp"
#include "print_table.hpp"
#include "random.hpp"
#include "bound_box.hpp"
#include "point.hpp"
#include "cell_shape_control_mesh.hpp"
#include "triad_field.hpp"
#include "scalar_io_field.hpp"
#include "point_io_field.hpp"
#include "triad_io_field.hpp"


using namespace mousse;


// Main program:
template<class Triangulation, class Type>
mousse::tmp<mousse::Field<Type>> filterFarPoints
(
  const Triangulation& mesh,
  const Field<Type>& field
)
{
  tmp<Field<Type>> tNewField{new Field<Type>{field.size()}};
  Field<Type>& newField = tNewField();
  label added = 0;
  label count = 0;
  for (auto vit = mesh.finite_vertices_begin();
       vit != mesh.finite_vertices_end();
       ++vit) {
    if (vit->real()) {
      newField[added++] = field[count];
    }
    count++;
  }
  newField.resize(added);
  return tNewField;
}


template<class T>
autoPtr<mapDistribute> buildMap
(
  const T& mesh,
  labelListList& pointPoints
)
{
  pointPoints.setSize(mesh.vertexCount());
  globalIndex globalIndexing{mesh.vertexCount()};
  for (auto vit = mesh.finite_vertices_begin();
       vit != mesh.finite_vertices_end();
       ++vit) {
    if (!vit->real()) {
      continue;
    }
    std::list<typename T::Vertex_handle> adjVerts;
    mesh.finite_adjacent_vertices(vit, std::back_inserter(adjVerts));
    DynamicList<label> indices{static_cast<label>(adjVerts.size())};
    for (auto adjVertI = adjVerts.begin();
         adjVertI != adjVerts.end();
         ++adjVertI) {
      typename T::Vertex_handle vh = *adjVertI;
      if (!vh->farPoint()) {
        indices.append
        (
          globalIndexing.toGlobal(vh->procIndex(), vh->index())
        );
      }
    }
    pointPoints[vit->index()].transfer(indices);
  }
  List<Map<label>> compactMap;
  return
    autoPtr<mapDistribute>
    {
      new mapDistribute
      {
        globalIndexing,
        pointPoints,
        compactMap
      }
    };
}


template<class T>
mousse::tmp<mousse::triadField> buildAlignmentField(const T& mesh)
{
  tmp<triadField> tAlignments
  {
    new triadField{mesh.vertexCount(), triad::unset}
  };
  triadField& alignments = tAlignments();
  for (auto vit = mesh.finite_vertices_begin();
       vit != mesh.finite_vertices_end();
       ++vit) {
    if (!vit->real()) {
      continue;
    }
    alignments[vit->index()] = vit->alignment();
  }
  return tAlignments;
}


template<class T>
mousse::tmp<mousse::pointField> buildPointField(const T& mesh)
{
  tmp<pointField> tPoints
  {
    new pointField{mesh.vertexCount(), point{GREAT, GREAT, GREAT}}
  };
  pointField& points = tPoints();
  for (auto vit = mesh.finite_vertices_begin();
       vit != mesh.finite_vertices_end();
       ++vit) {
    if (!vit->real()) {
      continue;
    }
    points[vit->index()] = topoint(vit->point());
  }
  return tPoints;
}


void refine
(
  cellShapeControlMesh& mesh,
  const conformationSurfaces& geometryToConformTo,
  const label maxRefinementIterations,
  const scalar defaultCellSize
)
{
  for (label iter = 0; iter < maxRefinementIterations; ++iter) {
    DynamicList<point> ptsToInsert;
    for (auto cit = mesh.finite_cells_begin();
         cit != mesh.finite_cells_end();
         ++cit) {
      const point newPoint =
        topoint
        (
          CGAL::centroid
          (
            cit->vertex(0)->point(),
            cit->vertex(1)->point(),
            cit->vertex(2)->point(),
            cit->vertex(3)->point()
          )
        );
      if (geometryToConformTo.inside(newPoint)) {
        ptsToInsert.append(newPoint);
      }
    }
    Info << "    Adding " << returnReduce(ptsToInsert.size(), sumOp<label>())
      << endl;
    FOR_ALL(ptsToInsert, ptI) {
      mesh.insert
      (
        ptsToInsert[ptI],
        defaultCellSize,
        triad::unset,
        Vb::vtInternal
      );
    }
  }
}


int main(int argc, char *argv[])
{
  #include "set_root_case.inc"
  #include "create_time.inc"
  label maxRefinementIterations = 2;
  label maxSmoothingIterations = 200;
  scalar minResidual = 0;
  scalar defaultCellSize = 0.001;
  scalar nearFeatDistSqrCoeff = 1e-8;
  // Need to decouple vertex and cell type from this class?
  // Vertex must have:
  // + index
  // + procIndex
  // - type should be optional
  cellShapeControlMesh mesh{runTime};
  IOdictionary foamyHexMeshDict
  {
    {
      "foamyHexMeshDict",
      runTime.system(),
      runTime,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  };
  Random rndGen{64293*Pstream::myProcNo()};
  searchableSurfaces allGeometry
  {
    {
      "cvSearchableSurfaces",
      runTime.constant(),
      "triSurface",
      runTime,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    },
    foamyHexMeshDict.subDict("geometry"),
    foamyHexMeshDict.lookupOrDefault("singleRegionName", true)
  };
  conformationSurfaces geometryToConformTo
  {
    runTime,
    // rndGen,
    allGeometry,
    foamyHexMeshDict.subDict("surfaceConformation")
  };
  autoPtr<backgroundMeshDecomposition> bMesh;
  if (Pstream::parRun()) {
    bMesh.set
    (
      new backgroundMeshDecomposition
      {
        runTime,
        rndGen,
        geometryToConformTo,
        foamyHexMeshDict.subDict("backgroundMeshDecomposition")
      }
    );
  }
  // Nice to have IO for the delaunay mesh
  // IO depend on vertex type.
  //
  // Define a delaunay mesh as:
  // + list of points of the triangulation
  // + optionally a list of cells
  Info << nl << "Loop over surfaces" << endl;
  FOR_ALL(geometryToConformTo.surfaces(), sI) {
    const label surfI = geometryToConformTo.surfaces()[sI];
    const auto& surface = geometryToConformTo.geometry()[surfI];
    Info << nl << "Inserting points from surface " << surface.name()
      << " (" << surface.type() << ")" << endl;
    const tmp<pointField> tpoints{surface.points()};
    const auto& points = tpoints();
    Info << "    Number of points = " << points.size() << endl;
    FOR_ALL(points, pI) {
      // Is the point in the extendedFeatureEdgeMesh? If so get the
      // point normal, otherwise get the surface normal from
      // searchableSurface
      pointIndexHit info;
      label infoFeature;
      geometryToConformTo.findFeaturePointNearest
      (
        points[pI],
        nearFeatDistSqrCoeff,
        info,
        infoFeature
      );
      autoPtr<triad> pointAlignment;
      if (info.hit()) {
        const extendedFeatureEdgeMesh& features =
          geometryToConformTo.features()[infoFeature];
        vectorField norms = features.featurePointNormals(info.index());
        // Create a triad from these norms.
        pointAlignment.set(new triad{});
        FOR_ALL(norms, nI) {
          pointAlignment() += norms[nI];
        }
        pointAlignment().normalize();
        pointAlignment().orthogonalize();
      } else {
        geometryToConformTo.findEdgeNearest
        (
          points[pI],
          nearFeatDistSqrCoeff,
          info,
          infoFeature
        );
        if (info.hit()) {
          const auto& features = geometryToConformTo.features()[infoFeature];
          vectorField norms = features.edgeNormals(info.index());
          // Create a triad from these norms.
          pointAlignment.set(new triad{});
          FOR_ALL(norms, nI) {
            pointAlignment() += norms[nI];
          }
          pointAlignment().normalize();
          pointAlignment().orthogonalize();
        } else {
          pointField ptField{1, points[pI]};
          scalarField distField{1, nearFeatDistSqrCoeff};
          List<pointIndexHit> infoList{1, pointIndexHit()};
          surface.findNearest(ptField, distField, infoList);
          vectorField normals{1};
          surface.getNormal(infoList, normals);
          pointAlignment.set(new triad{normals[0]});
        }
      }
      if (Pstream::parRun()) {
        if (bMesh().positionOnThisProcessor(points[pI])) {
          mesh.insert
          (
            points[pI],
            defaultCellSize,
            pointAlignment(),
            Vb::vtInternalNearBoundary
          );
        }
      } else {
        mesh.insert
        (
          points[pI],
          defaultCellSize,
          pointAlignment(),
          Vb::vtInternalNearBoundary
        );
      }
    }
  }
  // Refine the mesh
  refine
  (
    mesh,
    geometryToConformTo,
    maxRefinementIterations,
    defaultCellSize
  );
  if (Pstream::parRun()) {
    mesh.distribute(bMesh);
  }
  labelListList pointPoints;
  autoPtr<mapDistribute> meshDistributor = buildMap(mesh, pointPoints);
  triadField alignments{buildAlignmentField(mesh)};
  pointField points{buildPointField(mesh)};
  mesh.printInfo(Info);
  // Setup the sizes and alignments on each point
  triadField fixedAlignments(mesh.vertexCount(), triad::unset);
  for (auto vit = mesh.finite_vertices_begin();
       vit != mesh.finite_vertices_end();
       ++vit) {
    if (vit->nearBoundary()) {
      fixedAlignments[vit->index()] = vit->alignment();
    }
  }
  Info << nl << "Smoothing alignments" << endl;
  for (label iter = 0; iter < maxSmoothingIterations; iter++) {
    Info << "Iteration " << iter;
    meshDistributor().distribute(points);
    meshDistributor().distribute(alignments);
    scalar residual = 0;
    triadField triadAv{alignments.size(), triad::unset};
    FOR_ALL(pointPoints, pI) {
      const auto& pPoints = pointPoints[pI];
      if (pPoints.empty()) {
        continue;
      }
      const triad& oldTriad = alignments[pI];
      triad& newTriad = triadAv[pI];
      // Enforce the boundary conditions
      const triad& fixedAlignment = fixedAlignments[pI];
      FOR_ALL(pPoints, adjPointI) {
        const label adjPointIndex = pPoints[adjPointI];
        scalar dist = mag(points[pI] - points[adjPointIndex]);
        triad tmpTriad = alignments[adjPointIndex];
        for (direction dir = 0; dir < 3; dir++) {
          if (tmpTriad.set(dir)) {
            tmpTriad[dir] *= (1.0/dist);
          }
        }
        newTriad += tmpTriad;
      }
      newTriad.normalize();
      newTriad.orthogonalize();
      label nFixed = 0;
      FOR_ALL(fixedAlignment, dirI) {
        if (fixedAlignment.set(dirI)) {
          nFixed++;
        }
      }
      if (nFixed == 1) {
        FOR_ALL(fixedAlignment, dirI) {
          if (fixedAlignment.set(dirI)) {
            newTriad.align(fixedAlignment[dirI]);
          }
        }
      } else if (nFixed == 2) {
        FOR_ALL(fixedAlignment, dirI) {
          if (fixedAlignment.set(dirI)) {
            newTriad[dirI] = fixedAlignment[dirI];
          } else {
            newTriad[dirI] = triad::unset[dirI];
          }
        }
        newTriad.orthogonalize();
      } else if (nFixed == 3) {
        FOR_ALL(fixedAlignment, dirI) {
          if (fixedAlignment.set(dirI)) {
            newTriad[dirI] = fixedAlignment[dirI];
          }
        }
      }
      for (direction dir = 0; dir < 3; ++dir) {
        if (newTriad.set(dir) && oldTriad.set(dir)) {
          scalar dotProd = (oldTriad[dir] & newTriad[dir]);
          scalar diff = mag(dotProd) - 1.0;
          residual += mag(diff);
        }
      }
    }
    FOR_ALL(alignments, pI) {
      alignments[pI] = triadAv[pI].sortxyz();
    }
    reduce(residual, sumOp<scalar>());
    Info << ", Residual = " << residual << endl;
    if (residual <= minResidual) {
      break;
    }
  }
  // Write alignments to a .obj file
  OFstream str{runTime.path()/"alignments.obj"};
  FOR_ALL(alignments, pI) {
    const triad& tri = alignments[pI];
    if (tri.set())
      continue;
    FOR_ALL(tri, dirI) {
      meshTools::writeOBJ(str, points[pI], tri[dirI] + points[pI]);
    }
  }
  // Remove the far points
  pointIOField pointsIO
  {
    {
      "points",
      runTime.constant(),
      runTime,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    filterFarPoints(mesh, points)
  };
  scalarField sizes{points.size(), defaultCellSize};
  scalarIOField sizesIO
  {
    {
      "sizes",
      runTime.constant(),
      runTime,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    filterFarPoints(mesh, sizes)
  };
  triadIOField alignmentsIO
  {
    {
      "alignments",
      runTime.constant(),
      runTime,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    filterFarPoints(mesh, alignments)
  };
  pointsIO.write();
  sizesIO.write();
  alignmentsIO.write();
  Info << nl << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
    << "  ClockTime = " << runTime.elapsedClockTime() << " s"
    << nl << endl;
  Info << "\nEnd\n" << endl;
  return 0;
}

