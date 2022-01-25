
/*****************************************************************************
*                                                                            *
*                           ColorTracking.cpp     							 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : ColorTracking.cpp
    Author(S)       : Me Eunkyung
    Created         : 28 dec 2021

    Description     : ColorTracking.cpp
    Notes           : Tracking
*/

#include "ColorTracking.hpp"

ColoredTracking::ColoredTracking() {

}

ColoredTracking::~ColoredTracking() {

}

void ColoredTracking::SetBg(Mat& src, int frame_id) {

    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cv::resize(src, bg, Size(scale_w, scale_h));
    }
    imwrite("dump/cpu_bg.png", bg);
    CMd_INFO("Colored Setbg function finish {} {} ", bg.cols, bg.rows);    
}

void ColoredTracking::ImageProcess(Mat& src, Mat& dst) {

    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cv::resize(src, dst, Size(scale_w, scale_h));
    }
}

#if defined GPU
void ColoredTracking::SetBg(cuda::GpuMat& src, int frame_id) {

    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cuda::resize(src, src, Size(scale_w, scale_h));
    }
    src.download(bg);
    imwrite("dump/gpu_bg2.png", bg);    
    CMd_INFO("Colored Setbg function finish {} {} ", bg.cols, bg.rows);    
}

void ColoredTracking::ImageProcess(cuda::GpuMat& src, cuda::GpuMat& dst) {

    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cuda::resize(src, dst, Size(scale_w, scale_h));
    }
}

int ColoredTracking::TrackerInit(cuda::GpuMat& _src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    int result = -1;
    Mat src;
    _src.download(src);
    result = TrackerInit(src, index, obj, roi);
    return result;
}

int ColoredTracking::TrackerUpdate(cuda::GpuMat& _src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    int result = -1;
    Mat src;
    _src.download(src);
    result = TrackerUpdate(src, index, obj, roi);
    return result;
}

#endif

int ColoredTracking::TrackerInit(Mat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    int result = 0;
    double minval; double maxval;
    Point minloc; Point maxloc;
    Mat cur; Mat cur_gray; Mat bg_gray;
    ImageProcess(src, cur);
    CMd_DEBUG("PickArea cos/row {} {} st_frame {} index {}", cur.cols, cur.rows, start_frame, index);
    cv::cvtColor(bg, bg_gray, COLOR_BGR2GRAY);
    cv::cvtColor(cur, cur_gray, COLOR_BGR2GRAY);
    cv::subtract(bg_gray, cur_gray, diff);
    float diff_val = cv::sum(diff)[0]/(scale_w * scale_h);

    cv::minMaxLoc(diff, &minval, &maxval, &minloc, &maxloc, Mat());
    CMd_DEBUG("PickArea minval {} maxval {} minloc {} {} maxloc {} {}", minval, maxval, minloc.x, minloc.y, maxloc.x, maxloc.y);
    imwrite("dump/cpu_diff.png", diff);
    if(maxloc.x <= 0 )
        maxloc.x = p->roi_w/2 + 10;
    if(maxloc.y <= 0 )
        maxloc.x = p->roi_h/2 + 10;

    obj->update(int(maxloc.x - p->roi_w/2), int(maxloc.y -p->roi_h/2), p->roi_w , p->roi_h);    
    obj->update();
    roi->update(obj->sx - 10, obj->sy - 10, obj->w + 20, obj->h + 20);    
    roi->update();
    CMd_DEBUG("color obj {} {} {} {}", obj->sx, obj->sy ,obj->w , obj->h);
    CMd_DEBUG("color roi {} {} {} {}", roi->sx, roi->sy ,roi->w , roi->h);

    ConvertToRect(roi, &rect_roi);
    CMd_DEBUG("color rect roi for tracker init {} {} {} {}", rect_roi.x, rect_roi.y, rect_roi.width, rect_roi.height);
    tracker->init(cur, rect_roi);
    isfound = true;
    //DrawObjectTracking(diff, obj, roi, false, 1);
    return ERR_NONE;
}

int ColoredTracking::TrackerUpdate(Mat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    Mat cur; Mat dst;
    ImageProcess(src, cur);
    cur.copyTo(prev);
   
    bool ret = tracker->update(cur, rect_roi);
    //dl.Logger("[%d] colortracker update %d %d %d %d ",index, rect_roi.x, rect_roi.y, rect_roi.width, rect_roi.height);
   
    if (ret == false) {
        CMd_WARN("tracker miss --------------------------------------------");
    }

    ConvertToROI(rect_roi, obj, roi);
    isfound = true;    
    //DrawObjectTracking(cur, obj, roi, true, 1);
    //sprintf(filename, "saved\\%d_trck.png", index);
    //imwrite(filename, diff);
    tracker->init(cur, rect_roi);                    
    if(p->mode == DETECT_TRACKING_CH) {
        MakeROI(obj, feature_roi);
        ConvertToRect(feature_roi, &rect_feature_roi, p->track_scale);
    }

    return ERR_NONE;
}
