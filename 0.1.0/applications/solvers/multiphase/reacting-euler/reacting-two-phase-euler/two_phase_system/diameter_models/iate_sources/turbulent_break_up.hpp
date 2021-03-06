#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_TURBULENT_BREAK_UP_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_TURBULENT_BREAK_UP_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::IATEsources::turbulentBreakUp
// Description
//   Turbulence-induced break-up IATE source as defined in paper:
//   \verbatim
//     "Development of Interfacial Area Transport Equation"
//     Ishii, M., Kim, S. and Kelly, J.,
//     Nuclear Engineering and Technology, Vol.37 No.6 December 2005
//   \endverbatim
// SourceFiles
//   turbulent_break_up.cpp
#include "iate_source.hpp"
namespace mousse
{
namespace diameterModels
{
namespace IATEsources
{
class turbulentBreakUp
:
  public IATEsource
{
  // Private data
    dimensionedScalar Cti_;
    dimensionedScalar WeCr_;
public:
  //- Runtime type information
  TYPE_NAME("turbulentBreakUp");
  // Constructors
    turbulentBreakUp
    (
      const IATE& iate,
      const dictionary& dict
    );
  //- Destructor
  virtual ~turbulentBreakUp()
  {}
  // Member Functions
    virtual tmp<volScalarField> R() const;
};
}  // namespace IATEsources
}  // namespace diameterModels
}  // namespace mousse
#endif
