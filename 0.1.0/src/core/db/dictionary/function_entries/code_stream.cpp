// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "code_stream.hpp"
#include "add_to_member_function_selection_table.hpp"
#include "istring_stream.hpp"
#include "ostring_stream.hpp"
#include "dynamic_code.hpp"
#include "dynamic_code_context.hpp"
#include "time.hpp"
#include "pstream.hpp"
#include "iostreams.hpp"
#include "primitive_entry.hpp"
#include "pstream_reduce_ops.hpp"


// Static Data Members
namespace mousse {
namespace functionEntries {

DEFINE_TYPE_NAME_AND_DEBUG(codeStream, 0);

ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  codeStream,
  execute,
  dictionaryIstream
);

ADD_TO_MEMBER_FUNCTION_SELECTION_TABLE
(
  functionEntry,
  codeStream,
  execute,
  primitiveEntryIstream
);

}
}


const mousse::word mousse::functionEntries::codeStream::codeTemplateC
  = "code_stream_template.cpp";

// Private Member Functions 
mousse::dlLibraryTable& mousse::functionEntries::codeStream::libs
(
  const dictionary& dict
)
{
  const IOdictionary& d = static_cast<const IOdictionary&>(dict.topDict());
  return const_cast<Time&>(d.time()).libs();
}


mousse::functionEntries::codeStream::streamingFunctionType
mousse::functionEntries::codeStream::getFunction
(
  const dictionary& parentDict,
  const dictionary& codeDict
)
{
  // get code, codeInclude, codeOptions
  dynamicCodeContext context{codeDict};
  // codeName: codeStream + _<sha1>
  // codeDir : _<sha1>
  std::string sha1Str{context.sha1().str(true)};
  dynamicCode dynCode{"codeStream" + sha1Str, sha1Str};
  // Load library if not already loaded
  // Version information is encoded in the libPath (encoded with the SHA1)
  const fileName libPath = dynCode.libPath();
  // see if library is loaded
  void* lib = nullptr;
  if (isA<IOdictionary>(parentDict.topDict())) {
    lib = libs(parentDict).findLibrary(libPath);
  }
  if (!lib) {
    Info << "Using #codeStream with " << libPath << endl;
  }
  // nothing loaded
  // avoid compilation if possible by loading an existing library
  if (!lib) {
    if (isA<IOdictionary>(parentDict.topDict())) {
      // Cached access to dl libs. Guarantees clean up upon destruction
      // of Time.
      dlLibraryTable& dlLibs = libs(parentDict);
      if (dlLibs.open(libPath, false)) {
        lib = dlLibs.findLibrary(libPath);
      }
    } else {
      // Uncached opening of libPath. Do not complain if cannot be loaded
      lib = dlOpen(libPath, false);
    }
  }
  // create library if required
  if (!lib) {
    bool create =
      Pstream::master() || (regIOobject::fileModificationSkew <= 0); // not NFS
    if (create) {
      if (!dynCode.upToDate(context)) {
        // filter with this context
        dynCode.reset(context);
        // compile filtered C template
        dynCode.addCompileFile(codeTemplateC);
        // define Make/options
        dynCode.setMakeOptions
        (
          "EXE_INC = -g \\\n"
          + context.options()
          + "\n\nLIB_LIBS = \\\n"
          + "    -lmousse_core \\\n"
          + context.libs()
        );
        if (!dynCode.copyOrCreateFiles(true)) {
          FATAL_IO_ERROR_IN
          (
            "functionEntries::codeStream::execute(..)",
            parentDict
          )
          << "Failed writing files for" << nl
          << dynCode.libRelPath() << nl
          << exit(FatalIOError);
        }
      }
      if (!dynCode.wmakeLibso()) {
        FATAL_IO_ERROR_IN
        (
          "functionEntries::codeStream::execute(..)",
          parentDict
        )
        << "Failed wmake " << dynCode.libRelPath() << nl
        << exit(FatalIOError);
      }
    }
    //- Only block if we're not doing master-only reading. (flag set by
    //  regIOobject::read, IOdictionary constructor)
    if (!regIOobject::masterOnlyReading
        && regIOobject::fileModificationSkew > 0) {
      //- Since the library has only been compiled on the master the
      //  other nodes need to pick this library up through NFS
      //  We do this by just polling a few times using the
      //  fileModificationSkew.
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
            parentDict
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
    if (isA<IOdictionary>(parentDict.topDict())) {
      // Cached access to dl libs. Guarantees clean up upon destruction
      // of Time.
      dlLibraryTable& dlLibs = libs(parentDict);
      if (debug) {
        Pout << "Opening cached dictionary:" << libPath << endl;
      }
      if (!dlLibs.open(libPath, false)) {
        FATAL_IO_ERROR_IN
        (
          "functionEntries::codeStream::execute(..)",
          parentDict
        )
        << "Failed loading library " << libPath << nl
        << "Did you add all libraries to the 'libs' entry"
        << " in system/controlDict?"
        << exit(FatalIOError);
      }
      lib = dlLibs.findLibrary(libPath);
    } else {
      // Uncached opening of libPath
      if (debug) {
        Pout << "Opening uncached dictionary:" << libPath << endl;
      }
      lib = dlOpen(libPath, true);
    }
  }
  bool haveLib = lib;
  if (!regIOobject::masterOnlyReading) {
    reduce(haveLib, andOp<bool>());
  }
  if (!haveLib) {
    FATAL_IO_ERROR_IN
    (
      "functionEntries::codeStream::execute(..)",
      parentDict
    )
    << "Failed loading library " << libPath
    << " on some processors."
    << exit(FatalIOError);
  }
  // Find the function handle in the library
  streamingFunctionType function =
    reinterpret_cast<streamingFunctionType>
    (
      dlSym(lib, dynCode.codeName())
    );
  if (!function) {
    FATAL_IO_ERROR_IN
    (
      "functionEntries::codeStream::execute(..)",
      parentDict
    )
    << "Failed looking up symbol " << dynCode.codeName()
    << " in library " << lib << exit(FatalIOError);
  }
  return function;
}


// Member Functions 
bool mousse::functionEntries::codeStream::execute
(
  const dictionary& parentDict,
  primitiveEntry& entry,
  Istream& is
)
{
  Info << "Using #codeStream at line " << is.lineNumber()
    << " in file " <<  parentDict.name() << endl;
  dynamicCode::checkSecurity
  (
    "functionEntries::codeStream::execute(..)",
    parentDict
  );
  // get code dictionary
  // must reference parent for stringOps::expand to work nicely
  dictionary codeDict{"#codeStream", parentDict, is};
  streamingFunctionType function = getFunction(parentDict, codeDict);
  // use function to write stream
  OStringStream os{is.format()};
  (*function)(os, parentDict);
  // get the entry from this stream
  IStringStream resultStream{os.str()};
  entry.read(parentDict, resultStream);
  return true;
}


bool mousse::functionEntries::codeStream::execute
(
  dictionary& parentDict,
  Istream& is
)
{
  Info << "Using #codeStream at line " << is.lineNumber()
    << " in file " <<  parentDict.name() << endl;
  dynamicCode::checkSecurity
  (
    "functionEntries::codeStream::execute(..)",
    parentDict
  );
  // get code dictionary
  // must reference parent for stringOps::expand to work nicely
  dictionary codeDict{"#codeStream", parentDict, is};
  streamingFunctionType function = getFunction(parentDict, codeDict);
  // use function to write stream
  OStringStream os{is.format()};
  (*function)(os, parentDict);
  // get the entry from this stream
  IStringStream resultStream(os.str());
  parentDict.read(resultStream);
  return true;
}
