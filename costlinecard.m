function [costLC] = costlinecard(LCbr)
%UNTITLLD4 此处显示有关此函数的摘要
%   此处显示详细说明
if LCbr==40
    costLC=1;
else
    if LCbr==100
        costLC=2;
    else
        if LCbr==400
            costLC=4;
        else
            costLC=inf;
        end
    end
end
