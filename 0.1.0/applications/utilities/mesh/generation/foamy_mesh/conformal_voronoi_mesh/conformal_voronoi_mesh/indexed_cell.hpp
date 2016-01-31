// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   CGAL::indexedCell
// Description
//   An indexed form of CGAL::Triangulation_cell_base_3<K> used to keep
//   track of the Delaunay cells (tets) in the tessellation.
// SourceFiles
//   indexed_cell.cpp
#ifndef indexed_cell_hpp_
#define indexed_cell_hpp_
#include "CGAL/Triangulation_3.h"
#include "CGAL/Delaunay_triangulation_cell_base_with_circumcenter_3.h"
#include "indexed_vertex.hpp"
#include "list.hpp"
#include "global_index.hpp"
#include "pstream.hpp"
#include "swap.hpp"
#include "info_proxy.hpp"
#include "tet_cell.hpp"
#include "type_info.hpp"
#include "vector_tools.hpp"
#include "indexed_cell_enum.hpp"
namespace CGAL
{
template<class Gt, class Cb> class indexedCell;
}
namespace mousse
{
class Ostream;
template<class Gt, class Cb> Ostream& operator<<
(
  Ostream&,
  const mousse::InfoProxy<CGAL::indexedCell<Gt, Cb> >&
);
}
namespace CGAL
{
template
<
  class Gt,
  class Cb=CGAL::Delaunay_triangulation_cell_base_with_circumcenter_3<Gt>
>
class indexedCell
:
  public mousse::indexedCellEnum,
  public Cb
{
  // Private data
    //- The index for this Delaunay tetrahedral cell.  Type information is
    //- Also carried:
    //  ctFar : the dual point of this cell does not form part of the
    //          internal or boundary of the dual mesh
    //  >=0   : the (local) index of an internal or boundary dual point,
    //           not on a processor face
    //  < 0 && > ctFar : the (global) index of a dual point on a processor
    //                   face
    mousse::label index_;
    //- The number of times that this Delaunay cell has been limited
    //  during filtering
    int filterCount_;
  // Private member functions
    //- Same as globallyOrderedCellVertices but without sorting
    mousse::tetCell unsortedVertexGlobalIndices
    (
      const mousse::globalIndex& globalDelaunayVertexIndices
    ) const;
public:
  typedef typename Cb::Triangulation_data_structure    Tds;
  typedef typename Cb::Vertex_handle                   Vertex_handle;
  typedef typename Cb::Cell_handle                     Cell_handle;
  template<typename TDS2>
  struct Rebind_TDS
  {
    typedef typename Cb::template Rebind_TDS<TDS2>::Other Cb2;
    typedef indexedCell<Gt, Cb2>  Other;
  };
  // Constructors
    inline indexedCell();
    inline indexedCell
    (
      Vertex_handle v0,
      Vertex_handle v1,
      Vertex_handle v2,
      Vertex_handle v3
    );
    inline indexedCell
    (
      Vertex_handle v0,
      Vertex_handle v1,
      Vertex_handle v2,
      Vertex_handle v3,
      Cell_handle n0,
      Cell_handle n1,
      Cell_handle n2,
      Cell_handle n3
    );
  // Member Functions
    inline mousse::label& cellIndex();
    inline mousse::label cellIndex() const;
#ifdef CGAL_INEXACT
    inline const mousse::point& dual();
#else
    inline const mousse::point dual();
#endif
    inline bool unassigned() const;
    inline int& filterCount();
    inline int filterCount() const;
    //- Is the Delaunay cell real, i.e. any real vertex
    inline bool real() const;
    //- Does the Delaunay cell have a far point
    inline bool hasFarPoint() const;
    //- Does the Delaunay cell have a referred point
    inline bool hasReferredPoint() const;
    //- Does the Delaunay cell have a feature point
    inline bool hasFeaturePoint() const;
    //- Does the Delaunay cell have a seed point
    inline bool hasSeedPoint() const;
    inline bool hasInternalPoint() const;
    inline bool hasBoundaryPoint() const;
    inline bool hasConstrainedPoint() const;
    //- Does the Dual vertex form part of a processor patch
    inline bool parallelDualVertex() const;
    inline mousse::label vertexLowestProc() const;
    //- Using the globalIndex object, return a list of four (sorted) global
    //  Delaunay vertex indices that uniquely identify this tet in parallel
    inline mousse::tetCell vertexGlobalIndices
    (
      const mousse::globalIndex& globalDelaunayVertexIndices
    ) const;
    //- Using the globalIndex object, return a list of four vertices with
    //  so that the cell has a consistent orientation in parallel.
    inline mousse::FixedList<mousse::label, 4> globallyOrderedCellVertices
    (
      const mousse::globalIndex& globalDelaunayVertexIndices
    ) const;
    //- Is the Delaunay cell part of the final dual mesh, i.e. any vertex
    //  form part of the internal or boundary definition
    inline bool internalOrBoundaryDualVertex() const;
    //- Is the Delaunay cell real or referred (or mixed), i.e. all vertices
    //  form part of the real or referred internal or boundary definition
    inline bool anyInternalOrBoundaryDualVertex() const;
    //- A dual vertex on the boundary will result from a Delaunay cell with
    //  least one Delaunay vertex outside and at least one inside
    inline bool boundaryDualVertex() const;
    inline bool baffleSurfaceDualVertex() const;
    inline bool baffleEdgeDualVertex() const;
    //- A dual vertex on a feature edge will result from this Delaunay cell
    inline bool featureEdgeDualVertex() const;
    //- A dual vertex on a feature point will result from this Delaunay cell
    inline bool featurePointDualVertex() const;
    inline bool nearProcBoundary() const;
    inline bool potentialCoplanarCell() const;
    inline bool featurePointExternalCell() const;
    inline bool featurePointInternalCell() const;
  // Info
    //- Return info proxy.
    //  Used to print indexedCell information to a stream
    mousse::InfoProxy<indexedCell<Gt, Cb> > info() const
    {
      return *this;
    }
    friend mousse::Ostream& mousse::operator<< <Gt, Cb>
    (
      mousse::Ostream&,
      const mousse::InfoProxy<indexedCell<Gt, Cb> >&
    );
};
}  // namespace CGAL

