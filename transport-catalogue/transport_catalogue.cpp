#include "transport_catalogue.h"

namespace transport {

    void Catalog::AddRoute(std::string_view bus_number, const std::vector<const Stop*> stops, bool circular_route) {
        all_buses_.push_back({ std::string(bus_number), stops, circular_route });
        busname_to_bus_[all_buses_.back().number] = &all_buses_.back();
        for (const auto& route_stop : stops) {
            for (auto& stop_ : all_stops_) {
                if (stop_.name == route_stop->name) stop_.buses_by_stop.insert(std::string(bus_number));
            }
        }
    }

    void Catalog::AddStop(std::string_view stop_name, const geo::Coordinates coordinates) {
        all_stops_.push_back({ std::string(stop_name), coordinates, {} });
        stopname_to_stop_[all_stops_.back().name] = &all_stops_.back();
    }

    const Bus* Catalog::FindRoute(std::string_view bus_number) const {
        return busname_to_bus_.count(bus_number) ? busname_to_bus_.at(bus_number) : nullptr;
    }

    const Stop* Catalog::FindStop(std::string_view stop_name) const {
        return stopname_to_stop_.count(stop_name) ? stopname_to_stop_.at(stop_name) : nullptr;
    }

    size_t Catalog::UniqueStopsCount(std::string_view bus_number) const {
        const auto& stops = busname_to_bus_.at(bus_number)->stops;
        std::unordered_set<std::string_view> unique_stops;
        for (const auto& stop : stops) {
            unique_stops.insert(stop->name);
        }
        return unique_stops.size();
    }

    void Catalog::SetDistance(const Stop* from_there, const Stop* there, int distance) {
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

    const std::map<std::string_view, const Bus*> Catalog::GetSortedAllBuses() const {
        return std::map<std::string_view, const Bus*>(busname_to_bus_.begin(), busname_to_bus_.end());
    }

}