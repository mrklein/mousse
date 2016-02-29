#ifndef FV_OPTIONS_SOURCES_DERIVED_TABULATED_ACCELERATION_SOURCE_TABULATED_6DOF_ACCELERATION_TABULATED_6DOF_ACCELERATION_HPP_
#define FV_OPTIONS_SOURCES_DERIVED_TABULATED_ACCELERATION_SOURCE_TABULATED_6DOF_ACCELERATION_TABULATED_6DOF_ACCELERATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::tabulated6DoFAcceleration
// Description
//   Tabulated 6DoF acceleration.
//   Obtained by interpolating tabulated data for linear acceleration,
//   angular velocity and angular acceleration.
// SourceFiles
//   tabulated_6dof_acceleration.cpp
#include "primitive_fields.hpp"
#include "vector_2d.hpp"
#include "time.hpp"
namespace mousse
{
class tabulated6DoFAcceleration
{
  // Private data
    const Time& time_;
    dictionary accelerationCoeffs_;
    //- Time data file name read from dictionary
    fileName timeDataFileName_;
    //- Type used to read in the acceleration "vectors"
    typedef Vector<vector> accelerationVectors;
    //- Field of times
    scalarField times_;
    //- Field of acceleration "vectors"
    Field<accelerationVectors> values_;
public:
  //- Runtime type information
  TYPE_NAME("tabulated6DoFAcceleration");
  // Constructors
    //- Construct from components
    tabulated6DoFAcceleration
    (
      const dictionary& accelerationCoeffs,
      const Time& runTime
    );
    //- Disallow copy construct
    tabulated6DoFAcceleration(const tabulated6DoFAcceleration&) = delete;
    //- Disallow default bitwise assignment
    tabulated6DoFAcceleration& operator=
    (
      const tabulated6DoFAcceleration&
    ) = delete;
  //- Destructor
  virtual ~tabulated6DoFAcceleration();
  // Member Functions
    //- Return the solid-body accelerations
    virtual Vector<vector> acceleration() const;
    //- Update properties from given dictionary
    virtual bool read(const dictionary& accelerationCoeffs);
};
}  // namespace mousse
#endif
