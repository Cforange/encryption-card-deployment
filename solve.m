% create a data file
function [] = solve(n,f1,f,l,lt,e,et,k,link,d,h,M_BL,M_BE,M_BLt,M_BEt,M_CL,M_CE,M_bitrate,M_s,M_d,ul,ue)
data_filename = 'ILP.dat';
fp = fopen(data_filename,'w');

fprintf(fp,'data;\n');

fprintf(fp,'set N := \n');
fprintf(fp,'%d \n',1:(n-1));
fprintf(fp,'%d;\n',n);

fprintf(fp,'set F := \n');
fprintf(fp,'%d \n',1:(f-1));
fprintf(fp,'%d;\n',f);

fprintf(fp,'set F1 := \n');
if f1 == 1
    fprintf(fp,'%d;\n',f1);
else
    fprintf(fp,'%d \n',1:(f1-1));
    fprintf(fp,'%d;\n',f1);
end

fprintf(fp,'set F2 := \n');
if f == (f1 + 1)
    fprintf(fp,'%d;\n',f);
else
    fprintf(fp,'%d \n',(f1 + 1):(f-1));
    fprintf(fp,'%d;\n',f);
end

fprintf(fp,'set L := \n');
fprintf(fp,'%d \n',1:(l-1));
fprintf(fp,'%d;\n',l);

fprintf(fp,'set Lt := \n');
if lt == 1
    fprintf(fp,'%d;\n',1:lt);
else
    fprintf(fp,'%d \n',1:(lt-1));
    fprintf(fp,'%d;\n',lt);
end

fprintf(fp,'set E := \n');
fprintf(fp,'%d \n',1:(e-1));
fprintf(fp,'%d;\n',e);

fprintf(fp,'set Et := \n');
if et == 1
    fprintf(fp,'%d;\n',et);
else
    fprintf(fp,'%d \n',1:(et-1));
    fprintf(fp,'%d;\n',et);
end

fprintf(fp,'set K := \n');
fprintf(fp,'%d \n',1:(k-1));
fprintf(fp,'%d;\n',k);

fprintf(fp,'param link := \n');
for i = 1 : n
    for j = 1 : n
        if i == n && j == n
            fprintf(fp,'%d  %d %d;\n', i, j, link(i, j));
        else
            fprintf(fp,'%d  %d %d\n', i, j, link(i, j));
        end
    end
end

fprintf(fp,'param d := \n');
for i = 1:n
    for j = 1:n
        for t = 1:k
            if i ==n && j == n && t == k
                fprintf(fp,'%d %d %d %d;\n',t, i, j, d(t,i,j));
            else
                fprintf(fp,'%d %d %d %d \n',t, i, j, d(t,i,j));
            end
        end
    end
end

fprintf(fp,'param h :=\n');
for i = 1:n
    for j = 1:n
        for t = 1:k
            if i ==n && j == n && t == k
                fprintf(fp,'%d %d %d %d;\n',t, i, j, h(t,i,j));
            else
                fprintf(fp,'%d %d %d %d \n',t, i, j, h(t,i,j));
            end
        end
    end
end

fprintf(fp,'param BL := \n');
for i = 1:l
    if i == l
        fprintf(fp,'%d %d;\n',i,M_BL(i));
    else
        fprintf(fp,'%d %d \n',i,M_BL(i));
    end
end

fprintf(fp,'param BE := \n');
for i = 1:e
    if i == e
        fprintf(fp,'%d %d;\n',i,M_BE(i));
    else
        fprintf(fp,'%d %d \n',i,M_BE(i));
    end
end

fprintf(fp,'param BLt := \n');
for i = 1:lt
    if i == lt
        fprintf(fp,'%d %d;\n',i,M_BLt(i));
    else
        fprintf(fp,'%d %d \n',i,M_BLt(i));
    end
end

fprintf(fp,'param BEt := \n');
for i = 1:et
    if i == et
        fprintf(fp,'%d %d;\n',i,M_BEt(i));
    else
        fprintf(fp,'%d %d \n',i,M_BEt(i));
    end
end

fprintf(fp,'param CLt := \n');
for i = 1:l
    if i == l
        fprintf(fp,'%d %d;\n',i,M_CL(i));
    else
        fprintf(fp,'%d %d \n',i,M_CL(i));
    end
end

fprintf(fp,'param CEt := \n');
for i = 1:e
    if i == e
        fprintf(fp,'%d %d;\n',i,M_CE(i));
    else
        fprintf(fp,'%d %d \n',i,M_CE(i));
    end
end

fprintf(fp,'param Bit := \n');
for i = 1:f
    if i == f
        fprintf(fp,'%d %d;\n',i,M_bitrate(i));
    else
        fprintf(fp,'%d %d \n',i,M_bitrate(i));
    end
end

fprintf(fp,'param Source := \n');
for i = 1:f
    if i == f
        fprintf(fp,'%d %d;\n',i,M_s(i));
    else
        fprintf(fp,'%d %d \n',i,M_s(i));
    end
end

fprintf(fp,'param Des := \n');
for i = 1:f
     if i == f
        fprintf(fp,'%d %d;\n',i,M_d(i));
    else
        fprintf(fp,'%d %d \n',i,M_d(i));
     end
end

fprintf(fp,'param UL := \n');
for i = 1:lt
    if i == lt
        fprintf(fp,'%d %d;\n',i,ul(i));
    else
        fprintf(fp,'%d %d \n',i,ul(i));
    end
end

fprintf(fp,'param UE := \n');
for i = 1:et
    if i == et
        fprintf(fp,'%d %d;\n',i,ue(i));
    else
        fprintf(fp,'%d %d \n',i,ue(i));
    end
end

%system('glpsol --model  ILP.mod --data ILP.dat --simplex --output ILP.sol --ranges text');  
system('glpsol --model  ILP.mod --data ILP.dat --output ILP.txt'); %

