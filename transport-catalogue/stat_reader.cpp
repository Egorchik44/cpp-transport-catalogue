#include "stat_reader.h"

#include <iomanip>

namespace transport {

    void ProcessRequests(Catalog& catalog) {
        size_t requests_count;
        std::cin >> requests_count;
        for (size_t i = 0; i < requests_count; ++i) {
            std::string keyword, line;
            std::cin >> keyword;
            std::getline(std::cin, line);
            if (keyword == "Bus") {
                detail::PrintRoute(line, catalog, std::cout);
            }
            if (keyword == "Stop") {
                detail::PrintStop(line, catalog, std::cout);
            }
        }
    }

    namespace detail {

        void PrintRoute(std::string& line, Catalog& catalog, std::ostream& out) {
            std::string route_number = line.substr(1, line.npos);
            if (catalog.FindRoute(route_number)) {
                out << "Bus " << route_number << ": " << catalog.RouteInfo(route_number).stops_count << " stops on route, "
                    << catalog.RouteInfo(route_number).unique_stops_count << " unique stops, " << std::setprecision(6)
                    << catalog.RouteInfo(route_number).route_length << " route length, "
                    << catalog.RouteInfo(route_number).curvature << " curvature\n";
            }
            else {
                out << "Bus " << route_number << ": not found\n";
            }
        }

        void PrintStop(std::string& line, Catalog& catalog, std::ostream& out) {
            std::string stop_name = line.substr(1, line.npos);
            if (catalog.FindStop(stop_name)) {
                out << "Stop " << stop_name << ": ";
                std::set<std::string> buses = catalog.GetBusesOnStop(stop_name);
                if (!buses.empty()) {
                    std::cout << "buses ";
                    for (const auto& bus : buses) {
                        out << bus << " ";
                    }
                    out << "\n";
                }
                else {
                    out << "no buses\n";
                }
            }
            else {
                out << "Stop " << stop_name << ": not found\n";
            }
        }

    } 

} 