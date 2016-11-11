#include "./util.hpp"

using namespace cc;

float gauss_proposal(float v) {
    float m = 1.0f;
    float s = std::sqrt(0.5f);
    return std::exp(-(v - m) * (v - m) / (2 * s * s)) / std::sqrt(2.0f * M_PI * s * s);
}

float gamma_kernel(float v) {
    return std::exp(-13.0f * v) * std::pow(v, 10.0f);
}

int main(int argc, char *argv[]) {
    try {
        vec_t     samples;
        const int n_sample = 100000;
        int       accept   = 0;

        // ガウス分布を提案分布としたメトロポリス・ヘイスティングス法
        float t = gaussian_rand(1.0f, std::sqrt(0.5f));
        for (int i = 0; i < n_sample; i++) {
            float a = gaussian_rand(1.0f, std::sqrt(0.5f));
            float r = gauss_proposal(t) * gamma_kernel(a) / gauss_proposal(a) / gamma_kernel(t);

            // gamma分布の定義から
            if (a < 0.0001f || t < 0.0001f) continue;

            if (std::min(1.0f, r) > uniform_rand(0.0f, 1.0f)) {
                accept++;
                t = a;
            }
            samples.push_back(t);
        }

        // 受容率を調べる
        std::cout << format_str("persec: %.1f%%", 100.0f * accept / samples.size()) << std::endl;

        // バーンイン期間を破棄する
        const int n_burnin = n_sample / 100;
        std::rotate(samples.begin(), samples.begin() + n_burnin, samples.end());
        samples.resize(n_sample - n_burnin);

        // ステップ毎に頻度を計算する
        const float step  = 0.1f;
        const int   range = 30;
        vec_t       frequency(range);
        for (auto v : samples) {
            int pos = (int)(v / step);
            if (pos >= 0 && pos < range) {
                frequency[pos] += v;
            }
        }

        // 相対度数でプロットする
        std::ofstream ofs("output");
        for (size_t i = 0; i < frequency.size(); i++) {
            float x = i * step;
            float y = frequency[i] / (step * std::accumulate(frequency.begin(), frequency.end(), 0.0f));
            ofs << format_str("%f %f", x, y) << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << colorant('y', format_str("error: %s", e.what())) << std::endl;
    }
}

