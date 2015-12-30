// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LESModels::SpalartAllmarasDDES
// Group
//   grpDESTurbulence
// Description
//   SpalartAllmaras DDES turbulence model for incompressible and compressible
//   flows
//   Reference:
//   \verbatim
//     Spalart, P. R., Deck, S., Shur, M. L., Squires, K. D., Strelets, M. K.,
//     & Travin, A. (2006).
//     A new version of detached-eddy simulation, resistant to ambiguous grid
//     densities.
//     Theoretical and computational fluid dynamics, 20(3), 181-195.
//   \endverbatim
// SourceFiles
//   spalart_allmaras_ddes.cpp
#ifndef spalart_allmaras_ddes_hpp_
#define spalart_allmaras_ddes_hpp_
#include "spalart_allmaras_des.hpp"
namespace mousse
{
namespace LESModels
{
template<class BasicTurbulenceModel>
class SpalartAllmarasDDES
:
  public SpalartAllmarasDES<BasicTurbulenceModel>
{
  // Private Member Functions
    tmp<volScalarField> fd(const volScalarField& magGradU) const;
    tmp<volScalarField> rd(const volScalarField& magGradU) const;
    // Disallow default bitwise copy construct and assignment
    SpalartAllmarasDDES(const SpalartAllmarasDDES&);
    SpalartAllmarasDDES& operator=(const SpalartAllmarasDDES&);
protected:
  // Protected Member Functions
    //- Length scale
    virtual tmp<volScalarField> dTilda
    (
      const volScalarField& chi,
      const volScalarField& fv1,
      const volTensorField& gradU
    ) const;
public:
  typedef typename BasicTurbulenceModel::alphaField alphaField;
  typedef typename BasicTurbulenceModel::rhoField rhoField;
  typedef typename BasicTurbulenceModel::transportModel transportModel;
  //- Runtime type information
  TypeName("SpalartAllmarasDDES");
  // Constructors
    //- Construct from components
    SpalartAllmarasDDES
    (
      const alphaField& alpha,
      const rhoField& rho,
      const volVectorField& U,
      const surfaceScalarField& alphaRhoPhi,
      const surfaceScalarField& phi,
      const transportModel& transport,
      const word& propertiesName = turbulenceModel::propertiesName,
      const word& type = typeName
    );
  //- Destructor
  virtual ~SpalartAllmarasDDES()
  {}
};
}  // namespace LESModels
}  // namespace mousse
#ifdef NoRepository
#   include "spalart_allmaras_ddes.cpp"
#endif
#endif
