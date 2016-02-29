#ifndef TOPO_CHANGER_FV_MESH_MIXER_FV_MESH_MIXER_FV_MESH_HPP_
#define TOPO_CHANGER_FV_MESH_MIXER_FV_MESH_MIXER_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixerFvMesh
// Description
//   A rotating slider mesh
// SourceFiles
//   mixer_fv_mesh.cpp
#include "topo_changer_fv_mesh.hpp"
#include "cylindrical_cs.hpp"
namespace mousse
{
// Forward declaration of classes
class mixerFvMesh
:
  public topoChangerFvMesh
{
  // Private data
    //- Motion dictionary
    dictionary motionDict_;
    //- Coordinate system
    autoPtr<coordinateSystem> csPtr_;
    // - Rotational speed in rotations per minute (rpm)
    scalar rpm_;
    //- Markup field for points.  Moving points marked with 1
    mutable scalarField* movingPointsMaskPtr_;
  // Private Member Functions
    //- Add mixer zones and modifiers
    void addZonesAndModifiers();
    //- Calculate moving masks
    void calcMovingMasks() const;
    //- Return moving points mask
    const scalarField& movingPointsMask() const;
public:
  //- Runtime type information
  TYPE_NAME("mixerFvMesh");
  // Constructors
    //- Construct from IOobject
    explicit mixerFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    mixerFvMesh(const mixerFvMesh&) = delete;
    //- Disallow default bitwise assignment
    mixerFvMesh& operator=(const mixerFvMesh&) = delete;
  //- Destructor
  virtual ~mixerFvMesh();
  // Member Functions
    //- Return coordinate system
    const coordinateSystem& cs() const
    {
      return csPtr_();
    }
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
