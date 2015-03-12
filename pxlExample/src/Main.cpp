#include <PXL.h>

int main(int argc, char* args[]) {
	//srand(time(NULL));

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

	PXL_Window window(1024, 768, "PXL Example Project");
	PXL_init();

	PXL_print << "loading cat pix...\n";

	PXL_Texture cat;			cat.create_texture("assets/cat.png");
	PXL_Texture cat_2;			cat_2.create_texture("assets/cat2.png");
	PXL_Texture cute_cat;		cute_cat.create_texture("assets/cutecat.png");

	PXL_print << "loaded cat pix. loading fonts...\n";

	PXL_Font square("assets/square.ttf");
	PXL_Font arcade("assets/arcade.ttf");
	PXL_Text text(&arcade, "", 150, 450, 42);
	text.set_origin(PXL_ORIGIN_CENTER);

	PXL_print << "loaded fonts. creating batch...\n";

	PXL_Batch batch = PXL_Batch(PXL_BATCH_SMALL);

	PXL_print << "created batch. checking joysticks...\n";

	int num = PXL_num_joysticks();
	if (num > 0) {
		PXL_Joystick* j = PXL_get_joystick(0);
		j->activate();
	}

	PXL_print << "checked joysticks. running loop...\n";

	start_second_time.start();
	while (!quit) {
		start_time.start();

		PXL_Event e;
		while (window.poll_event(e)) {
			if (e.type == PXL_EVENT_QUIT) {
				quit = true;
				PXL_print << "am quitting\n";
				break;
			}
		}

		PXL_start_timer();

		t += .5f;
		PXL_set_clear_colour(0, sin(t / 10) + sin(t / 4) + cos(t) + cos(t / 20), 0, 1);
		PXL_clear();

		/*text.set_text("P1: 0     P2: 0");
		PXL_print << "running loop12.25\n";
		text.x = (window.get_width() / 2) - (text.get_width() / 2);
		PXL_print << "running loop12.5\n";
		text.y = 40;
		PXL_print << "running loop14\n";
		text.colour.set_colour(0, (cos(t / 10) + 1) / 2, 1, 1);
		text.z_depth = batch.get_max_z_depth() - 1;
		text.render(&batch);*/

		window.display();

		//double ms = start_time.end() / 1000.0f;
		//if (ms >= 0 && ms < ms_per_frame) { PXL_sleep(ms_per_frame - ms); }

		++frame_counter;
		++average_count;
		if (start_second_time.end() / 1000.0f >= 1000) {
			if (seconds_elapsed % 2 == 0) {
				PXL_print << "elapsed: " << average_time / average_count <<
					", ms: " << (average_time / average_count) / 1000.0f << "\n";
				average_time = 0;
				average_count = 0;
			}
			++seconds_elapsed;

			PXL_print << "fps: " << frame_counter << "\n";

			frame_counter = 0;
			start_second_time.start();
		}
	}

	return 0;
}