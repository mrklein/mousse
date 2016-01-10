// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "function_object_file.hpp"
#include "time.hpp"
#include "poly_mesh.hpp"
#include "ifstream.hpp"
#include "pstream.hpp"

// Static Data Members
const mousse::word mousse::functionObjectFile::outputPrefix = "postProcessing";
mousse::label mousse::functionObjectFile::addChars = 7;

// Protected Member Functions
void mousse::functionObjectFile::initStream(Ostream& os) const
{
  os.setf(ios_base::scientific, ios_base::floatfield);
  os.width(charWidth());
}
mousse::fileName mousse::functionObjectFile::baseFileDir() const
{
  fileName baseDir = obr_.time().path();
  if (Pstream::parRun())
  {
    // Put in undecomposed case (Note: gives problems for
    // distributed data running)
    baseDir = baseDir/".."/outputPrefix;
  }
  else
  {
    baseDir = baseDir/outputPrefix;
  }
  // Append mesh name if not default region
  if (isA<polyMesh>(obr_))
  {
    const polyMesh& mesh = refCast<const polyMesh>(obr_);
    if (mesh.name() != polyMesh::defaultRegion)
    {
      baseDir = baseDir/mesh.name();
    }
  }
  return baseDir;
}
mousse::fileName mousse::functionObjectFile::baseTimeDir() const
{
  return baseFileDir()/prefix_/obr_.time().timeName();
}
void mousse::functionObjectFile::createFiles()
{
  if (Pstream::master())
  {
    const word startTimeName =
      obr_.time().timeName(obr_.time().startTime().value());
    FOR_ALL(names_, i)
    {
      if (!filePtrs_.set(i))
      {
        fileName outputDir(baseFileDir()/prefix_/startTimeName);
        mkDir(outputDir);
        word fName(names_[i]);
        // Check if file already exists
        IFstream is(outputDir/(fName + ".dat"));
        if (is.good())
        {
          fName = fName + "_" + obr_.time().timeName();
        }
        filePtrs_.set(i, new OFstream(outputDir/(fName + ".dat")));
        initStream(filePtrs_[i]);
        writeFileHeader(i);
      }
    }
  }
}

void mousse::functionObjectFile::writeFileHeader(const label)
{}

void mousse::functionObjectFile::write()
{
  createFiles();
}
void mousse::functionObjectFile::resetNames(const wordList& names)
{
  names_.clear();
  names_.append(names);
  if (Pstream::master())
  {
    filePtrs_.clear();
    filePtrs_.setSize(names_.size());
    createFiles();
  }
}
void mousse::functionObjectFile::resetName(const word& name)
{
  names_.clear();
  names_.append(name);
  if (Pstream::master())
  {
    filePtrs_.clear();
    filePtrs_.setSize(1);
    createFiles();
  }
}
mousse::Omanip<int> mousse::functionObjectFile::valueWidth(const label offset) const
{
  return setw(IOstream::defaultPrecision() + addChars + offset);
}
// Constructors
mousse::functionObjectFile::functionObjectFile
(
  const objectRegistry& obr,
  const word& prefix
)
:
  obr_(obr),
  prefix_(prefix),
  names_(),
  filePtrs_()
{}
mousse::functionObjectFile::functionObjectFile
(
  const objectRegistry& obr,
  const word& prefix,
  const word& name
)
:
  obr_(obr),
  prefix_(prefix),
  names_(),
  filePtrs_()
{
  names_.clear();
  names_.append(name);
  if (Pstream::master())
  {
    filePtrs_.clear();
    filePtrs_.setSize(1);
    // Cannot create files - need to access virtual function
  }
}
mousse::functionObjectFile::functionObjectFile
(
  const objectRegistry& obr,
  const word& prefix,
  const wordList& names
)
:
  obr_(obr),
  prefix_(prefix),
  names_(names),
  filePtrs_()
{
  names_.clear();
  names_.append(names);
  if (Pstream::master())
  {
    filePtrs_.clear();
    filePtrs_.setSize(names_.size());
    // Cannot create files - need to access virtual function
  }
}

// Destructor
mousse::functionObjectFile::~functionObjectFile()
{}

// Member Functions
const mousse::wordList& mousse::functionObjectFile::names() const
{
  return names_;
}
mousse::OFstream& mousse::functionObjectFile::file()
{
  if (!Pstream::master())
  {
    FATAL_ERROR_IN("mousse::OFstream& mousse::functionObjectFile::file()")
      << "Request for file() can only be done by the master process"
      << abort(FatalError);
  }
  if (filePtrs_.size() != 1)
  {
    WARNING_IN("mousse::Ostream& mousse::functionObjectFile::file()")
      << "Requested single file, but multiple files are present"
      << endl;
  }
  if (!filePtrs_.set(0))
  {
    FATAL_ERROR_IN("mousse::OFstream& mousse::functionObjectFile::file()")
      << "File pointer at index " << 0 << " not allocated"
      << abort(FatalError);
  }
  return filePtrs_[0];
}
mousse::PtrList<mousse::OFstream>& mousse::functionObjectFile::files()
{
  if (!Pstream::master())
  {
    FATAL_ERROR_IN("mousse::OFstream& mousse::functionObjectFile::files()")
      << "Request for files() can only be done by the master process"
      << abort(FatalError);
  }
  return filePtrs_;
}
mousse::OFstream& mousse::functionObjectFile::file(const label i)
{
  if (!Pstream::master())
  {
    FATAL_ERROR_IN
    (
      "mousse::OFstream& mousse::functionObjectFile::file(const label)"
    )
      << "Request for file(i) can only be done by the master process"
      << abort(FatalError);
  }
  if (!filePtrs_.set(i))
  {
    FATAL_ERROR_IN("mousse::OFstream& mousse::functionObjectFile::file()")
      << "File pointer at index " << i << " not allocated"
      << abort(FatalError);
  }
  return filePtrs_[i];
}
mousse::label mousse::functionObjectFile::charWidth() const
{
  return IOstream::defaultPrecision() + addChars;
}
void mousse::functionObjectFile::writeCommented
(
  Ostream& os,
  const string& str
) const
{
  os  << setw(1) << "#" << setw(1) << ' '
    << setw(charWidth() - 2) << str.c_str();
}
void mousse::functionObjectFile::writeTabbed
(
  Ostream& os,
  const string& str
) const
{
  os  << tab << setw(charWidth()) << str.c_str();
}
void mousse::functionObjectFile::writeHeader
(
  Ostream& os,
  const string& str
) const
{
  os  << setw(1) << "#" << setw(1) << ' '
    << setf(ios_base::left) << setw(charWidth() - 2) << str.c_str() << nl;
}
