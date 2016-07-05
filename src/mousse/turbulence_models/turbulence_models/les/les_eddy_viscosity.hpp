#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_EDDY_VISCOSITY_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_EDDY_VISCOSITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::LESeddyViscosity
// Group
//   grpLESTurbulence
// Description
//   Eddy viscosity LES SGS model base class

#include "les_model.hpp"
#include "eddy_viscosity.hpp"


namespace mousse {
namespace LESModels {

template<class BasicTurbulenceModel>
class LESeddyViscosity
:
  public eddyViscosity<LESModel<BasicTurbulenceModel> >
{
protected:
  // Protected data
    dimensionedScalar Ce_;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  // Constructors
    //- Construct from components
    LESeddyViscosity
    (
      const word& type,
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName
    );
    // Disallow default bitwise copy construct and assignment
    LESeddyViscosity(const LESeddyViscosity&) = delete;
    LESeddyViscosity& operator=(const LESeddyViscosity&) = delete;
  //- Destructor
  virtual ~LESeddyViscosity()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const;
};

}  // namespace LESModels
}  // namespace mousse

#include "les_eddy_viscosity.ipp"

#endif
