#ifndef UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_DOXYGEN_XML_PARSER_DOXYGEN_XML_PARSER_HPP_
#define UTILITIES_MISCELLANEOUS_HELP_HELP_TYPES_DOXYGEN_XML_PARSER_DOXYGEN_XML_PARSER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::doxygenXmlParser
// Description
//   Parser for doxygen XML
// SourceFiles
//   doxygen_xml_parser.cpp
#include "dictionary.hpp"
#include "ifstream.hpp"
namespace mousse
{
class doxygenXmlParser
:
  public dictionary
{
public:
  //- Construct from components
  doxygenXmlParser
  (
    const fileName& fName,
    const string& startTag,
    const string& searchStr,
    const bool exactMatch
  );
  // Member functions
    //- Skip past a block
    void skipBlock(IFstream& is, const word& blockName) const;
    //- Skip forward to block
    void skipForward(IFstream& is, const word& blockName) const;
    //- Return the entry
    template<class Type>
    void getEntry(IFstream& is, Type& entry) const;
    //- Return the entry value
    template<class Type>
    void getValue(IFstream& is, Type& entry) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "doxygen_xml_parser_templates.cpp"
#endif
#endif
