// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::LESeddyViscosity
// Group
//   grpLESTurbulence
// Description
//   Eddy viscosity LES SGS model base class
// SourceFiles
//   les_eddy_viscosity.cpp
#ifndef les_eddy_viscosity_hpp_
#define les_eddy_viscosity_hpp_
#include "les_model.hpp"
#include "eddy_viscosity.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class LESeddyViscosity
:
  public eddyViscosity<LESModel<BasicTurbulenceModel> >
{
  // Private Member Functions
    // Disallow default bitwise copy construct and assignment
    LESeddyViscosity(const LESeddyViscosity&);
    LESeddyViscosity& operator=(const LESeddyViscosity&);
protected:
  // Protected data
    dimensionedScalar Ce_;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  // Constructors
    //- Construct from components
    LESeddyViscosity
    (
      const word& type,
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName
    );
  //- Destructor
  virtual ~LESeddyViscosity()
  {}
  // Member Functions
    //- Read model coefficients if they have changed
    virtual bool read();
    //- Return sub-grid disipation rate
    virtual tmp<volScalarField> epsilon() const;
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "les_eddy_viscosity.cpp"
#endif
#endif
