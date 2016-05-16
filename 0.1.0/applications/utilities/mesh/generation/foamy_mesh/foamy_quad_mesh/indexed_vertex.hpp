#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_INDEXED_VERTEX_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_INDEXED_VERTEX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   CGAL::indexedVertex
// Description
//   An indexed form of CGAL::Triangulation_vertex_base_2<K> used to keep
//   track of the vertices in the triangulation.

#include "CGAL/Triangulation_2.h"


namespace CGAL {

// Forward declaration of friend functions and operators
template<class Gt, class Vb>
class indexedVertex;

template<class Gt, class Vb>
bool pointPair
(
  const indexedVertex<Gt, Vb>& v0,
  const indexedVertex<Gt, Vb>& v1
);

template<class Gt, class Vb>
bool boundaryTriangle
(
  const indexedVertex<Gt, Vb>& v0,
  const indexedVertex<Gt, Vb>& v1,
  const indexedVertex<Gt, Vb>& v2
);

template<class Gt, class Vb>
bool outsideTriangle
(
  const indexedVertex<Gt, Vb>& v0,
  const indexedVertex<Gt, Vb>& v1,
  const indexedVertex<Gt, Vb>& v2
);


template<class Gt, class Vb=CGAL::Triangulation_vertex_base_2<Gt> >
class indexedVertex
:
  public Vb
{
  // Private data
    //- The index for this triangle vertex
    int index_;
    //- Index of pair-point :
    //  NEAR_BOUNDARY_POINT : internal near boundary point.
    //  INTERNAL_POINT      : internal point.
    //  FAR_POINT           : far-point.
    //  >= 0                : part of point-pair. Index of other point.
    //                        Lowest numbered is inside one (master).
    int type_;
public:
  enum pointTypes
  {
    NEAR_BOUNDARY_POINT = -4,
    INTERNAL_POINT      = -3,
    MIRROR_POINT        = -2,
    FAR_POINT           = -1
  };
  typedef typename Vb::Vertex_handle Vertex_handle;
  typedef typename Vb::Face_handle Face_handle;
  typedef typename Vb::Point Point;
  template<typename TDS2>
  struct Rebind_TDS
  {
    typedef typename Vb::template Rebind_TDS<TDS2>::Other    Vb2;
    typedef indexedVertex<Gt,Vb2>                           Other;
  };
  // Constructors
    inline indexedVertex();
    inline indexedVertex(const Point& p);
    inline indexedVertex(const Point& p, const int index, const int& type);
    inline indexedVertex(const Point& p, Face_handle f);
    inline indexedVertex(Face_handle f);
  // Member Functions
    inline int& index();
    inline int index() const;
    inline int& type();
    inline int type() const;
    //- Is point a far-point
    inline bool farPoint() const;
    //- Is point internal, i.e. not on boundary
    inline bool internalPoint() const;
    //- Is point internal and near the boundary
    inline bool nearBoundary() const;
    //- Set the point to be near the boundary
    inline void setNearBoundary();
    //- Is point a mirror point
    inline bool mirrorPoint() const;
    //- Either master or slave of pointPair.
    inline bool pairPoint() const;
    //- Master of a pointPair is the lowest numbered one.
    inline bool ppMaster() const;
    //- Slave of a pointPair is the highest numbered one.
    inline bool ppSlave() const;
    //- Either original internal point or master of pointPair.
    inline bool internalOrBoundaryPoint() const;
    //- Is point near the boundary or part of the boundary definition
    inline bool nearOrOnBoundary() const;
  // Friend Functions
    //- Do the two given vertices consitute a boundary point-pair
    friend bool pointPair <Gt, Vb>
    (
      const indexedVertex<Gt, Vb>& v0,
      const indexedVertex<Gt, Vb>& v1
    );
    //- Do the three given vertices consitute a boundary triangle
    friend bool boundaryTriangle <Gt, Vb>
    (
      const indexedVertex<Gt, Vb>& v0,
      const indexedVertex<Gt, Vb>& v1,
      const indexedVertex<Gt, Vb>& v2
    );
    //- Do the three given vertices consitute an outside triangle
    friend bool outsideTriangle <Gt, Vb>
    (
      const indexedVertex<Gt, Vb>& v0,
      const indexedVertex<Gt, Vb>& v1,
      const indexedVertex<Gt, Vb>& v2
    );
};

}  // namespace CGAL


