// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearValveLayersFvMesh
// Description
//   A sliding linear valve with layers
// SourceFiles
//   linear_valve_fv_mesh.cpp
#ifndef linear_valve_layers_fv_mesh_hpp_
#define linear_valve_layers_fv_mesh_hpp_
#include "topo_changer_fv_mesh.hpp"
#include "motion_solver.hpp"
namespace mousse
{
class linearValveLayersFvMesh
:
  public topoChangerFvMesh
{
  // Private data
    //- Motion dictionary
    dictionary motionDict_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    linearValveLayersFvMesh(const linearValveLayersFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const linearValveLayersFvMesh&);
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
  TypeName("linearValveLayersFvMesh");
  // Constructors
    //- Construct from database
    explicit linearValveLayersFvMesh(const IOobject& io);
  //- Destructor
  virtual ~linearValveLayersFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
