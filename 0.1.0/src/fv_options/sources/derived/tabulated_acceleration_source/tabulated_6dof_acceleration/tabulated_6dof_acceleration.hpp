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
#ifndef tabulated_6dof_acceleration_hpp_
#define tabulated_6dof_acceleration_hpp_
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
  // Private Member Functions
    //- Disallow copy construct
    tabulated6DoFAcceleration(const tabulated6DoFAcceleration&);
    //- Disallow default bitwise assignment
    void operator=(const tabulated6DoFAcceleration&);
public:
  //- Runtime type information
  TypeName("tabulated6DoFAcceleration");
  // Constructors
    //- Construct from components
    tabulated6DoFAcceleration
    (
      const dictionary& accelerationCoeffs,
      const Time& runTime
    );
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
