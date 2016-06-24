#include "system/ImageIO.h"

#include <fstream>
#include <png.h>
#include <string>

#if defined(PLATFORM_ANDROID)
#include <jni.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#endif

#include "system/Exception.h"
#include "system/Debug.h"
#include "system/android/AndroidWindow.h"
#include "system/IO.h"

namespace pxl { namespace system {
    
    const char* cache_dir;

    #if defined(PLATFORM_ANDROID)
    void write_asset_dir(std::string dir_name) {
	    //write files to cache
	    AAssetDir* asset_dir = AAssetManager_openDir(android_state->activity->assetManager, dir_name.c_str());
	    const char* file_name;
	    while ((file_name = AAssetDir_getNextFileName(asset_dir)) != NULL) {
		    AAsset* asset = AAssetManager_open(android_state->activity->assetManager, file_name, AASSET_MODE_STREAMING);

		    FILE* file = fopen(file_name, "w");
		    off_t size = AAsset_getLength(asset);
		    char* buffer = new char[size];

		    AAsset_read(asset, buffer, size);
		    fwrite(buffer, size, 1, file);

		    fclose(file);
		    AAsset_close(asset);

		    print << "wrote " << file_name << " to asset dir (" << size << " bytes)\n";
	    }
	    AAssetDir_close(asset_dir);
    }
    #endif

    void init_assets() {
	    #if defined(PLATFORM_ANDROID)
		    JNIEnv* env = android_state->activity->env;
		    JavaVM* vm = android_state->activity->vm;
		    vm->AttachCurrentThread(&env, NULL);

		    jclass activity_class = env->GetObjectClass(android_state->activity->clazz);

		    //get path to cache dir
		    jmethodID cache_dir_func = env->GetMethodID(activity_class, "getCacheDir", "()Ljava/io/File;");
		    jobject f = env->CallObjectMethod(android_state->activity->clazz, cache_dir_func);
		    jclass f_class = env->FindClass("java/io/File");
		    jmethodID full_path_func = env->GetMethodID(f_class, "getAbsolutePath", "()Ljava/lang/String;");
		    jstring j_cache_path = (jstring)env->CallObjectMethod(f, full_path_func);

		    //get full cache dir and print it
		    cache_dir = env->GetStringUTFChars(j_cache_path, NULL);
		    print << "cache assets dir: " << cache_dir << "\n";

		    //free jcachepath
		    //todo: delete j_cache_path properly without breaking cache_dir
		    //delete j_cache_path;

		    chdir(cache_dir);
		    std::string temp = cache_dir;
		    temp += "/assets";
		    rmdir(temp.c_str());
		    if (mkdir(temp.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
			    print << "could not create dir in (" << cache_dir << "/assets) error: " << strerror(errno) << "\n";
		    }
		    chdir(temp.c_str());

		    write_asset_dir("");

		    cache_dir = append_char(cache_dir, "/");

		    chdir(cache_dir);
	    #endif
    }

    #define PNG_SIG_SIZE 8

    bool png_validate(std::ifstream& file);
    void read_png(png_structp read, png_bytep data, png_size_t length);

    std::ifstream file;

    graphics::Bitmap* load_png(std::string file_name, graphics::Bitmap* bitmap) {
	    if (bitmap == NULL) return NULL;

	    #if defined(PLATFORM_ANDROID)
		    chdir(cache_dir);
	    #endif

	    file.open(file_name.c_str(), std::ios::binary);

	    if (!png_validate(file)) {
		    show_exception("(" + file_name + ") is not a valid png (or it may not exist)", ERROR_INVALID_PNG);
		    return NULL;
	    }

	    png_structp png_pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	    if (!png_pointer) {
		    show_exception("Could not initialise png read struct for (" + file_name + ")", ERROR_INVALID_PNG);
	    }

	    png_infop info_pointer = png_create_info_struct(png_pointer);
	    if (!info_pointer) {
		    show_exception("Could not initialise png info struct for (" + file_name + ")", ERROR_INVALID_PNG);
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

	    graphics::Channel channel;
	    if (colour_type == 0) channel = graphics::CHANNEL_ALPHA;
	    if (colour_type == 2) channel = graphics::CHANNEL_RGB;
	    if (colour_type == 3) show_exception("This colour type is not yet supported (palette index)", ERROR_INVALID_PNG);
	    if (colour_type == 4) channel = graphics::CHANNEL_GRAY_ALPHA;
	    if (colour_type == 6) channel = graphics::CHANNEL_RGBA;

	    bitmap->create_bitmap(png_width, png_height, graphics::COLOR_BLACK, channel);

	    const uint32 row_length = bitmap->get_width() * ((bit_depth * channels) / 8);
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
	    file.read((int8*)data, length);
    }

    bool png_validate(std::ifstream& file) {
	    //allocate a buffer of 8 constant bytes
	    png_byte png_sig[PNG_SIG_SIZE];

	    //read 8 byte sig from stream into buffer
	    file.read((char*)png_sig, PNG_SIG_SIZE);

	    //return valid/invalid sig
	    return (png_sig_cmp(png_sig, 0, PNG_SIG_SIZE) == 0);
    }
}};