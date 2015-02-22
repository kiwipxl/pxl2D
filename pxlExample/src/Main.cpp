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

	PXL_Sprite cat_sprite(cat);
	cat_sprite.set_origin(PXL_ORIGIN_CENTER);

	PXL_Font square("assets/square.ttf");
	PXL_Font arcade("assets/arcade.ttf");
	PXL_Text text(&arcade, "", 150, 450, 42);
	text.set_origin(PXL_ORIGIN_CENTER);

	PXL_Batch batch = PXL_Batch(PXL_BATCH_SMALL);
	PXL_set_default_shader(&batch);

	int num = PXL_num_joysticks();
	if (num > 0) {
		PXL_Joystick* j = PXL_get_joystick(0);
		j->activate();
	}

	PXL_set_clear_colour(0, 0, 0, 1);

	float grid_x = 0; float grid_y = 0;
	PXL_Colour grid_colour;
	float r_speed = 0; float g_speed = 0; float b_speed = 0;
	int r_timer = 0; int g_timer = 0; int b_timer = 0;

	start_second_time.start();
	while (!quit) {
		start_time.start();

		PXL_Event e;
		while (window->poll_event(e)) {
			if (e.type == PXL_EVENT_KEYDOWN) {
				std::cout << e.key_code << "\n";
				if (e.key_code == PXL_KEY_RETURN) {
					std::cout << "key pressed\n";
				}
			}

			if (e.type == PXL_EVENT_QUIT) {
				quit = true;
				break;
			}
		}

		PXL_set_clear_colour(0, 0, 0, 1);
		PXL_clear();

		const int scroll_speed = 8;
		if (PXL_key_down(PXL_KEY_A)) {
			grid_x += scroll_speed;
		}else if (PXL_key_down(PXL_KEY_D)) {
			grid_x -= scroll_speed;
		}
		if (PXL_key_down(PXL_KEY_W)) {
			grid_y += scroll_speed;
		}else if (PXL_key_down(PXL_KEY_S)) {
			grid_y -= scroll_speed;
		}
		t += .5f;
		PXL_Rect rect(grid_x, grid_y, 64, 64);
		PXL_Rect src_rect(0, 0, 64, 64);
		int s_w = 10; float w = s_w;
		int h = 25;
		++r_timer; ++g_timer; ++b_timer;
		if (r_timer >= 100) { r_speed = ((rand() / (float)RAND_MAX) - (rand() / (float)RAND_MAX)) / 10; r_timer = r_speed * 100; }

		grid_colour.r += r_speed;
		grid_colour.g += g_speed;
		grid_colour.b += b_speed;

		for (int y = 0; y < h; ++y) {
			for (int x = 0; x < int(w); ++x) {
				rect.x += 68;
				batch.add(grid_hex, &rect, &src_rect, 0, 0, PXL_FLIP_NONE, 0, grid_colour);
			}
			rect.x = -36 * (w - s_w + 1) + grid_x;
			rect.y += 52;

			if (y >= h / 2) {
				rect.x += 68;
				--w;
			}else {
				++w;
			}
		}

		text.set_text("timer: " + std::to_string(t) + "\nnewline testtext");
		text.rotation += PXL_fast_cos(t / 10);
		text.colour.set_colour(0, (cos(t / 10) + 1) / 2, 1, 1);
		text.scale(PXL_fast_sin(t / 10) / 50, PXL_fast_sin(t / 10) / 50);
		text.z_depth = 0;
		text.render(&batch);

		PXL_start_timer();
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