template<class Gt, class Cb>
mousse::tetCell CGAL::indexedCell<Gt, Cb>::unsortedVertexGlobalIndices
(
  const mousse::globalIndex& globalDelaunayVertexIndices
) const
{
  mousse::tetCell tVGI;
  for (int i = 0; i < 4; i++)
  {
    Vertex_handle v = this->vertex(i);
    // Finding the global index of each Delaunay vertex
    tVGI[i] = globalDelaunayVertexIndices.toGlobal
    (
      mousse::Pstream::myProcNo(),
      v->index()
    );
  }
  return tVGI;
}
// Constructors 
template<class Gt, class Cb>
CGAL::indexedCell<Gt, Cb>::indexedCell()
:
  Cb{},
  index_{ctUnassigned},
  filterCount_{0}
{}
template<class Gt, class Cb>
CGAL::indexedCell<Gt, Cb>::indexedCell
(
  Vertex_handle v0, Vertex_handle v1, Vertex_handle v2, Vertex_handle v3
)
:
  Cb{v0, v1, v2, v3},
  index_{ctUnassigned},
  filterCount_{0}
{}
template<class Gt, class Cb>
CGAL::indexedCell<Gt, Cb>::indexedCell
(
  Vertex_handle v0,
  Vertex_handle v1,
  Vertex_handle v2,
  Vertex_handle v3,
  Cell_handle n0,
  Cell_handle n1,
  Cell_handle n2,
  Cell_handle n3
)
:
  Cb{v0, v1, v2, v3, n0, n1, n2, n3},
  index_{ctUnassigned},
  filterCount_{0}
{}
// Member Functions 
template<class Gt, class Cb>
mousse::label& CGAL::indexedCell<Gt, Cb>::cellIndex()
{
  return index_;
}
template<class Gt, class Cb>
mousse::label CGAL::indexedCell<Gt, Cb>::cellIndex() const
{
  return index_;
}
#ifdef CGAL_INEXACT
  template<class Gt, class Cb>
  const mousse::point& CGAL::indexedCell<Gt, Cb>::dual()
  {
    return reinterpret_cast<const mousse::point&>(this->circumcenter());
  }
