// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_composition_model.hpp"


// Constructors 
template<class CloudType>
mousse::CompositionModel<CloudType>::CompositionModel(CloudType& owner)
:
  CloudSubModelBase<CloudType>{owner},
  thermo_{owner.thermo()},
  phaseProps_{}
{}


template<class CloudType>
mousse::CompositionModel<CloudType>::CompositionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>{owner, dict, typeName, type},
  thermo_{owner.thermo()},
  phaseProps_
  {
    this->coeffDict().lookup("phases"),
    thermo_.carrier().species(),
    thermo_.liquids().components(),
    thermo_.solids().components()
  }
{}


template<class CloudType>
mousse::CompositionModel<CloudType>::CompositionModel
(
  const CompositionModel<CloudType>& cm
)
:
  CloudSubModelBase<CloudType>{cm},
  thermo_{cm.thermo_},
  phaseProps_{cm.phaseProps_}
{}


// Destructor 
template<class CloudType>
mousse::CompositionModel<CloudType>::~CompositionModel()
{}


// Member Functions 
template<class CloudType>
const mousse::SLGThermo& mousse::CompositionModel<CloudType>::thermo() const
{
  return thermo_;
}


template<class CloudType>
const mousse::basicSpecieMixture&
mousse::CompositionModel<CloudType>::carrier() const
{
  return thermo_.carrier();
}


template<class CloudType>
const mousse::liquidMixtureProperties&
mousse::CompositionModel<CloudType>::liquids() const
{
  return thermo_.liquids();
}


template<class CloudType>
const mousse::solidMixtureProperties&
mousse::CompositionModel<CloudType>::solids() const
{
  return thermo_.solids();
}


template<class CloudType>
const mousse::phasePropertiesList&
mousse::CompositionModel<CloudType>::phaseProps() const
{
  return phaseProps_;
}


template<class CloudType>
mousse::label mousse::CompositionModel<CloudType>::nPhase() const
{
  return phaseProps_.size();
}


template<class CloudType>
const mousse::wordList& mousse::CompositionModel<CloudType>::phaseTypes() const
{
  // if only 1 phase, return the constituent component names
  if (phaseProps_.size() == 1) {
    return phaseProps_[0].names();
  } else {
    return phaseProps_.phaseTypes();
  }
}


template<class CloudType>
const mousse::wordList& mousse::CompositionModel<CloudType>::stateLabels() const
{
  return phaseProps_.stateLabels();
}


template<class CloudType>
const mousse::wordList&
mousse::CompositionModel<CloudType>::componentNames(const label phasei) const
{
  return phaseProps_[phasei].names();
}


template<class CloudType>
mousse::label mousse::CompositionModel<CloudType>::carrierId
(
  const word& cmptName,
  const bool allowNotFound
) const
{
  label id = thermo_.carrierId(cmptName);
  if (id < 0 && !allowNotFound) {
    FATAL_ERROR_IN
    (
      "label CompositionModel<CloudType>::carrierId"
      "("
      "  const word&, "
      "  const bool"
      ") const"
    )
    << "Unable to determine global id for requested component "
    << cmptName << ". Available components are " << nl
    << thermo_.carrier().species()
    << abort(FatalError);
  }
  return id;
}


template<class CloudType>
mousse::label mousse::CompositionModel<CloudType>::localId
(
  const label phasei,
  const word& cmptName,
  const bool allowNotFound
) const
{
  label id = phaseProps_[phasei].id(cmptName);
  if (id < 0 && !allowNotFound) {
    FATAL_ERROR_IN
    (
      "label CompositionModel<CloudType>::localId"
      "("
      "  const label, "
      "  const word&, "
      "  const bool"
      ") const"
    )
    << "Unable to determine local id for component " << cmptName
    << abort(FatalError);
  }
  return id;
}


