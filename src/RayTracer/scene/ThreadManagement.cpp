/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** thread_manage
*/

#include "Scene.hpp"
#include <algorithm>
#include <iomanip>
#include <thread>


namespace RayTracer {

void Scene::render(std::ostream &output) const
{
    if (width == 0 || height == 0)
        return;
    int num_threads = std::max(1, static_cast<int>(std::thread::hardware_concurrency()) - 1); //leave one core free
    num_threads = std::min(num_threads, static_cast<int>(width)); //cap to width: more threads than columns would cause out-of-bounds writes
    std::cerr << "Rendering " << width << "x" << height << " image using " << num_threads << " threads...\n";

    std::vector<std::vector<std::array<uint8_t, 3>>> pixels(height, std::vector<std::array<uint8_t, 3>>(width)); //2D vector to store pixel colors
    std::atomic<int> columns_rendered{0};
    std::mutex cerr_mutex; //we use a mutex to protect cerr output because multiple threads will be writing to cerr at the same time, and we want to avoid interleaving of output which can make it unreadable. By locking the mutex before writing to cerr and unlocking it afterwards, we ensure that only one thread writes to cerr at a time, keeping the output clean and coherent.
    int cols_per_thread = (static_cast<int>(width) + num_threads - 1) / num_threads; //calculate how many columns each thread should render (rounding up)

    auto t_start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int t = 0; t < num_threads; t++) {
        int start_col = t * cols_per_thread;
        int end_col = (t == num_threads - 1) ? static_cast<int>(width) : start_col + cols_per_thread; //last thread takes any remaining columns
        threads.emplace_back(&Scene::renderChunk, this, std::ref(pixels), std::ref(columns_rendered), std::ref(cerr_mutex), start_col, end_col); //last thread takes any remaining columns
    }
    for (auto &thread : threads)
        thread.join(); //wait for all threads to finish

    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(t_end - t_start).count();
    std::cerr << "\nDone in " << std::fixed << std::setprecision(2) << elapsed << " s.\n";

    output << serializeBuffer(pixels);
}


void Scene::renderChunk(std::vector<std::vector<std::array<uint8_t, 3>>> &pixels, std::atomic<int> &columns_rendered, std::mutex &cerr_mutex, int start_col, int end_col) const
{
    for (int i = start_col; i < end_col; i++) {
        for (int j = static_cast<int>(height) - 1; j >= 0; j--) {
            double u = (width == 1) ? 0.0 : static_cast<double>(i) / (width - 1);
            double v = (height == 1) ? 0.0 : static_cast<double>(j) / (height - 1);
            Ray ray = _camera.ray(u, v);
            Math::Vector3d color = traceRay(ray, 0);
            pixels[j][i] = {
                static_cast<uint8_t>(std::clamp(color.x, 0.0, 255.0)),
                static_cast<uint8_t>(std::clamp(color.y, 0.0, 255.0)),
                static_cast<uint8_t>(std::clamp(color.z, 0.0, 255.0))
            };
        }
        int done = ++columns_rendered;
        int print_interval = std::max(1, static_cast<int>(width) / 100); //print every ~1% of columns to avoid flooding the log
        if (done % print_interval == 0 || done == static_cast<int>(width)) {
            // hold the lock only for the print, release immediately after
            {
                std::lock_guard<std::mutex> lock(cerr_mutex);
                std::cerr << "\rRendering: " << std::fixed << std::setprecision(2) << (done * 100.0 / width) << "% (" << done << "/" << width << " columns)" << std::flush;
            }
        }
    }
}

std::string Scene::serializeBuffer(const std::vector<std::vector<std::array<uint8_t, 3>>> &pixels) const
{
    std::string ppm = "P3\n" + std::to_string(width) + " " + std::to_string(height) + "\n255\n";
    for (int j = static_cast<int>(height) - 1; j >= 0; j--)
        for (int i = 0; i < static_cast<int>(width); i++)
            write_color(pixels[j][i], ppm);
    return ppm;
}
}