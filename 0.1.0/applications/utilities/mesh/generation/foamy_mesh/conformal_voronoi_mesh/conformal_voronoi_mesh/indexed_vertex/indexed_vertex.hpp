// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   CGAL::indexedVertex
// Description
//   An indexed form of CGAL::Triangulation_vertex_base_3<K> used to keep
//   track of the Delaunay vertices in the tessellation.
// SourceFiles
//   indexed_vertex_i.hpp
//   indexed_vertex.cpp
#ifndef indexed_vertex_hpp_
#define indexed_vertex_hpp_
#include "CGAL/Triangulation_3.h"
#include "cgal_triangulation_3d_kernel.hpp"
#include "tensor.hpp"
#include "triad.hpp"
#include "info_proxy.hpp"
#include "point.hpp"
#include "indexed_vertex_enum.hpp"
namespace CGAL
{
template<class Gt, class Vb> class indexedVertex;
}
namespace mousse
{
class Ostream;
class Istream;
template<class Gt, class Vb> Ostream& operator<<
(
  Ostream&,
  const mousse::InfoProxy<CGAL::indexedVertex<Gt, Vb> >&
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
namespace CGAL
{
template<class Gt, class Vb = CGAL::Triangulation_vertex_base_3<Gt> >
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
  typedef typename Vb::Triangulation_data_structure   Tds;
  typedef typename Vb::Point                          Point;
  typedef typename Tds::Vertex_handle                 Vertex_handle;
  typedef typename Tds::Cell_handle                   Cell_handle;
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
      return
      (
        //this->point() == rhs.point()
        this->type_ == rhs.type()
      && this->index_ == rhs.index()
      && this->processor_ == rhs.procIndex()
      && this->vertexFixed_ == rhs.fixed()
      );
    }
    inline bool operator!=(const indexedVertex& rhs) const
    {
      return !(*this == rhs);
    }
  // Info
    //- Return info proxy.
    //  Used to print indexedVertex information to a stream
    mousse::InfoProxy<indexedVertex<Gt, Vb> > info() const
    {
      return *this;
    }
    friend mousse::Ostream& mousse::operator<< <Gt, Vb>
    (
      mousse::Ostream&,
      const mousse::InfoProxy<indexedVertex<Gt, Vb> >&
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
namespace mousse
{
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
#include "indexed_vertex_i.hpp"
#ifdef NoRepository
  #include "indexed_vertex.cpp"
#endif
#endif
