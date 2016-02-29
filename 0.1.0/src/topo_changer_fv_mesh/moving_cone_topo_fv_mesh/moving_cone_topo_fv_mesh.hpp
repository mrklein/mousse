#ifndef TOPO_CHANGER_FV_MESH_MOVING_CONE_TOPO_FV_MESH_MOVING_CONE_TOPO_FV_MESH_HPP_
#define TOPO_CHANGER_FV_MESH_MOVING_CONE_TOPO_FV_MESH_MOVING_CONE_TOPO_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::movingConeTopoFvMesh
// Description
//   Sample topoChangerFvMesh that moves an object in x direction
//   and introduces/removes layers.
// SourceFiles
//   moving_cone_topo_fv_mesh.cpp
#include "topo_changer_fv_mesh.hpp"
#include "motion_solver.hpp"
namespace mousse
{
// Forward declaration of classes
class movingConeTopoFvMesh
:
  public topoChangerFvMesh
{
  // Private data
    //- Motion dictionary
    dictionary motionDict_;
    //- Motion velocity amplitude
    vector motionVelAmplitude_;
    //- Motion velocity period
    scalar motionVelPeriod_;
    //- Motion velocity period
    vector curMotionVel_;
    //- Left edge
    scalar leftEdge_;
    //- Current left obstacle position
    scalar curLeft_;
    //- Current right obstacle position
    scalar curRight_;
    //- Vertex motion mask
    scalarField motionMask_;
  // Private Member Functions
    //- Add mixer zones and modifiers
    void addZonesAndModifiers();
    //- Markup motion vertices
    tmp<scalarField> vertexMarkup
    (
      const pointField& p,
      const scalar curLeft,
      const scalar curRight
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("movingConeTopoFvMesh");
  // Constructors
    //- Construct from database
    explicit movingConeTopoFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    movingConeTopoFvMesh(const movingConeTopoFvMesh&) = delete;
    //- Disallow default bitwise assignment
    movingConeTopoFvMesh& operator=(const movingConeTopoFvMesh&) = delete;
  //- Destructor
  virtual ~movingConeTopoFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
