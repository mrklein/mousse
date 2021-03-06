#ifndef BASIC_HPP_
#define BASIC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PDRDragModels::basic
// Description
//   Basic sub-grid obstacle drag model.
//   Details supplied by J Puttock 2/7/06.
//   <b> Sub-grid drag term </b>
//   The resistance term (force per unit of volume) is given by:
//   \f[
//     R = -\frac{1}{2} \rho \vert \dwea{\vec{U}} \vert \dwea{\vec{U}}.D
//   \f]
//   where:
//     \f$ D \f$ is the tensor field "CR" in \f$ m^{-1} \f$
//     This is term is treated implicitly in UEqn.H
//   <b> Sub-grid turbulence generation </b>
//   The turbulence source term \f$ G_{R} \f$ occurring in the
//   \f$ \kappa-\epsilon \f$ equations for the generation of turbulence due
//   to interaction with unresolved obstacles :
//   \f$ G_{R} = C_{s}\beta_{\nu}
//   \mu_{eff} A_{w}^{2}(\dwea{\vec{U}}-\dwea{\vec{U}_{s}})^2 + \frac{1}{2}
//   \rho \vert \dwea{\vec{U}} \vert \dwea{\vec{U}}.T.\dwea{\vec{U}} \f$
//   where:
//     \f$ C_{s} \f$ = 1
//     \f$ \beta_{\nu} \f$ is the volume porosity (file "betav").
//     \f$ \mu_{eff} \f$ is the effective viscosity.
//     \f$ A_{w}^{2}\f$ is the obstacle surface area per unit of volume
//     (file "Aw").
//     \f$ \dwea{\vec{U}_{s}} \f$ is the slip velocity and is considered
//     \f$ \frac{1}{2}. \dwea{\vec{U}} \f$.
//     \f$ T \f$ is a tensor in the file CT.
//   The term \f$ G_{R} \f$ is treated explicitly in the \f$ \kappa-\epsilon
//   \f$ Eqs in the \link PDRkEpsilon.C \endlink file.

#include "pdr_drag_model.hpp"
#include "xi_eq_model.hpp"


namespace mousse {
namespace PDRDragModels {

class basic
:
  public PDRDragModel
{
  // Private data
    dimensionedScalar Csu;
    dimensionedScalar Csk;
    volScalarField Aw_;
    volSymmTensorField CR_;
public:
  //- Runtime type information
  TYPE_NAME("basic");
  // Constructors
    //- Construct from components
    basic
    (
      const dictionary& PDRProperties,
      const compressible::RASModel& turbulence,
      const volScalarField& rho,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
    //- Disallow copy construct
    basic(const basic&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const basic&) = delete;
  //- Destructor
  virtual ~basic();
  // Member Functions
    //- Return the momentum drag coefficient
    virtual tmp<volSymmTensorField> Dcu() const;
    //- Return the momentum drag turbulence generation rate
    virtual tmp<volScalarField> Gk() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& PDRProperties);
    //- Write fields
    void writeFields() const;
};

}  // namespace PDRDragModels
}  // namespace mousse

#endif

