#ifndef ODE_ODE_SOLVERS_SIBS_HPP_
#define ODE_ODE_SOLVERS_SIBS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SIBS
// Description
//   mousse::SIBS
//   Bader, G. and Deuflhard, P.
//   "A Semi-Implicit Mid-Point Rule for
//   Stiff Systems of Ordinary Differential Equations."
//   Numer. Math. 41, 373-398, 1983.
// SourceFiles
//   simpr.cpp
//   poly_extrapolate.cpp
#include "ode_solver.hpp"
namespace mousse
{
class SIBS
:
  public ODESolver
{
  // Private data
    static const label kMaxX_ = 7, iMaxX_ = kMaxX_ + 1;
    static const label nSeq_[iMaxX_];
    static const scalar safe1, safe2, redMax, redMin, scaleMX;
    mutable scalarField a_;
    mutable scalarSquareMatrix alpha_;
    mutable scalarRectangularMatrix d_p_;
    mutable scalarField x_p_;
    mutable scalarField err_;
    mutable scalarField yTemp_;
    mutable scalarField ySeq_;
    mutable scalarField yErr_;
    mutable scalarField dydx0_;
    mutable scalarField dfdx_;
    mutable scalarSquareMatrix dfdy_;
    mutable label first_, kMax_, kOpt_;
    mutable scalar epsOld_, xNew_;
  // Private Member Functions
    void SIMPR
    (
      const scalar xStart,
      const scalarField& y,
      const scalarField& dydx,
      const scalarField& dfdx,
      const scalarSquareMatrix& dfdy,
      const scalar deltaX,
      const label nSteps,
      scalarField& yEnd
    ) const;
    void polyExtrapolate
    (
      const label iest,
      const scalar xest,
      const scalarField& yest,
      scalarField& yz,
      scalarField& dy,
      scalarField& x_p,
      scalarRectangularMatrix& d_p
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("SIBS");
  // Constructors
    //- Construct from ODE
    SIBS(const ODESystem& ode, const dictionary& dict);
  // Member Functions
    void solve
    (
      scalar& x,
      scalarField& y,
      scalar& dxTry
    ) const;
};
}  // namespace mousse
#endif
