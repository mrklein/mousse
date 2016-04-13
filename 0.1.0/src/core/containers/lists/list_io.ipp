// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "list.hpp"
#include "istream.hpp"
#include "token.hpp"
#include "sl_list.hpp"
#include "contiguous.hpp"


// IOstream Operators 
// Construct from Istream
template<class T>
mousse::List<T>::List(Istream& is)
:
  UList<T>{NULL, 0}
{
  operator>>(is, *this);
}


template<class T>
mousse::Istream& mousse::operator>>(Istream& is, List<T>& L)
{
  // Anull list
  L.setSize(0);
  is.fatalCheck("operator>>(Istream&, List<T>&)");
  token firstToken(is);
  is.fatalCheck("operator>>(Istream&, List<T>&) : reading first token");
  if (firstToken.isCompound()) {
    L.transfer
    (
      dynamicCast<token::Compound<List<T>>>
      (
        firstToken.transferCompoundToken(is)
      )
    );
  } else if (firstToken.isLabel()) {
    label s = firstToken.labelToken();
    // Set list length to that read
    L.setSize(s);
    // Read list contents depending on data format
    if (is.format() == IOstream::ASCII || !contiguous<T>()) {
      // Read beginning of contents
      char delimiter = is.readBeginList("List");
      if (s) {
        if (delimiter == token::BEGIN_LIST) {
          for (label i=0; i<s; i++) {
            is >> L[i];
            is.fatalCheck
            (
              "operator>>(Istream&, List<T>&) : reading entry"
            );
          }
        } else {
          T element;
          is >> element;
          is.fatalCheck
          (
            "operator>>(Istream&, List<T>&) : "
            "reading the single entry"
          );
          for (label i=0; i<s; i++) {
            L[i] = element;
          }
        }
      }
      // Read end of contents
      is.readEndList("List");
    } else {
      if (s) {
        is.read(reinterpret_cast<char*>(L.data()), s*sizeof(T));
        is.fatalCheck
        (
          "operator>>(Istream&, List<T>&) : reading the binary block"
        );
      }
    }
  } else if (firstToken.isPunctuation()) {
    if (firstToken.pToken() != token::BEGIN_LIST) {
      FATAL_IO_ERROR_IN("operator>>(Istream&, List<T>&)", is)
        << "incorrect first token, expected '(', found "
        << firstToken.info()
        << exit(FatalIOError);
    }
    // Putback the opening bracket
    is.putBack(firstToken);
    // Now read as a singly-linked list
    SLList<T> sll{is};
    // Convert the singly-linked list to this list
    L = sll;
  } else {
    FATAL_IO_ERROR_IN("operator>>(Istream&, List<T>&)", is)
      << "incorrect first token, expected <int> or '(', found "
      << firstToken.info()
      << exit(FatalIOError);
  }
  return is;
}


template<class T>
mousse::List<T> mousse::readList(Istream& is)
{
  List<T> L;
  token firstToken(is);
  is.putBack(firstToken);
  if (firstToken.isPunctuation()) {
    if (firstToken.pToken() != token::BEGIN_LIST) {
      FATAL_IO_ERROR_IN("readList<T>(Istream&)", is)
        << "incorrect first token, expected '(', found "
        << firstToken.info()
        << exit(FatalIOError);
    }
    // read via a singly-linked list
    L = SLList<T>{is};
  } else {
    // create list with a single item
    L.setSize(1);
    is >> L[0];
  }
  return L;
}
