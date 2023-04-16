#include "transport_catalogue.h"

namespace transport {


    void Catalog::AddBus(Bus& bus) {
        all_buses_.push_back(bus);
        for (const auto& route_stop : bus.stops) {
            for (auto& stop_ : all_stops_) {
                if (stop_.name == route_stop) {
                    buses_for_stop_[stop_.name].push_back(bus.number);
                }
            }
        }
        busname_to_bus_[all_buses_.back().number] = &all_buses_.back();

        
    }

    void Catalog::AddStop(Stop& stop) {
        all_stops_.emplace_back(std::move(stop));
        stopname_to_stop_.emplace(all_stops_.back().name, &all_stops_.back());
    }
    const Bus* Catalog::FindRoute(const std::string& route_number) const {
        return busname_to_bus_.count(route_number) ? busname_to_bus_.at(route_number) : nullptr;
    }

    Stop* Catalog::FindStop(const std::string& stop_name) const {
        return stopname_to_stop_.count(stop_name) ? stopname_to_stop_.at(stop_name) : nullptr;
    }

     const Route Catalog::RouteInfo(const std::string& route_number) const {
        Route route_info{};
        const Bus* bus = FindRoute(route_number);

        if (!bus) throw std::invalid_argument("bus not found");
        if (bus->circular_route) route_info.stops_count = bus->stops.size();
        else route_info.stops_count = bus->stops.size() * 2 - 1;

        int route_length = 0;
        double geographic_length = 0.0;

        for (auto iter = bus->stops.begin(); iter + 1 != bus->stops.end(); ++iter) {
            auto from = stopname_to_stop_.find(*iter)->second;
            auto to = stopname_to_stop_.find((*(iter + 1)))->second;
            if (bus->circular_route) {
                route_length += GetDistance(from, to);
                geographic_length += ComputeDistance(stopname_to_stop_.find(*iter)->second->coordinates,
                    stopname_to_stop_.find(*(iter + 1))->second->coordinates);
            }
            else {
                route_length += GetDistance(from, to) + GetDistance(to, from);
                geographic_length += ComputeDistance(stopname_to_stop_.find(*iter)->second->coordinates,
                    stopname_to_stop_.find(*(iter + 1))->second->coordinates) * 2;
            }
        }
        if (bus->circular_route) {
            auto from = stopname_to_stop_.find(*(bus->stops.end() - 1))->second;
            auto to = stopname_to_stop_.find(*(bus->stops.begin()))->second;
            route_length += GetDistance(from, to);
            geographic_length += ComputeDistance(from->coordinates, to->coordinates);
        }

        route_info.unique_stops_count = UniqueStopsCount(route_number);
        route_info.route_length = route_length;
        route_info.curvature = route_length / geographic_length;

        return route_info;
    }

    size_t Catalog::UniqueStopsCount(const std::string& route_number) const {
        std::unordered_set<std::string> unique_stops;
        const Bus* bus = FindRoute(route_number);
        if (!bus) {
            return 0;
        }
        for (const auto& stop : (*bus).stops) {
            unique_stops.insert(stop);
        }
        return unique_stops.size();
    }

    std::set<std::string> Catalog::GetBusesOnStop(const std::string& stop_name) const {
        std::set<std::string> result;
        auto buses_it = buses_for_stop_.find(stop_name);
        if (buses_it == buses_for_stop_.end()) {
            return result; 
        }

        for (const auto& bus_number : buses_it->second) {
            result.insert(bus_number);
        }

        return result;
    }

    void Catalog::SetDistance(Stop* from_there, Stop* there, int distance) {
        distances_between_stops_[{from_there, there}] = distance;
    }

    

    int Catalog::GetDistance(const Stop* from_there, const Stop* there) const {
        if (distances_between_stops_.count({ from_there,there }) != 0) {
            return distances_between_stops_.at({ from_there, there });
        } 
        else if (distances_between_stops_.count({ there,from_there }) != 0) {
            return distances_between_stops_.at({ there, from_there });
        }
        else {
            return 0;
        } 
    }

    bool Catalog::HasDistanceBetweenStops(const Stop* from_there, const Stop* there) {
        if (from_there == nullptr || there == nullptr) {
            return false;
        }

        if (busname_to_bus_.count(from_there->name) == 0 || busname_to_bus_.count(there->name) == 0) {
            return false; 
        }
        
        if (distances_between_stops_.count({ from_there, there }) == 0) {
            return false;
        }

        const Stop* to = FindStop(there->name);
        const Stop* from = FindStop(from_there->name);

        if (to == nullptr || from == nullptr) {
            return false;
        }

        return true;
    }

    
} 

