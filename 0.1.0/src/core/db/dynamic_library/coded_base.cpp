// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coded_base.hpp"
#include "sha1_digest.hpp"
#include "dynamic_code.hpp"
#include "dynamic_code_context.hpp"
#include "dl_library_table.hpp"
#include "pstream_reduce_ops.hpp"
#include "os_specific.hpp"
#include "reg_ioobject.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(codedBase, 0);

}


// Static Member Functions
void* mousse::codedBase::loadLibrary
(
  const fileName& libPath,
  const string& globalFuncName,
  const dictionary& contextDict
) const
{
  void* lib = 0;
  // avoid compilation by loading an existing library
  if (!libPath.empty()) {
    if (libs().open(libPath, false)) {
      lib = libs().findLibrary(libPath);
      // verify the loaded version and unload if needed
      if (lib) {
        // provision for manual execution of code after loading
        if (dlSymFound(lib, globalFuncName)) {
          loaderFunctionType function =
            reinterpret_cast<loaderFunctionType>
            (
              dlSym(lib, globalFuncName)
            );
          if (function) {
            (*function)(true);    // force load
          } else {
            FATAL_IO_ERROR_IN
            (
              "codedBase::updateLibrary()",
              contextDict
            )
            << "Failed looking up symbol " << globalFuncName
            << nl << "from " << libPath << exit(FatalIOError);
          }
        } else {
          FATAL_IO_ERROR_IN
          (
            "codedBase::loadLibrary()",
            contextDict
          )
          << "Failed looking up symbol " << globalFuncName << nl
          << "from " << libPath << exit(FatalIOError);
          lib = 0;
          if (!libs().close(libPath, false)) {
            FATAL_IO_ERROR_IN
            (
              "codedBase::loadLibrary()",
              contextDict
            )
            << "Failed unloading library "
            << libPath
            << exit(FatalIOError);
          }
        }
      }
    }
  }
  return lib;
}


void mousse::codedBase::unloadLibrary
(
  const fileName& libPath,
  const string& globalFuncName,
  const dictionary& contextDict
) const
{
  void* lib = 0;
  if (libPath.empty()) {
    return;
  }
  lib = libs().findLibrary(libPath);
  if (!lib) {
    return;
  }
  // provision for manual execution of code before unloading
  if (dlSymFound(lib, globalFuncName)) {
    loaderFunctionType function =
      reinterpret_cast<loaderFunctionType>
      (
        dlSym(lib, globalFuncName)
      );
    if (function) {
      (*function)(false);    // force unload
    } else {
      FATAL_IO_ERROR_IN
      (
        "codedBase::unloadLibrary()",
        contextDict
      )
      << "Failed looking up symbol " << globalFuncName << nl
      << "from " << libPath << exit(FatalIOError);
    }
  }
  if (!libs().close(libPath, false)) {
    FATAL_IO_ERROR_IN
    (
      "codedBase::updateLibrary()",
      contextDict
    )
    << "Failed unloading library " << libPath
    << exit(FatalIOError);
  }
}


// Private Member Functions 
void mousse::codedBase::createLibrary
(
  dynamicCode& dynCode,
  const dynamicCodeContext& context
) const
{
  bool create =
    Pstream::master() || (regIOobject::fileModificationSkew <= 0);  // not NFS
  if (create) {
    // Write files for new library
    if (!dynCode.upToDate(context)) {
      // filter with this context
      dynCode.reset(context);
      this->prepare(dynCode, context);
      if (!dynCode.copyOrCreateFiles(true)) {
        FATAL_IO_ERROR_IN
        (
          "codedBase::createLibrary(..)",
          context.dict()
        )
        << "Failed writing files for" << nl
        << dynCode.libRelPath() << nl
        << exit(FatalIOError);
      }
    }
    if (!dynCode.wmakeLibso()) {
      FATAL_IO_ERROR_IN
      (
        "codedBase::createLibrary(..)",
        context.dict()
      )
      << "Failed wmake " << dynCode.libRelPath() << nl
      << exit(FatalIOError);
    }
  }
  // all processes must wait for compile to finish
  if (regIOobject::fileModificationSkew > 0) {
    //- Since the library has only been compiled on the master the
    //  other nodes need to pick this library up through NFS
    //  We do this by just polling a few times using the
    //  fileModificationSkew.
    const fileName libPath = dynCode.libPath();
    off_t mySize = mousse::fileSize(libPath);
    off_t masterSize = mySize;
    Pstream::scatter(masterSize);
    if (debug) {
      Pout << endl<< "on processor " << Pstream::myProcNo()
        << " have masterSize:" << masterSize
        << " and localSize:" << mySize
        << endl;
    }
    if (mySize < masterSize) {
      if (debug) {
        Pout << "Local file " << libPath
          << " not of same size (" << mySize
          << ") as master ("
          << masterSize << "). Waiting for "
          << regIOobject::fileModificationSkew
          << " seconds." << endl;
      }
      mousse::sleep(regIOobject::fileModificationSkew);
      // Recheck local size
      mySize = mousse::fileSize(libPath);
      if (mySize < masterSize) {
        FATAL_IO_ERROR_IN
        (
          "functionEntries::codeStream::execute(..)",
          context.dict()
        )
        << "Cannot read (NFS mounted) library " << nl
        << libPath << nl
        << "on processor " << Pstream::myProcNo()
        << " detected size " << mySize
        << " whereas master size is " << masterSize
        << " bytes." << nl
        << "If your case is not NFS mounted"
        << " (so distributed) set fileModificationSkew"
        << " to 0"
        << exit(FatalIOError);
      }
    }
    if (debug) {
      Pout << endl<< "on processor " << Pstream::myProcNo()
        << " after waiting: have masterSize:" << masterSize
        << " and localSize:" << mySize
        << endl;
    }
  }
  reduce(create, orOp<bool>());
}


void mousse::codedBase::updateLibrary
(
  const word& redirectType
) const
{
  const dictionary& dict = this->codeDict();
  dynamicCode::checkSecurity
  (
    "codedBase::updateLibrary()",
    dict
  );
  dynamicCodeContext context{dict};
  // codeName: redirectType + _<sha1>
  // codeDir : redirectType
  dynamicCode dynCode
  {
    redirectType + context.sha1().str(true),
    redirectType
  };
  const fileName libPath = dynCode.libPath();
  // the correct library was already loaded => we are done
  if (libs().findLibrary(libPath)) {
    return;
  }
  Info << "Using dynamicCode for " << this->description().c_str()
    << " at line " << dict.startLineNumber()
    << " in " << dict.name() << endl;
  // remove instantiation of fvPatchField provided by library
  this->clearRedirect();
  // may need to unload old library
  unloadLibrary
  (
    oldLibPath_,
    dynamicCode::libraryBaseName(oldLibPath_),
    context.dict()
  );
  // try loading an existing library (avoid compilation when possible)
  if (!loadLibrary(libPath, dynCode.codeName(), context.dict()))
  {
    createLibrary(dynCode, context);
    loadLibrary(libPath, dynCode.codeName(), context.dict());
  }
  // retain for future reference
  oldLibPath_ = libPath;
}


// Constructors 
mousse::codedBase::codedBase()
{}


// Destructor 
mousse::codedBase::~codedBase()
{}

