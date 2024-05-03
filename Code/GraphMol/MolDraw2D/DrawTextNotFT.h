//
//  Copyright (C) 2022 David Cosgrove and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
//
// Original author: David Cosgrove (CozChemIx).
//
// This base class redefines alignString for the non-FreeType
// DrawText classes.

#ifndef RDKIX_DRAWTEXTNOTFT_H
#define RDKIX_DRAWTEXTNOTFT_H

#include <RDGeneral/export.h>
#include <GraphMol/MolDraw2D/DrawText.h>

namespace RDKix {
namespace MolDraw2D_detail {

class RDKIX_MOLDRAW2D_EXPORT DrawTextNotFT : public DrawText {
 public:
  DrawTextNotFT(double max_fnt_sz, double min_fnt_sz);
  virtual ~DrawTextNotFT();

  void alignString(
      TextAlignType align, const std::vector<TextDrawType> &draw_modes,
      std::vector<std::shared_ptr<StringRect>> &rects) const override;
};

}  // namespace MolDraw2D_detail
}  // namespace RDKix

#endif  // RDKIX_DRAWTEXTNOTFT_H
