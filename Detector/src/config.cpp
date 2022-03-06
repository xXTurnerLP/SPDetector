#include "config.hpp"

Config::Config(const char* path) noexcept : m_Path(path), m_ini(true)
{
	if (doesExist())
	{
		loadConfig();
	}
	else
	{
		generateDefault();
	}
}

bool Config::doesExist() noexcept
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

	fclose(cfg_file);
}

void Config::generateDefault() noexcept
{
	SI_Error error_code;

	std::string default_data =
		"[Required]\n"
		"id = 00000000-0000-0000-0000-000000000000\n"
		"total = 0\n"
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

