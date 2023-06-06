#include "request_handler.h"

void RequestHandler::ProcessRequests(const json::Node& stat_requests) const {
    json::Array result;
    for (auto& request : stat_requests.AsArray()) {
        const auto& request_map = request.AsMap();
        const auto& type = request_map.at("type").AsString();
        Print printer{};
        if (type == "Bus")
        {
            result.push_back(printer.RendererPrintRoute(catalog_, request_map).AsMap());
        }
        
        if (type == "Stop")
        {
            result.push_back(printer.RendererPrintStop(catalog_, request_map).AsMap());
        }
        
        if (type == "Map") 
        { 
            result.push_back(renderer_.RendererPrintMap(catalog_, request_map).AsMap());
        }
    }

    json::Print(json::Document{ result }, std::cout);
}


svg::Document MpRenderer::RenderMap() const {
    return renderer_.RendererSVG(catalog_.GetSortedAllBuses());
}


    
