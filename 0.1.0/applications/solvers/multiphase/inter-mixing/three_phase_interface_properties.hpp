// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::threePhaseInterfaceProperties
// Description
//   Properties to aid interFoam :
//   1. Correct the alpha boundary condition for dynamic contact angle.
//   2. Calculate interface curvature.
// SourceFiles
//   three_phase_interface_properties.cpp
#ifndef THREE_PHASE_INTERFACE_PROPERTIES_HPP_
#define THREE_PHASE_INTERFACE_PROPERTIES_HPP_
#include "incompressible_three_phase_mixture.hpp"
#include "surface_fields.hpp"
namespace mousse
{
class threePhaseInterfaceProperties
{
  // Private data
    const incompressibleThreePhaseMixture& mixture_;
    //- Compression coefficient
    scalar cAlpha_;
    //- Surface tension 1-2
    dimensionedScalar sigma12_;
    //- Surface tension 1-3
    dimensionedScalar sigma13_;
    //- Stabilisation for normalisation of the interface normal
    const dimensionedScalar deltaN_;
    surfaceScalarField nHatf_;
    volScalarField K_;
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    threePhaseInterfaceProperties(const threePhaseInterfaceProperties&);
    void operator=(const threePhaseInterfaceProperties&);
    //- Correction for the boundary condition on the unit normal nHat on
    //  walls to produce the correct contact dynamic angle.
    //  Calculated from the component of U parallel to the wall
    void correctContactAngle
    (
      surfaceVectorField::GeometricBoundaryField& nHat
    ) const;
    //- Re-calculate the interface curvature
    void calculateK();
public:
  //- Conversion factor for degrees into radians
  static const scalar convertToRad;
  // Constructors
    //- Construct from volume fraction field alpha and IOdictionary
    threePhaseInterfaceProperties
    (
      const incompressibleThreePhaseMixture& mixture
    );
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
    tmp<volScalarField> sigma() const
    {
      volScalarField limitedAlpha2{max(mixture_.alpha2(), scalar(0))};
      volScalarField limitedAlpha3{max(mixture_.alpha3(), scalar(0))};
      return
        (limitedAlpha2*sigma12_ + limitedAlpha3*sigma13_)
       /(limitedAlpha2 + limitedAlpha3 + SMALL);
    }
    tmp<volScalarField> sigmaK() const
    {
      return sigma()*K_;
    }
    tmp<surfaceScalarField> surfaceTensionForce() const;
    //- Indicator of the proximity of the interface
    //  Field values are 1 near and 0 away for the interface.
    tmp<volScalarField> nearInterface() const;
    void correct()
    {
      calculateK();
    }
};
}  // namespace mousse
#endif
