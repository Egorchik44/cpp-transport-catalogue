#pragma once

#include "geo.h"

#include <set>
#include <deque>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <unordered_map>


namespace route {

    struct Bus {
        std::string number;
        std::vector<std::string> stops;
        bool circular_route;
    };

    struct Stop {
        std::string name;
        Coordinates coordinates;
        std::set<std::string> buses;
        std::unordered_map<std::string, int> stop_distances;
    };

    struct Route {
        size_t stops_count;
        size_t unique_stops_count;
        double route_length;
        double curvature;
    };

    class Catalog {
    public:
        void AddBus(Bus& bus);
        void AddStop(Stop& stop);
        const Bus* FindRoute(const std::string& route_number) const;
        Stop* FindStop(const std::string& stop_name) const;
        const Route RouteInfo(const std::string& route_number) const;
        size_t UniqueStopsCount(const std::string& route_number) const;
        const std::set<std::string> BusesOnStop(const std::string& stop_name) const;
        int GetDistance(const Stop* from_there, const Stop* there) const;
        void SetDistance(Stop* from_there, Stop* there, int distance);

    private:
        std::deque<Bus> all_buses_;
        std::deque<Stop> all_stops_;
        std::unordered_map<std::string_view, const Bus*> busname_to_bus_;
        std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
    };

} 
