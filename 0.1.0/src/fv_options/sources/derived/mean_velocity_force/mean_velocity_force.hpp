// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::meanVelocityForce
// Description
//   Calculates and applies the force necessary to maintain the specified mean
//   velocity.
//   Note: Currently only handles kinematic pressure (incompressible solvers).
//   \heading Source usage
//   Example usage:
//   \verbatim
//   meanVelocityForceCoeffs
//   {
//     selectionMode   all;                    // Apply force to all cells
//     fieldNames      (U);                    // Name of velocity field
//     Ubar            (10.0 0 0);             // Desired mean velocity
//     relaxation      0.2;                    // Optional relaxation factor
//   }
//   \endverbatim
// SourceFiles
//   mean_velocity_force.cpp
#ifndef mean_velocity_force_hpp_
#define mean_velocity_force_hpp_
#include "auto_ptr.hpp"
#include "topo_set_source.hpp"
#include "cell_set.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "cell_set_option.hpp"
namespace mousse
{
namespace fv
{
class meanVelocityForce
:
  public cellSetOption
{
protected:
  // Protected data
    //- Average velocity
    vector Ubar_;
    //- Pressure gradient before correction
    scalar gradP0_;
    //- Change in pressure gradient
    scalar dGradP_;
    //- Flow direction
    vector flowDir_;
    //- Relaxation factor
    scalar relaxation_;
    //- Matrix 1/A coefficients field pointer
    autoPtr<volScalarField> rAPtr_;
  // Protected Member Functions
    //- Calculate and return the magnitude of the mean velocity
    //  averaged over the selected cellSet
    virtual scalar magUbarAve(const volVectorField& U) const;
    //- Write the pressure gradient to file (for restarts etc)
    void writeProps(const scalar gradP) const;
    //- Correct driving force for a constant mass flow rate
    void update(fvMatrix<vector>& eqn);
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    meanVelocityForce(const meanVelocityForce&);
    //- Disallow default bitwise assignment
    void operator=(const meanVelocityForce&);
public:
  //- Runtime type information
  TypeName("meanVelocityForce");
  // Constructors
    //- Construct from explicit source name and mesh
    meanVelocityForce
    (
      const word& sourceName,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  // Member Functions
    // Evaluate
      //- Correct the pressure gradient
      virtual void correct(volVectorField& U);
      //- Add explicit contribution to momentum equation
      virtual void addSup
      (
        fvMatrix<vector>& eqn,
        const label fieldI
      );
      //- Add explicit contribution to compressible momentum equation
      virtual void addSup
      (
        const volScalarField& rho,
        fvMatrix<vector>& eqn,
        const label fieldI
      );
      //- Set 1/A coefficient
      virtual void constrain
      (
        fvMatrix<vector>& eqn,
        const label fieldI
      );
    // IO
      //- Read source dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#endif
