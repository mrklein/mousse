#ifndef TOPO_CHANGER_FV_MESH_LINEAR_VALVE_LAYERS_FV_MESH_LINEAR_VALVE_LAYERS_FV_MESH_HPP_
#define TOPO_CHANGER_FV_MESH_LINEAR_VALVE_LAYERS_FV_MESH_LINEAR_VALVE_LAYERS_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearValveLayersFvMesh
// Description
//   A sliding linear valve with layers

#include "topo_changer_fv_mesh.hpp"
#include "motion_solver.hpp"


namespace mousse {

class linearValveLayersFvMesh
:
  public topoChangerFvMesh
{
  // Private data
    //- Motion dictionary
    dictionary motionDict_;
  // Private Member Functions
    //- Add linearValveLayers zones and modifiers
    void addZonesAndModifiers();
    //- Make layering modifiers live
    void makeLayersLive();
    //- Make sliding modifiers live
    void makeSlidersLive();
    //- Return true if sliders are attached
    bool attached() const;
    //- Return new point field
    tmp<pointField> newPoints() const;
public:
  //- Runtime type information
  TYPE_NAME("linearValveLayersFvMesh");
  // Constructors
    //- Construct from database
    explicit linearValveLayersFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    linearValveLayersFvMesh(const linearValveLayersFvMesh&) = delete;
    //- Disallow default bitwise assignment
    linearValveLayersFvMesh& operator=(const linearValveLayersFvMesh&) = delete;
  //- Destructor
  virtual ~linearValveLayersFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};

}  // namespace mousse

#endif

