// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "print_table.hpp"


// Constructors 
template<class KeyType, class DataType>
mousse::PrintTable<KeyType, DataType>::PrintTable()
:
  table_{},
  title_{string::null}
{}


template<class KeyType, class DataType>
mousse::PrintTable<KeyType, DataType>::PrintTable(const string& title)
:
  table_{},
  title_{title}
{}


template<class KeyType, class DataType>
mousse::PrintTable<KeyType, DataType>::PrintTable
(
  const PrintTable<KeyType, DataType>& table
)
:
  table_{table.table_},
  title_{table.title_}
{}


// Destructor 
template<class KeyType, class DataType>
mousse::PrintTable<KeyType, DataType>::~PrintTable()
{}


// Member Functions 
template<class KeyType, class DataType>
void mousse::PrintTable<KeyType, DataType>::print
(
  Ostream& os,
  const bool printSum,
  const bool printAverage
) const
{
  HashTable<HashTable<DataType, label>, KeyType> combinedTable;
  List<HashTableData> procData{Pstream::nProcs(), HashTableData()};
  procData[Pstream::myProcNo()] = table_;
  Pstream::gatherList(procData);
  if (!Pstream::master())
    return;
  label largestKeyLength = 6;
  label largestDataLength = 0;
  List<label> largestProcSize{Pstream::nProcs(), 0};
  FOR_ALL(procData, procI) {
    const auto& procIData = procData[procI];
    for (auto iter = procIData.begin(); iter != procIData.end(); ++iter) {
      if (!combinedTable.found(iter.key())) {
        combinedTable.insert
        (
          iter.key(),
          HashTable<DataType, label>{}
        );
      }
      auto& key = combinedTable[iter.key()];
      key.insert(procI, iter());
      for (auto dataIter = key.begin();
           dataIter != key.end();
           ++dataIter) {
        std::ostringstream buf;
        buf << dataIter();
        largestDataLength = max(largestDataLength, label(buf.str().length()));
      }
      std::ostringstream buf;
      buf << iter.key();
      largestKeyLength = max(largestKeyLength, label(buf.str().length()));
    }
  }
  os.width(largestKeyLength);
  os << nl << indent << tab << "# " << title_.c_str() << endl;
  os.width(largestKeyLength);
  os << indent << "# Proc";
  FOR_ALL(procData, procI) {
    os << tab;
    os.width(largestDataLength);
    os << procI;
  }
  if (printSum) {
    os << tab;
    os.width(largestDataLength);
    os << "Sum";
  }
  if (printAverage) {
    os << tab;
    os.width(largestDataLength);
    os << "Average";
  }
  os << endl;
  const auto& sortedTable = combinedTable.sortedToc();
  FOR_ALL(sortedTable, keyI) {
    const auto& procDataList = combinedTable[sortedTable[keyI]];
    os.width(largestKeyLength);
    os << indent << sortedTable[keyI];
    FOR_ALL(procDataList, elemI) {
      os << tab;
      os.width(largestDataLength);
      os << procDataList[elemI];
    }
    if (printSum) {
      DataType sum = 0;
      FOR_ALL(procDataList, elemI) {
        sum += procDataList[elemI];
      }
      os << tab;
      os.width(largestDataLength);
      os  << sum;
      if (printAverage) {
        os << tab;
        os.width(largestDataLength);
        os << sum/Pstream::nProcs();
      }
    }
    os << endl;
  }
}

