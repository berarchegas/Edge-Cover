import os

# Define the main folder containing subfolders with .txt files
main_folder = os.path.dirname(__file__) + '/teste/graphs'

k = str(input())
p = str(input())

# Function to update a graph from 0-based to 1-based
def update_graph(graph):
    updated_graph = [k + '\n', p + '\n']
    for line in graph:
        updated_graph.append(line)
    return updated_graph

# Walk through the main folder and its subfolders
for root, dirs, files in os.walk(main_folder):
    for file_name in files:
        if file_name.endswith('.txt'):
            file_path = os.path.join(root, file_name)
            with open(file_path, 'r') as file:
                # Read the contents of the file
                lines = file.readlines()

            # Update the graph data in the file
            updated_graph_data = update_graph(lines)

            # Write the updated content back to the file
            with open(file_path, 'w') as file:
                for line in updated_graph_data:
                    file.write(line)

print("Conversion complete.")