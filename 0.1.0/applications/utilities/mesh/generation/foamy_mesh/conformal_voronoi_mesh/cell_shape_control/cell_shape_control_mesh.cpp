// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_shape_control_mesh.hpp"
#include "cell_size_and_alignment_controls.hpp"
#include "point_io_field.hpp"
#include "scalar_io_field.hpp"
#include "triad_io_field.hpp"
#include "tetrahedron.hpp"
#include "plane.hpp"
#include "transform.hpp"
#include "mesh_tools.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(cellShapeControlMesh, 0);
word cellShapeControlMesh::meshSubDir = "cellShapeControlMesh";
}
// Private Member Functions 
mousse::label mousse::cellShapeControlMesh::removePoints()
{
  label nRemoved = 0;
  for
  (
    CellSizeDelaunay::Finite_vertices_iterator vit =
      finite_vertices_begin();
    vit != finite_vertices_end();
    ++vit
  )
  {
    std::list<Vertex_handle> verts;
    adjacent_vertices(vit, std::back_inserter(verts));
    bool removePt = true;
    for
    (
      std::list<Vertex_handle>::iterator aVit = verts.begin();
      aVit != verts.end();
      ++aVit
    )
    {
      Vertex_handle avh = *aVit;
      scalar diff =
        mag(avh->targetCellSize() - vit->targetCellSize())
       /max(vit->targetCellSize(), 1e-6);
      if (diff > 0.05)
      {
        removePt = false;
      }
    }
    if (removePt)
    {
      remove(vit);
      nRemoved++;
    }
  }
  return nRemoved;
}

mousse::tmp<mousse::pointField> mousse::cellShapeControlMesh::cellCentres() const
{
  tmp<pointField> tcellCentres(new pointField(number_of_finite_cells()));
  pointField& cellCentres = tcellCentres();
  label count = 0;
  for
  (
    CellSizeDelaunay::Finite_cells_iterator c = finite_cells_begin();
    c != finite_cells_end();
    ++c
  )
  {
    if (c->hasFarPoint())
    {
      continue;
    }
    scalarList bary;
    cellShapeControlMesh::Cell_handle ch;
    const mousse::point centre = topoint
    (
      CGAL::centroid<baseK>
      (
        c->vertex(0)->point(),
        c->vertex(1)->point(),
        c->vertex(2)->point(),
        c->vertex(3)->point()
      )
    );
    cellCentres[count++] = centre;
  }
  cellCentres.resize(count);
  return tcellCentres;
}

void mousse::cellShapeControlMesh::writeTriangulation()
{
  OFstream str
  {
    "refinementTriangulation_" + name(Pstream::myProcNo()) + ".obj"
  };
  label count = 0;
  Info<< "Write refinementTriangulation" << endl;
  for (auto e = finite_edges_begin(); e != finite_edges_end(); ++e)
  {
    Cell_handle c = e->first;
    Vertex_handle vA = c->vertex(e->second);
    Vertex_handle vB = c->vertex(e->third);
    // Don't write far edges
    if (vA->farPoint() || vB->farPoint())
    {
      continue;
    }
    // Don't write unowned edges
    if (vA->referred() && vB->referred())
    {
      continue;
    }
    pointFromPoint p1 = topoint(vA->point());
    pointFromPoint p2 = topoint(vB->point());
    meshTools::writeOBJ(str, p1, p2, count);
  }
  if (is_valid())
  {
    Info<< "    Triangulation is valid" << endl;
  }
  else
  {
    FATAL_ERROR_IN
    (
      "mousse::triangulatedMesh::writeRefinementTriangulation()"
    )
    << "Triangulation is not valid"
    << abort(FatalError);
  }
}

