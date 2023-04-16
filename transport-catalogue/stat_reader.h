#pragma once

#include "transport_catalogue.h"

namespace transport {

	void ProcessRequests(Catalog& catalog);

	namespace detail {

		void PrintRoute(std::string& line, Catalog& catalog, std::ostream& out);
		void PrintStop(std::string& line, Catalog& catalog, std::ostream& out);

	}

} 