#pragma once
#ifndef INCLUDE_COMPONENTS_HPP
#define INCLUDE_COMPONENTS_HPP

#include "TipsyProto.hpp"

struct USB_A_Port : rack::app::SvgPort {
	USB_A_Port() {
		rack::app::SvgPort::setSvg(rack::window::Svg::load(rack::asset::plugin(pluginInstance, "res/USB-A.svg")));
	}
};

struct USB_B_Port : rack::app::SvgPort {
	USB_B_Port() {
		rack::app::SvgPort::setSvg(rack::window::Svg::load(rack::asset::system("res/ComponentLibrary/USB_B.svg")));
	}
};

#endif