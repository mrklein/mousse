// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::TurbulenceModel
// Description
//   Templated abstract base class for turbulence models
// SourceFiles
//   _turbulence_model.cpp
#ifndef _turbulence_model_hpp_
#define _turbulence_model_hpp_
#include "turbulence_model.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
template
<
  class Alpha,
  class Rho,
  class BasicTurbulenceModel,
  class TransportModel
>
class TurbulenceModel
:
  public BasicTurbulenceModel
{
public:
  typedef Alpha alphaField;
  typedef Rho rhoField;
  typedef TransportModel transportModel;
protected:
  // Protected data
    const alphaField& alpha_;
    const transportModel& transport_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    TurbulenceModel(const TurbulenceModel&);
    //- Disallow default bitwise assignment
    void operator=(const TurbulenceModel&);
public:
  // Declare run-time constructor selection table
    declareRunTimeNewSelectionTable
    (
      autoPtr,
      TurbulenceModel,
      dictionary,
      (
        const alphaField& alpha,
        const rhoField& rho,
        const volVectorField& U,
        const surfaceScalarField& alphaRhoPhi,
        const surfaceScalarField& phi,
        const transportModel& transport,
        const word& propertiesName
      ),
      (alpha, rho, U, alphaRhoPhi, phi, transport, propertiesName)
    );
  // Constructors
    //- Construct
    TurbulenceModel
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName
    );
  // Selectors
    //- Return a reference to the selected turbulence model
    static autoPtr<TurbulenceModel> New
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~TurbulenceModel()
  {}
  // Member Functions
    //- Access function to phase fraction
    const alphaField& alpha() const
    {
      return alpha_;
    }
    //- Access function to incompressible transport model
    const transportModel& transport() const
    {
      return transport_;
    }
    //- Return the laminar viscosity
    tmp<volScalarField> nu() const
    {
      return transport_.nu();
    }
    //- Return the laminar viscosity on patchi
    tmp<scalarField> nu(const label patchi) const
    {
      return transport_.nu(patchi);
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "_turbulence_model.cpp"
#endif
#endif
