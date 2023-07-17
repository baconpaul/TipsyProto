/*
 * TipsyProto - Text Interchange Port System
 *
 * This source released under the MIT License, found in ~/LICENSE.md.
 *
 * Copyright 2023 by the authors as described in the github transaction log
 */

#include "TipsyProto.hpp"

#include <unordered_set>

rack::Plugin *pluginInstance;

__attribute__((__visibility__("default"))) void init(rack::Plugin *p)
{
    pluginInstance = p;

    pluginInstance->addModel(tipsySendText);
    pluginInstance->addModel(tipsyRecvText);

    INFO("Loading Tipsy");

}
