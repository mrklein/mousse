#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_INDEXED_FACE_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_FOAMY_QUAD_MESH_INDEXED_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   CGAL::indexedFace
// Description
//   An indexed form of CGAL::Triangulation_face_base_2<K> used to keep
//   track of the vertices in the triangulation.

#include "CGAL/Triangulation_2.h"
#include "indexed_vertex.hpp"


namespace CGAL {

template<class Gt, class Fb=CGAL::Triangulation_face_base_2<Gt>>
class indexedFace
:
  public Fb
{
  // Private data
    //- The index for this triangle face
    //  -1: triangle and changed and associated data needs updating
    // >=0: index of triangles, set by external update algorithm
    int index_;
public:
  enum faceTypes
  {
    UNCHANGED           = 0,
    CHANGED             = -1,
    SAVE_CHANGED        = -2
  };
  typedef typename Fb::Vertex_handle                   Vertex_handle;
  typedef typename Fb::Face_handle                     Face_handle;
  template<typename TDS2>
  struct Rebind_TDS
  {
    typedef typename Fb::template Rebind_TDS<TDS2>::Other Fb2;
    typedef indexedFace<Gt, Fb2>  Other;
  };
  // Constructors
    inline indexedFace();
    inline indexedFace
    (
      Vertex_handle v0,
      Vertex_handle v1,
      Vertex_handle v2
    );
    inline indexedFace
    (
      Vertex_handle v0,
      Vertex_handle v1,
      Vertex_handle v2,
      Face_handle n0,
      Face_handle n1,
      Face_handle n2
    );
  // Member Functions
    inline void set_vertex(int i, Vertex_handle v);
    inline void set_vertices();
    inline void set_vertices
    (
      Vertex_handle v0,
      Vertex_handle v1,
      Vertex_handle v2
    );
    inline int& faceIndex();
    inline int faceIndex() const;
};

}  // namespace CGAL


// Constructors 
template<class Gt, class Fb>
inline CGAL::indexedFace<Gt, Fb>::indexedFace()
:
  Fb{},
  index_{CHANGED}
{}


template<class Gt, class Fb>
inline CGAL::indexedFace<Gt, Fb>::indexedFace
(
  Vertex_handle v0,
  Vertex_handle v1,
  Vertex_handle v2
)
:
  Fb{v0, v1, v2},
  index_{CHANGED}
{}


template<class Gt, class Fb>
inline CGAL::indexedFace<Gt, Fb>::indexedFace
(
  Vertex_handle v0,
  Vertex_handle v1,
  Vertex_handle v2,
  Face_handle n0,
  Face_handle n1,
  Face_handle n2
)
:
  Fb{v0, v1, v2, n0, n1, n2},
  index_{CHANGED}
{}


// Member Functions 
template<class Gt, class Fb>
inline void CGAL::indexedFace<Gt, Fb>::set_vertex(int i, Vertex_handle v)
{
  index_ = CHANGED;
  Fb::set_vertex(i, v);
}


template<class Gt, class Fb>
inline void CGAL::indexedFace<Gt, Fb>::set_vertices()
{
  index_ = CHANGED;
  Fb::set_vertices();
}


template<class Gt, class Fb>
inline void CGAL::indexedFace<Gt, Fb>::set_vertices
(
  Vertex_handle v0,
  Vertex_handle v1,
  Vertex_handle v2
)
{
  index_ = CHANGED;
  Fb::set_vertices(v0, v1, v2);
}


template<class Gt, class Fb>
inline int& CGAL::indexedFace<Gt, Fb>::faceIndex()
{
  return index_;
}


template<class Gt, class Fb>
inline int CGAL::indexedFace<Gt, Fb>::faceIndex() const
{
  return index_;
}

#endif

