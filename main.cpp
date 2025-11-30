/*
 * Kravchuk Bogdan lab #3 variant 8
 * C++20
 * nt: 4
 * std::latch
 */

#include <iostream>
#include <vector>
#include <thread>
#include <latch>
#include <syncstream>
#include <chrono>

const int CNT_A = 5;
const int CNT_B = 6;
const int CNT_C = 4;
const int CNT_D = 8;
const int CNT_E = 9;
const int CNT_F = 6;
const int CNT_G = 6;
const int CNT_H = 9;
const int CNT_I = 7;
const int CNT_J = 6;

// --- (Synchronization Latches) ---
std::latch latch_a_done(4);

std::latch latch_b_done(2);

std::latch latch_c_done(1);

std::latch latch_d_done(2);

std::latch latch_e_done(2);

std::latch latch_f_done(1);

std::latch latch_g_done(1);

std::latch latch_h_done(2);

void f(char type, int index) {
    std::osyncstream(std::cout) << "From list " << type << " completed action " << index << ".\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


void thread0_work() {
    // --- Phase 1: A ---
    f('a', 1); f('a', 2);
    latch_a_done.arrive_and_wait();

    // --- Phase 2: D ---
    for (int i = 1; i <= 4; ++i) f('d', i);
    latch_d_done.count_down();

    // --- Phase 3: H ---
    latch_d_done.wait();
    for (int i = 1; i <= 8; ++i) f('h', i);
    latch_h_done.count_down();

    // --- Phase 4: I ---
    latch_e_done.wait();
    latch_f_done.wait();
    for (int i = 1; i <= 4; ++i) f('i', i);
}

void thread1_work() {
    // --- Phase 1: A ---
    f('a', 3);
    latch_a_done.arrive_and_wait();

    // --- Phase 2: D ---
    for (int i = 5; i <= 8; ++i) f('d', i);
    latch_d_done.count_down();

    // --- Phase 3: G, H ---
    latch_c_done.wait();
    for (int i = 1; i <= 6; ++i) f('g', i);
    latch_g_done.count_down();

    latch_d_done.wait();
    f('h', 9);
    latch_h_done.count_down();

    // --- Phase 4: I ---
    latch_e_done.wait();
    latch_f_done.wait();
    for (int i = 5; i <= 7; ++i) f('i', i);
}

void thread2_work() {
    // --- Phase 1: A ---
    f('a', 4);
    latch_a_done.arrive_and_wait();

    // --- Phase 2: B ---
    for (int i = 1; i <= 5; ++i) f('b', i);
    latch_b_done.count_down();

    // --- Phase 3: F, E ---
    latch_c_done.wait();
    for (int i = 1; i <= 6; ++i) f('f', i);
    latch_f_done.count_down();

    latch_b_done.wait();
    f('e', 1);
    latch_e_done.count_down();

    // --- Phase 4: J ---
    latch_g_done.wait();
    latch_h_done.wait();

    for (int i = 1; i <= 3; ++i) f('j', i);
}

void thread3_work() {
    f('a', 5);
    latch_a_done.arrive_and_wait();

    f('b', 6);
    latch_b_done.count_down();

    for (int i = 1; i <= 4; ++i) f('c', i);
    latch_c_done.count_down();

    latch_b_done.wait();

    for (int i = 2; i <= 9; ++i) f('e', i);
    latch_e_done.count_down();

    latch_g_done.wait();
    latch_h_done.wait();

    for (int i = 4; i <= 6; ++i) f('j', i);
}

int main() {
    std::cout << "Calculation started.\n";

    {
        std::vector<std::jthread> threads;
        threads.reserve(4);

        threads.emplace_back(thread0_work);
        threads.emplace_back(thread1_work);
        threads.emplace_back(thread2_work);
        threads.emplace_back(thread3_work);
    }

    std::cout << "Calculation ended.\n";

    return 0;
}