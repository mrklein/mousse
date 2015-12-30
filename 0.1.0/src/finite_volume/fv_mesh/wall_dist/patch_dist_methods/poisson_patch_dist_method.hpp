// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchDistMethods::Poisson
// Description
//   Calculation of approximate distance to nearest patch for all cells and
//   boundary by solving Poisson's equation.
//   References:
//   \verbatim
//     D.B. Spalding,
//     "Calculation of turbulent heat transfer in cluttered spaces",
//     Proc. 10th Int. Heat Transfer Conference, Brighton, UK, (1994).
//     E. Fares and W. Schroder,
//     "Differential Equation for Approximate Wall Distance",
//     Int.J.Numer.Meth., 39:743-762, (2002).
//     P.G. Tucker,
//     "Differential equation based wall distance computation for DES and
//     RANS",
//     J.Comp.Phys., Vol. 190, Issue 1, 1 st September, pp. 229-248 (2003)
//   \endverbatim
//   Example of the wallDist specification in fvSchemes:
//   \verbatim
//     laplacianSchemes
//     {
//       .
//       .
//       laplacian(yPsi) Gauss linear corrected;
//       .
//       .
//     }
//     wallDist
//     {
//       method Poisson;
//       // Optional entry enabling the calculation
//       // of the normal-to-wall field
//       nRequired false;
//     }
//   \endverbatim
//   Also the solver specification for yPsi is required in fvSolution, e.g.
//   for simple cases:
//   \verbatim
//     yPsi
//     {
//       solver          PCG;
//       preconditioner  DIC;
//       tolerance       1e-5;
//       relTol          0;
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
//       tolerance       1e-5;
//       relTol          0;
//     }
//   \endverbatim
// SeeAlso
//   mousse::patchDistMethod::meshWave
//   mousse::wallDist
// SourceFiles
//   poisson_patch_dist_method.cpp
#ifndef poisson_patch_dist_method_hpp_
#define poisson_patch_dist_method_hpp_
#include "patch_dist_method.hpp"
namespace mousse
{
namespace patchDistMethods
{
class Poisson
:
  public patchDistMethod
{
  // Private Member Data
    //- Cache yPsi for moving meshes
    tmp<volScalarField> tyPsi_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    Poisson(const Poisson&);
    //- Disallow default bitwise assignment
    void operator=(const Poisson&);
public:
  //- Runtime type information
  TypeName("Poisson");
  // Constructors
    //- Construct from coefficients dictionary, mesh
    //  and fixed-value patch set
    Poisson
    (
      const dictionary& dict,
      const fvMesh& mesh,
      const labelHashSet& patchIDs
    );
    //- Construct from mesh and fixed-value patch set
    Poisson
    (
      const fvMesh& mesh,
      const labelHashSet& patchIDs
    );
  // Member Functions
    //- Correct the given distance-to-patch field
    virtual bool correct(volScalarField& y);
    //- Correct the given distance-to-patch and normal-to-patch fields
    virtual bool correct(volScalarField& y, volVectorField& n);
};
}  // namespace patchDistMethods
}  // namespace mousse
#endif
