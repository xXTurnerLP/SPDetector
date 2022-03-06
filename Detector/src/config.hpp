#pragma once

#include <unistd.h>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "../vendor/simpleini/SimpleIni.h"

class Config
{
public:
	/**
	 * Generates default config if no config is found. Reads config file if its found.
	 * 
	 * \param path - Path to the config file
	 */
	Config(const char* path) noexcept;

private:
	bool doesExist() noexcept;
	void loadConfig() noexcept;
	void generateDefault() noexcept;

private:
	const char* m_Path;
	CSimpleIniA m_ini;
};
