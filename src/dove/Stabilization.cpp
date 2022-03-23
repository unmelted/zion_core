
/*****************************************************************************
*                                                                            *
*                           Stabilization.cpp  								 *
*                                                                            *
*   Copyright (C) 2021 By 4dreplay, Incoporated. All Rights Reserved.        *
******************************************************************************

    File Name       : Stabilization.cpp
    Author(S)       : Me Eunkyung
    Created         : 07 dec 2021

    Description     : main procee for Stabilization
    Notes           : Stabilization main class
*/


#if defined _WIN_ || _WINDOWS
#include "FrTrans.h"
#define WIN_TRANS
#endif
#include <thread>
#include "Stabilization.hpp"
using namespace std;
using namespace cv;
using namespace dove;

Dove::Dove() {
    p = new PARAM();
    t = new Configurator::TIMER();
    CMd_INFO("instance created.. ");    

}

Dove::Dove(VIDEO_INFO* vinfo) {
    p = new PARAM();
    t = new Configurator::TIMER();
    CMd_INFO("instance created.. ");    

    ConvertToParam(vinfo);
    Initialize();
}

void Dove::SetInfo(VIDEO_INFO* vinfo) {
    ConvertToParam(vinfo);
    Initialize();
}

void Dove::ConvertToParam(VIDEO_INFO* info) {
    _in = info->input;
    _out = info->output;
    p->event = info->event;
    if (info->width > 1920)
        p->scale = 2;
    else
        p->scale = 1;

    CMd_DEBUG("Video width {}, scale {}", info->width, p->scale);
    int size = info->swipe_period.size();
    CMd_DEBUG("Conver To Param swipe period size {}", size);
    for (int i = 0; i < size; i++)
    {
        SWIPE_INFO one;
        one.order = i;
        one.start = info->swipe_period[i].start;
        one.end = info->swipe_period[i].end;
            one.target_x = int(info->swipe_period[i].target_x);
            one.target_y = int(info->swipe_period[i].target_y);
        one.zoom = info->swipe_period[i].zoom;
        si.push_back(one);
        CMd_DEBUG("SW Period {} {} ", one.start, one.end);
        CMd_DEBUG("target {} {} zoom {}", one.target_x, one.target_y, one.zoom);        
    }

    if (p->event != FIGURE) {
        p->roi_input = true;
        p->colored = true;
    }
    else {
        p->roi_input = false;
        p->colored = false;        
    }
}

