//
//  Copyright (C) 2020-2022 Greg Landrum and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
// Original author: Greg Landrum
//
// A concrete class derived from DrawText that uses the JS Canvas
// to draw text onto a picture.
// Requires emscripten

#ifndef RDKIX_DRAWTEXTJS_H
#define RDKIX_DRAWTEXTJS_H

#include <iosfwd>

#include <emscripten.h>
#include <emscripten/val.h>
#include <GraphMol/MolDraw2D/DrawTextNotFT.h>

namespace RDKix {
namespace MolDraw2D_detail {
// ****************************************************************************

class DrawTextJS : public DrawTextNotFT {
 public:
  DrawTextJS(double max_fnt_sz, double min_fnt_sz, emscripten::val &context);
  DrawTextJS(const DrawTextJS &rhs) = delete;
  DrawTextJS(const DrawTextJS &&rhs) = delete;
  DrawTextJS &operator=(const DrawTextJS &rhs) = delete;
  DrawTextJS &operator=(const DrawTextJS &&rhs) = delete;

  void drawChar(char c, const Point2D &cds) override;

  emscripten::val &context_;

  // fills a vector of StringRects, one for each char in text, with
  // super- and subscripts taken into account.  Sizes in pixel coords,
  // i.e. scaled by fontScale().
  void getStringRects(const std::string &text,
                      std::vector<std::shared_ptr<StringRect>> &rects,
                      std::vector<TextDrawType> &draw_modes,
                      std::vector<char> &draw_chars) const override;
};

}  // namespace MolDraw2D_detail
}  // namespace RDKix

#endif  // RDKIX_DRAWTEXTSVG_H
