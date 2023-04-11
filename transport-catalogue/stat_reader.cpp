#include "stat_reader.h"

#include <iomanip>

namespace route {

    void ProcessRequests(Catalog& catalog) {
        size_t count_requests;
        std::cin >> count_requests;
        for (size_t i = 0; i < count_requests; ++i) {
            std::string word, line;
            std::cin >> word;
            std::getline(std::cin, line);
            if (word == "Stop") {
                detail::PrintStop(line, catalog);
            }
            if (word == "Bus") {
                detail::PrintRoute(line, catalog);
            }
        }
    }

    namespace detail {

        void PrintRoute(std::string& line, Catalog& catalog) {
            std::string route_number = line.substr(1, line.npos);
            if (catalog.FindRoute(route_number)) {
                std::cout << "Bus " << route_number << ": " << catalog.RouteInfo(route_number).stops_count << " stops on route, "
                    << catalog.RouteInfo(route_number).unique_stops_count << " unique stops, " << std::setprecision(6)
                    << catalog.RouteInfo(route_number).route_length << " route length, "
                    << catalog.RouteInfo(route_number).curvature << " curvature\n";
            }
            else {
                std::cout << "Bus " << route_number << ": not found\n";
            }
        }

        void PrintStop(std::string& line, Catalog& catalog) {
            std::string stop_name = line.substr(1, line.npos);
            if (catalog.FindStop(stop_name)) {
                std::cout << "Stop " << stop_name << ": ";
                std::set<std::string> buses = catalog.BusesOnStop(stop_name);
                if (!buses.empty()) {
                    std::cout << "buses ";
                    for (const auto& bus : buses) {
                        std::cout << bus << " ";
                    }
                    std::cout << "\n";
                }
                else {
                    std::cout << "no buses\n";
                }
            }
            else {
                std::cout << "Stop " << stop_name << ": not found\n";
            }
        }

    } 

} 