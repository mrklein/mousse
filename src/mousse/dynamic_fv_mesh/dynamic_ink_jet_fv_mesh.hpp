#ifndef DYNAMIC_FV_MESH_DYNAMIC_INK_JET_FV_MESH_DYNAMIC_INK_JET_FV_MESH_HPP_
#define DYNAMIC_FV_MESH_DYNAMIC_INK_JET_FV_MESH_DYNAMIC_INK_JET_FV_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicInkJetFvMesh
// Description
//   Mesh motion specifically for the "pumping" system of an ink-jet
//   injector.
//   The set of points in the "pumping" region are compressed and expanded
//   sinusoidally to impose a sinusoidal variation of the flow at the
//   nozzle exit.

#include "dynamic_fv_mesh.hpp"
#include "dictionary.hpp"
#include "point_io_field.hpp"


namespace mousse {

class dynamicInkJetFvMesh
:
  public dynamicFvMesh
{
  // Private data
    dictionary dynamicMeshCoeffs_;
    scalar amplitude_;
    scalar frequency_;
    scalar refPlaneX_;
    pointIOField stationaryPoints_;
public:
  //- Runtime type information
  TYPE_NAME("dynamicInkJetFvMesh");
  // Constructors
    //- Construct from IOobject
    dynamicInkJetFvMesh(const IOobject& io);
    //- Disallow default bitwise copy construct
    dynamicInkJetFvMesh(const dynamicInkJetFvMesh&) = delete;
    //- Disallow default bitwise assignment
    dynamicInkJetFvMesh& operator=(const dynamicInkJetFvMesh&) = delete;
  //- Destructor
  ~dynamicInkJetFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};

}  // namespace mousse

#endif

