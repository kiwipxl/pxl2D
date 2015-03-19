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

	PXL_Bitmap bitmap;		bitmap.create_bitmap(400, 300, PXL_COLOR_LIGHT_GREEN, PXL_CHANNEL_RGBA);

	PXL_Texture cat;			cat.create_texture("assets/cat2.png");
	PXL_Texture cat_2;			cat_2.create_texture("assets/cat2.png");
	PXL_Texture cute_cat;		cute_cat.create_texture("assets/cutecat.png");

	PXL_print << "loaded cat pix. loading fonts...\n";

	PXL_Font square("assets/square.ttf");
	PXL_Font arcade("assets/arcade.ttf");
	PXL_Text text(&arcade, "", 150, 450, 42);
	text.set_origin(PXL_ORIGIN_CENTER);

	PXL_print << "loaded fonts. creating batch...\n";

	PXL_Batch batch = PXL_Batch(&window, PXL_BATCH_SMALL);

	PXL_print << "created batch. checking joysticks...\n";

	int num = PXL_num_joysticks();
	if (num > 0) {
		PXL_Joystick* j = PXL_get_joystick(0);
		j->activate();
	}

	PXL_print << "checked joysticks. running loop...\n";

	PXL_Rect cat_rect(0, 0, 400, 300);

	start_second_time.start();
	while (!quit) {
		start_time.start();

		PXL_Event e;
		while (window.poll_event(e)) {
			if (e.type == PXL_EVENT_TOUCH) {
				if (e.touch_event.num_touching <= 2) {
					text.x = e.touch_event.touches[0].x;
					text.y = e.touch_event.touches[0].y;
				}
				if (e.touch_event.num_touching == 2) {
					cat_rect.x = e.touch_event.touches[1].x;
					cat_rect.y = e.touch_event.touches[1].y;
				}
			}

			if (e.type == PXL_EVENT_QUIT) {
				quit = true;
				PXL_print << "am quitting\n";
				break;
			}
		}

		PXL_start_timer();

		t += .5f;
		PXL_set_clear_colour(0, 0, 0, 1);
		PXL_clear();

		batch.add(cat, &cat_rect);
		PXL_Rect rect(cos(t / 20) * 400, 0, 400, 300);
		batch.add(cat_2, &rect);

		rect.x = 0; rect.y = 400;
		rect.w = arcade.get_glyph_sheet()->get_width(); rect.h = arcade.get_glyph_sheet()->get_height();
		batch.add(*arcade.get_glyph_sheet(), &rect, 0, 0, 0, PXL_FLIP_NONE, 0, PXL_COLOR_WHITE, PXL_text_shader);

		text.set_text("hey there sexy ladeh ;)\n");
		text.colour.set_colour(0, (cos(t / 10) + 1) / 2, 1, 1);
		text.z_depth = 1;
		text.render(&batch);

		batch.render_all();

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