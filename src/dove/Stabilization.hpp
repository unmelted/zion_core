
/*****************************************************************************
*                                                                            *
*                           Stabilization.hpp  								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : Stabilization.cpp
    Author(S)       : Me Eunkyung
    Created         : 07 dec 2021

    Description     : Stabilization.cpp
    Notes           : Stabilization main class
*/

#pragma once
#include <map>
#include "DefData.hpp"
#if defined _MAC_
#include "Detection.hpp"
#include "CMdLogger.hpp"
#endif
#include "ColorTracking.hpp"
#include "GrayTracking.hpp"
#include "Algebra.hpp"
#include "ExpUtil.hpp"

using namespace std;
using namespace cv;
using namespace dove;

class Dove {
public: 
    PARAM* p;
    Configurator::TIMER* t;
    KALMAN* k;
    Tracking* tck;
    Algebra al;
    ExpUtil ex;
    vector<FRAME_INFO> all;
    ANALYSIS an;

#if defined _MAC_
    Detection dt;
    map<int, DT_OBJECTS>objects;
    ofstream obj_trajectory;
    ofstream obj_c_trajectory;    
    CMdLogger _logger;
#endif
    map<int, DT_XY>dt_comp;     // win compile

    vector<SWIPE_INFO> si;
    bool swipe_on;

    TRACK_OBJ* obj;
    TRACK_OBJ* roi;

    string _in;
    string _out;

    Mat mask;
    Mat smth;

#if defined GPU
    cuda::GpuMat refcg;
    cuda::GpuMat refcwg;
    cuda::GpuMat refg;
#endif
    Mat refc;
    Mat refcw;
    Mat ref;

    Mat pre_affine;
    Mat affine;
    int cp_width = 0;
    int cp_height = 0;
    char filename[30];
    int i = 0;
    int threshold = 6;
    
    Dove(VIDEO_INFO* vinfo);
    Dove();
    ~Dove();
    void SetInfo(VIDEO_INFO* vinfo);
    void Initialize();    
    int Process();

#if defined GPU
    int ImageProcess(cuda::GpuMat& src, cuda::GpuMat& dst);
    void SetRefG(cuda::GpuMat& _src) { _src.copyTo(refg); };
    void SetRefCG(cuda::GpuMat& _src) { _src.copyTo(refcg); };
#else
    int ImageProcess(Mat& src, Mat& dst);
#endif
    void ConvertToParam(VIDEO_INFO* info);
    int MakeNewTrajectory(Rect* mg);

    void SetRef(Mat& _src) {_src.copyTo(ref); };
    void SetRefC(Mat& _src) {_src.copyTo(refc); };
    int CalculateMove(Mat& cur, int frame_id);
    int CalculateMove(int frame_id);

    int MakeMask();
    void ApplyImage(Mat& src, bool scaled = false);
    void ApplyImageRef();
    int CalculcateMargin(double minx, double maxx, double miny, double maxy, Rect* mg);

    int PickArea(Mat& src, WIN_INFO* _info, PARAM* p);

private :
    ofstream out_transform;
    ofstream out_trajectory;
    ofstream out_smoothed;
    ofstream out_new;
};