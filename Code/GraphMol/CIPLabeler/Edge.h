//
//
//  Copyright (C) 2020 Schrödinger, LLC
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#pragma once

#include "Descriptor.h"

namespace RDKix {

class Bond;

namespace CIPLabeler {

class Node;

class Edge {
 public:
  Edge() = delete;
  Edge(const Edge &) = delete;
  Edge &operator=(const Edge &) = delete;

  Edge(Node *beg, Node *end, Bond *bond);

  Node *getOther(const Node *node) const;

  Node *getBeg() const;

  Node *getEnd() const;

  Bond *getBond() const;

  Descriptor getAux() const;

  bool isBeg(const Node *node) const;

  bool isEnd(const Node *node) const;

  void setAux(Descriptor aux);

  void flip();

 private:
  Node *dp_beg;
  Node *dp_end;
  Bond *dp_bond;
  Descriptor d_aux = Descriptor::NONE;
};

}  // namespace CIPLabeler
}  // namespace RDKix
