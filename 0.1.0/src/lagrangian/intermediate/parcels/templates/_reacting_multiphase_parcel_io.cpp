// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_reacting_multiphase_parcel.hpp"
#include "iostreams.hpp"
// Static Data Members
template<class ParcelType>
mousse::string mousse::ReactingMultiphaseParcel<ParcelType>::propertyList_ =
  mousse::ReactingMultiphaseParcel<ParcelType>::propertyList();
template<class ParcelType>
const std::size_t mousse::ReactingMultiphaseParcel<ParcelType>::sizeofFields_
(
  0
);
// Constructors 
template<class ParcelType>
mousse::ReactingMultiphaseParcel<ParcelType>::ReactingMultiphaseParcel
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  ParcelType(mesh, is, readFields),
  YGas_(0),
  YLiquid_(0),
  YSolid_(0),
  canCombust_(0)
{
  if (readFields)
  {
    DynamicList<scalar> Yg;
    DynamicList<scalar> Yl;
    DynamicList<scalar> Ys;
    is >> Yg >> Yl >> Ys;
    YGas_.transfer(Yg);
    YLiquid_.transfer(Yl);
    YSolid_.transfer(Ys);
    // scale the mass fractions
    const scalarField& YMix = this->Y_;
    YGas_ /= YMix[GAS] + ROOTVSMALL;
    YLiquid_ /= YMix[LIQ] + ROOTVSMALL;
    YSolid_ /= YMix[SLD] + ROOTVSMALL;
  }
  // Check state of Istream
  is.check
  (
    "ReactingMultiphaseParcel<ParcelType>::ReactingMultiphaseParcel"
    "("
      "const polyMesh&, "
      "Istream&, "
      "bool"
    ")"
  );
}
template<class ParcelType>
template<class CloudType>
void mousse::ReactingMultiphaseParcel<ParcelType>::readFields(CloudType& c)
{
  if (!c.size())
  {
    return;
  }
  ParcelType::readFields(c);
}
template<class ParcelType>
template<class CloudType, class CompositionType>
void mousse::ReactingMultiphaseParcel<ParcelType>::readFields
(
  CloudType& c,
  const CompositionType& compModel
)
{
  if (!c.size())
  {
    return;
  }
  ParcelType::readFields(c, compModel);
  // Get names and sizes for each Y...
  const label idGas = compModel.idGas();
  const wordList& gasNames = compModel.componentNames(idGas);
  const label idLiquid = compModel.idLiquid();
  const wordList& liquidNames = compModel.componentNames(idLiquid);
  const label idSolid = compModel.idSolid();
  const wordList& solidNames = compModel.componentNames(idSolid);
  const wordList& stateLabels = compModel.stateLabels();
  // Set storage for each Y... for each parcel
  forAllIter(typename Cloud<ReactingMultiphaseParcel<ParcelType> >, c, iter)
  {
    ReactingMultiphaseParcel<ParcelType>& p = iter();
    p.YGas_.setSize(gasNames.size(), 0.0);
    p.YLiquid_.setSize(liquidNames.size(), 0.0);
    p.YSolid_.setSize(solidNames.size(), 0.0);
  }
  // Populate YGas for each parcel
  forAll(gasNames, j)
  {
    IOField<scalar> YGas
    (
      c.fieldIOobject
      (
        "Y" + gasNames[j] + stateLabels[idGas],
        IOobject::MUST_READ
      )
    );
    label i = 0;
    forAllIter
    (
      typename Cloud<ReactingMultiphaseParcel<ParcelType> >,
      c,
      iter
    )
    {
      ReactingMultiphaseParcel<ParcelType>& p = iter();
      p.YGas_[j] = YGas[i++]/(p.Y()[GAS] + ROOTVSMALL);
    }
  }
  // Populate YLiquid for each parcel
  forAll(liquidNames, j)
  {
    IOField<scalar> YLiquid
    (
      c.fieldIOobject
      (
        "Y" + liquidNames[j] + stateLabels[idLiquid],
        IOobject::MUST_READ
      )
    );
    label i = 0;
    forAllIter
    (
      typename Cloud<ReactingMultiphaseParcel<ParcelType> >,
      c,
      iter
    )
    {
      ReactingMultiphaseParcel<ParcelType>& p = iter();
      p.YLiquid_[j] = YLiquid[i++]/(p.Y()[LIQ] + ROOTVSMALL);
    }
  }
  // Populate YSolid for each parcel
  forAll(solidNames, j)
  {
    IOField<scalar> YSolid
    (
      c.fieldIOobject
      (
        "Y" + solidNames[j] + stateLabels[idSolid],
        IOobject::MUST_READ
      )
    );
    label i = 0;
    forAllIter
    (
      typename Cloud<ReactingMultiphaseParcel<ParcelType> >,
      c,
      iter
    )
    {
      ReactingMultiphaseParcel<ParcelType>& p = iter();
      p.YSolid_[j] = YSolid[i++]/(p.Y()[SLD] + ROOTVSMALL);
    }
  }
}
template<class ParcelType>
template<class CloudType>
void mousse::ReactingMultiphaseParcel<ParcelType>::writeFields(const CloudType& c)
{
  ParcelType::writeFields(c);
}
template<class ParcelType>
template<class CloudType, class CompositionType>
void mousse::ReactingMultiphaseParcel<ParcelType>::writeFields
(
  const CloudType& c,
  const CompositionType& compModel
)
{
  ParcelType::writeFields(c, compModel);
  label np = c.size();
  // Write the composition fractions
  if (np > 0)
  {
    const wordList& stateLabels = compModel.stateLabels();
    const label idGas = compModel.idGas();
    const wordList& gasNames = compModel.componentNames(idGas);
    forAll(gasNames, j)
    {
      IOField<scalar> YGas
      (
        c.fieldIOobject
        (
          "Y" + gasNames[j] + stateLabels[idGas],
          IOobject::NO_READ
        ),
        np
      );
      label i = 0;
      forAllConstIter
      (
        typename Cloud<ReactingMultiphaseParcel<ParcelType> >,
        c,
        iter
      )
      {
        const ReactingMultiphaseParcel<ParcelType>& p0 = iter();
        YGas[i++] = p0.YGas()[j]*p0.Y()[GAS];
      }
      YGas.write();
    }
    const label idLiquid = compModel.idLiquid();
    const wordList& liquidNames = compModel.componentNames(idLiquid);
    forAll(liquidNames, j)
    {
      IOField<scalar> YLiquid
      (
        c.fieldIOobject
        (
          "Y" + liquidNames[j] + stateLabels[idLiquid],
          IOobject::NO_READ
        ),
        np
      );
      label i = 0;
      forAllConstIter
      (
        typename Cloud<ReactingMultiphaseParcel<ParcelType> >,
        c,
        iter
      )
      {
        const ReactingMultiphaseParcel<ParcelType>& p0 = iter();
        YLiquid[i++] = p0.YLiquid()[j]*p0.Y()[LIQ];
      }
      YLiquid.write();
    }
    const label idSolid = compModel.idSolid();
    const wordList& solidNames = compModel.componentNames(idSolid);
    forAll(solidNames, j)
    {
      IOField<scalar> YSolid
      (
        c.fieldIOobject
        (
          "Y" + solidNames[j] + stateLabels[idSolid],
          IOobject::NO_READ
        ),
        np
      );
      label i = 0;
      forAllConstIter
      (
        typename Cloud<ReactingMultiphaseParcel<ParcelType> >,
        c,
        iter
      )
      {
        const ReactingMultiphaseParcel<ParcelType>& p0 = iter();
        YSolid[i++] = p0.YSolid()[j]*p0.Y()[SLD];
      }
      YSolid.write();
    }
  }
}
// IOstream Operators 
template<class ParcelType>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const ReactingMultiphaseParcel<ParcelType>& p
)
{
  scalarField YGasLoc(p.YGas()*p.Y()[0]);
  scalarField YLiquidLoc(p.YLiquid()*p.Y()[1]);
  scalarField YSolidLoc(p.YSolid()*p.Y()[2]);
  if (os.format() == IOstream::ASCII)
  {
    os  << static_cast<const ParcelType&>(p)
      << token::SPACE << YGasLoc
      << token::SPACE << YLiquidLoc
      << token::SPACE << YSolidLoc;
  }
  else
  {
    os  << static_cast<const ParcelType&>(p);
    os  << YGasLoc << YLiquidLoc << YSolidLoc;
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<"
    "("
      "Ostream&, "
      "const ReactingMultiphaseParcel<ParcelType>&"
    ")"
  );
  return os;
}
