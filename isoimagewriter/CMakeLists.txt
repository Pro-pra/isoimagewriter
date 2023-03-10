set(GPGME_REQUIRED_VERSION "1.8.0")
find_package(Gpgmepp ${GPGME_REQUIRED_VERSION} CONFIG)
if (QT_MAJOR_VERSION STREQUAL "6")
    find_package(QGpgmeQt6 ${GPGME_REQUIRED_VERSION} CONFIG)
else()
    find_package(QGpgme ${GPGME_REQUIRED_VERSION} CONFIG)
endif()

#find_package(KF${KF_MAJOR_VERSION} REQUIRED COMPONENTS QGpgme)

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX ISOIMAGEWRITER VERSION_HEADER isoimagewriter_version.h)

set(isoimagewriter_SRCS
    usbdevicemonitor.h
    common.cpp
    mainapplication.cpp
    mainwindow.cpp
    imagewriter.cpp
    physicaldevice.cpp
    isoverifier.cpp
    fetchisojob.cpp
    isolineedit.cpp

    common.h
    mainapplication.h
    mainwindow.h
    imagewriter.h
    physicaldevice.h
    isoverifier.h
    fetchisojob.h
    isolineedit.h
    main.cpp
)

ecm_qt_declare_logging_category(isoimagewriter_SRCS
                                HEADER isoimagewriter_debug.h
                                IDENTIFIER ISOIMAGEWRITER_LOG
                                CATEGORY_NAME org.kde.isoimagewriter
                                DEFAULT_SEVERITY Debug)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    message("Compiling isoimagewriter for Windows")
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    set(isoimagewriter_SRCS
        ${isoimagewriter_SRCS}
        platform_win.cpp
        usbdevicemonitor_win.cpp
        externalprogressbar_win.cpp
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
    message("Compiling isoimagewriter for Linux")
    set(isoimagewriter_SRCS
        ${isoimagewriter_SRCS}
        platform_lin.cpp
        usbdevicemonitor_lin.cpp
        externalprogressbar_lin.cpp
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
    message("Compiling isoimagewriter for FreeBSD")
    set(isoimagewriter_SRCS
        ${isoimagewriter_SRCS}
        platform_lin.cpp
        usbdevicemonitor_lin.cpp
        externalprogressbar_lin.cpp
    )
else()
    message("Unsupported Platform " . ${CMAKE_SYSTEM_NAME})
endif()

add_executable(isoimagewriter ${isoimagewriter_SRCS})

if (ROSA_BRANDING)
    target_compile_definitions(isoimagewriter PRIVATE -DROSA_BRANDING="1")
endif (ROSA_BRANDING)
target_compile_definitions(isoimagewriter PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")

target_link_libraries(isoimagewriter
    Qt::Widgets
    Qt::Network
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KF${KF_MAJOR_VERSION}::IconThemes
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::Archive
    KF${KF_MAJOR_VERSION}::Solid
)
if(QGpgme_FOUND)
    add_definitions(-D_USE_GPG)
    if (QT_MAJOR_VERSION STREQUAL "6")
        target_link_libraries(isoimagewriter QGpgmeQt6)
    else()
        target_link_libraries(isoimagewriter QGpgme)
    endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  target_link_libraries(isoimagewriter KF${KF_MAJOR_VERSION}::Solid)
endif()

if(USE_KAUTH)
  target_link_libraries(isoimagewriter KF${KF_MAJOR_VERSION}::AuthCore)
endif()

install(TARGETS isoimagewriter ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    install(PROGRAMS org.kde.isoimagewriter.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
    install(FILES org.kde.isoimagewriter.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
endif()

# KAuth
if(USE_KAUTH)
    add_executable(isoimagewriter_helper common.cpp imagewriter_helper.cpp imagewriter.cpp physicaldevice.cpp)
    target_link_libraries(isoimagewriter_helper Qt::Widgets KF${KF_MAJOR_VERSION}::AuthCore KF${KF_MAJOR_VERSION}::I18n KF${KF_MAJOR_VERSION}::WidgetsAddons KF${KF_MAJOR_VERSION}::Archive)
    install(TARGETS isoimagewriter_helper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
    kauth_install_helper_files(isoimagewriter_helper org.kde.isoimagewriter root)
    kauth_install_actions(org.kde.isoimagewriter isoimagewriter.actions)
endif()
