#ifndef DYNAMIC_MESH_MESH_CUT_MESH_MODIFIERS_UNDOABLE_MESH_CUTTER_UNDOABLE_MESH_CUTTER_HPP_
#define DYNAMIC_MESH_MESH_CUT_MESH_MODIFIERS_UNDOABLE_MESH_CUTTER_UNDOABLE_MESH_CUTTER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::undoableMeshCutter
// Description
//   The main refinement handler. Gets cellCuts which is structure that
//   describes which cells are to be cut and in what way. Maintains an undo
//   list (if told so during construction). Apart from undo list is just
//   wrapper around meshCutter.
//   Undo list: contains a refinement tree (of type splitCell; cell labels are
//   of no consequence) and a list of visible splitCells, i.e. the top of the
//   tree (where the cell labels are valid). Now every cell added gets put on
//   the tree and every updateMesh action updates the labels of visible
//   splitcells.
//   We can now ask this structure for a list of visible split cells or the list
//   of faces between these. These can be passed to removeFaces for actual
//   deletion and we delete the top splitCell and update the now newly visible
//   underlying cells for the new cell number (passed back from removeFaces).
//   NOTE: Undoing note properly tested. Expect it to fail if the faces to
//   be removed cause other faces to be additionally removed (i.e. removeFaces
//   adds additional faces to remove).
//   splitCell:
//   - original cell number.
//   - pointer to parent (null for first level splitCell)
//   - two pointers to splitCell children. Both null (unrefined=visible cell) or
//    both non-null.
//   - live are:
//     (-all unrefined cells (original cell without any splitCells))
//     -all splitCells with null children
//   - liveSplitCells contains pointers to splitCells with null children.
// SourceFiles
//   undoable_mesh_cutter.cpp
#include "edge_vertex.hpp"
#include "refine_cell.hpp"
#include "bool_list.hpp"
#include "cell_looper.hpp"
#include "mesh_cutter.hpp"
#include "map.hpp"
#include "type_info.hpp"
#include "remove_faces.hpp"
namespace mousse
{
// Forward declaration of classes
class polyMesh;
class polyTopoChange;
class refineCell;
class splitCell;
class undoableMeshCutter
:
  public meshCutter
{
  // Private data
    //- Whether or not to store actions for unplaying.
    const bool undoable_;
    //- Current split cells which are 'visible'. Only set if undoable.
    Map<splitCell*> liveSplitCells_;
    //- Face remover engine
    removeFaces faceRemover_;
  // Private Member Functions
    //- Debug print
    void printCellRefTree(Ostream& os, const word&, const splitCell*)
    const;
    //- Debug print
    void printRefTree(Ostream& os) const;
    //- Find shared face between two cells
    label sharedFace
    (
      const label cell0I,
      const label cell1I
    ) const;
    //- Update labels on splitCell structure after morphing.
    static void updateLabels(const labelList& map, Map<splitCell*>&);
public:
  //- Runtime type information
  CLASS_NAME("undoableMeshCutter");
  // Constructors
    //- Construct from mesh and flag whether refinement pattern needs
    //  to be stored.
    undoableMeshCutter(const polyMesh& mesh, const bool undoable = true);
    //- Disallow default bitwise copy construct
    undoableMeshCutter(const undoableMeshCutter&) = delete;
    //- Disallow default bitwise assignment
    undoableMeshCutter& operator=(const undoableMeshCutter&) = delete;
  //- Destructor
  ~undoableMeshCutter();
  // Member Functions
    // Access
      //- All current live split cells. Warning: cell labels will change
      //  during morphing. Only this map is guaranteed to hold uptodate
      //  info.
      const Map<splitCell*>& liveSplitCells() const
      {
        return liveSplitCells_;
      }
      const removeFaces& faceRemover() const
      {
        return faceRemover_;
      }
    // Edit
      //- Refine cells acc. to cellCuts. Plays topology changes
      //  into polyTopoChange.
      void setRefinement(const cellCuts& cuts, polyTopoChange&);
      //- Update stored refinement pattern for changes to mesh. Only
      //  call if undoable set.
      void updateMesh(const mapPolyMesh& morphMap);
      //- Calculate split faces from current liveCells. Only
      //  call if undoable set.
      labelList getSplitFaces() const;
      //- Like getSplitFaces but returns map from original to added cell.
      //  Only call if undoable set.
      Map<label> getAddedCells() const;
      //- Remove some refinement. Needs to be supplied subset of
      //  getSplitFaces() output. Returns list of faces removed
      //  (can be more or equal but never less than splitFaces - since
      //  removeFaces might decide to take down unnecessary faces)
      //  Only call if undoable set.
      labelList removeSplitFaces
      (
        const labelList& splitFaces,
        polyTopoChange&
      );
};
}  // namespace mousse
#endif
