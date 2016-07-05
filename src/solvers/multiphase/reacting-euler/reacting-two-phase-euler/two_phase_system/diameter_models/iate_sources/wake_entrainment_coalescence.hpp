#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_WAKE_ENTRAINMENT_COALESCENCE_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_WAKE_ENTRAINMENT_COALESCENCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::IATEsources::wakeEntrainmentCoalescence
// Description
//   Bubble coalescence due to wake entrainment IATE source as defined in paper:
//   \verbatim
//     "Development of Interfacial Area Transport Equation"
//     Ishii, M., Kim, S. and Kelly, J.,
//     Nuclear Engineering and Technology, Vol.37 No.6 December 2005
//   \endverbatim
// SourceFiles
//   wake_entrainment_coalescence.cpp
#include "iate_source.hpp"
namespace mousse
{
namespace diameterModels
{
namespace IATEsources
{
class wakeEntrainmentCoalescence
:
  public IATEsource
{
  // Private data
    dimensionedScalar Cwe_;
public:
  //- Runtime type information
  TYPE_NAME("wakeEntrainmentCoalescence");
  // Constructors
    wakeEntrainmentCoalescence
    (
      const IATE& iate,
      const dictionary& dict
    );
  //- Destructor
  virtual ~wakeEntrainmentCoalescence()
  {}
  // Member Functions
    virtual tmp<volScalarField> R() const;
};
}  // namespace IATEsources
}  // namespace diameterModels
}  // namespace mousse
#endif
