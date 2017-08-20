QT += widgets

FORMS += \
    manager_mainwindow.ui \
    dialog_setsort.ui \
    mainwindow_student.ui \
    mainwindow_teacher.ui \
    dialog_log.ui

SOURCES += \
    manager_mainwindow.cpp \
    main.cpp \
    object.cpp \
    lesson.cpp \
    dialog_setsort.cpp \
    mainwindow_student.cpp \
    mainwindow_teacher.cpp \
    dialog_log.cpp

HEADERS += \
    manager_mainwindow.h \
    object.h \
    vector.h \
    person.h \
    specific_person.h \
    lesson.h \
    vector_of.h \
    dialog_setsort.h \
    mainwindow_student.h \
    mainwindow_teacher.h \
    dialog_log.h

RESOURCES += \
    images.qrc

RC_ICONS = my.ico
