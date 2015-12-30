// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_reacting_parcel.hpp"
#include "iostreams.hpp"
// Static Data Members
template<class ParcelType>
mousse::string mousse::ReactingParcel<ParcelType>::propertyList_ =
  mousse::ReactingParcel<ParcelType>::propertyList();
template<class ParcelType>
const std::size_t mousse::ReactingParcel<ParcelType>::sizeofFields_
(
  sizeof(scalar)
);
// Constructors 
template<class ParcelType>
mousse::ReactingParcel<ParcelType>::ReactingParcel
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  ParcelType(mesh, is, readFields),
  mass0_(0.0),
  Y_(0),
  pc_(0.0)
{
  if (readFields)
  {
    DynamicList<scalar> Ymix;
    if (is.format() == IOstream::ASCII)
    {
      is >> mass0_ >> Ymix;
    }
    else
    {
      is.read(reinterpret_cast<char*>(&mass0_), sizeofFields_);
      is >> Ymix;
    }
    Y_.transfer(Ymix);
  }
  // Check state of Istream
  is.check
  (
    "ReactingParcel<ParcelType>::ReactingParcel"
    "("
      "const polyMesh&, "
      "Istream&, "
      "bool"
    ")"
  );
}
template<class ParcelType>
template<class CloudType>
void mousse::ReactingParcel<ParcelType>::readFields(CloudType& c)
{
  if (!c.size())
  {
    return;
  }
  ParcelType::readFields(c);
}
template<class ParcelType>
template<class CloudType, class CompositionType>
void mousse::ReactingParcel<ParcelType>::readFields
(
  CloudType& c,
  const CompositionType& compModel
)
{
  if (!c.size())
  {
    return;
  }
  ParcelType::readFields(c);
  IOField<scalar> mass0(c.fieldIOobject("mass0", IOobject::MUST_READ));
  c.checkFieldIOobject(c, mass0);
  label i = 0;
  forAllIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
  {
    ReactingParcel<ParcelType>& p = iter();
    p.mass0_ = mass0[i++];
  }
  // Get names and sizes for each Y...
  const wordList& phaseTypes = compModel.phaseTypes();
  const label nPhases = phaseTypes.size();
  wordList stateLabels(nPhases, "");
  if (compModel.nPhase() == 1)
  {
    stateLabels = compModel.stateLabels()[0];
  }
  // Set storage for each Y... for each parcel
  forAllIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
  {
    ReactingParcel<ParcelType>& p = iter();
    p.Y_.setSize(nPhases, 0.0);
  }
  // Populate Y for each parcel
  forAll(phaseTypes, j)
  {
    IOField<scalar> Y
    (
      c.fieldIOobject
      (
        "Y" + phaseTypes[j] + stateLabels[j],
        IOobject::MUST_READ
      )
    );
    label i = 0;
    forAllIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
    {
      ReactingParcel<ParcelType>& p = iter();
      p.Y_[j] = Y[i++];
    }
  }
}
template<class ParcelType>
template<class CloudType>
void mousse::ReactingParcel<ParcelType>::writeFields(const CloudType& c)
{
  ParcelType::writeFields(c);
}
template<class ParcelType>
template<class CloudType, class CompositionType>
void mousse::ReactingParcel<ParcelType>::writeFields
(
  const CloudType& c,
  const CompositionType& compModel
)
{
  ParcelType::writeFields(c);
  const label np = c.size();
  if (np > 0)
  {
    IOField<scalar> mass0(c.fieldIOobject("mass0", IOobject::NO_READ), np);
    label i = 0;
    forAllConstIter(typename Cloud<ReactingParcel<ParcelType> >, c, iter)
    {
      const ReactingParcel<ParcelType>& p = iter();
      mass0[i++] = p.mass0_;
    }
    mass0.write();
    // Write the composition fractions
    const wordList& phaseTypes = compModel.phaseTypes();
    wordList stateLabels(phaseTypes.size(), "");
    if (compModel.nPhase() == 1)
    {
      stateLabels = compModel.stateLabels()[0];
    }
    forAll(phaseTypes, j)
    {
      IOField<scalar> Y
      (
        c.fieldIOobject
        (
          "Y" + phaseTypes[j] + stateLabels[j],
          IOobject::NO_READ
        ),
        np
      );
      label i = 0;
      forAllConstIter
      (
        typename Cloud<ReactingParcel<ParcelType> >,
        c,
        iter
      )
      {
        const ReactingParcel<ParcelType>& p = iter();
        Y[i++] = p.Y()[j];
      }
      Y.write();
    }
  }
}
// IOstream Operators 
template<class ParcelType>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const ReactingParcel<ParcelType>& p
)
{
  if (os.format() == IOstream::ASCII)
  {
    os  << static_cast<const ParcelType&>(p)
      << token::SPACE << p.mass0()
      << token::SPACE << p.Y();
  }
  else
  {
    os  << static_cast<const ParcelType&>(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.mass0_),
      ReactingParcel<ParcelType>::sizeofFields_
    );
    os  << p.Y();
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const ReactingParcel<ParcelType>&)"
  );
  return os;
}
