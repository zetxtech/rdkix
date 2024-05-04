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

#ifdef __EMSCRIPTEN__
#include <GraphMol/MolDraw2D/DrawTextFT.h>
#include <GraphMol/MolDraw2D/DrawTextFTJS.h>

namespace RDKix {

std::string DrawColourToSVG(const RDKix::DrawColour &col);

namespace MolDraw2D_detail {
// ****************************************************************************
DrawTextFTJS::DrawTextFTJS(double max_fnt_sz, double min_fnt_sz,
                           const std::string &font_file,
                           emscripten::val &context)
    : DrawTextFT(max_fnt_sz, min_fnt_sz, font_file), context_(context) {}

// ****************************************************************************
double DrawTextFTJS::extractOutline() {
  std::string col = DrawColourToSVG(colour());
  context_.call<void>("beginPath");
  context_.set("fillStyle", col);

  double adv = DrawTextFT::extractOutline();
  context_.call<void>("fill");
  return adv;
}

// ****************************************************************************
int DrawTextFTJS::MoveToFunctionImpl(const FT_Vector *to) {
  double dx, dy;
  fontPosToDrawPos(to->x, to->y, dx, dy);
  context_.call<void>("moveTo", dx, dy);
  return 0;
}

// ****************************************************************************
int DrawTextFTJS::LineToFunctionImpl(const FT_Vector *to) {
  double dx, dy;
  fontPosToDrawPos(to->x, to->y, dx, dy);
  context_.call<void>("lineTo", dx, dy);
  return 0;
}

// ****************************************************************************
int DrawTextFTJS::ConicToFunctionImpl(const FT_Vector *control,
                                      const FT_Vector *to) {
  double controlX, controlY;
  fontPosToDrawPos(control->x, control->y, controlX, controlY);

  double dx, dy;
  fontPosToDrawPos(to->x, to->y, dx, dy);
  context_.call<void>("quadraticCurveTo", controlX, controlY, dx, dy);
  return 0;
}

// ****************************************************************************
int DrawTextFTJS::CubicToFunctionImpl(const FT_Vector *controlOne,
                                      const FT_Vector *controlTwo,
                                      const FT_Vector *to) {
  double controlOneX, controlOneY;
  fontPosToDrawPos(controlOne->x, controlOne->y, controlOneX, controlOneY);
  double controlTwoX, controlTwoY;
  fontPosToDrawPos(controlTwo->x, controlTwo->y, controlTwoX, controlTwoY);

  double dx, dy;
  fontPosToDrawPos(to->x, to->y, dx, dy);

  context_.call<void>("bezierCurveTo", controlOneX, controlOneY, controlTwoX,
                      controlTwoY, dx, dy);

  return 0;
}

}  // namespace MolDraw2D_detail
}  // namespace RDKix
#endif  // __EMSCRIPTEN__
