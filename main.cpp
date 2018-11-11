#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <limits>

#include <libraw/libraw.h>


#define CHECK_CALL(ret, message) do { \
    if(ret != 0) { \
        std::stringstream s(message); \
        s << message << " Code " << ret <<  " (" << __FILE__ << ":" << __LINE__ << ")"; \
        throw(std::runtime_error{s.str()}); \
    } \
} while(0)


struct ProcessedImageDeleter {
  void operator()(libraw_processed_image_t* im) { LibRaw::dcraw_clear_mem(im); }
};

std::ostream& operator<<(std::ostream& os, const libraw_data_t& data) {
        os << "Camera: " << data.idata.model << std::endl;
        os << "Make: " << data.idata.make << std::endl;
        os << "Raw Height: " << data.sizes.raw_height << std::endl;
        os << "Raw Width: " << data.sizes.raw_width << std::endl;

        if(data.image) {
            os << "Image Height: " << data.sizes.height << std::endl;
            os << "Image Width: " << data.sizes.iwidth << std::endl;
        }

        return os;
}

std::ostream& operator<<(std::ostream& os, const libraw_processed_image_t& data) {
        os << "Height: " << data.height << std::endl;
        os << "Width: " << data.width << std::endl;
        os << "Colors: " << data.colors << std::endl;
        os << "bits: " << data.bits << std::endl;
        return os;
}

void extractImage(const std::string file_name, LibRaw* processor) {
    std::cout << "Opening file " << file_name << std::endl;
    CHECK_CALL(processor->open_file(file_name.c_str()), "Error opening file"); 

    std::cout << "Extract data ..." << std::endl;  
    CHECK_CALL(processor->unpack(), "Error during unpacking"); 
    
    std::cout << "Conversion to Image ..." << std::endl;  
    CHECK_CALL(processor->dcraw_process(), "Error during Conversion");
}

void assert_image_size_eq(std::string candidate_name, const libraw_image_sizes_t& ref, const libraw_image_sizes_t& candidate) {
    if(ref.iwidth != candidate.iwidth || ref.iheight != candidate.iheight) {
        std::stringstream s("Invalid size for image ");
        s << candidate_name << ", expected " \
            << "(" << ref.iwidth << ", " << ref.iheight << ")" \
            << ", got " \
            << "(" << candidate.iwidth << ", " << candidate.iheight << ")";
        throw(std::runtime_error{s.str()});
    }
}

ushort clamp(ushort accumulator, ushort extra) {
    uint result = static_cast<uint>(accumulator) + static_cast<uint>(extra);
    if(result > static_cast<uint>(std::numeric_limits<ushort>::max()))
        return std::numeric_limits<ushort>::max();
    else
        return (ushort)result;
}

int main(int argc, char const *argv[])
{
    if(argc < 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << argv[0] << " INPUT_RAW1 [INPUT_RAWS] OUTPUT_FILE" << std::endl;;
        exit(1);
    }

    try {
        LibRaw accumulator;
        extractImage(argv[1], &accumulator);

        std::string output = argv[argc - 1];
        std::vector<std::string> input_files_paths;
        for(int i = 2; i < argc - 1; i++) {
            LibRaw processor{};
            extractImage(argv[i], &processor);
            assert_image_size_eq(argv[i], accumulator.imgdata.sizes, processor.imgdata.sizes);
            for(int color = 0; color < 4; color++) {
                for(int i=0; i<accumulator.imgdata.sizes.iheight * accumulator.imgdata.sizes.iwidth; i++) {
                    accumulator.imgdata.image[i][color] = clamp(accumulator.imgdata.image[i][color], processor.imgdata.image[i][color]);
                }
            }
        }
        std::cout << "Writing the output image to " << output << std::endl;
        accumulator.imgdata.params.output_tiff = 1;
        accumulator.imgdata.params.output_bps = 16;
        accumulator.imgdata.params.no_auto_bright = 1;
        accumulator.dcraw_ppm_tiff_writer(output.c_str());
    } catch(std::runtime_error& e) {
        std::cerr << "Runtime Error:" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}