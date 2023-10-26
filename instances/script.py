import os

# Define the main folder containing subfolders with .txt files
main_folder = os.path.dirname(__file__)

# Function to update a graph from 0-based to 1-based
def update_graph(graph):
    updated_graph = [graph[0]]
    for line in graph[1::]:
        parts = line.strip().split()
        if len(parts) != 1:
            updated_line = ' '.join(str(int(part) + 1) for part in parts) + '\n'
            updated_graph.append(updated_line)
        else:
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