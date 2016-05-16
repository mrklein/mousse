#ifndef ENGINE_ENGINE_MESH_ENGINE_MESH_ENGINE_MESH_HPP_
#define ENGINE_ENGINE_MESH_ENGINE_MESH_ENGINE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::engineMesh
// Description
//   mousse::engineMesh

#include "engine_time.hpp"
#include "fv_mesh.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

class engineMesh
:
  public fvMesh
{
protected:
  // Protected data
    const engineTime& engineDB_;
    label pistonIndex_;
    label linerIndex_;
    label cylinderHeadIndex_;
    dimensionedScalar deckHeight_;
    dimensionedScalar pistonPosition_;
public:
  //- Runtime type information
  TYPE_NAME("engineMesh");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      engineMesh,
      IOobject,
      (const IOobject& io),
      (io)
    );
  // Constructors
    //- Construct from objectRegistry, and read/write options
    explicit engineMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    engineMesh(const engineMesh&) = delete;
    //- Disallow default bitwise assignment
    engineMesh& operator=(const engineMesh&) = delete;
  // Selectors
    //- Select null constructed
    static autoPtr<engineMesh> New(const IOobject& io);
  //- Destructor
  virtual ~engineMesh();
  // Member Functions
    // Edit
      virtual void move() = 0;
};

}  // namespace mousse

#endif

