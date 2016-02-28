// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interfaceCompositionModel
// Description
//   Generic base class for interface composition models. These models describe
//   the composition in phase 1 of the supplied pair at the interface with phase
//   2.
// SourceFiles
//   interface_composition_model.cpp
#ifndef INTERFACE_COMPOSITION_MODEL_HPP_
#define INTERFACE_COMPOSITION_MODEL_HPP_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "hashed_word_list.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phaseModel;
class phasePair;
class interfaceCompositionModel
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
    //- Names of the transferring species
    const hashedWordList speciesNames_;
public:
  //- Runtime type information
  TYPE_NAME("interfaceCompositionModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      interfaceCompositionModel,
      dictionary,
      (
        const dictionary& dict,
        const phasePair& pair
      ),
      (dict, pair)
    );
  // Constructors
    //- Construct from a dictionary and a phase pair
    interfaceCompositionModel
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~interfaceCompositionModel();
  // Selectors
    static autoPtr<interfaceCompositionModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- Update the composition
    virtual void update(const volScalarField& Tf) = 0;
    //- Return the transferring species names
    const hashedWordList& species() const;
    //- Returns whether the species is transported by the model and
    //  provides the name of the diffused species
    bool transports
    (
      word& speciesName
    ) const;
    //- Interface mass fraction
    virtual tmp<volScalarField> Yf
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const = 0;
    //- The interface mass fraction derivative w.r.t. temperature
    virtual tmp<volScalarField> YfPrime
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const = 0;
    //- Mass fraction difference between the interface and the field
    virtual tmp<volScalarField> dY
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const = 0;
    //- Mass diffusivity
    virtual tmp<volScalarField> D
    (
      const word& speciesName
    ) const = 0;
    //- Latent heat
    virtual tmp<volScalarField> L
    (
      const word& speciesName,
      const volScalarField& Tf
    ) const = 0;
    //- Add latent heat flow rate to total
    virtual void addMDotL
    (
      const volScalarField& K,
      const volScalarField& Tf,
      volScalarField& mDotL,
      volScalarField& mDotLPrime
    ) const = 0;
};
}  // namespace mousse
#endif
