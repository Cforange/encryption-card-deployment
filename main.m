tic;
file_name = 'solve.mat';
instance_file = load(file_name);
filename = 'M.mat';
requests = load(filename);
n = 6;
k = 2;
link = [0 1 1 1 1 1;
        1 0 1 1 1 1;
        1 1 0 1 1 1;
        1 1 1 0 1 1;
        1 1 1 1 0 1;
        1 1 1 1 1 0];
dl=[1,4;
    3,6;
    4,1;
    6,3];
row = 4;
P = [0 1 inf 1 inf inf;
     1 0 1 1 inf inf;
     inf 1 0 inf 1 1;
     1 1 inf 0 1 inf;
     inf inf 1 1 0 1;
     inf inf 1 inf 1 0];
d = dangerPath(P,n,dl,row);

for u = 1:n
    for v = 1:n
        [shortestPaths,totalCosts] = kShortestPath(P, u, v, 2);
        for k = 1:2
            if(isempty(totalCosts))
                h(k,u,v) = 0;
            else
                h(k,u,v) = totalCosts(k);
            end
        end
    end
end

M_s = requests.M.s;
M_s1 = requests.M.s1;
%M_s2 = requests.M.s2;

f = size(M_s,2);
f1 = size(M_s1,2);
%f2 = size(M_s2,2) + size(M_s1,2);

M_d = requests.M.d;
M_bitrate = requests.M.bitrate;
M_BL = instance_file.solve.Lbitrate;
l = size(M_BL,2);

M_BE = instance_file.solve.Ebitrate;
e = size(M_BE,2);

M_BLt = instance_file.solve.Lt.bitrate;
lt = size(M_BLt,2);
M_BEt = instance_file.solve.Et.bitrate;
et = size(M_BEt,2);
M_CL = instance_file.solve.Lt.cost;
M_CE = instance_file.solve.Et.cost;

ul = instance_file.solve.Lt.ul;
ue = instance_file.solve.Et.ue;
        
solve(n,f1,f,l,lt,e,et,k,link,d,h,M_BL,M_BE,M_BLt,M_BEt,M_CL,M_CE,M_bitrate,M_s,M_d,ul,ue);
toc