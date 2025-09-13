#version 460 core

layout (location = 0) in vec2 vtexture_coords;

layout (location = 0) out vec4 frag_color;

uniform sampler2D screen_texture;
uniform int mode;
uniform float time;  // uniform for effect animation (procedural)

// safely sample the screen texture, clamping UV coordinates to [0,1] 
// to prevent reading outside the texture boundaries.
vec4 safe_tex(vec2 uv) { return texture(screen_texture, clamp(uv, 0.0, 1.0)); }
// returns the size of one texel (pixel) in UV space for the current texture.
// Useful for convolution kernels, offsets, or procedural effects that depend
// on neighboring pixels.
vec2 texel_size()      { return 1.0 / vec2(textureSize(screen_texture, 0)); }

// a spatial kernel is just a convolution matrix of weights applied to a pixel and its neighbors.
void use_kernel(float kernel[9]) {
    vec2 t = texel_size();
    // This could be sent to avoid recomputing offsets
    vec2 offsets[9] = vec2[](
        vec2(-t.x,  t.y), vec2(0.0,  t.y), vec2( t.x,  t.y),
        vec2(-t.x,  0.0), vec2(0.0,  0.0), vec2( t.x,  0.0),
        vec2(-t.x, -t.y), vec2(0.0, -t.y), vec2( t.x, -t.y)
    );

    vec3 color = vec3(0.0);
    for (int i = 0; i < 9; ++i) {
        color += safe_tex(vtexture_coords + offsets[i]).rgb * kernel[i];
    }
    frag_color = vec4(color, 1.0);
}

// Returns the convolution of a 3x3 kernel applied to luminance
float use_kernel_luma(float kernel[9]) {
    vec2 t = texel_size();
    // This could be sent to avoid recomputing offsets
    vec2 offsets[9] = vec2[](
        vec2(-t.x,  t.y), vec2(0.0,  t.y), vec2( t.x,  t.y),
        vec2(-t.x,  0.0), vec2(0.0,  0.0), vec2( t.x,  0.0),
        vec2(-t.x, -t.y), vec2(0.0, -t.y), vec2( t.x, -t.y)
    );

    float sum = 0.0;
    for (int i = 0; i < 9; ++i) {
        vec3 rgb = safe_tex(vtexture_coords + offsets[i]).rgb;
        float luma = dot(rgb, vec3(0.2126, 0.7152, 0.0722));
        sum += luma * kernel[i];
    }

    return sum;
}

void sobel_mode() {
    float gx[9] = float[](
        -1.0, 0.0, 1.0,
        -2.0, 0.0, 2.0,
        -1.0, 0.0, 1.0
    );
    float gy[9] = float[](
         1.0,  2.0,  1.0,
         0.0,  0.0,  0.0,
        -1.0, -2.0, -1.0
    );

    float sx = use_kernel_luma(gx);
    float sy = use_kernel_luma(gy);

    float edge = length(vec2(sx, sy));
    frag_color = vec4(vec3(edge), 1.0);
}

// edge detection on luma
void laplacian_mode() {
    float laplacian_kernel[9] = float[](
        0.0,  1.0, 0.0,
        1.0, -4.0, 1.0,
        0.0,  1.0, 0.0
    );

    float edge = use_kernel_luma(laplacian_kernel);
    frag_color = vec4(vec3(edge), 1.0);
}

// gaussian blur
void blur_mode() {
    float blur_kernel[9] = float[](
        1.f / 16, 2.f / 16, 1.f / 16,
        2.f / 16, 4.f / 16, 2.f / 16,
        1.f / 16, 2.f / 16, 1.f / 16  
    );
    use_kernel(blur_kernel);
}

// laplacian variant edge detection on rgb
void edge_mode() {
    float edge_kernel[9] = float[](
        -1.f,  -1.f, -1.f,
        -1.f, 8.f, -1.f,
        -1.f,  -1.f, -1.f  
    );
    use_kernel(edge_kernel);
}

void sharpen_mode() {
    float sharpen_kernel[9] = float[](
        0.f, -1.f, 0.f,
        -1.f, 5.f, -1.f,
        0.f, -1.f, 0.f  
    );
    use_kernel(sharpen_kernel);
}

void emboss_mode() {
    float emboss_kernel[9] = float[](
        -2.f, -1.f, 0.f,
        -1.f, 1.f, 1.f,
        0.f, 1.f, 2.f  
    );
    use_kernel(emboss_kernel);
}

void normal_mode() {
    frag_color = safe_tex(vtexture_coords);
}

void inverse_mode() {
    frag_color = vec4(vec3(1.f - safe_tex(vtexture_coords)), 1.f);
}

void red_inverse_mode() {
    vec4 color = safe_tex(vtexture_coords);
    frag_color = vec4(1.0 - color.r, color.g, color.b, 1.0);
}

void green_inverse_mode() {
    vec4 color = safe_tex(vtexture_coords);
    frag_color = vec4(color.r, 1.0 - color.g, color.b, 1.0);
}

