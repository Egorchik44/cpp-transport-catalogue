#include "input_reader.h"
#include "stat_reader.h"

using namespace route;

int main() {
    Catalog catalog;
    FillCatalog(catalog);
    ProcessRequests(catalog);
}