// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cloud.hpp"
#include "time.hpp"
#include "_ioposition.hpp"
// Static Data Members
template<class ParticleType>
mousse::word mousse::Cloud<ParticleType>::cloudPropertiesName("cloudProperties");
// Private Member Functions 
template<class ParticleType>
void mousse::Cloud<ParticleType>::readCloudUniformProperties()
{
  IOobject dictObj
  (
    cloudPropertiesName,
    time().timeName(),
    "uniform"/cloud::prefix/name(),
    db(),
    IOobject::MUST_READ_IF_MODIFIED,
    IOobject::NO_WRITE,
    false
  );
  if (dictObj.headerOk())
  {
    const IOdictionary uniformPropsDict(dictObj);
    const word procName("processor" + mousse::name(Pstream::myProcNo()));
    if (uniformPropsDict.found(procName))
    {
      uniformPropsDict.subDict(procName).lookup("particleCount")
        >> ParticleType::particleCount_;
    }
  }
  else
  {
    ParticleType::particleCount_ = 0;
  }
}
template<class ParticleType>
void mousse::Cloud<ParticleType>::writeCloudUniformProperties() const
{
  IOdictionary uniformPropsDict
  (
    IOobject
    (
      cloudPropertiesName,
      time().timeName(),
      "uniform"/cloud::prefix/name(),
      db(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    )
  );
  labelList np(Pstream::nProcs(), 0);
  np[Pstream::myProcNo()] = ParticleType::particleCount_;
  Pstream::listCombineGather(np, maxEqOp<label>());
  Pstream::listCombineScatter(np);
  forAll(np, i)
  {
    word procName("processor" + mousse::name(i));
    uniformPropsDict.add(procName, dictionary());
    uniformPropsDict.subDict(procName).add("particleCount", np[i]);
  }
  uniformPropsDict.writeObject
  (
    IOstream::ASCII,
    IOstream::currentVersion,
    time().writeCompression()
  );
}
template<class ParticleType>
void mousse::Cloud<ParticleType>::initCloud(const bool checkClass)
{
  readCloudUniformProperties();
  IOPosition<Cloud<ParticleType> > ioP(*this);
  if (ioP.headerOk())
  {
    ioP.readData(*this, checkClass);
    ioP.close();
    if (this->size())
    {
      readFields();
    }
  }
  else
  {
    if (debug)
    {
      Pout<< "Cannot read particle positions file:" << nl
        << "    " << ioP.objectPath() << nl
        << "Assuming the initial cloud contains 0 particles." << endl;
    }
  }
  // Ask for the tetBasePtIs to trigger all processors to build
  // them, otherwise, if some processors have no particles then
  // there is a comms mismatch.
  polyMesh_.tetBasePtIs();
  forAllIter(typename Cloud<ParticleType>, *this, pIter)
  {
    ParticleType& p = pIter();
    p.initCellFacePt();
  }
}
// Constructors 
template<class ParticleType>
mousse::Cloud<ParticleType>::Cloud
(
  const polyMesh& pMesh,
  const bool checkClass
)
:
  cloud(pMesh),
  polyMesh_(pMesh),
  labels_(),
  nTrackingRescues_(),
  cellWallFacesPtr_()
{
  checkPatches();
  initCloud(checkClass);
}
template<class ParticleType>
mousse::Cloud<ParticleType>::Cloud
(
  const polyMesh& pMesh,
  const word& cloudName,
  const bool checkClass
)
:
  cloud(pMesh, cloudName),
  polyMesh_(pMesh),
  labels_(),
  nTrackingRescues_(),
  cellWallFacesPtr_()
{
  checkPatches();
  initCloud(checkClass);
}
// Member Functions 
template<class ParticleType>
mousse::IOobject mousse::Cloud<ParticleType>::fieldIOobject
(
  const word& fieldName,
  const IOobject::readOption r
) const
{
  return IOobject
  (
    fieldName,
    time().timeName(),
    *this,
    r,
    IOobject::NO_WRITE,
    false
  );
}
template<class ParticleType>
template<class DataType>
void mousse::Cloud<ParticleType>::checkFieldIOobject
(
  const Cloud<ParticleType>& c,
  const IOField<DataType>& data
) const
{
  if (data.size() != c.size())
  {
    FatalErrorIn
    (
      "void Cloud<ParticleType>::checkFieldIOobject"
      "(const Cloud<ParticleType>&, const IOField<DataType>&) const"
    )   << "Size of " << data.name()
      << " field " << data.size()
      << " does not match the number of particles " << c.size()
      << abort(FatalError);
  }
}
template<class ParticleType>
template<class DataType>
void mousse::Cloud<ParticleType>::checkFieldFieldIOobject
(
  const Cloud<ParticleType>& c,
  const CompactIOField<Field<DataType>, DataType>& data
) const
{
  if (data.size() != c.size())
  {
    FatalErrorIn
    (
      "void Cloud<ParticleType>::checkFieldFieldIOobject"
      "("
        "const Cloud<ParticleType>&, "
        "const CompactIOField<Field<DataType>, DataType>&"
      ") const"
    )   << "Size of " << data.name()
      << " field " << data.size()
      << " does not match the number of particles " << c.size()
      << abort(FatalError);
  }
}
template<class ParticleType>
void mousse::Cloud<ParticleType>::readFields()
{}
template<class ParticleType>
void mousse::Cloud<ParticleType>::writeFields() const
{
  if (this->size())
  {
    ParticleType::writeFields(*this);
  }
}
template<class ParticleType>
bool mousse::Cloud<ParticleType>::writeObject
(
  IOstream::streamFormat fmt,
  IOstream::versionNumber ver,
  IOstream::compressionType cmp
) const
{
  writeCloudUniformProperties();
  if (this->size())
  {
    writeFields();
    return cloud::writeObject(fmt, ver, cmp);
  }
  else
  {
    return true;
  }
}
// Ostream Operators
template<class ParticleType>
mousse::Ostream& mousse::operator<<(Ostream& os, const Cloud<ParticleType>& pc)
{
  pc.writeData(os);
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const Cloud<ParticleType>&)");
  return os;
}
