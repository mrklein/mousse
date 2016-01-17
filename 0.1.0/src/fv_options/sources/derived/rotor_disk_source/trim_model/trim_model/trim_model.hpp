// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::trimModel
// Description
//   Trim model base class
// SourceFiles
//   trim_model.cpp
#ifndef trim_model_hpp_
#define trim_model_hpp_
#include "rotor_disk_source.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class trimModel
{
protected:
  // Protected data
    //- Reference to the rotor source model
    const fv::rotorDiskSource& rotor_;
    //- Name of model
    const word name_;
    //- Coefficients dictionary
    dictionary coeffs_;
public:
  //- Run-time type information
  TYPE_NAME("trimModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      trimModel,
      dictionary,
      (
        const fv::rotorDiskSource& rotor,
        const dictionary& dict
      ),
      (rotor, dict)
    );
  // Constructors
    //- Construct from components
    trimModel
    (
      const fv::rotorDiskSource& rotor,
      const dictionary& dict,
      const word& name
    );
  // Selectors
    //- Return a reference to the selected trim model
    static autoPtr<trimModel> New
    (
      const fv::rotorDiskSource& rotor,
      const dictionary& dict
    );
  //- Destructor
  virtual ~trimModel();
  // Member functions
    //- Read
    virtual void read(const dictionary& dict);
    //- Return the geometric angle of attack [rad]
    virtual tmp<scalarField> thetag() const = 0;
    //- Correct the model
    virtual void correct
    (
      const vectorField& U,
      vectorField& force
    ) = 0;
    //- Correct the model for compressible flow
    virtual void correct
    (
      const volScalarField rho,
      const vectorField& U,
      vectorField& force
    ) = 0;
};
}  // namespace mousse
#endif
