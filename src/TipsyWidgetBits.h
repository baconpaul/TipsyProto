//
// Created by Paul Walker on 7/14/23.
//

#ifndef TIPSYPROTO_TIPSYWIDGETBITS_H
#define TIPSYPROTO_TIPSYWIDGETBITS_H

#include "rack.hpp"
#include "sst/rackhelpers/ui.h"

#define SCREW_WIDTH 15
#define RACK_HEIGHT 380

struct TipsyBG : rack::Widget
{
    sst::rackhelpers::ui::BufferedDrawFunctionWidget *bg{nullptr};

    static TipsyBG *create(rack::Vec &sz)
    {
        auto res = new TipsyBG();
        res->box.size = sz;
        res->box.pos = {0,0};

        res->bg = new sst::rackhelpers::ui::BufferedDrawFunctionWidget(res->box.pos, res->box.size, [res](auto *v) { res->drawbg(v);});
        res->addChild(res->bg);

        return res;
    }

    void drawbg(NVGcontext *vg)
    {
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGB(180,220,180));
        nvgFillColor(vg, nvgRGB(30,50,30));
        nvgRect(vg, 0, 0, box.size.x, box.size.y);
        nvgFill(vg);
        nvgStroke(vg);
    }
};
#endif // TIPSYPROTO_TIPSYWIDGETBITS_H
