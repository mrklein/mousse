// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyRemoveCell
// Description
//   Class containing data for cell removal.
#ifndef poly_remove_cell_hpp_
#define poly_remove_cell_hpp_
#include "label.hpp"
#include "topo_action.hpp"
namespace mousse
{
class polyRemoveCell
:
  public topoAction
{
  // Private data
    //- Cell ID
    label cellID_;
    //- Merge cell ID or -1
    label mergeCellID_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("removeCell");
  // Constructors
    //- Construct null.  Used for constructing lists
    polyRemoveCell()
    :
      cellID_(-1),
      mergeCellID_(-1)
    {}
    //- Construct from components
    polyRemoveCell(const label cellID, const label mergeCellID = -1)
    :
      cellID_(cellID),
      mergeCellID_(mergeCellID)
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>(new polyRemoveCell(*this));
    }
  // Default Destructor
  // Member Functions
    //- Return cell ID
    label cellID() const
    {
      return cellID_;
    }
    //- Return cell ID
    label mergeCellID() const
    {
      return mergeCellID_;
    }
};
}  // namespace mousse
#endif
