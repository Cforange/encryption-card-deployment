function [costLC] = costlinecard(LCbr)
%UNTITLLD4 �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
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
