#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_MODIFY_OBJECT_POLY_MODIFY_POINT_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_MODIFY_OBJECT_POLY_MODIFY_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyModifyPoint
// Description
//   Class describing modification of a point.

#include "label.hpp"
#include "point.hpp"
#include "topo_action.hpp"


namespace mousse {

class polyModifyPoint
:
  public topoAction
{
  // Private data
    //- Point ID
    label pointID_;
    //- New point location
    point location_;
    //- Remove from current zone
    bool removeFromZone_;
    //- New zone ID
    label zoneID_;
    //- Does the point support a cell
    bool inCell_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("modifyPoint");
  // Constructors
    //- Construct null.  Used only for list construction
    polyModifyPoint()
    :
      pointID_{-1},
      location_{vector::zero},
      removeFromZone_{false},
      zoneID_{-1},
      inCell_{false}
    {}
    //- Construct from components
    polyModifyPoint
    (
      const label pointID,
      const point& newP,
      const bool removeFromZone,
      const label newZoneID,
      const bool inCell
    )
    :
      pointID_{pointID},
      location_{newP},
      removeFromZone_{removeFromZone},
      zoneID_{newZoneID},
      inCell_{inCell}
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>{new polyModifyPoint{*this}};
    }
  // Default Destructor
  // Member Functions
    //- Point ID
    label pointID() const { return pointID_; }
    //- New point location
    const point& newPoint() const { return location_; }
    //- Does the point belong to a zone?
    bool isInZone() const { return zoneID_ >= 0; }
    //- Should the point be removed from current zone
    bool removeFromZone() const { return removeFromZone_; }
    //- Point zone ID
    label zoneID() const { return zoneID_; }
    //- Does the point support a cell
    bool inCell() const { return inCell_; }
};

}  // namespace mousse

#endif

