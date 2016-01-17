// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleStressModel
// Description
//   Base class for inter-particle stress models.
// SourceFiles
//   particle_stress_model.cpp
#ifndef particle_stress_model_hpp_
#define particle_stress_model_hpp_
#include "field.hpp"
#include "field_field.hpp"
#include "dictionary.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class ParticleStressModel
{
protected:
  // Protected data
    //- Close pack volume fraction
    scalar alphaPacked_;
public:
  //- Runtime type information
  TYPE_NAME("particleStressModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    ParticleStressModel,
    dictionary,
    (const dictionary& dict),
    (dict)
  );
  //- Constructors
    //- Construct from components
    ParticleStressModel(const dictionary& dict);
    //- Construct a copy
    ParticleStressModel(const ParticleStressModel& sm);
    //- Construct and return a clone
    virtual autoPtr<ParticleStressModel> clone() const = 0;
    //- Disallow default bitwise assignment
    ParticleStressModel& operator=(const ParticleStressModel&) = delete;
  //- Selector
  static autoPtr<ParticleStressModel> New
  (
    const dictionary& dict
  );
  //- Destructor
  virtual ~ParticleStressModel();
  //- Member Functions
    //- Access max volume fraction
    scalar alphaPacked() const;
    //- Collision stress
    virtual tmp<Field<scalar> > tau
    (
      const Field<scalar>& alpha,
      const Field<scalar>& rho,
      const Field<scalar>& uRms
    ) const = 0;
    //- Collision stress derivaive w.r.t. the volume fraction
    virtual tmp<Field<scalar> > dTaudTheta
    (
      const Field<scalar>& alpha,
      const Field<scalar>& rho,
      const Field<scalar>& uRms
    ) const = 0;
    //- Collision stress using FieldFields
    tmp<FieldField<Field, scalar> > tau
    (
      const FieldField<Field, scalar>& alpha,
      const FieldField<Field, scalar>& rho,
      const FieldField<Field, scalar>& uRms
    ) const;
};
}  // namespace mousse
#endif
