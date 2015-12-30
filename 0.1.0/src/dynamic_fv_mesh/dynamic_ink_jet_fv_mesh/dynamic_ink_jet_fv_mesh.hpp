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
// SourceFiles
//   dynamic_ink_jet_fv_mesh.cpp
#ifndef dynamic_ink_jet_fv_mesh_hpp_
#define dynamic_ink_jet_fv_mesh_hpp_
#include "dynamic_fv_mesh.hpp"
#include "dictionary.hpp"
#include "point_io_field.hpp"
namespace mousse
{
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    dynamicInkJetFvMesh(const dynamicInkJetFvMesh&);
    //- Disallow default bitwise assignment
    void operator=(const dynamicInkJetFvMesh&);
public:
  //- Runtime type information
  TypeName("dynamicInkJetFvMesh");
  // Constructors
    //- Construct from IOobject
    dynamicInkJetFvMesh(const IOobject& io);
  //- Destructor
  ~dynamicInkJetFvMesh();
  // Member Functions
    //- Update the mesh for both mesh motion and topology change
    virtual bool update();
};
}  // namespace mousse
#endif
