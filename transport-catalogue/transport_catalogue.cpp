#include "transport_catalogue.h"

namespace route {

    void Catalog::AddBus(Bus& bus) {
        all_buses_.push_back(bus);
        for (const auto& route_stop : bus.stops) {
            for (auto& stop_ : all_stops_) {
                if (stop_.name == route_stop) stop_.buses.insert(bus.number);
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

        if (!bus) throw std::invalid_argument("Can't find bus");
        if (bus->circular_route) route_info.stops_count = bus->stops.size();
        else route_info.stops_count = bus->stops.size() * 2 - 1;

        int route_length = 0;
        double geographic_length = 0.0;

        for (auto iter = bus->stops.begin(); iter+1 != bus->stops.end(); ++iter) {
            auto from_there = stopname_to_stop_.find(*iter)->second;
            auto there = stopname_to_stop_.find((*(iter + 1)))->second;
            if (bus->circular_route) {
                route_length += GetDistance(from_there, there);
                geographic_length += ComputeDistance(stopname_to_stop_.find(*iter)->second->coordinates,
                    stopname_to_stop_.find(*(iter + 1))->second->coordinates);
            }
            else {
                route_length += GetDistance(from_there, there) + GetDistance(there, from_there);
                geographic_length += ComputeDistance(stopname_to_stop_.find(*iter)->second->coordinates,
                    stopname_to_stop_.find(*(iter + 1))->second->coordinates) * 2;
            }
        }
        if (bus->circular_route) {
            auto from_there = stopname_to_stop_.find(*(bus->stops.end() - 1))->second;
            auto to = stopname_to_stop_.find(*(bus->stops.begin()))->second;
            route_length += GetDistance(from_there, to);
            geographic_length += ComputeDistance(from_there->coordinates, to->coordinates);
        }

        route_info.unique_stops_count = UniqueStopsCount(route_number);
        route_info.route_length = route_length;
        route_info.curvature = route_length / geographic_length;

        return route_info;
    }

    size_t Catalog::UniqueStopsCount(const std::string& route_number) const {
        std::unordered_set<std::string> unique_stops;
        for (const auto& stop : busname_to_bus_.at(route_number)->stops) {
            unique_stops.insert(stop);
        }
        return unique_stops.size();
    }

    const std::set<std::string> Catalog::BusesOnStop(const std::string& stop_name) const {
        return stopname_to_stop_.at(stop_name)->buses;
    }

    int Catalog::GetDistance(const Stop* from_there, const Stop* there) const {
        if (from_there->stop_distances.count(there->name)) return from_there->stop_distances.at(there->name);
        else if (there->stop_distances.count(from_there->name)) return there->stop_distances.at(from_there->name);
        else return 0;
    }

    void Catalog::SetDistance(Stop* from, Stop* there, int distance) {
        from->stop_distances[there->name] = distance;
    }

} 