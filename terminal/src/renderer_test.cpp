#include <chrono>
#include <iostream>
#include <thread>

#include "renderer.hpp"

using namespace std::literals;
using namespace std::chrono_literals;
using namespace rascii;


int main()
{
	auto depth = Renderer::query_best_color_depth();
	auto extents = Renderer::query_terminal_extents();

	std::cout << "depth: " << (int) depth << "; width: " << std::get<0>(extents) << "; height: " 
	          << std::get<1>(extents) << "\n";

	Renderer renderer({std::get<0>(extents), std::get<1>(extents)}, depth);

	std::cout << "Renderer made.\n";

	renderer.replace_raster();

	std::cout << "Renderer replaced.\n";

	auto bg = color_b24({0, 0, 15});
	renderer.clear_raster(& bg);

	std::cout << "Renderer cleared.\n";

	renderer.paint_raster();

	auto start_time = std::chrono::high_resolution_clock::now();
	auto prev_time = start_time;
	auto run_delta = std::chrono::duration_cast<std::chrono::microseconds>(0ms)
		.count();
	[[maybe_unused]] auto frame_delta = run_delta;

	auto [ec, er] = extents;

	auto fg = color_b24({255, 255, 0});

	int run_time = 10;
	std::u8string msg = std::u8string(u8"Done in 0...");
	msg[8] = u8'0' + run_time - 1;

	int num_frames = 0;
	int const num_frame_times = 16;
	int frame_times[num_frame_times];
	int frame_time_cur = 0;
	auto calc_avg_frame_times = [&](auto frame_time)
	{
		frame_times[frame_time_cur] = frame_time;
		frame_time_cur = (frame_time_cur + 1) % num_frame_times;
		float total = 0.f;
		for (int i = 0; i < num_frame_times; ++i)
			{ total += frame_times[i]; }
		return total / num_frame_times;
	};
	

	std::ostringstream oss;

	while (run_delta < run_time * 1000 * 1000)
	{
		auto time = std::chrono::high_resolution_clock::now();
		frame_delta = std::chrono::duration_cast<std::chrono::microseconds>(time - prev_time)
			.count();
		run_delta = std::chrono::duration_cast<std::chrono::microseconds>(time - start_time)
			.count();
		prev_time = time;

		float fps = 1000.f * 1000.f / calc_avg_frame_times(frame_delta);
		oss << "FPS: " << std::fixed << std::setprecision(2) << fps;

		int offset = run_delta / 1000;

		for (uint y = 0; y < er * 2; y += 1)
		{
			for (uint x = 0; x < ec; x += 1)
			{
				auto off = offset / 8;
				auto co = color_b24({uint8_t((off + 4 * x) % 256),
									 uint8_t((off + 4 * (x + y) / 4 + 64) % 256),
									 uint8_t((off + 4 * y + 128) % 256)});
				renderer.set_pixel(x, y, co);
			}
		}
		uint fx = DOUBLE_UNDERLINE;
		renderer.print_str(ec / 2 - 5, er / 2, oss.view(), & fg, nullptr, & fx);
		oss.str("");
		renderer.paint_raster();
		++num_frames;
	}

	std::cout << "\n\nnum_frames: " << num_frames << "\x1b[?25h\n";
}
