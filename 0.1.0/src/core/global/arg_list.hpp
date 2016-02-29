#ifndef CORE_GLOBAL_ARG_LIST_HPP_
#define CORE_GLOBAL_ARG_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::argList
// Description
//   Extract command arguments and options from the supplied
//   \a argc and \a argv parameters.
//   Sequences with "(" ... ")" are transformed into a stringList.
//   For example,
//   \verbatim
//     program -listFiles \( *.txt \)
//   \endverbatim
//   would create a stringList:
//   \verbatim
//     ( "file1.txt" "file2.txt" ... "fileN.txt" )
//   \endverbatim
//   The backslash-escaping is required to avoid interpretation by the shell.
//   \par Default command-line options
//   \param -case \<dir\> \n
//     select a case directory instead of the current working directory
//   \param -parallel \n
//     specify case as a parallel job
//   \param -doc \n
//     display the documentation in browser
//   \param -srcDoc \n
//     display the source documentation in browser
//   \param -help \n
//    print the usage
//   The environment variable \b MOUSSE_CASE is set to the path of the
//   global case (same for serial and parallel jobs).
//   The environment variable \b MOUSSE_CASENAME is set to the name of the
//   global case.
// Note
//   - The document browser used is defined by the \b MOUSSE_DOC_BROWSER
//    environment variable or the <tt>Documentation/docBrowser</tt> entry
//    in the <tt>~mousse/controlDict</tt> file.
//    The \%f token is used as a placeholder for the file name.
//   - The valid (mandatory) arguments can be adjusted
//    by directly manipulating the argList::validArgs static member.
//   - The valid options can be adjusted
//    via the addOption/removeOption static methods instead of directly
//    manipulating the argList::validOptions static member.
// SourceFiles
//   arg_list.cpp
#include "string_list.hpp"
#include "sub_list.hpp"
#include "sl_list.hpp"
#include "hash_table.hpp"
#include "word.hpp"
#include "file_name.hpp"
#include "par_run.hpp"
#include "istring_stream.hpp"
#include "os_specific.hpp"
#include "sig_fpe.hpp"
#include "sig_int.hpp"
#include "sig_quit.hpp"
#include "sig_segv.hpp"
namespace mousse
{
class argList
{
  // Private data
    static bool bannerEnabled;
    //- Switch on/off parallel mode. Has to be first to be constructed
    //  so destructor is done last.
    ParRunControl parRunControl_;
    stringList args_;
    HashTable<string> options_;
    word executable_;
    fileName rootPath_;
    fileName globalCase_;
    fileName case_;
    string argListStr_;
    // Signal handlers
    sigFpe sigFpe_;
    sigInt sigInt_;
    sigQuit sigQuit_;
    sigSegv sigSegv_;
  // Private Member Functions
    //- Helper function for printUsage
    static void printOptionUsage
    (
      const label location,
      const string& str
    );
    //- Get rootPath_ / globalCase_ from one of the following forms
    //   * [-case dir]
    //   * cwd
    //
    // Also export MOUSSE_CASE and MOUSSE_CASENAME environment variables
    // so they can be used immediately (eg, in decomposeParDict)
    void getRootCase();
    //- Transcribe argv into internal args_
    //  return true if any "(" ... ")" sequences were captured
    bool regroupArgv(int& argc, char**& argv);
public:
  // Static data members
    //- A list of valid (mandatory) arguments
    static SLList<string> validArgs;
    //- A list of valid options
    static HashTable<string> validOptions;
    //- A list of valid parallel options
    static HashTable<string> validParOptions;
    //- Short usage information for validOptions
    static HashTable<string> optionUsage;
    //- Additional notes for usage
    static SLList<string> notes;
    //- Min offset for displaying usage (default: 20)
    static string::size_type usageMin;
    //- Max screen width for displaying usage (default: 80)
    static string::size_type usageMax;
    //! \cond internalClass
    class initValidTables
    {
    public:
      initValidTables();
    };
    //! \endcond
  // Constructors
    //- Construct from argc and argv
    //  checking the arguments and options as requested
    argList
    (
      int& argc,
      char**& argv,
      bool checkArgs = true,
      bool checkOpts = true,
      bool initialise = true
    );
    //- Construct copy with new options
    argList
    (
      const argList& args,
      const HashTable<string>& options,
      bool checkArgs = true,
      bool checkOpts = true,
      bool initialise = true
    );
  //- Destructor
  virtual ~argList();
  // Member functions
    //- Parse
    void parse
    (
      bool checkArgs,
      bool checkOpts,
      bool initialise
    );
    // Access
      //- Name of executable without the path
      inline const word& executable() const;
      //- Return root path
      inline const fileName& rootPath() const;
      //- Return case name (parallel run) or global case (serial run)
      inline const fileName& caseName() const;
      //- Return case name
      inline const fileName& globalCaseName() const;
      //- Return parRunControl
      inline const ParRunControl& parRunControl() const;
      //- Return the path to the caseName
      inline fileName path() const;
      //- Return arguments
      inline const stringList& args() const;
      //- Return non-const access to arguments
      inline stringList& args();
      //- Return the argument corresponding to index.
      inline const string& arg(const label index) const;
      //- Return the number of arguments
      inline label size() const;
      //- Read a value from the argument at index.
      //  Index 0 corresponds to the name of the executable.
      //  Index 1 corresponds to the first argument.
      template<class T>
      inline T argRead(const label index) const;
      //- Return arguments that are additional to the executable
      //  \deprecated use operator[] directly (deprecated Feb 2010)
      stringList::subList additionalArgs() const
      {
        return stringList::subList(args_, args_.size()-1, 1);
      }
      //- Return options
      inline const mousse::HashTable<string>& options() const;
      //- Return non-const access to options
      inline mousse::HashTable<string>& options();
      //- Return the argument string associated with the named option
      inline const string& option(const word& opt) const;
      //- Return true if the named option is found
      inline bool optionFound(const word& opt) const;
      //- Return an IStringStream from the named option
      inline IStringStream optionLookup(const word& opt) const;
      //- Read a value from the named option
      template<class T>
      inline T optionRead(const word& opt) const;
      //- Read a value from the named option if present.
      //  Return true if the named option was found.
      template<class T>
      inline bool optionReadIfPresent(const word& opt, T&) const;
      //- Read a value from the named option if present.
      //  Return true if the named option was found, otherwise
      //  use the supplied default and return false.
      template<class T>
      inline bool optionReadIfPresent
      (
        const word& opt,
        T&,
        const T& deflt
      ) const;
      //- Read a value from the named option if present.
      //  Return supplied default otherwise.
      template<class T>
      inline T optionLookupOrDefault
      (
        const word& opt,
        const T& deflt
      ) const;
      //- Read a List of values from the named option
      template<class T>
      List<T> optionReadList(const word& opt) const
      {
        return readList<T>(optionLookup(opt)());
      }
      //- Return the argument corresponding to index.
      //  Index 0 corresponds to the name of the executable.
      //  Index 1 corresponds to the first argument.
      inline const string& operator[](const label index) const;
      //- Return the argument string associated with the named option
      //  \sa option()
      inline const string& operator[](const word& opt) const;
    // Edit
      //- Add to a bool option to validOptions with usage information
      static void addBoolOption
      (
        const word& opt,
        const string& usage = ""
      );
      //- Add to an option to validOptions with usage information
      //  An option with an empty param is a bool option
      static void addOption
      (
        const word& opt,
        const string& param = "",
        const string& usage = ""
      );
      //- Add option usage information to optionUsage
      static void addUsage
      (
        const word& opt,
        const string& usage
      );
      //- Add extra notes for the usage information
      //  This string is used "as-is" without additional formatting
      static void addNote(const string&);
      //- Remove option from validOptions and from optionUsage
      static void removeOption(const word& opt);
      //- Disable emitting the banner information
      static void noBanner();
      //- Remove the parallel options
      static void noParallel();
      //- Set option directly (use with caution)
      //  An option with an empty param is a bool option.
      //  Not all valid options can also be set: eg, -case, -roots, ...
      //  Return true if the existing option value needed changing,
      //  or if the option did not previously exist.
      bool setOption(const word& opt, const string& param = "");
      //- Unset option directly (use with caution)
      //  Not all valid options can also be unset: eg, -case, -roots ...
      //  Return true if the option existed before being unset.
      bool unsetOption(const word& opt);
    // Print
      //- Print notes (if any)
      void printNotes() const;
      //- Print usage
      void printUsage() const;
      //- Display documentation in browser
      //  Optionally display the application source code
      void displayDoc(bool source=false) const;
    // Check
      //- Check argument list
      bool check(bool checkArgs=true, bool checkOpts=true) const;
      //- Check root path and case path
      bool checkRootCase() const;
};
}  // namespace mousse

