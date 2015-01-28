#include <PXL.h>
#include <ctime>
#include <Windows.h>

PXL_Texture* load_texture(char* path) {
	std::string texture_path = "assets/";
	texture_path += path;
	PXL_Texture* texture = new PXL_Texture(&PXL_Bitmap(texture_path));
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
	timeBeginPeriod(1);

	PXL_create_window(1024, 768, "PXL Example Project");
	PXL_init();

	PXL_Texture* cat = load_texture("cat.png");
	PXL_Texture* cat_2 = load_texture("cat2.png");
	PXL_Texture* cute_cat = load_texture("cutecat.png");

	PXL_Font* font = new PXL_Font("assets/square.ttf");
	PXL_Text text(font, "", 150, 450, 42);
	text.set_origin(PXL_CENTER_ORIGIN);

	PXL_Rect sheet_rect;
	PXL_Rect sheet_src;
	PXL_Vec2 sheet_origin;
	PXL_TextureSheet sheet;

	sheet_src.x = 128; sheet_src.y = 64; sheet_src.w = 320; sheet_src.h = 200;
	sheet_rect.x = 0; sheet_rect.y = 0; sheet_rect.w = 1024; sheet_rect.h = 512;
	sheet.add(cat, &sheet_rect, &sheet_src);

	sheet_rect.x = 700; sheet_rect.y = 0; sheet_rect.w = 512; sheet_rect.h = 256;
	sheet.add(cute_cat, &sheet_rect);

	sheet_rect.x = 400; sheet_rect.y = 280; sheet_rect.w = 1024; sheet_rect.h = 200;
	sheet.add(cat_2, &sheet_rect);

	sheet_rect.x = 0; sheet_rect.y = 0;
	sheet_rect.w = PXL_window_width; sheet_rect.h = PXL_window_height;
	//sheet_origin.x = sheet_rect.w / 2; sheet_origin.y = sheet_rect.h / 2;

	sheet.create();

	std::vector<PXL_PointLight*> point_lights;
	for (int n = 0; n < 72 * 7; n += 7) {
		float radius = 200 + ((rand() / float(RAND_MAX)) * 400);
		point_lights.push_back(PXL_create_point_light(int((rand() / float(RAND_MAX)) * PXL_window_width),
			int((rand() / float(RAND_MAX)) * PXL_window_height), radius, .15f, 
			rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	}

	PXL_Batch batch = PXL_Batch(PXL_SMALL_BATCH);
	PXL_set_default_shader(&batch);

	int amount = 1000;
	int* pos = new int[amount * 2];
	for (int n = 0; n < amount * 2; n += 2) {
		pos[n] = int((rand() / float(RAND_MAX)) * 800);
		pos[n + 1] = int((rand() / float(RAND_MAX)) * 700);
	}

	PXL_set_clear_colour(0, 0, 0, 1);

	MSG msg;
	start_second_time.start();
	while (!quit) {
		start_time.start();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				quit = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		PXL_clear();

		//silly test code stuff
		PXL_Rect rect;
		PXL_Vec2 origin;
		rect.x = 400;
		rect.y = 300;
		rect.w = 512 / 2;
		rect.h = 320 / 2;
		origin.x = rect.w / 2;
		origin.y = rect.h / 2;
		t += .5f;

		PXL_set_default_shader(&batch);
		for (int n = 0; n < amount * 2; n += 2) {
			rect.x = pos[n] + origin.x;
			rect.y = pos[n + 1] + origin.y;
			if (rect.x >= 512) {
				batch.add(cat, &rect, NULL, .75f, .5f, 1, 1, t, &origin, PXL_FLIP_NONE);
			}else {
				batch.add(cat_2, &rect, NULL, t, &origin, PXL_FLIP_NONE);
			}
		}

		PXL_start_timer();
		batch.render_all();
		average_time += PXL_stop_timer();

		for (int n = 0; n < point_lights.size(); ++n) {
			point_lights[n]->intensity = (sin(t / (10 + (n / 10))) + 1) / 8;
			point_lights[n]->radius += sin(t / (10 + (n / 10)));
			point_lights[n]->intensity = PXL_clamp(point_lights[n]->intensity, 0, 99);
		}

		PXL_render_point_lights(&batch);

		text.set_text("timer: " + std::to_string(t) + "\nnewline testtext");
		text.rotation += cos(t / 10);
		text.set_colour(.5f, 0, 1, 1);
		text.scale(sin(t / 10) / 50, sin(t / 10) / 50);
		text.render(&batch);

		batch.render_all();

		//swaps back buffer to front buffer
		//glBindFramebuffer(GL_FRAMEBUFFER, 1);
		//glBlitFramebuffer(0, 0, PXL_window_width, PXL_window_height, 0, 0, PXL_window_width, PXL_window_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		PXL_swap_buffers();

		double ms = start_time.end() / 1000.0f;
		if (ms >= 0 && ms < ms_per_frame) { Sleep(ms_per_frame - ms); }

		++frame_counter;
		if (start_second_time.end() / 1000.0f >= 1000) {
			std::cout << "elapsed: " << average_time / frame_counter << 
						 ", ms: " << (average_time / frame_counter) / 1000.0f << "\n";
			average_time = 0;

			std::cout << "fps: " << frame_counter << "\n";
			frame_counter = 0;
			start_second_time.start();
		}
	}

	return 0;
}