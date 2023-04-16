#include "input_reader.h"
#include "stat_reader.h"

using namespace transport;

int main() {
    Catalog catalog;
    FillCatalog(catalog);
    ProcessRequests(catalog);
}