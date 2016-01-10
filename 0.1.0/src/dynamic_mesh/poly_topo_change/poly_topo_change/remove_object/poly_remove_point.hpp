// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyRemovePoint
// Description
//   Class containing data for point removal.
#ifndef poly_remove_point_hpp_
#define poly_remove_point_hpp_
#include "label.hpp"
#include "topo_action.hpp"
namespace mousse
{
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
      pointID_(-1),
      mergePointID_(-1)
    {}
    //- Construct from components
    polyRemovePoint(const label pointID, const label mergePointID = -1)
    :
      pointID_(pointID),
      mergePointID_(mergePointID)
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>(new polyRemovePoint(*this));
    }
  // Default Destructor
  // Member Functions
    //- Return point ID
    label pointID() const
    {
      return pointID_;
    }
    label mergePointID() const
    {
      return mergePointID_;
    }
};
}  // namespace mousse
#endif
