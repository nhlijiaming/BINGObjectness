clc; clear;

debug = 0; %  0 - release mode ; 1 - debug mode
x64 = 1;  % 0 - 32bit mode ; 1 - 64bit mode

%----------  Path Setting ----------%

include = ['-I"' cd '" ' '-I"E:\OpenCV3.0\build\include" '];
libpath = '-L"E:\OpenCV3.0\build\x64\vc11\staticlib" ';
dependencypath = 'E:\OpenCV3.0\build\x64\vc11\lib';

%-----------------------------------%

if (debug==1)
    debug_flag = '-g ';
else
    debug_flag = '';
end

if (x64==1)
    x64_flag = '-largeArrayDims ';
else
    x64_flag = '';
end

compileflags = 'COMPFLAGS="$COMPFLAGS /MT" ';

mexcommand = [debug_flag x64_flag compileflags '-O ']

files = dir([dependencypath '\*.lib']);
dependencies = ['-L"' dependencypath '" '];
for( i = 1 : length(files) )
    dependencies = [dependencies '-l' files(i).name ' '];
end

objPath    = 'Objectness/Src/';
linearPath = [objPath 'LibLinear/'];
blasPath   = [linearPath 'blas/'];

% Objectness
%eval( ['mex -v CXXFLAGS="\$CXXFLAGS -std=c++0x" -O -c ' mexcommand include objPath 'DataSetVOC.cpp ' objPath 'CmFile.cpp ' ...
%    objPath 'CmShow.cpp ' objPath 'FilterTIG.cpp ' objPath 'Objectness.cpp '] );
eval( ['mex -v CXXFLAGS="\$CXXFLAGS -std=c++0x" -O -c ' mexcommand include objPath 'DataSetVOC.cpp ' objPath 'Objectness.cpp'] );
eval( ['mex CXXFLAGS="\$CXXFLAGS -std=c++0x" -O -c ' mexcommand include objPath 'CmShow.cpp ' objPath 'FilterTIG.cpp ' objPath 'CmFile.cpp'] );

% LibLinear
eval( ['mex CXXFLAGS="\$CXXFLAGS -std=c++0x" -O -c ' mexcommand linearPath 'linear.cpp ' linearPath 'tron.cpp'  ] )

% BLAS
eval( ['mex CXXFLAGS="\$CXXFLAGS -std=c++0x" -O -c ' mexcommand blasPath 'daxpy.c ' blasPath 'ddot.c ' blasPath 'dnrm2.c ' blasPath 'dscal.c'])

% MxArray
eval(['mex -O -c ' compileflags include 'MxArray.cpp'])

objectfiles = dir(['*.obj']);
objects = [];
for( i = 1 : length(objectfiles) )
    objects = [ objects objectfiles(i).name ' '];
end 

display('Linking ...')

eval(['mex BINGSingle.cpp ' mexcommand include libpath dependencies objects 'CFLAGS="\$CFLAGS -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" ']);
eval(['mex trainBING.cpp ' mexcommand include libpath dependencies objects ' CFLAGS="\$CFLAGS -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" ']);
eval(['mex BINGMultiple.cpp ' mexcommand include libpath dependencies objects ' CFLAGS="\$CFLAGS -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" ']);

display('Done!')

%delete *.obj