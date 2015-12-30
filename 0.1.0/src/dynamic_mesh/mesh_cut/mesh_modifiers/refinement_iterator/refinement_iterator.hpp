// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::refinementIterator
// Description
//   Utility class to do iterating meshCutter until all requests satisfied.
//   Needed since cell cutting can only cut cell once in one go so if
//   refinement pattern is not compatible on a cell by cell basis it will
//   refuse to cut.
//   Parallel: communicates. All decisions done on 'reduce'd variable.
// SourceFiles
//   refinement_iterator.cpp
#ifndef refinement_iterator_hpp_
#define refinement_iterator_hpp_
#include "edge_vertex.hpp"
#include "label_list.hpp"
#include "map.hpp"
#include "type_info.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class refineCell;
class undoableMeshCutter;
class cellLooper;
class refinementIterator
:
  public edgeVertex
{
  // Private data
    //- Reference to mesh
    polyMesh& mesh_;
    //- Reference to refinementEngine
    undoableMeshCutter& meshRefiner_;
    //- Reference to object to walk individual cells
    const cellLooper& cellWalker_;
    //- Whether to write intermediate meshes
    bool writeMesh_;
public:
  //- Runtime type information
  ClassName("refinementIterator");
  // Constructors
    //- Construct from mesh, refinementEngine and cell walking routine.
    //  If writeMesh = true increments runTime and writes intermediate
    //  meshes.
    refinementIterator
    (
      polyMesh& mesh,
      undoableMeshCutter& meshRefiner,
      const cellLooper& cellWalker,
      const bool writeMesh = false
    );
  //- Destructor
  ~refinementIterator();
  // Member Functions
    //- Try to refine cells in given direction. Constructs intermediate
    //  meshes. Returns map from old to added cells.
    Map<label> setRefinement(const List<refineCell>&);
};
}  // namespace mousse
#endif
