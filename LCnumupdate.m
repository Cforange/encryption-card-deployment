function [numL40,numL100,numL400] = LCnumupdate(numL40,numL100,numL400,LCbr)
%UNTITLED6 �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
if LCbr==40
    numL40=numL40-1;
else
    if LCbr==100
        numL100=numL100-1;
    else
        numL400=numL400-1;
    end
end
end
