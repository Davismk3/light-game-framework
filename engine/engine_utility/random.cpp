#include "random.hpp"

// Random Int: Returns an int ∈ [lower, upper]. The generator is seeded with 0, so the sequence is the same every run. lower <= upper. Note the order: upper comes first.
int randomInt(int upper, int lower) {
    static std::random_device rd;
    static std::mt19937 gen(0.0f);
    std::uniform_int_distribution<int> distrib(lower, upper);
    return distrib(gen);
}

// Random Float: Returns a float ∈ [lower, upper). The generator is seeded with 0, so the sequence is the same every run. lower < upper. Note the order: upper comes first.
float randomFloat(float upper, float lower) {
    static std::mt19937 gen(0.0f);
    std::uniform_real_distribution<float> distrib(lower, upper);
    return distrib(gen);
}

// Hash 11: Returns a deterministic pseudo-random float ∈ [-1.0f, 1.0f]. x, y, z, seed: any ints.
float hash11(int x, int y, int z, int seed) {
    std::uint32_t h =
        static_cast<std::uint32_t>(seed)
        ^ static_cast<std::uint32_t>(x) * 374761393u
        ^ static_cast<std::uint32_t>(y) * 668265263u
        ^ static_cast<std::uint32_t>(z) * 461393263u;
    h = (h ^ (h >> 16)) * 0x85ebca6b;
    h = (h ^ (h >> 13)) * 0xc2b2ae35;
    h ^= h >> 16;
    return (static_cast<float>(h & 0x7FFFFFFF) / 2147483647.0F) * 2.0F - 1.0F;
}

// Fade Helper: Returns the quintic fade 6t^5 - 15t^4 + 10t^3, which has zero slope and curvature at both ends. t ∈ [0.0f, 1.0f].
float fadeHelper(float t) {
    return t * t * t * (t * (t * 6.0F - 15.0F) + 10.0F);
}

// Lerp Helper: t ∈ [0.0f, 1.0f], where 0 returns a and 1 returns b.
float lerpHelper(float a, float b, float t) {
    return a + t * (b - a);
}

// Value Noise Single: Returns smooth noise ∈ [-1.0f, 1.0f], blended from hash11 at the cell's eight corners. x, y, z: any position, with one lattice cell per unit. seed: any int.
float valueNoiseSingle(float x, float y, float z, int seed) {
    int x0 = static_cast<int>(std::floor(x));
    int y0 = static_cast<int>(std::floor(y));
    int z0 = static_cast<int>(std::floor(z));

    int x1 = x0 + 1;
    int y1 = y0 + 1;
    int z1 = z0 + 1;

    float tx = x - static_cast<float>(x0);
    float ty = y - static_cast<float>(y0);
    float tz = z - static_cast<float>(z0);

    tx = fadeHelper(tx);
    ty = fadeHelper(ty);
    tz = fadeHelper(tz);

    float v000 = hash11(x0, y0, z0, seed);
    float v100 = hash11(x1, y0, z0, seed);
    float v010 = hash11(x0, y1, z0, seed);
    float v110 = hash11(x1, y1, z0, seed);
    float v001 = hash11(x0, y0, z1, seed);
    float v101 = hash11(x1, y0, z1, seed);
    float v011 = hash11(x0, y1, z1, seed);
    float v111 = hash11(x1, y1, z1, seed);

    float x00 = lerpHelper(v000, v100, tx);
    float x10 = lerpHelper(v010, v110, tx);
    float x01 = lerpHelper(v001, v101, tx);
    float x11 = lerpHelper(v011, v111, tx);
    float y0v = lerpHelper(x00, x10, ty);
    float y1v = lerpHelper(x01, x11, ty);

    return lerpHelper(y0v, y1v, tz);
}

// Value Noise Double: Returns the average of two valueNoiseSingle passes, the second rotated 33 degrees and offset, ∈ [-1.0f, 1.0f]. This hides the lattice's grid lines. x, y, z: any position, with one lattice cell per unit. seed: any int.
float valueNoiseDouble(float x, float y, float z, int seed) {
    float c = 0.8320502943378437;  // cos(33.0 degrees)
    float s = 0.5547001962252291;  // sin(33.0 degrees)

    float x2 = x * c - y * s + 19.1;
    float y2 = x * s + y * c - 7.7;
    float z2 = z + 13.7;

    float n1 = valueNoiseSingle(x, y, z, seed);
    float n2 = valueNoiseSingle(x2, y2, z2, seed + 1013);

    return 0.5 * (n1 + n2);
}

// Brownian Noise: Returns the sum of octaves of valueNoiseDouble, normalized to [-1.0f, 1.0f]. Returns 0 if octaves <= 0. octaves >= 1. persistence ∈ (0.0f, 1.0f], each octave's amplitude relative to the last. lacunarity > 1.0f, each octave's frequency relative to the last. scale > 0.0f, the first octave's frequency.
float brownianNoise(
    float x,
    float y,
    float z,
    int seed,
    int octaves,
    float persistence,
    float lacunarity,
    float scale
) {
    float total = 0.0F;
    float amplitude = 1.0F;
    float frequency = scale;
    float max_value = 0.0F;

    for (int i = 0; i < octaves; i++) {
        float nx = static_cast<float>(x) * frequency;
        float ny = static_cast<float>(y) * frequency;
        float nz = static_cast<float>(z) * frequency;

        total += valueNoiseDouble(nx, ny, nz, seed + i * 1013) * amplitude;
        max_value += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    if (max_value == 0.0F) {
        return 0.0F;
    }
    return total / max_value;
}