// Constructors 
mousse::cellShapeControlMesh::cellShapeControlMesh(const Time& runTime)
:
  DistributedDelaunayMesh<CellSizeDelaunay>
  {
    runTime,
    meshSubDir
  },
  runTime_{runTime}
{
  if (this->vertexCount())
  {
    fvMesh mesh
    {
      {
        meshSubDir,
        runTime.timeName(),
        runTime,
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
      }
    };
    if (mesh.nPoints() == this->vertexCount())
    {
      pointScalarField sizes
      {
        {
          "sizes",
          runTime.timeName(),
          meshSubDir,
          runTime,
          IOobject::READ_IF_PRESENT,
          IOobject::NO_WRITE,
          false
        },
        pointMesh::New(mesh)
      };
      triadIOField alignments
      {
        {
          "alignments",
          mesh.time().timeName(),
          meshSubDir,
          mesh.time(),
          IOobject::READ_IF_PRESENT,
          IOobject::AUTO_WRITE,
          false
        }
      };
      if (sizes.size() == this->vertexCount()
          && alignments.size() == this->vertexCount())
      {
        for (auto vit = finite_vertices_begin();
             vit != finite_vertices_end();
             ++vit)
        {
          vit->targetCellSize() = sizes[vit->index()];
          vit->alignment() = alignments[vit->index()];
        }
      }
      else
      {
        FATAL_ERROR_IN
        (
          "mousse::cellShapeControlMesh::cellShapeControlMesh"
          "(const Time&)"
        )
        << "Cell size point field is not the same size as the "
        << "mesh."
        << abort(FatalError);
      }
    }
  }
}

// Destructor 
mousse::cellShapeControlMesh::~cellShapeControlMesh()
{}

// Member Functions 
void mousse::cellShapeControlMesh::barycentricCoords
(
  const mousse::point& pt,
  scalarList& bary,
  Cell_handle& ch
) const
{
  // Use the previous cell handle as a hint on where to start searching
  // Giving a hint causes strange errors...
  ch = locate(toPoint(pt));
  if (dimension() > 2 && !is_infinite(ch))
  {
    oldCellHandle_ = ch;
    tetPointRef tet
    {
      topoint(ch->vertex(0)->point()),
      topoint(ch->vertex(1)->point()),
      topoint(ch->vertex(2)->point()),
      topoint(ch->vertex(3)->point())
    };
    tet.barycentric(pt, bary);
  }
}

mousse::boundBox mousse::cellShapeControlMesh::bounds() const
{
  DynamicList<mousse::point> pts{static_cast<label>(number_of_vertices())};
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       ++vit)
  {
    if (vit->real())
    {
      pts.append(topoint(vit->point()));
    }
  }
  boundBox bb{pts};
  return bb;
}

