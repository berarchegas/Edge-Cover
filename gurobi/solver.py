import gurobipy as gp

# Cria o modelo
model = gp.Model("Path")

# Recebe input
n, m, p = map(int, input().split())

# Recebe arestas
edges = list()
for i in range(m):
    a, b = map(int, input().split())
    edges.append([a, b])

# Recebe caminhos, cria os caminhos de arestas e cria as variaveis 
paths = list()
variables = list()
for i in range(p):
    sz = int(input())
    paths.append(list(map(int, input().split())))
    variables.append(model.addVar(vtype=gp.GRB.BINARY))
for i in range(m):
    paths.append(edges[i])
    variables.append(model.addVar(vtype=gp.GRB.BINARY))
p += m

model.setObjective(
    gp.quicksum(variables[i] for i in range(p)), sense=gp.GRB.MINIMIZE)

# Insere as restricoes
for i in range(m):
    constraint = list()
    for j in range(p):
        contains = False
        for k in range(len(paths[j]) - 1):
            if (edges[i][0] == paths[j][k] and edges[i][1] == paths[j][k + 1]) or (edges[i][0] == paths[j][k + 1] and edges[i][1] == paths[j][k]):
                contains = True
        if contains:
            constraint.append(variables[j])
    model.addConstr(
        gp.quicksum(constraint[j] for j in range(len(constraint))) == 1)

# Resolve modelo
model.optimize()

# Imprime quais caminhos foram utilizados
# for i in range(p):
#     if variables[i].X == 1:
#         print("Caminho", paths[i], "foi utilizado!")
#     else: 
#         print("Caminho", i, "não foi utilizado!")