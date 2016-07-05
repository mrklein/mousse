#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_WALL_DEPENDENT_MODEL_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_MODELS_WALL_DEPENDENT_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallDependentModel
// Description
//   A class which provides on-demand creation and caching of wall distance and
//   wall normal fields for use by multiple models.
// SourceFiles
//   wall_dependent_model.cpp
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class wallDependentModel
{
  // Private data
    //- Reference to the mesh
    const fvMesh& mesh_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    wallDependentModel(const wallDependentModel&);
    //- Disallow default bitwise assignment
    void operator=(const wallDependentModel&);
public:
  // Constructors
    //- Construct from a mesh
    wallDependentModel(const fvMesh& mesh);
  //- Destructor
  virtual ~wallDependentModel();
  // Member Functions
    // Return the wall distance, creating and storing it if necessary
    const volScalarField& yWall() const;
    // Return the wall normal, creating and storing it if necessary
    const volVectorField& nWall() const;
};
}  // namespace mousse
#endif
