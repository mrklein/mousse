#ifndef DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_MULTI_SOLID_BODY_MOTION_FV_MESH_HPP_
#define DYNAMIC_FV_MESH_SOLID_BODY_MOTION_FV_MESH_MULTI_SOLID_BODY_MOTION_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multiSolidBodyMotionFvMesh
// Description
//   Solid-body motion of the mesh specified by a run-time selectable
//   motion function.
// SourceFiles
//   multi_solid_body_motion_fv_mesh.cpp
#include "dynamic_fv_mesh.hpp"
#include "dictionary.hpp"
#include "point_io_field.hpp"
#include "solid_body_motion_function.hpp"
namespace mousse
{
class multiSolidBodyMotionFvMesh
:
  public dynamicFvMesh
{
  // Private data
    //- Dictionary of motion control parameters
    const dictionary dynamicMeshCoeffs_;
    //- The motion control function
    PtrList<solidBodyMotionFunction> SBMFs_;
    //- The reference points which are transformed
    pointIOField undisplacedPoints_;
    //- Specified cellZones
    labelList zoneIDs_;
    //- Points to move per cellZone
    labelListList pointIDs_;
public:
  //- Runtime type information
  TYPE_NAME("multiSolidBodyMotionFvMesh");
  // Constructors
    //- Construct from IOobject
    multiSolidBodyMotionFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    multiSolidBodyMotionFvMesh(const multiSolidBodyMotionFvMesh&) = delete;
    //- Disallow default bitwise assignment
    multiSolidBodyMotionFvMesh& operator=
    (
      const multiSolidBodyMotionFvMesh&
    ) = delete;
  //- Destructor
  ~multiSolidBodyMotionFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
