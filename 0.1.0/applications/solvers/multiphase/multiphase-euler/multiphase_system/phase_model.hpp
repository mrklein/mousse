// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseModel
// SourceFiles
//   phase_model.cpp
#ifndef phase_model_hpp_
#define phase_model_hpp_
#include "dictionary.hpp"
#include "dictionary_entry.hpp"
#include "dimensioned_scalar.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
// Forward declarations
class diameterModel;
class phaseModel
:
  public volScalarField
{
  // Private data
    //- Name of phase
    word name_;
    dictionary phaseDict_;
    //- Kinematic viscosity
    dimensionedScalar nu_;
    //- Thermal conductivity
    dimensionedScalar kappa_;
    //- Heat capacity
    dimensionedScalar Cp_;
    //- Density
    dimensionedScalar rho_;
    //- Velocity
    volVectorField U_;
    //- Substantive derivative of the velocity
    volVectorField DDtU_;
    //- Volumetric flux of the phase
    surfaceScalarField alphaPhi_;
    //- Volumetric flux for the phase
    autoPtr<surfaceScalarField> phiPtr_;
    //- Diameter model
    autoPtr<diameterModel> dPtr_;
public:
  // Constructors
    phaseModel
    (
      const word& phaseName,
      const dictionary& phaseDict,
      const fvMesh& mesh
    );
    //- Return clone
    autoPtr<phaseModel> clone() const;
    //- Return a pointer to a new phase created on freestore
    //  from Istream
    class iNew
    {
      const fvMesh& mesh_;
    public:
      iNew
      (
        const fvMesh& mesh
      )
      :
        mesh_(mesh)
      {}
      autoPtr<phaseModel> operator()(Istream& is) const
      {
        dictionaryEntry ent(dictionary::null, is);
        return autoPtr<phaseModel>
        (
          new phaseModel(ent.keyword(), ent, mesh_)
        );
      }
    };
  //- Destructor
  virtual ~phaseModel();
  // Member Functions
    const word& name() const
    {
      return name_;
    }
    const word& keyword() const
    {
      return name();
    }
    tmp<volScalarField> d() const;
    const dimensionedScalar& nu() const
    {
      return nu_;
    }
    const dimensionedScalar& kappa() const
    {
      return kappa_;
    }
    const dimensionedScalar& Cp() const
    {
      return Cp_;
    }
    const dimensionedScalar& rho() const
    {
      return rho_;
    }
    const volVectorField& U() const
    {
      return U_;
    }
    volVectorField& U()
    {
      return U_;
    }
    const volVectorField& DDtU() const
    {
      return DDtU_;
    }
    volVectorField& DDtU()
    {
      return DDtU_;
    }
    const surfaceScalarField& phi() const
    {
      return phiPtr_();
    }
    surfaceScalarField& phi()
    {
      return phiPtr_();
    }
    const surfaceScalarField& alphaPhi() const
    {
      return alphaPhi_;
    }
    surfaceScalarField& alphaPhi()
    {
      return alphaPhi_;
    }
    //- Correct the phase properties
    void correct();
    //-Inherit read from volScalarField
    using volScalarField::read;
    //- Read base transportProperties dictionary
    bool read(const dictionary& phaseDict);
};
}  // namespace mousse
#endif
