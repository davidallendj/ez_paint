#pragma once

#include "canvas.hpp"
#include <SFML/System/String.hpp>

namespace ez_paint::io
{
	template <typename Contents = Canvas, typename String = std::string>
	class IWriter
	{
	public:
		virtual void writeToFile(const String& filename, const Contents& canvas) = 0;
	};

	template <typename Contents, typename String = std::string>
	class IReader
	{
	public:
		virtual Contents readFromFile(const String& filename) = 0;
	};

	using Writer = IWriter<Canvas, std::string>;
	using Reader = IReader<ptr_type<Canvas>, std::string>;
	

	class PngReader : public Writer, Reader
	{
	public:
		void writeToFile(const std::string& filename, const Canvas& canvas);
		ptr_type<Canvas> readFromFile(const std::string& filename);
	};

	class AvifReader : public Writer, Reader
	{
	// See reference: https://github.com/strukturag/libheif
	public:
		void writeToFile(const std::string& filename, const Canvas& canvas);
		ptr_type<Canvas> readFromFile(const std::string& filename);
	};

	class JsonReader : public Writer, Reader
	{
	public:
		void writeToFile(const std::string& filename, const Canvas& canvas);
		ptr_type<Canvas> readFromFile(const std::string& filename);
	};

	class CsvReader : public Writer, Reader
	{
		void writeToFile(const std::string& filename, const Canvas& canvas);
		ptr_type<Canvas> readFromFile(const std::string& filename);
	};

}
