#pragma once

#include "json.h"
#include "json_reader.h"
#include "map_renderer.h"
#include "transport_catalogue.h"

#include <sstream>

using namespace renderer;
using namespace transport;

class RequestHandler {
public:
    explicit RequestHandler(const transport::Catalog& catalog, const renderer::MapRenderer& renderer)
        : catalog_(catalog)
        , renderer_(renderer)
    {
    }

    void ProcessRequests(const json::Node& stat_requests) const;

private:

    const Catalog& catalog_;
    const MapRenderer& renderer_;
    
};

// Разделение функциональности на отдельные классы

class RoutePrinter  {
public:
    explicit RoutePrinter(const Catalog& catalog) : catalog_(catalog) {}

    const json::Node PrintRoute(const json::Dict& request_map) const;

private:
    const Catalog& catalog_;
};

class StopPrinter  {
public:
    explicit StopPrinter(const Catalog& catalog) : catalog_(catalog) {}

    const json::Node PrintStop(const json::Dict& request_map) const;

private:
    const Catalog& catalog_;
};

class MapPrinter  {
public:
    explicit MapPrinter(const Catalog& catalog, const MapRenderer& renderer) : catalog_(catalog), renderer_(renderer) {}

    const json::Node PrintMap(const json::Dict& request_map) const;

private:
    const Catalog& catalog_;
    const MapRenderer& renderer_;
};

class BusStatistics {
public:
    explicit BusStatistics(const Catalog& catalog) : catalog_(catalog) {}

    std::optional<transport::Route> GetBusStat(const std::string_view bus_number) const;
    const std::set<std::string> GetBusesByStop(std::string_view stop_name) const;

private:
    const Catalog& catalog_;
};

class MpRenderer {
public:
    explicit MpRenderer(const Catalog& catalog, const MapRenderer& renderer) : catalog_(catalog), renderer_(renderer) {}

    svg::Document RenderMap() const;

private:
    const Catalog& catalog_;
    const MapRenderer& renderer_;
};