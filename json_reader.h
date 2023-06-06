#pragma once

#include "json.h"
#include "map_renderer.h"
#include "request_handler.h"
#include "transport_catalogue.h"

#include <iostream>

class JsonReader {
public:
    JsonReader(std::istream& input)
        : input_(json::Load(input))
    {}


    const json::Node& GetStatRequests() const;
    const json::Node& GetRenderSettings() const;

    

    void FillCatalogue(transport::Catalog& catalog);
    renderer::MapRenderer FillRenderSettings(const json::Dict& request_map) const;

private:
    json::Document input_;
    json::Node dummy_ = nullptr;

    const json::Node& GetBaseRequests() const;

    std::tuple<std::string_view, geo::Coordinates, std::map<std::string_view, int>> FillStop(const json::Dict& request_map) const;
    void FillStopDistances(transport::Catalog& catalog) const;
    std::tuple<std::string_view, std::vector<const transport::Stop*>, bool> FillRoute(const json::Dict& request_map, transport::Catalog& catalog) const;
};

class Print {
public:

    
 const json::Node RendererPrintRoute(const transport::Catalog& catalog, const json::Dict& request_map) const;
 const json::Node RendererPrintStop(const transport::Catalog& catalog, const json::Dict& request_map) const;
 
 
};