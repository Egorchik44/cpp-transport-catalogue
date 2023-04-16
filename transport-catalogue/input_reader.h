#pragma once

#include "transport_catalogue.h"

#include <iostream>
#include "geo.h"

namespace transport {

	void FillCatalog(Catalog& catalog);

	namespace detail {

		Bus FillRoute(std::string& line);
		Stop FillStop(std::string& line);
		void FillStopDistances(std::string& line, Catalog& catalogue);

	} 

} 
