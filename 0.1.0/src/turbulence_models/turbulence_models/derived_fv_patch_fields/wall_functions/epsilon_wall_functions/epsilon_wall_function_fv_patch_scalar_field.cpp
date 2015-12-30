// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "epsilon_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "fv_matrix.hpp"
#include "vol_fields.hpp"
#include "wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
mousse::scalar mousse::epsilonWallFunctionFvPatchScalarField::tolerance_ = 1e-5;
// Protected Member Functions 
void mousse::epsilonWallFunctionFvPatchScalarField::checkType()
{
  if (!isA<wallFvPatch>(patch()))
  {
    FatalErrorIn("epsilonWallFunctionFvPatchScalarField::checkType()")
      << "Invalid wall function specification" << nl
      << "    Patch type for patch " << patch().name()
      << " must be wall" << nl
      << "    Current patch type is " << patch().type() << nl << endl
      << abort(FatalError);
  }
}
void mousse::epsilonWallFunctionFvPatchScalarField::writeLocalEntries
(
  Ostream& os
) const
{
  os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
  os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
  os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
}
void mousse::epsilonWallFunctionFvPatchScalarField::setMaster()
{
  if (master_ != -1)
  {
    return;
  }
  const volScalarField& epsilon =
    static_cast<const volScalarField&>(this->dimensionedInternalField());
  const volScalarField::GeometricBoundaryField& bf = epsilon.boundaryField();
  label master = -1;
  forAll(bf, patchi)
  {
    if (isA<epsilonWallFunctionFvPatchScalarField>(bf[patchi]))
    {
      epsilonWallFunctionFvPatchScalarField& epf = epsilonPatch(patchi);
      if (master == -1)
      {
        master = patchi;
      }
      epf.master() = master;
    }
  }
}
void mousse::epsilonWallFunctionFvPatchScalarField::createAveragingWeights()
{
  const volScalarField& epsilon =
    static_cast<const volScalarField&>(this->dimensionedInternalField());
  const volScalarField::GeometricBoundaryField& bf = epsilon.boundaryField();
  const fvMesh& mesh = epsilon.mesh();
  if (initialised_ && !mesh.changing())
  {
    return;
  }
  volScalarField weights
  (
    IOobject
    (
      "weights",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false // do not register
    ),
    mesh,
    dimensionedScalar("zero", dimless, 0.0)
  );
  DynamicList<label> epsilonPatches(bf.size());
  forAll(bf, patchi)
  {
    if (isA<epsilonWallFunctionFvPatchScalarField>(bf[patchi]))
    {
      epsilonPatches.append(patchi);
      const labelUList& faceCells = bf[patchi].patch().faceCells();
      forAll(faceCells, i)
      {
        weights[faceCells[i]]++;
      }
    }
  }
  cornerWeights_.setSize(bf.size());
  forAll(epsilonPatches, i)
  {
    label patchi = epsilonPatches[i];
    const fvPatchScalarField& wf = weights.boundaryField()[patchi];
    cornerWeights_[patchi] = 1.0/wf.patchInternalField();
  }
  G_.setSize(dimensionedInternalField().size(), 0.0);
  epsilon_.setSize(dimensionedInternalField().size(), 0.0);
  initialised_ = true;
}
mousse::epsilonWallFunctionFvPatchScalarField&
mousse::epsilonWallFunctionFvPatchScalarField::epsilonPatch(const label patchi)
{
  const volScalarField& epsilon =
    static_cast<const volScalarField&>(this->dimensionedInternalField());
  const volScalarField::GeometricBoundaryField& bf = epsilon.boundaryField();
  const epsilonWallFunctionFvPatchScalarField& epf =
    refCast<const epsilonWallFunctionFvPatchScalarField>(bf[patchi]);
  return const_cast<epsilonWallFunctionFvPatchScalarField&>(epf);
}
void mousse::epsilonWallFunctionFvPatchScalarField::calculateTurbulenceFields
(
  const turbulenceModel& turbulence,
  scalarField& G0,
  scalarField& epsilon0
)
{
  // accumulate all of the G and epsilon contributions
  forAll(cornerWeights_, patchi)
  {
    if (!cornerWeights_[patchi].empty())
    {
      epsilonWallFunctionFvPatchScalarField& epf = epsilonPatch(patchi);
      const List<scalar>& w = cornerWeights_[patchi];
      epf.calculate(turbulence, w, epf.patch(), G0, epsilon0);
    }
  }
  // apply zero-gradient condition for epsilon
  forAll(cornerWeights_, patchi)
  {
    if (!cornerWeights_[patchi].empty())
    {
      epsilonWallFunctionFvPatchScalarField& epf = epsilonPatch(patchi);
      epf == scalarField(epsilon0, epf.patch().faceCells());
    }
  }
}
void mousse::epsilonWallFunctionFvPatchScalarField::calculate
(
  const turbulenceModel& turbulence,
  const List<scalar>& cornerWeights,
  const fvPatch& patch,
  scalarField& G,
  scalarField& epsilon
)
{
  const label patchi = patch.index();
  const scalarField& y = turbulence.y()[patchi];
  const scalar Cmu25 = pow025(Cmu_);
  const scalar Cmu75 = pow(Cmu_, 0.75);
  const tmp<volScalarField> tk = turbulence.k();
  const volScalarField& k = tk();
  const tmp<scalarField> tnuw = turbulence.nu(patchi);
  const scalarField& nuw = tnuw();
  const tmp<scalarField> tnutw = turbulence.nut(patchi);
  const scalarField& nutw = tnutw();
  const fvPatchVectorField& Uw = turbulence.U().boundaryField()[patchi];
  const scalarField magGradUw(mag(Uw.snGrad()));
  // Set epsilon and G
  forAll(nutw, facei)
  {
    label celli = patch.faceCells()[facei];
    scalar w = cornerWeights[facei];
    epsilon[celli] += w*Cmu75*pow(k[celli], 1.5)/(kappa_*y[facei]);
    G[celli] +=
      w
     *(nutw[facei] + nuw[facei])
     *magGradUw[facei]
     *Cmu25*sqrt(k[celli])
     /(kappa_*y[facei]);
  }
}
// Constructors 
mousse::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>(p, iF),
  Cmu_(0.09),
  kappa_(0.41),
  E_(9.8),
  G_(),
  epsilon_(),
  initialised_(false),
  master_(-1),
  cornerWeights_()
{
  checkType();
}
mousse::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
  const epsilonWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchField<scalar>(ptf, p, iF, mapper),
  Cmu_(ptf.Cmu_),
  kappa_(ptf.kappa_),
  E_(ptf.E_),
  G_(),
  epsilon_(),
  initialised_(false),
  master_(-1),
  cornerWeights_()
{
  checkType();
}
mousse::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchField<scalar>(p, iF, dict),
  Cmu_(dict.lookupOrDefault<scalar>("Cmu", 0.09)),
  kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
  E_(dict.lookupOrDefault<scalar>("E", 9.8)),
  G_(),
  epsilon_(),
  initialised_(false),
  master_(-1),
  cornerWeights_()
{
  checkType();
  // apply zero-gradient condition on start-up
  this->operator==(patchInternalField());
}
mousse::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
  const epsilonWallFunctionFvPatchScalarField& ewfpsf
)
:
  fixedValueFvPatchField<scalar>(ewfpsf),
  Cmu_(ewfpsf.Cmu_),
  kappa_(ewfpsf.kappa_),
  E_(ewfpsf.E_),
  G_(),
  epsilon_(),
  initialised_(false),
  master_(-1),
  cornerWeights_()
{
  checkType();
}
mousse::epsilonWallFunctionFvPatchScalarField::
epsilonWallFunctionFvPatchScalarField
(
  const epsilonWallFunctionFvPatchScalarField& ewfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchField<scalar>(ewfpsf, iF),
  Cmu_(ewfpsf.Cmu_),
  kappa_(ewfpsf.kappa_),
  E_(ewfpsf.E_),
  G_(),
  epsilon_(),
  initialised_(false),
  master_(-1),
  cornerWeights_()
{
  checkType();
}
// Member Functions 
mousse::scalarField& mousse::epsilonWallFunctionFvPatchScalarField::G(bool init)
{
  if (patch().index() == master_)
  {
    if (init)
    {
      G_ = 0.0;
    }
    return G_;
  }
  return epsilonPatch(master_).G();
}
mousse::scalarField& mousse::epsilonWallFunctionFvPatchScalarField::epsilon
(
  bool init
)
{
  if (patch().index() == master_)
  {
    if (init)
    {
      epsilon_ = 0.0;
    }
    return epsilon_;
  }
  return epsilonPatch(master_).epsilon(init);
}
void mousse::epsilonWallFunctionFvPatchScalarField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
  (
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      dimensionedInternalField().group()
    )
  );
  setMaster();
  if (patch().index() == master_)
  {
    createAveragingWeights();
    calculateTurbulenceFields(turbModel, G(true), epsilon(true));
  }
  const scalarField& G0 = this->G();
  const scalarField& epsilon0 = this->epsilon();
  typedef DimensionedField<scalar, volMesh> FieldType;
  FieldType& G =
    const_cast<FieldType&>
    (
      db().lookupObject<FieldType>(turbModel.GName())
    );
  FieldType& epsilon = const_cast<FieldType&>(dimensionedInternalField());
  forAll(*this, facei)
  {
    label celli = patch().faceCells()[facei];
    G[celli] = G0[celli];
    epsilon[celli] = epsilon0[celli];
  }
  fvPatchField<scalar>::updateCoeffs();
}
void mousse::epsilonWallFunctionFvPatchScalarField::updateCoeffs
(
  const scalarField& weights
)
{
  if (updated())
  {
    return;
  }
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
  (
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      dimensionedInternalField().group()
    )
  );
  setMaster();
  if (patch().index() == master_)
  {
    createAveragingWeights();
    calculateTurbulenceFields(turbModel, G(true), epsilon(true));
  }
  const scalarField& G0 = this->G();
  const scalarField& epsilon0 = this->epsilon();
  typedef DimensionedField<scalar, volMesh> FieldType;
  FieldType& G =
    const_cast<FieldType&>
    (
      db().lookupObject<FieldType>(turbModel.GName())
    );
  FieldType& epsilon = const_cast<FieldType&>(dimensionedInternalField());
  scalarField& epsilonf = *this;
  // only set the values if the weights are > tolerance
  forAll(weights, facei)
  {
    scalar w = weights[facei];
    if (w > tolerance_)
    {
      label celli = patch().faceCells()[facei];
      G[celli] = (1.0 - w)*G[celli] + w*G0[celli];
      epsilon[celli] = (1.0 - w)*epsilon[celli] + w*epsilon0[celli];
      epsilonf[facei] = epsilon[celli];
    }
  }
  fvPatchField<scalar>::updateCoeffs();
}
void mousse::epsilonWallFunctionFvPatchScalarField::manipulateMatrix
(
  fvMatrix<scalar>& matrix
)
{
  if (manipulatedMatrix())
  {
    return;
  }
  matrix.setValues(patch().faceCells(), patchInternalField());
  fvPatchField<scalar>::manipulateMatrix(matrix);
}
void mousse::epsilonWallFunctionFvPatchScalarField::manipulateMatrix
(
  fvMatrix<scalar>& matrix,
  const Field<scalar>& weights
)
{
  if (manipulatedMatrix())
  {
    return;
  }
  DynamicList<label> constraintCells(weights.size());
  DynamicList<scalar> constraintEpsilon(weights.size());
  const labelUList& faceCells = patch().faceCells();
  const DimensionedField<scalar, volMesh>& epsilon
    = dimensionedInternalField();
  label nConstrainedCells = 0;
  forAll(weights, facei)
  {
    // only set the values if the weights are > tolerance
    if (weights[facei] > tolerance_)
    {
      nConstrainedCells++;
      label celli = faceCells[facei];
      constraintCells.append(celli);
      constraintEpsilon.append(epsilon[celli]);
    }
  }
  if (debug)
  {
    Pout<< "Patch: " << patch().name()
      << ": number of constrained cells = " << nConstrainedCells
      << " out of " << patch().size()
      << endl;
  }
  matrix.setValues
  (
    constraintCells,
    scalarField(constraintEpsilon.xfer())
  );
  fvPatchField<scalar>::manipulateMatrix(matrix);
}
void mousse::epsilonWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  writeLocalEntries(os);
  fixedValueFvPatchField<scalar>::write(os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchScalarField,
    epsilonWallFunctionFvPatchScalarField
  );
}
