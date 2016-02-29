#ifndef FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_MOTION_DIFFUSIVITY_MOTION_DIFFUSIVITY_HPP_
#define FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_MOTION_DIFFUSIVITY_MOTION_DIFFUSIVITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::motionDiffusivity
// Description
//   Abstract base class for cell-centre mesh motion diffusivity.
// SourceFiles
//   motion_diffusivity.cpp
#include "surface_fields_fwd.hpp"
#include "fv_mesh.hpp"
namespace mousse
{
class motionDiffusivity
{
  // Private data
    //- Mesh reference
    const fvMesh& mesh_;
public:
  //- Runtime type information
  TYPE_NAME("motionDiffusivity");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      motionDiffusivity,
      Istream,
      (
        const fvMesh& mesh,
        Istream& mdData
      ),
      (mesh, mdData)
    );
  // Selectors
    //- Select null constructed
    static autoPtr<motionDiffusivity> New
    (
      const fvMesh& mesh,
      Istream& mdData
    );
  // Constructors
    //- Construct for the given fvMesh
    motionDiffusivity(const fvMesh& mesh);
  //- Destructor
  virtual ~motionDiffusivity();
  // Member Functions
    //- Return reference to the mesh
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Return diffusivity field
    virtual tmp<surfaceScalarField> operator()() const = 0;
    //- Correct the motion diffusivity
    virtual void correct() = 0;
};
}  // namespace mousse
#endif
