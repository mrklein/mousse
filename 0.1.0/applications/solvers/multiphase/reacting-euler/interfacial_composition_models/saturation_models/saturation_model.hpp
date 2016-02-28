// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::saturationModel
// Description
// SourceFiles
//   saturation_model.cpp
//   new_saturation_model.cpp
#ifndef saturation_model_hpp_
#define saturation_model_hpp_
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class saturationModel
{
public:
  //- Runtime type information
  TYPE_NAME("saturationModel");
  //- Declare runtime construction
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    saturationModel,
    dictionary,
    (
      const dictionary& dict
    ),
    (dict)
  );
  // Constructors
    //- Construct null
    saturationModel();
    //- Disallow default bitwise copy construct
    saturationModel(const saturationModel&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const saturationModel&) = delete;
  // Selectors
    //- Select null constructed
    static autoPtr<saturationModel> New(const dictionary& dict);
  //- Destructor
  virtual ~saturationModel();
  // Member Functions
    //- Saturation pressure
    virtual tmp<volScalarField> pSat
    (
      const volScalarField& T
    ) const = 0;
    //- Saturation pressure derivetive w.r.t. temperature
    virtual tmp<volScalarField> pSatPrime
    (
      const volScalarField& T
    ) const = 0;
    //- Natural log of the saturation pressure
    virtual tmp<volScalarField> lnPSat
    (
      const volScalarField& T
    ) const = 0;
    //- Saturation temperature
    virtual tmp<volScalarField> Tsat
    (
      const volScalarField& p
    ) const = 0;
};
}  // namespace mousse
#endif
