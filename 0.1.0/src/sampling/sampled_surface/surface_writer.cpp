// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_writer.hpp"
#include "meshed_surface_proxy.hpp"
#include "proxy_surface_writer.hpp"
#include "hash_table.hpp"
#include "word.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(surfaceWriter, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(surfaceWriter, word);
DEFINE_RUN_TIME_SELECTION_TABLE(surfaceWriter, wordDict);
ADD_NAMED_TO_RUN_TIME_SELECTION_TABLE
(
  surfaceWriter,
  surfaceWriter,
  word,
  null
);

}


// Static Member Functions
mousse::autoPtr<mousse::surfaceWriter>
mousse::surfaceWriter::New(const word& writeType)
{
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(writeType);
  if (cstrIter == wordConstructorTablePtr_->end()) {
    if (MeshedSurfaceProxy<face>::canWriteType(writeType)) {
      // generally unknown, but can be written via MeshedSurfaceProxy
      // use 'proxy' handler instead
      return autoPtr<surfaceWriter>(new proxySurfaceWriter(writeType));
    }
    if (cstrIter == wordConstructorTablePtr_->end()) {
      FATAL_ERROR_IN
      (
        "surfaceWriter::New(const word&)"
      )
      << "Unknown write type \"" << writeType << "\"\n\n"
      << "Valid write types : "
      << wordConstructorTablePtr_->sortedToc() << nl
      << "Valid proxy types : "
      << MeshedSurfaceProxy<face>::writeTypes() << endl
      << exit(FatalError);
    }
  }
  return autoPtr<surfaceWriter>{cstrIter()()};
}


mousse::autoPtr<mousse::surfaceWriter>
mousse::surfaceWriter::New(const word& writeType, const dictionary& optDict)
{
  // find constructors with dictionary options
  wordDictConstructorTable::iterator cstrIter =
    wordDictConstructorTablePtr_->find(writeType);
  if (cstrIter == wordDictConstructorTablePtr_->end()) {
    // revert to versions without options
    return mousse::surfaceWriter::New(writeType);
  }
  return autoPtr<surfaceWriter>{cstrIter()(optDict)};
}


// Constructors 
mousse::surfaceWriter::surfaceWriter()
{}


// Destructor 
mousse::surfaceWriter::~surfaceWriter()
{}

