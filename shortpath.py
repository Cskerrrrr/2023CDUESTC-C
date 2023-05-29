graph = {'A': ['B', 'G', 'F'],
         'B': ['A', 'F', 'C'],
         'C': ['B', 'F', 'E', 'D'],
         'D': ['C','E'],
         'E': ['G', 'F', 'C', 'D'],
         'F': ['A', 'B', 'C', 'E', 'G'],
         'G': ['A', 'F', 'E']}

code = ['A','B','C','D','E','F','G']

def findShortestPath(graph,start,end,path=[]):
    path = path +[start]
    if start == end:
        return path
    
    shortestPath = []
    for node in graph[start]:
        if node not in path:
            newpath = findShortestPath(graph,node,end,path)
            if newpath:
                if not shortestPath or len(newpath)<len(shortestPath):
                    shortestPath = newpath
    return shortestPath
t = 0      
if __name__ == '__main__':    
    
    for i in range(len(code)):
        for j in range(len(code)):
            if(code[i] != code[j]):
                t = t+1
                shortpath = findShortestPath(graph,code[i],code[j])
                print('最短路径：',shortpath)
    print(t)

