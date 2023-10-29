import os
import subprocess

# Define the main folder containing subfolders with graph files
main_folder = os.path.join(os.path.dirname(__file__), 'teste', 'graphs')

# Define the path to your C++ executable
cpp_executable = os.path.join(os.path.dirname(__file__), 'geradores', 'randomGrowthPaths.exe')

k = input()
p = input()

# Function to generate paths of size 3 and save them to an output file
def generate_paths(input_graph_file, output_path, file_name):
    output_file = os.path.join(output_path, file_name)
    subprocess.run(f'{cpp_executable} < {input_graph_file} > {output_file}', shell=True)

# Walk through the main folder and its subfolders
for root, dirs, files in os.walk(main_folder):
    for file_name in files:
        if file_name.endswith('.txt'):
            input_graph_path = os.path.join(root, file_name)

            # Create the output directory structure
            relative_path = os.path.relpath(root, main_folder)
            output_directory = os.path.join(os.path.dirname(__file__), 'geromel', relative_path)
            # print(output_directory)

            if not os.path.exists(output_directory):
                os.makedirs(output_directory)

            # Process the graph file and generate paths
            generate_paths(input_graph_path, output_directory, file_name[:-4] + f"_{k}_{p}.txt")

print("Processing complete.")