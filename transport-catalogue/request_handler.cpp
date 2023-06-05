#include "request_handler.h"

void RequestHandler::ProcessRequests(const json::Node& stat_requests) const {
    json::Array result;
    for (auto& request : stat_requests.AsArray()) {
        const auto& request_map = request.AsMap();
        const auto& type = request_map.at("type").AsString();
        if (type == "Bus")
        {
            RoutePrinter rp{ catalog_ };
            result.push_back(rp.PrintRoute(request_map).AsMap());
        }
        
        if (type == "Stop")
        {
            StopPrinter sp{ catalog_ };
            result.push_back(sp.PrintStop(request_map).AsMap());
        }
        
        if (type == "Map") 
        { 
            MapPrinter mp{ catalog_, renderer_};
            result.push_back(mp.PrintMap(request_map).AsMap()); 
        }
    }

    json::Print(json::Document{ result }, std::cout);
}

const json::Node RoutePrinter::PrintRoute(const json::Dict& request_map) const {
    json::Dict result;
    const std::string& route_number = request_map.at("name").AsString();
    result["request_id"] = request_map.at("id").AsInt();
    if (!catalog_.FindRoute(route_number)) {
        result["error_message"] = json::Node{ static_cast<std::string>("not found") };
    }
    else {
        BusStatistics bs{catalog_};
        result["curvature"] = bs.GetBusStat(route_number)->curvature;
        result["route_length"] = bs.GetBusStat(route_number)->route_length;
        result["stop_count"] = static_cast<int>(bs.GetBusStat(route_number)->stops_count);
        result["unique_stop_count"] = static_cast<int>(bs.GetBusStat(route_number)->unique_stops_count);
    }

    return json::Node{ result };
}

const json::Node StopPrinter::PrintStop(const json::Dict& request_map) const {
    json::Dict result;
    const std::string& stop_name = request_map.at("name").AsString();
    result["request_id"] = request_map.at("id").AsInt();
    
    if (!catalog_.FindStop(stop_name)) {
        result["error_message"] = json::Node{ static_cast<std::string>("not found") };
    }
    else {
        BusStatistics bs{catalog_};
        json::Array buses;
        for (auto& bus : bs.GetBusesByStop(stop_name)) {
            buses.push_back(bus);
        }
        result["buses"] = buses;
    }

    return json::Node{ result };
}

const json::Node MapPrinter::PrintMap(const json::Dict& request_map) const {
    json::Dict result;
    result["request_id"] = request_map.at("id").AsInt();
    std::ostringstream strm;
    
    MpRenderer mp{catalog_,renderer_};
    svg::Document map = mp.RenderMap();
    map.Render(strm);
    result["map"] = strm.str();

    return json::Node{ result };
}

std::optional<transport::Route> BusStatistics::GetBusStat(const std::string_view bus_number) const {
    transport::Route bus_stat{};
    const transport::Bus* bus = catalog_.FindRoute(bus_number);

    if (!bus) throw std::invalid_argument("Can't find bus");
    if (bus->circular_route) bus_stat.stops_count = bus->stops.size();
    else bus_stat.stops_count = bus->stops.size() * 2 - 1;

    int route_length = 0;
    double geographic_length = 0.0;

    for (size_t i = 0; i < bus->stops.size() - 1; ++i) {
        auto from_there = bus->stops[i];
        auto there = bus->stops[i + 1];
        if (bus->circular_route) {
            route_length += catalog_.GetDistance(from_there, there);
            geographic_length += geo::ComputeDistance(from_there->coordinates,
                there->coordinates);
        }
        else {
            route_length += catalog_.GetDistance(from_there, there) + catalog_.GetDistance(there, from_there);
            geographic_length += geo::ComputeDistance(from_there->coordinates,
                there->coordinates) * 2;
        }
    }

    bus_stat.unique_stops_count = catalog_.UniqueStopsCount(bus_number);
    bus_stat.route_length = route_length;
    bus_stat.curvature = route_length / geographic_length;

    return bus_stat;
}

const std::set<std::string> BusStatistics::GetBusesByStop(std::string_view stop_name) const {
    return catalog_.FindStop(stop_name)->buses_by_stop;
}

svg::Document MpRenderer::RenderMap() const {
    return renderer_.RendererSVG(catalog_.GetSortedAllBuses());
}