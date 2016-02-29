// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "help_type.hpp"
#include "doxygen_xml_parser.hpp"
#include "sortable_list.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(helpType, 0);
  defineRunTimeSelectionTable(helpType, dictionary);
}
// Protected Member Functions 
mousse::fileName mousse::helpType::doxygenPath() const
{
  const dictionary& docDict = debug::controlDict().subDict("Documentation");
  List<fileName> docDirs(docDict.lookup("doxyDocDirs"));
  label dirI = -1;
  forAll(docDirs, i)
  {
    if (isDir(docDirs[i].expand()))
    {
      dirI = i;
      break;
    }
  }
  if (dirI == -1)
  {
    Info<< "No Doxygen sources found under search paths: "
      << docDirs << endl;
    return fileName();
  }
  return docDirs[dirI];
}
void mousse::helpType::displayDocOptions
(
  const string& searchStr,
  const bool exactMatch
) const
{
  fileName doxyPath(doxygenPath());
  if (doxyPath.empty())
  {
    return;
  }
  Info<< "Found doxygen help in " << doxyPath.c_str() << nl << endl;
  doxygenXmlParser parser
  (
    doxyPath/"../DTAGS",
    "tagfile",
    searchStr,
    exactMatch
  );
  Info<< "Valid types include:" << nl << SortableList<word>(parser.toc());
}
void mousse::helpType::displayDoc
(
  const word& className,
  const string& searchStr,
  const bool exactMatch
) const
{
  fileName doxyPath(doxygenPath());
  if (doxyPath.empty())
  {
    return;
  }
  Info<< "Found doxygen help in " << doxyPath.c_str() << nl << endl;
  string docBrowser = getEnv("FOAM_DOC_BROWSER");
  if (docBrowser.empty())
  {
    const dictionary& docDict =
      debug::controlDict().subDict("Documentation");
    docDict.lookup("docBrowser") >> docBrowser;
  }
  doxygenXmlParser parser
  (
    doxyPath/"../DTAGS",
    "tagfile",
    searchStr,
    exactMatch
  );
  if (debug)
  {
    Info<< parser;
  }
  if (parser.found(className))
  {
    fileName docFile(doxyPath/parser.subDict(className).lookup("filename"));
    // can use FOAM_DOC_BROWSER='application file://%f' if required
    docBrowser.replaceAll("%f", docFile);
    fileName classDirectory(parser.subDict(className).lookup("path"));
    word classFile(parser.subDict(className).lookup("name"));
    Info<< "Showing documentation for type " << className << nl << endl;
    Info<< "Source file: " << classDirectory.c_str() << classFile << nl
      << endl;
    system(docBrowser);
  }
  else
  {
    FatalErrorIn
    (
      "void mousse::helpType::displayDoc"
      "("
        "const word&, "
        "const string&, "
        "const bool"
      ")"
    )
      << "No help for type " << className << " found."
      << "  Valid options include:" << SortableList<word>(parser.toc())
      << exit(FatalError);
  }
}
// Constructors 
mousse::helpType::helpType()
{}
// Destructor 
mousse::helpType::~helpType()
{}
// Member Functions 
void mousse::helpType::init()
{
  argList::addOption
  (
    "browse",
    "word",
    "display documentation for boundary condition in browser"
  );
}
