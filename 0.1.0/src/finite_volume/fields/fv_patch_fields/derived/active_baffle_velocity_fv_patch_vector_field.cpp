// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "active_baffle_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "cyclic_fv_patch.hpp"
// Constructors 
mousse::activeBaffleVelocityFvPatchVectorField::
activeBaffleVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(p, iF),
  pName_("p"),
  cyclicPatchName_(),
  cyclicPatchLabel_(-1),
  orientation_(1),
  initWallSf_(0),
  initCyclicSf_(0),
  nbrCyclicSf_(0),
  openFraction_(0),
  openingTime_(0),
  maxOpenFractionDelta_(0),
  curTimeIndex_(-1)
{}
mousse::activeBaffleVelocityFvPatchVectorField::
activeBaffleVelocityFvPatchVectorField
(
  const activeBaffleVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField(ptf, p, iF, mapper),
  pName_(ptf.pName_),
  cyclicPatchName_(ptf.cyclicPatchName_),
  cyclicPatchLabel_(ptf.cyclicPatchLabel_),
  orientation_(ptf.orientation_),
  initWallSf_(ptf.initWallSf_),
  initCyclicSf_(ptf.initCyclicSf_),
  nbrCyclicSf_(ptf.nbrCyclicSf_),
  openFraction_(ptf.openFraction_),
  openingTime_(ptf.openingTime_),
  maxOpenFractionDelta_(ptf.maxOpenFractionDelta_),
  curTimeIndex_(-1)
{}
mousse::activeBaffleVelocityFvPatchVectorField::
activeBaffleVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField(p, iF),
  pName_(dict.lookupOrDefault<word>("p", "p")),
  cyclicPatchName_(dict.lookup("cyclicPatch")),
  cyclicPatchLabel_(p.patch().boundaryMesh().findPatchID(cyclicPatchName_)),
  orientation_(readLabel(dict.lookup("orientation"))),
  initWallSf_(p.Sf()),
  initCyclicSf_(p.boundaryMesh()[cyclicPatchLabel_].Sf()),
  nbrCyclicSf_
  (
    refCast<const cyclicFvPatch>
    (
      p.boundaryMesh()[cyclicPatchLabel_]
    ).neighbFvPatch().Sf()
  ),
  openFraction_(readScalar(dict.lookup("openFraction"))),
  openingTime_(readScalar(dict.lookup("openingTime"))),
  maxOpenFractionDelta_(readScalar(dict.lookup("maxOpenFractionDelta"))),
  curTimeIndex_(-1)
{
  fvPatchVectorField::operator=(vector::zero);
}
mousse::activeBaffleVelocityFvPatchVectorField::
activeBaffleVelocityFvPatchVectorField
(
  const activeBaffleVelocityFvPatchVectorField& ptf
)
:
  fixedValueFvPatchVectorField(ptf),
  pName_(ptf.pName_),
  cyclicPatchName_(ptf.cyclicPatchName_),
  cyclicPatchLabel_(ptf.cyclicPatchLabel_),
  orientation_(ptf.orientation_),
  initWallSf_(ptf.initWallSf_),
  initCyclicSf_(ptf.initCyclicSf_),
  nbrCyclicSf_(ptf.nbrCyclicSf_),
  openFraction_(ptf.openFraction_),
  openingTime_(ptf.openingTime_),
  maxOpenFractionDelta_(ptf.maxOpenFractionDelta_),
  curTimeIndex_(-1)
{}
mousse::activeBaffleVelocityFvPatchVectorField::
activeBaffleVelocityFvPatchVectorField
(
  const activeBaffleVelocityFvPatchVectorField& ptf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField(ptf, iF),
  pName_(ptf.pName_),
  cyclicPatchName_(ptf.cyclicPatchName_),
  cyclicPatchLabel_(ptf.cyclicPatchLabel_),
  orientation_(ptf.orientation_),
  initWallSf_(ptf.initWallSf_),
  initCyclicSf_(ptf.initCyclicSf_),
  nbrCyclicSf_(ptf.nbrCyclicSf_),
  openFraction_(ptf.openFraction_),
  openingTime_(ptf.openingTime_),
  maxOpenFractionDelta_(ptf.maxOpenFractionDelta_),
  curTimeIndex_(-1)
{}
// Member Functions 
void mousse::activeBaffleVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedValueFvPatchVectorField::autoMap(m);
  //- Note: cannot map field from cyclic patch anyway so just recalculate
  //  Areas should be consistent when doing autoMap except in case of
  //  topo changes.
  //- Note: we don't want to use Sf here since triggers rebuilding of
  //  fvMesh::S() which will give problems when mapped (since already
  //  on new mesh)
  const vectorField& areas = patch().boundaryMesh().mesh().faceAreas();
  initWallSf_ = patch().patchSlice(areas);
  initCyclicSf_ = patch().boundaryMesh()
  [
    cyclicPatchLabel_
  ].patchSlice(areas);
  nbrCyclicSf_ = refCast<const cyclicFvPatch>
  (
    patch().boundaryMesh()
    [
      cyclicPatchLabel_
    ]
  ).neighbFvPatch().patch().patchSlice(areas);
}
void mousse::activeBaffleVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  fixedValueFvPatchVectorField::rmap(ptf, addr);
  // See autoMap.
  const vectorField& areas = patch().boundaryMesh().mesh().faceAreas();
  initWallSf_ = patch().patchSlice(areas);
  initCyclicSf_ = patch().boundaryMesh()
  [
    cyclicPatchLabel_
  ].patchSlice(areas);
  nbrCyclicSf_ = refCast<const cyclicFvPatch>
  (
    patch().boundaryMesh()
    [
      cyclicPatchLabel_
    ]
  ).neighbFvPatch().patch().patchSlice(areas);
}
void mousse::activeBaffleVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  // Execute the change to the openFraction only once per time-step
  if (curTimeIndex_ != this->db().time().timeIndex())
  {
    const volScalarField& p = db().lookupObject<volScalarField>
    (
      pName_
    );
    const fvPatch& cyclicPatch = patch().boundaryMesh()[cyclicPatchLabel_];
    const labelList& cyclicFaceCells = cyclicPatch.patch().faceCells();
    const fvPatch& nbrPatch = refCast<const cyclicFvPatch>
    (
      cyclicPatch
    ).neighbFvPatch();
    const labelList& nbrFaceCells = nbrPatch.patch().faceCells();
    scalar forceDiff = 0;
    // Add this side
    forAll(cyclicFaceCells, facei)
    {
      forceDiff += p[cyclicFaceCells[facei]]*mag(initCyclicSf_[facei]);
    }
    // Remove other side
    forAll(nbrFaceCells, facei)
    {
      forceDiff -= p[nbrFaceCells[facei]]*mag(nbrCyclicSf_[facei]);
    }
    openFraction_ =
      max
      (
        min
        (
          openFraction_
         + min
          (
            this->db().time().deltaTValue()/openingTime_,
            maxOpenFractionDelta_
          )
         *(orientation_*sign(forceDiff)),
          1 - 1e-6
        ),
        1e-6
      );
    Info<< "openFraction = " << openFraction_ << endl;
    vectorField::subField Sfw = this->patch().patch().faceAreas();
    const vectorField newSfw((1 - openFraction_)*initWallSf_);
    forAll(Sfw, facei)
    {
      Sfw[facei] = newSfw[facei];
    }
    const_cast<scalarField&>(patch().magSf()) = mag(patch().Sf());
    // Update owner side of cyclic
    const_cast<vectorField&>(cyclicPatch.Sf()) =
      openFraction_*initCyclicSf_;
    const_cast<scalarField&>(cyclicPatch.magSf()) =
      mag(cyclicPatch.Sf());
    // Update neighbour side of cyclic
    const_cast<vectorField&>(nbrPatch.Sf()) =
      openFraction_*nbrCyclicSf_;
    const_cast<scalarField&>(nbrPatch.magSf()) =
      mag(nbrPatch.Sf());
    curTimeIndex_ = this->db().time().timeIndex();
  }
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::activeBaffleVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "p", "p", pName_);
  os.writeKeyword("cyclicPatch")
    << cyclicPatchName_ << token::END_STATEMENT << nl;
  os.writeKeyword("orientation")
    << orientation_ << token::END_STATEMENT << nl;
  os.writeKeyword("openingTime")
    << openingTime_ << token::END_STATEMENT << nl;
  os.writeKeyword("maxOpenFractionDelta")
    << maxOpenFractionDelta_ << token::END_STATEMENT << nl;
  os.writeKeyword("openFraction")
    << openFraction_ << token::END_STATEMENT << nl;
  writeEntryIfDifferent<word>(os, "p", "p", pName_);
  writeEntry("value", os);
}
namespace mousse
{
  makePatchTypeField
  (
    fvPatchVectorField,
    activeBaffleVelocityFvPatchVectorField
  );
}
