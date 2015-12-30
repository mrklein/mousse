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
#ifndef multi_solid_body_motion_fv_mesh_hpp_
#define multi_solid_body_motion_fv_mesh_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    multiSolidBodyMotionFvMesh(const multiSolidBodyMotionFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const multiSolidBodyMotionFvMesh&);
public:
  //- Runtime type information
  TypeName("multiSolidBodyMotionFvMesh");
  // Constructors
    //- Construct from IOobject
    multiSolidBodyMotionFvMesh(const IOobject& io);
  //- Destructor
  ~multiSolidBodyMotionFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
