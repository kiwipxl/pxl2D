#include "system/PXL_ImageIO.h"

#include <fstream>
#include <png.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <unistd.h>

#include "system/PXL_Exception.h"
#include "system/PXL_Debug.h"
#include "system/android/PXL_AndroidWindow.h"

#define PNG_SIG_SIZE 8

bool png_validate(std::ifstream& file);
void read_png(png_structp read, png_bytep data, png_size_t length);

std::ifstream file;

PXL_Bitmap* PXL_load_png(std::string file_name, PXL_Bitmap* bitmap) {
	if (bitmap == NULL) return NULL;

	PXL_print << "test\n";

	JNIEnv* env = android_state->activity->env;
	JavaVM* vm = android_state->activity->vm;
	vm->AttachCurrentThread(&env, NULL);

	jclass activityClass = env->GetObjectClass(android_state->activity->clazz);

	PXL_print << "test2\n";

	// Get path to cache dir (/data/data/org.wikibooks.OpenGL/cache)
	jmethodID getCacheDir = env->GetMethodID(activityClass, "getCacheDir", "()Ljava/io/File;");
	jobject file2 = env->CallObjectMethod(android_state->activity->clazz, getCacheDir);
	jclass fileClass = env->FindClass("java/io/File");
	jmethodID getAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
	jstring jpath = (jstring)env->CallObjectMethod(file2, getAbsolutePath);
	const char* app_dir = env->GetStringUTFChars(jpath, NULL);

	PXL_print << "test4\n";

	// chdir in the application cache directory
	PXL_print << "app_dir: " << app_dir << "\n";
	chdir(app_dir);
	env->ReleaseStringUTFChars(jpath, app_dir);

	PXL_print << "test8\n";

	//jobject assetManager = android_state->activity->assetManager;
	//AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	AAssetDir* assetDir = AAssetManager_openDir(android_state->activity->assetManager, "");
	const char* filename = (const char*)NULL;
	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
		AAsset* asset = AAssetManager_open(android_state->activity->assetManager, filename, AASSET_MODE_STREAMING);
		char buf[BUFSIZ];
		int nb_read = 0;
		FILE* out = fopen(filename, "w");
		while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
			fwrite(buf, nb_read, 1, out);
		fclose(out);
		AAsset_close(asset);
	}
	AAssetDir_close(assetDir);

	PXL_print << "test12\n";

	file.open(file_name.c_str(), std::ios::binary);

	if (!png_validate(file)) {
		PXL_show_exception("(" + file_name + ") is not a valid png (or it may not exist)", PXL_ERROR_INVALID_PNG);
		return NULL;
	}

	png_structp png_pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_pointer) {
		PXL_show_exception("Could not initialise png read struct for (" + file_name + ")", PXL_ERROR_INVALID_PNG);
	}

	png_infop info_pointer = png_create_info_struct(png_pointer);
	if (!info_pointer) {
		PXL_show_exception("Could not initialise png info struct for (" + file_name + ")", PXL_ERROR_INVALID_PNG);
		png_destroy_read_struct(&png_pointer, (png_infopp)0, (png_infopp)0);
	}

	png_set_read_fn(png_pointer, (png_voidp)&file, read_png);

	//set amount of bytes already read
	png_set_sig_bytes(png_pointer, PNG_SIG_SIZE);

	//read png header info into libpng
	png_read_info(png_pointer, info_pointer);

	png_uint_32 png_width = png_get_image_width(png_pointer, info_pointer);
	png_uint_32 png_height = png_get_image_height(png_pointer, info_pointer);

	png_uint_32 bit_depth = png_get_bit_depth(png_pointer, info_pointer);
	png_uint_32 channels = png_get_channels(png_pointer, info_pointer);
	png_uint_32 colour_type = png_get_color_type(png_pointer, info_pointer);

	PXL_Channel channel;
	if (colour_type == 0) channel = PXL_CHANNEL_ALPHA;
	if (colour_type == 2) channel = PXL_CHANNEL_RGB;
	if (colour_type == 3) PXL_show_exception("This colour type is not yet supported (palette index)", PXL_ERROR_INVALID_PNG);
	if (colour_type == 4) channel = PXL_CHANNEL_GRAY_ALPHA;
	if (colour_type == 6) channel = PXL_CHANNEL_RGBA;

	bitmap->create_bitmap(png_width, png_height, PXL_COLOR_BLACK, channel);

	const PXL_uint row_length = bitmap->get_width() * ((bit_depth * channels) / 8);
	png_bytep* row_pointers = new png_bytep[bitmap->get_height()];
	for (size_t y = 0; y < bitmap->get_height(); ++y) {
		row_pointers[y] = (png_bytep)(bitmap->get_pixels() + (y * row_length));
	}

	png_read_image(png_pointer, row_pointers);

	delete[] row_pointers;
	png_destroy_read_struct(&png_pointer, &info_pointer, (png_infopp)0);

	file.close();

	return bitmap;
}

void read_png(png_structp png_pointer, png_bytep data, png_size_t length) {
	file.read((char*)data, length);
}

bool png_validate(std::ifstream& file) {
	//allocate a buffer of 8 constant bytes
	png_byte png_sig[PNG_SIG_SIZE];

	//read 8 byte sig from stream into buffer
	file.read((char*)png_sig, PNG_SIG_SIZE);

	//return valid/invalid sig
	return (png_sig_cmp(png_sig, 0, PNG_SIG_SIZE) == 0);
}