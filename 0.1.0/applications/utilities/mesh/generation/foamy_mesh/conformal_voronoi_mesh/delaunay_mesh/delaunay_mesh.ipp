// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "delaunay_mesh.hpp"
#include "label_pair.hpp"
#include "print_table.hpp"
#include "point_io_field.hpp"
#include "scalar_io_field.hpp"
#include "label_io_field.hpp"
#include "point_conversion.hpp"
#include "poly_mesh.hpp"


// Constructors 
template<class Triangulation>
mousse::DelaunayMesh<Triangulation>::DelaunayMesh(const Time& runTime)
:
  Triangulation{},
  vertexCount_{0},
  cellCount_{0},
  runTime_{runTime}
{}


template<class Triangulation>
mousse::DelaunayMesh<Triangulation>::DelaunayMesh
(
  const Time& runTime,
  const word& meshName
)
:
  Triangulation{},
  vertexCount_{0},
  cellCount_{0},
  runTime_{runTime}
{
  Info << "Reading " << meshName << " from " << runTime.timeName() << endl;
  pointIOField pts
  {
    {
      "points",
      runTime.timeName(),
      meshName/polyMesh::meshSubDir,
      runTime,
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    }
  };
  if (pts.headerOk()) {
    labelIOField types
    {
      {
        "types",
        runTime.timeName(),
        meshName,
        runTime,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      }
    };
    labelIOField processorIndices
    {
      {
        "processorIndices",
        runTime.timeName(),
        meshName,
        runTime,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      }
    };
    List<Vb> pointsToInsert{pts.size()};
    FOR_ALL(pointsToInsert, pI) {
      pointsToInsert[pI] =
        Vb
        {
          toPoint(pts[pI]),
          pI,
          static_cast<indexedVertexEnum::vertexType>(types[pI]),
          static_cast<int>(processorIndices[pI])
        };
    }
    rangeInsertWithInfo
    (
      pointsToInsert.begin(),
      pointsToInsert.end(),
      false,
      false
    );
    vertexCount_ = Triangulation::number_of_vertices();
  }
}


// Destructor 
template<class Triangulation>
mousse::DelaunayMesh<Triangulation>::~DelaunayMesh()
{}


// Private Member Functions 
// Protected Member Functions 
// Member Functions 
template<class Triangulation>
void mousse::DelaunayMesh<Triangulation>::reset()
{
  Info << "Clearing triangulation" << endl;
  DynamicList<Vb> vertices;
  for (auto vit = Triangulation::finite_vertices_begin();
       vit != Triangulation::finite_vertices_end();
       ++vit) {
    if (!vit->fixed())
      continue;
    vertices.append
    (
      Vb
      {
        vit->point(),
        vit->index(),
        vit->type(),
        vit->procIndex()
      }
    );
    vertices.last().fixed() = vit->fixed();
  }
  this->clear();
  resetVertexCount();
  resetCellCount();
  insertPoints(vertices, false);
  Info << "Inserted " << vertexCount() << " fixed points" << endl;
}


template<class Triangulation>
mousse::Map<mousse::label> mousse::DelaunayMesh<Triangulation>::insertPoints
(
  const List<Vb>& vertices,
  const bool reIndex
)
{
  return rangeInsertWithInfo(vertices.begin(), vertices.end(), false, reIndex);
}


template<class Triangulation>
bool mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::Less_x_3::
operator()
(
  const Point_3& p,
  const Point_3& q
) const
{
  return typename Gt::Less_x_3()(*(p.first), *(q.first));
}


template<class Triangulation>
bool mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::Less_y_3::
operator()
(
  const Point_3& p,
  const Point_3& q
) const
{
  return typename Gt::Less_y_3()(*(p.first), *(q.first));
}


template<class Triangulation>
bool mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::Less_z_3::
operator()
(
  const Point_3& p,
  const Point_3& q
) const
{
  return typename Gt::Less_z_3()(*(p.first), *(q.first));
}


template<class Triangulation>
typename mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::Less_x_3
mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::less_x_3_object()
const
{
  return Less_x_3();
}


template<class Triangulation>
typename mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::Less_y_3
mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::less_y_3_object()
const
{
  return Less_y_3();
}


template<class Triangulation>
typename mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::Less_z_3
mousse::DelaunayMesh<Triangulation>::Traits_for_spatial_sort::less_z_3_object()
const
{
  return Less_z_3();
}


template<class Triangulation>
template<class PointIterator>
mousse::Map<mousse::label> mousse::DelaunayMesh<Triangulation>::rangeInsertWithInfo
(
  PointIterator begin,
  PointIterator end,
  bool printErrors,
  bool reIndex
)
{
  typedef DynamicList
  <
    std::pair
    <
      const typename Triangulation::Point*,
      label
    >
  > vectorPairPointIndex;
  vectorPairPointIndex points;
  label count = 0;
  for (PointIterator it = begin; it != end; ++it) {
    points.append
    (
      std::make_pair(&(it->point()), count++)
    );
  }
  std::random_shuffle(points.begin(), points.end());
  spatial_sort
  (
    points.begin(),
    points.end(),
    Traits_for_spatial_sort()
  );
  Vertex_handle hint;
  Map<label> oldToNewIndex(points.size());
  for (auto p = points.begin(); p != points.end(); ++p) {
    const size_t checkInsertion = Triangulation::number_of_vertices();
    hint = this->insert(*(p->first), hint);
    const Vb& vert = *(begin + p->second);
    if (checkInsertion != Triangulation::number_of_vertices() - 1) {
      if (printErrors) {
        Vertex_handle nearV =
          Triangulation::nearest_vertex(*(p->first));
        Pout << "Failed insertion : " << vert.info()
          << "         nearest : " << nearV->info();
      }
    } else {
      const label oldIndex = vert.index();
      hint->index() = getNewVertexIndex();
      if (reIndex) {
        oldToNewIndex.insert(oldIndex, hint->index());
      }
      hint->type() = vert.type();
      hint->procIndex() = vert.procIndex();
      hint->targetCellSize() = vert.targetCellSize();
      hint->alignment() = vert.alignment();
    }
  }
  return oldToNewIndex;
}

#include "delaunay_mesh_io.ipp"

