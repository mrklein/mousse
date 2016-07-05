#ifndef UTILITIES_MESH_MANIPULATION_CREATE_BAFFLES_FACE_SELECTION_FACE_SELECTION_HPP_
#define UTILITIES_MESH_MANIPULATION_CREATE_BAFFLES_FACE_SELECTION_FACE_SELECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceSelection
// Description
//   Face selection method for createBaffles

#include "dictionary.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "auto_ptr.hpp"
#include "bool_list.hpp"
#include "label_list.hpp"
#include "switch.hpp"


namespace mousse {

// Forward declaration of classes
class fvMesh;


class faceSelection
{
protected:
  // Protected data
    //- Name
    const word name_;
    //- Reference to mesh
    const fvMesh& mesh_;
    //- Input dictionary
    const dictionary dict_;
    //- Switch direction?
    const Switch flip_;
public:
  //- Runtime type information
  TYPE_NAME("faceSelection");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      faceSelection,
      dictionary,
      (
        const word& name,
        const fvMesh& mesh,
        const dictionary& dict
      ),
      (name, mesh, dict)
    );
  // Constructors
    //- Construct from dictionary
    faceSelection
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
  // Selectors
    //- Return a reference to the selected faceSelection
    static autoPtr<faceSelection> New
    (
      const word& name,
      const fvMesh& mesh,
      const dictionary& dict
    );
  //- Destructor
  virtual ~faceSelection();
  // Member Functions
    const word& name() const
    {
      return name_;
    }
    const dictionary& dict() const
    {
      return dict_;
    }
    virtual void select(const label, labelList&, boolList&) const = 0;
};

}  // namespace mousse

#endif