template<class CloudType>
mousse::label mousse::CompositionModel<CloudType>::localToCarrierId
(
  const label phasei,
  const label id,
  const bool allowNotFound
) const
{
  label cid = phaseProps_[phasei].carrierIds()[id];
  if (cid < 0 && !allowNotFound) {
    FATAL_ERROR_IN
    (
      "label "
      "CompositionModel<CloudType>::localToCarrierId"
      "("
      "  const label, "
      "  const label, "
      "  const bool"
      ") const"
    )
    << "Unable to determine global carrier id for phase "
    << phasei << " with local id " << id
    << abort(FatalError);
  }
  return cid;
}


template<class CloudType>
const mousse::scalarField& mousse::CompositionModel<CloudType>::Y0
(
  const label phasei
) const
{
  return phaseProps_[phasei].Y();
}


template<class CloudType>
mousse::scalarField mousse::CompositionModel<CloudType>::X
(
  const label phasei,
  const scalarField& Y
) const
{
  const phaseProperties& props = phaseProps_[phasei];
  scalarField X(Y.size());
  scalar WInv = 0.0;
  switch (props.phase()) {
    case phaseProperties::GAS:
    {
      FOR_ALL(Y, i) {
        label cid = props.carrierIds()[i];
        X[i] = Y[i]/thermo_.carrier().W(cid);
        WInv += X[i];
      }
      break;
    }
    case phaseProperties::LIQUID:
    {
      FOR_ALL(Y, i) {
        X[i] = Y[i]/thermo_.liquids().properties()[i].W();
        WInv += X[i];
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "scalarField CompositionModel<CloudType>::X"
        "("
        "  const label, "
        "  const scalarField&"
        ") const"
      )
      << "Only possible to convert gas and liquid mass fractions"
      << abort(FatalError);
    }
  }
  X /= WInv;
  return X;
}


template<class CloudType>
mousse::scalar mousse::CompositionModel<CloudType>::H
(
  const label phasei,
  const scalarField& Y,
  const scalar p,
  const scalar T
) const
{
  const phaseProperties& props = phaseProps_[phasei];
  scalar HMixture = 0.0;
  switch (props.phase()) {
    case phaseProperties::GAS:
    {
      FOR_ALL(Y, i) {
        label cid = props.carrierIds()[i];
        HMixture += Y[i]*thermo_.carrier().Ha(cid, p, T);
      }
      break;
    }
    case phaseProperties::LIQUID:
    {
      FOR_ALL(Y, i) {
        HMixture += Y[i]*thermo_.liquids().properties()[i].h(p, T);
      }
      break;
    }
    case phaseProperties::SOLID:
    {
      FOR_ALL(Y, i) {
        HMixture +=
          Y[i]
          *(thermo_.solids().properties()[i].Hf()
            + thermo_.solids().properties()[i].Cp()*T);
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "scalar CompositionModel<CloudType>::H"
        "("
        "    const label, "
        "    const scalarField&, "
        "    const scalar, "
        "    const scalar"
        ") const"
      )
      << "Unknown phase enumeration" << abort(FatalError);
    }
  }
  return HMixture;
}


template<class CloudType>
mousse::scalar mousse::CompositionModel<CloudType>::Hs
(
  const label phasei,
  const scalarField& Y,
  const scalar p,
  const scalar T
) const
{
  const phaseProperties& props = phaseProps_[phasei];
  scalar HsMixture = 0.0;
  switch (props.phase()) {
    case phaseProperties::GAS:
    {
      FOR_ALL(Y, i) {
        label cid = props.carrierIds()[i];
        HsMixture += Y[i]*thermo_.carrier().Hs(cid, p, T);
      }
      break;
    }
    case phaseProperties::LIQUID:
    {
      FOR_ALL(Y, i) {
        HsMixture +=
          Y[i]
          *(thermo_.liquids().properties()[i].h(p, T)
            - thermo_.liquids().properties()[i].h(p, 298.15));
      }
      break;
    }
    case phaseProperties::SOLID:
    {
      FOR_ALL(Y, i) {
        HsMixture += Y[i]*thermo_.solids().properties()[i].Cp()*T;
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "scalar CompositionModel<CloudType>::Hs"
        "("
        "  const label, "
        "  const scalarField&, "
        "  const scalar, "
        "  const scalar"
        ") const"
      )
      << "Unknown phase enumeration"
      << abort(FatalError);
    }
  }
  return HsMixture;
}


