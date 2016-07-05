#ifndef THERMOPHYSICAL_MODELS_BAROTROPIC_COMPRESSIBILITY_MODEL_BAROTROPIC_COMPRESSIBILITY_MODEL_HPP_
#define THERMOPHYSICAL_MODELS_BAROTROPIC_COMPRESSIBILITY_MODEL_BAROTROPIC_COMPRESSIBILITY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::barotropicCompressibilityModel
// Description
//   Abstract class for barotropic compressibility models

#include "iodictionary.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "vol_fields.hpp"
#include "dimensioned_scalar.hpp"
#include "auto_ptr.hpp"


namespace mousse {

class barotropicCompressibilityModel
{
protected:
  // Protected data
    dictionary compressibilityProperties_;
    volScalarField psi_;
    const volScalarField& gamma_;
public:
  //- Runtime type information
  TYPE_NAME("barotropicCompressibilityModel");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      barotropicCompressibilityModel,
      dictionary,
      (
        const dictionary& compressibilityProperties,
        const volScalarField& gamma,
        const word& psiName
      ),
      (compressibilityProperties, gamma, psiName)
    );
  // Selectors
    //- Return a reference to the selected compressibility model
    static autoPtr<barotropicCompressibilityModel> New
    (
      const dictionary& compressibilityProperties,
      const volScalarField& gamma,
      const word& psiName = "psi"
    );
  // Constructors
    //- Construct from components
    barotropicCompressibilityModel
    (
      const dictionary& compressibilityProperties,
      const volScalarField& gamma,
      const word& psiName = "psi"
    );
    //- Disallow copy construct
    barotropicCompressibilityModel
    (
      const barotropicCompressibilityModel&
    ) = delete;
    //- Disallow default bitwise assignment
    barotropicCompressibilityModel& operator=
    (
      const barotropicCompressibilityModel&
    ) = delete;
  //- Destructor
  virtual ~barotropicCompressibilityModel()
  {}
  // Member Functions
    //- Return the phase transport properties dictionary
    const dictionary& compressibilityProperties() const
    {
      return compressibilityProperties_;
    }
    //- Return the compressibility
    const volScalarField& psi() const
    {
      return psi_;
    }
    //- Correct the compressibility
    virtual void correct() = 0;
    //- Read compressibilityProperties dictionary
    virtual bool read(const dictionary& compressibilityProperties) = 0;
};

}  // namespace mousse

#endif

