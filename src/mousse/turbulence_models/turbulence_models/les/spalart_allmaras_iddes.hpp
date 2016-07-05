#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_SPALART_ALLMARAS_IDDES_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_SPALART_ALLMARAS_IDDES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::SpalartAllmarasIDDES
// Group
//   grpDESTurbulence
// Description
//   SpalartAllmaras IDDES turbulence model for incompressible and compressible
//   flows
//   Reference:
//   \verbatim
//     Shur, M. L., Spalart, P. R., Strelets, M. K., & Travin, A. K. (2008).
//     A hybrid RANS-LES approach with delayed-DES and wall-modelled LES
//     capabilities.
//     International Journal of Heat and Fluid Flow, 29(6), 1638-1649.
//   \endverbatim

#include "spalart_allmaras_des.hpp"
#include "iddes_delta.hpp"


namespace mousse {
namespace LESModels {

template<class BasicTurbulenceModel>
class SpalartAllmarasIDDES
:
  public SpalartAllmarasDES<BasicTurbulenceModel>
{
  // Private data
    // Model constants
      dimensionedScalar fwStar_;
      dimensionedScalar cl_;
      dimensionedScalar ct_;
    // Fields
      const IDDESDelta& IDDESDelta_;
  // Private Member Functions
    tmp<volScalarField> alpha() const;
    tmp<volScalarField> ft(const volScalarField& magGradU) const;
    tmp<volScalarField> fl(const volScalarField& magGradU) const;
    tmp<volScalarField> rd
    (
      const volScalarField& nur,
      const volScalarField& magGradU
    ) const;
    //- Delay function
    tmp<volScalarField> fd(const volScalarField& magGradU) const;
protected:
  // Protected Member Functions
    //- Length scale
    virtual tmp<volScalarField> dTilda
    (
      const volScalarField& chi,
      const volScalarField& fv1,
      const volTensorField& gradU
    ) const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TYPE_NAME("SpalartAllmarasIDDES");
  // Constructors
    //- Construct from components
    SpalartAllmarasIDDES
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
    SpalartAllmarasIDDES(const SpalartAllmarasIDDES&) = delete;
    SpalartAllmarasIDDES& operator=(const SpalartAllmarasIDDES&) = delete;
  //- Destructor
  virtual ~SpalartAllmarasIDDES()
  {}
  // Member Functions
    //- Read LESProperties dictionary
    virtual bool read();
};

}  // namespace LESModels
}  // namespace mousse

#include "spalart_allmaras_iddes.ipp"

#endif
