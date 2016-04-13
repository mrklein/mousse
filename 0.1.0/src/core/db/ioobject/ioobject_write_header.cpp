// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ioobject.hpp"
#include "object_registry.hpp"


bool mousse::IOobject::writeHeader(Ostream& os, const word& type) const
{
  if (!os.good()) {
    Info << "IOobject::writeHeader(Ostream&) : "
      << "no stream open for write" << nl
      << os.info() << endl;
    return false;
  }
  writeBanner(os)
    << "FoamFile\n{\n"
    << "    version     " << os.version() << ";\n"
    << "    format      " << os.format() << ";\n"
    << "    class       " << type << ";\n";
  if (note().size()) {
    os << "    note        " << note() << ";\n";
  }
  os << "    location    " << instance()/db().dbDir()/local() << ";\n"
    << "    object      " << name() << ";\n"
    << "}" << nl;
  writeDivider(os) << endl;
  return true;
}


bool mousse::IOobject::writeHeader(Ostream& os) const
{
  return writeHeader(os, type());
}
