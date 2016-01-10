// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef word_io_list_hpp_
#define word_io_list_hpp_
#include "word_list.hpp"
#include "iolist.hpp"
namespace mousse
{
  typedef IOList<word> wordIOList;
  typedef IOList<wordList> wordListIOList;
  // Print word list list as a table
  void printTable(const List<wordList>&, List<string::size_type>&, Ostream&);
  void printTable(const List<wordList>&, Ostream&);
}
#endif
