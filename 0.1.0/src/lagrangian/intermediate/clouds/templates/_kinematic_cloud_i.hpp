// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvm_sup.hpp"
#include "sortable_list.hpp"
// Member Functions 
template<class CloudType>
inline const mousse::KinematicCloud<CloudType>&
mousse::KinematicCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const mousse::fvMesh& mousse::KinematicCloud<CloudType>::mesh() const
{
  return mesh_;
}
template<class CloudType>
inline const mousse::IOdictionary&
mousse::KinematicCloud<CloudType>::particleProperties() const
{
  return particleProperties_;
}
template<class CloudType>
inline const mousse::IOdictionary&
mousse::KinematicCloud<CloudType>::outputProperties() const
{
  return outputProperties_;
}
template<class CloudType>
inline mousse::IOdictionary& mousse::KinematicCloud<CloudType>::outputProperties()
{
  return outputProperties_;
}
template<class CloudType>
inline const mousse::cloudSolution&
mousse::KinematicCloud<CloudType>::solution() const
{
  return solution_;
}
template<class CloudType>
inline mousse::cloudSolution& mousse::KinematicCloud<CloudType>::solution()
{
  return solution_;
}
template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::KinematicCloud<CloudType>::constProps() const
{
  return constProps_;
}
template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::KinematicCloud<CloudType>::constProps()
{
  return constProps_;
}
template<class CloudType>
inline const mousse::dictionary&
mousse::KinematicCloud<CloudType>::subModelProperties() const
{
  return subModelProperties_;
}
template<class CloudType>
inline const mousse::volScalarField& mousse::KinematicCloud<CloudType>::rho() const
{
  return rho_;
}
template<class CloudType>
inline const mousse::volVectorField& mousse::KinematicCloud<CloudType>::U() const
{
  return U_;
}
template<class CloudType>
inline const mousse::volScalarField& mousse::KinematicCloud<CloudType>::mu() const
{
  return mu_;
}
template<class CloudType>
inline const mousse::dimensionedVector& mousse::KinematicCloud<CloudType>::g() const
{
  return g_;
}
template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::pAmbient() const
{
  return pAmbient_;
}
template<class CloudType>
inline mousse::scalar& mousse::KinematicCloud<CloudType>::pAmbient()
{
  return pAmbient_;
}
template<class CloudType>
//inline const typename CloudType::parcelType::forceType&
inline const typename mousse::KinematicCloud<CloudType>::forceType&
mousse::KinematicCloud<CloudType>::forces() const
{
  return forces_;
}
template<class CloudType>
inline typename mousse::KinematicCloud<CloudType>::forceType&
mousse::KinematicCloud<CloudType>::forces()
{
  return forces_;
}
template<class CloudType>
inline typename mousse::KinematicCloud<CloudType>::functionType&
mousse::KinematicCloud<CloudType>::functions()
{
  return functions_;
}
template<class CloudType>
inline const mousse::InjectionModelList<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::injectors() const
{
  return injectors_;
}
template<class CloudType>
inline mousse::InjectionModelList<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::injectors()
{
  return injectors_;
}
template<class CloudType>
inline const mousse::DispersionModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::dispersion() const
{
  return dispersionModel_;
}
template<class CloudType>
inline mousse::DispersionModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::dispersion()
{
  return dispersionModel_();
}
template<class CloudType>
inline const mousse::PatchInteractionModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::patchInteraction() const
{
  return patchInteractionModel_;
}
template<class CloudType>
inline mousse::PatchInteractionModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::patchInteraction()
{
  return patchInteractionModel_();
}
template<class CloudType>
inline const mousse::StochasticCollisionModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::stochasticCollision() const
{
  return stochasticCollisionModel_();
}
template<class CloudType>
inline mousse::StochasticCollisionModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::stochasticCollision()
{
  return stochasticCollisionModel_();
}
template<class CloudType>
inline const mousse::SurfaceFilmModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::surfaceFilm() const
{
  return surfaceFilmModel_();
}
template<class CloudType>
inline mousse::SurfaceFilmModel<mousse::KinematicCloud<CloudType> >&
mousse::KinematicCloud<CloudType>::surfaceFilm()
{
  return surfaceFilmModel_();
}
template<class CloudType>
inline const mousse::vectorIntegrationScheme&
mousse::KinematicCloud<CloudType>::UIntegrator() const
{
  return UIntegrator_;
}
template<class CloudType>
inline mousse::label mousse::KinematicCloud<CloudType>::nParcels() const
{
  return this->size();
}
template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::massInSystem() const
{
  scalar sysMass = 0.0;
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    sysMass += p.nParticle()*p.mass();
  }
  return sysMass;
}
template<class CloudType>
inline mousse::vector
mousse::KinematicCloud<CloudType>::linearMomentumOfSystem() const
{
  vector linearMomentum(vector::zero);
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    linearMomentum += p.nParticle()*p.mass()*p.U();
  }
  return linearMomentum;
}
template<class CloudType>
inline mousse::scalar
mousse::KinematicCloud<CloudType>::linearKineticEnergyOfSystem() const
{
  scalar linearKineticEnergy = 0.0;
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    linearKineticEnergy += p.nParticle()*0.5*p.mass()*(p.U() & p.U());
  }
  return linearKineticEnergy;
}
template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::Dij
(
  const label i,
  const label j
) const
{
  scalar si = 0.0;
  scalar sj = 0.0;
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    si += p.nParticle()*pow(p.d(), i);
    sj += p.nParticle()*pow(p.d(), j);
  }
  reduce(si, sumOp<scalar>());
  reduce(sj, sumOp<scalar>());
  sj = max(sj, VSMALL);
  return si/sj;
}
template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::Dmax() const
{
  scalar d = -GREAT;
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    d = max(d, p.d());
  }
  reduce(d, maxOp<scalar>());
  return max(0.0, d);
}
template<class CloudType>
inline mousse::scalar mousse::KinematicCloud<CloudType>::penetration
(
  const scalar fraction
) const
{
  if ((fraction < 0) || (fraction > 1))
  {
    FatalErrorIn
    (
      "inline mousse::scalar mousse::KinematicCloud<CloudType>::penetration"
      "("
        "const scalar"
      ") const"
    )
      << "fraction should be in the range 0 < fraction < 1"
      << exit(FatalError);
  }
  scalar distance = 0.0;
  const label nParcel = this->size();
  globalIndex globalParcels(nParcel);
  const label nParcelSum = globalParcels.size();
  if (nParcelSum == 0)
  {
    return distance;
  }
  // lists of parcels mass and distance from initial injection point
  List<List<scalar> > procMass(Pstream::nProcs());
  List<List<scalar> > procDist(Pstream::nProcs());
  List<scalar>& mass = procMass[Pstream::myProcNo()];
  List<scalar>& dist = procDist[Pstream::myProcNo()];
  mass.setSize(nParcel);
  dist.setSize(nParcel);
  label i = 0;
  scalar mSum = 0.0;
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    scalar m = p.nParticle()*p.mass();
    scalar d = mag(p.position() - p.position0());
    mSum += m;
    mass[i] = m;
    dist[i] = d;
    i++;
  }
  // calculate total mass across all processors
  reduce(mSum, sumOp<scalar>());
  Pstream::gatherList(procMass);
  Pstream::gatherList(procDist);
  if (Pstream::master())
  {
    // flatten the mass lists
    List<scalar> allMass(nParcelSum, 0.0);
    SortableList<scalar> allDist(nParcelSum, 0.0);
    for (label procI = 0; procI < Pstream::nProcs(); procI++)
    {
      SubList<scalar>
      (
        allMass,
        globalParcels.localSize(procI),
        globalParcels.offset(procI)
      ).assign(procMass[procI]);
      // flatten the distance list
      SubList<scalar>
      (
        allDist,
        globalParcels.localSize(procI),
        globalParcels.offset(procI)
      ).assign(procDist[procI]);
    }
    // sort allDist distances into ascending order
    // note: allMass masses are left unsorted
    allDist.sort();
    if (nParcelSum > 1)
    {
      const scalar mLimit = fraction*mSum;
      const labelList& indices = allDist.indices();
      if (mLimit > (mSum - allMass[indices.last()]))
      {
        distance = allDist.last();
      }
      else
      {
        // assuming that 'fraction' is generally closer to 1 than 0,
        // loop through in reverse distance order
        const scalar mThreshold = (1.0 - fraction)*mSum;
        scalar mCurrent = 0.0;
        label i0 = 0;
        forAllReverse(indices, i)
        {
          label indI = indices[i];
          mCurrent += allMass[indI];
          if (mCurrent > mThreshold)
          {
            i0 = i;
            break;
          }
        }
        if (i0 == indices.size() - 1)
        {
          distance = allDist.last();
        }
        else
        {
          // linearly interpolate to determine distance
          scalar alpha = (mCurrent - mThreshold)/allMass[indices[i0]];
          distance =
            allDist[i0] + alpha*(allDist[i0+1] - allDist[i0]);
        }
      }
    }
    else
    {
      distance = allDist.first();
    }
  }
  Pstream::scatter(distance);
  return distance;
}
template<class CloudType>
inline mousse::cachedRandom& mousse::KinematicCloud<CloudType>::rndGen()
{
  return rndGen_;
}
template<class CloudType>
inline mousse::List<mousse::DynamicList<typename CloudType::particleType*> >&
mousse::KinematicCloud<CloudType>::cellOccupancy()
{
  if (cellOccupancyPtr_.empty())
  {
    buildCellOccupancy();
  }
  return cellOccupancyPtr_();
}
template<class CloudType>
inline const mousse::scalarField&
mousse::KinematicCloud<CloudType>::cellLengthScale() const
{
  return cellLengthScale_;
}
template<class CloudType>
inline mousse::DimensionedField<mousse::vector, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UTrans()
{
  return UTrans_();
}
template<class CloudType>
inline const mousse::DimensionedField<mousse::vector, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UTrans() const
{
  return UTrans_();
}
template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UCoeff()
{
  return UCoeff_();
}
template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::KinematicCloud<CloudType>::UCoeff() const
{
  return UCoeff_();
}
template<class CloudType>
inline mousse::tmp<mousse::fvVectorMatrix>
mousse::KinematicCloud<CloudType>::SU(volVectorField& U) const
{
  if (debug)
  {
    Info<< "UTrans min/max = " << min(UTrans()).value() << ", "
      << max(UTrans()).value() << nl
      << "UCoeff min/max = " << min(UCoeff()).value() << ", "
      << max(UCoeff()).value() << endl;
  }
  if (solution_.coupled())
  {
    if (solution_.semiImplicit("U"))
    {
      const DimensionedField<scalar, volMesh>
        Vdt(mesh_.V()*this->db().time().deltaT());
      return UTrans()/Vdt - fvm::Sp(UCoeff()/Vdt, U) + UCoeff()/Vdt*U;
    }
    else
    {
      tmp<fvVectorMatrix> tfvm(new fvVectorMatrix(U, dimForce));
      fvVectorMatrix& fvm = tfvm();
      fvm.source() = -UTrans()/(this->db().time().deltaT());
      return tfvm;
    }
  }
  return tmp<fvVectorMatrix>(new fvVectorMatrix(U, dimForce));
}
template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::vDotSweep() const
{
  tmp<volScalarField> tvDotSweep
  (
    new volScalarField
    (
      IOobject
      (
        this->name() + ":vDotSweep",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      mesh_,
      dimensionedScalar("zero", dimless/dimTime, 0.0),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  volScalarField& vDotSweep = tvDotSweep();
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    const label cellI = p.cell();
    vDotSweep[cellI] += p.nParticle()*p.areaP()*mag(p.U() - U_[cellI]);
  }
  vDotSweep.internalField() /= mesh_.V();
  vDotSweep.correctBoundaryConditions();
  return tvDotSweep;
}
template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::theta() const
{
  tmp<volScalarField> ttheta
  (
    new volScalarField
    (
      IOobject
      (
        this->name() + ":theta",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      mesh_,
      dimensionedScalar("zero", dimless, 0.0),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  volScalarField& theta = ttheta();
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    const label cellI = p.cell();
    theta[cellI] += p.nParticle()*p.volume();
  }
  theta.internalField() /= mesh_.V();
  theta.correctBoundaryConditions();
  return ttheta;
}
template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::alpha() const
{
  tmp<volScalarField> talpha
  (
    new volScalarField
    (
      IOobject
      (
        this->name() + ":alpha",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      mesh_,
      dimensionedScalar("zero", dimless, 0.0)
    )
  );
  scalarField& alpha = talpha().internalField();
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    const label cellI = p.cell();
    alpha[cellI] += p.nParticle()*p.mass();
  }
  alpha /= (mesh_.V()*rho_);
  return talpha;
}
template<class CloudType>
inline const mousse::tmp<mousse::volScalarField>
mousse::KinematicCloud<CloudType>::rhoEff() const
{
  tmp<volScalarField> trhoEff
  (
    new volScalarField
    (
      IOobject
      (
        this->name() + ":rhoEff",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      mesh_,
      dimensionedScalar("zero", dimDensity, 0.0)
    )
  );
  scalarField& rhoEff = trhoEff().internalField();
  forAllConstIter(typename KinematicCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    const label cellI = p.cell();
    rhoEff[cellI] += p.nParticle()*p.mass();
  }
  rhoEff /= mesh_.V();
  return trhoEff;
}
