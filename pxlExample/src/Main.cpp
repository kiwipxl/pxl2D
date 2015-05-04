#include <PXL.h>

//temporary to_string function until one has been added to pxl
template <typename T> std::string to_string(const T& n) {
	std::ostringstream stm;
	stm << n;
	return stm.str();
}

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

	PXL_Window window(480, 800, "PXL Example Project");
	PXL_init();

	PXL_print << "loading assets...\n";

	PXL_Texture arena_texture;			arena_texture.create_texture("assets/arena.png");
	PXL_Texture puck_texture;			puck_texture.create_texture("assets/puck.png");
	PXL_Texture p1paddle_texture;		p1paddle_texture.create_texture("assets/p1paddle.png");
	PXL_Texture p2paddle_texture;		p2paddle_texture.create_texture("assets/p2paddle.png");

	PXL_print << "loaded assets. loading fonts...\n";

	PXL_Vec2 window_center(window.get_width() / 2, window.get_height() / 2);

	struct Paddle : public PXL_Sprite {

		PXL_Vec2 dest;
		int touchid;
		PXL_Vec2 center;
	};

	Paddle p1paddle;		p1paddle.set_texture(p1paddle_texture);
	p1paddle.width = 96;	p1paddle.height = 96;
	p1paddle.dest.x = window_center.x;
	p1paddle.dest.y = window_center.y / 2;
	p1paddle.x = p1paddle.dest.x;
	p1paddle.y = p1paddle.dest.y;
	Paddle p2paddle;		p2paddle.set_texture(p2paddle_texture);
	p2paddle.dest.x = window_center.x;
	p2paddle.dest.y = window_center.y + (window_center.y / 2);
	p2paddle.x = p2paddle.dest.x;
	p2paddle.y = p2paddle.dest.y;
	p2paddle.width = 96;	p2paddle.height = 96;

	p1paddle.center.x = p1paddle.width / 2;		p1paddle.center.y = p1paddle.height / 2;
	p2paddle.center.x = p2paddle.width / 2;		p2paddle.center.y = p2paddle.height / 2;

	PXL_Sprite puck(puck_texture);
	PXL_Vec2 puck_speed;
	puck.x = window.get_width() / 2;	puck.y = window.get_height() / 2;
	puck.width = 64;					puck.height = 64;
	puck.z_depth = 1;

	PXL_Font square("assets/square.ttf");
	PXL_Font arcade("assets/arcade.ttf");
	PXL_Text p1score_text(&arcade, "0", window.get_width() - 40, window_center.y - 100, 42);
	p1score_text.set_origin(PXL_ORIGIN_MID_LEFT);
	p1score_text.rotation = 90;
	p1score_text.z_depth = 2;

	PXL_Text p2score_text(&arcade, "0", window.get_width() - 40, window_center.y + 32, 42);
	p2score_text.set_origin(PXL_ORIGIN_MID_LEFT);
	p2score_text.rotation = 90;
	p2score_text.z_depth = 2;

	int p1score = 0;
	int p2score = 0;

	PXL_print << "loaded fonts. creating batch...\n";

	PXL_Batch batch = PXL_Batch(&window, PXL_BATCH_TINY);

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
			if (e.type == PXL_EVENT_TOUCH) {
				for (int i = 0; i < e.touch_event.num_touching; ++i) {
					PXL_TouchInfo info = e.touch_event.touches[i];

					if (info.y - p1paddle.center.y >= window_center.y && info.state == PXL_TOUCH_DOWN) {
						p1paddle.touchid = info.id;
						p1paddle.dest.x = info.x - p1paddle.center.x;
						p1paddle.dest.y = info.y - p1paddle.center.y;
					}else if (p1paddle.touchid == info.id) {
						p1paddle.dest.x = info.x - p1paddle.center.x;
						if (info.y - p1paddle.center.y >= window_center.y) {
							p1paddle.dest.y = info.y - p1paddle.center.y;
						}else {
							p1paddle.dest.y = window_center.y;
						}
						if (info.state == PXL_TOUCH_UP) {
							p1paddle.touchid = -1;
						}
					}else if (info.y - p2paddle.center.y <= window_center.y - p2paddle.height && info.state == PXL_TOUCH_DOWN) {
						p2paddle.touchid = info.id;
						p2paddle.dest.x = info.x - p2paddle.center.x;
						p2paddle.dest.y = info.y - p2paddle.center.y;
					}else if (p2paddle.touchid == info.id) {
						p2paddle.dest.x = info.x - p2paddle.center.x;
						if (info.y - p2paddle.center.y <= window_center.y - p2paddle.height) {
							p2paddle.dest.y = info.y - p2paddle.center.y;
						}else {
							p2paddle.dest.y = window_center.y - p2paddle.height;
						}
						if (info.state == PXL_TOUCH_UP) {
							p2paddle.touchid = -1;
						}
					}
				}
			}

			/*if (e.mouse_y - p1paddle.center.y >= window_center.y) {
				p1paddle.dest.x = e.mouse_x - p1paddle.center.x;
				if (e.mouse_y - p1paddle.center.y >= window_center.y) {
					p1paddle.dest.y = e.mouse_y - p1paddle.center.y;
				}else {
					p1paddle.dest.y = window_center.y;
				}
			}else if (e.mouse_y - p2paddle.center.y <= window_center.y - p2paddle.height) {
				p2paddle.dest.x = e.mouse_x - p2paddle.center.x;
				if (e.mouse_y - p2paddle.center.y <= window_center.y - p2paddle.height) {
					p2paddle.dest.y = e.mouse_y - p2paddle.center.y;
				}else {
					p2paddle.dest.y = window_center.y - p2paddle.height;
				}
			}*/

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

		PXL_Rect rect(0, 0, window.get_width(), window.get_height());
		batch.add(arena_texture, &rect);

		p1score_text.colour.set_colour(0, (cos(t / 10) + 1) / 2, 1, 1);
		p1score_text.render(&batch);

		p2score_text.colour.set_colour(0, (cos(t / 10) + 1) / 2, 1, 1);
		p2score_text.render(&batch);

		p1paddle.x -= (p1paddle.x - p1paddle.dest.x) / 4;
		p1paddle.y -= (p1paddle.y - p1paddle.dest.y) / 4;
		p2paddle.x -= (p2paddle.x - p2paddle.dest.x) / 4;
		p2paddle.y -= (p2paddle.y - p2paddle.dest.y) / 4;

		p1paddle.render(&batch);
		p2paddle.render(&batch);

		if (puck.x >= window.get_width() - 64) {	puck_speed.x = -puck_speed.x; puck.x = window.get_width() - 64; }
		if (puck.x <= 64) {							puck_speed.x = -puck_speed.x; puck.x = 64; }
		if (puck.y >= window.get_height() - 64) {
			if (puck.x >= window_center.x / 2 && puck.x <= window_center.x + (window_center.x / 2)) {
				puck.x = window.get_width() / 2;	puck.y = window.get_height() / 2;
				++p2score;
				p2score_text.set_text(to_string(p2score));
			}else {
				puck_speed.y = -puck_speed.y;		puck.y = window.get_height() - 64;
			}
		}
		if (puck.y <= 64) {
			if (puck.x >= window_center.x / 2 && puck.x <= window_center.x + (window_center.x / 2)) {
				puck.x = window.get_width() / 2;	puck.y = window.get_height() / 2;
				++p1score;
				p1score_text.set_text(to_string(p1score));
				p1score_text.y = (window_center.y - 100) - (p1score_text.get_width());
			}else {
				puck_speed.y = -puck_speed.y;		puck.y = 64;
			}
		}

		puck.x += puck_speed.x;
		puck.y += puck_speed.y;
		puck_speed.x = puck_speed.x * .95f;
		puck_speed.y = puck_speed.y * .95f;

		float p1dist = sqrt(pow(float(p1paddle.x - puck.x), 2) + pow(float(p1paddle.y - puck.y), 2));
		if (p1dist <= 80) {
			puck_speed.x -= (p1paddle.x - puck.x) / 4;
			puck_speed.y -= (p1paddle.y - puck.y) / 4;
		}
		float p2dist = sqrt(pow(float(p2paddle.x - puck.x), 2) + pow(float(p2paddle.y - puck.y), 2));
		if (p2dist <= 80) {
			puck_speed.x -= (p2paddle.x - puck.x) / 4;
			puck_speed.y -= (p2paddle.y - puck.y) / 4;
		}

		puck.render(&batch);

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