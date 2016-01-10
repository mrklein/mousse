// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dynamic_code.hpp"
#include "dynamic_code_context.hpp"
#include "string_ops.hpp"
#include "ifstream.hpp"
#include "ofstream.hpp"
#include "os_specific.hpp"
#include "dictionary.hpp"

// Static Data Members
int mousse::dynamicCode::allowSystemOperations
(
  mousse::debug::infoSwitch("allowSystemOperations", 0)
);

const mousse::word mousse::dynamicCode::codeTemplateEnvName
  = "MOUSSE_CODE_TEMPLATES";

const mousse::fileName mousse::dynamicCode::codeTemplateDirName
  = "code_templates/dynamic_code";

const char* const mousse::dynamicCode::libTargetRoot =
  "LIB = $(PWD)/../platforms/$(WM_OPTIONS)/lib/lib";

const char* const mousse::dynamicCode::topDirName = "dynamic_code";

// Static Member Functions
void mousse::dynamicCode::checkSecurity
(
  const char* title,
  const dictionary& dict
)
{
  if (isAdministrator())
  {
    FATAL_IO_ERROR_IN
    (
      title,
      dict
    )
    << "This code should not be executed by someone with administrator"
    << " rights due to security reasons." << nl
    << "(it writes a shared library which then gets loaded "
    << "using dlopen)"
    << exit(FatalIOError);
  }
  if (!allowSystemOperations)
  {
    FATAL_IO_ERROR_IN
    (
      title,
      dict
    )
    << "Loading a shared library using case-supplied code is not"
    << " enabled by default" << nl
    << "because of security issues. If you trust the code you can"
    << " enable this" << nl
    << "facility be adding to the InfoSwitches setting in the system"
    << " controlDict:" << nl << nl
    << "    allowSystemOperations 1" << nl << nl
    << "The system controlDict is either" << nl << nl
    << "    ~/.mousse/$WM_PROJECT_VERSION/controlDict" << nl << nl
    << "or" << nl << nl
    << "    $WM_PROJECT_DIR/etc/controlDict" << nl
    << endl
    << exit(FatalIOError);
  }
}


mousse::word mousse::dynamicCode::libraryBaseName(const fileName& libPath)
{
  word libName(libPath.name(true));
  libName.erase(0, 3);    // remove leading 'lib' from name
  return libName;
}


// Protected Member Functions 
void mousse::dynamicCode::copyAndFilter
(
  ISstream& is,
  OSstream& os,
  const HashTable<string>& mapping
)
{
  if (!is.good())
  {
    FATAL_ERROR_IN
    (
      "dynamicCode::copyAndFilter()"
      " const"
    )   << "Failed opening for reading " << is.name()
      << exit(FatalError);
  }
  if (!os.good())
  {
    FATAL_ERROR_IN
    (
      "dynamicCode::copyAndFilter()"
      " const"
    )   << "Failed writing " << os.name()
      << exit(FatalError);
  }
  // Copy file while rewriting $VARS and ${VARS}
  string line;
  do
  {
    is.getLine(line);
    // expand according to mapping
    // expanding according to env variables might cause too many
    // surprises
    stringOps::inplaceExpand(line, mapping);
    os.writeQuoted(line, false) << nl;
  }
  while (is.good());
}


bool mousse::dynamicCode::resolveTemplates
(
  const UList<fileName>& templateNames,
  DynamicList<fileName>& resolvedFiles,
  DynamicList<fileName>& badFiles
)
{
  // try to get template from MOUSSE_CODESTREAM_TEMPLATES
  const fileName templateDir(mousse::getEnv(codeTemplateEnvName));
  bool allOkay = true;
  FOR_ALL(templateNames, fileI)
  {
    const fileName& templateName = templateNames[fileI];
    fileName file;
    if (!templateDir.empty() && isDir(templateDir))
    {
      file = templateDir/templateName;
      if (!isFile(file, false))
      {
        file.clear();
      }
    }
    // not found - fallback to ~mousse expansion
    if (file.empty())
    {
      file = findEtcFile(codeTemplateDirName/templateName);
    }
    if (file.empty())
    {
      badFiles.append(templateName);
      allOkay = false;
    }
    else
    {
      resolvedFiles.append(file);
    }
  }
  return allOkay;
}


