// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "time.hpp"
#include "dictionary.hpp"
#include "ifstream.hpp"
using namespace mousse;
int main(int argc, char *argv[])
{
  argList::addNote
  (
    "interrogates a case and prints information to stdout"
  );
  argList::noBanner();
  argList::addBoolOption("times", "list available times");
  argList::addBoolOption("latestTime", "list last time");
  argList::addBoolOption
  (
    "keywords",
    "report keywords for the specified dictionary"
  );
  #include "add_dict_option.inc"
  argList::addOption
  (
    "entry",
    "name",
    "report the named entry for the specified dictionary"
  );
  #include "set_root_case.inc"
  if (args.optionFound("times"))
  {
    instantList times
    {
      mousse::Time::findTimes(args.rootPath()/args.caseName())
    };
    FOR_ALL(times, i)
    {
      Info<< times[i].name() << endl;
    }
  }
  else if (args.optionFound("latestTime"))
  {
    instantList times
    {
      mousse::Time::findTimes(args.rootPath()/args.caseName())
    };
    Info<< times.last().name() << endl;
  }
  if (args.optionFound("dict"))
  {
    fileName dictPath = args["dict"];
    const fileName dictFileName
    {
      dictPath.isAbsolute()
     ? dictPath
     : args.rootPath()/args.caseName()/args["dict"]
    };
    IFstream dictFile{dictFileName};
    if (dictFile.good())
    {
      dictionary dict{dictFile};
      if (args.optionFound("entry"))
      {
        fileName entryName{args.option("entry")};
        const entry* entPtr = NULL;
        if (entryName.find('.') != string::npos)
        {
          // New syntax
          entPtr = dict.lookupScopedEntryPtr
          (
            entryName,
            false,
            true            // wildcards
          );
        }
        else
        {
          // Old syntax
          wordList entryNames{entryName.components(':')};
          if (dict.found(entryNames[0]))
          {
            entPtr = &dict.lookupEntry
            (
              entryNames[0],
              false,
              true            // wildcards
            );
            for (int i=1; i<entryNames.size(); ++i)
            {
              if (entPtr->dict().found(entryNames[i]))
              {
                entPtr = &entPtr->dict().lookupEntry
                (
                  entryNames[i],
                  false,
                  true    // wildcards
                );
              }
              else
              {
                FATAL_ERROR_IN(args.executable())
                  << "Cannot find sub-entry " << entryNames[i]
                  << " in entry " << args["entry"]
                  << " in dictionary " << dictFileName;
                FatalError.exit(3);
              }
            }
          }
        }
        if (entPtr)
        {
          if (args.optionFound("keywords"))
          {
            const dictionary& dict = entPtr->dict();
            FOR_ALL_CONST_ITER(dictionary, dict, iter)
            {
              Info<< iter().keyword() << endl;
            }
          }
          else
          {
            Info<< *entPtr << endl;
          }
        }
        else
        {
          FATAL_ERROR_IN(args.executable())
            << "Cannot find entry "
            << entryName
            << " in dictionary " << dictFileName;
          FatalError.exit(2);
        }
      }
      else if (args.optionFound("keywords"))
      {
        FOR_ALL_CONST_ITER(dictionary, dict, iter)
        {
          Info<< iter().keyword() << endl;
        }
      }
      else
      {
        Info<< dict;
      }
    }
    else
    {
      FATAL_ERROR_IN(args.executable())
        << "Cannot open file " << dictFileName;
      FatalError.exit(1);
    }
  }
  return 0;
}
