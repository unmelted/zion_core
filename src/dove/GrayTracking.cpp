/*****************************************************************************
*                                                                            *
*                           GrayTracking.cpp     							 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : GrayTracking.cpp
    Author(S)       : Me Eunkyung
    Created         : 28 dec 2021

    Description     : GrayTracking.cpp
    Notes           : Tracking
*/
#include "GrayTracking.hpp"
#include <opencv2/core/cuda.hpp>

GrayTracking::GrayTracking() {

}

GrayTracking::~GrayTracking() {

}

void GrayTracking::SetBg(Mat& src, int frame_id) {    
    int histbin = 256;
    double minval; double maxval;
    double cut_threshold;
    Point minloc; Point maxloc;
    Mat hist;
    start_frame = frame_id;

    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cv::resize(src, bg, Size(scale_w, scale_h));
    }    
    calcHist(&bg, 1, 0, Mat(), hist, 1, &histbin, 0);
    // for (int i = 0 ; i < histbin; i ++){
    //      Cmd_DEBUG(" [{}d] hist {} \n", i , (int)hist.at<float>(i));
    // }

    cv::minMaxLoc(hist, &minval, &maxval, &minloc, &maxloc, Mat());
    CMd_INFO("searched minval {} maxval {} minloc {} {} maxloc {} {}", minval, maxval, minloc.x, minloc.y, maxloc.x, maxloc.y);
    cut_threshold = maxloc.y * 0.83;
    lut = Mat::zeros(1, histbin, CV_8UC1);
    for (int i = 0 ; i < histbin; i ++){
        if(i <= cut_threshold) 
            lut.at<unsigned char>(i) = i;
        else 
            lut.at<unsigned char>(i) = 255;

//        printf(" [%d] lut--  %d \n", i , lut.at<unsigned char>(i));            
    }

    Mat result; Mat temp;
    LUT(bg, lut, result);

    Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(20);
    clahe->apply(result, temp);
    GaussianBlur(temp, bg, {3, 3}, 1.3, 1.3);
    char fname[200];
    sprintf(fname, "dump/cpu_bg_%s.png", Configurator::Get().getCurrentDateTime("now").c_str());
    imwrite(fname, bg);
    CMd_DEBUG("Setbg function finish {} {} ", bg.cols, bg.rows);
}

void GrayTracking::ImageProcess(Mat& src, Mat& dst) {
    Mat temp;
    Mat result;
    src.copyTo(temp);

    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cv::resize(temp, temp, Size(scale_w, scale_h));
//        imwrite("check2.png", dst);
    }
    LUT(temp, lut, result);

    Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(20);
    clahe->apply(result, temp);
    GaussianBlur(temp, dst, {5, 5}, 1.3, 1.3);
}

#if defined GPU
void GrayTracking::SetBg(cuda::GpuMat& src, int frame_id) {
    int histbin = 256;
    double minval; double maxval;
    double cut_threshold;
    Point minloc; Point maxloc;
    cuda::GpuMat hist = cuda::GpuMat(Size(256, 1), CV_32SC1, Scalar(0));
    start_frame = frame_id;

    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cuda::resize(src, bgg, Size(scale_w, scale_h));
    }

    Mat check;
    Mat hist_t;
    bgg.download(check);
    calcHist(&check, 1, 0, Mat(), hist_t, 1, &histbin, 0);
    //for (int i = 0 ; i < histbin; i ++){
    //    CMd_DEBUG(" [{}] hist_t {} \n", i , (int)hist_t.at<float>(i));
    //}

    cuda::calcHist(bgg, hist, cuda::Stream::Null());
    cuda::minMaxLoc(hist, &minval, &maxval, &minloc, &maxloc, noArray());
    CMd_DEBUG("searched minval {} maxval {} minloc {} {} maxloc {} {}", minval, maxval, minloc.x, minloc.y, maxloc.x, maxloc.y);
    cut_threshold = maxloc.x * 0.83;
    lut = Mat::zeros(1, histbin, CV_8UC1);
    for (int i = 0 ; i < histbin; i ++){
        if(i <= cut_threshold) 
            lut.at<unsigned char>(i) = i;
        else 
            lut.at<unsigned char>(i) = 255;
    }

    cuda::GpuMat gt; 
    gt.upload(lut);
    cuda::GpuMat result; 
    cuda::GpuMat temp;
    Ptr<cuda::LookUpTable> glut = cuda::createLookUpTable(lut);
    glut->transform(bgg, result);

    Ptr<cuda::CLAHE> clahe = cuda::createCLAHE(20);
    clahe->apply(result, temp);
    Ptr<cuda::Filter>gblur = cuda::createGaussianFilter(CV_8UC1, CV_8UC1, Size(3, 3), 1.3);
    gblur->apply(temp, bgg);
    gt.release();
    result.release();
    temp.release();
    
    bgg.download(check);
    char fname[200];
    sprintf(fname, "dump/gpu_bg_%s.png", Configurator::Get().getCurrentDateTime("date").c_str());    
    imwrite(fname, check);
    CMd_DEBUG("Setbg function finish {} {} ", bg.cols, bg.rows);
}

