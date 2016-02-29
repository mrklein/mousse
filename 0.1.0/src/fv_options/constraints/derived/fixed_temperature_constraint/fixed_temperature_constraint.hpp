#ifndef FV_OPTIONS_CONSTRAINTS_DERIVED_FIXED_TEMPERATURE_CONSTRAINT_FIXED_TEMPERATURE_CONSTRAINT_HPP_
#define FV_OPTIONS_CONSTRAINTS_DERIVED_FIXED_TEMPERATURE_CONSTRAINT_FIXED_TEMPERATURE_CONSTRAINT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::fixedTemperatureConstraint
// Description
//   Fixed temperature equation constraint
//   Sources described by:
//     fixedTemperatureConstraintCoeffs
//     {
//       mode            uniform;    // uniform or lookup
//       // uniform option
//       temperature     constant 500; // fixed temperature with time [K]
//       // lookup option
//       // TName        T;          // optional temperature field name
//     }
// Note:
//   The 'uniform' option allows the use of a time-varying uniform temperature
//   by means of the DataEntry type.
// SourceFiles
//   fv_option.cpp
#include "cell_set_option.hpp"
#include "named_enum.hpp"
#include "data_entry.hpp"
namespace mousse
{
namespace fv
{
class fixedTemperatureConstraint
:
  public cellSetOption
{
public:
  //- Temperature mode
  enum temperatureMode
  {
    tmUniform,
    tmLookup
  };
  //- String representation of temperatureMode enums
  static const NamedEnum<temperatureMode, 2> temperatureModeNames_;
protected:
  // Protected data
    //- Operation mode
    temperatureMode mode_;
    //- Uniform temperature [K]
    autoPtr<DataEntry<scalar> > Tuniform_;
    //- Temperature field name
    word TName_;
public:
  //- Runtime type information
  TYPE_NAME("fixedTemperatureConstraint");
  // Constructors
    //- Construct from components
    fixedTemperatureConstraint
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
    //- Disallow default bitwise copy construct
    fixedTemperatureConstraint(const fixedTemperatureConstraint&) = delete;
    //- Disallow default bitwise assignment
    fixedTemperatureConstraint& operator=(const fixedTemperatureConstraint&) = delete;
  //- Destructor
  virtual ~fixedTemperatureConstraint()
  {}
  // Member Functions
    //- Constrain energy equation to fix the temperature
    virtual void constrain(fvMatrix<scalar>& eqn, const label fieldI);
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#endif
