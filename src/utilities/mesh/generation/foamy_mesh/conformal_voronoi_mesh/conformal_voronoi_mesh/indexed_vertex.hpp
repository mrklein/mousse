#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_INDEXED_VERTEX_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CONFORMAL_VORONOI_MESH_INDEXED_VERTEX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   CGAL::indexedVertex
// Description
//   An indexed form of CGAL::Triangulation_vertex_base_3<K> used to keep
//   track of the Delaunay vertices in the tessellation.

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include "CGAL/Triangulation_3.h"
#pragma clang diagnostic pop
#include "cgal_triangulation_3d_kernel.hpp"
#include "tensor.hpp"
#include "triad.hpp"
#include "info_proxy.hpp"
#include "point.hpp"
#include "indexed_vertex_enum.hpp"
#include "pstream.hpp"


namespace CGAL {

template<class Gt, class Vb> class indexedVertex;

}


namespace mousse {

class Ostream;
class Istream;
template<class Gt, class Vb> Ostream& operator<<
(
  Ostream&,
  const mousse::InfoProxy<CGAL::indexedVertex<Gt, Vb>>&
);
template<class Gt, class Vb> Ostream& operator<<
(
  Ostream&,
  const CGAL::indexedVertex<Gt, Vb>&
);
template<class Gt, class Vb> Istream& operator>>
(
  Istream&,
  CGAL::indexedVertex<Gt, Vb>&
);
inline Istream& operator>>
(
  Istream& is,
  CGAL::Point_3<baseK>& p
);
inline Ostream& operator<<
(
  Ostream& os,
  const CGAL::Point_3<baseK>& p
);

}  // namespace mousse


namespace CGAL {

template<class Gt, class Vb = CGAL::Triangulation_vertex_base_3<Gt>>
class indexedVertex
:
  public mousse::indexedVertexEnum,
  public Vb
{
  // Private data
    //- Type of pair-point
    vertexType type_;
    //- The index for this Delaunay vertex.  For referred vertices, the
    //  index is negative for vertices that are the outer (slave) of point
    //  pairs
    mousse::label index_;
    //- Number of the processor that owns this vertex
    int processor_;
    //- Required alignment of the dual cell of this vertex
    mousse::tensor alignment_;
    //- Target size of the dual cell of this vertex
    mousse::scalar targetCellSize_;
    //- Specify whether the vertex is fixed or movable.
    bool vertexFixed_;
public:
  typedef typename Vb::Triangulation_data_structure Tds;
  typedef typename Vb::Point Point;
  typedef typename Tds::Vertex_handle Vertex_handle;
  typedef typename Tds::Cell_handle Cell_handle;
  template<typename TDS2>
  struct Rebind_TDS
  {
    typedef typename Vb::template Rebind_TDS<TDS2>::Other   Vb2;
    typedef indexedVertex<Gt,Vb2>                           Other;
  };
  // Constructors
    inline indexedVertex();
    inline indexedVertex(const Point& p);
    inline indexedVertex(const Point& p, vertexType type);
    inline indexedVertex(const mousse::point& p, vertexType type);
    inline indexedVertex
    (
      const Point& p,
      mousse::label index,
      vertexType type,
      int processor
    );
    inline indexedVertex
    (
      const mousse::point& p,
      mousse::label index,
      vertexType type,
      int processor
    );
    inline indexedVertex(const Point& p, Cell_handle f);
    inline indexedVertex(Cell_handle f);
  // Member Functions
    inline mousse::label& index();
    inline mousse::label index() const;
    inline vertexType& type();
    inline vertexType type() const;
    inline mousse::tensor& alignment();
    inline const mousse::tensor& alignment() const;
    inline mousse::scalar& targetCellSize();
    inline mousse::scalar targetCellSize() const;
    //- Is point a far-point
    inline bool farPoint() const;
    //- Is point internal, i.e. not on boundary
    inline bool internalPoint() const;
    //- Is this a referred vertex
    inline bool referred() const;
    //- Is this a "real" point on this processor, i.e. is internal or part
    //  of the boundary description, and not a "far" or "referred" point
    inline bool real() const;
    // For referred vertices, what is the original processor index
    inline int procIndex() const;
    // For referred vertices, set the original processor index
    inline int& procIndex();
    //- Set the point to be internal
    inline void setInternal();
    //- Is point internal and near the boundary
    inline bool nearBoundary() const;
    //- Set the point to be near the boundary
    inline void setNearBoundary();
    //- Is point internal and near a proc boundary
    inline bool nearProcBoundary() const;
    //- Set the point to be near a proc boundary
    inline void setNearProcBoundary();
    //- Either master or slave of pointPair.
    inline bool boundaryPoint() const;
    //- Either original internal point or master of pointPair.
    inline bool internalOrBoundaryPoint() const;
    //- Is point near the boundary or part of the boundary definition
    inline bool nearOrOnBoundary() const;
    //- Part of a feature point
    inline bool featurePoint() const;
    //- Part of a feature edge
    inline bool featureEdgePoint() const;
    //- Part of a surface point pair
    inline bool surfacePoint() const;
    inline bool internalBoundaryPoint() const;
    inline bool internalBaffleSurfacePoint() const;
    inline bool internalBaffleEdgePoint() const;
    inline bool externalBoundaryPoint() const;
    inline bool externalBaffleSurfacePoint() const;
    inline bool externalBaffleEdgePoint() const;
    inline bool constrained() const;
    //- Is the vertex fixed or movable
    inline bool fixed() const;
    //- Fix the vertex so that it can't be moved
    inline bool& fixed();
    inline indexedVertex& operator=(const indexedVertex& rhs)
    {
      Vb::operator=(rhs);
      this->type_ = rhs.type();
      this->index_ = rhs.index();
      this->processor_ = rhs.procIndex();
      this->alignment_ = rhs.alignment();
      this->targetCellSize_ = rhs.targetCellSize();
      this->vertexFixed_ = rhs.fixed();
      return *this;
    }
    inline bool operator==(const indexedVertex& rhs) const
    {
      return (this->type_ == rhs.type()
              && this->index_ == rhs.index()
              && this->processor_ == rhs.procIndex()
              && this->vertexFixed_ == rhs.fixed());
    }
    inline bool operator!=(const indexedVertex& rhs) const
    {
      return !(*this == rhs);
    }
  // Info
    //- Return info proxy.
    //  Used to print indexedVertex information to a stream
    mousse::InfoProxy<indexedVertex<Gt, Vb>> info() const
    {
      return *this;
    }
    friend mousse::Ostream& mousse::operator<< <Gt, Vb>
    (
      mousse::Ostream&,
      const mousse::InfoProxy<indexedVertex<Gt, Vb>>&
    );
    friend mousse::Ostream& mousse::operator<< <Gt, Vb>
    (
      mousse::Ostream&,
      const indexedVertex<Gt, Vb>&
    );
    friend mousse::Istream& mousse::operator>> <Gt, Vb>
    (
      mousse::Istream&,
      indexedVertex<Gt, Vb>&
    );
};

}  // namespace CGAL


