#ifndef FIXED_HPP_
#define FIXED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiModels::fixed
// Description
//   Fixed value model for Xi. See \link XiModel.H \endlink for more details
//   on flame wrinkling modelling.

#include "xi_model.hpp"


namespace mousse {
namespace XiModels {

class fixed
:
  public XiModel
{
public:
  //- Runtime type information
  TYPE_NAME("fixed");
  // Constructors
    //- Construct from components
    fixed
    (
      const dictionary& XiProperties,
      const psiuReactionThermo& thermo,
      const compressible::RASModel& turbulence,
      const volScalarField& Su,
      const volScalarField& rho,
      const volScalarField& b,
      const surfaceScalarField& phi
    );
    //- Disallow copy construct
    fixed(const fixed&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const fixed&) = delete;
  //- Destructor
  virtual ~fixed();
  // Member Functions
    //- Correct the flame-wrinking Xi
    virtual void correct()
    {}
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiProperties);
    //- Write fields of the XiEq model
    virtual void writeFields()
    {}
};

}  // namespace XiModels
}  // namespace mousse

#endif

