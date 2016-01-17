// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermalBaffle
// Description
//   2D thermal baffle
// SourceFiles
//   thermal_baffle.cpp
#ifndef thermal_baffle_hpp_
#define thermal_baffle_hpp_
#include "thermal_baffle_model.hpp"
#include "vol_fields_fwd.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
namespace mousse
{
namespace regionModels
{
namespace thermalBaffleModels
{
class thermalBaffle
:
  public thermalBaffleModel
{
private:
  // Private member functions
    //- Initialize thermalBaffle
    void init();
protected:
  // Protected data
    // Solution parameters
      //- Number of non orthogonal correctors
      label nNonOrthCorr_;
    // Thermo properties
      //- Solid thermo
      autoPtr<solidThermo> thermo_;
      //- Enthalpy/internal energy
      volScalarField& h_;
    // Source term fields
      //- Surface energy source  / [J/m2/s]
      volScalarField Qs_;
      //- Volumetric energy source  / [J/m3/s]
      volScalarField Q_;
    // Sub models
      //- Pointer to radiation model
      autoPtr<radiation::radiationModel> radiation_;
  // Protected member functions
    //- Read control parameters IO dictionary
    virtual bool read();
    //- Read control parameters from dictionary
    virtual bool read(const dictionary& dict);
    // Equations
      //- Solve energy equation
      void solveEnergy();
public:
  //- Runtime type information
  TYPE_NAME("thermalBaffle");
  // Constructors
    //- Construct from components
    thermalBaffle(const word& modelType, const fvMesh& mesh);
    //- Construct from components and dict
    thermalBaffle
    (
      const word& modelType,
      const fvMesh& mesh,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    thermalBaffle(const thermalBaffle&) = delete;
    //- Disallow default bitwise assignment
    thermalBaffle& operator=(const thermalBaffle&) = delete;
  //- Destructor
  virtual ~thermalBaffle();
  // Member Functions
    // Thermo properties
      //- Return const reference to the solidThermo
      virtual const solidThermo& thermo() const;
      // Fields
        //- Return the film specific heat capacity [J/kg/K]
        virtual const tmp<volScalarField> Cp() const;
        //- Return solid absortivity [1/m]
        virtual const volScalarField& kappaRad() const;
        //- Return temperature [K]
        virtual const volScalarField& T() const;
        //- Return density [Kg/m3]
        virtual const volScalarField& rho() const;
        //- Return thermal conductivity [W/m/K]
        virtual const volScalarField& kappa() const;
      // Helper functions
        //- Return sensible enthalpy/internal energy
        //  as a function of temperature
        //  for a patch
        inline tmp<scalarField> he
        (
          const scalarField& p,
          const scalarField& T,
          const label patchI
        ) const;
        //- Return sensible enthalpy/internal energy
        inline tmp<volScalarField> he() const;
    // Evolution
      //- Pre-evolve  thermal baffle
      virtual void preEvolveRegion();
      //- Evolve the thermal baffle
      virtual void evolveRegion();
   // I-O
      //- Provide some feedback
      virtual void info();
};
}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse

namespace mousse
{
namespace regionModels
{
namespace thermalBaffleModels
{
inline tmp<scalarField> thermalBaffle::he
(
  const scalarField& p,
  const scalarField& T,
  const label patchI
) const
{
  return thermo_->he(p, T, patchI);
}
inline tmp<volScalarField> thermalBaffle::he() const
{
  return thermo_->he();
}
}  // namespace thermalBaffleModels
}  // namespace regionModels
}  // namespace mousse
#endif
