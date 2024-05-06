import os
import glob

def consolidate_files(directory, output_file):
    # Check for valid directory
    if not os.path.isdir(directory):
        print("The specified path is not a directory.")
        return
    
    # Create a list of all .cpp, .h, and .ui files in the directory and subdirectories
    file_paths = []
    for extension in ['*.cpp', '*.h', '*.ui']:
        file_paths.extend(glob.glob(os.path.join(directory, '**', extension), recursive=True))
    
    # Open the output file
    with open(output_file, 'w') as outfile:
        # Process each file
        for file_path in file_paths:
            with open(file_path, 'r') as file:
                # Write the complete file path at the top of its content
                outfile.write(f"File Path: {file_path}\n")
                # Write the content of the file
                outfile.write(file.read())
                # Add a separator between file contents
                outfile.write("\n\n")

        print(f"All files have been consolidated into {output_file}")

# Example usage
directory_path = '/Users/spartan/EmployeePayrollSystem/src'  # Replace with your directory path
output_filename = '/Users/spartan/EmployeePayrollSystem/utils/consolidated_files.txt'  # Name of the output file
consolidate_files(directory_path, output_filename)
