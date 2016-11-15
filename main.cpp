#include "./util.hpp"

using namespace cc;

float gamma_kernel(float v) {
    return std::exp(-13.0f * v) * std::pow(v, 10.0f);
}

float beta_kernel(float v) {
    return std::pow(v, 9.2f) * std::pow(1.0f - v, 4.8f);
}

int main(int argc, char *argv[]) {
    try {
        vec_t     samples;
        const int n_loop = 100000;
        int       accept = 0;

        // ガンマ分布を目標分布、ガウス分布を提案分布としたメトロポリス・ヘイスティングス法
        float t = gaussian_rand(1.0f, std::sqrt(0.5f));
        for (int i = 0; i < n_loop; i++) {
            float a = gaussian_rand(1.0f, std::sqrt(0.5f));
            float r = (t * gamma_kernel(a)) / (a * gamma_kernel(t));

            // gamma分布の定義から
            if (a < 0.0001f || t < 0.0001f) continue;

            if (std::min(1.0f, r) > uniform_rand(0.0f, 1.0f)) {
                accept++;
                t = a;
            }
            samples.push_back(t);
        }

        // // ベータ分布を目標分布、一様分布を提案分布としたメトロポリス・ヘイスティングス法
        // float t = 0.0f;
        // for (int i = 0; i < n_loop; i++) {
        //     float a = uniform_rand(0.0f, 1.0f);
        //     float r = beta_kernel(a) / beta_kernel(t);
        //
        //     if (std::min(1.0f, r) > uniform_rand(0.0f, 1.0f)) {
        //         accept++;
        //         t = a;
        //     }
        //     samples.push_back(t);
        // }

        // 受容率を調べる
        std::cout << format_str("persec: %.1f%%", 100.0f * accept / samples.size()) << std::endl;

        // バーンイン期間を破棄する
        const int n_burnin = samples.size() / 10;
        std::rotate(samples.begin(), samples.begin() + n_burnin, samples.end());
        samples.resize(samples.size() - n_burnin);

        // EAP推定値を求める
        std::cout << format_str("mean: %f", std::accumulate(samples.begin(), samples.end(), 0.0f) / samples.size()) << std::endl;

        // // logsumexp試してみる
        // float sm     = *std::max_element(samples.begin(), samples.end());
        // float sumexp = 0.0f;
        // for (auto v : samples) {
        //     sumexp += std::exp(std::log(v) - sm);
        // }
        // float s = sm + std::log(sumexp);
        // printf("%f", std::exp(s) / samples.size());

        // ステップ毎に頻度を計算する
        const float step  = 0.05f;
        const int   range = 60;
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

