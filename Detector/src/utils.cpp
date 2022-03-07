#include <curl/curl.h>
#include <string>
#include <cstring>

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void SendPost(const char* url, std::string message)
{
	CURL* curl = curl_easy_init();
	if (curl)
	{
		std::string postFields = ReplaceAll(ReplaceAll(message, "\"", "\\\""), "\n", "\\n");
		char* _postFields = new char[postFields.size() + 1];

		strcpy(_postFields, postFields.c_str());
		_postFields[postFields.size()] = 0;

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, _postFields);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

		CURLcode result = curl_easy_perform(curl);
		if (result != CURLE_OK)
		{
			printf("CRITICAL WARN: Latest data was not transmitted to the server, libcurl error: %d", result);
		}

		curl_easy_cleanup(curl);

		delete _postFields;
	}
}