// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "three_phase_interface_properties.hpp"
#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "mathematical_constants.hpp"
#include "surface_interpolate.hpp"
#include "fvc_div.hpp"
#include "fvc_grad.hpp"
#include "fvc_sn_grad.hpp"
// Static Member Data 
const mousse::scalar mousse::threePhaseInterfaceProperties::convertToRad =
  mousse::constant::mathematical::pi/180.0;
// Private Member Functions 
void mousse::threePhaseInterfaceProperties::correctContactAngle
(
  surfaceVectorField::GeometricBoundaryField& nHatb
) const
{
  const volScalarField::GeometricBoundaryField& alpha1 =
    mixture_.alpha1().boundaryField();
  const volScalarField::GeometricBoundaryField& alpha2 =
    mixture_.alpha2().boundaryField();
  const volScalarField::GeometricBoundaryField& alpha3 =
    mixture_.alpha3().boundaryField();
  const volVectorField::GeometricBoundaryField& U =
    mixture_.U().boundaryField();
  const fvMesh& mesh = mixture_.U().mesh();
  const fvBoundaryMesh& boundary = mesh.boundary();
  FOR_ALL(boundary, patchi)
  {
    if (isA<alphaContactAngleFvPatchScalarField>(alpha1[patchi]))
    {
      const alphaContactAngleFvPatchScalarField& a2cap =
        refCast<const alphaContactAngleFvPatchScalarField>
        (alpha2[patchi]);
      const alphaContactAngleFvPatchScalarField& a3cap =
        refCast<const alphaContactAngleFvPatchScalarField>
        (alpha3[patchi]);
      scalarField twoPhaseAlpha2{max(a2cap, scalar(0))};
      scalarField twoPhaseAlpha3{max(a3cap, scalar(0))};
      scalarField sumTwoPhaseAlpha
      {
        twoPhaseAlpha2 + twoPhaseAlpha3 + SMALL
      };
      twoPhaseAlpha2 /= sumTwoPhaseAlpha;
      twoPhaseAlpha3 /= sumTwoPhaseAlpha;
      fvsPatchVectorField& nHatp = nHatb[patchi];
      scalarField theta
      {
        convertToRad
        *(twoPhaseAlpha2*(180 - a2cap.theta(U[patchi], nHatp))
          + twoPhaseAlpha3*(180 - a3cap.theta(U[patchi], nHatp)))
      };
      vectorField nf{boundary[patchi].nf()};
      // Reset nHatPatch to correspond to the contact angle
      scalarField a12{nHatp & nf};
      scalarField b1{cos(theta)};
      scalarField b2{nHatp.size()};
      FOR_ALL(b2, facei)
      {
        b2[facei] = cos(acos(a12[facei]) - theta[facei]);
      }
      scalarField det{1.0 - a12*a12};
      scalarField a{(b1 - a12*b2)/det};
      scalarField b{(b2 - a12*b1)/det};
      nHatp = a*nf + b*nHatp;
      nHatp /= (mag(nHatp) + deltaN_.value());
    }
  }
}
void mousse::threePhaseInterfaceProperties::calculateK()
{
  const volScalarField& alpha1 = mixture_.alpha1();
  const fvMesh& mesh = alpha1.mesh();
  const surfaceVectorField& Sf = mesh.Sf();
  // Cell gradient of alpha
  volVectorField gradAlpha{fvc::grad(alpha1)};
  // Interpolated face-gradient of alpha
  surfaceVectorField gradAlphaf{fvc::interpolate(gradAlpha)};
  // Face unit interface normal
  surfaceVectorField nHatfv{gradAlphaf/(mag(gradAlphaf) + deltaN_)};
  correctContactAngle(nHatfv.boundaryField());
  // Face unit interface normal flux
  nHatf_ = nHatfv & Sf;
  // Simple expression for curvature
  K_ = -fvc::div(nHatf_);
  // Complex expression for curvature.
  // Correction is formally zero but numerically non-zero.
  // volVectorField nHat = gradAlpha/(mag(gradAlpha) + deltaN_);
  // nHat.boundaryField() = nHatfv.boundaryField();
  // K_ = -fvc::div(nHatf_) + (nHat & fvc::grad(nHatfv) & nHat);
}
// Constructors 
mousse::threePhaseInterfaceProperties::threePhaseInterfaceProperties
(
  const incompressibleThreePhaseMixture& mixture
)
:
  mixture_{mixture},
  cAlpha_
  {
    readScalar
    (
      mixture.U().mesh().solverDict
      (
        mixture_.alpha1().name()
      ).lookup("cAlpha")
    )
  },
  sigma12_{mixture.lookup("sigma12")},
  sigma13_{mixture.lookup("sigma13")},
  deltaN_
  {
    "deltaN",
    1e-8/pow(average(mixture.U().mesh().V()), 1.0/3.0)
  },
  nHatf_
  {
    {
      "nHatf",
      mixture.alpha1().time().timeName(),
      mixture.alpha1().mesh()
    },
    mixture.alpha1().mesh(),
    {"nHatf", dimArea, 0.0}
  },
  K_
  {
    {
      "interfaceProperties:K",
      mixture.alpha1().time().timeName(),
      mixture.alpha1().mesh()
    },
    mixture.alpha1().mesh(),
    {"K", dimless/dimLength, 0.0}
  }
{
  calculateK();
}
// Member Functions 
mousse::tmp<mousse::surfaceScalarField>
mousse::threePhaseInterfaceProperties::surfaceTensionForce() const
{
  return fvc::interpolate(sigmaK())*fvc::snGrad(mixture_.alpha1());
}
mousse::tmp<mousse::volScalarField>
mousse::threePhaseInterfaceProperties::nearInterface() const
{
  return max
  (
    pos(mixture_.alpha1() - 0.01)*pos(0.99 - mixture_.alpha1()),
    pos(mixture_.alpha2() - 0.01)*pos(0.99 - mixture_.alpha2())
  );
}
