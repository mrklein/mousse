// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "os_specific.hpp"
#include "clock.hpp"
#include "ifstream.hpp"
#include "dictionary.hpp"
#include "ioobject.hpp"
#include "job_info.hpp"
#include "label_list.hpp"
#include "reg_ioobject.hpp"
#include "dynamic_code.hpp"
#include "ipstream.hpp"
#include "opstream.hpp"
#include <cctype>

// Static Data Members
bool mousse::argList::bannerEnabled = true;
mousse::SLList<mousse::string>    mousse::argList::validArgs;
mousse::HashTable<mousse::string> mousse::argList::validOptions;
mousse::HashTable<mousse::string> mousse::argList::validParOptions;
mousse::HashTable<mousse::string> mousse::argList::optionUsage;
mousse::SLList<mousse::string>    mousse::argList::notes;
mousse::string::size_type mousse::argList::usageMin = 20;
mousse::string::size_type mousse::argList::usageMax = 80;
mousse::argList::initValidTables::initValidTables()
{
  argList::addOption
  (
    "case", "dir",
    "specify alternate case directory, default is the cwd"
  );
  argList::addBoolOption("parallel", "run in parallel");
  validParOptions.set("parallel", "");
  argList::addOption
  (
    "roots", "(dir1 .. dirN)",
    "slave root directories for distributed running"
  );
  validParOptions.set("roots", "(dir1 .. dirN)");
  argList::addBoolOption
  (
    "noFunctionObjects",
    "do not execute functionObjects"
  );
  Pstream::addValidParOptions(validParOptions);
}
mousse::argList::initValidTables dummyInitValidTables;
// Static Member Functions
void mousse::argList::addBoolOption
(
  const word& opt,
  const string& usage
)
{
  addOption(opt, "", usage);
}
void mousse::argList::addOption
(
  const word& opt,
  const string& param,
  const string& usage
)
{
  validOptions.set(opt, param);
  if (!usage.empty())
  {
    optionUsage.set(opt, usage);
  }
}
void mousse::argList::addUsage
(
  const word& opt,
  const string& usage
)
{
  if (usage.empty())
  {
    optionUsage.erase(opt);
  }
  else
  {
    optionUsage.set(opt, usage);
  }
}
void mousse::argList::addNote(const string& note)
{
  if (!note.empty())
  {
    notes.append(note);
  }
}
void mousse::argList::removeOption(const word& opt)
{
  validOptions.erase(opt);
  optionUsage.erase(opt);
}
void mousse::argList::noBanner()
{
  bannerEnabled = false;
}
void mousse::argList::noParallel()
{
  removeOption("parallel");
  removeOption("roots");
  validParOptions.clear();
}
void mousse::argList::printOptionUsage
(
  const label location,
  const string& str
)
{
  const string::size_type textWidth = usageMax - usageMin;
  const string::size_type strLen = str.size();
  if (strLen)
  {
    // minimum of 2 spaces between option and usage:
    if (string::size_type(location) + 2 <= usageMin)
    {
      for (string::size_type i = location; i < usageMin; ++i)
      {
        Info<<' ';
      }
    }
    else
    {
      // or start a new line
      Info<< nl;
      for (string::size_type i = 0; i < usageMin; ++i)
      {
        Info<<' ';
      }
    }
    // text wrap
    string::size_type pos = 0;
    while (pos != string::npos && pos + textWidth < strLen)
    {
      // potential end point and next point
      string::size_type curr = pos + textWidth - 1;
      string::size_type next = string::npos;
      if (isspace(str[curr]))
      {
        // we were lucky: ended on a space
        next = str.find_first_not_of(" \t\n", curr);
      }
      else if (isspace(str[curr+1]))
      {
        // the next one is a space - so we are okay
        curr++;  // otherwise the length is wrong
        next = str.find_first_not_of(" \t\n", curr);
      }
      else
      {
        // search for end of a previous word break
        string::size_type prev = str.find_last_of(" \t\n", curr);
        // reposition to the end of previous word if possible
        if (prev != string::npos && prev > pos)
        {
          curr = prev;
        }
      }
      if (next == string::npos)
      {
        next = curr + 1;
      }
      // indent following lines (not the first one)
      if (pos)
      {
        for (string::size_type i = 0; i < usageMin; ++i)
        {
          Info<<' ';
        }
      }
      Info<< str.substr(pos, (curr - pos)).c_str() << nl;
      pos = next;
    }
    // output the remainder of the string
    if (pos != string::npos)
    {
      // indent following lines (not the first one)
      if (pos)
      {
        for (string::size_type i = 0; i < usageMin; ++i)
        {
          Info<<' ';
        }
      }
      Info<< str.substr(pos).c_str() << nl;
    }
  }
  else
  {
    Info<< nl;
  }
}
// Private Member Functions 
// convert argv -> args_
// transform sequences with "(" ... ")" into string lists in the process
bool mousse::argList::regroupArgv(int& argc, char**& argv)
{
  int nArgs = 0;
  int listDepth = 0;
  string tmpString;
  // note: we also re-write directly into args_
  // and use a second pass to sort out args/options
  for (int argI = 0; argI < argc; ++argI)
  {
    if (strcmp(argv[argI], "(") == 0)
    {
      ++listDepth;
      tmpString += "(";
    }
    else if (strcmp(argv[argI], ")") == 0)
    {
      if (listDepth)
      {
        --listDepth;
        tmpString += ")";
        if (listDepth == 0)
        {
          args_[nArgs++] = tmpString;
          tmpString.clear();
        }
      }
      else
      {
        args_[nArgs++] = argv[argI];
      }
    }
    else if (listDepth)
    {
      // quote each string element
      tmpString += "\"";
      tmpString += argv[argI];
      tmpString += "\"";
    }
    else
    {
      args_[nArgs++] = argv[argI];
    }
  }
  if (tmpString.size())
  {
    args_[nArgs++] = tmpString;
  }
  args_.setSize(nArgs);
  return nArgs < argc;
}
void mousse::argList::getRootCase()
{
  fileName casePath;
  // [-case dir] specified
  HashTable<string>::const_iterator iter = options_.find("case");
  if (iter != options_.end())
  {
    casePath = iter();
    casePath.clean();
    if (casePath.empty() || casePath == ".")
    {
      // handle degenerate form and '-case .' like no -case specified
      casePath = cwd();
      options_.erase("case");
    }
    else if (!casePath.isAbsolute() && casePath.name() == "..")
    {
      // avoid relative cases ending in '..' - makes for very ugly names
      casePath = cwd()/casePath;
      casePath.clean();
    }
  }
  else
  {
    // nothing specified, use the current dir
    casePath = cwd();
  }
  rootPath_   = casePath.path();
  globalCase_ = casePath.name();
  case_       = globalCase_;
  // Set the case and case-name as an environment variable
  if (rootPath_.isAbsolute())
  {
    // absolute path - use as-is
    setEnv("MOUSSE_CASE", rootPath_/globalCase_, true);
    setEnv("MOUSSE_CASENAME", globalCase_, true);
  }
  else
  {
    // qualify relative path
    casePath = cwd()/rootPath_/globalCase_;
    casePath.clean();
    setEnv("MOUSSE_CASE", casePath, true);
    setEnv("MOUSSE_CASENAME", casePath.name(), true);
  }
}
// Constructors 
mousse::argList::argList
(
  int& argc,
  char**& argv,
  bool checkArgs,
  bool checkOpts,
  const bool initialise
)
:
  args_(argc),
  options_(argc)
{
  // Check if this run is a parallel run by searching for any parallel option
  // If found call runPar which might filter argv
  for (int argI = 0; argI < argc; ++argI)
  {
    if (argv[argI][0] == '-')
    {
      const char *optionName = &argv[argI][1];
      if (validParOptions.found(optionName))
      {
        parRunControl_.runPar(argc, argv);
        break;
      }
    }
  }
  // convert argv -> args_ and capture ( ... ) lists
  // for normal arguments and for options
  regroupArgv(argc, argv);
  // Get executable name
  args_[0]    = fileName(argv[0]);
  executable_ = fileName(argv[0]).name();
  // Check arguments and options, we already have argv[0]
  int nArgs = 1;
  argListStr_ = args_[0];
  for (int argI = 1; argI < args_.size(); ++argI)
  {
    argListStr_ += ' ';
    argListStr_ += args_[argI];
    if (args_[argI][0] == '-')
    {
      const char *optionName = &args_[argI][1];
      if
      (
        (
          validOptions.found(optionName)
        && !validOptions[optionName].empty()
        )
      || (
          validParOptions.found(optionName)
        && !validParOptions[optionName].empty()
        )
      )
      {
        ++argI;
        if (argI >= args_.size())
        {
          FatalError
            <<"Option '-" << optionName
            << "' requires an argument" << endl;
          printUsage();
          FatalError.exit();
        }
        argListStr_ += ' ';
        argListStr_ += args_[argI];
        options_.insert(optionName, args_[argI]);
      }
      else
      {
        options_.insert(optionName, "");
      }
    }
    else
    {
      if (nArgs != argI)
      {
        args_[nArgs] = args_[argI];
      }
      ++nArgs;
    }
  }
  args_.setSize(nArgs);
  parse(checkArgs, checkOpts, initialise);
}
mousse::argList::argList
(
  const argList& args,
  const HashTable<string>& options,
  bool checkArgs,
  bool checkOpts,
  bool initialise
)
:
  parRunControl_(args.parRunControl_),
  args_(args.args_),
  options_(options),
  executable_(args.executable_),
  rootPath_(args.rootPath_),
  globalCase_(args.globalCase_),
  case_(args.case_),
  argListStr_(args.argListStr_)
{
  parse(checkArgs, checkOpts, initialise);
}
void mousse::argList::parse
(
  bool checkArgs,
  bool checkOpts,
  bool initialise
)
{
  // Help/documentation options:
  //   -help    print the usage
  //   -doc     display application documentation in browser
  //   -srcDoc  display source code in browser
  if
  (
    options_.found("help")
  || options_.found("doc")
  || options_.found("srcDoc")
  )
  {
    if (options_.found("help"))
    {
      printUsage();
    }
    // only display one or the other
    if (options_.found("srcDoc"))
    {
      displayDoc(true);
    }
    else if (options_.found("doc"))
    {
      displayDoc(false);
    }
    ::exit(0);
  }
  // Print the usage message and exit if the number of arguments is incorrect
  if (!check(checkArgs, checkOpts))
  {
    FatalError.exit();
  }
  if (initialise)
  {
    string dateString = clock::date();
    string timeString = clock::clockTime();
    // Print the banner once only for parallel runs
    if (Pstream::master() && bannerEnabled)
    {
      IOobject::writeBanner(Info, true)
        << "Build  : " << mousse::mousse_build << nl
        << "Exec   : " << argListStr_.c_str() << nl
        << "Date   : " << dateString.c_str() << nl
        << "Time   : " << timeString.c_str() << nl
        << "Host   : " << hostName() << nl
        << "PID    : " << pid() << endl;
    }
    jobInfo.add("startDate", dateString);
    jobInfo.add("startTime", timeString);
    jobInfo.add("userName", userName());
    jobInfo.add("mousseVersion", word(mousse_version));
    jobInfo.add("code", executable_);
    jobInfo.add("argList", argListStr_);
    jobInfo.add("currentDir", cwd());
    jobInfo.add("PPID", ppid());
    jobInfo.add("PGID", pgid());
    // add build information - only use the first word
    {
      std::string build(mousse::mousse_build);
      std::string::size_type found = build.find(' ');
      if (found != std::string::npos)
      {
        build.resize(found);
      }
      jobInfo.add("mousseBuild", build);
    }
  }
  // Case is a single processor run unless it is running parallel
  int nProcs = 1;
  // Roots if running distributed
  fileNameList roots;
  // If this actually is a parallel run
  if (parRunControl_.parRun())
  {
    // For the master
    if (Pstream::master())
    {
      // establish rootPath_/globalCase_/case_ for master
      getRootCase();
      // See if running distributed (different roots for different procs)
      label dictNProcs = -1;
      fileName source;
      if (options_.found("roots"))
      {
        source = "-roots";
        IStringStream is(options_["roots"]);
        roots = readList<fileName>(is);
        if (roots.size() != 1)
        {
          dictNProcs = roots.size()+1;
        }
      }
      else
      {
        source = rootPath_/globalCase_/"system/decomposeParDict";
        IFstream decompDictStream(source);
        if (!decompDictStream.good())
        {
          FatalError
            << "Cannot read "
            << decompDictStream.name()
            << exit(FatalError);
        }
        dictionary decompDict(decompDictStream);
        dictNProcs = readLabel
        (
          decompDict.lookup("numberOfSubdomains")
        );
        if (decompDict.lookupOrDefault("distributed", false))
        {
          decompDict.lookup("roots") >> roots;
        }
      }
      // convenience:
      // when a single root is specified, use it for all processes
      if (roots.size() == 1)
      {
        const fileName rootName(roots[0]);
        roots.setSize(Pstream::nProcs()-1, rootName);
        // adjust dictNProcs for command-line '-roots' option
        if (dictNProcs < 0)
        {
          dictNProcs = roots.size()+1;
        }
      }
      // Check number of processors.
      // nProcs     => number of actual procs
      // dictNProcs => number of procs specified in decompositionDict
      // nProcDirs  => number of processor directories
      //               (n/a when running distributed)
      //
      // - normal running : nProcs = dictNProcs = nProcDirs
      // - decomposition to more  processors : nProcs = dictNProcs
      // - decomposition to fewer processors : nProcs = nProcDirs
      if (dictNProcs > Pstream::nProcs())
      {
        FatalError
          << source
          << " specifies " << dictNProcs
          << " processors but job was started with "
          << Pstream::nProcs() << " processors."
          << exit(FatalError);
      }
      // distributed data
      if (roots.size())
      {
        if (roots.size() != Pstream::nProcs()-1)
        {
          FatalError
            << "number of entries in roots "
            << roots.size()
            << " is not equal to the number of slaves "
            << Pstream::nProcs()-1
            << exit(FatalError);
        }
        FOR_ALL(roots, i)
        {
          roots[i].expand();
        }
        // Distribute the master's argument list (with new root)
        bool hadCaseOpt = options_.found("case");
        for
        (
          int slave = Pstream::firstSlave();
          slave <= Pstream::lastSlave();
          slave++
        )
        {
          options_.set("case", roots[slave-1]/globalCase_);
          OPstream toSlave(Pstream::scheduled, slave);
          toSlave << args_ << options_;
        }
        options_.erase("case");
        // restore [-case dir]
        if (hadCaseOpt)
        {
          options_.set("case", rootPath_/globalCase_);
        }
      }
      else
      {
        // Possibly going to fewer processors.
        // Check if all procDirs are there.
        if (dictNProcs < Pstream::nProcs())
        {
          label nProcDirs = 0;
          while
          (
            isDir
            (
              rootPath_/globalCase_/"processor"
             + name(++nProcDirs)
            )
          )
          {}
          if (nProcDirs != Pstream::nProcs())
          {
            FatalError
              << "number of processor directories = "
              << nProcDirs
              << " is not equal to the number of processors = "
              << Pstream::nProcs()
              << exit(FatalError);
          }
        }
        // Distribute the master's argument list (unaltered)
        for
        (
          int slave = Pstream::firstSlave();
          slave <= Pstream::lastSlave();
          slave++
        )
        {
          OPstream toSlave(Pstream::scheduled, slave);
          toSlave << args_ << options_;
        }
      }
    }
    else
    {
      // Collect the master's argument list
      IPstream fromMaster(Pstream::scheduled, Pstream::masterNo());
      fromMaster >> args_ >> options_;
      // establish rootPath_/globalCase_/case_ for slave
      getRootCase();
    }
    nProcs = Pstream::nProcs();
    case_ = globalCase_/(word("processor") + name(Pstream::myProcNo()));
  }
  else
  {
    // establish rootPath_/globalCase_/case_
    getRootCase();
    case_ = globalCase_;
  }
  stringList slaveProcs;
  // collect slave machine/pid
  if (parRunControl_.parRun())
  {
    if (Pstream::master())
    {
      slaveProcs.setSize(Pstream::nProcs() - 1);
      string  slaveMachine;
      label slavePid;
      label procI = 0;
      for
      (
        int slave = Pstream::firstSlave();
        slave <= Pstream::lastSlave();
        slave++
      )
      {
        IPstream fromSlave(Pstream::scheduled, slave);
        fromSlave >> slaveMachine >> slavePid;
        slaveProcs[procI++] = slaveMachine + "." + name(slavePid);
      }
    }
    else
    {
      OPstream toMaster(Pstream::scheduled, Pstream::masterNo());
      toMaster << hostName() << pid();
    }
  }
  if (Pstream::master() && bannerEnabled)
  {
    Info<< "Case   : " << (rootPath_/globalCase_).c_str() << nl
      << "nProcs : " << nProcs << endl;
    if (parRunControl_.parRun())
    {
      Info<< "Slaves : " << slaveProcs << nl;
      if (roots.size())
      {
        Info<< "Roots  : " << roots << nl;
      }
      Info<< "Pstream initialized with:" << nl
        << "    floatTransfer      : " << Pstream::floatTransfer << nl
        << "    nProcsSimpleSum    : " << Pstream::nProcsSimpleSum << nl
        << "    commsType          : "
        << Pstream::commsTypeNames[Pstream::defaultCommsType] << nl
        << "    polling iterations : " << Pstream::nPollProcInterfaces
        << endl;
    }
  }
  if (initialise)
  {
    jobInfo.add("root", rootPath_);
    jobInfo.add("case", globalCase_);
    jobInfo.add("nProcs", nProcs);
    if (slaveProcs.size())
    {
      jobInfo.add("slaves", slaveProcs);
    }
    if (roots.size())
    {
      jobInfo.add("roots", roots);
    }
    jobInfo.write();
    // Switch on signal trapping. We have to wait until after Pstream::init
    // since this sets up its own ones.
    sigFpe_.set(bannerEnabled);
    sigInt_.set(bannerEnabled);
    sigQuit_.set(bannerEnabled);
    sigSegv_.set(bannerEnabled);
    if (bannerEnabled)
    {
      Info<< "fileModificationChecking : "
        << "Monitoring run-time modified files using "
        << regIOobject::fileCheckTypesNames
          [
            regIOobject::fileModificationChecking
          ]
        << endl;
      Info<< "allowSystemOperations : ";
      if (dynamicCode::allowSystemOperations)
      {
        Info<< "Allowing user-supplied system call operations" << endl;
      }
      else
      {
        Info<< "Disallowing user-supplied system call operations"
          << endl;
      }
    }
    if (Pstream::master() && bannerEnabled)
    {
      Info<< endl;
      IOobject::writeDivider(Info);
    }
  }
}
// Destructor 
mousse::argList::~argList()
{
  jobInfo.end();
}
// Member Functions 
bool mousse::argList::setOption(const word& opt, const string& param)
{
  bool changed = false;
  // only allow valid options
  if (validOptions.found(opt))
  {
    // some options are to be protected
    if
    (
      opt == "case"
    || opt == "parallel"
    || opt == "roots"
    )
    {
      FatalError
        <<"used argList::setOption on a protected option: '"
        << opt << "'" << endl;
      FatalError.exit();
    }
    if (validOptions[opt].empty())
    {
      // bool option
      if (!param.empty())
      {
        // disallow change of type
        FatalError
          <<"used argList::setOption to change bool to non-bool: '"
          << opt << "'" << endl;
        FatalError.exit();
      }
      else
      {
        // did not previously exist
        changed = !options_.found(opt);
      }
    }
    else
    {
      // non-bool option
      if (param.empty())
      {
        // disallow change of type
        FatalError
          <<"used argList::setOption to change non-bool to bool: '"
          << opt << "'" << endl;
        FatalError.exit();
      }
      else
      {
        // existing value needs changing, or did not previously exist
        changed = options_.found(opt) ? options_[opt] != param : true;
      }
    }
  }
  else
  {
    FatalError
      <<"used argList::setOption on an invalid option: '"
      << opt << "'" << nl << "allowed are the following:"
      << validOptions << endl;
    FatalError.exit();
  }
  // set/change the option as required
  if (changed)
  {
    options_.set(opt, param);
  }
  return changed;
}
bool mousse::argList::unsetOption(const word& opt)
{
  // only allow valid options
  if (validOptions.found(opt))
  {
    // some options are to be protected
    if
    (
      opt == "case"
    || opt == "parallel"
    || opt == "roots"
    )
    {
      FatalError
        <<"used argList::unsetOption on a protected option: '"
        << opt << "'" << endl;
      FatalError.exit();
    }
    // remove the option, return true if state changed
    return options_.erase(opt);
  }
  else
  {
    FatalError
      <<"used argList::unsetOption on an invalid option: '"
      << opt << "'" << nl << "allowed are the following:"
      << validOptions << endl;
    FatalError.exit();
  }
  return false;
}
void mousse::argList::printNotes() const
{
  // output notes directly - no automatic text wrapping
  if (!notes.empty())
  {
    Info<< nl;
    FOR_ALL_CONST_ITER(SLList<string>, notes, iter)
    {
      Info<< iter().c_str() << nl;
    }
  }
}
void mousse::argList::printUsage() const
{
  Info<< "\nUsage: " << executable_ << " [OPTIONS]";
  FOR_ALL_CONST_ITER(SLList<string>, validArgs, iter)
  {
    Info<< " <" << iter().c_str() << '>';
  }
  Info<< "\noptions:\n";
  wordList opts = validOptions.sortedToc();
  FOR_ALL(opts, optI)
  {
    const word& optionName = opts[optI];
    HashTable<string>::const_iterator iter = validOptions.find(optionName);
    Info<< "  -" << optionName;
    label len = optionName.size() + 3;  // length includes leading '  -'
    if (iter().size())
    {
      // length includes space and between option/param and '<>'
      len += iter().size() + 3;
      Info<< " <" << iter().c_str() << '>';
    }
    HashTable<string>::const_iterator usageIter =
      optionUsage.find(optionName);
    if (usageIter != optionUsage.end())
    {
      printOptionUsage
      (
        len,
        usageIter()
      );
    }
    else
    {
      Info<< nl;
    }
  }
  //
  // place srcDoc/doc/help options at the end
  //
  Info<< "  -srcDoc";
  printOptionUsage
  (
    9,
    "display source code in browser"
  );
  Info<< "  -doc";
  printOptionUsage
  (
    6,
    "display application documentation in browser"
  );
  Info<< "  -help";
  printOptionUsage
  (
    7,
    "print the usage"
  );
  printNotes();
  Info<< nl
    <<"Using: mousse-" << mousse::mousse_version
    << " (see http://github.com/mrklein/mousse)" << nl
    <<"Build: " << mousse::mousse_build << nl
    << endl;
}
void mousse::argList::displayDoc(bool source) const
{
  const dictionary& docDict = debug::controlDict().subDict("Documentation");
  List<fileName> docDirs(docDict.lookup("doxyDocDirs"));
  List<fileName> docExts(docDict.lookup("doxySourceFileExts"));
  // for source code: change foo_8C.html to foo_8C_source.html
  if (source)
  {
    FOR_ALL(docExts, extI)
    {
      docExts[extI].replace(".", "_source.");
    }
  }
  fileName docFile;
  bool found = false;
  FOR_ALL(docDirs, dirI)
  {
    FOR_ALL(docExts, extI)
    {
      docFile = docDirs[dirI]/executable_ + docExts[extI];
      docFile.expand();
      if (isFile(docFile))
      {
        found = true;
        break;
      }
    }
    if (found)
    {
      break;
    }
  }
  if (found)
  {
    string docBrowser = getEnv("MOUSSE_DOC_BROWSER");
    if (docBrowser.empty())
    {
      docDict.lookup("docBrowser") >> docBrowser;
    }
    // can use MOUSSE_DOC_BROWSER='application file://%f' if required
    docBrowser.replaceAll("%f", docFile);
    Info<< "Show documentation: " << docBrowser.c_str() << endl;
    system(docBrowser);
  }
  else
  {
    Info<< nl
      << "No documentation found for " << executable_
      << ", but you can use -help to display the usage\n" << endl;
  }
}
bool mousse::argList::check(bool checkArgs, bool checkOpts) const
{
  bool ok = true;
  if (Pstream::master())
  {
    if (checkArgs && args_.size() - 1 != validArgs.size())
    {
      FatalError
        << "Wrong number of arguments, expected " << validArgs.size()
        << " found " << args_.size() - 1 << endl;
      ok = false;
    }
    if (checkOpts)
    {
      FOR_ALL_CONST_ITER(HashTable<string>, options_, iter)
      {
        if
        (
          !validOptions.found(iter.key())
          && !validParOptions.found(iter.key())
        )
        {
          FatalError << "Invalid option: -" << iter.key() << endl;
          ok = false;
        }
      }
    }
    if (!ok)
    {
      printUsage();
    }
  }
  return ok;
}
bool mousse::argList::checkRootCase() const
{
  if (!isDir(rootPath()))
  {
    FatalError
      << executable_
      << ": cannot open root directory " << rootPath()
      << endl;
    return false;
  }
  if (!isDir(path()) && Pstream::master())
  {
    // Allow slaves on non-existing processor directories, created later
    FatalError
      << executable_
      << ": cannot open case directory " << path()
      << endl;
    return false;
  }
  return true;
}