void mousse::cellShapeControlMesh::distribute
(
  const backgroundMeshDecomposition& decomposition
)
{
  DynamicList<mousse::point> points{static_cast<label>(number_of_vertices())};
  DynamicList<scalar> sizes{static_cast<label>(number_of_vertices())};
  DynamicList<tensor> alignments{static_cast<label>(number_of_vertices())};
  DynamicList<Vb> farPts{8};
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       ++vit)
  {
    if (vit->real())
    {
      points.append(topoint(vit->point()));
      sizes.append(vit->targetCellSize());
      alignments.append(vit->alignment());
    }
    else if (vit->farPoint())
    {
      farPts.append
      (
        Vb
        (
          vit->point(),
          -1,
          Vb::vtFar,
          Pstream::myProcNo()
        )
      );
      farPts.last().targetCellSize() = vit->targetCellSize();
      farPts.last().alignment() = vit->alignment();
    }
  }
  autoPtr<mapDistribute> mapDist =
    DistributedDelaunayMesh<CellSizeDelaunay>::distribute
    (
      decomposition,
      points
    );
  mapDist().distribute(sizes);
  mapDist().distribute(alignments);
  // Reset the entire tessellation
  DelaunayMesh<CellSizeDelaunay>::reset();
  // Internal points have to be inserted first
  DynamicList<Vb> verticesToInsert(points.size());
  FOR_ALL(farPts, ptI)
  {
    verticesToInsert.append(farPts[ptI]);
  }
  FOR_ALL(points, pI)
  {
    verticesToInsert.append
    (
      Vb
      (
        toPoint(points[pI]),
        -1,
        Vb::vtInternal,
        Pstream::myProcNo()
      )
    );
    verticesToInsert.last().targetCellSize() = sizes[pI];
    verticesToInsert.last().alignment() = alignments[pI];
  }
  Info << nl << "    Inserting distributed background tessellation..." << endl;
  this->rangeInsertWithInfo
  (
    verticesToInsert.begin(),
    verticesToInsert.end(),
    true
  );
  sync(decomposition.procBounds());
  Info << "    Total number of vertices after redistribution "
    << returnReduce(label(number_of_vertices()), sumOp<label>()) << endl;
}
mousse::tensorField mousse::cellShapeControlMesh::dumpAlignments() const
{
  tensorField alignmentsTmp(number_of_vertices(), tensor::zero);
  label count = 0;
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       ++vit)
  {
    alignmentsTmp[count++] = vit->alignment();
  }
  return alignmentsTmp;
}
void mousse::cellShapeControlMesh::write() const
{
  Info << "Writing " << meshSubDir << endl;
  // Reindex the cells
  label cellCount = 0;
  for (auto cit = finite_cells_begin();
       cit != finite_cells_end();
       ++cit)
  {
    if (!cit->hasFarPoint() && !is_infinite(cit))
    {
      cit->cellIndex() = cellCount++;
    }
  }
  DelaunayMesh<CellSizeDelaunay>::labelTolabelPairHashTable vertexMap;
  labelList cellMap;
  autoPtr<polyMesh> meshPtr = DelaunayMesh<CellSizeDelaunay>::createMesh
  (
    meshSubDir,
    vertexMap,
    cellMap
  );
  const polyMesh& mesh = meshPtr();
  pointScalarField sizes
  {
    {
      "sizes",
      mesh.time().timeName(),
      meshSubDir,
      mesh.time(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    pointMesh::New(mesh),
    scalar(0)
  };
  triadIOField alignments
  {
    {
      "alignments",
      mesh.time().timeName(),
      meshSubDir,
      mesh.time(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    sizes.size()
  };
  // Write alignments
//    OFstream str(runTime_.path()/"alignments.obj");
  for (auto vit = finite_vertices_begin();
       vit != finite_vertices_end();
       ++vit)
  {
    if (!vit->farPoint())
    {
      // Populate sizes
      sizes[vertexMap[labelPair(vit->index(), vit->procIndex())]] =
        vit->targetCellSize();
      alignments[vertexMap[labelPair(vit->index(), vit->procIndex())]] =
        vit->alignment();
    }
  }
  mesh.write();
  sizes.write();
  alignments.write();
}
mousse::label mousse::cellShapeControlMesh::estimateCellCount
(
  const autoPtr<backgroundMeshDecomposition>& decomposition
) const
{
  // Loop over all the tets and estimate the cell count in each one
  scalar cellCount = 0;
  for (auto cit = finite_cells_begin();
       cit != finite_cells_end();
       ++cit)
  {
    if (!cit->hasFarPoint() && !is_infinite(cit))
    {
      // @todo Check if tet centre is on the processor..
      CGAL::Tetrahedron_3<baseK> tet
      (
        cit->vertex(0)->point(),
        cit->vertex(1)->point(),
        cit->vertex(2)->point(),
        cit->vertex(3)->point()
      );
      pointFromPoint centre = topoint(CGAL::centroid(tet));
      if
      (
        Pstream::parRun() && !decomposition().positionOnThisProcessor(centre)
      )
      {
        continue;
      }
      scalar volume = CGAL::to_double(tet.volume());
      scalar averagedPointCellSize = 0;
      //scalar averagedPointCellSize = 1;
      // Get an average volume by averaging the cell size of the vertices
      for (label vI = 0; vI < 4; ++vI)
      {
        averagedPointCellSize += cit->vertex(vI)->targetCellSize();
      }
      averagedPointCellSize /= 4;
      cellCount += volume/pow(averagedPointCellSize, 3);
    }
  }
  return cellCount;
}
