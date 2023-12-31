#include "point_io.hpp"
#include "render.hpp"
#include "utils.hpp"

#include "vendor/cxxopts.hpp"


int main(int argc, char **argv) {
    cxxopts::Options options("renderdem", "Render a point cloud to a raster DEM");
    options.add_options()
        ("i,input", "Input point cloud (.las, .las, .ply)", cxxopts::value<std::string>())
        ("t,tile-size", "Tile size", cxxopts::value<int>()->default_value("4096"))
        ("c,classification", "Only use points matching this classification", cxxopts::value<int>()->default_value("-1"))
        ("d,decimation", "Read every Nth point", cxxopts::value<int>()->default_value("1"))
        ("o,output-type", "One of: [max, idw]", cxxopts::value<std::string>()->default_value("max"))
        ("s,radiuses", "Comma separated list of radius values to generate and stack", cxxopts::value<std::string>()->default_value("0.56"))
        ("r,resolution", "Resolution of output GeoTIFF DEM", cxxopts::value<double>()->default_value("0.1"))
        ("x,max-tiles", "Maximum number of tiles to generate (as safety precaution for OOM issues)", cxxopts::value<int>()->default_value("0"))
        ("u,outdir", "Directory to store results", cxxopts::value<std::string>()->default_value("output"))
        
        ("f,force", "Overwrite existing results")
        ("h,help", "Print usage")
        ;
    options.parse_positional({ "input" });
    options.positional_help("[point cloud]");
    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (result.count("help") || !result.count("input")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    try {
        const auto inputFilename = result["input"].as<std::string>();
        const auto outDir = result["outdir"].as<std::string>();
        const auto outputType = result["output-type"].as<std::string>();
        const auto tileSize = result["tile-size"].as<int>();
        const auto classification = result["classification"].as<int>();
        const auto decimation = result["decimation"].as<int>();
        const auto radiuses = parseCSV(result["radiuses"].as<std::string>());
        const auto resolution = result["resolution"].as<double>();
        const bool force = result.count("force");
        const auto maxTiles = result["max-tiles"].as<int>();

        auto *pset = readPointSet(inputFilename, classification, decimation);
        render(pset, outDir, outputType, tileSize, radiuses, resolution, maxTiles, force);
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
