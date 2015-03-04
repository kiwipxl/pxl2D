#include <PXL.h>
#include <ctime>
#include <Windows.h>

int main(int argc, char* args[]) {
	srand(time(NULL));

	float t = 0;
	float fps = 60;
	float ms_per_frame = 1000 / fps;
	bool quit = false;
	PXL_Timer start_time;
	PXL_Timer start_second_time;
	int frame_counter = 0;
	long average_time = 0;
	int average_count = 0;
	int seconds_elapsed = 0;

	PXL_Window* window = PXL_create_window(1024, 768, "PXL Example Project");
	PXL_init();

	PXL_Texture cat;			cat.create_texture("assets/cat.png");
	PXL_Texture cat_2;			cat_2.create_texture("assets/cat2.png");
	PXL_Texture cute_cat;		cute_cat.create_texture("assets/cutecat.png");
	PXL_Texture grid_hex;		grid_hex.create_texture("assets/grid_hex.png");

	PXL_Font square("assets/square.ttf");
	PXL_Font arcade("assets/arcade.ttf");
	PXL_Text text(&arcade, "", 150, 450, 42);
	text.set_origin(PXL_ORIGIN_CENTER);

	PXL_Batch batch = PXL_Batch(PXL_BATCH_SMALL);

	int num = PXL_num_joysticks();
	if (num > 0) {
		PXL_Joystick* j = PXL_get_joystick(0);
		j->activate();
	}

	PXL_set_clear_colour(0, 0, 0, 1);

	float grid_x = 0; float grid_y = 0;
	PXL_Colour grid_colour;
	float r_speed = 0; float g_speed = 0; float b_speed = 0;
	int r_timer = 100; int g_timer = 100; int b_timer = 100;

	PXL_FrameBuffer grid_buffer(PXL_window_width, PXL_window_height);
	PXL_FrameBuffer small_buffer(PXL_window_width, PXL_window_height);
	PXL_Texture screen_texture;
	screen_texture.create_texture(PXL_window_width, PXL_window_height, 0);

	const int s_w = 25;
	const int s_h = 18;
	struct Node {

		PXL_Colour colour;
		PXL_Colour smooth_colour;
		float rgb_speed;
		int rgb_timer;
	};
	Node** grid = new Node*[s_h];
	for (int y = 0; y < s_h; ++y) {
		Node* rows = new Node[s_w];
		for (int x = 0; x < s_w; ++x) {
			rows[x] = Node();
		}
		grid[y] = rows;
	}
	struct Pulse {

		Pulse(PXL_uint colour_val) {
			colour.set_colour(colour_val);
		}

		float radius = 1;
		float cycles = 8;
		float r = 360 / cycles;
		int start_x = 5;
		int start_y = 10;
		int s_x = start_x + radius;
		int s_y = start_y;
		PXL_Colour colour;
	};
	std::vector<Pulse> pulses;

	PXL_Sprite player(grid_hex);
	PXL_Sprite player_2(grid_hex);

	start_second_time.start();
	while (!quit) {
		start_time.start();

		PXL_Event e;
		while (window->poll_event(e)) {
			if (e.type == PXL_EVENT_KEYUP && e.key_code == PXL_KEY_P) {
				pulses.push_back(Pulse(rand() % 0xffffffff));
			}
			if (e.type == PXL_EVENT_QUIT) {
				quit = true;
				break;
			}
		}

		PXL_start_timer();

		PXL_set_clear_colour(0, 0, 0, 1);
		PXL_clear();

		t += .5f;
		PXL_Rect rect(grid_x, grid_y, 64, 64);
		PXL_Rect src_rect(0, 0, 64, 64);

		grid_colour.r += r_speed;
		grid_colour.g += g_speed;
		grid_colour.b += b_speed;

		batch.view_mat.identity();
		float t_x = (player_2.x - player.x) + (PXL_window_width / 2);
		float t_y = (player_2.y - player.y) + (PXL_window_height / 2);
		batch.view_mat.translate(t_x, t_y);

		++r_timer;
		if (r_timer >= 100 || grid_colour.r >= 1 || grid_colour.r <= .2f) {
			r_speed = ((rand() / (float)RAND_MAX) - (rand() / (float)RAND_MAX)) / 40; r_timer = r_speed * 100;

			if (grid_colour.r <= .2f) { grid_colour.r = .2f; r_speed = abs(r_speed); }
			if (grid_colour.r >= 1) { grid_colour.r = 1; r_speed = -abs(r_speed); }
		}
		
		++g_timer;
		if (g_timer >= 100 || grid_colour.g >= 1 || grid_colour.g <= .2f) {
			g_speed = ((rand() / (float)RAND_MAX) - (rand() / (float)RAND_MAX)) / 40; g_timer = g_speed * 100;

			if (grid_colour.g <= .2f) { grid_colour.g = .2f; g_speed = abs(g_speed); }
			if (grid_colour.g >= 1) { grid_colour.g = 1; g_speed = -abs(g_speed); }
		}
		
		++b_timer;
		if (b_timer >= 100 || grid_colour.b >= 1 || grid_colour.b <= .2f) {
			b_speed = ((rand() / (float)RAND_MAX) - (rand() / (float)RAND_MAX)) / 40; b_timer = b_speed * 100;

			if (grid_colour.b <= .2f) { grid_colour.b = .2f; b_speed = abs(b_speed); }
			if (grid_colour.b >= 1) { grid_colour.b = 1; b_speed = -abs(b_speed); }
		}

		for (int n = 0; n < pulses.size(); ++n) {
			Pulse& pulse = pulses[n];
			if (pulse.radius <= 20) {
				pulse.radius += .5f;
				pulse.cycles = pulse.radius * 14;
				pulse.r = 360 / pulse.cycles;
				pulse.s_x = pulse.start_x;
				pulse.s_y = pulse.start_y;
				for (int n = 0; n < pulse.cycles; ++n) {
					float rotation = (n * pulse.r) / PXL_radian;
					int pos_x = (int)round(pulse.s_y + (sin(rotation) * pulse.radius));
					int pos_y = (int)round(pulse.s_x + (cos(rotation) * pulse.radius));
					if (n == 0) { pos_x = pulse.start_x; pos_y = pulse.start_y; }
					if (pos_y >= 0 && pos_y < s_h && pos_x >= 0 && pos_x < s_w) {
						pulse.colour.a = 1;
						grid[pos_y][pos_x].colour = pulse.colour;
					}
				}
			}else {
				pulses.erase(pulses.begin() + n);
				--n;
			}
		}

		batch.set_target(&grid_buffer);
		grid_buffer.clear(0, 0, 0, 1);
		PXL_Colour temp = grid_colour;
		for (int y = 0; y < s_h; ++y) {
			for (int x = 0; x < s_w; ++x) {
				rect.x += 64;

				const float smooth_min = .1f;
				const float smooth_max = 1;

				if (grid[y][x].smooth_colour.r <= smooth_min || grid[y][x].smooth_colour.r >= smooth_max ||
					grid[y][x].smooth_colour.g <= smooth_min || grid[y][x].smooth_colour.g >= smooth_max ||
					grid[y][x].smooth_colour.b <= smooth_min || grid[y][x].smooth_colour.b >= smooth_max) {
					grid[y][x].rgb_speed = ((rand() / (float)RAND_MAX) - (rand() / (float)RAND_MAX)) / 40;

					if (grid[y][x].smooth_colour.r <= smooth_min) { grid[y][x].smooth_colour.r = smooth_min; grid[y][x].rgb_speed = abs(grid[y][x].rgb_speed); }
					if (grid[y][x].smooth_colour.r >= smooth_max) { grid[y][x].smooth_colour.r = smooth_max; grid[y][x].rgb_speed = -abs(grid[y][x].rgb_speed); }

					if (grid[y][x].smooth_colour.g <= smooth_min) { grid[y][x].smooth_colour.g = smooth_min; grid[y][x].rgb_speed = abs(grid[y][x].rgb_speed); }
					if (grid[y][x].smooth_colour.g >= smooth_max) { grid[y][x].smooth_colour.g = smooth_max; grid[y][x].rgb_speed = -abs(grid[y][x].rgb_speed); }

					if (grid[y][x].smooth_colour.b <= smooth_min) { grid[y][x].smooth_colour.b = smooth_min; grid[y][x].rgb_speed = abs(grid[y][x].rgb_speed); }
					if (grid[y][x].smooth_colour.b >= smooth_max) { grid[y][x].smooth_colour.b = smooth_max; grid[y][x].rgb_speed = -abs(grid[y][x].rgb_speed); }
				}

				grid[y][x].smooth_colour.r += grid[y][x].rgb_speed;
				grid[y][x].smooth_colour.g += grid[y][x].rgb_speed;
				grid[y][x].smooth_colour.b += grid[y][x].rgb_speed;

				grid[y][x].colour.r -= (grid[y][x].colour.r - 1) / 50.0f;
				grid[y][x].colour.g -= (grid[y][x].colour.g - 1) / 50.0f;
				grid[y][x].colour.b -= (grid[y][x].colour.b - 1) / 50.0f;

				grid_colour = temp;
				grid_colour.r *= grid[y][x].colour.r;
				grid_colour.g *= grid[y][x].colour.g;
				grid_colour.b *= grid[y][x].colour.b;
				grid_colour.a *= grid[y][x].colour.a;
				grid_colour.r *= grid[y][x].smooth_colour.r;
				grid_colour.g *= grid[y][x].smooth_colour.g;
				grid_colour.b *= grid[y][x].smooth_colour.b;
				grid_colour.clamp(0, 1);

				batch.add(grid_hex, &rect, &src_rect, 0, 0, PXL_FLIP_NONE, 0, grid_colour);
			}
			rect.x = (-((y + 1) % 2) * 32) + grid_x;
			rect.y += 48;
		}
		grid_colour = temp;

		batch.render_all();

		text.set_text("P1: 0     P2: 0");
		text.x = (PXL_window_width / 2) - (text.get_width() / 2);
		text.y = 40;
		text.colour.set_colour(0, (cos(t / 10) + 1) / 2, 1, 1);
		text.z_depth = batch.get_max_z_depth() - 1;
		text.render(&batch);

		const int scroll_speed = 8;
		if (PXL_key_down(PXL_KEY_A)) {
			player.x -= scroll_speed;
		}else if (PXL_key_down(PXL_KEY_D)) {
			player.x += scroll_speed;
		}
		if (PXL_key_down(PXL_KEY_W)) {
			player.y -= scroll_speed;
		}else if (PXL_key_down(PXL_KEY_S)) {
			player.y += scroll_speed;
		}
		player.render(&batch);

		batch.render_all();

		PXL_set_bloom_shader(&batch, 10, 1);

		rect.x = 0; rect.y = 0; rect.w = PXL_window_width; rect.h = PXL_window_height;
		batch.set_target(0);
		batch.add(*grid_buffer.get_texture(), &rect, 0, 0, 0, PXL_FLIP_VERTICAL, 0, PXL_COLOR_WHITE, PXL_bloom_shader);
		batch.render_all();

		average_time += PXL_stop_timer();

		//swaps back buffer to front buffer
		PXL_swap_buffers();

		double ms = start_time.end() / 1000.0f;
		if (ms >= 0 && ms < ms_per_frame) { Sleep(ms_per_frame - ms); }

		++frame_counter;
		++average_count;
		if (start_second_time.end() / 1000.0f >= 1000) {
			if (seconds_elapsed % 2 == 0) {
				std::cout << "elapsed: " << average_time / average_count <<
					", ms: " << (average_time / average_count) / 1000.0f << "\n";
				average_time = 0;
				average_count = 0;
			}
			++seconds_elapsed;

			std::cout << "fps: " << frame_counter << "\n";
			frame_counter = 0;
			start_second_time.start();
		}
	}

	return 0;
}