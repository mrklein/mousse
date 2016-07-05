#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_CELLS_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_CELLS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::removeCells
// Description
//   Given list of cells to remove insert all the topology changes.
//   Works in two passes:
//   - get faces that will become boundary faces
//   - given these faces and the patches they have to go into make the
//    changes.

#include "label_list.hpp"
#include "type_info.hpp"


namespace mousse {

// Forward declaration of classes
class polyMesh;
class polyTopoChange;
class mapPolyMesh;


class removeCells
{
  // Private data
    //- Reference to mesh
    const polyMesh& mesh_;
    //- Whether or not to synchronize parallel case.
    const bool syncPar_;
  // Private Member Functions
    //- Decrease count of elements of f
    static void uncount
    (
      const labelList& f,
      labelList& nUsage
    );
public:
  //- Runtime type information
  CLASS_NAME("removeCells");
  // Constructors
    //- Construct from mesh. syncPar: do parallel synchronization.
    removeCells(const polyMesh& mesh, const bool syncPar = true);
  // Member Functions
    // Topology changes
      //- Get labels of exposed faces.
      //  These are
      //  - internal faces that become boundary faces
      //  - coupled faces that become uncoupled (since one of the sides
      //    gets deleted)
      labelList getExposedFaces(const labelList& cellsToRemove) const;
      //- Play commands into polyTopoChange to remove cells.
      //  patchIDs is for every element in facesToExpose (see above) the
      //  patch it has to go into. This cannot be a coupled patch!
      void setRefinement
      (
        const labelList& cellsToRemove,
        const labelList& facesToExpose,
        const labelList& patchIDs,
        polyTopoChange&
      ) const;
      //- Force recalculation of locally stored data on topological change
      void updateMesh(const mapPolyMesh&)
      {}
};

}  // namespace mousse

#endif

