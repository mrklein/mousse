// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sloan_renumber.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "decomposition_method.hpp"
#include "processor_poly_patch.hpp"
#include "sync_tools.hpp"
#include <boost/config.hpp>
#include <vector>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/graph/sloan_ordering.hpp>
#pragma GCC diagnostic pop
#include <boost/graph/properties.hpp>
#include <boost/graph/bandwidth.hpp>
#include <boost/graph/profile.hpp>
#include <boost/graph/wavefront.hpp>
using namespace boost;
using namespace std;
//Defining the graph type
typedef adjacency_list
<
  setS,
  vecS,
  undirectedS,
  property
  <
    vertex_color_t,
    default_color_type,
    property
    <
      vertex_degree_t,
      mousse::label,
      property
      <
        vertex_priority_t,
        mousse::scalar
      >
    >
  >
> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::vertices_size_type size_type;
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(SloanRenumber, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    renumberMethod,
    SloanRenumber,
    dictionary
  );
}
// Constructors 
mousse::SloanRenumber::SloanRenumber(const dictionary& renumberDict)
:
  renumberMethod(renumberDict),
  reverse_
  (
    renumberDict.found(typeName + "Coeffs")
   ? Switch(renumberDict.subDict(typeName + "Coeffs").lookup("reverse"))
   : Switch(false)
  )
{}
// Member Functions 
mousse::labelList mousse::SloanRenumber::renumber
(
  const polyMesh& mesh,
  const pointField& /*points*/
) const
{
  const polyBoundaryMesh& pbm = mesh.boundaryMesh();
  // Construct graph : faceOwner + connections across cyclics.
  // Determine neighbour cell
  labelList nbr(mesh.nFaces()-mesh.nInternalFaces(), -1);
  FOR_ALL(pbm, patchI)
  {
    if (pbm[patchI].coupled() && !isA<processorPolyPatch>(pbm[patchI]))
    {
      SubList<label>
      (
        nbr,
        pbm[patchI].size(),
        pbm[patchI].start()-mesh.nInternalFaces()
      ).assign(pbm[patchI].faceCells());
    }
  }
  syncTools::swapBoundaryFaceList(mesh, nbr);
  Graph G(mesh.nCells());
  // Add internal faces
  FOR_ALL(mesh.faceNeighbour(), faceI)
  {
    add_edge(mesh.faceOwner()[faceI], mesh.faceNeighbour()[faceI], G);
  }
  // Add cyclics
  FOR_ALL(pbm, patchI)
  {
    if
    (
      pbm[patchI].coupled()
    && !isA<processorPolyPatch>(pbm[patchI])
    &&  refCast<const coupledPolyPatch>(pbm[patchI]).owner()
    )
    {
      const labelUList& faceCells = pbm[patchI].faceCells();
      FOR_ALL(faceCells, i)
      {
        label bFaceI = pbm[patchI].start()+i-mesh.nInternalFaces();
        label nbrCellI = nbr[bFaceI];
        if (faceCells[i] < nbrCellI)
        {
          add_edge(faceCells[i], nbrCellI, G);
        }
        else
        {
          add_edge(nbrCellI, faceCells[i], G);
        }
      }
    }
  }
  //Creating two iterators over the vertices
  graph_traits<Graph>::vertex_iterator ui, ui_end;
  //Creating a property_map with the degrees of the degrees of each vertex
  property_map<Graph,vertex_degree_t>::type deg = get(vertex_degree, G);
  for (boost::tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
    deg[*ui] = degree(*ui, G);
  //Creating a property_map for the indices of a vertex
  property_map<Graph, vertex_index_t>::type index_map = get(vertex_index, G);
  //Creating a vector of vertices
  std::vector<Vertex> sloan_order(num_vertices(G));
  sloan_ordering
  (
    G,
    sloan_order.begin(),
    get(vertex_color, G),
    make_degree_map(G),
    get(vertex_priority, G)
  );
  labelList orderedToOld(sloan_order.size());
  FOR_ALL(orderedToOld, c)
  {
    orderedToOld[c] = index_map[sloan_order[c]];
  }
  if (reverse_)
  {
    reverse(orderedToOld);
  }
  return orderedToOld;
}
mousse::labelList mousse::SloanRenumber::renumber
(
  const labelListList& cellCells,
  const pointField& /*points*/
) const
{
  Graph G(cellCells.size());
  FOR_ALL(cellCells, cellI)
  {
    const labelList& nbrs = cellCells[cellI];
    FOR_ALL(nbrs, i)
    {
      if (nbrs[i] > cellI)
      {
        add_edge(cellI, nbrs[i], G);
      }
    }
  }
  //Creating two iterators over the vertices
  graph_traits<Graph>::vertex_iterator ui, ui_end;
  //Creating a property_map with the degrees of the degrees of each vertex
  property_map<Graph,vertex_degree_t>::type deg = get(vertex_degree, G);
  for (boost::tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
    deg[*ui] = degree(*ui, G);
  //Creating a property_map for the indices of a vertex
  property_map<Graph, vertex_index_t>::type index_map = get(vertex_index, G);
  //Creating a vector of vertices
  std::vector<Vertex> sloan_order(num_vertices(G));
  sloan_ordering
  (
    G,
    sloan_order.begin(),
    get(vertex_color, G),
    make_degree_map(G),
    get(vertex_priority, G)
  );
  labelList orderedToOld(sloan_order.size());
  FOR_ALL(orderedToOld, c)
  {
    orderedToOld[c] = index_map[sloan_order[c]];
  }
  if (reverse_)
  {
    reverse(orderedToOld);
  }
  return orderedToOld;
}
