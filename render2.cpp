#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 800;
const int FPS = 60;
const double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067;

const Uint8 BG_R = 0x0, BG_G = 0x0, BG_B = 0x0;    // #000000
const Uint8 FG_R = 0x50, FG_G = 0xFF, FG_B = 0x50; // #50ff50

struct Vec3d
{
    double x, y, z;
};
struct Vec2d
{
    double x, y;
};

Vec3d rotate_xz(Vec3d v, double angle)
{
    double c = std::cos(angle);
    double s = std::sin(angle);
    return {v.x * c - v.z * s, v.y, v.x * s + v.z * c};
}

Vec3d rotate_xy(Vec3d v, double angle)
{
    double c = std::cos(angle);
    double s = std::sin(angle);
    return {v.x * c - v.y * s, v.x * s + v.y * c, v.z};
}

Vec3d rotate_yz(Vec3d v, double angle)
{
    double c = std::cos(angle);
    double s = std::sin(angle);
    return {v.x, v.y * c - v.z * s, v.y * s + v.z * c};
}

Vec3d translate_z(Vec3d v, double dz)
{
    v.z += dz;
    return v;
}

Vec2d project(Vec3d v)
{
    if (v.z <= 0.0001)
        v.z = 0.0001;
    return {v.x / v.z, v.y / v.z};
}

SDL_Point screen(Vec2d p)
{
    int x = static_cast<int>((p.x + 1.0) * 0.5 * WIDTH);
    int y = static_cast<int>((1.0 - (p.y + 1.0) * 0.5) * HEIGHT);
    return {x, y};
}

void draw_wireframe(
    SDL_Renderer *ctx,
    const std::vector<Vec3d> &vs,
    const std::vector<std::vector<int>> &fs,
    double dt,
    double dz,
    double ax, double ay, double az,
    double &rx, double &ry, double &rz)
{
    for (const auto &f : fs)
    {
        for (size_t i = 0; i < f.size(); ++i)
        {
            size_t j = (i + 1) % f.size();

            Vec3d a = vs[f[i]];
            Vec3d b = vs[f[j]];

            rx += ax * dt;
            ry += ay * dt;
            rz += az * dt;

            a = rotate_xy(a, rz);
            a = rotate_yz(a, rx);
            a = rotate_xz(a, ry);
            a = translate_z(a, dz);

            b = rotate_xy(b, rz);
            b = rotate_yz(b, rx);
            b = rotate_xz(b, ry);
            b = translate_z(b, dz);

            Vec2d pa = project(a);
            Vec2d pb = project(b);

            SDL_Point sa = screen(pa);
            SDL_Point sb = screen(pb);

            SDL_RenderDrawLine(ctx, sa.x, sa.y, sb.x, sb.y);
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *game = SDL_CreateWindow(
        "Tsoding's Renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0);
    SDL_Renderer *ctx = SDL_CreateRenderer(game, -1, 0);

    std::vector<Vec3d> vs = {
        {0.25, 0.25, 0.25},
        {-0.25, 0.25, 0.25},
        {-0.25, -0.25, 0.25},
        {0.25, -0.25, 0.25},

        {0.25, 0.25, -0.25},
        {-0.25, 0.25, -0.25},
        {-0.25, -0.25, -0.25},
        {0.25, -0.25, -0.25},
    };

    std::vector<std::vector<int>> fs = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
    };

    double dz = 1.0;
    double ax = ((std::rand() % 200) - 100) * 0.0025;
    double ay = ((std::rand() % 200) - 100) * 0.0025;
    double az = ((std::rand() % 200) - 100) * 0.0025;

    double rx = 0.0, ry = 0.0, rz = 0.0;
    bool running = true;

    while (running)
    {
        Uint32 start = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                running = false;

        double dt = 1.0 / FPS;

        SDL_SetRenderDrawColor(ctx, BG_R, BG_G, BG_B, 255);

        SDL_RenderClear(ctx);
        SDL_SetRenderDrawColor(ctx, FG_R, FG_G, FG_B, 255);

        draw_wireframe(ctx, vs, fs, dt, dz, ax, ay, az, rx, ry, rz);

        SDL_RenderPresent(ctx);

        Uint32 frameTime = SDL_GetTicks() - start;
        if (frameTime < (Uint32)(1000 / FPS))
            SDL_Delay((Uint32)(1000 / FPS) - frameTime);
    }

    SDL_Quit();
    return 0;
}
