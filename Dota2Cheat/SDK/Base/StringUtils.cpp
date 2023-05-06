#include "StringUtils.h"

std::string encodeURI(const std::string& value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;
	for (auto i = value.begin(), n = value.end(); i != n; ++i) {
		auto c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded

		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}
