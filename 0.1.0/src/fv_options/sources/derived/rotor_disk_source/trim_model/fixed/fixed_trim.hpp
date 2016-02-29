#ifndef FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_TRIM_MODEL_FIXED_FIXED_TRIM_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_ROTOR_DISK_SOURCE_TRIM_MODEL_FIXED_FIXED_TRIM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedTrim
// Description
//   Fixed trim coefficients
// SourceFiles
//   fixed_trim.cpp
#include "trim_model.hpp"
namespace mousse
{
class fixedTrim
:
  public trimModel
{
protected:
  // Protected data
    //- Geometric angle of attack [rad]
    scalarField thetag_;
public:
  //- Run-time type information
  TYPE_NAME("fixedTrim");
  //- Constructor
  fixedTrim(const fv::rotorDiskSource& rotor, const dictionary& dict);
  //- Destructor
  virtual ~fixedTrim();
  // Member functions
    //- Read
    void read(const dictionary& dict);
    //- Return the geometric angle of attack [rad]
    virtual tmp<scalarField> thetag() const;
    //- Correct the model
    virtual void correct
    (
      const vectorField& U,
      vectorField& force
    );
    //- Correct the model for compressible flow
    virtual void correct
    (
      const volScalarField rho,
      const vectorField& U,
      vectorField& force
    );
};
}  // namespace mousse
#endif