template<class CloudType>
mousse::scalar mousse::CompositionModel<CloudType>::Hc
(
  const label phasei,
  const scalarField& Y,
  const scalar p,
  const scalar /*T*/
) const
{
  const phaseProperties& props = phaseProps_[phasei];
  scalar HcMixture = 0.0;
  switch (props.phase()) {
    case phaseProperties::GAS:
    {
      FOR_ALL(Y, i) {
        label cid = props.carrierIds()[i];
        HcMixture += Y[i]*thermo_.carrier().Hc(cid);
      }
      break;
    }
    case phaseProperties::LIQUID:
    {
      FOR_ALL(Y, i) {
        HcMixture +=
          Y[i]*thermo_.liquids().properties()[i].h(p, 298.15);
      }
      break;
    }
    case phaseProperties::SOLID:
    {
      FOR_ALL(Y, i) {
        HcMixture += Y[i]*thermo_.solids().properties()[i].Hf();
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "scalar CompositionModel<CloudType>::Hc"
        "("
        "  const label, "
        "  const scalarField&, "
        "  const scalar, "
        "  const scalar"
        ") const"
      )
      << "Unknown phase enumeration"
      << abort(FatalError);
    }
  }
  return HcMixture;
}


template<class CloudType>
mousse::scalar mousse::CompositionModel<CloudType>::Cp
(
  const label phasei,
  const scalarField& Y,
  const scalar p,
  const scalar T
) const
{
  const phaseProperties& props = phaseProps_[phasei];
  scalar CpMixture = 0.0;
  switch (props.phase()) {
    case phaseProperties::GAS:
    {
      FOR_ALL(Y, i) {
        label cid = props.carrierIds()[i];
        CpMixture += Y[i]*thermo_.carrier().Cp(cid, p, T);
      }
      break;
    }
    case phaseProperties::LIQUID:
    {
      FOR_ALL(Y, i) {
        CpMixture += Y[i]*thermo_.liquids().properties()[i].Cp(p, T);
      }
      break;
    }
    case phaseProperties::SOLID:
    {
      FOR_ALL(Y, i) {
        CpMixture += Y[i]*thermo_.solids().properties()[i].Cp();
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "scalar CompositionModel<CloudType>::Cp"
        "("
        "  const label, "
        "  const scalarField&, "
        "  const scalar, "
        "  const scalar"
        ") const"
      )
      << "Unknown phase enumeration"
      << abort(FatalError);
    }
  }
  return CpMixture;
}


template<class CloudType>
mousse::scalar mousse::CompositionModel<CloudType>::L
(
  const label phasei,
  const scalarField& Y,
  const scalar p,
  const scalar T
) const
{
  const phaseProperties& props = phaseProps_[phasei];
  scalar LMixture = 0.0;
  switch (props.phase()) {
    case phaseProperties::GAS:
    {
      if (debug) {
        WARNING_IN
        (
          "scalar CompositionModel<CloudType>::L"
          "("
          "  const label, "
          "  const scalarField&, "
          "  const scalar, "
          "  const scalar"
          ") const\n"
        )
        << "No support for gaseous components" << endl;
      }
      break;
    }
    case phaseProperties::LIQUID:
    {
      FOR_ALL(Y, i) {
        LMixture += Y[i]*thermo_.liquids().properties()[i].hl(p, T);
      }
      break;
    }
    case phaseProperties::SOLID:
    {
      if (debug) {
        WARNING_IN
        (
          "scalar CompositionModel<CloudType>::L"
          "("
            "const label, "
            "const scalarField&, "
            "const scalar, "
            "const scalar"
          ") const\n"
        )
        << "No support for solid components" << endl;
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "scalar CompositionModel<CloudType>::L"
        "("
        "  const label, "
        "  const scalarField&, "
        "  const scalar, "
        "  const scalar"
        ") const"
      )
      << "Unknown phase enumeration"
      << abort(FatalError);
    }
  }
  return LMixture;
}

#include "_composition_model_new.ipp"
