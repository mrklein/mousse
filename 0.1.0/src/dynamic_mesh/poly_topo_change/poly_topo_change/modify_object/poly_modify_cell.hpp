// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyModifyCell
// Description
//   Class describing modification of a cell.
#ifndef poly_modify_cell_hpp_
#define poly_modify_cell_hpp_
#include "label.hpp"
#include "topo_action.hpp"
namespace mousse
{
class polyModifyCell
:
  public topoAction
{
  // Private data
    //- Cell ID
    label cellID_;
    //- Remove from current zone
    bool removeFromZone_;
    //- New zone ID
    label zoneID_;
public:
  // Static data members
    //- Runtime type information
    TypeName("modifyCell");
  // Constructors
    //- Construct null.  Used only for list construction
    polyModifyCell()
    :
      cellID_(-1),
      removeFromZone_(false),
      zoneID_(-1)
    {}
    //- Construct from components
    polyModifyCell
    (
      const label cellID,
      const bool removeFromZone,
      const label newZoneID
    )
    :
      cellID_(cellID),
      removeFromZone_(removeFromZone),
      zoneID_(newZoneID)
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>(new polyModifyCell(*this));
    }
  // Default Destructor
  // Member Functions
    //- Cell ID
    label cellID() const
    {
      return cellID_;
    }
    //- Does the cell belong to a zone?
    bool isInZone() const
    {
      return zoneID_ >= 0;
    }
    bool removeFromZone() const
    {
      return removeFromZone_;
    }
    //- Cell zone ID
    label zoneID() const
    {
      return zoneID_;
    }
};
}  // namespace mousse
#endif
