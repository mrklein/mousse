// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiModels::algebraic
// Description
//   Simple algebraic model for Xi based on Gulders correlation
//   with a linear correction function to give a plausible profile for Xi.
//   See report TR/HGW/10 for details on the Weller two equations model.
//   See \link XiModel.H \endlink for more details on flame wrinkling modelling.
// SourceFiles
//   algebraic.cpp
#ifndef ALGEBRAIC_HPP_
#define ALGEBRAIC_HPP_
#include "xi_model.hpp"
#include "xi_eq_model.hpp"
#include "xi_g_model.hpp"
namespace mousse
{
namespace XiModels
{
class algebraic
:
  public XiModel
{
  // Private data
    scalar XiShapeCoef;
    autoPtr<XiEqModel> XiEqModel_;
    autoPtr<XiGModel> XiGModel_;
public:
  //- Runtime type information
  TYPE_NAME("algebraic");
  // Constructors
    //- Construct from components
    algebraic
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
    algebraic(const algebraic&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const algebraic&) = delete;
  //- Destructor
  virtual ~algebraic();
  // Member Functions
    //- Return the flame diffusivity
    virtual tmp<volScalarField> Db() const;
    //- Correct the flame-wrinking Xi
    virtual void correct();
    //- Update properties from given dictionary
    virtual bool read(const dictionary& XiProperties);
    //- Write fields of the XiEq model
    virtual void writeFields()
    {
      XiEqModel_().writeFields();
    }
};
}  // namespace XiModels
}  // namespace mousse
#endif