// Constructors 
template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex()
:
  Vb{},
  index_{INTERNAL_POINT},
  type_{INTERNAL_POINT}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex(const Point& p)
:
  Vb{p},
  index_{INTERNAL_POINT},
  type_{INTERNAL_POINT}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex
(
  const Point& p,
  const int index,
  const int& type
)
:
  Vb{p},
  index_{index},
  type_{type}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex(const Point& p, Face_handle f)
:
  Vb{f, p},
  index_{INTERNAL_POINT},
  type_{INTERNAL_POINT}
{}


template<class Gt, class Vb>
inline CGAL::indexedVertex<Gt, Vb>::indexedVertex(Face_handle f)
:
  Vb{f},
  index_{INTERNAL_POINT},
  type_{INTERNAL_POINT}
{}


// Member Functions 
template<class Gt, class Vb>
inline int& CGAL::indexedVertex<Gt, Vb>::index()
{
  return index_;
}


template<class Gt, class Vb>
inline int CGAL::indexedVertex<Gt, Vb>::index() const
{
  return index_;
}


template<class Gt, class Vb>
inline int& CGAL::indexedVertex<Gt, Vb>::type()
{
  return type_;
}


template<class Gt, class Vb>
inline int CGAL::indexedVertex<Gt, Vb>::type() const
{
  return type_;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::farPoint() const
{
  return type_ == FAR_POINT;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::internalPoint() const
{
  return type_ <= INTERNAL_POINT;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::nearBoundary() const
{
  return type_ == NEAR_BOUNDARY_POINT;
}


template<class Gt, class Vb>
inline void CGAL::indexedVertex<Gt, Vb>::setNearBoundary()
{
  type_ = NEAR_BOUNDARY_POINT;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::mirrorPoint() const
{
  return type_ == MIRROR_POINT;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::pairPoint() const
{
  return type_ >= 0;
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::ppMaster() const
{
  return (type_ > index_);
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::ppSlave() const
{
  return (type_ >= 0 && type_ < index_);
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::internalOrBoundaryPoint() const
{
  return internalPoint() || ppMaster();
}


template<class Gt, class Vb>
inline bool CGAL::indexedVertex<Gt, Vb>::nearOrOnBoundary() const
{
  return pairPoint() || mirrorPoint() || nearBoundary();
}


// Friend Functions 
template<class Gt, class Vb>
bool CGAL::pointPair
(
  const indexedVertex<Gt, Vb>& v0,
  const indexedVertex<Gt, Vb>& v1
)
{
  return v0.index_ == v1.type_ || v1.index_ == v0.type_;
}


template<class Gt, class Vb>
bool CGAL::boundaryTriangle
(
  const indexedVertex<Gt, Vb>& v0,
  const indexedVertex<Gt, Vb>& v1,
  const indexedVertex<Gt, Vb>& v2
)
{
  return (v0.pairPoint() && pointPair(v1, v2))
    || (v1.pairPoint() && pointPair(v2, v0))
    || (v2.pairPoint() && pointPair(v0, v1));
}


template<class Gt, class Vb>
bool CGAL::outsideTriangle
(
  const indexedVertex<Gt, Vb>& v0,
  const indexedVertex<Gt, Vb>& v1,
  const indexedVertex<Gt, Vb>& v2
)
{
  return (v0.farPoint() || v0.ppSlave())
    || (v1.farPoint() || v1.ppSlave())
    || (v2.farPoint() || v2.ppSlave());
}

#endif
