// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "x3d_surface_format_core.hpp"
#include "clock.hpp"
// Private Member Functions 
void mousse::fileFormats::X3DsurfaceFormatCore::writeHeader
(
  Ostream& os
)
{
  os  <<
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<!DOCTYPE X3D PUBLIC \"ISO//Web3D//DTD X3D 3.0//EN\" "
    "\"http://www.web3d.org/specifications/x3d-3.0.dtd\">\n"
    "<X3D\n"
    "  version='3.0'\n"
    "  profile='Immersive'\n"
    "  xmlns:xsd='http://www.w3.org/2001/XMLSchema-instance'\n"
    "  xsd:noNamespaceSchemaLocation="
    "'http://www.web3d.org/specifications/x3d-3.0.xsd'\n"
    "  >\n";
}
void mousse::fileFormats::X3DsurfaceFormatCore::writeAppearance
(
  Ostream& os
)
{
  os  <<
    "  <Appearance>\n"
    "   <Material"
    " diffuseColor='0.8 0.8 0.8'"
    " specularColor='1.0 1.0 1.0'"
    " shininess='0.5'"
    " transparency='0.0'"
    " />\n"           // end material
    "  </Appearance>\n";
}
