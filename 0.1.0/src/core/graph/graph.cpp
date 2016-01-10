// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "graph.hpp"
#include "ofstream.hpp"
#include "iomanip.hpp"
#include "pair.hpp"
#include "os_specific.hpp"

// Static Data Members
namespace mousse
{

typedef graph::writer graphWriter;
DEFINE_TYPE_NAME_AND_DEBUG(graphWriter, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(graphWriter, word);

}

// Private Member Functions 
mousse::word mousse::graph::wordify(const mousse::string& sname)
{
  string wname = sname;
  wname.replace(' ', '_');
  wname.replace('(', '_');
  wname.replace(')', "");
  return word(wname);
}
void mousse::graph::readCurves(Istream& is)
{
  List<xy> xyData(is);
  x_.setSize(xyData.size());
  scalarField y(xyData.size());
  FOR_ALL(xyData, i)
  {
    x_[i] = xyData[i].x_;
    y[i] = xyData[i].y_;
  }
  insert
  (
    wordify(yName_),
    new curve(wordify(yName_), curve::curveStyle::CONTINUOUS, y)
  );
}
// Member Functions 
mousse::graph::graph
(
  const string& title,
  const string& xName,
  const string& yName,
  const scalarField& x
)
:
  title_(title),
  xName_(xName),
  yName_(yName),
  x_(x)
{}
mousse::graph::graph
(
  const string& title,
  const string& xName,
  const string& yName,
  const scalarField& x,
  const scalarField& y
)
:
  title_(title),
  xName_(xName),
  yName_(yName),
  x_(x)
{
  insert(wordify(yName), new curve(yName, curve::curveStyle::CONTINUOUS, y));
}
mousse::graph::graph
(
  const string& title,
  const string& xName,
  const string& yName,
  Istream& is
)
:
  title_(title),
  xName_(xName),
  yName_(yName)
{
  readCurves(is);
}
mousse::graph::graph(Istream& is)
:
  title_(is),
  xName_(is),
  yName_(is)
{
  readCurves(is);
}
const mousse::scalarField& mousse::graph::y() const
{
  if (size() != 1)
  {
    FATAL_ERROR_IN("const scalarField& graph::y() const")
      << "y field requested for graph containing " << size()
      << "ys" << exit(FatalError);
  }
  return *begin()();
}
mousse::scalarField& mousse::graph::y()
{
  if (size() != 1)
  {
    FATAL_ERROR_IN("scalarField& graph::y()")
      << "y field requested for graph containing " << size()
      << "ys" << exit(FatalError);
  }
  return *begin()();
}
mousse::autoPtr<mousse::graph::writer> mousse::graph::writer::New
(
  const word& graphFormat
)
{
  if (!wordConstructorTablePtr_)
  {
    FATAL_ERROR_IN
    (
      "graph::writer::New(const word&)"
    )   << "Graph writer table is empty"
      << exit(FatalError);
  }
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(graphFormat);
  if (cstrIter == wordConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "graph::writer::New(const word&)"
    )   << "Unknown graph format " << graphFormat
      << endl << endl
      << "Valid graph formats are : " << endl
      << wordConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<graph::writer>(cstrIter()());
}
void mousse::graph::writer::writeXY
(
  const scalarField& x,
  const scalarField& y,
  Ostream& os
) const
{
  FOR_ALL(x, xi)
  {
    os << setw(10) << x[xi] << token::SPACE << setw(10) << y[xi]<< endl;
  }
}
void mousse::graph::writeTable(Ostream& os) const
{
  FOR_ALL(x_, xi)
  {
    os  << setw(10) << x_[xi];
    FOR_ALL_CONST_ITER(graph, *this, iter)
    {
      os  << token::SPACE << setw(10) << (*iter())[xi];
    }
    os  << endl;
  }
}
void mousse::graph::write(Ostream& os, const word& format) const
{
  writer::New(format)().write(*this, os);
}
void mousse::graph::write(const fileName& pName, const word& format) const
{
  autoPtr<writer> graphWriter(writer::New(format));
  OFstream graphFile(pName + '.' + graphWriter().ext());
  if (graphFile.good())
  {
    write(graphFile, format);
  }
  else
  {
    WARNING_IN("graph::write(const word& format, const fileName& dir)")
      << "Could not open graph file " << graphFile.name()
      << endl;
  }
}
void mousse::graph::write
(
  const fileName& path,
  const word& name,
  const word& format
) const
{
  mkDir(path);
  write(path/name, format);
}
mousse::Ostream& mousse::operator<<(Ostream& os, const graph& g)
{
  g.writeTable(os);
  os.check("Ostream& operator<<(Ostream&, const graph&)");
  return os;
}
