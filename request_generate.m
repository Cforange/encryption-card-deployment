clear all;
close all;
clc;
nodesnum = 6;
flow = 16;
important_flow = 6;

M.s = [];
M.s1 = [];
M.s2 = [];
M.d = [];
M.d1 = [];
M.d2 = [];
M.bitrate = [];

for jj = 1:flow
    src_id = 1;
    dest_id = 1;
    while src_id == dest_id
        temp_ids = randi(nodesnum,1,2);   %nodes_in_total 仍然代表网络中原有节点的个数
        src_id = temp_ids(1);
        dest_id = temp_ids(2);
    end
    request_bandwidth = randsrc(1,1,[10,25,40,50,75,100,125,150,175,200]);
    requests(jj).s = src_id;
    requests(jj).d = dest_id;
    requests(jj).bitrate = request_bandwidth;
    if jj <=important_flow 
        M.s = [M.s, requests(jj).s];
        M.s1 = [M.s1, requests(jj).s];
        M.d = [M.d, requests(jj).d];
        M.d1 = [M.d1, requests(jj).d];
        M.bitrate = [M.bitrate, requests(jj).bitrate];
    else
        M.s = [M.s, requests(jj).s];
        M.s2 = [M.s2, requests(jj).s];
        M.d = [M.d, requests(jj).d];
        M.d2 = [M.d2, requests(jj).d];
        M.bitrate = [M.bitrate, requests(jj).bitrate];
    end
end


A=sum( M.bitrate(:));
fprintf('带宽总和=%d\n',A ) ;
filename=['M','.mat'];
save(filename,'M');