#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_DUMMY_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_REACTING_TWO_PHASE_EULER_TWO_PHASE_SYSTEM_DIAMETER_MODELS_IATE_SOURCES_DUMMY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::diameterModels::IATEsources::dummy
// Description
// SourceFiles
//   dummy.cpp
#include "iat_esource.hpp"
namespace mousse
{
namespace diameterModels
{
namespace IATEsources
{
class dummy
:
  public IATEsource
{
public:
  //- Runtime type information
  TypeName("dummy");
  // Constructors
    dummy
    (
      const word& name,
      const IATE& iate,
      const dictionary& dict
    )
    :
      IATEsource(iate)
    {}
  //- Destructor
  virtual ~dummy()
  {}
  // Member Functions
    virtual tmp<volScalarField> R() const;
};
}  // namespace IATEsources
}  // namespace diameterModels
}  // namespace mousse
#endif
