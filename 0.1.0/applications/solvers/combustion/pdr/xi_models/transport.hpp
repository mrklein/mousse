#ifndef TRANSPORT_HPP_
#define TRANSPORT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::XiModels::transport
// Description
//   Simple transport model for Xi based on Gulders correlation
//   with a linear correction function to give a plausible profile for Xi.
//   See report TR/HGW/10 for details on the Weller two equations model.
//   See \link XiModel.H \endlink for more details on flame wrinkling modelling.

#include "xi_model.hpp"
#include "xi_eq_model.hpp"
#include "xi_g_model.hpp"


namespace mousse {
namespace XiModels {

class transport
:
  public XiModel
{
  // Private data
    scalar XiShapeCoef;
    autoPtr<XiEqModel> XiEqModel_;
    autoPtr<XiGModel> XiGModel_;
public:
  //- Runtime type information
  TYPE_NAME("transport");
  // Constructors
    //- Construct from components
    transport
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
    transport(const transport&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const transport&) = delete;
  //- Destructor
  virtual ~transport();
  // Member Functions
    //- Return the flame diffusivity
    virtual tmp<volScalarField> Db() const;
    //- Add Xi to the multivariateSurfaceInterpolationScheme table
    virtual void addXi
    (
      multivariateSurfaceInterpolationScheme<scalar>::fieldTable& fields
    )
    {
      fields.add(Xi_);
    }
    //- Correct the flame-wrinking Xi
    virtual void correct()
    {
      NOT_IMPLEMENTED("transport::correct()");
    }
    //- Correct the flame-wrinking Xi using the given convection scheme
    virtual void correct(const fv::convectionScheme<scalar>& mvConvection);
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

