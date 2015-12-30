// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoThermoCombustion
// Description
//   Density-based thermo model wrapper for combustion models
// SourceFiles
//   rho_thermo_combustion.cpp
#ifndef rho_thermo_combustion_hpp_
#define rho_thermo_combustion_hpp_
#include "auto_ptr.hpp"
#include "rho_combustion_model.hpp"
#include "rho_chemistry_model.hpp"
namespace mousse
{
namespace combustionModels
{
class rhoThermoCombustion
:
  public rhoCombustionModel
{
  // Private Member Functions
    //- Construct as copy (not implemented)
    rhoThermoCombustion(const rhoThermoCombustion&);
    //- Disallow default bitwise assignment
    void operator=(const rhoThermoCombustion&);
protected:
  // Protected data
    //- Pointer to thermo model
    autoPtr<rhoReactionThermo> thermoPtr_;
public:
  // Constructors
    //- Construct from components
    rhoThermoCombustion
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
  //- Destructor
  virtual ~rhoThermoCombustion();
  // Member Functions
    //- Return access to the thermo package
    virtual rhoReactionThermo& thermo();
    //- Return const access to the thermo package
    virtual const rhoReactionThermo& thermo() const;
    //- Return const access to the density field
    virtual tmp<volScalarField> rho() const;
};
}  // namespace combustionModels
}  // namespace mousse
#endif
