#ifndef DYNAMIC_FV_MESH_DYNAMIC_FV_MESH_DYNAMIC_FV_MESH_HPP_
#define DYNAMIC_FV_MESH_DYNAMIC_FV_MESH_DYNAMIC_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicFvMesh
// Description
//   Abstract base class for geometry and/or topology changing fvMesh.

#include "fv_mesh.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class dynamicFvMesh
:
  public fvMesh
{
public:
  //- Runtime type information
  TYPE_NAME("dynamicFvMesh");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      dynamicFvMesh,
      IOobject,
      (const IOobject& io),
      (io)
    );
  // Constructors
    //- Construct from objectRegistry, and read/write options
    explicit dynamicFvMesh(const IOobject& io);
    //- Construct from components without boundary.
    //  Boundary is added using addFvPatches() member function
    dynamicFvMesh
    (
      const IOobject& io,
      const Xfer<pointField>& points,
      const Xfer<faceList>& faces,
      const Xfer<labelList>& allOwner,
      const Xfer<labelList>& allNeighbour,
      const bool syncPar = true
    );
    //- Construct without boundary from cells rather than owner/neighbour.
    //  Boundary is added using addPatches() member function
    dynamicFvMesh
    (
      const IOobject& io,
      const Xfer<pointField>& points,
      const Xfer<faceList>& faces,
      const Xfer<cellList>& cells,
      const bool syncPar = true
    );
    //- Disallow default bitwise copy construct
    dynamicFvMesh(const dynamicFvMesh&) = delete;
    //- Disallow default bitwise assignment
    dynamicFvMesh& operator=(const dynamicFvMesh&) = delete;
  // Selectors
    //- Select null constructed
    static autoPtr<dynamicFvMesh> New(const IOobject& io);
  //- Destructor
  virtual ~dynamicFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update() = 0;
};

}  // namespace mousse

#endif

