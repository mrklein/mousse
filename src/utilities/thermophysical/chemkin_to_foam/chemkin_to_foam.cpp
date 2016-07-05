// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arg_list.hpp"
#include "chemkin_reader.hpp"
#include "ofstream.hpp"


using namespace mousse;


int main(int argc, char *argv[])
{
  argList::validArgs.append("CHEMKINFile");
  argList::validArgs.append("CHEMKINThermodynamicsFile");
  argList::validArgs.append("FOAMChemistryFile");
  argList::validArgs.append("FOAMThermodynamicsFile");
  argList::addBoolOption
  (
    "newFormat",
    "read Chemkin thermo file in new format"
  );
  argList args(argc, argv);
  bool newFormat = args.optionFound("newFormat");
  speciesTable species;
  chemkinReader cr{args[1], species, args[2], newFormat};
  OFstream reactionsFile{args[3]};
  reactionsFile
    << "species" << cr.species() << token::END_STATEMENT << nl << nl;
  cr.reactions().write(reactionsFile);
  OFstream thermoFile{args[4]};
  cr.speciesThermo().write(thermoFile);
  Info << "End\n" << endl;
  return 0;
}

