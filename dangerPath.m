function [d] = dangerPath(M,n,dl,row)
%dl : d x 2 matrix
%n : n nodes
for u = 1:n
    for v = 1:n
        [shortestPaths,totalCosts] = kShortestPath(M, u, v, 2);
        if(~isempty(shortestPaths))
            for k = 1:2
                d(k,u,v) = 0;
                a = cell2mat(shortestPaths(k));
                len = length(a);
                for i = 1:row
                    for t = 1:(len-1)
                        if a(t) == dl(i,1) && a(t+1) == dl(i,2)
                            d(k,u,v) = 1;
                        end
                    end
                end
            end
        end
    end
end