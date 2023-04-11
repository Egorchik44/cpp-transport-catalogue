#pragma once

#include "transport_catalogue.h"

namespace route {
	void ProcessRequests(Catalog& catalog);
	namespace detail {
		void PrintRoute(std::string& line, Catalog& catalog);
		void PrintStop(std::string& line, Catalog& catalog);
	}
}

	

