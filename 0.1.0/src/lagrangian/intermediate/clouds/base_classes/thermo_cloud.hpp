// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermoCloud
// Description
//   Virtual abstract base class for templated ThermoCloud
// SourceFiles
//   thermo_cloud.cpp
#ifndef thermo_cloud_hpp_
#define thermo_cloud_hpp_
#include "vol_fields.hpp"
namespace mousse
{
class thermoCloud
{
public:
  //- Runtime type information
  TYPE_NAME("thermoCloud");
  // Constructors
    //- Null constructor
    thermoCloud();
    //- Disallow default bitwise copy construct
    thermoCloud(const thermoCloud&) = delete;
    //- Disallow default bitwise assignment
    thermoCloud& operator=(const thermoCloud&) = delete;
  //- Destructor
  virtual ~thermoCloud();
  // Member Functions
    // Access
      // Radiation
        //- Return tmp equivalent particulate emission
        virtual tmp<volScalarField> Ep() const = 0;
        //- Return tmp equivalent particulate absorption
        virtual tmp<volScalarField> ap() const = 0;
        //- Return tmp equivalent particulate scattering factor
        virtual tmp<volScalarField> sigmap() const = 0;
};
}  // namespace mousse
#endif
