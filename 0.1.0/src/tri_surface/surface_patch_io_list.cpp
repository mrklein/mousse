// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_patch_io_list.hpp"
#include "entry.hpp"
#include "ptr_list.hpp"
#include "dictionary.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(surfacePatchIOList, 0);
}
// Constructors
// Construct from IOObject
mousse::surfacePatchIOList::surfacePatchIOList
(
  const IOobject& io
)
:
  surfacePatchList(),
  regIOobject(io)
{
  mousse::string functionName =
    "surfacePatchIOList::surfacePatchIOList"
    "(const IOobject& io)";
  if (readOpt() == IOobject::MUST_READ
      || readOpt() == IOobject::MUST_READ_IF_MODIFIED)
  {
    if (readOpt() == IOobject::MUST_READ_IF_MODIFIED)
    {
      WARNING_IN
      (
        "surfacePatchIOList::surfacePatchIOList\n"
        "(\n"
        "    const IOobject&\n"
        ")"
      )
      << "Specified IOobject::MUST_READ_IF_MODIFIED but class"
      << " does not support automatic rereading."
      << endl;
    }
    surfacePatchList& patches = *this;
    // read polyPatchList
    Istream& is = readStream(typeName);
    PtrList<entry> patchEntries{is};
    patches.setSize(patchEntries.size());
    label faceI = 0;
    FOR_ALL(patches, patchI)
    {
      const dictionary& dict = patchEntries[patchI].dict();
      label patchSize = readLabel(dict.lookup("nFaces"));
      label startFaceI = readLabel(dict.lookup("startFace"));
      patches[patchI] =
        surfacePatch{
          word(dict.lookup("geometricType")),
          patchEntries[patchI].keyword(),
          patchSize,
          startFaceI,
          patchI
        };
      if (startFaceI != faceI)
      {
        FATAL_ERROR_IN(functionName)
          << "Patches are not ordered. Start of patch " << patchI
          << " does not correspond to sum of preceding patches."
          << endl
          << "while reading " << io.objectPath()
          << exit(FatalError);
      }
      faceI += patchSize;
    }
    // Check state of IOstream
    is.check(functionName.c_str());
    close();
  }
}
// Construct from IOObject
mousse::surfacePatchIOList::surfacePatchIOList
(
  const IOobject& io,
  const surfacePatchList& patches
)
:
  surfacePatchList{patches},
  regIOobject{io}
{}

// Destructor
mousse::surfacePatchIOList::~surfacePatchIOList()
{}

// Member Functions
// writeData member function required by regIOobject
bool mousse::surfacePatchIOList::writeData(Ostream& os) const
{
  os << *this;
  return os.good();
}

// Friend Operators
mousse::Ostream& mousse::operator<<(Ostream& os, const surfacePatchIOList& patches)
{
  os << patches.size() << nl << token::BEGIN_LIST;
  FOR_ALL(patches, patchI)
  {
    patches[patchI].writeDict(os);
  }
  os << token::END_LIST;
  return os;
}

