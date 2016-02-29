#ifndef TRANSPORT_MODELS_INTERFACE_PROPERTIES_INTERFACE_PROPERTIES_HPP_
#define TRANSPORT_MODELS_INTERFACE_PROPERTIES_INTERFACE_PROPERTIES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interfaceProperties
// Description
//   Contains the interface properties.
//   Properties to aid interFoam:
//   -# Correct the alpha boundary condition for dynamic contact angle.
//   -# Calculate interface curvature.
// SourceFiles
//   interface_properties.cpp
#include "iodictionary.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class interfaceProperties
{
  // Private data
    //- Keep a reference to the transportProperties dictionary
    const dictionary& transportPropertiesDict_;
    //- Compression coefficient
    scalar cAlpha_;
    //- Surface tension
    dimensionedScalar sigma_;
    //- Stabilisation for normalisation of the interface normal
    const dimensionedScalar deltaN_;
    const volScalarField& alpha1_;
    const volVectorField& U_;
    surfaceScalarField nHatf_;
    volScalarField K_;
  // Private Member Functions
    //- Correction for the boundary condition on the unit normal nHat on
    //  walls to produce the correct contact dynamic angle
    //  calculated from the component of U parallel to the wall
    void correctContactAngle
    (
      surfaceVectorField::GeometricBoundaryField& nHat,
      surfaceVectorField::GeometricBoundaryField& gradAlphaf
    ) const;
    //- Re-calculate the interface curvature
    void calculateK();
public:
  //- Conversion factor for degrees into radians
  static const scalar convertToRad;
  // Constructors
    //- Construct from volume fraction field gamma and IOdictionary
    interfaceProperties
    (
      const volScalarField& alpha1,
      const volVectorField& U,
      const IOdictionary&
    );
    //- Disallow default bitwise copy construct and assignment
    interfaceProperties(const interfaceProperties&) = delete;
    interfaceProperties& operator=(const interfaceProperties&) = delete;
  // Member Functions
    scalar cAlpha() const
    {
      return cAlpha_;
    }
    const dimensionedScalar& deltaN() const
    {
      return deltaN_;
    }
    const surfaceScalarField& nHatf() const
    {
      return nHatf_;
    }
    const volScalarField& K() const
    {
      return K_;
    }
    const dimensionedScalar& sigma() const
    {
      return sigma_;
    }
    tmp<volScalarField> sigmaK() const
    {
      return sigma_*K_;
    }
    tmp<surfaceScalarField> surfaceTensionForce() const;
    //- Indicator of the proximity of the interface
    //  Field values are 1 near and 0 away for the interface.
    tmp<volScalarField> nearInterface() const;
    void correct()
    {
      calculateK();
    }
    //- Read transportProperties dictionary
    bool read();
};
}  // namespace mousse
#endif
