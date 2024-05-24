cmake_minimum_required(VERSION 3.5)

project(Emp_payroll_sys VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)
find_package(SQLite3 REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        UIcomponents/dashboard/dashboardwidget.cpp
        UIcomponents/dashboard/dashboardwidget.h
        src/user_authentication/Login.h
        src/user_authentication/Login.cpp
        src/database/Database.h
        UIcomponents/dashboard/dashboardwidget.ui
        UIcomponents/department/departmentwidget.h
        UIcomponents/department/departmentwidget.cpp
        UIcomponents/department/departmentwidget.ui
        UIcomponents/manageEmployee/manageemployeewidget.h
        UIcomponents/manageEmployee/manageemployeewidget.cpp
        UIcomponents/manageEmployee/manageemployeewidget.ui
        UIcomponents/managePaygrade/managepaygrade.h
        UIcomponents/managePaygrade/managepaygrade.cpp
        UIcomponents/managePaygrade/managepaygrade.ui
        src/department/Department.h
        src/department/Department.cpp
        src/employee/Employee.h
        src/employee/Employee.cpp
        src/pay_grade/PayGrade.h
        src/pay_grade/PayGrade.cpp
        src/payroll/Payroll.cpp
        src/payroll/Payroll.h
        UIcomponents/generateReport/generatereport.h
        UIcomponents/generateReport/generatereport.cpp
        UIcomponents/generateReport/generatereport.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Emp_payroll_sys
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        UIcomponents/dashboard/dashboardwidget.h UIcomponents/dashboard/dashboardwidget.cpp UIcomponents/dashboard/dashboardwidget.ui
        UIcomponents/department/departmentwidget.h UIcomponents/department/departmentwidget.cpp UIcomponents/department/departmentwidget.ui
        UIcomponents/manageEmployee/manageemployeewidget.h UIcomponents/manageEmployee/manageemployeewidget.cpp UIcomponents/manageEmployee/manageemployeewidget.ui
        UIcomponents/managePaygrade/managepaygrade.h UIcomponents/managePaygrade/managepaygrade.cpp UIcomponents/managePaygrade/managepaygrade.ui
        UIcomponents/manageEmpPayroll/manageemppayroll.h UIcomponents/manageEmpPayroll/manageemppayroll.cpp UIcomponents/manageEmpPayroll/manageemppayroll.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Emp_payroll_sys APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Emp_payroll_sys SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Emp_payroll_sys
            ${PROJECT_SOURCES}
        )
    endif()
endif()


target_link_libraries(Emp_payroll_sys PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(Emp_payroll_sys PRIVATE ${SQLite3_LIBRARIES})  # Link SQLite3
target_include_directories(Emp_payroll_sys PRIVATE ${SQLite3_INCLUDE_DIRS})


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Emp_payroll_sys)
endif()
set_target_properties(Emp_payroll_sys PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Emp_payroll_sys
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Emp_payroll_sys)
endif()
