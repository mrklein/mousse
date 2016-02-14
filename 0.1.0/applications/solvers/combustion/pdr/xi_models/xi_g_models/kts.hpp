// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiGModels::KTS
// Description
//   Simple Kolmogorov time-scale (KTS) model for the flame-wrinling generation
//   rate.
// SourceFiles
//   kts.cpp
#ifndef KTS_HPP_
#define KTS_HPP_
#include "xi_g_model.hpp"
namespace mousse
{
namespace XiGModels
{
class KTS
:
  public XiGModel
{
  // Private data
    scalar GEtaCoef_;
public:
  //- Runtime type information
  TYPE_NAME("KTS");
  // Constructors
    //- Construct from components
    KTS
    (
      const dictionary& XiGProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su
    );
    //- Disallow copy construct
    KTS(const KTS&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const KTS&) = delete;
  //- Destructor
  virtual ~KTS();
  // Member Functions
    //- Return the flame-wrinking generation rate
    virtual tmp<volScalarField> G() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiGProperties);
};
}  // namespace XiGModels
}  // namespace mousse
#endif
