// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "dynamic_list.hpp"
#include "demand_driven_data.hpp"
#include "sortable_list.hpp"
#include "list_ops.hpp"


// Private Member Functions 
// Returns edgeI between two points.
mousse::label mousse::primitiveMesh::getEdge
(
  List<DynamicList<label> >& pe,
  DynamicList<edge>& es,
  const label pointI,
  const label nextPointI
)
{
  // Find connection between pointI and nextPointI
  FOR_ALL(pe[pointI], ppI) {
    label eI = pe[pointI][ppI];
    const edge& e = es[eI];
    if (e.start() == nextPointI || e.end() == nextPointI) {
      return eI;
    }
  }
  // Make new edge.
  label edgeI = es.size();
  pe[pointI].append(edgeI);
  pe[nextPointI].append(edgeI);
  if (pointI < nextPointI) {
    es.append(edge(pointI, nextPointI));
  } else {
    es.append(edge(nextPointI, pointI));
  }
  return edgeI;
}


void mousse::primitiveMesh::calcEdges(const bool doFaceEdges) const
{
  if (debug) {
    Pout << "primitiveMesh::calcEdges(const bool) : "
      << "calculating edges, pointEdges and optionally faceEdges"
      << endl;
  }
  // It is an error to attempt to recalculate edges
  // if the pointer is already set
  if ((edgesPtr_ || pePtr_) || (doFaceEdges && fePtr_)) {
    FATAL_ERROR_IN("primitiveMesh::calcEdges(const bool) const")
      << "edges or pointEdges or faceEdges already calculated"
      << abort(FatalError);
  } else {
    // ALGORITHM:
    // Go through the faces list. Search pointEdges for existing edge.
    // If not found create edge and add to pointEdges.
    // In second loop sort edges in order of increasing neighbouring
    // point.
    // This algorithm replaces the one using pointFaces which used more
    // allocations but less memory and was on practical cases
    // quite a bit slower.
    const faceList& fcs = faces();
    // Size up lists
    // ~~~~~~~~~~~~~
    // Estimate pointEdges storage
    List<DynamicList<label>> pe{nPoints()};
    FOR_ALL(pe, pointI) {
      pe[pointI].setCapacity(primitiveMesh::edgesPerPoint_);
    }
    // Estimate edges storage
    DynamicList<edge> es{pe.size()*primitiveMesh::edgesPerPoint_/2};
    // Estimate faceEdges storage
    if (doFaceEdges) {
      fePtr_ = new labelListList{fcs.size()};
      labelListList& faceEdges = *fePtr_;
      FOR_ALL(fcs, faceI) {
        faceEdges[faceI].setSize(fcs[faceI].size());
      }
    }
    // Find consecutive face points in edge list
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Edges on boundary faces
    label nExtEdges = 0;
    // Edges using no boundary point
    nInternal0Edges_ = 0;
    // Edges using 1 boundary point
    label nInt1Edges = 0;
    // Edges using two boundary points but not on boundary face:
    // edges.size()-nExtEdges-nInternal0Edges_-nInt1Edges
    // Ordering is different if points are ordered.
    if (nInternalPoints_ == -1) {
      // No ordering. No distinction between types.
      FOR_ALL(fcs, faceI) {
        const face& f = fcs[faceI];
        FOR_ALL(f, fp) {
          label pointI = f[fp];
          label nextPointI = f[f.fcIndex(fp)];
          label edgeI = getEdge(pe, es, pointI, nextPointI);
          if (doFaceEdges) {
            (*fePtr_)[faceI][fp] = edgeI;
          }
        }
      }
      // Assume all edges internal
      nExtEdges = 0;
      nInternal0Edges_ = es.size();
      nInt1Edges = 0;
    } else {
      // 1. Do external faces first. This creates external edges.
      for (label faceI = nInternalFaces_; faceI < fcs.size(); faceI++) {
        const face& f = fcs[faceI];
        FOR_ALL(f, fp) {
          label pointI = f[fp];
          label nextPointI = f[f.fcIndex(fp)];
          label oldNEdges = es.size();
          label edgeI = getEdge(pe, es, pointI, nextPointI);
          if (es.size() > oldNEdges) {
            nExtEdges++;
          }
          if (doFaceEdges) {
            (*fePtr_)[faceI][fp] = edgeI;
          }
        }
      }
      // 2. Do internal faces. This creates internal edges.
      for (label faceI = 0; faceI < nInternalFaces_; faceI++) {
        const face& f = fcs[faceI];
        FOR_ALL(f, fp) {
          label pointI = f[fp];
          label nextPointI = f[f.fcIndex(fp)];
          label oldNEdges = es.size();
          label edgeI = getEdge(pe, es, pointI, nextPointI);
          if (es.size() > oldNEdges) {
            if (pointI < nInternalPoints_) {
              if (nextPointI < nInternalPoints_) {
                nInternal0Edges_++;
              } else {
                nInt1Edges++;
              }
            } else {
              if (nextPointI < nInternalPoints_) {
                nInt1Edges++;
              } else {
                // Internal edge with two points on boundary
              }
            }
          }
          if (doFaceEdges) {
            (*fePtr_)[faceI][fp] = edgeI;
          }
        }
      }
    }
    // For unsorted meshes the edges will be in order of occurrence inside
    // the faces. For sorted meshes the first nExtEdges will be the external
    // edges.
    if (nInternalPoints_ != -1) {
      nInternalEdges_ = es.size()-nExtEdges;
      nInternal1Edges_ = nInternal0Edges_+nInt1Edges;
    }
    // Like faces sort edges in order of increasing neigbouring point.
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Automatically if points are sorted into internal and external points
    // the edges will be sorted into
    // - internal point to internal point
    // - internal point to external point
    // - external point to external point
    // The problem is that the last one mixes external edges with internal
    // edges with two points on the boundary.
    // Map to sort into new upper-triangular order
    labelList oldToNew(es.size());
    if (debug) {
      oldToNew = -1;
    }
    // start of edges with 0 boundary points
    label internal0EdgeI = 0;
    // start of edges with 1 boundary points
    label internal1EdgeI = nInternal0Edges_;
    // start of edges with 2 boundary points
    label internal2EdgeI = nInternal1Edges_;
    // start of external edges
    label externalEdgeI = nInternalEdges_;
    // To sort neighbouring points in increasing order. Defined outside
    // for optimisation reasons: if all connectivity size same will need
    // no reallocations
    SortableList<label> nbrPoints{primitiveMesh::edgesPerPoint_};
    FOR_ALL(pe, pointI) {
      const DynamicList<label>& pEdges = pe[pointI];
      nbrPoints.setSize(pEdges.size());
      FOR_ALL(pEdges, i) {
        const edge& e = es[pEdges[i]];
        label nbrPointI = e.otherVertex(pointI);
        if (nbrPointI < pointI) {
          nbrPoints[i] = -1;
        } else {
          nbrPoints[i] = nbrPointI;
        }
      }
      nbrPoints.sort();
      if (nInternalPoints_ == -1) {
        // Sort all upper-triangular
        FOR_ALL(nbrPoints, i) {
          if (nbrPoints[i] != -1) {
            label edgeI = pEdges[nbrPoints.indices()[i]];
            oldToNew[edgeI] = internal0EdgeI++;
          }
        }
      } else {
        if (pointI < nInternalPoints_) {
          FOR_ALL(nbrPoints, i) {
            label nbrPointI = nbrPoints[i];
            label edgeI = pEdges[nbrPoints.indices()[i]];
            if (nbrPointI != -1) {
              if (edgeI < nExtEdges) {
                // External edge
                oldToNew[edgeI] = externalEdgeI++;
              } else if (nbrPointI < nInternalPoints_) {
                // Both points inside
                oldToNew[edgeI] = internal0EdgeI++;
              } else {
                // One points inside, one outside
                oldToNew[edgeI] = internal1EdgeI++;
              }
            }
          }
        } else {
          FOR_ALL(nbrPoints, i) {
            label nbrPointI = nbrPoints[i];
            label edgeI = pEdges[nbrPoints.indices()[i]];
            if (nbrPointI != -1) {
              if (edgeI < nExtEdges) {
                // External edge
                oldToNew[edgeI] = externalEdgeI++;
              } else if (nbrPointI < nInternalPoints_) {
                // Not possible!
                FATAL_ERROR_IN("primitiveMesh::calcEdges(..)")
                  << abort(FatalError);
              } else {
                // Both points outside
                oldToNew[edgeI] = internal2EdgeI++;
              }
            }
          }
        }
      }
    }
    if (debug) {
      label edgeI = findIndex(oldToNew, -1);
      if (edgeI != -1) {
        const edge& e = es[edgeI];
        FATAL_ERROR_IN("primitiveMesh::calcEdges(..)")
          << "Did not sort edge " << edgeI << " points:" << e
          << " coords:" << points()[e[0]] << points()[e[1]]
          << endl
          << "Current buckets:" << endl
          << "    internal0EdgeI:" << internal0EdgeI << endl
          << "    internal1EdgeI:" << internal1EdgeI << endl
          << "    internal2EdgeI:" << internal2EdgeI << endl
          << "    externalEdgeI:" << externalEdgeI << endl
          << abort(FatalError);
      }
    }
    // Renumber and transfer.
    // Edges
    edgesPtr_ = new edgeList{es.size()};
    edgeList& edges = *edgesPtr_;
    FOR_ALL(es, edgeI) {
      edges[oldToNew[edgeI]] = es[edgeI];
    }
    // pointEdges
    pePtr_ = new labelListList{nPoints()};
    labelListList& pointEdges = *pePtr_;
    FOR_ALL(pe, pointI) {
      DynamicList<label>& pEdges = pe[pointI];
      pEdges.shrink();
      inplaceRenumber(oldToNew, pEdges);
      pointEdges[pointI].transfer(pEdges);
      mousse::sort(pointEdges[pointI]);
    }
    // faceEdges
    if (doFaceEdges) {
      labelListList& faceEdges = *fePtr_;
      FOR_ALL(faceEdges, faceI) {
        inplaceRenumber(oldToNew, faceEdges[faceI]);
      }
    }
  }
}


