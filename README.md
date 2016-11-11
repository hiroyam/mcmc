# metropolis-hastings

Markov chain Monte Carlo methods (Metropolis-Hastings)

[![Build Status](https://travis-ci.org/hiroyam/metropolis-hastings.svg?branch=master)](https://travis-ci.org/hiroyam/metropolis-hastings)

---

#### これは何？

MCMCのMetropolis-Hastings法のC++によるフルスクラッチです。  
乱数の発生は困難だが値の評価は容易な事後分布のカーネルと、乱数発生が容易な提案分布とを利用して、事後分布から母数の実現値の擬似乱数列を発生させる手法です。


##### 動作サンプル

ガウス分布を提案分布、ガンマ分布を目標分布とした例です。

![](images/plot1.png)

一様分布を提案分布、ベータ分布を目標分布とした例です。

![](images/plot2.png)
