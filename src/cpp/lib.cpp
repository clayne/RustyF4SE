#include <iostream>
#include <rustyf4se.h>
#include "Papyrus/Papyrus.h"

#define PROJECT_NAME "rustyf4se"

namespace rustyf4se {

void log_info(rust::Str text) {logger::info(std::string(text));}
void log_warning(rust::Str text) {logger::warn(std::string(text));}

} // namespace rustyf4se

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "" PROJECT_NAME ".log";
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = PROJECT_NAME;
	a_info->version = Version::MAJOR;

	if (a_f4se->IsEditor()) {
		logger::critical("loaded in editor"sv);
		return false;
	}

	const auto ver = a_f4se->RuntimeVersion();
	if (ver < F4SE::RUNTIME_1_10_162) {
		logger::critical("unsupported runtime v{}"sv, ver.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);

	logger::info("loaded " PROJECT_NAME " v{}"sv, Version::NAME);

	const auto papyrus = F4SE::GetPapyrusInterface();
	papyrus->Register(Papyrus::Bind);

    rustyf4se::introduce_yourself();

	return true;
}