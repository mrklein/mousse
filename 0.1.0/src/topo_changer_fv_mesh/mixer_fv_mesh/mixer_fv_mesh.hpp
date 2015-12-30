// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixerFvMesh
// Description
//   A rotating slider mesh
// SourceFiles
//   mixer_fv_mesh.cpp
#ifndef mixer_fv_mesh_hpp_
#define mixer_fv_mesh_hpp_
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
    //- Disallow default bitwise copy construct
    mixerFvMesh(const mixerFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const mixerFvMesh&);
    //- Add mixer zones and modifiers
    void addZonesAndModifiers();
    //- Calculate moving masks
    void calcMovingMasks() const;
    //- Return moving points mask
    const scalarField& movingPointsMask() const;
public:
  //- Runtime type information
  TypeName("mixerFvMesh");
  // Constructors
    //- Construct from IOobject
    explicit mixerFvMesh(const IOobject& io);
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
