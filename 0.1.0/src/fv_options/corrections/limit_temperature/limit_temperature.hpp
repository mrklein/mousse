// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::limitTemperature
// Description
//   Correction for temperature to apply limits between minimum and maximum
//   values
//   Constraint described by:
//     limitTemperatureCoeffs
//     {
//       minimum     200;
//       maximum     500;
//     }
// SourceFiles
//   limit_temperature.cpp
#ifndef limit_temperature_hpp_
#define limit_temperature_hpp_
#include "cell_set_option.hpp"
namespace mousse
{
namespace fv
{
class limitTemperature
:
  public cellSetOption
{
protected:
  // Protected data
    //- Minimum temperature limit [K]
    scalar Tmin_;
    //- Maximum temperature limit [K]
    scalar Tmax_;
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    limitTemperature(const limitTemperature&);
    //- Disallow default bitwise assignment
    void operator=(const limitTemperature&);
public:
  //- Runtime type information
  TypeName("limitTemperature");
  // Constructors
    //- Construct from components
    limitTemperature
    (
      const word& name,
      const word& modelType,
      const dictionary& dict,
      const fvMesh& mesh
    );
  //- Destructor
  virtual ~limitTemperature()
  {}
  // Member Functions
    // Evaluate
      //- Correct the energy field
      virtual void correct(volScalarField& he);
    // IO
      //- Read dictionary
      virtual bool read(const dictionary& dict);
};
}  // namespace fv
}  // namespace mousse
#endif
