// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::radiation::radiativeIntensityRay
// Description
//   Radiation intensity for a ray in a given direction
// SourceFiles
//   radiative_intensity_ray.cpp
#ifndef radiative_intensity_ray_hpp_
#define radiative_intensity_ray_hpp_
#include "absorption_emission_model.hpp"
#include "black_body_emission.hpp"
namespace mousse
{
namespace radiation
{
// Forward declaration of classes
class fvDOM;
class radiativeIntensityRay
{
public:
  static const word intensityPrefix;
private:
  // Private data
    //- Refence to the owner fvDOM object
    const fvDOM& dom_;
    //- Reference to the mesh
    const fvMesh& mesh_;
    //- Absorption/emission model
    const absorptionEmissionModel& absorptionEmission_;
    //- Black body
    const blackBodyEmission& blackBody_;
    //- Total radiative intensity / [W/m2]
    volScalarField I_;
    //- Total radiative heat flux on boundary
    volScalarField Qr_;
    //- Incident radiative heat flux on boundary
    volScalarField Qin_;
    //- Emitted radiative heat flux on boundary
    volScalarField Qem_;
    //- Direction
    vector d_;
    //- Average direction vector inside the solid angle
    vector dAve_;
    //- Theta angle
    scalar theta_;
    //- Phi angle
    scalar phi_;
    //- Solid angle
    scalar omega_;
    //- Number of wavelengths/bands
    label nLambda_;
    //- List of pointers to radiative intensity fields for given wavelengths
    PtrList<volScalarField> ILambda_;
    //- Global ray id - incremented in constructor
    static label rayId;
    //- My ray Id
    label myRayId_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    radiativeIntensityRay(const radiativeIntensityRay&);
    //- Disallow default bitwise assignment
    void operator=(const radiativeIntensityRay&);
public:
  // Constructors
    //- Construct form components
    radiativeIntensityRay
    (
      const fvDOM& dom,
      const fvMesh& mesh,
      const scalar phi,
      const scalar theta,
      const scalar deltaPhi,
      const scalar deltaTheta,
      const label lambda,
      const absorptionEmissionModel& absEmmModel_,
      const blackBodyEmission& blackBody,
      const label rayId
    );
  //- Destructor
  ~radiativeIntensityRay();
  // Member functions
    // Edit
      //- Update radiative intensity on i direction
      scalar correct();
      //- Initialise the ray in i direction
      void init
      (
        const scalar phi,
        const scalar theta,
        const scalar deltaPhi,
        const scalar deltaTheta,
        const scalar lambda
      );
      //- Add radiative intensities from all the bands
      void addIntensity();
    // Access
      //- Return intensity
      inline const volScalarField& I() const;
      //- Return const access to the boundary heat flux
      inline const volScalarField& Qr() const;
      //- Return non-const access to the boundary heat flux
      inline volScalarField& Qr();
      //- Return non-const access to the boundary incident heat flux
      inline volScalarField& Qin();
      //- Return non-const access to the boundary emmited heat flux
      inline volScalarField& Qem();
      //- Return const access to the boundary incident heat flux
      inline const volScalarField& Qin() const;
      //- Return const access to the boundary emmited heat flux
      inline const volScalarField& Qem() const;
      //- Return direction
      inline const vector& d() const;
      //- Return the average vector inside the solid angle
      inline const vector& dAve() const;
      //- Return the number of bands
      inline scalar nLambda() const;
      //- Return the phi angle
      inline scalar phi() const;
      //- Return the theta angle
      inline scalar theta() const;
      //- Return the solid angle
      inline scalar omega() const;
      //- Return the radiative intensity for a given wavelength
      inline const volScalarField& ILambda(const label lambdaI) const;
};
}  // namespace radiation
}  // namespace mousse
#include "radiative_intensity_ray_i.hpp"
#endif
