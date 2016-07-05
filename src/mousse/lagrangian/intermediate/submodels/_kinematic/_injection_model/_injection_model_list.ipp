// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_injection_model.hpp"


// Constructors 
template<class CloudType>
mousse::InjectionModelList<CloudType>::InjectionModelList(CloudType& /*owner*/)
:
  PtrList<InjectionModel<CloudType>>{}
{}


template<class CloudType>
mousse::InjectionModelList<CloudType>::InjectionModelList
(
  const dictionary& dict,
  CloudType& owner
)
:
  PtrList<InjectionModel<CloudType>>{}
{
  wordList modelNames{dict.toc()};
  Info << "Constructing particle injection models" << endl;
  if (modelNames.size() > 0) {
    this->setSize(modelNames.size());
    label i = 0;
    FOR_ALL_CONST_ITER(IDLList<entry>, dict, iter) {
      const word& model = iter().keyword();
      Info << "Creating injector: " << model << endl;
      const dictionary& props = iter().dict();
      this->set
      (
        i++,
        InjectionModel<CloudType>::New
        (
          props,
          model,
          props.lookup("type"),
          owner
        )
      );
    }
  } else {
    this->setSize(1);
    this->set
    (
      0,
      InjectionModel<CloudType>::New
      (
        dict,
        "none",
        "none",
        owner
      )
    );
  }
}


template<class CloudType>
mousse::InjectionModelList<CloudType>::InjectionModelList
(
  const InjectionModelList<CloudType>& iml
)
:
  PtrList<InjectionModel<CloudType>>{iml}
{}


// Destructor 
template<class CloudType>
mousse::InjectionModelList<CloudType>::~InjectionModelList()
{}


// Member Functions 
template<class CloudType>
mousse::scalar mousse::InjectionModelList<CloudType>::timeStart() const
{
  scalar minTime = GREAT;
  FOR_ALL(*this, i) {
    minTime = min(minTime, this->operator[](i).timeStart());
  }
  return minTime;
}


template<class CloudType>
mousse::scalar mousse::InjectionModelList<CloudType>::timeEnd() const
{
  scalar maxTime = -GREAT;
  FOR_ALL(*this, i) {
    maxTime = max(maxTime, this->operator[](i).timeEnd());
  }
  return maxTime;
}


template<class CloudType>
mousse::scalar mousse::InjectionModelList<CloudType>::volumeToInject
(
  const scalar time0,
  const scalar time1
)
{
  scalar vol = 0.0;
  FOR_ALL(*this, i) {
    vol += this->operator[](i).volumeToInject(time0, time1);
  }
  return vol;
}


template<class CloudType>
mousse::scalar mousse::InjectionModelList<CloudType>::averageParcelMass()
{
  scalar mass = 0.0;
  scalar massTotal = 0.0;
  FOR_ALL(*this, i) {
    scalar mt = this->operator[](i).massTotal();
    mass += mt*this->operator[](i).averageParcelMass();
    massTotal += mt;
  }
  return mass/massTotal;
}


template<class CloudType>
void mousse::InjectionModelList<CloudType>::updateMesh()
{
  FOR_ALL(*this, i) {
    this->operator[](i).updateMesh();
  }
}


template<class CloudType>
template<class TrackData>
void mousse::InjectionModelList<CloudType>::inject(TrackData& td)
{
  FOR_ALL(*this, i) {
    this->operator[](i).inject(td);
  }
}


template<class CloudType>
template<class TrackData>
void mousse::InjectionModelList<CloudType>::injectSteadyState
(
  TrackData& td,
  const scalar trackTime
)
{
  FOR_ALL(*this, i) {
    this->operator[](i).injectSteadyState(td, trackTime);
  }
}


template<class CloudType>
void mousse::InjectionModelList<CloudType>::info(Ostream& os)
{
  FOR_ALL(*this, i) {
    this->operator[](i).info(os);
  }
}

