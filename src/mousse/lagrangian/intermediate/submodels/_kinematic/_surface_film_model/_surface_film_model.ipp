// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_surface_film_model.hpp"
#include "surface_film_model.hpp"
#include "mathematical_constants.hpp"


using namespace mousse::constant;


// Constructors 
template<class CloudType>
mousse::SurfaceFilmModel<CloudType>::SurfaceFilmModel(CloudType& owner)
:
  CloudSubModelBase<CloudType>{owner},
  g_{owner.g()},
  ejectedParcelType_{0},
  massParcelPatch_{0},
  diameterParcelPatch_{0},
  UFilmPatch_{0},
  rhoFilmPatch_{0},
  deltaFilmPatch_{0},
  nParcelsTransferred_{0},
  nParcelsInjected_{0}
{}


template<class CloudType>
mousse::SurfaceFilmModel<CloudType>::SurfaceFilmModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>{owner, dict, typeName, type},
  g_{owner.g()},
  ejectedParcelType_
  {
    this->coeffDict().lookupOrDefault("ejectedParcelType", -1)
  },
  massParcelPatch_{0},
  diameterParcelPatch_{0},
  UFilmPatch_{0},
  rhoFilmPatch_{0},
  deltaFilmPatch_{owner.mesh().boundary().size()},
  nParcelsTransferred_{0},
  nParcelsInjected_{0}
{}


template<class CloudType>
mousse::SurfaceFilmModel<CloudType>::SurfaceFilmModel
(
  const SurfaceFilmModel<CloudType>& sfm
)
:
  CloudSubModelBase<CloudType>{sfm},
  g_{sfm.g_},
  ejectedParcelType_{sfm.ejectedParcelType_},
  massParcelPatch_{sfm.massParcelPatch_},
  diameterParcelPatch_{sfm.diameterParcelPatch_},
  UFilmPatch_{sfm.UFilmPatch_},
  rhoFilmPatch_{sfm.rhoFilmPatch_},
  deltaFilmPatch_{sfm.deltaFilmPatch_},
  nParcelsTransferred_{sfm.nParcelsTransferred_},
  nParcelsInjected_{sfm.nParcelsInjected_}
{}


// Destructor 
template<class CloudType>
mousse::SurfaceFilmModel<CloudType>::~SurfaceFilmModel()
{}


// Member Functions 
template<class CloudType>
template<class TrackData>
void mousse::SurfaceFilmModel<CloudType>::inject(TrackData& td)
{
  if (!this->active()) {
    return;
  }
  // Retrieve the film model from the owner database
  const regionModels::surfaceFilmModels::surfaceFilmModel& filmModel =
    this->owner().mesh().time().objectRegistry::template lookupObject
    <regionModels::surfaceFilmModels::surfaceFilmModel>
    (
      "surfaceFilmProperties"
    );
  if (!filmModel.active()) {
    return;
  }
  const labelList& filmPatches = filmModel.intCoupledPatchIDs();
  const labelList& primaryPatches = filmModel.primaryPatchIDs();
  const fvMesh& mesh = this->owner().mesh();
  const polyBoundaryMesh& pbm = mesh.boundaryMesh();
  FOR_ALL(filmPatches, i) {
    const label filmPatchI = filmPatches[i];
    const label primaryPatchI = primaryPatches[i];
    const labelList& injectorCellsPatch = pbm[primaryPatchI].faceCells();
    cacheFilmFields(filmPatchI, primaryPatchI, filmModel);
    const vectorField& Cf = mesh.C().boundaryField()[primaryPatchI];
    const vectorField& Sf = mesh.Sf().boundaryField()[primaryPatchI];
    const scalarField& magSf = mesh.magSf().boundaryField()[primaryPatchI];
    FOR_ALL(injectorCellsPatch, j) {
      if (diameterParcelPatch_[j] > 0) {
        const label cellI = injectorCellsPatch[j];
        // The position could bein any tet of the decomposed cell,
        // so arbitrarily choose the first face of the cell as the
        // tetFace and the first point on the face after the base
        // point as the tetPt.  The tracking will pick the cell
        // consistent with the motion in the first tracking step.
        const label tetFaceI = this->owner().mesh().cells()[cellI][0];
        const label tetPtI = 1;
        const scalar offset =
          max
          (
            diameterParcelPatch_[j],
            deltaFilmPatch_[primaryPatchI][j]
          );
        const point pos = Cf[j] - 1.1*offset*Sf[j]/magSf[j];
        // Create a new parcel
        parcelType* pPtr =
          new parcelType
          {
            this->owner().pMesh(),
            pos,
            cellI,
            tetFaceI,
            tetPtI
          };
        // Check/set new parcel thermo properties
        td.cloud().setParcelThermoProperties(*pPtr, 0.0);
        setParcelProperties(*pPtr, j);
        if (pPtr->nParticle() > 0.001) {
          // Check new parcel properties
  //                td.cloud().checkParcelProperties(*pPtr, 0.0, true);
          td.cloud().checkParcelProperties(*pPtr, 0.0, false);
          // Add the new parcel to the cloud
          td.cloud().addParticle(pPtr);
          nParcelsInjected_++;
        } else {
          // TODO: cache mass and re-distribute?
          delete pPtr;
        }
      }
    }
  }
}


