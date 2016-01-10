// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::EdgeMap
// Description
//   Map from edge (expressed as its endpoints) to value
#ifndef edge_map_hpp_
#define edge_map_hpp_
#include "hash_table.hpp"
#include "edge.hpp"
namespace mousse
{
template<class T>
class EdgeMap
:
  public HashTable<T, edge, Hash<edge> >
{
public:
  // Constructors
    //- Construct given initial map size
    EdgeMap(const label size = 128)
    :
      HashTable<T, edge, Hash<edge> >(size)
    {}
    //- Construct from Istream
    EdgeMap(Istream& is)
    :
      HashTable<T, edge, Hash<edge> >(is)
    {}
    //- Construct as copy
    EdgeMap(const EdgeMap<T>& map)
    :
      HashTable<T, edge, Hash<edge> >(map)
    {}
};
}  // namespace mousse
#endif
