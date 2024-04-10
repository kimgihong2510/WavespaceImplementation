#include "WavespaceModel.h" 

WavespaceModel::WavespaceModel()
{
    // read .onnx file
    mSession.reset();
    mSession = std::make_unique<Ort::Session>(mEnv, (void*) model_ort_start, model_ort_size, Ort::SessionOptions{nullptr});

    mInputShape[0] = 1;
    mInputShape[1] = static_cast<int64_t>(NUM_STYLE * 2 + NUM_DESCRIPTOR);

    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    mInputTensor = Ort::Value::CreateTensor<float>(memory_info, mInputArray.data(), mInputArray.size(),
                                                   mInputShape.data(), mInputShape.size());
}

void WavespaceModel::setNewWaveform(ConditionData conditionData, std::array<float, SIZE_WAVEFORM>& waveform)
{
    // check if the model is in the middle of its load OR model is not Loaded yet.
    if (mSession == nullptr)
    {
        return;
    }
    const char *inputNames[1] = {"input"};
    const char *outputNames[2] = {"amp", "phase"};

    auto idx = 0;
    for (int i = 0; i < NUM_STYLE; i++)
    {
        mInputArray[idx++] = conditionData.valuesStyleX[i];
        mInputArray[idx++] = conditionData.valuesStyleY[i];
    }
    for(int i = 0; i < NUM_DESCRIPTOR; i++)
    {
        if(IDX_SYMMETRY == i)
            mInputArray[idx++] = conditionData.valuesDescriptorX[i] * 2*PI - PI;
        else
            mInputArray[idx++] = conditionData.valuesDescriptorX[i];
        
    }
    Ort::RunOptions run_options;

    auto outputTensors = mSession->Run(run_options, inputNames, &mInputTensor, 1, outputNames, 2);

    auto ampArray = outputTensors[0].GetTensorData<float>();
    auto phaseArray = outputTensors[1].GetTensorData<float>();

    std::array<float, SIZE_WAVEFORM*2> fftArray;
    performIFFT(mFFT, ampArray, phaseArray, fftArray);
    float ampOverall = 0.0f;
    for(int i=0; i<SIZE_WAVEFORM; i++)
        ampOverall += fftArray[i] * fftArray[i];
    ampOverall = sqrt(ampOverall);
    for(int i=0; i<SIZE_WAVEFORM; i++)
        waveform[i] = fftArray[i] / ampOverall;
}