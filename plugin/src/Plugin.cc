#include <glob.h>
#include <libgen.h>

#include <zeek/input.h>
#include <zeek/util.h>
#include <zeek/Obj.h>

#include "config.h"
#include "Plugin.h"

namespace zeek::plugin::Corelight_GlobLoad { Plugin plugin; }

using namespace zeek::plugin::Corelight_GlobLoad;

zeek::plugin::Configuration Plugin::Configure()
	{
	zeek::plugin::Configuration config;
	config.name = "Corelight::GlobLoad";
	config.description = "Support for glob patterns in @load directives";
	config.version.major = VERSION_MAJOR;
	config.version.minor = VERSION_MINOR;
	config.version.patch = VERSION_PATCH;
	return config;
	}

void Plugin::InitPreScript()
	{
	zeek::plugin::Plugin::InitPreScript();
	EnableHook(plugin::HOOK_LOAD_FILE);
	}

int Plugin::HookLoadFile(const LoadType type, const std::string& file, const std::string& resolved)
	{
	// Return value recap: 1 means we took over the file and Zeek
	// should look no further, 0 means we took over but hit
	// trouble, -1 if we don't care about the file.

	// At least for now, we only handle only script files.
	if ( type != SCRIPT )
		return -1;

	// If Zeek managed to resolve a path for the file/directory,
	// then this isn't a glob pattern and we're done.
	if ( ! resolved.empty() )
		return -1;

	// See if the glob pattern starts with an absolute path. If
	// so, we can use it as-is, otherwise we prefix our current
	// location's directory.
	std::string pattern;

	if ( file.rfind("/", 0) == 0 )
		pattern = file;
	else
		{
		auto cl = zeek::detail::GetCurrentLocation();

		if ( ! cl.filename )
			return -1;

		char path[PATH_MAX];

		if ( ! realpath(cl.filename, path) )
			return -1;

		pattern = std::string(dirname(path)) + "/" + file;
		}

	glob_t gt;
	memset(&gt, 0, sizeof(gt));

	if ( glob(pattern.c_str(), 0, nullptr, &gt) == 0 )
		{
		for ( size_t i = 0; i < gt.gl_pathc; i++ )
			add_input_file(gt.gl_pathv[i]);
		}

	globfree(&gt);
	return 1;
	}
