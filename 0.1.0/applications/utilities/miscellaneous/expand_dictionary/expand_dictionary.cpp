// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "ifstream.hpp"
#include "ioobject.hpp"
#include "dictionary.hpp"
#include "include_entry.hpp"

using namespace mousse;
int main(int argc, char *argv[])
{
  argList::addNote
  (
    "Read the specified dictionary file, expand the macros etc. and write\n"
    "the resulting dictionary to standard output."
  );
  argList::addBoolOption
  (
    "list",
    "Report the #include/#includeIfPresent to stdout only"
  );
  argList::noBanner();
  argList::noParallel();
  argList::validArgs.append("inputDict");
  argList args(argc, argv);
  const string dictName = args[1];
  const bool listOpt = args.optionFound("list");
  if (listOpt)
  {
    mousse::functionEntries::includeEntry::report = true;
  }
  dictionary dict{IFstream(dictName)(), true};
  if (!listOpt)
  {
    IOobject::writeBanner(Info)
      <<"//\n// " << dictName << "\n//\n";
    dict.write(Info, false);
    IOobject::writeDivider(Info);
  }
  return 0;
}
