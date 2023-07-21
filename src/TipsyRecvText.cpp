//
// Created by Paul Walker on 7/14/23.
//

#include "TipsyWidgetBits.h"
#include "components.hpp"
#include <tipsy/tipsy.h>

struct TipsyRecvText : rack::Module
{
    std::string currentMessage{""};

    enum ParamIds
    {
        NUM_PARAMS
    };

    enum InputIds
    {
        TXT_IN,
        NUM_INPUTS
    };

    enum OutputIds
    {
        NUM_OUTPUTS
    };

    enum LightIds
    {
        NUM_LIGHTS
    };

    static constexpr size_t recvBufferSize{1024 * 64};
    unsigned char recvBuffer[recvBufferSize];
    TipsyRecvText()
    {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        decoder.provideDataBuffer(recvBuffer, recvBufferSize);
    }

    tipsy::ProtocolDecoder decoder;
    void process(const ProcessArgs &args) override
    {
        auto rf = decoder.readFloat(inputs[TXT_IN].getVoltage());
        if (!decoder.isError(rf))
        {
            if (rf == tipsy::DecoderResult::BODY_READY)
            {
                // Obviously check more things like it s a string byg
                currentMessage = std::string((const char *)recvBuffer);
            }
        }
    }
};

struct TxtOut : rack::LedDisplayTextField
{
    TipsyRecvText *module;

    TxtOut()
    {
        textOffset = rack::Vec(1, 1);
        multiline = true;
    }

    void draw(const DrawArgs &args) override
    {
        nvgBeginPath(args.vg);
        nvgRoundedRect(args.vg, 0, 0, box.size.x, box.size.y, 3);
        nvgFillColor(args.vg, nvgRGB(60, 80, 60));
        nvgFill(args.vg);

        nvgStrokeColor(args.vg, nvgRGB(0, 255, 0));
        nvgStroke(args.vg);

        LedDisplayTextField::draw(args);
    }

    void step() override
    {
        if (module)
        {
            if (module->currentMessage != getText())
            {
                setText(module->currentMessage);
            }
        }
    }
};

struct TipsyRecvTextWidget : rack::ModuleWidget
{
    TipsyRecvTextWidget(TipsyRecvText *m)
    {
        setModule(m);
        box.size = rack::Vec(SCREW_WIDTH * 10, RACK_HEIGHT);
        addChild(TipsyBG::create(box.size));

        auto ti = new TxtOut();
        ti->module = m;
        if (m)
            ti->setText(m->currentMessage);
        ti->box.pos = rack::Vec(10, 10);
        ti->box.size = box.size;
        ti->box.size.x -= 20;
        ti->box.size.y -= 80;
        addChild(ti);

        addInput(rack::createInput<USB_B_Port>(rack::Vec(box.size.x - 50, RACK_HEIGHT - 40), module,
                                               TipsyRecvText::TXT_IN));
    }
};

rack::Model *tipsyRecvText = rack::createModel<TipsyRecvText, TipsyRecvTextWidget>("TipsyRecvText");