void blue_inverse_mode() {
    vec4 color = safe_tex(vtexture_coords);
    frag_color = vec4(color.r, color.g, 1.0 - color.b, 1.0);
}

void greyscale_mode() {
    frag_color = safe_tex(vtexture_coords);
    float average = 0.2126f * frag_color.r + 0.7152f * frag_color.g + 0.0722f * frag_color.b;
    frag_color = vec4(average, average, average, 1.f);
}

void nightvision_mode() {
    greyscale_mode();
    frag_color = vec4(frag_color.r, frag_color.g * 3, frag_color.b, 1.f);
}

void sepia_mode() {
    vec3 color = safe_tex(vtexture_coords).rgb;
    float r = dot(color, vec3(0.393, 0.769, 0.189));
    float g = dot(color, vec3(0.349, 0.686, 0.168));
    float b = dot(color, vec3(0.272, 0.534, 0.131));
    frag_color = vec4(r, g, b, 1.0);
}

void comic_mode() {
    vec3 color = safe_tex(vtexture_coords).rgb;
    float levels = 4.0; // reduce for more cartoonish effect
    frag_color = vec4(floor(color * levels) / levels, 1.0);
}

void brightpass_mode() {
    vec4 color = safe_tex(vtexture_coords);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    // threshold filter
    frag_color = brightness > 0.7 ? color : vec4(0.0, 0.0, 0.0, 1.0);
}

void thermal_mode() {
    vec4 color = safe_tex(vtexture_coords);
    float intensity = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    // simple thermal gradient
    vec3 thermal = vec3(0.0);
    if (intensity < 0.33)
        thermal = mix(vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0), intensity / 0.33);
    else if (intensity < 0.66)
        thermal = mix(vec3(0.0, 1.0, 1.0), vec3(1.0, 1.0, 0.0), (intensity - 0.33) / 0.33);
    else
        thermal = mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), (intensity - 0.66) / 0.34);

    frag_color = vec4(thermal, 1.0);
}

void cyanotype_mode() {
    vec4 color = safe_tex(vtexture_coords);
    float grey = dot(color.rgb, vec3(0.3, 0.59, 0.11));
    frag_color = vec4(0.0, grey * 0.7, grey, 1.0);
}

void heatmap_mode() {
    vec4 color = safe_tex(vtexture_coords);
    float intensity = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    frag_color = vec4(intensity, intensity * 0.5, 1.0 - intensity, 1.0);
}

void crt_mode() {
    vec4 color = safe_tex(vtexture_coords);
    float scanline = sin(color.y * 800.0 * 3.14159); // adjust freq
    scanline = (scanline * 0.5 + 0.5) * 0.9 + 0.1;
    frag_color = vec4(color.rgb * scanline, 1.0);
}

void chromatic_aberration_mode() {
    float offset = 0.002; // tweak for strength
    float r = texture(screen_texture, vtexture_coords + vec2(offset, 0.0)).r;
    float g = texture(screen_texture, vtexture_coords).g;
    float b = texture(screen_texture, vtexture_coords - vec2(offset, 0.0)).b;
    frag_color = vec4(r, g, b, 1.0);
}

void swirl_mode() {
    vec2 uv = vtexture_coords - 0.5;
    float angle = 2.0 * length(uv); // swirl strength
    float s = sin(angle);
    float c = cos(angle);
    mat2 rot = mat2(c, -s, s, c);
    vec2 new_uv = rot * uv + 0.5;
    frag_color = safe_tex(new_uv);
}

void horizontal_glitch_mode() { // broken camera
    float slice = floor(vtexture_coords.y * 20.0); // 20 slices
    float shift = fract(sin(slice * 123.4) * 43758.5) * 0.05;
    vec2 new_uv = vtexture_coords;
    new_uv.x += shift;
    frag_color = safe_tex(new_uv);
}

void wave_mode() {
    vec2 uv = vtexture_coords;
    uv.x += 0.02 * sin(uv.y * 30.0 + time);
    uv.y += 0.02 * cos(uv.x * 30.0 + time);
    frag_color = safe_tex(uv);
}

void kaleidoscope_mode() {
    vec2 uv = vtexture_coords * 2.0 - 1.0; // [-1,1] range
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    angle = mod(angle, 3.14159 / 3.0); // 6-way mirror
    vec2 new_uv = vec2(cos(angle), sin(angle)) * radius * 0.5 + 0.5;
    frag_color = safe_tex(new_uv);
}

void datamosh_mode() { // lines in broken TV
    vec4 color = safe_tex(vtexture_coords);
    if (fract(sin(color.y * 100.0) * 43758.5453) < 0.05) {
        color.g = 0.0; // drop green sometimes
    }
    frag_color = color;
}

void barrel_distortion_mode() { // tv screen mode
    vec2 uv = vtexture_coords * 2.0 - 1.0; // [-1,1] range
    float radius = length(uv);
    float strength = 0.5; // Adjust for more or less distortion
    uv += uv * radius * strength;
    uv = uv * 0.5 + 0.5; // back to [0,1]
    frag_color = safe_tex(uv);
}

