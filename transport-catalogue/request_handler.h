#pragma once

#include "json.h"
#include "json_reader.h"
#include "map_renderer.h"
#include "transport_catalogue.h"

#include <sstream>

class RequestHandler {
public:
    explicit RequestHandler(const transport::Catalog& catalog, const renderer::MapRenderer& renderer)
        : catalog_(catalog)
        , renderer_(renderer)
    {
    }

    void ProcessRequests(const json::Node& stat_requests) const;

private:
    const transport::Catalog& catalog_;
    const renderer::MapRenderer& renderer_;

    std::optional<transport::Route> GetBusStat(const std::string_view bus_number) const;
    const std::set<std::string> GetBusesByStop(std::string_view stop_name) const;

    const json::Node PrintRoute(const json::Dict& request_map) const;
    const json::Node PrintStop(const json::Dict& request_map) const;
    const json::Node PrintMap(const json::Dict& request_map) const;

    svg::Document RenderMap() const;
};