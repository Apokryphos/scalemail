#include <algorithm>
#include <string>

namespace ScaleMail
{
//  ============================================================================
std::string toLowercase(std::string value) {
	std::transform(value.begin(), value.end(), value.begin(), ::tolower);
	return value;
}
}