void GrayTracking::ImageProcess(cuda::GpuMat& src, cuda::GpuMat& dst) {
    cuda::GpuMat temp;
    cuda::GpuMat result;
    src.copyTo(temp);    
    if(p->track_scale != 1 ) {
        scale_w = int(src.cols/p->track_scale);
        scale_h = int(src.rows/p->track_scale);
        cuda::resize(temp, temp, Size(scale_w, scale_h));
//        imwrite("check2.png", dst);
    }

    cuda::GpuMat gt; 
    gt.upload(lut);
    Ptr<cuda::LookUpTable> glut = cuda::createLookUpTable(lut);
    glut->transform(temp, result);

    Ptr<cuda::CLAHE> clahe = cuda::createCLAHE(20);
    clahe->apply(result, temp);
    Ptr<cuda::Filter>gblur = cuda::createGaussianFilter(CV_8UC1, CV_8UC1, Size(3, 3), 1.3);
    gblur->apply(temp, dst);
    gt.release();
    result.release();
    temp.release();    
}

int GrayTracking::TrackerInit(cuda::GpuMat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    int result = 0;
    double minval; double maxval;
    Point minloc; Point maxloc;
    cuda::GpuMat cur;
    ImageProcess(src, cur);
    Mat cur_check;
    cur.download(cur_check);
    imwrite("dump/tracker_init.png", cur_check);
    CMd_DEBUG("PickArea cos/row {} {} st_frame {} index {}", cur.cols, cur.rows, start_frame, index);
    cuda::subtract(bgg, cur, diffg);
    //float diff_val = cuda::sum(diffg)[0] / (scale_w * scale_h);

    cuda::minMaxLoc(diffg, &minval, &maxval, &minloc, &maxloc, noArray());
    CMd_DEBUG("PickArea minval {} maxval {} minloc {} {} maxloc {} {}", minval, maxval, minloc.x, minloc.y, maxloc.x, maxloc.y);
    if(maxloc.x == 0 && maxloc.y == 0)
        return STABIL_CANT_GRAP_TRACKINGPT;        
    diffg.download(diff);
    char fname[200];
    sprintf(fname, "dump/gpu_track_init_%s.png", Configurator::Get().getCurrentDateTime("date").c_str());    
    imwrite(fname, diff);
    result = TrackerInitPost(maxloc, obj, roi);
    return ERR_NONE;    
}

int GrayTracking::TrackerUpdate(cuda::GpuMat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    int result = -1;
    cuda::GpuMat cur;
    ImageProcess(src, cur);
    //CMd_INFO("TrackerUpdate cos/row {} {} st_frame {} index {}", cur.cols, cur.rows, start_frame, index);
    cuda::subtract(bgg, cur, diffg);
    //float diff_val = cuda::sum(diff)[0] / (scale_w * scale_h);
    /* if you need to check the same image, please uncommnet these block.
    if(index > start_frame +1 && !prev.empty()) {
        Mat same;
        subtract(prev, cur, same);
        float same_check = sum(same)[0]/(scale_w * scale_h);
        //dl.Logger("same check %f ", same_check);
        if (same_check < 0.2) {
            issame = true;
            //return same_check;
        }
        else
            issame = false;
    }
    else if( index == start_frame ) {
        first_summ = diff_val;
    }
    cur.copyTo(prevg);
    */


    diffg.download(diff);

   result = TrackerUpdatePost(obj, roi);
   return result;
}

#endif

