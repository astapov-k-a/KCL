TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

SOURCES += main.cpp \
    KC_library/KC_c_fun.c \
    KC_library/KC_cpp_fun.cpp
INCLUDEPATH += KC_Library

DEFINES += KCL_NO_KERNEL


MOC_DIR = build/.moc
OBJECTS_DIR = build/.obj
RCC_DIR = build/.rcc
UI_DIR = build/.ui

unix {
  TARGET = bin/KCLib_test
}
windows {
  TARGET = ../bin/KCLib_test
}

 #CONFIG(debug, debug|release) {
CONFIG(debug) {
  DEFINES += KC_LIBRARY_PROTECT_OFF #Защита заголовочных файлов, применённая в заголовочниках библиотеки KC_Lib, путает qtcreator, и он считает, что текст не компилится, что отключает полезные опции редактирования. Для обхода этого применён этот флаг
}

HEADERS += \
    KC_library/KC_library.h \
    KC_library/KC_library_declarations.h \
    KC_library/KC_bitfield.h \
    KC_library/KC_debug.h \
    KC_library/KC_list.h \
    KC_library/KC_memory.h \
    KC_library/KC_new.h \
    KC_library/KC_pod_array.h \
    KC_library/KC_pod_array_internal.h \
    KC_library/KC_pointers.h \
    KC_library/KC_random.h \
    KC_library/KC_string.h \
    KC_library/KC_types.h \
    KC_library/KC_vector.h