// Global Functions 
#ifdef CGAL_INEXACT
namespace mousse {

// For inexact representations where the storage type is a double, the data
// is contiguous. This may not be true for exact number types.
template<>
  inline bool contiguous
  <
    CGAL::indexedVertex
    <
      K,
      CGAL::Triangulation_vertex_base_3<K>
    >
  >()
  {
    return true;
  }

template<>
  inline bool contiguous<CGAL::Triangulation_vertex_base_3<K>::Point>()
  {
    return true;
  }

}  // namespace mousse

#endif


// Constructors 
template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex()
:
  Vb{},
  type_{vtUnassigned},
  index_{-1},
  processor_{mousse::Pstream::myProcNo()},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex(const Point& p)
:
  Vb{p},
  type_{vtUnassigned},
  index_{-1},
  processor_{mousse::Pstream::myProcNo()},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex
(
  const Point& p,
  vertexType type
)
:
  Vb{p},
  type_{type},
  index_{-1},
  processor_{mousse::Pstream::myProcNo()},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex
(
  const mousse::point& p,
  vertexType type
)
:
  Vb{Point(p.x(), p.y(), p.z())},
  type_{type},
  index_{-1},
  processor_{mousse::Pstream::myProcNo()},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex
(
  const Point& p,
  mousse::label index,
  vertexType type,
  int processor
)
:
  Vb{p},
  type_{type},
  index_{index},
  processor_{processor},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex
(
  const mousse::point& p,
  mousse::label index,
  vertexType type,
  int processor
)
:
  Vb{Point(p.x(), p.y(), p.z())},
  type_{type},
  index_{index},
  processor_{processor},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex(const Point& p, Cell_handle f)
