solve.Lbitrate = [40,40,100,100,400,400];
solve.Ebitrate = [40,40,100,100,400,400];
solve.Lt.bitrate = [40,100,400];
solve.Lt.cost = [1,1,2,2,4,4];
solve.Lt.ul = [2,2,2];
solve.Et.bitrate = [40,100,400];
solve.Et.cost = [2,2,4,4,8,8];
solve.Et.ue = [2,2,2];

filename=['solve','.mat'];
save(filename,'solve');