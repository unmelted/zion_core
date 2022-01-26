#include "FrTrans.h"

FrTrans::FrTrans() {
    //FrSetTraceFileName((char*)"stabilizer_test.log");
    //SetTraceLogLevel(FDR_LOG_LEVEL_INFO);
}

FrTrans::~FrTrans() {

}

int FrTrans::Open(std::string& srcName, std::string& dstName) {
    // init value..
    _readNum = 0;
    _decNum = 0;
    _writeNum = 0;
    _eof = false;
    _encFrame = nullptr;


    _urlInfo.pUrl = (char*)srcName.c_str();
    LRSLT lRet = FrReaderOpen(&_hReader, &_urlInfo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    lRet = FrReaderGetInfo(_hReader, &_srcInfo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    // decoder
    FrVideoInfo* pSrcVideoInfo = &_srcInfo.FrVideo;
    pSrcVideoInfo->eCodecType = CODEC_HW;
    pSrcVideoInfo->eColorFormat = VideoFmtYV12;     // output color format, //VideoFmtBGRA
    pSrcVideoInfo->bLowDelayDecoding = TRUE;
    pSrcVideoInfo->memDir = MEM_GPU;   // or MEM_GPU
    lRet = FrVideoDecOpen(&_hDecVideo, NULL, pSrcVideoInfo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    // color conv
    //lRet = FrVideoConvOpen(&_hConv, pSrcVideoInfo, VideoFmtYV12);
    //if (FRFAILED(lRet)) {
    //    return -1;
    //}

    // encoder
    _encInfo.dwVideoTotal = 1;
    _encInfo.FrVideo.dwFourCC = FOURCC_H264;
    _encInfo.FrVideo.eCodecType = CODEC_HW;    //CODEC_HW;
    _encInfo.FrVideo.dwWidth = 1920; //_srcInfo.FrVideo.dwWidth;
    _encInfo.FrVideo.dwHeight = 1080; //_srcInfo.FrVideo.dwHeight;
    _encInfo.FrVideo.dwBitRate = 20 * 1024 * 1024;
    _encInfo.FrVideo.dwFrameRate = 30 * 1000;
    _encInfo.FrVideo.fIntraFrameRefresh = 1;
    _encInfo.FrVideo.eColorFormat = VideoFmtYV12;
    _encInfo.FrVideo.dwGopLength = 1;

    lRet = FrVideoEncOpen(&_hEncVideo, &_encInfo.FrVideo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    // writer
    FrURLInfo	DstUrl = {};
    
    DstUrl.pUrl = (char*)dstName.c_str();
    DstUrl.URLType = _MP4_FF_FILE; //_MP4_FILE;
    lRet = FrWriterOpen(&_hWriter, &DstUrl);
    if (FRFAILED(lRet)) {
        return -1;
    }

    lRet = FrWriterSetInfo(_hWriter, &_encInfo, 0);
    if (FRFAILED(lRet)) {
        return -1;
    }

    unsigned int dwStart = 0;
    lRet = FrReaderStart(_hReader, &dwStart);
    if (FRFAILED(lRet)) {
        return -1;
    }

}

int FrTrans::OpenReader(std::string& srcName) {
    // init value..
    _readNum = 0;
    _decNum = 0;
    _eof = false;

    _urlInfo.pUrl = (char*)srcName.c_str();
    LRSLT lRet = FrReaderOpen(&_hReader, &_urlInfo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    lRet = FrReaderGetInfo(_hReader, &_srcInfo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    // decoder
    FrVideoInfo* pSrcVideoInfo = &_srcInfo.FrVideo;
    pSrcVideoInfo->eCodecType = CODEC_HW;
    pSrcVideoInfo->eColorFormat = VideoFmtYV12;     // output color format, //VideoFmtBGRA
    pSrcVideoInfo->bLowDelayDecoding = TRUE;
    pSrcVideoInfo->memDir = MEM_GPU;   // or MEM_GPU
    lRet = FrVideoDecOpen(&_hDecVideo, NULL, pSrcVideoInfo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    // color conv
    //lRet = FrVideoConvOpen(&_hConv, pSrcVideoInfo, VideoFmtYV12);
    //if (FRFAILED(lRet)) {
    //    return -1;
    //}

    unsigned int dwStart = 0;
    lRet = FrReaderStart(_hReader, &dwStart);
    if (FRFAILED(lRet)) {
        return -1;
    }

    return 1;
}

int FrTrans::OpenWriter(WRITER_CONTEXT_T& ctx) {
    // init value..
    _writeNum = 0;
    _encFrame = nullptr;

    // encoder
    _encInfo.dwVideoTotal = 1;
    _encInfo.FrVideo.dwFourCC = FOURCC_H264;
    _encInfo.FrVideo.eCodecType = CODEC_HW;    //CODEC_HW;
    _encInfo.FrVideo.dwWidth = ctx.width;
    _encInfo.FrVideo.dwHeight = ctx.height;
    _encInfo.FrVideo.dwBitRate = _srcInfo.FrVideo.dwBitRate;
    _encInfo.FrVideo.dwFrameRate = ctx.fps * 1000;
    _encInfo.FrVideo.fIntraFrameRefresh = 1;
    _encInfo.FrVideo.eColorFormat = VideoFmtYV12;
    _encInfo.FrVideo.dwGopLength = ctx.gop;

    LRSLT lRet = FrVideoEncOpen(&_hEncVideo, &_encInfo.FrVideo);
    if (FRFAILED(lRet)) {
        return -1;
    }

    // writer
    FrURLInfo	DstUrl = {};
    DstUrl.pUrl = (char*)ctx.path.c_str();
    DstUrl.URLType = _MP4_FF_FILE; //_MP4_FILE;
    lRet = FrWriterOpen(&_hWriter, &DstUrl);
    if (FRFAILED(lRet)) {
        return -1;
    }

    lRet = FrWriterSetInfo(_hWriter, &_encInfo, 0);
    if (FRFAILED(lRet)) {
        return -1;
    }

    unsigned int dwStart = 0;
    lRet = FrReaderStart(_hReader, &dwStart);
    if (FRFAILED(lRet)) {
        return -1;
    }

    return 1;
}

int FrTrans::Close() {
    if (_hWriter != nullptr) {
        FrWriterUpdateInfo(_hWriter);
        FrWriterClose(_hWriter);
        _hWriter = nullptr;
    }

    if (_hEncVideo != nullptr) {
        FrVideoEncClose(_hEncVideo);
        _hEncVideo = nullptr;
    }

    if (_hDecVideo != nullptr) {
        FrVideoDecClose(_hDecVideo);
        _hDecVideo = nullptr;
    }

    if (_hConv != nullptr) {
        FrVideoConvClose(_hConv);
        _hConv = nullptr;
    }

    if (_hReader != nullptr) {
        FrReaderClose(_hReader);
        _hReader = nullptr;
    }

    if (_encFrame != nullptr) {
        cudaFree(_encFrame);
        _encFrame = nullptr;
    }
    
    for (int i = 0; i < _vDecFrame.size(); i++) {
        uint8_t* frame = _vDecFrame[i];
        if (frame) {
            cudaFree(frame);
            frame = nullptr;
        }
    }
    _vDecFrame.clear();
    
    return 1;
}

int FrTrans::CloseReader() {
    if (_hDecVideo != nullptr) {
        FrVideoDecClose(_hDecVideo);
        _hDecVideo = nullptr;
    }

    if (_hConv != nullptr) {
        FrVideoConvClose(_hConv);
        _hConv = nullptr;
    }

    if (_hReader != nullptr) {
        FrReaderClose(_hReader);
        _hReader = nullptr;
    }

    for (int i = 0; i < _vDecFrame.size(); i++) {
        uint8_t* frame = _vDecFrame[i];
        if (frame) {
            cudaFree(frame);
            frame = nullptr;
        }
    }
    _vDecFrame.clear();

    return 1;
}

int FrTrans::CloseWriter() {
    if (_hWriter != nullptr) {
        FrWriterUpdateInfo(_hWriter);
        FrWriterClose(_hWriter);
        _hWriter = nullptr;
    }

    if (_hEncVideo != nullptr) {
        FrVideoEncClose(_hEncVideo);
        _hEncVideo = nullptr;
    }

    if (_encFrame != nullptr) {
        cudaFree(_encFrame);
        _encFrame = nullptr;
    }

    return 1;
}

int FrTrans::ReadFrame(cv::cuda::GpuMat& frame) {
    LRSLT lRet;
    FrMediaStream VideoData;
    FrMediaStream EncStream;
    FrRawVideo  ConvVideo;
    
    memset(&EncStream, 0, sizeof(FrMediaStream));
    memset(&VideoData, 0, sizeof(FrMediaStream));
    VideoData.tMediaType = VIDEO_MEDIA;
    EncStream.tMediaType = VIDEO_MEDIA;

    if (!_eof) {
        lRet = FrReaderReadFrame(_hReader, &VideoData);
        if (FRFAILED(lRet)) {
            if (lRet == COMMON_ERR_ENDOFDATA) {                
                _eof = true;
                _readNum--;
                LOG_I("[Trans] read frame: end of frame.._eof(%d), readNum(%d)", _eof, _readNum);
            }
            else
                return 0;            
        }
        _readNum++;    
    }
    

    int decodedNum = 0;
    if (!_eof) {
        lRet = FrVideoDecDecode(_hDecVideo, &VideoData, &_rawVideo);
        if (FRFAILED(lRet)) {
            return 0;   // again..
        }
        decodedNum = _rawVideo.nDecoded;
    }
    else {
        if (!_remain) {
            lRet = FrVideoDecDecode(_hDecVideo, NULL, &_rawVideo);
            if (FRFAILED(lRet)) {
                return -1;   // break..
            }
            decodedNum = _rawVideo.nDecoded;
        }
        else {
            decodedNum = 0;
        }
    }
    
    for (int i = 0; i < decodedNum; i++) {
        uint8_t* decFrame;
        
        cudaError_t cerr = cudaMallocPitch((void**)&decFrame, &_decFramePitch, _srcInfo.FrVideo.dwWidth, (_srcInfo.FrVideo.dwHeight >> 1) * 3);
        if (cerr != cudaSuccess) {
            LOG_E("[Trans] enc frame cudaMallocPitch(width:%d/height:%d)=%d", frame.cols, frame.rows, cerr);
            return -1;
        }
        
        // save gpu mat..
        uint8_t* src = _rawVideo.ppDecoded[i];
        uint8_t* dst = decFrame;

        cudaMemcpy2D(dst, _decFramePitch, src, _rawVideo.dwPitch, _rawVideo.dwDecodedWidth, _rawVideo.dwDecodedHeight, cudaMemcpyDeviceToDevice);
        src += _rawVideo.dwPitch * _rawVideo.dwDecodedHeight;
        dst += _decFramePitch * _rawVideo.dwDecodedHeight;
        cudaMemcpy2D(dst, _decFramePitch / 2, src, _rawVideo.dwPitch / 2, _rawVideo.dwDecodedWidth / 2, _rawVideo.dwDecodedHeight / 2, cudaMemcpyDeviceToDevice);
        src += _rawVideo.dwPitch * _rawVideo.dwDecodedHeight / 4;
        dst += _decFramePitch * _rawVideo.dwDecodedHeight / 4;
        cudaMemcpy2D(dst, _decFramePitch / 2, src, _rawVideo.dwPitch / 2, _rawVideo.dwDecodedWidth / 2, _rawVideo.dwDecodedHeight / 2, cudaMemcpyDeviceToDevice);

        _vDecFrame.push_back(decFrame);
        _decNum++;
    }
    
    uint8_t* src = _vDecFrame.front();
    
    FrCvClrConv::YUV2BGR(frame, src, _rawVideo.dwDecodedWidth, _rawVideo.dwDecodedHeight, _rawVideo.dwPitch);
    cudaFree(src);
    _vDecFrame.erase(_vDecFrame.begin());
    _remain = _vDecFrame.size();

    LOG_I("[Trans] read frame: readNum(%d), decNum(%d), remain(%d), _eof(%d)", _readNum, _decNum, _remain, _eof);

    return 1;
}

int FrTrans::WriteFrame(cv::cuda::GpuMat& frame) {
    LRSLT lRet;
    FrMediaStream EncStream;
    FrRawVideo  ConvVideo;
    
    memset(&EncStream, 0, sizeof(FrMediaStream));
    memset(&ConvVideo, 0, sizeof(FrRawVideo));
    EncStream.tMediaType = VIDEO_MEDIA;

    if (_encFrame == nullptr) {
        cudaError_t cerr = cudaMallocPitch((void**)&_encFrame, &_encFramePitch, frame.cols, (frame.rows >> 1) * 3);
        if (cerr != cudaSuccess) {
            LOG_E("[Trans] enc frame cudaMallocPitch(width:%d/height:%d)=%d", frame.cols, frame.rows, cerr);
            return -1;
        }
    }
    
#ifdef _DEBUG
    cv::Mat canvas_t;
    frame.download(canvas_t);
#endif

    ConvVideo.pY = _encFrame;
    ConvVideo.dwPitch = _encFramePitch;
    ConvVideo.dwCTS = _writeNum * 1000. / (_encInfo.FrVideo.dwFrameRate / 1000.);
    FrCvClrConv::BGR2YUV(ConvVideo.pY, _encFramePitch, frame);

#ifdef _DEBUG
    frame.download(canvas_t);
#endif
    
    lRet = FrVideoEncEncode(_hEncVideo, &ConvVideo, &EncStream);
    if (FRFAILED(lRet)) {
        return 0;
    }

    lRet = FrWriterWriteFrame(_hWriter, &EncStream);
    if (FRFAILED(lRet)) {
        return 0;
    }
    
    _writeNum++;
    LOG_I("[Trans] write frame: _writeNum(%d), cts(%d)", _writeNum, ConvVideo.dwCTS);
    return 1;
}