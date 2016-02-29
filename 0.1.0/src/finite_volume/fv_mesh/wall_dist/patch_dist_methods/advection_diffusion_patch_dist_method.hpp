#ifndef FINITE_VOLUME_FV_MESH_WALL_DIST_PATCH_DIST_METHODS_ADVECTION_DIFFUSION_PATCH_DIST_METHOD_HPP_
#define FINITE_VOLUME_FV_MESH_WALL_DIST_PATCH_DIST_METHODS_ADVECTION_DIFFUSION_PATCH_DIST_METHOD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchDistMethods::advectionDiffusion
// Description
//   Calculation of approximate distance to nearest patch for all cells and
//   boundary by solving the Eikonal equation in advection form with diffusion
//   smoothing.
//   If the diffusion coefficient is set to 0 this method is exact in principle
//   but the numerical schemes used are not rendering the scheme approximate, but
//   more accurate than the Poisson method.  Also many models relying on the
//   distance to the wall benefit from this field being smooth and monotonic so
//   the addition of diffusion smoothing improves both the convergence and
//   stability of the solution of the Eikonal equation and the behavior of the
//   models using the distance field generated.  However, it is not clear what
//   the optimum value for the diffusion coefficient epsilon should be; a
//   default value of 0.1 is provided but higher values may be preferable under
//   some circumstances.
//   Convergence is accelerated by first generating an approximate solution
//   using one of the simpler methods, e.g. Poisson.  The method used for
//   this prediction step is specified in the 'advectionDiffusionCoeffs'
//   sub-dictionary, see below.
//   References:
//   \verbatim
//     P.G. Tucker, C.L. Rumsey, R.E. Bartels, R.T. Biedron,
//     "Transport equation based wall distance computations aimed at flows
//     with time-dependent geometry",
//     NASA/TM-2003-212680, December 2003.
//   \endverbatim
//   Example of the wallDist specification in fvSchemes:
//   \verbatim
//     laplacianSchemes
//     {
//       .
//       .
//       laplacian(yPsi) Gauss linear corrected;
//       laplacian(yWall) Gauss linear corrected;
//       .
//       .
//     }
//     wallDist
//     {
//       method advectionDiffusion;
//       // Optional entry enabling the calculation
//       // of the normal-to-wall field
//       nRequired false;
//       advectionDiffusionCoeffs
//       {
//         method    Poisson;
//         epsilon   0.1;
//         tolerance 1e-3;
//         maxIter   10;
//       }
//     }
//   \endverbatim
//   Also the solver specification for yWall is required in fvSolution, e.g.
//   for simple cases:
//   \verbatim
//     yPsi
//     {
//       solver          PCG;
//       preconditioner  DIC;
//       tolerance       1e-4;
//       relTol          0;
//     }
//     yWall
//     {
//       solver          PBiCG;
//       preconditioner  DILU;
//       tolerance       1e-4;
//       relTol          0;
//     }
//     relaxationFactors
//     {
//       equations
//       {
//         .
//         .
//         yWall           1;
//         .
//         .
//       }
//     }
//   or for more complex cases:
//     yPsi
//     {
//       solver          GAMG;
//       smoother        GaussSeidel;
//       cacheAgglomeration true;
//       nCellsInCoarsestLevel 10;
//       agglomerator    faceAreaPair;
//       mergeLevels     1;
//       tolerance       1e-4;
//       relTol          0;
//     }
//     yWall
//     {
//       solver          GAMG;
//       smoother        symGaussSeidel;
//       cacheAgglomeration true;
//       nCellsInCoarsestLevel 10;
//       agglomerator    faceAreaPair;
//       mergeLevels     1;
//       tolerance       1e-4;
//       relTol          0;
//     }
//     relaxationFactors
//     {
//       equations
//       {
//         .
//         .
//         yWall           0.7;
//         .
//         .
//       }
//     }
//   \endverbatim
// SeeAlso
//   mousse::patchDistMethod::Poisson
//   mousse::patchDistMethod::meshWave
//   mousse::wallDist
// SourceFiles
//   advection_diffusion_patch_dist_method.cpp
#include "patch_dist_method.hpp"
namespace mousse
{
namespace patchDistMethods
{
class advectionDiffusion
:
  public patchDistMethod
{
  // Private Member Data
    //- Sub-dictionary of coefficients
    dictionary coeffs_;
    //- Run-time selected method to predict the distance-to-wall field
    autoPtr<patchDistMethod> pdmPredictor_;
    //- Diffusion coefficient multiplying y*laplacian(y)
    scalar epsilon_;
    //- Convergence tolerance for the iterations of the advection-diffusion
    //  equation to correct the distance-to-patch and normal-to-patch fields
    scalar tolerance_;
    //- Maximum number of iterations of the advection-diffusion equation
    //  to correct the distance-to-patch and normal-to-patch fields
    int maxIter_;
    //- Flag to indicate the predictor step has been executed
    bool predicted_;
public:
  //- Runtime type information
  TYPE_NAME("advectionDiffusion");
  // Constructors
    //- Construct from coefficients dictionary, mesh
    //  and fixed-value patch set
    advectionDiffusion
    (
      const dictionary& dict,
      const fvMesh& mesh,
      const labelHashSet& patchIDs
    );
    //- Disallow default bitwise copy construct
    advectionDiffusion(const advectionDiffusion&) = delete;
    //- Disallow default bitwise assignment
    advectionDiffusion& operator=(const advectionDiffusion&) = delete;
  // Member Functions
    //- Correct the given distance-to-patch field
    virtual bool correct(volScalarField& y);
    //- Correct the given distance-to-patch and normal-to-patch fields
    virtual bool correct(volScalarField& y, volVectorField& n);
};
}  // namespace patchDistMethods
}  // namespace mousse
#endif
