#include "pch.h"
#include "MeshGenerator.h"

void GenerateCylinderMesh(
    std::vector<int>& counts,
    std::vector<int>& indices,
    std::vector<float3> &points,
    std::vector<float2> &uv,
    float radius, float height,
    int cseg, int hseg)
{
    const int num_vertices = cseg * hseg;

    // vertices
    points.resize(num_vertices);
    uv.resize(points.size());
    for (int ih = 0; ih < hseg; ++ih)
    {
        float y = (float(ih) / float(hseg - 1)) * height;
        for (int ic = 0; ic < cseg; ++ic)
        {
            int i = cseg * ih + ic;
            float ang = ((360.0f / cseg) * ic) * Deg2Rad;
            float r = radius;

            float3 pos{ std::cos(ang) * r, y, std::sin(ang) * r };
            float2 t{ float(ic) / float(cseg - 1), float(ih) / float(hseg - 1), };

            points[i] = pos;
            uv[i] = t;
        }
    }

    // topology
    const int num_faces = cseg * (hseg - 1);
    const int num_indices = num_faces * 4;

    counts.resize(num_faces, 4);
    indices.resize(num_indices);
    for (int ih = 0; ih < hseg - 1; ++ih)
    {
        for (int ic = 0; ic < cseg; ++ic)
        {
            auto *dst = &indices[(ih * cseg + ic) * 4];
            dst[0] = cseg * ih + ic;
            dst[1] = cseg * (ih + 1) + ic;
            dst[2] = cseg * (ih + 1) + ((ic + 1) % cseg);
            dst[3] = cseg * ih + ((ic + 1) % cseg);
        }
    }
}

static inline int GetMiddlePoint(int p1, int p2, std::vector<float3>& vertices, std::map<int64_t, int>& cache, float radius)
{
    // first check if we have it already
    bool firstIsSmaller = p1 < p2;
    int64_t smallerIndex = firstIsSmaller ? p1 : p2;
    int64_t greaterIndex = firstIsSmaller ? p2 : p1;
    int64_t key = (smallerIndex << 32) + greaterIndex;

    {
        auto it = cache.find(key);
        if (it != cache.end())
        {
            return it->second;
        }
    }

    // not in cache, calculate it
    const auto& point1 = vertices[p1];
    const auto& point2 = vertices[p2];
    float3 middle{
        (point1.x + point2.x) * 0.5f,
        (point1.y + point2.y) * 0.5f,
        (point1.z + point2.z) * 0.5f
    };

    // add vertex makes sure point is on unit sphere
    size_t i = vertices.size();
    vertices.push_back(normalize(middle) * radius);

    // store it, return index
    cache[key] = i;
    return i;
}

void GenerateIcoSphereMesh(
    std::vector<int>& counts,
    std::vector<int>& indices,
    std::vector<float3>& points,
    std::vector<float2>& uv,
    float radius,
    int iteration)
{
    float t = (1.0f + std::sqrt(5.0f)) * 0.5f;

    points = {
        normalize(float3{-1.0f,    t, 0.0f }) * radius,
        normalize(float3{ 1.0f,    t, 0.0f }) * radius,
        normalize(float3{-1.0f,   -t, 0.0f }) * radius,
        normalize(float3{ 1.0f,   -t, 0.0f }) * radius,
        normalize(float3{ 0.0f, -1.0f,    t }) * radius,
        normalize(float3{ 0.0f, 1.0f,    t }) * radius,
        normalize(float3{ 0.0f, -1.0f,   -t }) * radius,
        normalize(float3{ 0.0f, 1.0f,   -t }) * radius,
        normalize(float3{    t, 0.0f, -1.0f }) * radius,
        normalize(float3{    t, 0.0f, 1.0f }) * radius,
        normalize(float3{   -t, 0.0f, -1.0f }) * radius,
        normalize(float3{   -t, 0.0f, 1.0f }) * radius,
    };

    indices = {
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,

        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,

        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,

        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1,
    };

    std::map<int64_t, int> cache;
    for (int it = 0; it < iteration; it++)
    {
        std::vector<int> indices2;
        size_t n = indices.size();
        for (size_t fi = 0; fi < n; fi += 3)
        {
            int i1 = indices[fi + 0];
            int i2 = indices[fi + 1];
            int i3 = indices[fi + 2];
            int a = GetMiddlePoint(i1, i2, points, cache, radius);
            int b = GetMiddlePoint(i2, i3, points, cache, radius);
            int c = GetMiddlePoint(i3, i1, points, cache, radius);

            int addition[]{
                i1, a, c,
                i2, b, a,
                i3, c, b,
                a, b, c,
            };
            indices2.insert(indices2.end(), std::begin(addition), std::end(addition));
        }
        indices = indices2;
    }

    counts.resize(indices.size() / 3);
    for (int& c : counts)
    {
        c = 3;
    }
}
