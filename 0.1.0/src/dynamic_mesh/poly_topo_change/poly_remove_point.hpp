#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_OBJECT_POLY_REMOVE_POINT_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_OBJECT_POLY_REMOVE_POINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyRemovePoint
// Description
//   Class containing data for point removal.

#include "label.hpp"
#include "topo_action.hpp"


namespace mousse {

class polyRemovePoint
:
  public topoAction
{
  // Private data
    //- Point ID
    label pointID_;
    //- Merge point ID or -1
    label mergePointID_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("removePoint");
  // Constructors
    //- Construct null.  Used for constructing lists
    polyRemovePoint()
    :
      pointID_{-1},
      mergePointID_{-1}
    {}
    //- Construct from components
    polyRemovePoint(const label pointID, const label mergePointID = -1)
    :
      pointID_{pointID},
      mergePointID_{mergePointID}
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>{new polyRemovePoint{*this}};
    }
  // Default Destructor
  // Member Functions
    //- Return point ID
    label pointID() const { return pointID_; }
    label mergePointID() const { return mergePointID_; }
};

}  // namespace mousse

#endif