void Dove::Initialize() {
#if defined LOCAL_TEST
//    ex.TestGetSwipeInfo(_in, p);
#endif    

    p->mode = DETECT_TRACKING;        
    p->interpolation_mode = SPLINE_LSF; //MEDIAN_KERNEL

    if (p->mode == OPTICALFLOW_LK_2DOF) {
        p->scale = 2;
        p->run_kalman = true;
        p->run_detection = false;

    } else if (p->mode == DARKNET_DETECT_MOVE) {
        p->scale = 2;
        p->run_kalman = true;
        p->run_detection = true;
        p->detector_type = DARKNET_YOLOV4;
    
    } else if (p->mode == DETECT_TRACKING || 
            p->mode == DETECT_TRACKING_CH) {
        obj = new TRACK_OBJ();
        roi = new TRACK_OBJ();
        p->run_tracking =   true;
        p->run_detection = false;        
        p->detector_type = BLOB_MSER;
        p->tracker_type = CSRT; //tracker_none;
        p->track_scale = 3;
        p->limit_lx = 5; // only use for mser detect
        p->limit_ly = 5; // only use for mser detect
        p->limit_bx = 630; // only use for mser detect
        p->limit_by = 350; // only use for mser detect
        p->roi_w = 60;
        p->roi_h = 90;
        p->roi_w_default = 60;
        p->roi_h_default = 90;

        p->swipe_threshold = 15;
        p->area_threshold = 200;
        p->iou_threshold = 0.3;
        p->center_threshold  = 60;

        p->smoothing_radius = 20;
        p->run_kalman = false;
        p->run_kalman_pre = false;
        p->run_kalman_post = false;        
    }

    if(p->colored == false) {
        p->colored = false;
        tck = new GrayTracking();    
        tck->SetInitialData(p);
    } else if(p->colored == true) {
        p->colored = true;
        tck = new ColoredTracking();    
        tck->SetInitialData(p);
    }

    p->blur_size = 5;
    p->blur_sigma = 0.7;
    p->dst_width = 1920;
    p->dst_height = 1080;
    p->drop_threshold = p->dst_width;
    
    if(p->run_detection == true) {
#if defined _MAC_
        if(p->detector_type == DARKNET_YOLOV4) {
            dt = Detection();
            p->detect_threshold = 0.5;
            p->detector_type = DARKNET_YOLOV4;
            p->names_file = "darknet/data/coco.names";
            p->cfg_file = "darknet/cfg/yolov4-tiny.cfg";
            p->weights_file = "darknet/weights/yolov4-tiny.weights";
            p->id_filter.push_back(0); //id based on coco names            
            obj_trajectory.open("analysis/detected_obj.txt");
            obj_c_trajectory.open("analysis/detected_obj_center.txt");        
            dt.LoadModel(p);            
        }
#endif
    }

    k = new KALMAN();   

    if(p->interpolation_mode == KALMAN_FILTER) {
        k->Q.set(k->pstd, k->pstd, k->pstd);
        k->R.set(k->cstd, k->cstd, k->cstd);      
    }

    string prev_cur = "analysis/prev_to_cur_transformation.txt";
    string cur_tra = "analysis/trajectory.txt";
    string new_tra = "analysis/smoothed_trajectory.txt";
    string new_delta = "analysis/new_prev_to_cur_transformation.txt";
    out_transform.open(prev_cur);
    out_trajectory.open(cur_tra);
    out_smoothed.open(new_tra);
    out_new.open(new_delta);
    an.out_transform = &out_transform;
    an.out_transform = &out_trajectory;
    an.out_smoothed = &out_smoothed;
    an.out_new = &out_new;

    smth.create(2 , 3 , CV_64F);       
    smth.at<double>(0,0) = 1; 
    smth.at<double>(0,1) = 0; 
    smth.at<double>(1,0) = 0; 
    smth.at<double>(1,1) = 1;      
    p->read_wait = 1000; //msec

    CMd_INFO("Initialized compelete.");    
}

Dove::~Dove() {
    delete k;
    delete p;
    delete t;
    CMd_INFO("delete instance  OK");
}