:
  Vb{f, p},
  type_{vtUnassigned},
  index_{-1},
  processor_{mousse::Pstream::myProcNo()},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex(Cell_handle f)
:
  Vb{f},
  type_{vtUnassigned},
  index_{-1},
  processor_{mousse::Pstream::myProcNo()},
  alignment_{mousse::triad::unset},
  targetCellSize_{0.0},
  vertexFixed_{false}
{}


// Member Functions 
template<class Gt, class Vb>
inline mousse::label& CGAL::indexedVertex<Gt, Vb>::index()
{
  return index_;
}


template<class Gt, class Vb>
inline mousse::label CGAL::indexedVertex<Gt, Vb>::index() const
{
  return index_;
}


template<class Gt, class Vb>
inline mousse::indexedVertexEnum::vertexType&
CGAL::indexedVertex<Gt, Vb>::type()
{
  return type_;
}


template<class Gt, class Vb>
inline mousse::indexedVertexEnum::vertexType
CGAL::indexedVertex<Gt, Vb>::type() const
{
  return type_;
}


template<class Gt, class Vb>
inline mousse::tensor& CGAL::indexedVertex<Gt, Vb>::alignment()
{
  return alignment_;
}


template<class Gt, class Vb>
inline const mousse::tensor& CGAL::indexedVertex<Gt, Vb>::alignment() const
{
  return alignment_;
}


template<class Gt, class Vb>
inline mousse::scalar& CGAL::indexedVertex<Gt, Vb>::targetCellSize()
{
  return targetCellSize_;
}


template<class Gt, class Vb>
inline mousse::scalar CGAL::indexedVertex<Gt, Vb>::targetCellSize() const
{
  return targetCellSize_;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::farPoint() const
{
  return type_ == vtFar;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::internalPoint() const
{
  return type_ == vtInternal || type_ == vtInternalNearBoundary;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::referred() const
{
  // Can't be zero as the first few points are far points which won't be
  // referred
  //return index_ < 0;
  // processor_ will be take the value of the processor that this vertex is
  // from, so it cannot be on this processor.
  return processor_ != mousse::Pstream::myProcNo();
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::real() const
{
  return (internalPoint() || boundaryPoint()) && !referred();
}


template<class Gt, class Vb>
inline int CGAL::indexedVertex<Gt, Vb>::procIndex() const
{
  return processor_;
}


template<class Gt, class Vb>
inline int& CGAL::indexedVertex<Gt, Vb>::procIndex()
{
  return processor_;
}


template<class Gt, class Vb>
inline void CGAL::indexedVertex<Gt, Vb>::setInternal()
{
  type_ = vtInternal;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::nearBoundary() const
{
  return type_ == vtInternalNearBoundary;
}


template<class Gt, class Vb>
inline void CGAL::indexedVertex<Gt, Vb>::setNearBoundary()
{
  type_ = vtInternalNearBoundary;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::boundaryPoint() const
{
  return type_ >= vtInternalSurface && !farPoint();
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::internalOrBoundaryPoint() const
{
  return internalPoint() || internalBoundaryPoint();
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::nearOrOnBoundary() const
{
  return boundaryPoint() || nearBoundary();
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::internalBoundaryPoint() const
{
  return type_ >= vtInternalSurface && type_ <= vtInternalFeaturePoint;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::internalBaffleSurfacePoint() const
{
  return (type_ == vtInternalSurfaceBaffle);
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::internalBaffleEdgePoint() const
{
  return (type_ == vtInternalFeatureEdgeBaffle);
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::externalBoundaryPoint() const
{
  return type_ >= vtExternalSurface && type_ <= vtExternalFeaturePoint;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::externalBaffleSurfacePoint() const
{
  return (type_ == vtExternalSurfaceBaffle);
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::externalBaffleEdgePoint() const
{
  return (type_ == vtExternalFeatureEdgeBaffle);
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::constrained() const
{
  return type_ == vtConstrained;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::featurePoint() const
{
  return type_ == vtInternalFeaturePoint || type_ == vtExternalFeaturePoint;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::featureEdgePoint() const
{
  return type_ == vtInternalFeatureEdge || type_ == vtExternalFeatureEdge;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::surfacePoint() const
{
  return type_ == vtInternalSurface || type_ == vtExternalSurface;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::fixed() const
{
  return vertexFixed_;
}


template<class Gt, class Vb>
inline bool& CGAL::indexedVertex<Gt, Vb>::fixed()
{
  return vertexFixed_;
}

#include "indexed_vertex.ipp"

#endif
