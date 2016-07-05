// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "csv_table_reader.hpp"
#include "ifstream.hpp"
#include "dynamic_list.hpp"


// Constructors 
template<class Type>
mousse::csvTableReader<Type>::csvTableReader(const dictionary& dict)
:
  tableReader<Type>{dict},
  headerLine_{readBool(dict.lookup("hasHeaderLine"))},
  timeColumn_{readLabel(dict.lookup("timeColumn"))},
  componentColumns_{dict.lookup("valueColumns")},
  separator_{dict.lookupOrDefault<string>("separator", string(","))[0]}
{
  if (componentColumns_.size() != pTraits<Type>::nComponents) {
    FATAL_ERROR_IN("csvTableReader<Type>::csvTableReader(const dictionary&)")
      << componentColumns_ << " does not have the expected length "
      << pTraits<Type>::nComponents << endl
      << exit(FatalError);
  }
}


// Destructor 
template<class Type>
mousse::csvTableReader<Type>::~csvTableReader()
{}


// Member Functions 
namespace mousse {

// doesn't recognize specialization otherwise
template<>
scalar csvTableReader<scalar>::readValue(const List<string>& splitted)
{
  if (componentColumns_[0] >= splitted.size()) {
    FATAL_ERROR_IN
    (
      "csvTableReader<scalar>::readValue(const List<string>&)"
    )
    << "No column " << componentColumns_[0] << " in "
    << splitted << endl
    << exit(FatalError);
  }
  return readScalar(IStringStream(splitted[componentColumns_[0]])());
}


template<class Type>
Type csvTableReader<Type>::readValue(const List<string>& splitted)
{
  Type result;
  for(label i = 0;i < pTraits<Type>::nComponents; i++) {
    if (componentColumns_[i] >= splitted.size()) {
      FATAL_ERROR_IN
      (
        "csvTableReader<Type>::readValue(const List<string>&)"
      )
      << "No column " << componentColumns_[i] << " in "
      << splitted << endl
      << exit(FatalError);
    }
    result[i] = readScalar
    (
      IStringStream{splitted[componentColumns_[i]]}()
    );
  }
  return result;
}

}


template<class Type>
void mousse::csvTableReader<Type>::operator()
(
  const fileName& fName,
  List<Tuple2<scalar, Type>>& data
)
{
  IFstream in{fName};
  DynamicList<Tuple2<scalar, Type>> values;
  // Skip header
  if (headerLine_) {
    string line;
    in.getLine(line);
  }
  while (in.good()) {
    string line;
    in.getLine(line);
    DynamicList<string> splitted;
    std::size_t pos = 0;
    while (pos != std::string::npos) {
      std::size_t nPos = line.find(separator_, pos);
      if (nPos == std::string::npos) {
        splitted.append(line.substr(pos));
        pos=nPos;
      } else {
        splitted.append(line.substr(pos, nPos-pos));
        pos=nPos+1;
      }
    }
    if (splitted.size() <= 1) {
      break;
    }
    scalar time = readScalar(IStringStream{splitted[timeColumn_]}());
    Type value = readValue(splitted);
    values.append(Tuple2<scalar,Type>(time, value));
  }
  data.transfer(values);
}


template<class Type>
void mousse::csvTableReader<Type>::operator()
(
  const fileName& /*fName*/,
  List<Tuple2<scalar, List<Tuple2<scalar, Type>>>>& /*data*/
)
{
  NOT_IMPLEMENTED("csvTableReader<Type>::operator()");
}


template<class Type>
void mousse::csvTableReader<Type>::write(Ostream& os) const
{
  tableReader<Type>::write(os);
  os.writeKeyword("hasHeaderLine") << headerLine_ << token::END_STATEMENT << nl;
  os.writeKeyword("timeColumn") << timeColumn_ << token::END_STATEMENT << nl;
  // Force writing labelList in ascii
  os.writeKeyword("valueColumns");
  if (os.format() == IOstream::BINARY) {
    os.format(IOstream::ASCII);
    os  << componentColumns_;
    os.format(IOstream::BINARY);
  }
  os << token::END_STATEMENT << nl;
  os.writeKeyword("separator")
    << string(separator_) << token::END_STATEMENT << nl;
}

