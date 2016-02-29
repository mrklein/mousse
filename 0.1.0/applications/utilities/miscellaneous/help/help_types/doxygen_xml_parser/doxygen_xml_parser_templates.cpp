// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "doxygen_xml_parser.hpp"
// Member Functions 
template<class Type>
void mousse::doxygenXmlParser::getEntry(IFstream& is, Type& entry) const
{
  char c;
  while (is.get(c) && c != '<')
  {}
  entry = "";
  if (is.get(c) && c  == '/')
  {
    return;
  }
  else
  {
    entry = entry + c;
  }
  while (is.get(c) && c  != '>')
  {
    entry = entry + c;
  }
}
template<class Type>
void mousse::doxygenXmlParser::getValue(IFstream& is, Type& entry) const
{
  char c;
  entry = "";
  while (is.get(c) && c  != '<')
  {
    entry = entry + c;
  }
}
