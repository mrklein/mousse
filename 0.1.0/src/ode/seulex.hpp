#ifndef ODE_ODE_SOLVERS_SEULEX_HPP_
#define ODE_ODE_SOLVERS_SEULEX_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::seulex
// Description
//   An extrapolation-algorithm, based on the linearly implicit Euler method
//   with step size control and order selection.
//   The implementation is based on the SEULEX algorithm in
//   \verbatim
//     "Solving Ordinary Differential Equations II: Stiff
//     and Differential-Algebraic Problems, second edition",
//     Hairer, E.,
//     Nørsett, S.,
//     Wanner, G.,
//     Springer-Verlag, Berlin. 1996.
//   \endverbatim

#include "ode_solver.hpp"
#include "scalar_matrices.hpp"
#include "label_field.hpp"


namespace mousse {

class seulex
:
  public ODESolver
{
  // Private data
    // Static constants
      static const label kMaxx_ = 12;
      static const label iMaxx_ = kMaxx_ + 1;
      static const scalar
        stepFactor1_, stepFactor2_, stepFactor3_,
        stepFactor4_, stepFactor5_,
        kFactor1_, kFactor2_;
    // Evaluated constants
      scalar jacRedo_;
      labelField nSeq_;
      scalarField cpu_;
      scalarSquareMatrix coeff_;
    // Temporary storage
    // held to avoid dynamic memory allocation between calls
    // and to transfer internal values between functions
      mutable scalar theta_;
      mutable label kTarg_;
      mutable scalarRectangularMatrix table_;
      mutable scalarField dfdx_;
      mutable scalarSquareMatrix dfdy_;
      mutable scalarSquareMatrix a_;
      mutable labelList pivotIndices_;
      // Fields space for "solve" function
      mutable scalarField dxOpt_, temp_;
      mutable scalarField y0_, ySequence_, scale_;
      // Fields used in "seul" function
      mutable scalarField dy_, yTemp_, dydx_;
  // Private Member Functions
    //- Computes the j-th line of the extrapolation table
    bool seul
    (
      const scalar x0,
      const scalarField& y0,
      const scalar dxTot,
      const label k,
      scalarField& y,
      const scalarField& scale
    ) const;
    //- Polynomial extrpolation
    void extrapolate
    (
      const label k,
      scalarRectangularMatrix& table,
      scalarField& y
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("seulex");
  // Constructors
    //- Construct from ODE
    seulex(const ODESystem& ode, const dictionary& dict);
  // Member Functions
    //- Solve the ODE system and the update the state
    void solve
    (
      scalar& x,
      scalarField& y,
      stepState& step
    ) const;
};

}  // namespace mousse

#endif

