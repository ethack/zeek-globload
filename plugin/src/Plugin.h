#pragma once

#include <zeek/plugin/Plugin.h>

namespace zeek::plugin {
namespace Corelight_GlobLoad {

class Plugin : public zeek::plugin::Plugin
{
protected:
	// Overridden from zeek::plugin::Plugin.
	zeek::plugin::Configuration Configure() override;

	void InitPreScript();
	int HookLoadFile(const LoadType, const std::string& file, const std::string& resolved) override;
};

extern Plugin plugin;

}
}
