#pragma once

#include "geo.h"
#include "transport_catalogue.h"

#include <iomanip>
#include <iostream>

namespace route {
	void FillCatalog(Catalog& catalog);
	namespace detail {
		Bus FillRoute(std::string_view line);
		Stop FillStop(std::string& line);
		void FillStopDistances(std::string& line, Catalog& catalogue);
	}
}

	 


