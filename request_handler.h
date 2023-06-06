#pragma once

#include "json.h"
#include "json_reader.h"
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


class MpRenderer {
public:
    explicit MpRenderer(const Catalog& catalog, const MapRenderer& renderer) : catalog_(catalog), renderer_(renderer) {}

    svg::Document RenderMap() const;

private:
    const Catalog& catalog_;
    const MapRenderer& renderer_;
};