// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "word_io_list.hpp"
#include "token.hpp"


namespace mousse {

DEFINE_COMPOUND_TYPE_NAME(List<word>, wordList);
ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<word>, wordList);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME(wordIOList, "wordList", 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME
(
  wordListIOList,
  "wordListList",
  0
);

}


void mousse::printTable
(
  const List<wordList>& wll,
  List<string::size_type>& columnWidth,
  Ostream& os
)
{
  if (!wll.size())
    return;
  // Find the maximum word length for each column
  columnWidth.setSize(wll[0].size(), string::size_type(0));
  FOR_ALL(columnWidth, j)
  {
    FOR_ALL(wll, i)
    {
      columnWidth[j] = max(columnWidth[j], wll[i][j].size());
    }
  }
  // Print the rows adding spacing for the columns
  FOR_ALL(wll, i)
  {
    FOR_ALL(wll[i], j)
    {
      os << wll[i][j];
      for
      (
        string::size_type k=0;
        k<columnWidth[j] - wll[i][j].size() + 2;
        k++
      )
      {
        os << ' ';
      }
    }
    os << nl;
    if (i == 0)
      os  << nl;
  }
}


void mousse::printTable(const List<wordList>& wll, Ostream& os)
{
  List<string::size_type> columnWidth;
  printTable(wll, columnWidth, os);
}
