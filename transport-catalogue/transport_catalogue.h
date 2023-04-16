#pragma once

#include "geo.h"

#include <map>
#include <set>
#include <deque>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <string_view>
#include <unordered_set>
#include <unordered_map>

namespace transport {

    struct Bus {
        std::string number;
        std::vector<std::string> stops;
        bool circular_route;
    };

    struct Stop {
        std::string name;
        Coordinates coordinates;
        
    };

    struct Route {
        size_t stops_count;
        size_t unique_stops_count;
        double route_length;
        double curvature;
    };

    

    struct HashStopPtr {
        size_t operator()(const std::pair<const Stop*, const Stop*>& stops) const {
            static const std::hash<const Stop*> hasher{};
            return hasher(stops.first) ^ hasher(stops.second);
        }
    };

    class Catalog {
    public:
        void AddBus(Bus& bus);
        void AddStop(Stop& stop);
        const Bus* FindRoute(const std::string& route_number) const;
        Stop* FindStop(const std::string& stop_name) const;
        const Route RouteInfo(const std::string& route_number) const;
        std::set<std::string> GetBusesOnStop(const std::string& stop_name) const;
        void SetDistance(Stop* from, Stop* to, int distance) ;
        int GetDistance(const Stop* from, const Stop* to) const;
        bool HasDistanceBetweenStops(const Stop* from, const Stop* to);

    private:
        std::deque<Bus> all_buses_;
        std::deque<Stop> all_stops_;
        std::unordered_map<std::string_view, const Bus*> busname_to_bus_;
        std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
        std::unordered_map<std::string, std::vector<std::string>> buses_for_stop_;
        std::unordered_map< std::pair<const Stop*, const Stop*>, int, HashStopPtr> distances_between_stops_;

        size_t UniqueStopsCount(const std::string& route_number) const;
    };

} 
