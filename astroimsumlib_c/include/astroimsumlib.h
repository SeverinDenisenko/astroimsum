#ifndef ASTROIMSUMLIB_H
#define ASTROIMSUMLIB_H

struct astroimsum;
struct astroimsum_frame;

/*
 * Create and destroy astroimsum handle
 */
int astroimsum_init(struct astroimsum** handle_out);
int astroimsum_destroy(struct astroimsum* handle_in);

/*
 * Routine for creation, deletion and access to frames
 */
int astroimsum_create_frame(
    struct astroimsum* handle_in,
    struct astroimsum_frame** frame_out,
    char* name_in);

int astroimsum_access_frame(
    struct astroimsum* handle_in,
    struct astroimsum_frame* frame_in,
    int* width_out,
    int* height_out,
    float** data_out);

int astroimsum_destroy_frame(
    struct astroimsum* handle_in, struct astroimsum_frame* frame_in);

int astroimsum_write_frame(
    struct astroimsum* handle_in,
    struct astroimsum_frame* frame_in,
    char* name_in);

/*
 * Main routine for frame summation
 * Result is placed in base frame
 */
int astroimsum_set_base_frame(
    struct astroimsum* handle_in, struct astroimsum_frame* frame_in);
int astroimsum_add_frame(
    struct astroimsum* handle_in, struct astroimsum_frame* frame_in);

#endif // ASTROIMSUMLIB_H
