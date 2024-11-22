#ifndef ASTROIMSUMLIB_H
#define ASTROIMSUMLIB_H

struct astroimsum;
struct astroimsum_frame;

/*
 * Create and destroy astroimsum handle
 */
int astroimsum_init(struct astroimsum* handle);
int astroimsum_destroy(struct astroimsum* handle);

/*
 * Routine for creation, deletion and access to frames
 * `float* data` must exists all liftime of the frame
 */
int astroimsum_create_frame(struct astroimsum_frame** frame_out, char* name_in);
int astroimsum_access_frame(struct astroimsum_frame* frame_in, int* width_out, int* height_out, float** data_out);
int astroimsum_destroy_frame(struct astroimsum_frame* frame_in);
int astroimsum_write_frame(struct astroimsum_frame* frame_in, char* name_in);

/*
 * Main routine for frame summation
 */
int astroimsum_add_frames(struct astroimsum_frame* add_to, struct astroimsum_frame* add_what);

#endif // ASTROIMSUMLIB_H
