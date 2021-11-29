function [ availablecard ] = Capacitydecision( minimumcard,num40,num100,num400)
if minimumcard==40
    if num40~=0
        availablecard=40;
    else
        if num100~=0  
            availablecard=100;
        else
            if num400~=0
                availablecard=400;
            else
                availablecard=inf;
            end
        end
    end
else
    if  minimumcard==100
        if num100~=0
            availablecard=100;
        else
            if num400~=0
                availablecard=400;
            else
                 availablecard=inf; 
            end
        end
    else
        if num400~=0
            availablecard=400;
        else
            availablecard=inf; 
        end
    end
end
end