int Dove::Process() {

#if defined GPU
    cuda::GpuMat src1ocg; 
    cuda::GpuMat src1og;

#ifdef WIN_TRANS
    // add win gpu
    std::string srcName = _in;
    std::string outName = _out;
    FrTrans trans;
    int ret = trans.OpenReader(srcName);
    if (ret < 0) {
        CMd_ERROR("trans.openreader fail..ret:{}, name:{}", ret, srcName);
        return EXECUTE_CLIENT_EXCEPTION;
    }
#else
    cv::Ptr<cudacodec::VideoReader> in = cudacodec::createVideoReader(_in);
#endif

#else
//    VideoCapture in(_in);
    VideoCapture in(_in);
#endif
    Mat src1oc; Mat src1o;
    int frame_index = 0;
    int swipe_index = 0;
    int result = 0;
    int found = 0;
    bool final = false;
    TRACK_OBJ* pre_obj = new TRACK_OBJ();;

    Configurator::TIMER* tm = new Configurator::TIMER();    
    Configurator::Get().StartTimer(tm);

    CMd_DEBUG("si {} {}  ",si[swipe_index].start, si[swipe_index].end);     
    int t_frame_start = si[swipe_index].start;
    int t_frame_end = si[swipe_index].end;
    int retry_cnt = 0;
    while(true) {
#if defined GPU
#ifdef WIN_TRANS
         int ret = trans.ReadFrame(src1ocg);
         if (ret < 0) {
             CMd_DEBUG("trans.readframe ret {} break.. ", ret);
             break;
         }
         else if (!ret) {
             CMd_DEBUG("trans.readframe ret {} continue.. ", ret);
             std::this_thread::sleep_for(std::chrono::milliseconds(100));
             retry_cnt++;
             if (p->read_wait <= retry_cnt * 100)
                 return STABIL_CANT_DECODE_FILE;
             continue;
         }             
         //else {
         //    //cv::Mat img;
         //    //src1ocg.download(img);

         //    ImageProcess(src1ocg, src1og);
         //}
#else
        if (!in->nextFrame(src1ocg))
            break;
#endif
#else 
        in >> src1oc;
        if(src1oc.data == NULL)
            break;
#endif
        CMd_INFO("Frameindex [{}] ", frame_index);
        if ( frame_index == 0)
        {                     
#if defined GPU
            CMd_DEBUG("ImageProcess() before.. ");
            cv::Mat img;
            ImageProcess(src1ocg, src1og);
            cv::Mat timg;
            src1og.download(timg);
            cv::imwrite("dump/frame0_proces.png", timg);
            CMd_DEBUG("ImageProcess() done..size :{}, {} ", src1og.cols, src1og.rows);
            tck->SetBg(src1og, frame_index);
            CMd_DEBUG("tck->SetBg() frame_index {}.. ", frame_index);
#else
            ImageProcess(src1oc, src1o);
            tck->SetBg(src1o, frame_index);
#endif
            if (frame_index != t_frame_start) {
                FRAME_INFO one(frame_index);
                all.push_back(one);
                frame_index++;
                continue;
            }
        }
                
        if(frame_index < t_frame_start || frame_index > t_frame_end || final == true) {
            FRAME_INFO one(frame_index);
            all.push_back(one);            
            frame_index++;
            continue;            
        }

#if defined GPU
        ImageProcess(src1ocg, src1og);
#else
        ImageProcess(src1oc, src1o);
#endif

        if (frame_index == t_frame_start) {
                pre_obj->clear();            
#if defined GPU
            if (p->roi_input) {
                p->roi_sx = si[swipe_index].target_x;
                p->roi_sy = si[swipe_index].target_y;  
                p->roi_w = p->roi_w_default;
                p->roi_h = p->roi_h_default;
                if(si[swipe_index].zoom != 100) {
                    p->roi_w = (p->roi_w_default) + ((si[swipe_index].zoom - 100) /4);
                    p->roi_h = (p->roi_h_default) + ((si[swipe_index].zoom - 100) /4);
                    CMd_DEBUG("zooming. roi : {} {}", p->roi_w, p->roi_h);
                } 
                tck->TrackerInitFx(src1og, frame_index, p->roi_sx, p->roi_sy, obj, roi);
            }
            else {
                result = tck->TrackerInit(src1og, frame_index, obj, roi);
                if(result != ERR_NONE)
                    return STABIL_CANT_GRAP_TRACKINGPT;
            }
#else
            if(p->roi_input) {
                p->roi_sx = si[swipe_index].target_x;
                p->roi_sy = si[swipe_index].target_y;
                p->roi_w = p->roi_w_default;
                p->roi_h = p->roi_h_default;
                if(si[swipe_index].zoom != 100) {                
                    p->roi_w = (p->roi_w_default) + ((si[swipe_index].zoom - 100) /4);
                    p->roi_h = (p->roi_h_default) + ((si[swipe_index].zoom - 100) /4);
                    CMd_DEBUG("zooming. roi : {} {}", p->roi_w, p->roi_h);                    
                }
                tck->TrackerInitFx(src1o, frame_index, p->roi_sx, p->roi_sy, obj, roi);
            }
            else {
                result = tck->TrackerInit(src1o, frame_index, obj, roi);
                if(result != ERR_NONE)
                    return STABIL_CANT_GRAP_TRACKINGPT;
            }
#endif
            FRAME_INFO one(frame_index);
            all.push_back(one);
        } 
        else {
#if defined GPU            
            tck->TrackerUpdate(src1og, frame_index, obj, roi);
#else
            tck->TrackerUpdate(src1o, frame_index, obj, roi);     
#endif            
        }

        //tck->DrawObjectTracking(src1o, obj, roi, false);
        double dx = 0;
        double dy = 0;
        double da = 0;
        if (frame_index > t_frame_start && frame_index <= t_frame_end) {

            dx = (pre_obj->cx - obj->cx) * p->track_scale;
            dy = (pre_obj->cy - obj->cy) * p->track_scale;
            if(abs(dx) > p->drop_threshold || abs(dy) > p->drop_threshold) {
                CMd_DEBUG("remove frame occurred  {} {} {} ", dx, dy, p->drop_threshold);
                FRAME_INFO one(frame_index, true, swipe_index);
                all.push_back(one);               
            } else {
                CMd_DEBUG("pre origin {} {} ", dx, dy);
                FRAME_INFO one(frame_index, swipe_index, dx, dy);
                all.push_back(one);               
            }
            if(frame_index == t_frame_end) {
                swipe_index++;
                if(swipe_index == si.size())
                    final = true;
                else {
                    t_frame_start = si[swipe_index].start;
                    t_frame_end = si[swipe_index].end;
#if defined GPU
                    tck->SetBg(src1og, frame_index);
#else
                    tck->SetBg(src1o, frame_index);
#endif
                }
            }
        }
        if(final)
            break;

        if (tck->isfound)
            obj->copy(pre_obj);        
        frame_index++;            
    }

    // add win gpu
#if defined GPU
#ifdef WIN_TRANS
    CMd_DEBUG("trans.CloseReader() start.. ");
    trans.CloseReader();
    CMd_DEBUG("trans.CloseReader() finish.. ");
#endif
#endif

    //dl.Logger("[%d] Image Analysis  %f ", i, LapTimer(all)); 
    Rect mg;     
    result = MakeNewTrajectory(&mg);
    if(result != ERR_NONE)
        return result;

    int last_frame_index = frame_index;
    frame_index = 0;
    swipe_index = 0;
#if defined GPU
    
#ifdef WIN_TRANS
    // add win gpu
    trans.OpenReader(srcName);
    FrTrans::WRITER_CONTEXT_T wctx;
    wctx.path = outName;
    wctx.fps = 30;
    wctx.width = p->dst_width;
    wctx.height = p->dst_height;
    wctx.gop = 1;
    CMd_DEBUG("trans.OpenWriter start.. path:{}, fps:{}, w:{}, h:{}", wctx.path, wctx.fps, wctx.width, wctx.height);
    trans.OpenWriter(wctx);
    CMd_DEBUG("trans.OpenWriter finish.. ");
#else
    Ptr<cudacodec::VideoReader> in2 = cudacodec::createVideoReader(_in);
    cv::VideoWriter out;
    out.open(_out, VideoWriter::fourcc('A', 'V', 'C', '1'), 30, Size(p->dst_width, p->dst_height));
#endif

#else
    VideoCapture in2(_in);
    cv::VideoWriter out;  
    out.open(_out, VideoWriter::fourcc('A', 'V', 'C', '1'), 30, Size(p->dst_width, p->dst_height));
#endif

    while(true) {
#if defined GPU
        
#ifdef WIN_TRANS
        // add win gpu
        int ret = trans.ReadFrame(src1ocg);
        if (ret < 0)
            break;
        else if (!ret)
            continue;
        //else {
        //    //cv::Mat img;
        //    //src1ocg.download(img);

        //    ImageProcess(src1ocg, src1og);
        //}
#else
        if (!in2->nextFrame(src1ocg))
             break;
#endif

        //src1ocg.upload(src1oc);
        //ImageProcess(src1ocg, src1og);
        if (src1ocg.cols > p->dst_width)
            cuda::resize(src1ocg, src1ocg, Size(p->dst_width, p->dst_height));

#else 
        in2 >> src1oc;
        if(src1oc.data == NULL)
            break;
        if (src1oc.cols > p->dst_width)
            cv::resize(src1oc, src1oc, Size(p->dst_width, p->dst_height));
#endif

        if (frame_index == 0)
        {
#if defined GPU
            SetRefCG(src1ocg);
#else
            SetRefC(src1oc);
#endif
            frame_index++;
            continue;
        }

#if defined GPU
        cuda::GpuMat canvas = cuda::GpuMat(cv::Size(p->dst_width, p->dst_height), CV_8UC3);
#else
        Mat canvas;
        canvas = Mat::zeros(p->dst_height, p->dst_width, CV_8UC3);
#endif
        bool skp = false;
        if (all[frame_index].onswipe == true && frame_index < last_frame_index) {
            if (all[frame_index].remove == true) {
                CMd_WARN("frame drop apply . {}", frame_index);
                skp = true;
            } else {
                double dx = -all[frame_index].new_dx;
                double dy = -all[frame_index].new_dy;

                if(p->run_kalman_post) {
                    double new_dx = 0;
                    double new_dy = 0;
                    al.KalmanInOutput(k, &an, dx, dy, frame_index, &new_dx, &new_dy);
                    CMd_DEBUG("post from kalman {} {} ", dx, dy);

                    smth.at<double>(0,2) = new_dx;
                    smth.at<double>(1,2) = new_dy;
                } else {
                    smth.at<double>(0,2) = dx;
                    smth.at<double>(1,2) = dy;
                    CMd_DEBUG(" {} will Apply {} {} ", frame_index, smth.at<double>(0,2), smth.at<double>(1,2));
                }
                ApplyImageRef();
            }
        }
        else {
#if defined GPU
            refcg.copyTo(refcwg);
#else
            refc.copyTo(refcw);
#endif
            CMd_DEBUG("Frame {}", frame_index);
        }

        if(skp) {
            frame_index++;
            continue;
        }

#if defined GPU
        refcwg.copyTo(canvas);
        canvas = refcwg(mg);
        cuda::resize(canvas, canvas, Size(p->dst_width, p->dst_height));
#else
        canvas = refcw(mg);
        cv::resize(canvas, canvas, Size(p->dst_width, p->dst_height));
#endif


#if defined GPU
#ifdef WIN_TRANS
        // add win gpu
        trans.WriteFrame(canvas);
#else
        //out->write(canvas);
        //SetRefCG(src1ocg);
        Mat canvas_t;
        canvas.download(canvas_t);
        //sprintf(filename, "%d_canvas_t.png", i);
        //imwrite(filename, canvas_t);
        
        out << canvas_t;
#endif
        SetRefCG(src1ocg);
#else
        out << canvas;        
        SetRefC(src1oc);
#endif
        frame_index++;
    }

    CMd_DEBUG("Spend time :  {}", Configurator::Get().LapTimer(tm));
#ifdef GPU
#ifdef WIN_TRANS
    trans.CloseReader();
    trans.CloseWriter();
#else
    out.release();
#endif
#endif

    return STABIL_COMPLETE;
}

