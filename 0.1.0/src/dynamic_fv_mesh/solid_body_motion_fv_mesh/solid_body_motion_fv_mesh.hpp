// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidBodyMotionFvMesh
// Description
//   Solid-body motion of the mesh specified by a run-time selectable
//   motion function.
// SourceFiles
//   solid_body_motion_fv_mesh.cpp
#ifndef solid_body_motion_fv_mesh_hpp_
#define solid_body_motion_fv_mesh_hpp_
#include "dynamic_fv_mesh.hpp"
#include "dictionary.hpp"
#include "point_io_field.hpp"
#include "solid_body_motion_function.hpp"
namespace mousse
{
class solidBodyMotionFvMesh
:
  public dynamicFvMesh
{
  // Private data
    //- Dictionary of motion control parameters
    const dictionary dynamicMeshCoeffs_;
    //- The motion control function
    autoPtr<solidBodyMotionFunction> SBMFPtr_;
    //- The reference points which are transformed
    pointIOField undisplacedPoints_;
    //- Points to move when cell zone is supplied
    labelList pointIDs_;
    //- Flag to indicate whether all cells should move
    bool moveAllCells_;
    //- Name of velocity field
    word UName_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    solidBodyMotionFvMesh(const solidBodyMotionFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const solidBodyMotionFvMesh&);
public:
  //- Runtime type information
  TypeName("solidBodyMotionFvMesh");
  // Constructors
    //- Construct from IOobject
    solidBodyMotionFvMesh(const IOobject& io);
  //- Destructor
  ~solidBodyMotionFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
