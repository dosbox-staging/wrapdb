/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright (C) 2022-2022  kcgen <kcgen@users.noreply.github.com> */

#include <stdint.h>
#include <arm_neon.h>
#include <math.h>

static void square4_serial(float*);
static void square4_vectorized(float*);
static int compare4(const float*, const float*);

static void square4_serial(float* array) {
    for (uint8_t i = 0; i < 4; ++i) {
        const float f = array[i];
        array[i] = f * f;
    }
}

static void square4_vectorized(float* array) {
    float32x4_t f = vld1q_f32(array);
    f = vmulq_f32(f, f);
    vst1q_f32(array, f);
}

static int compare4(const float* a, const float* b) {
    float sum_of_diffs = 0.0f;
    for (uint8_t i = 0; i < 4; ++i) {
        sum_of_diffs += fabsf(a[i] - b[i]);
    }
    const int rounded_sum = (int)(roundf(sum_of_diffs));
    return rounded_sum;
}

int main() {
    float values1[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    square4_serial(values1);

    float values2[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    square4_vectorized(values2);

    return compare4(values1, values2);
}
