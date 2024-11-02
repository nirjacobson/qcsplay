cmake_minimum_required(VERSION 3.5)

project(qcsplay VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LINUX_ARM OFF)
set(WIN32_ARM OFF)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    if (WIN32_ARM)
        include_directories("C:/Program Files (Arm)/portaudio/include"
                            "C:/Program Files (Arm)/zlib/include")
    else()
        include_directories("C:/Program Files/portaudio/include"
                            "C:/Program Files/zlib/include")
    endif()
elseif (APPLE)
    include_directories("/System/Volumes/Data/usr/local/include"
                        "/System/Volumes/Data/usr/local/include/libbson-1.0")
elseif (NOT LINUX_ARM)
    include_directories("/usr/x86_64-pc-linux-gnu/usr/lib/gcc/x86_64-pc-linux-gnu/14/include/g++-v14/")
endif()


set(CMAKE_OSX_ARCHITECTURES x86_64 arm64)

set(PROJECT_SOURCES
    chromasound/emu/2413tone.h
    chromasound/emu/2423tone.h
    chromasound/emu/281btone.h
    chromasound/emu/audio.h
    chromasound/emu/audio_output.h
    chromasound/emu/Ay_Apu.cc
    chromasound/emu/Ay_Apu.h
    chromasound/emu/blargg_common.h
    chromasound/emu/blargg_endian.h
    chromasound/emu/blargg_source.h
    chromasound/emu/Blip_Buffer.cc
    chromasound/emu/Blip_Buffer.h
    chromasound/emu/Classic_Emu.cc
    chromasound/emu/Classic_Emu.h
    chromasound/emu/consumer.h
    chromasound/emu/Data_Reader.cc
    chromasound/emu/Data_Reader.h
    chromasound/emu/Dual_Resampler.cc
    chromasound/emu/Dual_Resampler.h
    chromasound/emu/Effects_Buffer.cc
    chromasound/emu/Effects_Buffer.h
    chromasound/emu/Fir_Resampler.cc
    chromasound/emu/Fir_Resampler.h
    chromasound/emu/gme.cc
    chromasound/emu/gme.h
    chromasound/emu/Gme_File.cc
    chromasound/emu/Gme_File.h
    chromasound/emu/gme_types.h
    chromasound/emu/M3u_Playlist.cc
    chromasound/emu/M3u_Playlist.h
    chromasound/emu/Multi_Buffer.cc
    chromasound/emu/Multi_Buffer.h
    chromasound/emu/Music_Emu.cc
    chromasound/emu/Music_Emu.h
    chromasound/emu/producer.h
    chromasound/emu/Sms_Apu.cc
    chromasound/emu/Sms_Apu.h
    chromasound/emu/Sms_Oscs.h
    chromasound/emu/Vgm_Emu.cc
    chromasound/emu/Vgm_Emu.h
    chromasound/emu/Vgm_Emu_Impl.cc
    chromasound/emu/Vgm_Emu_Impl.h
    chromasound/emu/Ym2413_Emu.cc
    chromasound/emu/Ym2413_Emu.h
    chromasound/emu/Ym2612_Emu.cc
    chromasound/emu/Ym2612_Emu.h
    chromasound/chromasound.h
    chromasound/chromasound_dual.cpp
    chromasound/chromasound_dual.h
    chromasound/chromasound_emu.cpp
    chromasound/chromasound_emu.h
    chromasound/chromasound_standin.cpp
    chromasound/chromasound_standin.h
    dialogs/settings/emulationsettingswidget.cpp
    dialogs/settings/emulationsettingswidget.h
    dialogs/settings/emulationsettingswidget.ui
    dialogs/settings/emulatoroutputdevicesettingswidget.cpp
    dialogs/settings/emulatoroutputdevicesettingswidget.h
    dialogs/settings/emulatoroutputdevicesettingswidget.ui
    dialogs/settings/settingsdialog.cpp
    dialogs/settings/settingsdialog.h
    dialogs/settings/settingsdialog.ui
    player/player.cpp
    player/player.h
    player/player.ui
    player/playlistitem.cpp
    player/playlistitem.h
    player/playlisttablemodel.cpp
    player/playlisttablemodel.h
    formats/gd3.cpp
    formats/gd3.h
    application.cpp
    application.h
    main.cpp
    qcsplay.cpp
    qcsplay.h
    formats/vgm.cpp
    formats/vgm.h
    dialogs/settings/outputdevicesettingswidget.h
    dialogs/settings/outputdevicesettingswidget.cpp
    dialogs/settings/outputdevicesettingswidget.ui
)

if (LINUX)
    set (PROJECT_SOURCES
            ${PROJECT_SOURCES}
            chromasound/chromasound_direct.cpp
            chromasound/chromasound_direct.h
            chromasound/direct/vgmplayer.cpp
            chromasound/direct/vgmplayer.h
            chromasound/direct/gpio.h
            chromasound/direct/gpio.cpp
            chromasound/direct/spi.h
            chromasound/direct/spi.cpp
    )
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if (WIN32)
        set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/qcsplay.rc")
        qt_add_executable(qcsplay
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${app_icon_resource_windows}
        )
    elseif (LINUX)
        qt_add_executable(qcsplay
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
        )
    elseif (APPLE)
        set(MACOSX_BUNDLE_ICON_FILE qcsplay.icns)
        set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/qcsplay.icns")
        set_source_files_properties(${app_icon_macos} PROPERTIES
                   MACOSX_PACKAGE_LOCATION "Resources")
        qt_add_executable(qcsplay
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${app_icon_macos}
        )
    endif()
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qcsplay APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qcsplay SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qcsplay
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if (LINUX)
    if (LINUX_ARM)
        target_link_libraries(qcsplay PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                    /usr/lib64/libportaudio.so
                                                    /usr/lib64/libz.so)
    else()
        target_link_libraries(qcsplay PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                    /usr/x86_64-pc-linux-gnu/usr/lib64/libportaudio.so
                                                    /usr/x86_64-pc-linux-gnu/usr/lib64/libz.so)
    endif()
elseif (WIN32)
    if (WIN32_ARM)
        target_link_libraries(qcsplay PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                    "C:/Program Files (Arm)/portaudio/lib/portaudio_x64.lib"
                                                    "C:/Program Files (Arm)/zlib/lib/zlib.lib")
    else()
        target_link_libraries(qcsplay PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                    "C:/Program Files/portaudio/lib/portaudio_x64.lib"
                                                    "C:/Program Files/zlib/lib/zlib.lib")
    endif()
elseif (APPLE)
    target_link_libraries(qcsplay PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                                "/System/Volumes/Data/usr/local/lib/libportaudio.dylib"
                                                "/System/Volumes/Data/usr/local/lib/libz.dylib")
endif()


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.qcsplay)
endif()
set_target_properties(qcsplay PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS qcsplay
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qcsplay)
endif()
