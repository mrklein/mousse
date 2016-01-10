// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ioposition.hpp"
// Constructors 
template<class CloudType>
mousse::IOPosition<CloudType>::IOPosition(const CloudType& c)
:
  regIOobject
  {
    IOobject
    {
      "positions",
      c.time().timeName(),
      c,
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  },
  cloud_(c)
{}
// Member Functions 
template<class CloudType>
bool mousse::IOPosition<CloudType>::write() const
{
  if (cloud_.size())
  {
    return regIOobject::write();
  }
  else
  {
    return true;
  }
}
template<class CloudType>
bool mousse::IOPosition<CloudType>::writeData(Ostream& os) const
{
  os  << cloud_.size() << nl << token::BEGIN_LIST << nl;
  FOR_ALL_CONST_ITER(typename CloudType, cloud_, iter)
  {
    iter().writePosition(os);
    os  << nl;
  }
  os  << token::END_LIST << endl;
  return os.good();
}
template<class CloudType>
void mousse::IOPosition<CloudType>::readData(CloudType& c, bool checkClass)
{
  const polyMesh& mesh = c.pMesh();
  Istream& is = readStream(checkClass ? typeName : "");
  token firstToken(is);
  if (firstToken.isLabel())
  {
    label s = firstToken.labelToken();
    // Read beginning of contents
    is.readBeginList("IOPosition<CloudType>::readData(CloudType, bool)");
    for (label i=0; i<s; i++)
    {
      // Read position only
      c.append(new typename CloudType::particleType(mesh, is, false));
    }
    // Read end of contents
    is.readEndList("IOPosition<CloudType>::readData(CloudType, bool)");
  }
  else if (firstToken.isPunctuation())
  {
    if (firstToken.pToken() != token::BEGIN_LIST)
    {
      FATAL_IO_ERROR_IN
      (
        "void IOPosition<CloudType>::readData(CloudType&, bool)",
        is
      )
      << "incorrect first token, '(', found "
      << firstToken.info() << exit(FatalIOError);
    }
    token lastToken(is);
    while(!(lastToken.isPunctuation()
            && lastToken.pToken() == token::END_LIST))
    {
      is.putBack(lastToken);
      // Write position only
      c.append(new typename CloudType::particleType(mesh, is, false));
      is  >> lastToken;
    }
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "void IOPosition<ParticleType>::readData(CloudType&, bool)",
      is
    )
    << "incorrect first token, expected <int> or '(', found "
    << firstToken.info() << exit(FatalIOError);
  }
  // Check state of IOstream
  is.check
  (
    "void IOPosition<CloudType>::readData(CloudType&, bool)"
  );
}
