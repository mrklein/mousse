#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_DEARDORFF_DIFF_STRESS_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_DEARDORFF_DIFF_STRESS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::DeardorffDiffStress
// Group
//   grpLESTurbulence
// Description
//   Differential SGS Stress Equation Model for incompressible and
//   compressible flows
//   Reference:
//   \verbatim
//     Deardorff, J. W. (1973).
//     The use of subgrid transport equations in a three-dimensional model
//     of atmospheric turbulence.
//     Journal of Fluids Engineering, 95(3), 429-438.
//   \endverbatim
//   This SGS model uses a full balance equation for the SGS stress tensor to
//   simulate the behaviour of B.
//   This implementation is as described in the above paper except that the
//   triple correlation model of Donaldson is replaced with the generalized
//   gradient diffusion model of Daly and Harlow:
//   \verbatim
//     Daly, B. J., & Harlow, F. H. (1970).
//     Transport equations in turbulence.
//     Physics of Fluids (1958-1988), 13(11), 2634-2649.
//   \endverbatim
//   with the default value for the coefficient Cs of 0.25 from
//   \verbatim
//     Launder, B. E., Reece, G. J., & Rodi, W. (1975).
//     Progress in the development of a Reynolds-stress turbulence closure.
//     Journal of fluid mechanics, 68(03), 537-566.
//   \endverbatim
// SourceFiles
//   deardorff_diff_stress.cpp
#include "les_model.hpp"
#include "reynolds_stress.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class DeardorffDiffStress
:
  public ReynoldsStress<LESModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    // Model constants
      dimensionedScalar Ck_;
      dimensionedScalar Cm_;
      dimensionedScalar Ce_;
      dimensionedScalar Cs_;
  // Protected Member Functions
    //- Update the eddy-viscosity
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("DeardorffDiffStress");
  // Constructors
    //- Constructor from components
    DeardorffDiffStress
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
    // Disallow default bitwise copy construct and assignment
    DeardorffDiffStress(const DeardorffDiffStress&) = delete;
    DeardorffDiffStress& operator=(const DeardorffDiffStress&) = delete;
  //- Destructor
  virtual ~DeardorffDiffStress()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return the turbulence kinetic energy dissipation rate
    virtual tmp<volScalarField> epsilon() const;
    //- Correct sub-grid stress, eddy-Viscosity and related properties
    virtual void correct();
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "deardorff_diff_stress.cpp"
#endif
#endif
