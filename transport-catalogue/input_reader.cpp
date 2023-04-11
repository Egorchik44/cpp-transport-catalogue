#include "input_reader.h"


namespace route {

    void FillCatalog(Catalog& catalog) {
        std::vector<std::string> bus_query;
        std::vector<std::string> stop_query;
        std::vector<std::string> distance_query;
        size_t requests_count;

        std::cin >> requests_count;
        for (size_t i = 0; i < requests_count; ++i) {
            std::string word, line;
            std::cin >> word;
            std::getline(std::cin, line);
            if (word == "Stop") {
                stop_query.push_back(line);
            }
            if (word == "Bus") {
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

        Bus FillRoute(std::string_view line) {
            std::vector<std::string> route_stops;
            std::string route_number = std::string(line.substr(1, line.find_first_of(':') - 1));
            line.remove_prefix(line.find_first_of(':') + 2);

            bool circular_route = (line.find('>') != std::string_view::npos);
            std::string stop_name;
            char pos = circular_route ? '>' : '-';
            while (line.find(pos) != std::string_view::npos) {
                stop_name = std::string(line.substr(0, line.find_first_of(pos) - 1));
                route_stops.push_back(stop_name);
                line.remove_prefix(line.find_first_of(pos) + 2);
            }
            stop_name = std::string(line.substr(0, line.npos - 1));
            route_stops.push_back(stop_name);
            circular_route = (pos == '>');

            Bus bus;
            bus.number = std::move(route_number);
            bus.stops = std::move(route_stops);
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
            stop.buses = {};

            return stop;
        }

        void FillStopDistances(std::string& line, Catalog& catalog) {
            if (!line.empty()) {
                std::string stop_from_name = FillStop(line).name;
                Stop* from_there = catalog.FindStop(stop_from_name);

                while (!line.empty()) {
                    int distanse = 0;
                    std::string stop_to_name;
                    distanse = std::stoi(line.substr(0, line.find_first_of("m to ")));
                    line.erase(0, line.find_first_of("m to ") + 5);
                    if (line.find("m to ") == line.npos) {
                        stop_to_name = line.substr(0, line.npos - 1);
                        Stop* there = catalog.FindStop(stop_to_name);
                        catalog.SetDistance(from_there, there, distanse);
                        if (!catalog.FindStop(there->name)->stop_distances.count(from_there->name)) {
                            catalog.SetDistance(there, from_there, distanse);
                        }
                        line.clear();
                    }
                    else {
                        stop_to_name = line.substr(0, line.find_first_of(','));
                        Stop* there = catalog.FindStop(stop_to_name);
                        catalog.SetDistance(from_there, there, distanse);
                        if (!catalog.FindStop(there->name)->stop_distances.count(from_there->name)) {
                            catalog.SetDistance(there, from_there, distanse);
                        }
                        line.erase(0, line.find_first_of(',') + 2);
                    }
                }
            }
        }

    }

}

