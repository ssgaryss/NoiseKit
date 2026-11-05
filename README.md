# NoiseKit

一个不依赖第三方库的噪声生成工具，首个探索方向是 AMD SSSR 的蓝噪声采样。

| | |
|:---|:---|
| ![Perlin 噪声](docs/images/perlin-noise.png)<br>**Perlin Noise（柏林噪声）**<br>连续、平滑，常用于地形、云层和程序化纹理。 | ![Worley 噪声](docs/images/worley-noise.png)<br>**Worley Noise（细胞噪声）**<br>基于特征点距离，常用于云和细胞纹理。 |
| ![白噪声](docs/images/white-noise.png)<br>**White Noise（白噪声）**<br>随机值相互独立，常作为随机采样的基础。 | ![蓝噪声风格点集示意](docs/images/blue-points.png)<br>**Blue Noise（蓝噪声）**<br>抑制低频结构，常用于抖动和随机渲染；图为分散点集示意。 |
