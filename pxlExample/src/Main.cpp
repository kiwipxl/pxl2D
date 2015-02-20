#include <PXL.h>
#include <ctime>
#include <Windows.h>

PXL_Texture* load_texture(char* path) {
	std::string texture_path = "assets/";
	texture_path += path;
	PXL_Texture* texture = new PXL_Texture();
	texture->create_texture(texture_path);
	return texture;
}

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

	PXL_Texture* cat = load_texture("cat.png");
	PXL_Texture* cat_2 = load_texture("cat2.png");
	PXL_Texture* cute_cat = load_texture("cutecat.png");

	PXL_Font* font = new PXL_Font("assets/square.ttf");

	PXL_Batch batch = PXL_Batch(PXL_BATCH_TINY);
	PXL_set_default_shader(&batch);

	int num = PXL_num_joysticks();
	if (num > 0) {
		PXL_Joystick* j = PXL_get_joystick(0);
		j->activate();
	}

	PXL_set_clear_colour(0, 0, 0, 1);

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