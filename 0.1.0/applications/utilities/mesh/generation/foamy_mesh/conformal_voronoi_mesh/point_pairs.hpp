#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_POINT_PAIRS_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_POINT_PAIRS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointPairs
// Description
//   HashSet of unique edges. The edges are stored as a pair of pairs:
//     ( (local index, processor index) (local index, processor index) )
//   e.g.,
//     ( (0 1) (3 1) )
//     ( (0 2) (5 1) )
#include "label_pair.hpp"
#include "hash_set.hpp"
namespace mousse
{
typedef HashSet
<
  Pair<labelPair>,
  FixedList<labelPair, 2>::Hash<>
> ptPairTable;
template<class Triangulation>
class pointPairs
:
  public ptPairTable
{
  // Private typedefs
    typedef typename Triangulation::Vertex_handle Vertex_handle;
  // Private data
    const Triangulation& triangulation_;
  // Private Member Functions
    inline Pair<labelPair> orderPointPair
    (
      const labelPair& vA,
      const labelPair& vB
    ) const;
    inline bool insertPointPair
    (
      const labelPair& vA,
      const labelPair& vB
    );
    inline bool findPointPair
    (
      const labelPair& vA,
      const labelPair& vB
    ) const;
public:
  // Constructors
    //- Construct from triangulation
    pointPairs(const Triangulation& triangulation);
  //- Destructor
  ~pointPairs();
  // Member Functions
    // Access
      inline bool isPointPair
      (
        const Vertex_handle& vA,
        const Vertex_handle& vB
      ) const;
      inline bool isPointPair
      (
        const labelPair& vA,
        const labelPair& vB
      ) const;
    // Edit
      inline bool addPointPair
      (
        const labelPair& vA,
        const labelPair& vB
      );
      inline bool addPointPair
      (
        const labelPair& master,
        const DynamicList<labelPair>& slaves
      );
      inline bool addPointPair
      (
        const label vA,
        const label vB
      );
      void reIndex(const Map<label>& oldToNewIndices);
};
}  // namespace mousse
#ifdef NoRepository
  #include "point_pairs.cpp"
#endif
#endif
