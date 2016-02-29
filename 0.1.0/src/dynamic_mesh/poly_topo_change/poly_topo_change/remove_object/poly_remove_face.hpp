#ifndef DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_OBJECT_POLY_REMOVE_FACE_HPP_
#define DYNAMIC_MESH_POLY_TOPO_CHANGE_POLY_TOPO_CHANGE_REMOVE_OBJECT_POLY_REMOVE_FACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyRemoveFace
// Description
//   Class containing data for face removal.
#include "label.hpp"
#include "topo_action.hpp"
namespace mousse
{
class polyRemoveFace
:
  public topoAction
{
  // Private data
    //- Face ID
    label faceID_;
    //- Merge faceID or -1
    label mergeFaceID_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("removeFace");
  // Constructors
    //- Construct null.  Used for constructing lists
    polyRemoveFace()
    :
      faceID_(-1),
      mergeFaceID_(-1)
    {}
    //- Construct from components
    polyRemoveFace(const label faceID, const label mergeFaceID = -1)
    :
      faceID_(faceID),
      mergeFaceID_(mergeFaceID)
    {}
    //- Construct and return a clone
    virtual autoPtr<topoAction> clone() const
    {
      return autoPtr<topoAction>(new polyRemoveFace(*this));
    }
  // Default Destructor
  // Member Functions
    //- Return face ID
    label faceID() const
    {
      return faceID_;
    }
    //- Return merge face ID
    label mergeFaceID() const
    {
      return mergeFaceID_;
    }
};
}  // namespace mousse
#endif
