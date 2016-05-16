#ifndef TURBULENCE_MODELS_PHASE_COMPRESSIBLE_LES_SMAGORINSKY_ZHANG_HPP_
#define TURBULENCE_MODELS_PHASE_COMPRESSIBLE_LES_SMAGORINSKY_ZHANG_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::SmagorinskyZhang
// Group
//   grpLESTurbulence
// Description
//   The Smagorinsky SGS model including bubble-generated turbulence
//   Reference:
//   \verbatim
//     Zhang, D., Deen, N. G., & Kuipers, J. A. M. (2006).
//     Numerical simulation of the dynamic flow behavior in a bubble column:
//     a study of closures for turbulence and interface forces.
//     Chemical Engineering Science, 61(23), 7593-7608.
//   \endverbatim
//   The default model coefficients are
//   \verbatim
//     SmagorinskyZhangCoeffs
//     {
//       Ck              0.094;
//       Ce              1.048;
//       Cmub            0.6;
//     }
//   \endverbatim

#include "les_model.hpp"
#include "eddy_viscosity.hpp"


namespace mousse {
namespace LESModels {

template<class BasicTurbulenceModel>
class SmagorinskyZhang
:
  public Smagorinsky<BasicTurbulenceModel>
{
  // Private data
    mutable const PhaseCompressibleTurbulenceModel
    <
      typename BasicTurbulenceModel::transportModel
    > *gasTurbulencePtr_;
  // Private Member Functions
    //- Return the turbulence model for the gas phase
    const PhaseCompressibleTurbulenceModel
    <
      typename BasicTurbulenceModel::transportModel
    >&
    gasTurbulence() const;
protected:
  // Protected data
    // Model coefficients
      dimensionedScalar Cmub_;
  // Protected Member Functions
    virtual void correctNut();
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("SmagorinskyZhang");
  // Constructors
    //- Construct from components
    SmagorinskyZhang
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
    SmagorinskyZhang(const SmagorinskyZhang&) = delete;
    SmagorinskyZhang& operator=(const SmagorinskyZhang&) = delete;
  //- Destructor
  virtual ~SmagorinskyZhang()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
};

}  // namespace LESModels
}  // namespace mousse

#include "smagorinsky_zhang.ipp"

#endif
