#pragma once

#include <iostream>
#include <vector>
#include "cuda_runtime.h"
#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include "opencv2/cudawarping.hpp"
#include "opencv2/cudaarithm.hpp"

#include "FrCvClrConv.h"
#include "SystemAPI.h"
#include "TraceAPI.h"
#include "StreamReaderAPI.h"
#include "StreamWriterAPI.h"
#include "ConverterAPI.h"
#include "DecoderAPI.h"
#include "EncoderAPI.h"

class FrTrans {
public:
    //static const int MAX_DEC_FRAME = 12;
    typedef struct _WRITER_CONTEXT_T {
        std::string path;
        
        int32_t fps{};
        int32_t width{};
        int32_t height{};
        int32_t gop{};
    } WRITER_CONTEXT_T;

	FrTrans();
	~FrTrans();

	int Open(std::string& srcName, std::string& dstName);
	int ReadFrame(cv::cuda::GpuMat& frame);
    int WriteFrame(cv::cuda::GpuMat& frame);
	int Close();

    int OpenReader(std::string& name);
    int CloseReader();
    int OpenWriter(WRITER_CONTEXT_T& ctx);
    int CloseWriter();

private:
    FrReaderHandle	    _hReader;
    FrVideoDecHandle    _hDecVideo;
    FrVideoEncHandle    _hEncVideo;
    FrWriterHandle	    _hWriter;
    FrVideoConvHandle	_hConv;

    FrURLInfo	        _urlInfo;
    FrMediaInfo         _srcInfo;
    FrMediaInfo         _encInfo;

    FrMediaStream       _videoStream;
    FrRawVideo          _rawVideo;
    
    int _readNum{};
    int _decNum{};
    int _writeNum{};

    std::vector<uint8_t*> _vDecFrame;
    size_t _decFramePitch;
    int _curFrame;
    bool _eof{};
    int _remain{};

    uint8_t* _encFrame{};
    size_t _encFramePitch;
};