int Dove::CalculcateMargin(double minx, double maxx, double miny, double maxy, Rect* mg) {
    // int mintop = abs(miny);
    // int minleft = abs(minx);
    int mx = max( abs(minx), maxx);
    int my = max( abs(miny), maxy);
    int mintop = my;
    int minleft = mx;
    int minright = p->dst_width - mx;
    int minbottom = p->dst_height - my;
    CMd_DEBUG("top {} left {} right {} bottom {}", mintop, minleft, minright, minbottom);

    if (minleft > mintop * p->dst_width / p->dst_height)
        mintop = minleft * p->dst_height / p->dst_width;
    else
        minleft = mintop * p->dst_width / p->dst_height;
    
    if (minright < minbottom * p->dst_width / p->dst_height)
        minbottom = minright * p->dst_height / p->dst_width;
    else
        minright = minbottom * p->dst_width / p->dst_height;

    int temp = 0;
    if(minright < minleft) {
        temp = minleft;
        minleft = minright;
        minright = temp;
    }
    if(minbottom < mintop){
        temp = mintop;
        mintop = minbottom;
        minbottom = temp;
    }
    
    mg->x = minleft;
    mg->y = mintop;
    mg->width = minright - minleft;
    mg->height = minbottom - mintop;

    CMd_DEBUG("Rect Margin {} {} {} {}", mg->x, mg->y, mg->width, mg->height);
    if(mg->x >= 0 && mg->y >= 0 && mg->width >= 960 && mg->height >= 540)
        return ERR_NONE;
    else 
        return STABIL_CANT_MAKE_PROPER_VIDEO;
}

