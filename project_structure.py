import os

def create_directory(directory_path):
    if not os.path.exists(directory_path):
        os.makedirs(directory_path)

def create_file(file_path, content=""):
    with open(file_path, "w") as f:
        f.write(content)

def generate_backend_structure():
    backend_dirs = [
        "src/company",
        "src/department",
        "src/employee",
        "src/pay_grade",
        "src/payroll",
        "src/user_authentication",
        "include/company",
        "include/department",
        "include/employee",
        "include/pay_grade",
        "include/payroll",
        "include/user_authentication",
    ]

    for directory in backend_dirs:
        create_directory(directory)

    backend_files = [
        "src/company/Company.h",
        "src/company/Company.cpp",
        "src/department/Department.h",
        "src/department/Department.cpp",
        "src/employee/Employee.h",
        "src/employee/Employee.cpp",
        "src/pay_grade/PayGrade.h",
        "src/pay_grade/PayGrade.cpp",
        "src/payroll/Payroll.h",
        "src/payroll/Payroll.cpp",
        "src/user_authentication/User.h",
        "src/user_authentication/User.cpp",
        "src/user_authentication/Session.h",
        "src/user_authentication/Session.cpp",
        "include/company/Company.h",
        "include/department/Department.h",
        "include/employee/Employee.h",
        "include/pay_grade/PayGrade.h",
        "include/payroll/Payroll.h",
        "include/user_authentication/User.h",
        "include/user_authentication/Session.h",
        "CMakeLists.txt",
    ]

    for file in backend_files:
        create_file(file)

if __name__ == "__main__":
    generate_backend_structure()
    print("Backend structure created successfully.")
