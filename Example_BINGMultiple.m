%% Example to call BINGMultiple.mexa64

% clc; clear;

vocPath = 'D:\ML\BingObjectnessCVPR14\VOC2007\';
display('***********************')
tic
bboxes = BINGMultiple( vocPath, 2, 8, 2, 130 );
display('***********************')
toc
save data;