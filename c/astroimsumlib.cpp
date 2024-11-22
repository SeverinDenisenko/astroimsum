#include "frame.hpp"
#include "sumation.hpp"

struct astroimsumlib { };
struct astroimsum_frame { };

extern "C" {

int astroimsum_init(struct astroimsum* handle)
{
    return 0;
}

int astroimsum_destroy(struct astroimsum* handle)
{
    return 0;
}

int astroimsum_create_frame(struct astroimsum_frame** frame_out, char* name_in)
{
    return 0;
}

int astroimsum_access_frame(
    struct astroimsum_frame* frame_in,
    int* width_out,
    int* height_out,
    float** data_out)
{
    return 0;
}

int astroimsum_destroy_frame(struct astroimsum_frame* frame_in)
{
    return 0;
}

int astroimsum_write_frame(struct astroimsum_frame* frame_in, char* name_in)
{
    return 0;
}

int astroimsum_add_frames(
    struct astroimsum_frame* add_to, struct astroimsum_frame* add_what)
{
    return 0;
}
}