bool mousse::dynamicCode::writeCommentSHA1(Ostream& os) const
{
  const bool hasSHA1 = filterVars_.found("SHA1sum");
  if (hasSHA1)
  {
    os  << "/* dynamicCode:\n * SHA1 = ";
    os.writeQuoted(filterVars_["SHA1sum"], false) << "\n */\n";
  }
  return hasSHA1;
}


bool mousse::dynamicCode::createMakeFiles() const
{
  // Create _make/files
  if (compileFiles_.empty())
  {
    return false;
  }
  const fileName dstFile(this->codePath()/"_make/files");
  // Create dir
  mkDir(dstFile.path());
  OFstream os(dstFile);
  //Info<< "Writing to " << dstFile << endl;
  if (!os.good())
  {
    FATAL_ERROR_IN
      (
        "dynamicCode::createMakeFiles()"
        " const"
      )   << "Failed writing " << dstFile
        << exit(FatalError);
  }
  writeCommentSHA1(os);
  // Write compile files
  FOR_ALL(compileFiles_, fileI)
  {
    os.writeQuoted(compileFiles_[fileI], false) << nl;
  }
  os  << nl
    << libTargetRoot << codeName_.c_str() << nl;
  return true;
}


bool mousse::dynamicCode::createMakeOptions() const
{
  // Create _make/options
  if (compileFiles_.empty() || makeOptions_.empty())
  {
    return false;
  }
  const fileName dstFile(this->codePath()/"_make/options");
  // Create dir
  mkDir(dstFile.path());
  OFstream os(dstFile);
  //Info<< "Writing to " << dstFile << endl;
  if (!os.good())
  {
    FATAL_ERROR_IN
    (
      "dynamicCode::createMakeOptions()"
      " const"
    )
    << "Failed writing " << dstFile
    << exit(FatalError);
  }
  writeCommentSHA1(os);
  os.writeQuoted(makeOptions_, false) << nl;
  return true;
}


bool mousse::dynamicCode::writeDigest(const SHA1Digest& sha1) const
{
  const fileName file = digestFile();
  mkDir(file.path());
  OFstream os(file);
  sha1.write(os, true) << nl;
  return os.good();
}


bool mousse::dynamicCode::writeDigest(const std::string& sha1) const
{
  const fileName file = digestFile();
  mkDir(file.path());
  OFstream os(file);
  os  << '_';
  os.writeQuoted(sha1, false) << nl;
  return os.good();
}


// Constructors 
mousse::dynamicCode::dynamicCode(const word& codeName, const word& codeDirName)
:
  codeRoot_(stringOps::expand("$MOUSSE_CASE")/topDirName),
  libSubDir_(stringOps::expand("platforms/$WM_OPTIONS/lib")),
  codeName_(codeName),
  codeDirName_(codeDirName)
{
  if (codeDirName_.empty())
  {
    codeDirName_ = codeName_;
  }
  clear();
}


// Member Functions 
mousse::fileName mousse::dynamicCode::codeRelPath() const
{
  return topDirName/codeDirName_;
}


mousse::fileName mousse::dynamicCode::libRelPath() const
{
#if defined(darwin64)
  return codeRelPath()/libSubDir_/"lib" + codeName_ + ".dylib";
#else
  return codeRelPath()/libSubDir_/"lib" + codeName_ + ".so";
#endif
}


void mousse::dynamicCode::clear()
{
  compileFiles_.clear();
  copyFiles_.clear();
  createFiles_.clear();
  filterVars_.clear();
  filterVars_.set("typeName", codeName_);
  filterVars_.set("SHA1sum", SHA1Digest().str());
  // provide default _make/options
  makeOptions_ =
    "EXE_INC = -g\n"
    "\n\nLIB_LIBS = ";
}


void mousse::dynamicCode::reset
(
  const dynamicCodeContext& context
)
{
  clear();
  setFilterContext(context);
}


void mousse::dynamicCode::addCompileFile(const fileName& name)
{
  compileFiles_.append(name);
}


void mousse::dynamicCode::addCopyFile(const fileName& name)
{
  copyFiles_.append(name);
}


