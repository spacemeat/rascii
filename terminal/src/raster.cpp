#include "renderer.hpp"


namespace rascii
{


void RasterBase::set_pixel(
	[[maybe_unused]] uint x,
	[[maybe_unused]] uint y,
	[[maybe_unused]] color_b24 color)
{
}

void RasterBase::set_cell_colors(
	[[maybe_unused]] int cell,
	[[maybe_unused]] color_b24 const * fg,
	[[maybe_unused]] color_b24 const * bg)
{
}

void RasterBase::set_cell(
	[[maybe_unused]] uint c,
	[[maybe_unused]] uint r,
	[[maybe_unused]] char32_t const * ch,
	[[maybe_unused]] color_b24 const * fg,
	[[maybe_unused]] color_b24 const * bg)
{
}

void RasterBase::set_cell(
	[[maybe_unused]] uint c,
	[[maybe_unused]] uint r,
	[[maybe_unused]] char8_t const * ch,
	[[maybe_unused]] int len,
	[[maybe_unused]] color_b24 const * fg,
    [[maybe_unused]] color_b24 const * bg)
{
}

std::string const & RasterBase::get_stream()
{
	return stream;
}

std::string_view RasterBase::get_stream2()
{
	return stream;
}

} // using namespace rascii
