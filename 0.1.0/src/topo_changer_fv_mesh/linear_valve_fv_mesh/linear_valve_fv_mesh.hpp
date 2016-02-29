#ifndef TOPO_CHANGER_FV_MESH_LINEAR_VALVE_FV_MESH_LINEAR_VALVE_FV_MESH_HPP_
#define TOPO_CHANGER_FV_MESH_LINEAR_VALVE_FV_MESH_LINEAR_VALVE_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearValveFvMesh
// Description
//   A sliding linear valve
// SourceFiles
//   linear_valve_fv_mesh.cpp
#include "topo_changer_fv_mesh.hpp"
#include "motion_solver.hpp"
namespace mousse
{
class linearValveFvMesh
:
  public topoChangerFvMesh
{
  // Private data
    //- Motion dictionary
    dictionary motionDict_;
    //- Motion solver
    autoPtr<motionSolver> msPtr_;
  // Private Member Functions
    //- Add linearValve zones and modifiers
    void addZonesAndModifiers();
    //- Make sliding modifiers live
    void makeSlidersDead();
    //- Make sliding modifiers live
    void makeSlidersLive();
    //- Return true if sliders are attached
    bool attached() const;
public:
  //- Runtime type information
  TYPE_NAME("linearValveFvMesh");
  // Constructors
    //- Construct from database
    explicit linearValveFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    linearValveFvMesh(const linearValveFvMesh&) = delete;
    //- Disallow default bitwise assignment
    linearValveFvMesh& operator=(const linearValveFvMesh&) = delete;
  //- Destructor
  virtual ~linearValveFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
