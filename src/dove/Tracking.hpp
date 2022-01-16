/*****************************************************************************
*                                                                            *
*                           Tracking.hpp     								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : Tracking.hpp
    Author(S)       : Me Eunkyung
    Created         : 17 dec 2021

    Description     : Tracking.hpp
    Notes           : Tracking
*/

#pragma once
#include "DefData.hpp"
#include <opencv2/tracking.hpp>

using namespace std;
using namespace cv;
using namespace cv::cuda;
using namespace dove;

class Tracking {

    public :      
    PARAM* p;                                     
    Dlog dl;
    int start_frame;
    float first_summ;
    bool isfound;
    bool issame;
    Rect rect_feature_roi;

    char filename[50];
    GpuMat bgg;
    GpuMat diffg;
    Mat bg;
    Mat prev;
    Mat diff;
    Ptr<MSER>ms;
    Ptr<Tracker>tracker;
    Rect rect_roi;
    TRACK_OBJ* feature_roi;

    Mat lut;
    int scale_w;
    int scale_h;
    int prev_summ;
    int prev_diff;

    Tracking();
    virtual ~Tracking();
    void SetInitialData(PARAM* _p);
    void SetLogFilename(string name) {this->dl.SetLogFilename(name); };    
    float DetectAndTrack(Mat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi);
    void DrawObjectTracking(Mat& src, TRACK_OBJ* obj, TRACK_OBJ* roi, bool borigin = false);
    void DrawObjectTracking(TRACK_OBJ* obj, TRACK_OBJ* roi, vector<Rect> rects);

    int TrackerInitFx(Mat& src, int index, int cx, int cy, TRACK_OBJ* obj, TRACK_OBJ* roi);
#if defined GPU    
    int TrackerInitFx(GpuMat& src, int index, int cx, int cy, TRACK_OBJ* obj, TRACK_OBJ* roi);
#endif

    virtual void SetBg(Mat& src, int frame_id) = 0;
    virtual void ImageProcess(Mat& src, Mat& dst) = 0;
    virtual int TrackerInit(Mat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) = 0;
    virtual int TrackerUpdate(Mat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) = 0;
#if defined GPU
    virtual void SetBg(cuda::GpuMat& src, int frame_id) = 0;    
    virtual void ImageProcess(cuda::GpuMat& src, cuda::GpuMat& dst) = 0;    
    virtual int TrackerInit(cuda::GpuMat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) = 0;
    virtual int TrackerUpdate(cuda::GpuMat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) = 0;    
#endif

    void MakeROI(TRACK_OBJ* obj, TRACK_OBJ* roi);
    float GetIOU(Rect& r, int index, vector<Rect>& rects);
    bool CheckWithin(Rect& r);
    bool CheckWithin(Rect& r, int index, vector<Rect>& rects);
    void ConvertToRect(TRACK_OBJ* roi, Rect* rec, int scale = 1);
    void ConvertToROI(Rect& rec, TRACK_OBJ* obj, TRACK_OBJ* roi);

};
