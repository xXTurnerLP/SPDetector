#pragma once

#include <unistd.h>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "../vendor/simpleini/SimpleIni.h"

typedef struct ConfigData
{
	const char* id;
	unsigned int total;
	const char* remote_server_endpoint;
	const char* name;
	const char* address;
	double gps_lat;
	double gps_lng;
	const char* type;
} ConfigData;

class Config
{
public:
	/**
	 * Generates default config if no config is found. Reads config file if its found.
	 * 
	 * \param path - Path to the config file
	 */
	Config(const char* path) noexcept;

public:
	const ConfigData& GetConfigData() const noexcept;

private:
	bool doesExist() const noexcept;
	void loadConfig() noexcept;
	void validateLoadedConfig() const noexcept;
	void populateBuffer() noexcept;
	void generateDefault() noexcept;

private:
	const char* m_Path;
	CSimpleIniA m_ini;
	ConfigData m_cfgData;
};