int GrayTracking::TrackerInit(Mat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    int result = 0;
    double minval; double maxval;
    Point minloc; Point maxloc;
    Mat cur; Mat dst;
    ImageProcess(src, cur);
    CMd_DEBUG("PickArea cos/row {} {} st_frame {}  index {}", cur.cols, cur.rows, start_frame, index);
    Mat mask = Mat::zeros(cur.rows, cur.cols, CV_8UC1);
    rectangle(mask, Point(60, 40), Point(cur.cols - 60, cur.rows - 40), Scalar(255), -1);
    cv::subtract(bg, cur, diff);
    //float diff_val = cv::sum(diff)[0]/(scale_w * scale_h);

    cv::minMaxLoc(diff, &minval, &maxval, &minloc, &maxloc, mask);
    CMd_DEBUG("PickArea minval {} maxval {}  minloc {} {} maxloc {} {}", minval, maxval, minloc.x, minloc.y, maxloc.x, maxloc.y);
    if(maxloc.x == 0 && maxloc.y == 0)
        return STABIL_CANT_GRAP_TRACKINGPT;
    char fname[200];
    sprintf(fname, "dump/cpu_track_init_%s.png", Configurator::Get().getCurrentDateTime("now").c_str());    
    imwrite(fname, diff);
    result = TrackerInitPost(maxloc, obj, roi);
    return ERR_NONE;       
}

int GrayTracking::TrackerUpdate(Mat& src, int index, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    int result = -1;
    Mat cur; Mat dst;
    ImageProcess(src, cur);
    //CMd_INFO("TrackerUpdate cos/row {} {} st_frame {} index {}", cur.cols, cur.rows, start_frame, index);
    cv::subtract(bg, cur, diff);
    float diff_val = cv::sum(diff)[0]/(scale_w * scale_h);
    /* if you need to check the same image, please uncommnet these block.
    if(index > start_frame +1 && !prev.empty()) {
        Mat same;        
        subtract(prev, cur, same);
        float same_check = sum(same)[0]/(scale_w * scale_h);
        //dl.Logger("same check %f ", same_check);
        if (same_check < 0.2) {
            issame = true;
            //return same_check;
        }
        else
            issame = false;    
    }
    else if( index == start_frame ) {
        first_summ = diff_val;    
    }
        cur.copyTo(prev);
    */
   result = TrackerUpdatePost(obj, roi);
   return result;

}

int GrayTracking::TrackerInitPost(Point& max, TRACK_OBJ* obj, TRACK_OBJ* roi) {
    if(max.x <= 0 )
        max.x = p->roi_w/2 + 10;
    if(max.y <= 0 )
        max.x = p->roi_h/2 + 10;
    if(max.x + p->roi_w/2 > p->limit_bx)
        max.x =  p->limit_bx - p->roi_w/2 - 10;
    if(max.y + p->roi_h/2 > p->limit_by)    
        max.y =  p->limit_by - p->roi_h/2 - 10;        

    obj->update(int(max.x - p->roi_w/2), int(max.y - p->roi_h/2), p->roi_w, p->roi_h);
    obj->update();
    roi->update(obj->sx - 10, obj->sy - 10, obj->w + 20, obj->h + 20);
    roi->update();
    CMd_DEBUG("gray obj {} {} {} {}", obj->sx, obj->sy, obj->w, obj->h);
    CMd_DEBUG("gray roi {} {} {} {}", roi->sx, roi->sy, roi->w, roi->h);
    ConvertToRect(roi, &rect_roi);
    CMd_DEBUG("gray rect roi for tracker init {} {} {} {}", rect_roi.x, rect_roi.y, rect_roi.width, rect_roi.height);
    tracker->init(diff, rect_roi);
    //img debug    
    Mat du;
    diff.copyTo(du);
    circle(du, Point(max.x, max.y), 3, Scalar(255, 0, 0), -1);    
    char fname[200];
    sprintf(fname, "dump/track_init_%s.png", Configurator::Get().getCurrentDateTime("now").c_str());
    imwrite(fname, du);    

    isfound = true;
    //DrawObjectTracking(diff, obj, roi, false, 1);
    return ERR_NONE;
}

int GrayTracking::TrackerUpdatePost(TRACK_OBJ* obj, TRACK_OBJ* roi) {
    // add linux compile test..
    bool ret = false;
    //bool ret = tracker->update(diff, rect_roi);
    //CMd_INFO("tracker update {} {} {} {} ", rect_roi.x, rect_roi.y, rect_roi.width, rect_roi.height);
   
    if (ret == false) {
        CMd_WARN("tracker miss --------------------------------------------");
//        tracker->init(diff, rect_roi);            
    }

    ConvertToROI(rect_roi, obj, roi);
    isfound = true;    
    //DrawObjectTracking(diff, obj, roi, false, 1);
    //sprintf(filename, "saved\\%d_trck.png", index);
    //imwrite(filename, diff);
    tracker->init(diff, rect_roi);                    
    if(p->mode == DETECT_TRACKING_CH) {
        MakeROI(obj, feature_roi);
        ConvertToRect(feature_roi, &rect_feature_roi, p->track_scale);
    }

    return ERR_NONE;
}
