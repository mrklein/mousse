// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::viscosityModels::HerschelBulkley
// Description
//   Herschel-Bulkley non-Newtonian viscosity model.
// SourceFiles
//   herschel_bulkley.cpp
#ifndef herschel_bulkley_hpp_
#define herschel_bulkley_hpp_
#include "viscosity_model.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"
namespace mousse
{
namespace viscosityModels
{
class HerschelBulkley
:
  public viscosityModel
{
  // Private data
    dictionary HerschelBulkleyCoeffs_;
    dimensionedScalar k_;
    dimensionedScalar n_;
    dimensionedScalar tau0_;
    dimensionedScalar nu0_;
    volScalarField nu_;
  // Private Member Functions
    //- Calculate and return the laminar viscosity
    tmp<volScalarField> calcNu() const;
public:
  //- Runtime type information
  TYPE_NAME("HerschelBulkley");
  // Constructors
    //- Construct from components
    HerschelBulkley
    (
      const word& name,
      const dictionary& viscosityProperties,
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  ~HerschelBulkley()
  {}
  // Member Functions
    //- Return the laminar viscosity
    tmp<volScalarField> nu() const
    {
      return nu_;
    }
    //- Return the laminar viscosity for patch
    tmp<scalarField> nu(const label patchi) const
    {
      return nu_.boundaryField()[patchi];
    }
    //- Correct the laminar viscosity
    void correct()
    {
      nu_ = calcNu();
    }
    //- Read transportProperties dictionary
    bool read(const dictionary& viscosityProperties);
};
}  // namespace viscosityModels
}  // namespace mousse
#endif
