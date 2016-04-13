#ifndef LAGRANGIAN_BASIC_TIOPOSITION_TIOPOSITION_HPP_
#define LAGRANGIAN_BASIC_TIOPOSITION_TIOPOSITION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::IOPosition
// Description
//   Helper IO class to read and write particle positions

#include "reg_ioobject.hpp"


namespace mousse {

template<class CloudType>
class IOPosition
:
  public regIOobject
{
  // Private data
    //- Reference to the cloud
    const CloudType& cloud_;
public:
  // Static data
    //- Runtime type name information. Use cloud type.
    virtual const word& type() const
    {
      return Cloud<typename CloudType::particleType>::typeName;
    }
  // Constructors
    //- Construct from cloud
    IOPosition(const CloudType&);
  // Member functions
    //- Inherit readData from regIOobject
    using regIOobject::readData;
    virtual void readData(CloudType& c, bool checkClass);
    virtual bool write() const;
    virtual bool writeData(Ostream& os) const;
};

}  // namespace mousse

#include "_ioposition.ipp"

#endif

