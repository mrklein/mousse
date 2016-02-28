// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::massTransferModel
// Description
// SourceFiles
//   mass_transfer_model.cpp
#ifndef mass_transfer_model_hpp_
#define mass_transfer_model_hpp_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class massTransferModel
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
public:
  //- Runtime type information
  TYPE_NAME("massTransferModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      massTransferModel,
      dictionary,
      (
        const dictionary& dict,
        const phasePair& pair
      ),
      (dict, pair)
    );
  // Static data members
    //- Coefficient dimensions
    static const dimensionSet dimK;
  // Constructors
    //- Construct from a dictionary and a phase pair
    massTransferModel
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~massTransferModel();
  // Selectors
    static autoPtr<massTransferModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- The implicit mass transfer coefficient
    //  Note: this has had the species mass diffusivity factored out
    virtual tmp<volScalarField> K() const = 0;
};
}  // namespace mousse
#endif
