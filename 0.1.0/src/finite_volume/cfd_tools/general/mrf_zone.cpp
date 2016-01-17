// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mrf_zone.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrices.hpp"
#include "face_set.hpp"
#include "geometric_one_field.hpp"
#include "sync_tools.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(MRFZone, 0);
}
// Private Member Functions 
void mousse::MRFZone::setMRFFaces()
{
  const polyBoundaryMesh& patches = mesh_.boundaryMesh();
  // Type per face:
  //  0:not in zone
  //  1:moving with frame
  //  2:other
  labelList faceType(mesh_.nFaces(), 0);
  // Determine faces in cell zone
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // (without constructing cells)
  const labelList& own = mesh_.faceOwner();
  const labelList& nei = mesh_.faceNeighbour();
  // Cells in zone
  boolList zoneCell(mesh_.nCells(), false);
  if (cellZoneID_ != -1)
  {
    const labelList& cellLabels = mesh_.cellZones()[cellZoneID_];
    FOR_ALL(cellLabels, i)
    {
      zoneCell[cellLabels[i]] = true;
    }
  }
  label nZoneFaces = 0;
  for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++)
  {
    if (zoneCell[own[faceI]] || zoneCell[nei[faceI]])
    {
      faceType[faceI] = 1;
      nZoneFaces++;
    }
  }
  labelHashSet excludedPatches(excludedPatchLabels_);
  FOR_ALL(patches, patchI)
  {
    const polyPatch& pp = patches[patchI];
    if (pp.coupled() || excludedPatches.found(patchI))
    {
      FOR_ALL(pp, i)
      {
        label faceI = pp.start()+i;
        if (zoneCell[own[faceI]])
        {
          faceType[faceI] = 2;
          nZoneFaces++;
        }
      }
    }
    else if (!isA<emptyPolyPatch>(pp))
    {
      FOR_ALL(pp, i)
      {
        label faceI = pp.start()+i;
        if (zoneCell[own[faceI]])
        {
          faceType[faceI] = 1;
          nZoneFaces++;
        }
      }
    }
  }
  // Synchronize the faceType across processor patches
  syncTools::syncFaceList(mesh_, faceType, maxEqOp<label>());
  // Now we have for faceType:
  //  0   : face not in cellZone
  //  1   : internal face or normal patch face
  //  2   : coupled patch face or excluded patch face
  // Sort into lists per patch.
  internalFaces_.setSize(mesh_.nFaces());
  label nInternal = 0;
  for (label faceI = 0; faceI < mesh_.nInternalFaces(); faceI++)
  {
    if (faceType[faceI] == 1)
    {
      internalFaces_[nInternal++] = faceI;
    }
  }
  internalFaces_.setSize(nInternal);
  labelList nIncludedFaces(patches.size(), 0);
  labelList nExcludedFaces(patches.size(), 0);
  FOR_ALL(patches, patchi)
  {
    const polyPatch& pp = patches[patchi];
    FOR_ALL(pp, patchFacei)
    {
      label faceI = pp.start() + patchFacei;
      if (faceType[faceI] == 1)
      {
        nIncludedFaces[patchi]++;
      }
      else if (faceType[faceI] == 2)
      {
        nExcludedFaces[patchi]++;
      }
    }
  }
  includedFaces_.setSize(patches.size());
  excludedFaces_.setSize(patches.size());
  FOR_ALL(nIncludedFaces, patchi)
  {
    includedFaces_[patchi].setSize(nIncludedFaces[patchi]);
    excludedFaces_[patchi].setSize(nExcludedFaces[patchi]);
  }
  nIncludedFaces = 0;
  nExcludedFaces = 0;
  FOR_ALL(patches, patchi)
  {
    const polyPatch& pp = patches[patchi];
    FOR_ALL(pp, patchFacei)
    {
      label faceI = pp.start() + patchFacei;
      if (faceType[faceI] == 1)
      {
        includedFaces_[patchi][nIncludedFaces[patchi]++] = patchFacei;
      }
      else if (faceType[faceI] == 2)
      {
        excludedFaces_[patchi][nExcludedFaces[patchi]++] = patchFacei;
      }
    }
  }
  if (debug)
  {
    faceSet internalFaces(mesh_, "internalFaces", internalFaces_);
    Pout<< "Writing " << internalFaces.size()
      << " internal faces in MRF zone to faceSet "
      << internalFaces.name() << endl;
    internalFaces.write();
    faceSet MRFFaces(mesh_, "includedFaces", 100);
    FOR_ALL(includedFaces_, patchi)
    {
      FOR_ALL(includedFaces_[patchi], i)
      {
        label patchFacei = includedFaces_[patchi][i];
        MRFFaces.insert(patches[patchi].start()+patchFacei);
      }
    }
    Pout<< "Writing " << MRFFaces.size()
      << " patch faces in MRF zone to faceSet "
      << MRFFaces.name() << endl;
    MRFFaces.write();
    faceSet excludedFaces(mesh_, "excludedFaces", 100);
    FOR_ALL(excludedFaces_, patchi)
    {
      FOR_ALL(excludedFaces_[patchi], i)
      {
        label patchFacei = excludedFaces_[patchi][i];
        excludedFaces.insert(patches[patchi].start()+patchFacei);
      }
    }
    Pout<< "Writing " << excludedFaces.size()
      << " faces in MRF zone with special handling to faceSet "
      << excludedFaces.name() << endl;
    excludedFaces.write();
  }
}
// Constructors 
mousse::MRFZone::MRFZone
(
  const word& name,
  const fvMesh& mesh,
  const dictionary& dict,
  const word& cellZoneName
)
:
  mesh_(mesh),
  name_(name),
  coeffs_(dict),
  active_(coeffs_.lookupOrDefault("active", true)),
  cellZoneName_(cellZoneName),
  cellZoneID_(),
  excludedPatchNames_
  (
    wordReList(coeffs_.lookupOrDefault("nonRotatingPatches", wordReList()))
  ),
  origin_(coeffs_.lookup("origin")),
  axis_(coeffs_.lookup("axis")),
  omega_(DataEntry<scalar>::New("omega", coeffs_))
{
  if (cellZoneName_ == word::null)
  {
    coeffs_.lookup("cellZone") >> cellZoneName_;
  }
  if (!active_)
  {
    cellZoneID_ = -1;
  }
  else
  {
    cellZoneID_ = mesh_.cellZones().findZoneID(cellZoneName_);
    axis_ = axis_/mag(axis_);
    const labelHashSet excludedPatchSet
    (
      mesh_.boundaryMesh().patchSet(excludedPatchNames_)
    );
    excludedPatchLabels_.setSize(excludedPatchSet.size());
    label i = 0;
    FOR_ALL_CONST_ITER(labelHashSet, excludedPatchSet, iter)
    {
      excludedPatchLabels_[i++] = iter.key();
    }
    bool cellZoneFound = (cellZoneID_ != -1);
    reduce(cellZoneFound, orOp<bool>());
    if (!cellZoneFound)
    {
      FATAL_ERROR_IN
      (
        "MRFZone"
        "("
          "const word&, "
          "const fvMesh&, "
          "const dictionary&, "
          "const word&"
        ")"
      )
        << "cannot find MRF cellZone " << cellZoneName_
        << exit(FatalError);
    }
    setMRFFaces();
  }
}
// Member Functions 
mousse::vector mousse::MRFZone::Omega() const
{
  return omega_->value(mesh_.time().timeOutputValue())*axis_;
}
void mousse::MRFZone::addCoriolis
(
  const volVectorField& U,
  volVectorField& ddtU
) const
{
  if (cellZoneID_ == -1)
  {
    return;
  }
  const labelList& cells = mesh_.cellZones()[cellZoneID_];
  vectorField& ddtUc = ddtU.internalField();
  const vectorField& Uc = U.internalField();
  const vector Omega = this->Omega();
  FOR_ALL(cells, i)
  {
    label celli = cells[i];
    ddtUc[celli] += (Omega ^ Uc[celli]);
  }
}
void mousse::MRFZone::addCoriolis(fvVectorMatrix& UEqn, const bool rhs) const
{
  if (cellZoneID_ == -1)
  {
    return;
  }
  const labelList& cells = mesh_.cellZones()[cellZoneID_];
  const scalarField& V = mesh_.V();
  vectorField& Usource = UEqn.source();
  const vectorField& U = UEqn.psi();
  const vector Omega = this->Omega();
  if (rhs)
  {
    FOR_ALL(cells, i)
    {
      label celli = cells[i];
      Usource[celli] += V[celli]*(Omega ^ U[celli]);
    }
  }
  else
  {
    FOR_ALL(cells, i)
    {
      label celli = cells[i];
      Usource[celli] -= V[celli]*(Omega ^ U[celli]);
    }
  }
}
void mousse::MRFZone::addCoriolis
(
  const volScalarField& rho,
  fvVectorMatrix& UEqn,
  const bool rhs
) const
{
  if (cellZoneID_ == -1)
  {
    return;
  }
  const labelList& cells = mesh_.cellZones()[cellZoneID_];
  const scalarField& V = mesh_.V();
  vectorField& Usource = UEqn.source();
  const vectorField& U = UEqn.psi();
  const vector Omega = this->Omega();
  if (rhs)
  {
    FOR_ALL(cells, i)
    {
      label celli = cells[i];
      Usource[celli] += V[celli]*rho[celli]*(Omega ^ U[celli]);
    }
  }
  else
  {
    FOR_ALL(cells, i)
    {
      label celli = cells[i];
      Usource[celli] -= V[celli]*rho[celli]*(Omega ^ U[celli]);
    }
  }
}
void mousse::MRFZone::makeRelative(volVectorField& U) const
{
  const volVectorField& C = mesh_.C();
  const vector Omega = this->Omega();
  const labelList& cells = mesh_.cellZones()[cellZoneID_];
  FOR_ALL(cells, i)
  {
    label celli = cells[i];
    U[celli] -= (Omega ^ (C[celli] - origin_));
  }
  // Included patches
  FOR_ALL(includedFaces_, patchi)
  {
    FOR_ALL(includedFaces_[patchi], i)
    {
      label patchFacei = includedFaces_[patchi][i];
      U.boundaryField()[patchi][patchFacei] = vector::zero;
    }
  }
  // Excluded patches
  FOR_ALL(excludedFaces_, patchi)
  {
    FOR_ALL(excludedFaces_[patchi], i)
    {
      label patchFacei = excludedFaces_[patchi][i];
      U.boundaryField()[patchi][patchFacei] -=
        (Omega ^ (C.boundaryField()[patchi][patchFacei] - origin_));
    }
  }
}
void mousse::MRFZone::makeRelative(surfaceScalarField& phi) const
{
  makeRelativeRhoFlux(geometricOneField(), phi);
}
void mousse::MRFZone::makeRelative(FieldField<fvsPatchField, scalar>& phi) const
{
  makeRelativeRhoFlux(oneFieldField(), phi);
}
void mousse::MRFZone::makeRelative
(
  const surfaceScalarField& rho,
  surfaceScalarField& phi
) const
{
  makeRelativeRhoFlux(rho, phi);
}
void mousse::MRFZone::makeAbsolute(volVectorField& U) const
{
  const volVectorField& C = mesh_.C();
  const vector Omega = this->Omega();
  const labelList& cells = mesh_.cellZones()[cellZoneID_];
  FOR_ALL(cells, i)
  {
    label celli = cells[i];
    U[celli] += (Omega ^ (C[celli] - origin_));
  }
  // Included patches
  FOR_ALL(includedFaces_, patchi)
  {
    FOR_ALL(includedFaces_[patchi], i)
    {
      label patchFacei = includedFaces_[patchi][i];
      U.boundaryField()[patchi][patchFacei] =
        (Omega ^ (C.boundaryField()[patchi][patchFacei] - origin_));
    }
  }
  // Excluded patches
  FOR_ALL(excludedFaces_, patchi)
  {
    FOR_ALL(excludedFaces_[patchi], i)
    {
      label patchFacei = excludedFaces_[patchi][i];
      U.boundaryField()[patchi][patchFacei] +=
        (Omega ^ (C.boundaryField()[patchi][patchFacei] - origin_));
    }
  }
}
void mousse::MRFZone::makeAbsolute(surfaceScalarField& phi) const
{
  makeAbsoluteRhoFlux(geometricOneField(), phi);
}
void mousse::MRFZone::makeAbsolute
(
  const surfaceScalarField& rho,
  surfaceScalarField& phi
) const
{
  makeAbsoluteRhoFlux(rho, phi);
}
void mousse::MRFZone::correctBoundaryVelocity(volVectorField& U) const
{
  const vector Omega = this->Omega();
  // Included patches
  FOR_ALL(includedFaces_, patchi)
  {
    const vectorField& patchC = mesh_.Cf().boundaryField()[patchi];
    vectorField pfld(U.boundaryField()[patchi]);
    FOR_ALL(includedFaces_[patchi], i)
    {
      label patchFacei = includedFaces_[patchi][i];
      pfld[patchFacei] = (Omega ^ (patchC[patchFacei] - origin_));
    }
    U.boundaryField()[patchi] == pfld;
  }
}
void mousse::MRFZone::writeData(Ostream& os) const
{
  os << nl;
  os.write(name_) << nl;
  os << token::BEGIN_BLOCK << incrIndent << nl;
  os.writeKeyword("active") << active_ << token::END_STATEMENT << nl;
  os.writeKeyword("cellZone") << cellZoneName_ << token::END_STATEMENT << nl;
  os.writeKeyword("origin") << origin_ << token::END_STATEMENT << nl;
  os.writeKeyword("axis") << axis_ << token::END_STATEMENT << nl;
  omega_->writeData(os);
  if (excludedPatchNames_.size())
  {
    os.writeKeyword("nonRotatingPatches") << excludedPatchNames_
      << token::END_STATEMENT << nl;
  }
  os << decrIndent << token::END_BLOCK << nl;
}
bool mousse::MRFZone::read(const dictionary& dict)
{
  coeffs_ = dict;
  active_ = coeffs_.lookupOrDefault("active", true);
  coeffs_.lookup("cellZone") >> cellZoneName_;
  cellZoneID_ = mesh_.cellZones().findZoneID(cellZoneName_);
  return true;
}
