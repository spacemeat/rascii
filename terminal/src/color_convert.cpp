#include "renderer.hpp"

namespace rascii
{


void convert_color(color_b4 const & color_in, color_b4 & color_out)
{
	color_out = color_in;
}

void convert_color(color_b4 const & color_in, color_b8 & color_out)
{
	color_out = decltype(color_out)(color_in); // TODO: So wrong
}

void convert_color(color_b4 const & color_in, color_b24 & color_out)
{
	color_out = { color_in, color_in, color_in }; // TODO: This is very wrong
}

void convert_color(color_b8 const & color_in, color_b4 & color_out)
{
	color_out = decltype(color_out)(color_in); // TODO: So wrong
}

void convert_color(color_b8 const & color_in, color_b8 & color_out)
{
	color_out = color_in;
}

void convert_color(color_b8 const & color_in, color_b24 & color_out)
{
	color_out = { color_in, color_in, color_in }; // TODO: This is very wrong
}

void convert_color(color_b24 const & color_in, color_b4 & color_out)
{
	color_out = decltype(color_out)(color_in[0]);	// TODO: This is very wrong
}

void convert_color(color_b24 const & color_in, color_b8 & color_out)
{
	color_out = decltype(color_out)(color_in[0]);	// TODO: This is very wrong
}

void convert_color(color_b24 const & color_in, color_b24 & color_out)
{
	color_out = color_in;
}

} // using namespace rascii