void ripple_mode() {
    vec2 uv = vtexture_coords - 0.5;
    float dist = length(uv);
    uv += uv / dist * 0.03 * sin(dist * 50.0 - time * 5.0);
    frag_color = safe_tex(uv + 0.5);
}

void plasma_mode() {
    vec4 color = safe_tex(vtexture_coords);
    float shift = sin(color.x * 20.0 + time) + cos(color.y * 20.0 - time);
    color.rgb *= vec3(0.5 + 0.5 * sin(shift + vec3(0, 2, 4)));
    frag_color = color;
}

void pixelate_mode() {
    vec2 uv = vtexture_coords;
    float size = 0.005; // pixel block size
    uv = floor(uv / size) * size;
    frag_color = safe_tex(uv);
}

void mirror_mode() {
    vec2 uv = vtexture_coords * 2.0;
    uv = abs(fract(uv) - 0.5) * 2.0;
    frag_color = texture(screen_texture, uv);
}

void one_bit_shading_mode() {
    vec3 color = texture(screen_texture, vtexture_coords).rgb;
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    // Threshold to produce pure black or white
    float bw = luma > 0.5 ? 1.0 : 0.0;
    frag_color = vec4(vec3(bw), 1.0);
}

// 1-bit dithered shading (Obra Dinn style)
void one_bit_dithered_mode() {
    // 4x4 Bayer matrix, normalized [0,1]
    float bayer[16] = float[](
        0.0/16.0,  8.0/16.0,  2.0/16.0, 10.0/16.0,
       12.0/16.0,  4.0/16.0, 14.0/16.0,  6.0/16.0,
        3.0/16.0, 11.0/16.0,  1.0/16.0,  9.0/16.0,
       15.0/16.0,  7.0/16.0, 13.0/16.0,  5.0/16.0
    );

    // Sample the current pixel
    vec3 color = texture(screen_texture, vtexture_coords).rgb;
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));

    // Determine pixel position in Bayer matrix
    ivec2 pix = ivec2(mod(gl_FragCoord.xy, 4.0));
    int index = pix.y * 4 + pix.x;

    // Compare luminance to Bayer threshold
    float bw = luma > bayer[index] ? 1.0 : 0.0;

    frag_color = vec4(vec3(bw), 1.0);
}

void ps1_mode() {
    vec2 uv = vtexture_coords;
    float bit_depth = 5.0; // ps1 bit depth

    // pixelate to simulate blocky rendering
    vec2 block = texel_size() * 4.0; // 4x4 blocks
    uv = floor(uv / block) * block;

    vec3 color = safe_tex(uv).rgb;

    // quantize each channel to simulate low bit depth
    float levels = pow(2.0, float(bit_depth)) - 1.0;
    color = floor(color * levels) / levels;

    frag_color = vec4(color, 1.0);
}

void cell_shader_mode() {
    float levels = 3.5; // number of color bands
    vec3 color = texture(screen_texture, vtexture_coords).rgb;
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    // Quantize luminance into discrete bands
    float step = 1.0 / float(levels);
    float quantized = floor(luma / step) * step;
    // Scale color by quantized luminance to maintain hue
    vec3 toon_color = color * (quantized / max(luma, 0.0001));
    frag_color = vec4(toon_color, 1.0);
}

void main() {
    switch (mode) {
        case 0:
            normal_mode();
            break;
        case 1:
            inverse_mode();
            break;
        case 2:
            greyscale_mode();
            break;
        case 3:
            blur_mode();
            break;
        case 4:
            edge_mode();
            break;
        case 5:
            sharpen_mode();
            break;
        case 6:
            nightvision_mode();
            break;
        case 7:
            emboss_mode();
            break;
        case 8:
            sepia_mode();
            break;
        case 9:
            comic_mode();
            break;
        case 10:
            one_bit_shading_mode();
            break;
        case 11:
            one_bit_dithered_mode();
            break;
        case 12:
            ps1_mode();
            break;
        case 13:
            cell_shader_mode();
            break;
        case 14:
            thermal_mode();
            break;
        case 15:
            cyanotype_mode();
            break;
        case 16:
            heatmap_mode();
            break;
        case 17:
            crt_mode();
            break;
        case 18:
            chromatic_aberration_mode();
            break;
        case 19:
            swirl_mode();
            break;
        case 20:
            horizontal_glitch_mode();
            break;
        case 21:
            wave_mode();
            break;
        case 22:
            kaleidoscope_mode();
            break;
        case 23:
            datamosh_mode();
            break;
        case 24:
            barrel_distortion_mode();
            break;
        case 25:
            ripple_mode();
            break;
        case 26:
            plasma_mode();
            break;
        case 27:
            pixelate_mode();
            break;
        case 28:
            mirror_mode();
            break;
    }
}