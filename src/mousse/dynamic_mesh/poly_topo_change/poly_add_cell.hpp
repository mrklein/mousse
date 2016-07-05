#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_ADD_OBJECT_POLY_ADD_CELL_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_ADD_OBJECT_POLY_ADD_CELL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyAddCell
// Description
//   Class containing data for cell addition.

#include "label.hpp"
#include "topo_action.hpp"


namespace mousse {

class polyAddCell
:
  public topoAction
{
  // Private data
    //- Master point ID for cells blown up from points
    label masterPointID_;
    //- Master edge ID for cells blown up from edges
    label masterEdgeID_;
    //- Master face ID for cells blown up from faces
    label masterFaceID_;
    //- Master cell ID for cells blown up from cells
    label masterCellID_;
    //- Cell zone ID
    label zoneID_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("addCell");
  // Constructors
    //- Construct null.  Used for constructing lists
    polyAddCell()
    :
      masterPointID_{-1},
      masterEdgeID_{-1},
      masterFaceID_{-1},
      masterCellID_{-1},
      zoneID_{-1}
    {}
    //- Construct from components
    polyAddCell
    (
      const label masterPointID,
      const label masterEdgeID,
      const label masterFaceID,
      const label masterCellID,
      const label zoneID
    )
    :
      masterPointID_{masterPointID},
      masterEdgeID_{masterEdgeID},
      masterFaceID_{masterFaceID},
      masterCellID_{masterCellID},
      zoneID_{zoneID}
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>{new polyAddCell{*this}};
    }
  // Default Destructor
  // Member Functions
    //- Is the cell mastered by a point
    bool isPointMaster() const { return masterPointID_ >= 0; }
    //- Is the cell mastered by an edge
    bool isEdgeMaster() const { return masterEdgeID_ >= 0; }
    //- Is the cell mastered by another face
    bool isFaceMaster() const { return masterFaceID_ >= 0; }
    //- Is the cell mastered by another cell
    bool isCellMaster() const { return masterCellID_ >= 0; }
    //- Is the cell appended with no master
    bool appended() const
    {
      return !isPointMaster() && !isEdgeMaster()
        && !isFaceMaster() && !isCellMaster();
    }
    //- Return master point ID
    label masterPointID() const { return masterPointID_; }
    //- Return master edge ID
    label masterEdgeID() const { return masterEdgeID_; }
    //- Return master face ID
    label masterFaceID() const { return masterFaceID_; }
    //- Return master cell ID
    label masterCellID() const { return masterCellID_; }
    //- Does the cell belong to a zone?
    bool isInZone() const { return zoneID_ >= 0; }
    //- Cell zone ID
    label zoneID() const { return zoneID_; }
};

}  // namespace mousse

#endif

