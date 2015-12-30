// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rhoCombustionModel
// Description
//   Combustion models for rho-based thermodynamics
// SourceFiles
//   rho_combustion_model_i.hpp
//   rho_combustion_model.cpp
//   rho_combustion_model_new.cpp
#ifndef rho_combustion_model_hpp_
#define rho_combustion_model_hpp_
#include "combustion_model.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "rho_reaction_thermo.hpp"
namespace mousse
{
namespace combustionModels
{
class rhoCombustionModel
:
  public combustionModel
{
  // Private Member Functions
    //- Construct as copy (not implemented)
    rhoCombustionModel(const rhoCombustionModel&);
    //- Disallow default bitwise assignment
    void operator=(const rhoCombustionModel&);
public:
  //- Runtime type information
  TypeName("rhoCombustionModel");
  //- Declare run-time constructor selection tables
  declareRunTimeSelectionTable
  (
    autoPtr,
    rhoCombustionModel,
    dictionary,
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    ),
    (modelType, mesh, phaseName)
  );
  // Constructors
    //- Construct from components
    rhoCombustionModel
    (
      const word& modelType,
      const fvMesh& mesh,
      const word& phaseName
    );
  //- Selector
  static autoPtr<rhoCombustionModel> New
  (
    const fvMesh& mesh,
    const word& phaseName=word::null
  );
  //- Destructor
  virtual ~rhoCombustionModel();
  // Member Functions
    // Access functions
      //- Access combustion dict
      inline const dictionary& coeff() const;
    //- Return access to the thermo package
    virtual rhoReactionThermo& thermo() = 0;
    //- Return const access to the thermo package
    virtual const rhoReactionThermo& thermo() const = 0;
    //- Return tmp of rho
    virtual tmp<volScalarField> rho() const = 0;
  // IO
    //- Update properties from given dictionary
    virtual bool read();
};
}  // namespace combustionModels
}  // namespace mousse
#endif
