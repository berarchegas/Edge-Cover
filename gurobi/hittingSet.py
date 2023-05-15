import gurobipy as gp

# Cria o modelo
model = gp.Model("Hitting Set")

# Recebe input
n, m = map(int, input().split())

# Cria as variaveis
variables = list()
for i in range(n):
    variables.append(model.addVar(vtype=gp.GRB.BINARY))

# Define a funcao objetivo
model.setObjective(
    gp.quicksum(variables[i] for i in range(n)), sense=gp.GRB.MINIMIZE)

# Recebe hiperarestas e insere as restricoes
edges = list()
for i in range(m):
    sz = int(input())
    edges.append(list(map(int, input().split())))
    vertices = list()
    for j in range(sz):
        vertices.append(variables[edges[i][j]])
    model.addConstr(
        gp.quicksum(vertices[j] for j in range(len(vertices))) >= 1)

# Resolve modelo
model.optimize()

# Imprime quais vertices foram utilizados
for i in range(n):
    if variables[i].X == 1:
        print("Vertice", i, "foi utilizado!")