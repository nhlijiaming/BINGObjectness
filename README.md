# BING Objectness

BING Objectness proposal estimator Matlab (mex-c) wrapper, runs at 80 FPS at an i5-3210M CPU (2.5GHz) with Windows 64-bit and Matlab R2014b. Compared with the original version written in C++ provided by Mingming Cheng, runs at 150FPS, it might needs to be optimized.

## Introduction

This is the matlab wrapper of BING Objectness for efficient objectness proposal estimator following the CVPR 2014 paper BING, please consider to cite and refer to this paper.

@inproceedings{BingObj2014,
  title={{BING}: Binarized Normed Gradients for Objectness Estimation at
300fps},
  author={Ming-Ming Cheng and Ziming Zhang and Wen-Yan Lin and Philip H.
S. Torr},
  booktitle={IEEE CVPR},
  year={2014},
}

The original author Ming-Ming Cheng has already released the source code for windows 64-bit platform, and Tianfei Zhou has provided the MATLAB code for the linux/mac users.

I modified the compile.m in order to fit the VS2012 compiler so that windows users in MATLAB can easily reproduce the results in the paper or use BING for some other applications.

## Requirements

In order to make the code running, you need to download the
images/annotations PASCAL VOC 2007 data from the link:
[VOC2007](http://pascallin.ecs.soton.ac.uk/challenges/VOC/voc2007/#testdata).

Please refer to the FAQs #2 in
[http://mmcheng.net/bing/](http://mmcheng.net/bing/) for more details
about how to prepare for the VOC2007 dataset.

## How To Use

* compile.m - For re-compile the files.

* trainBING.cpp - For training BING Objectness.
* BINGMultiple.cpp - For reproducing the results of the original paper.
* BINGSingle.cpp - For users to use BING on a single image.

Note that the function __trainBING__ is a bit needless because __BINGMultiple__ and __BINGSingle__ themselves will learn the models if they do not exists. 

You can run Example\_BINGMultiple.m to reproduce the results in the origin paper, and a script called __PerImgAll.m__ will be generated in your VOC2007 folder. 
You can use the script as well as __PlotsCVPR14.m__ to plot the Figure 3 in the paper.

I have tested the code in Windows 7 64-bit (8G Memory) and Matlab R2014b and OpenCV3.0, and it produces the same accuracy results as the original windows version, except that it runs at 80 FPS compared to 300 FPS reported in the paper, due to lower efficiency of the CPU in my laptop. 

Please contact me (removethisifyouarehuman-nhlijiaming@gmail.com) or create an issue if you have problems to run the codes. 

## Known Problem

If you want to re-compile it yourself, you would need to replace "ACCESS_MASK" in line 63 of mat.hpp in OpenCV to another name, such as "_ACCESS_MASK". Otherwise, "ACCESS_MASK" could lead to redefinition because it has been defined in windows.h. In Linux/mac, there is no such problem.

## Other Source Code Repos

* [http://mmcheng.net/bing/](http://mmcheng.net/bing/) -- The original
  code / FAQ / Paper by Ming-Ming Cheng.
* [https://github.com/bittnt/Objectness](https://github.com/bittnt/Objectness) -- The Linux version library by Shuai Zheng.
* [https://github.com/tfzhou/BINGObjectness](https://github.com/tfzhou/BINGObjectness) -- The MATLAB version, using GCC as the compiler, in Linux/mac by Tianfei Zhou.