function [numE40,numE100,numE400] = ECnumupdate(numE40,numE100,numE400,ECbr)
%UNTITLED6 此处显示有关此函数的摘要
%   此处显示详细说明
if ECbr==40
    numE40=numE40-1;
else
    if ECbr==100
        numE100=numE100-1;
    else
        numE400=numE400-1;
    end
end
end