mousse::label mousse::primitiveMesh::findFirstCommonElementFromSortedLists
(
  const labelList& list1,
  const labelList& list2
)
{
  label result = -1;
  labelList::const_iterator iter1 = list1.begin();
  labelList::const_iterator iter2 = list2.begin();
  while (iter1 != list1.end() && iter2 != list2.end()) {
    if (*iter1 < *iter2) {
      ++iter1;
    } else if (*iter1 > *iter2) {
      ++iter2;
    } else {
      result = *iter1;
      break;
    }
  }
  if (result == -1) {
    FATAL_ERROR_IN
    (
      "primitiveMesh::findFirstCommonElementFromSortedLists"
      "(const labelList&, const labelList&)"
    )
    << "No common elements in lists " << list1 << " and " << list2
    << abort(FatalError);
  }
  return result;
}


// Member Functions 
const mousse::edgeList& mousse::primitiveMesh::edges() const
{
  if (!edgesPtr_) {
    //calcEdges(true);
    calcEdges(false);
  }
  return *edgesPtr_;
}


const mousse::labelListList& mousse::primitiveMesh::pointEdges() const
{
  if (!pePtr_) {
    //calcEdges(true);
    calcEdges(false);
  }
  return *pePtr_;
}


const mousse::labelListList& mousse::primitiveMesh::faceEdges() const
{
  if (!fePtr_) {
    if (debug) {
      Pout << "primitiveMesh::faceEdges() : "
        << "calculating faceEdges" << endl;
    }
    //calcEdges(true);
    const faceList& fcs = faces();
    const labelListList& pe = pointEdges();
    const edgeList& es = edges();
    fePtr_ = new labelListList{fcs.size()};
    labelListList& faceEdges = *fePtr_;
    FOR_ALL(fcs, faceI) {
      const face& f = fcs[faceI];
      labelList& fEdges = faceEdges[faceI];
      fEdges.setSize(f.size());
      FOR_ALL(f, fp) {
        label pointI = f[fp];
        label nextPointI = f[f.fcIndex(fp)];
        // Find edge between pointI, nextPontI
        const labelList& pEdges = pe[pointI];
        FOR_ALL(pEdges, i) {
          label edgeI = pEdges[i];
          if (es[edgeI].otherVertex(pointI) == nextPointI) {
            fEdges[fp] = edgeI;
            break;
          }
        }
      }
    }
  }
  return *fePtr_;
}


