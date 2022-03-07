#include "config.hpp"

Config::Config(const char* path) noexcept : m_Path(path), m_ini(true), m_cfgData()
{
	if (doesExist())
	{
		loadConfig();
		validateLoadedConfig();
		populateBuffer();
	}
	else
	{
		generateDefault();
	}
}

const ConfigData& Config::GetConfigData() const noexcept
{
	return m_cfgData;
}

bool Config::doesExist() const noexcept
{
	return access(m_Path, F_OK) == 0;
}

void Config::loadConfig() noexcept
{
	FILE* cfg_file = fopen(m_Path, "rb");
	if (!cfg_file)
	{
		puts("Fatal Error: Config file not found, open program again to regenerate");
		exit(-1);
	}

	SI_Error error_code = m_ini.LoadFile(m_Path);
	if (error_code < 0)
	{
		printf("Fatal Error (Internal): Cannot parse config file, Code: %d", error_code);
		exit(-1);
	}

	fclose(cfg_file);
}

void Config::validateLoadedConfig() const noexcept
{
	const char* id = m_ini.GetValue("Required", "id");
	const long total = m_ini.GetLongValue("Required", "total", 0xFFFFFFFF);
	const char* remote_endpoint = m_ini.GetValue("Required", "remote-server-endpoint");

	if (!id || !remote_endpoint || total == 0xFFFFFFFF)
	{
		puts("Config error: Corrupted config, one or more of required fields: id, total or remote-server-endpoint were not found. Recommended to delete config file and regenerate it.");
		exit(-1);
	}

	if (strcmp(id, "00000000-0000-0000-0000-000000000000") == 0 ||
		strcmp(remote_endpoint, "https://address-here.domain/path/to/endpoint") == 0 ||
		total == 0)
	{
		puts("Config error: Default id, total or remote-server-endpoint value found! Change them to a valid entry.");
		exit(-1);
	}
}

void Config::populateBuffer() noexcept
{
	const char* id = m_ini.GetValue("Required", "id");
	const long total = m_ini.GetLongValue("Required", "total");
	const char* remote_endpoint = m_ini.GetValue("Required", "remote-server-endpoint");
	const char* name = m_ini.GetValue("Optional", "name");
	const char* address = m_ini.GetValue("Optional", "address");
	double gps_lng = m_ini.GetDoubleValue("Optional", "GPS Lng");
	double gps_lat = m_ini.GetDoubleValue("Optional", "GPS Lat");
	const char* type = m_ini.GetValue("Optional", "type");

	m_cfgData.id = id;
	m_cfgData.total = total;
	m_cfgData.remote_server_endpoint = remote_endpoint;
	m_cfgData.name = name;
	m_cfgData.address = address;
	m_cfgData.gps_lat = gps_lat;
	m_cfgData.gps_lng = gps_lng;
	m_cfgData.type = type;
}

void Config::generateDefault() noexcept
{
	SI_Error error_code;

	std::string default_data =
		"[Required]\n"
		"id = 00000000-0000-0000-0000-000000000000\n"
		"total = 0\n"
		"remote-server-endpoint = https://address-here.domain/path/to/endpoint\n"
		"\n"
		"[Optional]\n"
		"type = free\n"
		"name =\n"
		"address =\n"
		"GPS Lng =\n"
		"GPS Lat =";

	error_code = m_ini.LoadData(default_data);
	if (error_code < 0)
	{
		printf("Fatal Error (Internal): Cannot parse defualt config data, Code: %d", error_code);
		exit(-1);
	}

	FILE* cfg_file = fopen(m_Path, "wb");
	if (!cfg_file)
	{
		puts("Fatal Error: Cannot create default config file");
		exit(-1);
	}

	error_code = m_ini.SaveFile(cfg_file);
	if (error_code < 0)
	{
		printf("Fatal Error (Internal): Cannot parse defualt config data, Code: %d", error_code);
		exit(-1);
	}

	fclose(cfg_file);
	printf("Generated new default config file at \"%s\". Edit it and run the program again.\nPress enter to exit..", m_Path);
	getchar();
	exit(0);
}

