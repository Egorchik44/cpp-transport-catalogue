#include "json_reader.h"
#include "request_handler.h"

int main() {
    transport::Catalog catalog;
    JsonReader json_doc(std::cin);
    
    json_doc.FillCatalogue(catalog);
    
    const auto& stat_requests = json_doc.GetStatRequests();
    const auto& render_settings = json_doc.GetRenderSettings().AsDict();
    const auto& renderer = json_doc.FillRenderSettings(render_settings);
    
    RequestHandler rh(catalog, renderer);
    rh.ProcessRequests(stat_requests);
}