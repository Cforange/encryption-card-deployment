function [G] = LCchoose(LCbr,LC_inf,LC_use,numLC)
for i = 1:numLC
    if LC_inf(i) == LCbr && LC_use(i) == 1
        LC_use(i) = 0;% π”√EC[i]
        G = i;
        break;
    end
end