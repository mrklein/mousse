// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heatTransferModel
// Description
// SourceFiles
//   heat_transfer_model.cpp
#ifndef heat_transfer_model_hpp_
#define heat_transfer_model_hpp_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class phasePair;
class heatTransferModel
{
protected:
  // Protected data
    //- Phase pair
    const phasePair& pair_;
    //- Residual phase fraction
    const dimensionedScalar residualAlpha_;
public:
  //- Runtime type information
  TYPE_NAME("heatTransferModel");
  // Declare runtime construction
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      heatTransferModel,
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
    //- Construct froma dictionary and a phase pair
    heatTransferModel
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~heatTransferModel();
  // Selectors
    static autoPtr<heatTransferModel> New
    (
      const dictionary& dict,
      const phasePair& pair
    );
  // Member Functions
    //- The heat transfer function K used in the enthalpy equation
    //    ddt(alpha1*rho1*ha) + ... = ... K*(Ta - Tb)
    //    ddt(alpha2*rho2*hb) + ... = ... K*(Tb - Ta)
    virtual tmp<volScalarField> K() const = 0;
};
}  // namespace mousse
#endif
