// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "contact_angle_force.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvc_grad.hpp"
#include "unit_conversion.hpp"
#include "fv_patch_field.hpp"
#include "mesh_wave_patch_dist_method.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(contactAngleForce, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(force, contactAngleForce, dictionary);
// Private Member Functions 
void contactAngleForce::initialise()
{
  const wordReList zeroForcePatches(coeffDict_.lookup("zeroForcePatches"));
  if (zeroForcePatches.size())
  {
    const polyBoundaryMesh& pbm = owner_.regionMesh().boundaryMesh();
    scalar dLim = readScalar(coeffDict_.lookup("zeroForceDistance"));
    Info<< "        Assigning zero contact force within " << dLim
      << " of patches:" << endl;
    labelHashSet patchIDs = pbm.patchSet(zeroForcePatches);
    FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter)
    {
      label patchI = iter.key();
      Info<< "            " << pbm[patchI].name() << endl;
    }
    // Temporary implementation until run-time selection covers this case
    patchDistMethods::meshWave dist(owner_.regionMesh(), patchIDs);
    volScalarField y
    (
      IOobject
      (
        "y",
        owner_.regionMesh().time().timeName(),
        owner_.regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      owner_.regionMesh(),
      dimensionedScalar("y", dimLength, GREAT)
    );
    dist.correct(y);
    mask_ = pos(y - dimensionedScalar("dLim", dimLength, dLim));
  }
}
// Constructors 
contactAngleForce::contactAngleForce
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  force(typeName, owner, dict),
  Ccf_(readScalar(coeffDict_.lookup("Ccf"))),
  rndGen_(label(0), -1),
  distribution_
  (
    distributionModels::distributionModel::New
    (
      coeffDict_.subDict("contactAngleDistribution"),
      rndGen_
    )
  ),
  mask_
  (
    IOobject
    (
      typeName + ":contactForceMask",
      owner_.time().timeName(),
      owner_.regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    ),
    owner_.regionMesh(),
    dimensionedScalar("mask", dimless, 1.0)
  )
{
  initialise();
}
// Destructor 
contactAngleForce::~contactAngleForce()
{}
// Member Functions 
tmp<fvVectorMatrix> contactAngleForce::correct(volVectorField& U)
{
  tmp<volVectorField> tForce
  (
    new volVectorField
    (
      IOobject
      (
        typeName + ":contactForce",
        owner_.time().timeName(),
        owner_.regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      owner_.regionMesh(),
      dimensionedVector("zero", dimForce/dimArea, vector::zero)
    )
  );
  vectorField& force = tForce().internalField();
  const labelUList& own = owner_.regionMesh().owner();
  const labelUList& nbr = owner_.regionMesh().neighbour();
  const scalarField& magSf = owner_.magSf();
  const volScalarField& alpha = owner_.alpha();
  const volScalarField& sigma = owner_.sigma();
  volVectorField gradAlpha(fvc::grad(alpha));
  FOR_ALL(nbr, faceI)
  {
    const label cellO = own[faceI];
    const label cellN = nbr[faceI];
    label cellI = -1;
    if ((alpha[cellO] > 0.5) && (alpha[cellN] < 0.5))
    {
      cellI = cellO;
    }
    else if ((alpha[cellO] < 0.5) && (alpha[cellN] > 0.5))
    {
      cellI = cellN;
    }
    if (cellI != -1 && mask_[cellI] > 0.5)
    {
      const scalar invDx = owner_.regionMesh().deltaCoeffs()[faceI];
      const vector n =
        gradAlpha[cellI]/(mag(gradAlpha[cellI]) + ROOTVSMALL);
      scalar theta = cos(degToRad(distribution_->sample()));
      force[cellI] += Ccf_*n*sigma[cellI]*(1.0 - theta)/invDx;
    }
  }
  FOR_ALL(alpha.boundaryField(), patchI)
  {
    if (!owner().isCoupledPatch(patchI))
    {
      const fvPatchField<scalar>& alphaf = alpha.boundaryField()[patchI];
      const fvPatchField<scalar>& maskf = mask_.boundaryField()[patchI];
      const scalarField& invDx = alphaf.patch().deltaCoeffs();
      const labelUList& faceCells = alphaf.patch().faceCells();
      FOR_ALL(alphaf, faceI)
      {
        if (maskf[faceI] > 0.5)
        {
          label cellO = faceCells[faceI];
          if ((alpha[cellO] > 0.5) && (alphaf[faceI] < 0.5))
          {
            const vector n =
              gradAlpha[cellO]
             /(mag(gradAlpha[cellO]) + ROOTVSMALL);
            scalar theta = cos(degToRad(distribution_->sample()));
            force[cellO] +=
              Ccf_*n*sigma[cellO]*(1.0 - theta)/invDx[faceI];
          }
        }
      }
    }
  }
  force /= magSf;
  if (owner_.regionMesh().time().outputTime())
  {
    tForce().write();
  }
  tmp<fvVectorMatrix>
    tfvm(new fvVectorMatrix(U, dimForce/dimArea*dimVolume));
  tfvm() += tForce;
  return tfvm;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
