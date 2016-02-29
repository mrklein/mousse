#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_RANDOM_COALESCENCE_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_RANDOM_COALESCENCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::IATEsources::randomCoalescence
// Description
//   Random coalescence IATE source as defined in paper:
//   \verbatim
//     "Development of Interfacial Area Transport Equation"
//     Ishii, M., Kim, S. and Kelly, J.,
//     Nuclear Engineering and Technology, Vol.37 No.6 December 2005
//   \endverbatim
// SourceFiles
//   random_coalescence.cpp
#include "iate_source.hpp"
namespace mousse
{
namespace diameterModels
{
namespace IATEsources
{
class randomCoalescence
:
  public IATEsource
{
  // Private data
    dimensionedScalar Crc_;
    dimensionedScalar C_;
    dimensionedScalar alphaMax_;
public:
  //- Runtime type information
  TYPE_NAME("randomCoalescence");
  // Constructors
    randomCoalescence
    (
      const IATE& iate,
      const dictionary& dict
    );
  //- Destructor
  virtual ~randomCoalescence()
  {}
  // Member Functions
    virtual tmp<volScalarField> R() const;
};
}  // namespace IATEsources
}  // namespace diameterModels
}  // namespace mousse
#endif
