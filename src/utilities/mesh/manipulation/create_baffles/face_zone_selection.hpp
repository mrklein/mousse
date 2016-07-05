#ifndef UTILITIES_MESH_MANIPULATION_CREATE_BAFFLES_FACE_SELECTION_FACE_ZONE_SELECTION_HPP_
#define UTILITIES_MESH_MANIPULATION_CREATE_BAFFLES_FACE_SELECTION_FACE_ZONE_SELECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceSelections::faceZoneSelection
// Description
//   Select faces from faceZone

#include "face_selection.hpp"
#include "point_field.hpp"
#include "bool_list.hpp"
#include "vol_fields.hpp"


namespace mousse {

class regionSplit;

namespace faceSelections {

class faceZoneSelection
:
  public faceSelection
{
  // Private data
    //- Name of faceZone
    const word zoneName_;
 // Private Member Functions
public:
  //- Runtime type information
  TYPE_NAME("faceZone");
  // Constructors
    //- Construct from dictionary
    faceZoneSelection
    (
      const word& name,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Clone
    autoPtr<faceSelection> clone() const
    {
      NOT_IMPLEMENTED("autoPtr<faceSelection> clone() const");
      return autoPtr<faceSelection>{nullptr};
    }
  //- Destructor
  virtual ~faceZoneSelection();
  // Member Functions
    //- Apply this selector
    virtual void select(const label zoneID, labelList&, boolList&) const;
};

}  // namespace faceSelections
}  // namespace mousse

#endif

