function [costEC] = costencryptioncard(ECbr)
%UNTITLED4 �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
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