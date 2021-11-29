function [costEC] = costencryptioncard(ECbr)
%UNTITLED4 此处显示有关此函数的摘要
%   此处显示详细说明
if ECbr==40
    costEC=4;
else
    if ECbr==100
        costEC=8;
    else
        if ECbr==400
            costEC=16;
        else
            costEC=inf;
        end
    end
end
end