void mousse::primitiveMesh::clearOutEdges()
{
  deleteDemandDrivenData(edgesPtr_);
  deleteDemandDrivenData(pePtr_);
  deleteDemandDrivenData(fePtr_);
  labels_.clear();
  labelSet_.clear();
}


const mousse::labelList& mousse::primitiveMesh::faceEdges
(
  const label faceI,
  DynamicList<label>& storage
) const
{
  if (hasFaceEdges()) {
    return faceEdges()[faceI];
  } else {
    const labelListList& pointEs = pointEdges();
    const face& f = faces()[faceI];
    storage.clear();
    if (f.size() > storage.capacity()) {
      storage.setCapacity(f.size());
    }
    FOR_ALL(f, fp) {
      storage.append
      (
        findFirstCommonElementFromSortedLists
        (
          pointEs[f[fp]],
          pointEs[f.nextLabel(fp)]
        )
      );
    }
    return storage;
  }
}


const mousse::labelList& mousse::primitiveMesh::faceEdges(const label faceI) const
{
  return faceEdges(faceI, labels_);
}


const mousse::labelList& mousse::primitiveMesh::cellEdges
(
  const label cellI,
  DynamicList<label>& storage
) const
{
  if (hasCellEdges()) {
    return cellEdges()[cellI];
  } else {
    const labelList& cFaces = cells()[cellI];
    labelSet_.clear();
    FOR_ALL(cFaces, i) {
      const labelList& fe = faceEdges(cFaces[i]);
      FOR_ALL(fe, feI) {
        labelSet_.insert(fe[feI]);
      }
    }
    storage.clear();
    if (labelSet_.size() > storage.capacity()) {
      storage.setCapacity(labelSet_.size());
    }
    FOR_ALL_CONST_ITER(labelHashSet, labelSet_, iter) {
      storage.append(iter.key());
    }
    return storage;
  }
}


const mousse::labelList& mousse::primitiveMesh::cellEdges(const label cellI) const
{
  return cellEdges(cellI, labels_);
}