template<class CloudType>
void mousse::SurfaceFilmModel<CloudType>::cacheFilmFields
(
  const label filmPatchI,
  const label primaryPatchI,
  const regionModels::surfaceFilmModels::surfaceFilmModel& filmModel
)
{
  massParcelPatch_ = filmModel.cloudMassTrans().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, massParcelPatch_);
  diameterParcelPatch_ =
    filmModel.cloudDiameterTrans().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, diameterParcelPatch_, maxEqOp<scalar>());
  UFilmPatch_ = filmModel.Us().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, UFilmPatch_);
  rhoFilmPatch_ = filmModel.rho().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, rhoFilmPatch_);
  deltaFilmPatch_[primaryPatchI] =
    filmModel.delta().boundaryField()[filmPatchI];
  filmModel.toPrimary(filmPatchI, deltaFilmPatch_[primaryPatchI]);
}


template<class CloudType>
void mousse::SurfaceFilmModel<CloudType>::setParcelProperties
(
  parcelType& p,
  const label filmFaceI
) const
{
  // Set parcel properties
  scalar vol = mathematical::pi/6.0*pow3(diameterParcelPatch_[filmFaceI]);
  p.d() = diameterParcelPatch_[filmFaceI];
  p.U() = UFilmPatch_[filmFaceI];
  p.rho() = rhoFilmPatch_[filmFaceI];
  p.nParticle() = massParcelPatch_[filmFaceI]/p.rho()/vol;
  if (ejectedParcelType_ >= 0) {
    p.typeId() = ejectedParcelType_;
  }
}


template<class CloudType>
void mousse::SurfaceFilmModel<CloudType>::info(Ostream& os)
{
  label nTrans0 =
    this->template getModelProperty<label>("nParcelsTransferred");
  label nInject0 =
    this->template getModelProperty<label>("nParcelsInjected");
  label nTransTotal =
    nTrans0 + returnReduce(nParcelsTransferred_, sumOp<label>());
  label nInjectTotal =
    nInject0 + returnReduce(nParcelsInjected_, sumOp<label>());
  os << "    Parcels absorbed into film      = " << nTransTotal << nl
    << "    New film detached parcels       = " << nInjectTotal << endl;
  if (this->outputTime()) {
    this->setModelProperty("nParcelsTransferred", nTransTotal);
    this->setModelProperty("nParcelsInjected", nInjectTotal);
    nParcelsTransferred_ = 0;
    nParcelsInjected_ = 0;
  }
}

#include "_surface_film_model_new.ipp"

