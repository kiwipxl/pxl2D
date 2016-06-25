#include <PXL.h>
#include <ctime>

using namespace pxl;

//temporary to_string function until one has been added to pxl
template <typename T> std::string to_string(const T& n) {
	std::ostringstream stm;
	stm << n;
	return stm.str();
}

const inline void func() {

}

int main(int argc, char* args[]) {
	srand(time(NULL));

	float t = 0;
	float fps = 60;
	float ms_per_frame = 1000 / fps;
	bool quit = false;
	sys::Timer start_time;
	sys::Timer start_second_time;
	int frame_counter = 0;
	long average_time = 0;
	int average_count = 0;
    int seconds_elapsed = 0;

    /*//test time comparisons
    std::vector<int> test;
    int* test2 = new int[10000];
    for (int n = 0; n < 10000; ++n) {
        test.push_back(n);
        test2[n] = n;
    }

    Timer time;
    time.start();
    for (int n = 0; n < 100000000; ++n) {
        test[n % 10000] = n * 2;
    }
    long long result = time.end();
    std::cout << "r1: " << result << "\n";

    time.start();
    for (int n = 0; n < 100000000; ++n) {
        test2[n % 10000] = n * 2;
    }
    result = time.end();
    std::cout << "r2: " << result << "\n";*/

    sys::Window window(480, 800, "PXL Example Project");
    init();

    graphics::Batch batch = graphics::Batch(&window);

    sys::print << "loading assets...\n";

    graphics::Texture arena_texture;		arena_texture.create_texture("assets/arena.png");
    graphics::Texture puck_texture;			puck_texture.create_texture("assets/puck.png");
    graphics::Texture p1paddle_texture;		p1paddle_texture.create_texture("assets/p1paddle.png");
    graphics::Texture p2paddle_texture;		p2paddle_texture.create_texture("assets/p2paddle.png");
    graphics::Texture test_cat_texture;     test_cat_texture.create_texture("assets/lol.png");

	sys::print << "loaded assets. loading fonts...\n";

	Vec2 window_center(window.get_width() / 2, window.get_height() / 2);

	struct Paddle : public graphics::Sprite {

		Vec2 dest;
		int touchid;
		Vec2 center;
    };

    graphics::Sprite puck(puck_texture);
	Vec2 puck_speed;
	puck.x = window.get_width() / 2;	puck.y = window.get_height() / 2;
	puck.width = 64;					puck.height = 64;
    puck.z_depth = 1;

    sys::print << "loaded fonts. creating batch...\n";

	sys::print << "created batch. checking joysticks...\n";

    {
        using namespace input;
	   
       /* int num = num_joysticks();
	    if (num > 0) {
		    Joystick* j = get_joystick(0);
		    j->activate();
	    }*/
    }

	sys::print << "checked joysticks. running loop...\n";

	start_second_time.start();
	while (!quit) {
		start_time.start();

		sys::Event e;
		while (window.poll_event(e)) {
			if (e.type == EVENT_QUIT) {
				quit = true;
				sys::print << "am quitting\n";
				break;
			}
		}

		sys::start_timer();

		t += .5f;
        graphics::set_clear_colour(0, 0, 0, 1);
        graphics::clear();

        puck.render(&batch);

		batch.render_all();

		window.display();

		//double ms = start_time.end() / 1000.0f;
		//if (ms >= 0 && ms < ms_per_frame) { sleep(ms_per_frame - ms); }

		++frame_counter;
		++average_count;
		if (start_second_time.end() / 1000.0f >= 1000) {
			if (seconds_elapsed % 2 == 0) {
				sys::print << "elapsed: " << average_time / average_count <<
					", ms: " << (average_time / average_count) / 1000.0f << "\n";
				average_time = 0;
				average_count = 0;
			}
			++seconds_elapsed;

			sys::print << "fps: " << frame_counter << "\n";

			frame_counter = 0;
			start_second_time.start();
		}
	}

	return 0;
}