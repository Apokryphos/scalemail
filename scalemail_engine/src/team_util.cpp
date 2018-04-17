#include "team.hpp"
#include <string>

namespace ScaleMail
{
std::string teamToString(const Team team) {
	switch (team) {
		case Team::PLAYER:
			return "PLAYER";
		case Team::VILLAIN:
			return "VILLAIN";
		default:
			return "?";
	}
}
}
