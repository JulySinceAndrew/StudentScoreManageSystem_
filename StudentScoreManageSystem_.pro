QT += widgets

FORMS += \
    manager_mainwindow.ui \
    dialog_setsort.ui

SOURCES += \
    manager_mainwindow.cpp \
    main.cpp \
    object.cpp \
    lesson.cpp \
    dialog_setsort.cpp

HEADERS += \
    manager_mainwindow.h \
    object.h \
    vector.h \
    person.h \
    specific_person.h \
    lesson.h \
    vector_of.h \
    dialog_setsort.h

RESOURCES += \
    images.qrc \
    file.qrc

RC_ICONS = my.ico
