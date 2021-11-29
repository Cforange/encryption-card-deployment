function [F] = ECchoose(ECbr,EC_inf,EC_use,numEC)
for i = 1:numEC
    if EC_inf(i) == ECbr && EC_use(i) == 1
        EC_use(i) = 0;% π”√EC[i]
        F = i;
        break;
    end
end