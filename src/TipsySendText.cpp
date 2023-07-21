//
// Created by Paul Walker on 7/14/23.
//

#include "TipsyWidgetBits.h"
#include "components.hpp"
#include <tipsy/tipsy.h>
#include <iostream>

struct TipsySendText : rack::Module
{
    enum ParamIds
    {
        NUM_PARAMS
    };

    enum InputIds
    {
        NUM_INPUTS
    };

    enum OutputIds
    {
        TXT_OUT,
        NUM_OUTPUTS
    };

    enum LightIds
    {
        NUM_LIGHTS
    };

    TipsySendText() { config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS); }

    std::string currentMessage{"Message Please"};
    std::atomic<int> messageUpdates;
    std::string sendingMessage{""};

    tipsy::ProtocolEncoder encoder;

    void process(const ProcessArgs &args) override
    {
        if (messageUpdates > 0 && encoder.isDormant())
        {
            sendingMessage = currentMessage;
            messageUpdates = 0;
            auto er = encoder.initiateMessage("text/plain", sendingMessage.size() + 1,
                                              (unsigned char *)sendingMessage.c_str());
            if (encoder.isError(er))
            {
                // do something
            }
        }

        float f = 0.f;
        auto res = encoder.getNextMessageFloat(f);
        if (!encoder.isError(res))
        {
            outputs[TXT_OUT].setVoltage(f);
        }
    }
};

struct TxtIn : rack::LedDisplayTextField
{
    TipsySendText *module;

    TxtIn()
    {
        textOffset = rack::Vec(1, 1);
        multiline = true;
    }

    void draw(const DrawArgs &args) override
    {
        nvgBeginPath(args.vg);
        nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 3);
        nvgFillColor(args.vg, nvgRGB(0, 0, 0));
        nvgFill(args.vg);

        nvgStrokeColor(args.vg, nvgRGB(255, 0x90, 0));
        nvgStroke(args.vg);

        LedDisplayTextField::draw(args);
    }

    void onChange(const ChangeEvent &e) override
    {
        if (module)
        {
            // Obviously this isn't thread safe. Make this way nicer
            module->currentMessage = getText();
            module->messageUpdates++;
        }
    }
};

struct TipsySendTextWidget : rack::ModuleWidget
{
    TipsySendTextWidget(TipsySendText *m)
    {
        setModule(m);

        box.size = rack::Vec(SCREW_WIDTH * 10, RACK_HEIGHT);
        addChild(TipsyBG::create(box.size));

        auto ti = new TxtIn();
        ti->module = m;
        if (m)
            ti->setText(m->currentMessage);
        ti->box.pos = rack::Vec(10, 10);
        ti->box.size = box.size;
        ti->box.size.x -= 20;
        ti->box.size.y -= 80;
        addChild(ti);

        addOutput(rack::createOutput<USB_A_Port>(rack::Vec(box.size.x - 50, RACK_HEIGHT - 40),
                                                 module, TipsySendText::TXT_OUT));
    }
};

rack::Model *tipsySendText = rack::createModel<TipsySendText, TipsySendTextWidget>("TipsySendText");
