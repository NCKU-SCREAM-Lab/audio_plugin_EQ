# 數位音樂訊號處理 EQ組 開發日誌

## **主要功能**
> 學期開始時，我們希望能實作出使用 EQ 時常見的六種 Filter，
因此便開始研究音訊處理的基本原理，
過程中，也認識到 Filter 分為兩種類型：**FIR** 與 **IIR**
由其各自的特性，在做 DSP 時，通常使用 IIR 會更有效率，其中最常見的是 Biquad Filter。
但經過老師的提點，我們發現在現今的設備上實現高階的 FIR filter 是非常 OK 的，

因此，我們最後規劃以：
1. FIR 實現 LPF、HPF
2. IIR 實現 Notch、Peak、LSF、HSF
3. 並期待能提供 GUI，畫出訊號的頻譜 & Filter 的頻率響應

### **LPF、HPF** (using FIR)

#### **Time Domain Convolution**
剛開始，我們只希望能先看到一些成果，因此，查到[ 這個網站 ](https://tomroelandts.com/articles/how-to-create-a-simple-low-pass-filter?fbclid=IwAR17-Sasn2w2OZBPYCKB8Y1lMOjzYOFKpQfwGA0ljS0ZaMHWpk3BTiezs5c)介紹如何產生 LPF 的 impulse response 時，

```cpp=
/* Construct impulse response of Low Pass Filter (FIR) */
std::array <float, 560> h = { 0.0 };
for (int i = -(order - 1) / 2, j = 0; i <= order / 2; i++, j++) {
    if (i != 0)
        h[j] = (sin(2 * fc * i * MY_PI) / (2 * fc * i * MY_PI));
    else
        h[j] = (1.0);
}

/* Set blackman window */
w.clear();
for (int i = 0; i < order; i++)
    w.push_back(0.42 - 0.5 * cos(2 * MY_PI * i / (order - 1)) + 0.08 * os(4 * MY_PI * i / (order - 1)));

/* Windowed-Sinc Filter */
for (int i = 0; i < order; i++) {
    h[i] = h[i] * w.at(i);
    impulse_response_sum += h[i];
}

/* Normalized windowed-sinc filter */
for (int i = 0; i < order; i++)
    h[i] /= impulse_response_sum;
```

我們就直接將 $h[n]$ 對時域上的訊號做卷積。
做完之後雖然有 filter 成功，但 FIR 計算量龐大的問題也隨之而來，
即當 FIR 的 tap 數太高時，程式會跑不動，使 EQ 的表現大大受限。

#### **Frequency Domain Multiplication**
因此，再往下研究之後，**辣個男人** 出現了。

![](https://i.imgur.com/gsphO2C.png) **" 約瑟夫 · 傅立葉 "**

下面是針對某段訊號進行濾波後的結果，
可以看見，比起直接在時域作 convolution，
先將 Signal 及 Filter 傅立葉轉換至頻域，做點乘再逆轉換回來的運算速度，快了將近 25 倍。 
[*Image Source*](https://www.youtube.com/watch?v=u8t-h31baFE&ab_channel=YoungmooKim)

![](https://i.imgur.com/mZKOrhj.png)


**時間複雜度分析 ↓**
![](https://i.imgur.com/jmeyN08.png)

```cpp=
/* signal FFT */
forwardFFT.performRealOnlyForwardTransform(&fftData.at(0));
/* Dot product with the frequency response of the filter */
for (int i = 0; i < fftSize * 2; i += 2)
{
	float sr = fftData[i];
	float si = fftData[i + 1];
	float fr = FIR_freq_response_c[i];
	float fi = FIR_freq_response_c[i + 1];
	fftData[i] = sr * fr - si * fi;
	fftData[i + 1] = sr * fi + si * fr;
}
/* signal iFFT */
forwardFFT.performRealOnlyInverseTransform(&fftData.at(0));
```
```cpp=
/* filter FFT */
for (int i = 0; i < fftSize * 2; i++)
{
    H[i] = (i < order) ? h[i] : 0;  // 將 filter 0-padding 至 fft size
}
forwardFFT.performRealOnlyForwardTransform(&H.at(0));
```

:::warning
但做完之後，聲音聽起來還是怪怪的?
:::
#### **Overlap-add**
一個長度 $L$ 和長度 $M$ 的有限長度離散訊號，做卷積之後會變成長度 $L+M-1$ 的訊號。
同樣，將此二訊號先傅立葉轉換至頻域做點乘，再逆轉換回來，訊號長度也是 $L+M-1$。
[維基百科 - 重疊-相加之摺積法](https://zh.wikipedia.org/wiki/%E9%87%8D%E7%96%8A-%E7%9B%B8%E5%8A%A0%E4%B9%8B%E6%91%BA%E7%A9%8D%E6%B3%95)

![](https://i.imgur.com/q7cao1V.png)

因此，每個 Process Block 做完 Filtering 都會多出一段不完整的輸出，
下次的結果須再加上此段訊號。

動圖為例 [*GIF Source*](https://www.youtube.com/watch?v=u8t-h31baFE&ab_channel=YoungmooKim) ↓
![image alt](https://j.gifs.com/J8l4Yl.gif)

```cpp=
/* Overlap-Add method */

L = buffer.getNumSamples(); // process block size
M = numTap;                 // filter size

for (int i = 0; i < M - 1; i++)
{
	fftData[i] += overlap[i];
}
for (int i = 0; i<buffer.getNumSamples(); i++) {
	buffer.setSample(0, i, fftData[i]);
	buffer.setSample(1, i, fftData[i]);
}
for (int i = 0; i < M - 1; i++)
{
	overlap[i] = fftData[L + i];
}
```
在修正這些訊號之後，聲音聽起來就比較沒有雜音了！

### Notch、Peak、LSF、HSF (using IIR)
這個部分，我們使用 juce 內建的 IIR API，取出特定 filter 的 $a,b$ 係數之後，
以 IIR filter 的公式來實作。

![](https://i.imgur.com/cfT0NE9.png)


```cpp=
/* initialize the first few input and output signal to 0 */
x = {0, 0, 0};  //input signal
y = {0, 0};     //output signal

/* construct impulse response */
coef = juce::dsp::IIR::ArrayCoefficients<float>::makeNotch(getSampleRate(), f, Q);
h1 = { coef[2] / coef[3], coef[1] / coef[3], coef[0] / coef[3] }; 
h2 = { coef[5] / coef[3], coef[4] / coef[3] };

/* apply IIR filter */
for (int i = 0; i < buffer.getNumSamples(); ++i) {
    x.erase(x.begin());
    x.push_back(buffer.getSample(0, i));

    value = 0.0;
    for (int j = 0; j < 3; j++)
        value += x.at(j) * h1.at(j);
    for (int j = 0; j < 2; j++)
        value -= y.at(j) * h2.at(j);

    y.erase(y.begin());
    y.push_back(value);

    buffer.setSample(0, i, value);
    buffer.setSample(1, i, value);
}
```

### **Frequency Response Curve**

#### **FIR**
將 Filter 的脈衝響應做 FFT 之後，得到頻率響應。
```cpp=
/* FFT */
for (int i = 0; i < fftSize * 2; i++)
{
	H_freq[i] = (i < order) ? h[i] : 0;
}
forwardFFT.performFrequencyOnlyForwardTransform(&H_freq.at(0));
```

#### **IIR**
利用 JUCE 提供的 API，給定 IIR 的 $a,b$ 係數與特定頻率組合，得到相對應的 Magnitude。
```cpp=
juce::dsp::IIR::Coefficients<float> IIR_filter(coef[0], coef[1], coef[2], coef[3], coef[4], coef[5]);
IIR_filter.getMagnitudeForFrequencyArray(&frequencies[0], &response[0], fftSize/2, getSampleRate());
```

為了待會與 FIR 的頻率響應相結合，我們將 IIR 的頻率響應也切為 fftSize$/2$ 個 bin。
```cpp=
std::vector<double> response;
response.resize(fftSize/2);
juce::FloatVectorOperations::fill(&response[0], 0, fftSize/2);
```
設定欲求的特定頻率組合：
```cpp=
for (int i = 0; i < fftSize/2; ++i)
{
    frequencies.push_back(binToFreq(i,sampleRate));
}

float binToFreq(int binNum, double sr)
{
	float binWidth = sr / fftSize;
	auto binFreq = binNum * binWidth;
	return binFreq;
}
//   0   ~   511   bin
//       ↓
//   0   ~  24000  Hz
//  
//  frequencies[] = { 0Hz, 46.875Hz, ... , 24000Hz }*/
```



#### **實際繪製圖形**
首先將剛剛求得的 FIR、IIR 各自的頻率響應結合：
```cpp=
for (int i = 0; i < fftSize / 2; i++)
{
    freqResponse[i] = IIR_Response[i] * FIR_freq_response[i];
}
```

接著我們沿用助教提供的範例程式碼，實作音訊的頻譜和 filter 的響應曲線。
```cpp=
/* filter response curve generator */
void generateResponsePath(const std::vector<double>& renderData,
		juce::Rectangle<float> fftBounds,
		int fftSize,
		float binWidth,
		float negativeInfinity)
{
    auto left   = fftBounds.getX();
    auto top    = fftBounds.getY();
    auto bottom = fftBounds.getHeight();
    auto width  = fftBounds.getWidth();

    int numBins = (int)fftSize/2;  // FFT block 的後半段與前半段對稱，僅取前半

    PathType p;
    p.preallocateSpace(3 * (int)fftBounds.getWidth());

    /* 利用 lambda expression 將頻率響應 map 到 GUI 上 */
    auto map = [bottom, top, negativeInfinity](float v)
    {
        return juce::jmap(v,
            0.f, 2.5f,
            float(bottom), top);
    };

    auto y = map(renderData[0]);
    if (std::isnan(y) || std::isinf(y))
        y = bottom;

    p.startNewSubPath(left, y);

    const int pathResolution = 1;
    for (int binNum = 1; binNum < numBins; binNum += pathResolution)
    {
        y = map(renderData[binNum]);

        if (!std::isnan(y) && !std::isinf(y))
        {
            auto binFreq = binNum * binWidth;
            auto normalizedBinX = juce::mapFromLog10(binFreq, 20.f, 20000.f);
            int binX = std::floor(left + normalizedBinX * width);
            p.lineTo(binX, y);
        }
    }

    pathFifo.push(p);
}
```

### GUI

![](https://i.imgur.com/IIDg5uL.png)