#else
  template<class Gt, class Cb>
  const mousse::point CGAL::indexedCell<Gt, Cb>::dual()
  {
    const typename Gt::Point_3& P = this->circumcenter();
    return mousse::point(CGAL::to_double(P.x()),
                         CGAL::to_double(P.y()),
                         CGAL::to_double(P.z()));
  }
#endif
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::unassigned() const
{
  return index_ == ctUnassigned;
}
template<class Gt, class Cb>
inline int& CGAL::indexedCell<Gt, Cb>::filterCount()
{
  return filterCount_;
}
template<class Gt, class Cb>
inline int CGAL::indexedCell<Gt, Cb>::filterCount() const
{
  return filterCount_;
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::real() const
{
  return ((this->vertex(0)->real()
           || this->vertex(1)->real()
           || this->vertex(2)->real()
           || this->vertex(3)->real())
          &&
          !(this->vertex(0)->farPoint()
            || this->vertex(1)->farPoint()
            || this->vertex(2)->farPoint()
            || this->vertex(3)->farPoint()));
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::hasFarPoint() const
{
  return (this->vertex(0)->farPoint()
          || this->vertex(1)->farPoint()
          || this->vertex(2)->farPoint()
          || this->vertex(3)->farPoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::hasReferredPoint() const
{
  return (this->vertex(0)->referred()
          || this->vertex(1)->referred()
          || this->vertex(2)->referred()
          || this->vertex(3)->referred());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::hasFeaturePoint() const
{
  return (this->vertex(0)->featurePoint()
          || this->vertex(1)->featurePoint()
          || this->vertex(2)->featurePoint()
          || this->vertex(3)->featurePoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::hasSeedPoint() const
{
  return (this->vertex(0)->seedPoint()
          || this->vertex(1)->seedPoint()
          || this->vertex(2)->seedPoint()
          || this->vertex(3)->seedPoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::hasInternalPoint() const
{
  return (this->vertex(0)->internalPoint()
          || this->vertex(1)->internalPoint()
          || this->vertex(2)->internalPoint()
          || this->vertex(3)->internalPoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::hasBoundaryPoint() const
{
  return (this->vertex(0)->boundaryPoint()
          || this->vertex(1)->boundaryPoint()
          || this->vertex(2)->boundaryPoint()
          || this->vertex(3)->boundaryPoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::hasConstrainedPoint() const
{
  return (this->vertex(0)->constrained()
          || this->vertex(1)->constrained()
          || this->vertex(2)->constrained()
          || this->vertex(3)->constrained());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::parallelDualVertex() const
{
  return (!this->hasFarPoint()
          &&
          (this->vertex(0)->referred()
           || this->vertex(1)->referred()
           || this->vertex(2)->referred()
           || this->vertex(3)->referred())
          &&
          (this->vertex(0)->real()
           || this->vertex(1)->real()
           || this->vertex(2)->real()
           || this->vertex(3)->real()));
}
template<class Gt, class Cb>
inline mousse::label CGAL::indexedCell<Gt, Cb>::vertexLowestProc() const
{
  mousse::label lowestProc = -1;
  for (int i = 0; i < 4; ++i)
  {
    if (this->vertex(i)->referred())
    {
      lowestProc = min(lowestProc, this->vertex(i)->procIndex());
    }
  }
  return lowestProc;
}
template<class Gt, class Cb>
inline mousse::tetCell CGAL::indexedCell<Gt, Cb>::vertexGlobalIndices
(
  const mousse::globalIndex& globalDelaunayVertexIndices
) const
{
  // tetVertexGlobalIndices
  mousse::tetCell tVGI
    = unsortedVertexGlobalIndices(globalDelaunayVertexIndices);
  // bubble sort
  for (int i = 0; i < tVGI.size(); i++)
  {
    for (int j = tVGI.size() - 1 ; j > i; j--)
    {
      if (tVGI[j - 1] > tVGI[j])
      {
        mousse::Swap(tVGI[j - 1], tVGI[j]);
      }
    }
  }
  return tVGI;
}
template<class Gt, class Cb>
inline mousse::FixedList<mousse::label, 4>
CGAL::indexedCell<Gt, Cb>::globallyOrderedCellVertices
(
  const mousse::globalIndex& globalDelaunayVertexIndices
) const
{
  // tetVertexGlobalIndices
  mousse::tetCell tVGI
    = unsortedVertexGlobalIndices(globalDelaunayVertexIndices);
  mousse::FixedList<mousse::label, 4> vertexMap(mousse::identity(4));
  // bubble sort
  for (int i = 0; i < tVGI.size(); i++)
  {
    for (int j = tVGI.size() - 1 ; j > i; j--)
    {
      if (tVGI[j - 1] > tVGI[j])
      {
        mousse::Swap(tVGI[j - 1], tVGI[j]);
        mousse::Swap(vertexMap[j - 1], vertexMap[j]);
      }
    }
  }
  for (int i = 0; i < 4; i++)
  {
    tVGI[i] = vertexMap[i];
  }
  return tVGI;
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::internalOrBoundaryDualVertex() const
{
  return (this->vertex(0)->internalOrBoundaryPoint()
          || this->vertex(1)->internalOrBoundaryPoint()
          || this->vertex(2)->internalOrBoundaryPoint()
          || this->vertex(3)->internalOrBoundaryPoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::anyInternalOrBoundaryDualVertex() const
{
  return (this->vertex(0)->internalOrBoundaryPoint()
          || this->vertex(0)->externalBoundaryPoint()
          || this->vertex(1)->internalOrBoundaryPoint()
          || this->vertex(1)->externalBoundaryPoint()
          || this->vertex(2)->internalOrBoundaryPoint()
          || this->vertex(2)->externalBoundaryPoint()
          || this->vertex(3)->internalOrBoundaryPoint()
          || this->vertex(3)->externalBoundaryPoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::boundaryDualVertex() const
{
  return ((this->vertex(0)->internalBoundaryPoint()
           || this->vertex(1)->internalBoundaryPoint()
           || this->vertex(2)->internalBoundaryPoint()
           || this->vertex(3)->internalBoundaryPoint())
          &&
          (this->vertex(0)->externalBoundaryPoint()
           || this->vertex(1)->externalBoundaryPoint()
           || this->vertex(2)->externalBoundaryPoint()
           || this->vertex(3)->externalBoundaryPoint()));
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::baffleSurfaceDualVertex() const
{
  return ((this->vertex(0)->internalBaffleSurfacePoint()
           || this->vertex(1)->internalBaffleSurfacePoint()
           || this->vertex(2)->internalBaffleSurfacePoint()
           || this->vertex(3)->internalBaffleSurfacePoint())
          &&
          (this->vertex(0)->externalBaffleSurfacePoint()
           || this->vertex(1)->externalBaffleSurfacePoint()
           || this->vertex(2)->externalBaffleSurfacePoint()
           || this->vertex(3)->externalBaffleSurfacePoint()));
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::baffleEdgeDualVertex() const
{
  return ((this->vertex(0)->internalBaffleEdgePoint()
           || this->vertex(1)->internalBaffleEdgePoint()
           || this->vertex(2)->internalBaffleEdgePoint()
           || this->vertex(3)->internalBaffleEdgePoint())
          &&
          (this->vertex(0)->externalBaffleEdgePoint()
           || this->vertex(1)->externalBaffleEdgePoint()
           || this->vertex(2)->externalBaffleEdgePoint()
           || this->vertex(3)->externalBaffleEdgePoint()));
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::featureEdgeDualVertex() const
{
  return (this->vertex(0)->featureEdgePoint()
          && this->vertex(1)->featureEdgePoint()
          && this->vertex(2)->featureEdgePoint()
          && this->vertex(3)->featureEdgePoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::featurePointDualVertex() const
{
  return (this->vertex(0)->featurePoint()
          && this->vertex(1)->featurePoint()
          && this->vertex(2)->featurePoint()
          && this->vertex(3)->featurePoint());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::nearProcBoundary() const
{
  return (this->vertex(0)->nearProcBoundary()
          || this->vertex(1)->nearProcBoundary()
          || this->vertex(2)->nearProcBoundary()
          || this->vertex(3)->nearProcBoundary());
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::potentialCoplanarCell() const
{
  mousse::label nMasters = 0;
  mousse::label nSlaves = 0;
  Vertex_handle vM[2];
  Vertex_handle vS[2];
  for (mousse::label i = 0; i < 4; ++i)
  {
    Vertex_handle v = this->vertex(i);
    if (v->internalBoundaryPoint())
    {
      vM[nMasters] = v;
      nMasters++;
    }
    if (v->externalBoundaryPoint())
    {
      vS[nSlaves] = v;
      nSlaves++;
    }
  }
  mousse::label nPairs = 0;
  if (nMasters == 2 && nSlaves == 2)
  {
    mousse::vector vp0(mousse::vector::zero);
    mousse::vector vp1(mousse::vector::zero);
    if (vM[0]->type() == vS[0]->index() && vM[0]->index() == vS[0]->type())
    {
      vp0 = reinterpret_cast<const mousse::point&>(vM[0]->point())
        - reinterpret_cast<const mousse::point&>(vS[0]->point());
      nPairs++;
    }
    else if (vM[0]->type() == vS[1]->index()
             && vM[0]->index() == vS[1]->type())
    {
      vp0 = reinterpret_cast<const mousse::point&>(vM[0]->point())
        - reinterpret_cast<const mousse::point&>(vS[1]->point());
      nPairs++;
    }
    if (vM[1]->type() == vS[0]->index() && vM[1]->index() == vS[0]->type())
    {
      vp1 = reinterpret_cast<const mousse::point&>(vM[1]->point())
        - reinterpret_cast<const mousse::point&>(vS[0]->point());
      nPairs++;
    }
    else if (vM[1]->type() == vS[1]->index()
             && vM[1]->index() == vS[1]->type())
    {
      vp1 = reinterpret_cast<const mousse::point&>(vM[1]->point())
        - reinterpret_cast<const mousse::point&>(vS[1]->point());
      nPairs++;
    }
    if (nPairs == 2)
    {
      if (mousse::vectorTools::areParallel(vp0, vp1))
      {
        mousse::Pout<< "PARALLEL" << mousse::endl;
        return true;
      }
    }
  }
  return false;
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::featurePointExternalCell() const
{
  int featureVertex = -1;
  for (int i = 0; i < 4; ++i)
  {
    if (this->vertex(i)->constrained())
    {
      featureVertex = i;
    }
  }
  // Pick cell with a face attached to an infinite cell
  if (featureVertex != -1)
  {
    Vertex_handle v1 =
      this->vertex(Tds::vertex_triple_index(featureVertex, 0));
    Vertex_handle v2 =
      this->vertex(Tds::vertex_triple_index(featureVertex, 1));
    Vertex_handle v3 =
      this->vertex(Tds::vertex_triple_index(featureVertex, 2));
    if (v1->internalBoundaryPoint())
    {
      if (v2->externalBoundaryPoint() && v3->externalBoundaryPoint())
      {
        return true;
      }
    }
    else if (v2->internalBoundaryPoint())
    {
      if (v1->externalBoundaryPoint() && v3->externalBoundaryPoint())
      {
        return true;
      }
    }
    else if (v3->internalBoundaryPoint())
    {
      if (v1->externalBoundaryPoint() && v2->externalBoundaryPoint())
      {
        return true;
      }
    }
  }
  return false;
}
template<class Gt, class Cb>
inline bool CGAL::indexedCell<Gt, Cb>::featurePointInternalCell() const
{
  int featureVertex = -1;
  for (int i = 0; i < 4; ++i)
  {
    if (this->vertex(i)->constrained())
    {
      featureVertex = i;
    }
  }
  // Pick cell with a face attached to an infinite cell
  if (featureVertex != -1)
  {
    Vertex_handle v1 =
      this->vertex(Tds::vertex_triple_index(featureVertex, 0));
    Vertex_handle v2 =
      this->vertex(Tds::vertex_triple_index(featureVertex, 1));
    Vertex_handle v3 =
      this->vertex(Tds::vertex_triple_index(featureVertex, 2));
    if (v1->externalBoundaryPoint())
    {
      if (v2->internalBoundaryPoint() && v3->internalBoundaryPoint())
      {
        return true;
      }
    }
    else if (v2->externalBoundaryPoint())
    {
      if (v1->internalBoundaryPoint() && v3->internalBoundaryPoint())
      {
        return true;
      }
    }
    else if (v3->externalBoundaryPoint())
    {
      if (v1->internalBoundaryPoint() && v2->internalBoundaryPoint())
      {
        return true;
      }
    }
  }
  return false;
}

#ifdef NoRepository
  #include "indexed_cell.cpp"
#endif
#endif
