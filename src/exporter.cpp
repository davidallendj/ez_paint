
#include "exporter.hpp"
#include <png++/rgba_pixel.hpp>
#include <png++/image.hpp>
#include <libheif/heif.h>

namespace ez_paint::io
{
	void PngReader::writeToFile(const std::string &filename, const Canvas& canvas)
	{
		// Convert vertex array data from canvas to png::image<png::rgb_pixel>
		sf::Vector2u size = canvas.getSize();
		png::image<png::rgba_pixel> image(size.x, size.y);
		for(sf::Uint64 y = 0; y < size.y; ++y){
			for(sf::Uint64 x = 0; x < size.x; ++x){
				image.set_pixel(x, y, png::rgba_pixel(x, y, x + y));
			}
		}

		// filename += hasExtension(filename, ".png") ? "" : ".png"; // NOTE: This function does not exist yet
		image.write(filename);
	}

	ptr_type<Canvas> PngReader::readFromFile(const std::string &filename)
	{ 
		png::image<png::rgba_pixel> image(filename);
		sf::Vector2u size(image.get_width(), image.get_height());
		// Canvas *canvas = new Canvas(nullptr, size);
		std::unique_ptr<Canvas> canvas = std::make_unique<Canvas>(nullptr, size);

		// Convert *.png data back to Canvas data for editing
		for(sf::Uint64 y = 0; y < size.y; ++y){
			for(sf::Uint64 x = 0; x < size.x; ++x){
				png::rgba_pixel p = image.get_pixel(x, y);
				canvas->setPixel(sf::Vector2u(x, y), sf::Color(p.red, p.green, p.blue, p.alpha));
			}
		}
		return canvas;
	}

	void AvifReader::writeToFile(const std::string &filename, const Canvas &canvas)
	{
		// Write to a HEIF file!!
		// TODO: Should work with AV1 instead
		heif_context *context = heif_context_alloc();

		// Get the default encoder
		heif_encoder *encoder;
		heif_context_get_encoder_for_format(context, heif_compression_HEVC, &encoder);

		// Set the encoder parameters
		heif_encoder_set_lossy_quality(encoder, 50);

		// Encode the image
		heif_image *image; // TODO: code to fill in the image omitted in this example
		heif_context_encode_image(context, image, encoder, nullptr, nullptr);
		heif_encoder_release(encoder);
		heif_context_write_to_file(context, filename.data());

		// FIXME: Does the context need to be freed here?

	}

	ptr_type<Canvas> AvifReader::readFromFile(const std::string &filename)
	{
		heif_context * context = heif_context_alloc();
		heif_context_read_from_file(context, filename.data(), nullptr);

		// Get a handle to the primary image
		heif_image_handle *handle;
		heif_context_get_primary_image_handle(context, &handle);

		// Decode the image and onvert colorspace to RGB, saved as 24bit interleaved
		heif_image *image;
		heif_decode_image(handle, &image, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr);

		int stride;
		const uint8_t *data = heif_image_get_plane_readonly(image, heif_channel_interleaved, &stride);

		// TODO: Now build the Canvas using the image data!!!

		// FIXME: Does the context need to be freed here?
		sf::Vector2u size(0, 0);
		return make<Canvas>(nullptr, size);
	}

	void JsonReader::writeToFile(const std::string& filename, const Canvas& canvas){

	}

	ptr_type<Canvas> JsonReader::readFromFile(const std::string& filename){
		sf::Vector2u size(0, 0);
		return make<Canvas>(nullptr, size);
	}

	void CsvReader::writeToFile(const std::string& filename, const Canvas& canvas){

	}

	ptr_type<Canvas> CsvReader::readFromFile(const std::string& filename){
		sf::Vector2u size(0, 0);
		return make<Canvas>(nullptr, size);
	}


}