void mousse::dynamicCode::addCreateFile
(
  const fileName& name,
  const string& contents
)
{
  createFiles_.append(fileAndContent(name, contents));
}


void mousse::dynamicCode::setFilterContext
(
  const dynamicCodeContext& context
)
{
  filterVars_.set("localCode", context.localCode());
  filterVars_.set("code", context.code());
  filterVars_.set("codeInclude", context.include());
  filterVars_.set("SHA1sum", context.sha1().str());
}


void mousse::dynamicCode::setFilterVariable
(
  const word& key,
  const std::string& value
)
{
  filterVars_.set(key, value);
}


void mousse::dynamicCode::setMakeOptions(const std::string& content)
{
  makeOptions_ = content;
}


bool mousse::dynamicCode::copyOrCreateFiles(const bool verbose) const
{
  if (verbose)
  {
    Info<< "Creating new library in " << this->libRelPath() << endl;
  }
  const label nFiles = compileFiles_.size() + copyFiles_.size();
  DynamicList<fileName> resolvedFiles(nFiles);
  DynamicList<fileName> badFiles(nFiles);
  // resolve template, or add to bad-files
  resolveTemplates(compileFiles_, resolvedFiles, badFiles);
  resolveTemplates(copyFiles_, resolvedFiles, badFiles);
  if (!badFiles.empty())
  {
    FATAL_ERROR_IN
    (
      "dynamicCode::copyFilesContents(..)"
    )   << "Could not find the code template(s): "
      << badFiles << nl
      << "Under the $" << codeTemplateEnvName
      << " directory or via via the ~mousse/"
      << codeTemplateDirName << " expansion"
      << exit(FatalError);
  }
  // Create dir
  const fileName outputDir = this->codePath();
  // Create dir
  mkDir(outputDir);
  // Copy/filter files
  FOR_ALL(resolvedFiles, fileI)
  {
    const fileName& srcFile = resolvedFiles[fileI];
    const fileName  dstFile(outputDir/srcFile.name());
    IFstream is(srcFile);
    //Info<< "Reading from " << is.name() << endl;
    if (!is.good())
    {
      FATAL_ERROR_IN
      (
        "dynamicCode::copyFilesContents(const fileName&)"
        " const"
      )   << "Failed opening " << srcFile
        << exit(FatalError);
    }
    OFstream os(dstFile);
    //Info<< "Writing to " << dstFile.name() << endl;
    if (!os.good())
    {
      FATAL_ERROR_IN
      (
        "dynamicCode::copyFilesContents(const fileName&)"
        " const"
      )   << "Failed writing " << dstFile
        << exit(FatalError);
    }
    // Copy lines while expanding variables
    copyAndFilter(is, os, filterVars_);
  }
  // Create files:
  FOR_ALL(createFiles_, fileI)
  {
    const fileName dstFile
    (
      outputDir/stringOps::expand(createFiles_[fileI].first())
    );
    mkDir(dstFile.path());
    OFstream os(dstFile);
    //Info<< "Writing to " << createFiles_[fileI].first() << endl;
    if (!os.good())
    {
      FATAL_ERROR_IN
      (
        "dynamicCode::copyOrCreateFiles()"
        " const"
      )   << "Failed writing " << dstFile
        << exit(FatalError);
    }
    os.writeQuoted(createFiles_[fileI].second(), false) << nl;
  }
  // Create _make/files + _make/options
  createMakeFiles();
  createMakeOptions();
  writeDigest(filterVars_["SHA1sum"]);
  return true;
}


bool mousse::dynamicCode::wmakeLibso() const
{
  const mousse::string wmakeCmd("wmake -s libso " + this->codePath());
  Info<< "Invoking " << wmakeCmd << endl;
  if (mousse::system(wmakeCmd))
  {
    return false;
  }
  else
  {
    return true;
  }
}


bool mousse::dynamicCode::upToDate(const SHA1Digest& sha1) const
{
  const fileName file = digestFile();
  if (!exists(file, false) || SHA1Digest(IFstream(file)()) != sha1)
  {
    return false;
  }
  return true;
}


bool mousse::dynamicCode::upToDate(const dynamicCodeContext& context) const
{
  return upToDate(context.sha1());
}