#if defined GPU
int Dove::ImageProcess(cuda::GpuMat& src, cuda::GpuMat& dst) {
#else
int Dove::ImageProcess(Mat& src, Mat& dst) {
#endif
#if defined GPU
    //CMd_INFO("Dove::ImageProcess function start {} {}, scale: {}, colored: {} ", src.cols, src.rows, p->scale, p->colored);
    cuda::GpuMat temp;
    if (p->scale != 1) {
        cuda::resize(src, temp, Size(int((float)src.cols / p->scale), int(float(src.rows) / p->scale)), 0, 0, 1);
    }
    else
        src.copyTo(temp);

    if (!p->colored) {
        cuda::GpuMat sub = cuda::GpuMat(Size(temp.cols, temp.rows), CV_8UC1, Scalar(19));
#ifdef WIN_TRANS
        cuda::cvtColor(temp, temp, cv::COLOR_BGR2GRAY);
#else
        cuda::cvtColor(temp, temp, cv::COLOR_BGRA2GRAY);
#endif
        cuda::subtract(temp, sub, dst);
    }
    else {
#ifdef WIN_TRANS
        cuda::cvtColor(temp, temp, cv::COLOR_RGB2BGR);
#endif
        //cuda::cvtColor(temp, temp, cv::COLOR_BGRA2RGBA);
        temp.copyTo(dst);
    }

    //CMd_INFO("Dove::ImageProcess function end {} {}, scale: {}, colored: {} ", src.cols, src.rows, p->scale, p->colored);
#else
    Mat temp;
    if(p->scale != 1)
        cv::resize(src, temp, Size(int((float)src.cols/p->scale), int(float(src.rows)/p->scale)), 0,0,1);
    else 
        src.copyTo(temp);

    if(!p->colored)
        cv::cvtColor(temp, temp, COLOR_BGR2GRAY);
    //if tk on? 
    if(!p->run_tracking)
        cv::GaussianBlur(temp, dst, {p->blur_size, p->blur_size}, p->blur_sigma, p->blur_sigma);
    else
        temp.copyTo(dst);

    if (p->has_mask)
        MakeMask();

#endif    
    return ERR_NONE;
}

int Dove::MakeNewTrajectory(Rect* mg) {
    CMd_INFO("PostPrcess start ... all frame {} ", all.size());
    double a = 0;
    double x = 0;
    double y = 0;
    double minx = p->dst_width;
    double maxx = 0;
    double miny = p->dst_height;
    double maxy = 0;    
    int sindex = 0;
    for(int index = 0 ; index < si.size(); index ++) {
        vector<TransformParam>cur_delta;        
        vector<Trajectory>cur_traj;
        vector<Trajectory>smoothed_traj;
        vector<TransformParam>new_delta;

        a = 0;
        x = 0;
        y = 0;
        for(size_t i = si[index].start+1 ; i < si[index].end; i ++) {
            //findex = i - si[index].start;
            cur_delta.push_back(TransformParam(all[i].dx, all[i].dy, 0));
            x += all[i].dx;
            y += all[i].dy;
            a += 0;

            cur_traj.push_back(Trajectory(x, y, a));
            out_transform << i << " " << all[i].dx << " " << all[i].dy << " 0" << endl;
            out_trajectory << i << " " << x << " " << y << " " << a << endl;
        }

        if(p->interpolation_mode == SPLINE_LSF) {
            vector<dove::Trajectory> sp_xout;
            vector<dove::Trajectory> sp_yout;    
            al.BSplineTrajectory(cur_traj, &sp_xout, 0);
            al.BSplineTrajectory(cur_traj, &sp_yout, 1);    

            for(size_t i = 0, j = si[index].start +1; i < cur_traj.size(); i++, j++) {
                //CMd_DEBUG("spline output {} {} ", sp_xout[i].y, sp_yout[i].y);
                smoothed_traj.push_back(dove::Trajectory(sp_xout[i].y, sp_yout[i].y, 0));
                out_smoothed << j << " " << sp_xout[i].y << " " << sp_yout[i].y << " " << "0" << endl;
            }
        }
        else if (p->interpolation_mode == MEDIAN_KERNEL) {
            al.MedianKernel(&an, cur_traj, &smoothed_traj, p->smoothing_radius);
        }

        a = 0;
        x = 0;
        y = 0;

        for(size_t i = 0, j = si[index].start +1; j < si[index].end; i++, j++) {
            x += cur_delta[i].dx;
            y += cur_delta[i].dy;
            a += cur_delta[i].da;

            // target - current
            double diff_x = smoothed_traj[i].x - x;
            double diff_y = smoothed_traj[i].y - y;
            double diff_a = smoothed_traj[i].a - a;

            double dx = cur_delta[i].dx + diff_x;
            double dy = cur_delta[i].dy + diff_y;
            double da = cur_delta[i].da + diff_a;

            new_delta.push_back(TransformParam(dx, dy, da));
            out_new << j << " " << dx << " " << dy << " " << da << endl;
            
            all[j].new_dx = dx;
            all[j].new_dy = dy; 

            if(dx < minx)
                minx = dx;
            if(dx > maxx)
                maxx = dx;
            if(dy < miny)
                miny = dy;
            if (dy > maxy)
                maxy = dy;
        }

        cur_delta.clear();
        cur_traj.clear();
        smoothed_traj.clear();
        new_delta.clear();
    }
    
    CMd_DEBUG("minx {} maxx {} miny {} maxy {}", minx, maxx, miny, maxy);
    int result = CalculcateMargin(minx, maxx, miny, maxy, mg);
    return result;
}

void Dove::ApplyImage(Mat& src, bool scaled) {
    if( smth.at<double>(0,2) == 0.0 && smth.at<double>(1,2) == 0.0) {
        CMd_WARN("no warp");
        return;
    }

    if(scaled == true) {
        smth.at<double>(0,2) = smth.at<double>(0,2) * p->scale;
        smth.at<double>(1,2) = smth.at<double>(1,2) * p->scale;      
    }

    CMd_DEBUG("apply image {} {} ", smth.at<double>(0,2), smth.at<double>(1,2));
    cv::warpAffine(src, src, smth, src.size());
    // if(scaled == true) {
    //      static int i = 1;
    //      sprintf(filename, "saved/%d_apply.png", i);
    //      imwrite(filename, src);
    //      i++;
    // }
}

void Dove::ApplyImageRef() {
#if defined GPU
    cuda::warpAffine(refcg, refcwg, smth, refcg.size());
#else
    cv::warpAffine(refc, refcw, smth, refc.size());
#endif
}

int Dove::MakeMask() {
    mask = Mat::zeros(p->dst_height, p->dst_width, CV_8UC1);
    mask.setTo(Scalar(255));
    rectangle(mask, Point(p->sx, p->sy), Point(p->sx + p->width, p->sy + p->height), Scalar(0), -1);
    imwrite("dump/mask.png", mask);

    return ERR_NONE;
}