// Member Functions 
inline const mousse::word& mousse::argList::executable() const
{
  return executable_;
}
inline const mousse::fileName& mousse::argList::rootPath() const
{
  return rootPath_;
}
inline const mousse::fileName& mousse::argList::caseName() const
{
  return case_;
}
inline const mousse::fileName& mousse::argList::globalCaseName() const
{
  return globalCase_;
}
inline const mousse::ParRunControl& mousse::argList::parRunControl() const
{
  return parRunControl_;
}
inline mousse::fileName mousse::argList::path() const
{
  return rootPath()/caseName();
}
inline const mousse::stringList& mousse::argList::args() const
{
  return args_;
}
inline mousse::stringList& mousse::argList::args()
{
  return args_;
}
inline const mousse::string& mousse::argList::arg(const label index) const
{
  return args_[index];
}
inline mousse::label mousse::argList::size() const
{
  return args_.size();
}
inline const mousse::HashTable<mousse::string>& mousse::argList::options() const
{
  return options_;
}
inline mousse::HashTable<mousse::string>& mousse::argList::options()
{
  return options_;
}
inline const mousse::string& mousse::argList::option(const word& opt) const
{
  return options_[opt];
}
inline bool mousse::argList::optionFound(const word& opt) const
{
  return options_.found(opt);
}
inline mousse::IStringStream mousse::argList::optionLookup(const word& opt) const
{
  return IStringStream(options_[opt]);
}
// Template Specializations 
namespace mousse
{
  // Template specialization for string
  template<>
  inline mousse::string
  mousse::argList::argRead<mousse::string>(const label index) const
  {
    return args_[index];
  }
  // Template specialization for word
  template<>
  inline mousse::word
  mousse::argList::argRead<mousse::word>(const label index) const
  {
    return args_[index];
  }
  // Template specialization for fileName
  template<>
  inline mousse::fileName
  mousse::argList::argRead<mousse::fileName>(const label index) const
  {
    return args_[index];
  }
  // Template specialization for string
  template<>
  inline mousse::string
  mousse::argList::optionRead<mousse::string>(const word& opt) const
  {
    return options_[opt];
  }
  // Template specialization for word
  template<>
  inline mousse::word
  mousse::argList::optionRead<mousse::word>(const word& opt) const
  {
    return options_[opt];
  }
  // Template specialization for fileName
  template<>
  inline mousse::fileName
  mousse::argList::optionRead<mousse::fileName>(const word& opt) const
  {
    return options_[opt];
  }
}
// Member Functions 
template<class T>
inline T mousse::argList::argRead(const label index) const
{
  T val;
  IStringStream(args_[index])() >> val;
  return val;
}
template<class T>
inline T mousse::argList::optionRead(const word& opt) const
{
  T val;
  optionLookup(opt)() >> val;
  return val;
}
template<class T>
inline bool mousse::argList::optionReadIfPresent
(
  const word& opt,
  T& val
) const
{
  if (optionFound(opt))
  {
    val = optionRead<T>(opt);
    return true;
  }
  else
  {
    return false;
  }
}
template<class T>
inline bool mousse::argList::optionReadIfPresent
(
  const word& opt,
  T& val,
  const T& deflt
) const
{
  if (optionReadIfPresent<T>(opt, val))
  {
    return true;
  }
  else
  {
    val = deflt;
    return false;
  }
}
template<class T>
inline T mousse::argList::optionLookupOrDefault
(
  const word& opt,
  const T& deflt
) const
{
  if (optionFound(opt))
  {
    return optionRead<T>(opt);
  }
  else
  {
    return deflt;
  }
}
// Member Operators 
inline const mousse::string& mousse::argList::operator[](const label index) const
{
  return args_[index];
}
inline const mousse::string& mousse::argList::operator[](const word& opt) const
{
  return options_[opt];
}
#endif
