#ifndef BASIC_XI_SUB_G_HPP_
#define BASIC_XI_SUB_G_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiGModels::basicSubGrid
// Description
//   Basic sub-grid obstacle flame-wrinking generation rate coefficient model.
//   Details supplied by J Puttock 2/7/06.
//   \f$ G_{sub} \f$ denotes the generation coefficient and it is given by
//   \f[
//     G_{sub} = k_{1} /frac{\vert \dwea{\vec{U}} \vert}{L_{obs}}
//         \frac{/Xi_{{sub}_{eq}}-1}{/Xi_{sub}}
//   \f]
//   and the removal:
//   \f[
//     - k_{1} /frac{\vert \dwea{\vec{U}} \vert}{L_{sub}}
//     \frac{\Xi_{sub}-1}{\Xi_{sub}}
//   \f]
//   Finally, \f$ G_{sub} \f$ is added to generation rate \f$ G_{in} \f$
//   due to the turbulence.

#include "xi_g_model.hpp"


namespace mousse {
namespace XiGModels {

class basicSubGrid
:
  public XiGModel
{
  // Private data
    //- Sub-grid generation rate coefficient
    scalar k1;
    //- Xi generation rate model due to turbulence
    autoPtr<XiGModel> XiGModel_;
public:
  //- Runtime type information
  TYPE_NAME("basicSubGridG");
  // Constructors
    //- Construct from components
    basicSubGrid
    (
      const dictionary& XiGProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    basicSubGrid(const basicSubGrid&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const basicSubGrid&) = delete;
  //- Destructor
  virtual ~basicSubGrid();
  // Member Functions
    //- Return the flame-wrinking generation rate
    virtual tmp<volScalarField> G() const;
    //- Return the flame diffusivity
    virtual tmp<volScalarField> Db() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiGProperties);
};

}  // namespace XiGModels
}  // namespace mousse

#endif

