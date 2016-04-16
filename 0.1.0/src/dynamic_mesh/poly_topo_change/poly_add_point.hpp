#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_ADD_OBJECT_POLY_ADD_POINT_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_ADD_OBJECT_POLY_ADD_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyAddPoint
// Description
//   Class containing data for point addition.

#include "label.hpp"
#include "point.hpp"
#include "topo_action.hpp"


namespace mousse {

class polyAddPoint
:
  public topoAction
{
  // Private data
    //- Point to add
    point p_;
    //- Master point
    label masterPointID_;
    //- Point zone ID
    label zoneID_;
    //- Does the point support a cell
    bool inCell_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("addPoint");
  // Constructors
    //- Construct null.  Used only for list construction
    polyAddPoint()
    :
      p_{vector::zero},
      masterPointID_{-1},
      zoneID_{-1},
      inCell_{false}
    {}
    //- Construct from components
    polyAddPoint
    (
      const point& p,
      const label masterPointID,
      const label zoneID,
      const bool inCell
    )
    :
      p_{p},
      masterPointID_{masterPointID},
      zoneID_{zoneID},
      inCell_{inCell}
    {
      if (zoneID_ < 0 && !inCell) {
        FATAL_ERROR_IN
        (
          "polyAddPoint\n"
          "(\n"
          "    const point& p,\n"
          "    const label masterPointID,\n"
          "    const label zoneID,\n"
          "    const bool inCell\n"
          ")"
        )
        << "Point is not in a cell and not in a zone.  "
        << "This is not allowed.\n"
        << "point: " << p
        << " master: " << masterPointID_
        << " zone: " << zoneID_
        << abort(FatalError);
      }
    }
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>{new polyAddPoint{*this}};
    }
  // Default Destructor
  // Member Functions
    //- Point location
    const point& newPoint() const { return p_; }
    //- Master point label
    label masterPointID() const { return masterPointID_; }
    //- Is the point appended with no master
    bool appended() const { return masterPointID_ < 0; }
    //- Does the point belong to a zone?
    bool isInZone() const { return zoneID_ >= 0; }
    //- Point zone ID
    label zoneID() const { return zoneID_; }
    //- Does the point support a cell
    bool inCell() const { return inCell_; }
};

}  // namespace mousse

#endif

