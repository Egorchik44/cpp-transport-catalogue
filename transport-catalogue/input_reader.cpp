#include "input_reader.h"


namespace transport {

    void FillCatalog(Catalog& catalog) {
        std::vector<std::string> bus_query;
        std::vector<std::string> stop_query;
        std::vector<std::string> distance_query;
        size_t requests_count;

        std::cin >> requests_count;
        for (size_t i = 0; i < requests_count; ++i) {
            std::string keyword, line;
            std::cin >> keyword;
            std::getline(std::cin, line);
            if (keyword == "Stop") {
                stop_query.push_back(line);
            }
            if (keyword == "Bus") {
                bus_query.push_back(line);
            }
        }
        distance_query = stop_query;
        for (auto& stop_ : stop_query) {
            Stop stop = detail::FillStop(stop_);
            catalog.AddStop(stop);
        }
        for (auto& stop_ : distance_query) {
            detail::FillStopDistances(stop_, catalog);
        }
        for (auto& bus_ : bus_query) {
            Bus bus = detail::FillRoute(bus_);
            catalog.AddBus(bus);
            bus = {};
        }
    }

    namespace detail {

        Bus FillRoute(std::string& line) {
            Bus bus;
            std::vector<std::string> route_stops;
            std::string route_number = line.substr(1, line.find_first_of(':') - 1);
            line.erase(0, line.find_first_of(':') + 2);
            bool circular_route = false;
            std::string stop_name;
            auto pos = line.find('>') != line.npos ? '>' : '-';
            while (line.find(pos) != line.npos) {
                stop_name = line.substr(0, line.find_first_of(pos) - 1);
                route_stops.push_back(stop_name);
                line.erase(0, line.find_first_of(pos) + 2);
            }
            stop_name = line.substr(0, line.npos - 1);
            route_stops.push_back(stop_name);
            if (pos == '>') circular_route = true;

            bus.number = route_number;
            bus.stops = route_stops;
            bus.circular_route = circular_route;

            return bus;
        }

        Stop FillStop(std::string& line) {
            Stop stop;
            std::string stop_name = line.substr(1, line.find_first_of(':') - line.find_first_of(' ') - 1);
            double lat = std::stod(line.substr(line.find_first_of(':') + 2, line.find_first_of(',') - 1));
            double lng;
            line.erase(0, line.find_first_of(',') + 2);
            if (line.find_last_of(',') == line.npos) {
                lng = std::stod(line.substr(0, line.npos - 1));
                line.clear();
            }
            else {
                lng = std::stod(line.substr(0, line.find_first_of(',')));
                line.erase(0, line.find_first_of(',') + 2);
            }
            Coordinates stop_coordinates = { lat, lng };

            stop.name = stop_name;
            stop.coordinates = stop_coordinates;

            return stop;
        }

        void FillStopDistances(std::string& line, Catalog& catalog) {
            if (!line.empty()) {
                std::string stop_from_name = FillStop(line).name;
                Stop* from = catalog.FindStop(stop_from_name);

                while (!line.empty()) {
                    int distanse = 0;
                    std::string stop_to_name;
                    distanse = std::stoi(line.substr(0, line.find_first_of("m to ")));
                    line.erase(0, line.find_first_of("m to ") + 5);
                    if (line.find("m to ") == line.npos) {
                        stop_to_name = line.substr(0, line.npos - 1);
                        Stop* to = catalog.FindStop(stop_to_name);
                        catalog.SetDistance(from, to, distanse);
                        if (catalog.HasDistanceBetweenStops(from, to)) {
                            catalog.SetDistance(to, from, distanse);
                        }
                        line.clear();
                    }
                    else {
                        stop_to_name = line.substr(0, line.find_first_of(','));
                        Stop* to = catalog.FindStop(stop_to_name);
                        catalog.SetDistance(from, to, distanse);
                        if (catalog.HasDistanceBetweenStops(from, to)) {
                            catalog.SetDistance(to, from, distanse);
                        }
                        line.erase(0, line.find_first_of(',') + 2);
                    }
                }
            }
        }

    } 

} 

