// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interface_properties.hpp"
#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
#include "mathematical_constants.hpp"
#include "surface_interpolate.hpp"
#include "fvc_div.hpp"
#include "fvc_grad.hpp"
#include "fvc_sn_grad.hpp"
// Static Member Data 
const mousse::scalar mousse::interfaceProperties::convertToRad =
  mousse::constant::mathematical::pi/180.0;
// Private Member Functions 
// Correction for the boundary condition on the unit normal nHat on
// walls to produce the correct contact angle.
// The dynamic contact angle is calculated from the component of the
// velocity on the direction of the interface, parallel to the wall.
void mousse::interfaceProperties::correctContactAngle
(
  surfaceVectorField::GeometricBoundaryField& nHatb,
  surfaceVectorField::GeometricBoundaryField& gradAlphaf
) const
{
  const fvMesh& mesh = alpha1_.mesh();
  const volScalarField::GeometricBoundaryField& abf = alpha1_.boundaryField();
  const fvBoundaryMesh& boundary = mesh.boundary();
  forAll(boundary, patchi)
  {
    if (isA<alphaContactAngleFvPatchScalarField>(abf[patchi]))
    {
      alphaContactAngleFvPatchScalarField& acap =
        const_cast<alphaContactAngleFvPatchScalarField&>
        (
          refCast<const alphaContactAngleFvPatchScalarField>
          (
            abf[patchi]
          )
        );
      fvsPatchVectorField& nHatp = nHatb[patchi];
      const scalarField theta
      (
        convertToRad*acap.theta(U_.boundaryField()[patchi], nHatp)
      );
      const vectorField nf
      (
        boundary[patchi].nf()
      );
      // Reset nHatp to correspond to the contact angle
      const scalarField a12(nHatp & nf);
      const scalarField b1(cos(theta));
      scalarField b2(nHatp.size());
      forAll(b2, facei)
      {
        b2[facei] = cos(acos(a12[facei]) - theta[facei]);
      }
      const scalarField det(1.0 - a12*a12);
      scalarField a((b1 - a12*b2)/det);
      scalarField b((b2 - a12*b1)/det);
      nHatp = a*nf + b*nHatp;
      nHatp /= (mag(nHatp) + deltaN_.value());
      acap.gradient() = (nf & nHatp)*mag(gradAlphaf[patchi]);
      acap.evaluate();
    }
  }
}
void mousse::interfaceProperties::calculateK()
{
  const fvMesh& mesh = alpha1_.mesh();
  const surfaceVectorField& Sf = mesh.Sf();
  // Cell gradient of alpha
  const volVectorField gradAlpha(fvc::grad(alpha1_, "nHat"));
  // Interpolated face-gradient of alpha
  surfaceVectorField gradAlphaf(fvc::interpolate(gradAlpha));
  //gradAlphaf -=
  //    (mesh.Sf()/mesh.magSf())
  //   *(fvc::snGrad(alpha1_) - (mesh.Sf() & gradAlphaf)/mesh.magSf());
  // Face unit interface normal
  surfaceVectorField nHatfv(gradAlphaf/(mag(gradAlphaf) + deltaN_));
  // surfaceVectorField nHatfv
  // (
  //     (gradAlphaf + deltaN_*vector(0, 0, 1)
  //    *sign(gradAlphaf.component(vector::Z)))/(mag(gradAlphaf) + deltaN_)
  // );
  correctContactAngle(nHatfv.boundaryField(), gradAlphaf.boundaryField());
  // Face unit interface normal flux
  nHatf_ = nHatfv & Sf;
  // Simple expression for curvature
  K_ = -fvc::div(nHatf_);
  // Complex expression for curvature.
  // Correction is formally zero but numerically non-zero.
  /*
  volVectorField nHat(gradAlpha/(mag(gradAlpha) + deltaN_));
  forAll(nHat.boundaryField(), patchi)
  {
    nHat.boundaryField()[patchi] = nHatfv.boundaryField()[patchi];
  }
  K_ = -fvc::div(nHatf_) + (nHat & fvc::grad(nHatfv) & nHat);
  */
}
// Constructors 
mousse::interfaceProperties::interfaceProperties
(
  const volScalarField& alpha1,
  const volVectorField& U,
  const IOdictionary& dict
)
:
  transportPropertiesDict_(dict),
  cAlpha_
  (
    readScalar
    (
      alpha1.mesh().solverDict(alpha1.name()).lookup("cAlpha")
    )
  ),
  sigma_("sigma", dimensionSet(1, 0, -2, 0, 0), dict),
  deltaN_
  (
    "deltaN",
    1e-8/pow(average(alpha1.mesh().V()), 1.0/3.0)
  ),
  alpha1_(alpha1),
  U_(U),
  nHatf_
  (
    IOobject
    (
      "nHatf",
      alpha1_.time().timeName(),
      alpha1_.mesh()
    ),
    alpha1_.mesh(),
    dimensionedScalar("nHatf", dimArea, 0.0)
  ),
  K_
  (
    IOobject
    (
      "interfaceProperties:K",
      alpha1_.time().timeName(),
      alpha1_.mesh()
    ),
    alpha1_.mesh(),
    dimensionedScalar("K", dimless/dimLength, 0.0)
  )
{
  calculateK();
}
// Member Functions 
mousse::tmp<mousse::surfaceScalarField>
mousse::interfaceProperties::surfaceTensionForce() const
{
  return fvc::interpolate(sigmaK())*fvc::snGrad(alpha1_);
}
mousse::tmp<mousse::volScalarField>
mousse::interfaceProperties::nearInterface() const
{
  return pos(alpha1_ - 0.01)*pos(0.99 - alpha1_);
}
bool mousse::interfaceProperties::read()
{
  alpha1_.mesh().solverDict(alpha1_.name()).lookup("cAlpha") >> cAlpha_;
  transportPropertiesDict_.lookup("sigma") >> sigma_;
  return true;
}
