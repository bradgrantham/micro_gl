#ifdef SUBPIXEL_COORDS
static const int subpixel_coordinate_scale = 1 << SUBPIXEL_FRACTIONAL_BITS;
#endif

struct screen_vertex
{
    short x, y;
    // float z;
    short r, g, b;
    // vec4f texcoord;
};

void clear_color_buffer(float r, float g, float b);
void rasterize_triangle(const screen_vertex* sv0, const screen_vertex* sv1, const screen_vertex* sv2);
void rasterize_line(const screen_vertex* sv0, const screen_vertex* sv1);
