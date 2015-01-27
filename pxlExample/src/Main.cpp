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
	std::clock_t start_time;
	std::clock_t start_second_time = 0;
	int frame_counter = 0;
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
		float radius = 200 + ((rand() / float(RAND_MAX)) * 200);
		point_lights.push_back(PXL_create_point_light(int((rand() / float(RAND_MAX)) * (PXL_window_width + radius)),
			int((rand() / float(RAND_MAX)) * (PXL_window_height + radius)), radius, .25f, 
			rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
	}

	PXL_Batch batch = PXL_Batch(PXL_LARGE_BATCH);

	int amount = 2;
	int* pos = new int[amount * 2];
	for (int n = 0; n < amount * 2; n += 2) {
		pos[n] = int((rand() / float(RAND_MAX)) * 700);
		pos[n + 1] = int((rand() / float(RAND_MAX)) * 550);
	}

	MSG msg;

	while (!quit) {
		start_time = std::clock();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				quit = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		std::clock_t render_timer = std::clock();

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
		batch.add(&sheet, &sheet_rect, NULL, 0, &sheet_origin, PXL_FLIP_NONE);

		for (int n = 0; n < point_lights.size(); ++n) {
			point_lights[n]->intensity += cos(t / (10 + (n / 10))) / 40;
			point_lights[n]->radius += sin(t / (10 + (n / 10))) / 15;
			point_lights[n]->intensity = PXL_clamp(point_lights[n]->intensity, 0, 99);
		}
		PXL_render_point_lights(&batch);

		PXL_set_bloom_shader(&batch, cos(t / 4) + 1, (sin(t / 8) / 2) + .5f);
		for (int n = 0; n < amount * 2; n += 2) {
			rect.x = pos[n] + rect.w;
			rect.y = pos[n + 1] + rect.h;
			batch.add(cat, &rect, NULL, t, &origin, PXL_FLIP_NONE);
		}

		batch.render_all();

		text.set_text("timer: " + std::to_string(t) + "\nnewline testtext");
		text.rotation += cos(t / 10);
		text.colour.r = ((cos(t / 4) / 2) + .5f) * 255;
		text.colour.g = ((sin(t / 6) / 2) + .5f) * 255;
		text.colour.b = ((sin(t / 8) / 2) + .5f) * 255;
		text.scale(sin(t / 10) / 50, sin(t / 10) / 50);
		text.render(&batch);

		batch.render_all();

		//swaps back buffer to front buffer
		//glBindFramebuffer(GL_FRAMEBUFFER, 1);
		//glBlitFramebuffer(0, 0, PXL_window_width, PXL_window_height, 0, 0, PXL_window_width, PXL_window_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		PXL_swap_buffers();

		double ms = std::clock() - start_time;
		if (ms >= 0 && ms < ms_per_frame) { Sleep(ms_per_frame - ms); }

		++frame_counter;
		if (std::clock() - start_second_time >= 1000) {
			std::cout << "fps: " << frame_counter << "\n";
			frame_counter = 0;
			start_second_time = std::clock();
		}
	}

	return 0;
}