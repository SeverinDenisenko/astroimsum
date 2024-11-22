#include <exception>

#include "frame.hpp"
#include "frame_io.hpp"
#include "sumation.hpp"
#include "types.hpp"

using namespace astro;

struct astroimsum { };

struct astroimsum_frame {
    frame frame;
    uptr<iframeloader> frameloader;
};

extern "C" {

#include "astroimsumlib.h"

int astroimsum_init(struct astroimsum** handle)
{
    int res = 0;

    try {
        *handle = new astroimsum();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        res = 1;
    }

    return res;
}

int astroimsum_destroy(struct astroimsum* handle)
{
    int res = 0;

    try {
        delete handle;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        res = 1;
    }

    return res;
}

int astroimsum_create_frame(
    struct astroimsum* handle_in [[maybe_unused]],
    struct astroimsum_frame** frame_out,
    char* name_in)
{
    int res = 0;

    try {
        *frame_out = new astroimsum_frame();

        string_t name(name_in);
        array_t<string_t> names { name };
        (*frame_out)->frameloader = make_uptr<batch_frameloader>(names);
        (*frame_out)->frame       = (*frame_out)->frameloader->get_frame();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        res = 1;
    }

    return res;
}

int astroimsum_access_frame(
    struct astroimsum* handle_in [[maybe_unused]],
    struct astroimsum_frame* frame_in,
    int* width_out,
    int* height_out,
    unsigned long** data_out)
{
    int res = 0;

    try {
        *width_out  = frame_in->frame.x_size();
        *height_out = frame_in->frame.y_size();
        *data_out   = frame_in->frame.data().data();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        res = 1;
    }

    return res;
}

int astroimsum_destroy_frame(
    struct astroimsum* handle_in [[maybe_unused]],
    struct astroimsum_frame* frame_in)
{
    int res = 0;

    try {
        frame_in->frameloader = nullptr;
        frame_in->frame       = frame();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        res = 1;
    }

    return res;
}

int astroimsum_write_frame(
    struct astroimsum* handle_in [[maybe_unused]],
    struct astroimsum_frame* frame_in,
    char* name_in)
{
    int res = 0;

    try {
        string_t name(name_in);
        sptr<iframesaver> framesaver = make_sptr<casual_framesaver>(name);
        framesaver->save(frame_in->frame);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        res = 1;
    }

    return res;
}

int astroimsum_add_frames(
    struct astroimsum* handle_in [[maybe_unused]],
    struct astroimsum_frame* add_to,
    struct astroimsum_frame* add_what)
{
    int res = 0;

    try {
        sptr<isumattor> sumattor
            = make_sptr<basic_star_sumattor>(add_to->frame);
        sumattor->sum(add_what->frame);
        add_to->frame = sumattor->result();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        res = 1;
    }

    return res;
}
}
